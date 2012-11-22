#pragma once

#include <zmq.hpp>
#include <server/server_interface.hpp>
#include <model/tetromino.hpp>
#include <queue>
#include <pthread.h>

/* *
 * Class Control
 *
 * Main control loop for Tetris client. Coordinates querying server for updates,
 * searching for best move, and sending move commands to server.
 * */
class Control {
public:
    Control(zmq::context_t& context, ServerInterface const& si);
    ~Control();
    // Main execution method
    void Execute();
    // Launched in additional thread. Parse state messages into queue.
    void PollStateMessages();
private:
    // Sends sequence of moves to be executed on server.
    void ExecuteSequence(PathSequence const& sequence, int pieceId, 
                            zmq::socket_t& commandSocket); 
    zmq::context_t& m_context;
    ServerInterface const& m_si;
    std::queue<State const*> m_messageQueue;
    pthread_mutex_t m_queueMutex;
    pthread_cond_t m_queueNonempty;
};

