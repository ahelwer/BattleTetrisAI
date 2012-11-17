#pragma once

#include <zmq.hpp>
#include <string>
#include <server/message_factory.hpp>
#include <server/state.hpp>
#include <model/tetromino.hpp>

class ServerInterface {
public:
    ServerInterface();
    ServerInterface(std::string const& commandServer,
                    std::string const& stateServer,
                    std::string const& matchToken);
    virtual ~ServerInterface();
    // Connects provided socket to command server
    virtual bool ConnectToCommandServer(zmq::socket_t& commandSocket) const;
    // Connects provided socket to state server
    virtual void ConnectToStateServer(zmq::socket_t& stateSocket) const;
    // Polls state
    virtual State const* GetState(zmq::socket_t& stateSocket) const;
    // Sends move
    virtual bool SendMove(enum Tetromino::Move move, int pieceId, zmq::socket_t& commandSocket) const;
private:
    std::string const& m_commandServer;
    std::string const& m_stateServer;
    std::string const& m_matchToken;
    MessageFactory m_factory;
};

