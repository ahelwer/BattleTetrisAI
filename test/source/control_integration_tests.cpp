#include <test/control_integration_tests.hpp>

TestServerInterface::TestServerInterface(std::queue<State const*>& messages)
    : m_messages(messages), m_commandConnected(false), m_stateConnected(false)
{ 
    pthread_mutex_init(&m_messageMutex, NULL);
    pthread_mutex_init(&m_moveMutex, NULL);
    pthread_cond_init(&m_messagesAvailable, NULL);
    pthread_cond_init(&m_movesAvailable, NULL);
}

TestServerInterface::~TestServerInterface()
{ 
    pthread_mutex_destroy(&m_messageMutex);
    pthread_mutex_destroy(&m_moveMutex);
    pthread_cond_destroy(&m_messagesAvailable);
    pthread_cond_destroy(&m_movesAvailable);
    while (!m_messages.empty()) {
        State const* s = m_messages.front();
        m_messages.pop();
        if (s != NULL) {
            delete s;
            s = NULL;
        }
    }
}

bool TestServerInterface::ConnectToCommandServer(zmq::socket_t&) const {
    m_commandConnected = true;
    return m_commandConnected;
}

void TestServerInterface::ConnectToStateServer(zmq::socket_t&) const {
    m_stateConnected = true;
}

State const* TestServerInterface::GetState(zmq::socket_t&) const {
    CPPUNIT_ASSERT(m_stateConnected);
    if (!m_stateConnected)
        return NULL;
    pthread_mutex_lock(&m_messageMutex);
    // Waits for messages to be available
    if (m_messages.empty()) {
        pthread_cond_wait(&m_messagesAvailable, &m_messageMutex);
    }
    State const* message = m_messages.front();
    m_messages.pop();
    pthread_mutex_unlock(&m_messageMutex);
    return message;
}

bool TestServerInterface::SendMove(enum Tetromino::Move move, int pieceId, 
                                    zmq::socket_t&) const {
    CPPUNIT_ASSERT(m_commandConnected);
    if (!m_commandConnected)
        return false;
    pthread_mutex_lock(&m_moveMutex);
    m_moves.push(std::pair<enum Tetromino::Move, int>(move, pieceId));
    pthread_cond_signal(&m_movesAvailable);
    pthread_mutex_unlock(&m_moveMutex);
    return true;
}

void TestServerInterface::AddStateMessage(State const* message) const {
    pthread_mutex_lock(&m_messageMutex);
    m_messages.push(message);
    pthread_cond_signal(&m_messagesAvailable);
    pthread_mutex_unlock(&m_messageMutex);
}

std::pair<enum Tetromino::Move, int> TestServerInterface::GetNextMove() const {
    pthread_mutex_lock(&m_moveMutex);
    if (m_moves.empty()) {
        pthread_cond_wait(&m_movesAvailable, &m_moveMutex);
    }
    std::pair<enum Tetromino::Move, int> move = m_moves.front();
    m_moves.pop();
    // If queue is empty, set condition variable
    pthread_mutex_unlock(&m_moveMutex);
    return move;
}

