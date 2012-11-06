#include "server_interface.hpp"
#include <json/json.h>


ServerInterface::ServerInterface(zmq::context_t& context, 
									std::string const& commandServer,
									std::string const& stateServer,
									std::string const& matchToken)
	: m_command(context, ZMQ_REQ), m_state(context, ZMQ_SUB),
		m_matchToken(matchToken)
{
	m_command.connect(commandServer.c_str());
	m_state.connect(stateServer.c_str());
}
