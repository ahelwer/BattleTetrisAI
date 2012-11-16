#pragma once

#include <zmq.hpp>
#include <server/server_interface.hpp>
#include <queue>
#include <pthread.h>

class Control {
public:
    Control(zmq::context_t& context, ServerInterface& si);
    ~Control();
    void Execute();
    void PollStateMessages();
private:
    void ExecuteSequence(std::vector<enum Tetromino::Move> const& sequence, 
                            int pieceId, zmq::socket_t& commandSocket); 
    zmq::context_t& m_context;
    ServerInterface& m_si;
    std::queue<State const*> m_messageQueue;
    pthread_mutex_t m_queueMutex;
};

