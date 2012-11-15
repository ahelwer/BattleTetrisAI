#pragma once

#include <zmq.hpp>
#include <server/server_interface.hpp>

class Control {
public:
    Control(zmq::context_t& context, ServerInterface& si);
    void Execute();
private:
    void ExecuteSequence(std::vector<enum Tetromino::Move> const& sequence, 
                            int pieceId); 
    ServerInterface& m_si;
};

