#pragma once

#include <zmq.hpp>
#include <string>

class ServerInterface {
public:
	ServerInterface(zmq::context_t& context, 
					std::string const& commandServer,
					std::string const& stateServer,
					std::string const& matchToken);
private:
	zmq::socket_t& m_command;
	zmq::socket_t& m_state;
	std::string const& m_matchToken;
	std::string m_userToken;
};

