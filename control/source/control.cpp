#include <control/control.hpp>
#include <model/game_state.hpp>
#include <core/game_state_eval.hpp>
#include <core/tetris_oracle.hpp>
#include <core/harmony.hpp>
#include <server/state.hpp>
#include <algorithm>
#include <queue>

void* DispatchThread(void* varg) {
    Control* c = static_cast<Control*>(varg);
    c->PollStateMessages();
}

Control::Control(zmq::context_t& context, ServerInterface& si)
    : m_context(context), m_si(si)
{ 
    pthread_mutex_init(&m_queueMutex, NULL);
}

Control::~Control() {
    pthread_mutex_destroy(&m_queueMutex);
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
    std::cout << "Entering main loop." << std::endl;

    int weightCount = GetVarCount();
    float weights[] = {0.831252, -0.833718, -0.484039, -0.669239, -0.74901, 
                        -0.445552, -0.129955, -0.0353312, 0.468689, -0.115297, 
                        -0.604853, -0.436222, 0.760341, -0.0589, -0.79345, 
                        0.102076, 0.941241, -0.154894, -0.437886};
    /*
    // Original weights
    float weights[] = {0.413627, -0.723112, -0.214118, 0.0572252, 
                        0.666599, -0.758947, 0.297272, 0.112221, 
                        -0.571268, -0.255358, -0.63998, -0.572694, 
                        0.870288, -0.777475, -0.13282, -0.329776, 
                        0.285539, -0.739775, -0.163327};
                        */
    Harmony h;
    for (int i = 0; i < weightCount; ++i)
        h.push_back(weights[i]);

    bool gameOver = false;
    GameState game;
    Tetromino const* best = NULL;
    bool inMoveSequence = false;
    std::vector<enum Tetromino::Move> const* sequence = NULL;

    // Main loop
    while (!gameOver) {
        // Finds path to current best move then executes it
        if (best != NULL && game.GetPieceInPlay() != NULL && inMoveSequence) {
            if (sequence != NULL) {
                delete sequence;
                sequence = NULL;
            }
            sequence = FindPath(game, *(game.GetPieceInPlay()), *best);
			if (sequence != NULL && sequence->size() > 0)
				ExecuteSequence(*sequence, game.GetCurrentPieceNumber(), 
                                    commandSocket);
        }

        // Updates state by batching messages in queue
        pthread_mutex_lock(&m_queueMutex);
        while (!m_messageQueue.empty()) {
            State const* s = m_messageQueue.front();
            m_messageQueue.pop();
            if (s != NULL) {
                gameOver = s->ExecuteUpdates(game);
                delete s;
            }
        }

        // Determines if a re-search for best move is required
        pthread_mutex_unlock(&m_queueMutex);
        if (game.WasRowClearEvent() || game.PieceHasChanged()) {
            inMoveSequence = false;
        }

        // Finds best move in current state
        if (game.GetPieceInPlay() != NULL && !inMoveSequence) {
            if (best != NULL) {
                delete best;
                best = NULL;
            }
            best = FindBestMove(game, h);
			/**
            if (best != NULL) {
                GameState next = game;
                next.ApplyMove(*best);
                std::cout << next << std::endl;
            }
			*/
            if (best != NULL && game.GetPieceInPlay() != NULL && 
                    (*best) != *(game.GetPieceInPlay())) {
                inMoveSequence = true;
            }
        }
    }

    // Cleanup
    if (best != NULL) {
        delete best;
        best = NULL;
    }
    if (sequence != NULL) {
        delete sequence;
        sequence = NULL;
    }
    pthread_join(worker, NULL);
    commandSocket.close();
}

void Control::ExecuteSequence(std::vector<enum Tetromino::Move> const& sequence,
                               int pieceId, zmq::socket_t& commandSocket) {
    int sequenceSize = sequence.size();
    //int executeCount = std::min(sequenceSize, 10);
    int executeCount = sequenceSize;
    for (int i = 0; i < executeCount; ++i) {
        enum Tetromino::Move move = sequence.at(i);
        bool success = m_si.SendMove(move, pieceId, commandSocket);
        if (!success)
            return;
    }
}

