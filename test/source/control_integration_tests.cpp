#include <test/control_integration_tests.hpp>
#include <time.h>

TestServerInterface::TestServerInterface(std::queue<State const*>& messages)
    : m_messages(messages), m_commandConnected(false), m_stateConnected(false)
{ }

TestServerInterface::~TestServerInterface()
{ }

bool TestServerInterface::ConnectToCommandServer(zmq::socket_t& commandSocket) const {
    m_commandConnected = true;
    return m_commandConnected;
}

void TestServerInterface::ConnectToStateServer(zmq::socket_t& stateSocket) const {
    m_stateConnected = true;
}

State const* TestServerInterface::GetState(zmq::socket_t& stateSocket) const {
    CPPUNIT_ASSERT(m_stateConnected);
    if (!m_stateConnected)
        return NULL;
    /*
    timespec t0 = 0;
    timespec t1 = 250000000L;
    nanosleep(&t0, &t1);
    */
    State const* message = m_messages.front();
    m_messages.pop();
    return message;
}

bool TestServerInterface::SendMove(enum Tetromino::Move move, int pieceId, 
                                    zmq::socket_t& commandSocket) const {
    CPPUNIT_ASSERT(m_commandConnected);
    if (!m_commandConnected)
        return false;
    /*
    timespec t0 = 0;
    timespec t1 = 25000000L;
    nanosleep(&t0, &t1);
    */
    return true;
}

