#pragma once

#include <zmq.hpp>
#include <string>
#include "message_factory.hpp"
#include "state.hpp"

class ServerInterface {
public:
	ServerInterface(zmq::context_t& context, 
					std::string const& commandServer,
					std::string const& stateServer,
					std::string const& matchToken);
    ~ServerInterface();
    // Performs initial handshake with server
    void Initialize();
    // Polls state
    State const& GetState();
private:
	zmq::socket_t m_command;
	zmq::socket_t m_state;
	std::string const& m_matchToken;
    MessageFactory m_factory;
};

