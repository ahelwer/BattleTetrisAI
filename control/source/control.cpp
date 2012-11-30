#include <control/control.hpp>
#include <model/game_state.hpp>
#include <core/game_state_eval.hpp>
#include <core/tetris_oracle.hpp>
#include <server/state.hpp>
#include <algorithm>
#include <queue>

void* DispatchThread(void* varg) {
    Control* c = static_cast<Control*>(varg);
    c->PollStateMessages();
    return varg;
}

Control::Control(zmq::context_t& context, ServerInterface const& si,
                    Harmony const& weights)
    : m_context(context), m_si(si), m_weights(weights)
{ 
    pthread_mutex_init(&m_queueMutex, NULL);
    pthread_cond_init(&m_queueNonempty, NULL);
}

Control::~Control() {
    pthread_mutex_destroy(&m_queueMutex);
    pthread_cond_destroy(&m_queueNonempty);
    while (!m_messageQueue.empty()) {
        State const* s = m_messageQueue.front();
        m_messageQueue.pop();
        if (s != NULL) {
            delete s;
            s = NULL;
        }
    }
}

void Control::PollStateMessages() {
    zmq::socket_t stateSocket(m_context, ZMQ_SUB);
    m_si.ConnectToStateServer(stateSocket);
    bool terminate = false;
    // Reads messages into queue
    while (!terminate) {
        State const* state = m_si.GetState(stateSocket); 
        if (state != NULL) {
            terminate = state->ShouldTerminate();
            pthread_mutex_lock(&m_queueMutex);
            m_messageQueue.push(state);
            pthread_cond_signal(&m_queueNonempty);
            pthread_mutex_unlock(&m_queueMutex);
        }
    }
    stateSocket.close();
}

void Control::Execute() {
    // Launches state messaging thread
    pthread_t worker = 0;
    pthread_create(&worker, NULL, DispatchThread, static_cast<void*>(this));

    zmq::socket_t commandSocket(m_context, ZMQ_REQ);
    bool success = m_si.ConnectToCommandServer(commandSocket);
    if (!success)
        exit(-1);

    bool gameOver = false;
    GameState game;
    int executedNumber = -1;

    // Main loop
    while (!gameOver) {
        // Finds and executes best move
        if (game.GetPieceInPlay() != NULL && game.GetCurrentPieceNumber() != executedNumber) {
            Tetromino const* best = FindBestMove(game, m_weights);
            Tetromino const* inPlay = game.GetPieceInPlay();
            if (best != NULL && *best != *inPlay) {
                PathSequence const* sequence = FindPath(game, *inPlay, *best);
                int pieceNumber = game.GetCurrentPieceNumber();
                if (sequence != NULL && sequence->size() > 0) {
                    ExecuteSequence(*sequence, pieceNumber, commandSocket); 
                    executedNumber = pieceNumber;
                    delete sequence;
                }
                delete best;
            }
        }

        // Updates state by batch-processing messages in queue
        pthread_mutex_lock(&m_queueMutex);
        if (m_messageQueue.empty()) {
            pthread_cond_wait(&m_queueNonempty, &m_queueMutex);
        }
        while (!m_messageQueue.empty()) {
            State const* s = m_messageQueue.front();
            m_messageQueue.pop();
            if (s != NULL) {
                gameOver = s->ExecuteUpdates(game);
                delete s;
            }
        }
        pthread_mutex_unlock(&m_queueMutex);
    }

    // Cleanup
    pthread_join(worker, NULL);
    commandSocket.close();
}

void Control::ExecuteSequence(std::vector<enum Tetromino::Move> const& sequence,
                               int pieceId, zmq::socket_t& commandSocket) {
    for (unsigned i = 0; i < sequence.size(); ++i) {
        enum Tetromino::Move move = sequence.at(i);
        bool success = m_si.SendMove(move, pieceId, commandSocket);
        if (!success)
            return;
    }
}

