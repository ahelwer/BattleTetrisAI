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
                            int pieceId); 
    ServerInterface& m_si;
    std::queue<State const*> m_messageQueue;
    pthread_mutex_t m_queueMutex;
};

