#pragma once

#include <zmq.hpp>
#include <string>
#include <server/message_factory.hpp>
#include <server/state.hpp>
#include <model/tetromino.hpp>

class ServerInterface {
public:
    ServerInterface(std::string const& commandServer,
                    std::string const& stateServer,
                    std::string const& matchToken);
    // Connects provided socket to command server
    bool ConnectToCommandServer(zmq::socket_t& commandSocket) const;
    // Connects provided socket to state server
    void ConnectToStateServer(zmq::socket_t& stateSocket) const;
    // Polls state
    State const* GetState(zmq::socket_t& stateSocket) const;
    // Sends move
    bool SendMove(enum Tetromino::Move move, int pieceId, zmq::socket_t& commandSocket) const;
private:
    std::string const& m_commandServer;
    std::string const& m_stateServer;
    std::string const& m_matchToken;
    MessageFactory m_factory;
};

