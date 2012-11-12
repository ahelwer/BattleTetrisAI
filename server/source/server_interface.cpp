#include <server/server_interface.hpp>
#include <iostream>

ServerInterface::ServerInterface(zmq::context_t& context, 
									std::string const& commandServer,
									std::string const& stateServer,
									std::string const& matchToken)
	: m_command(context, ZMQ_REQ), m_state(context, ZMQ_SUB),
		m_matchToken(matchToken)
{
    std::cout << "Connecting to command server " << commandServer << std::endl;
	m_command.connect(commandServer.c_str());
    std::cout << "Connecting to state server " << stateServer << std::endl;
	m_state.connect(stateServer.c_str());
}

ServerInterface::~ServerInterface() {
    std::cout << "Closing sockets." << std::endl;
    m_command.close();
    m_state.close();
}

bool ServerInterface::Initialize() {
    // Sends initial message to command server
    std::cout << "Initiating handshake... ";
    std::string const* requestS = m_factory.CreateInitMessage(m_matchToken);
	zmq::message_t request (requestS->size());
    memcpy(static_cast<void*>(request.data()), requestS->data(), requestS->size());
    delete requestS;
    m_command.send(request);

    // Subscribes to state server
    m_state.setsockopt(ZMQ_SUBSCRIBE, m_matchToken.c_str(), m_matchToken.size());

    zmq::message_t reply;
    m_command.recv(&reply);
    std::string replyS (static_cast<char const*>(reply.data()), reply.size());
    bool success = m_factory.ParseInitReply(replyS);
    if (success)
        std::cout << "Success!" << std::endl;
    else
        std::cout << "Failure." << std::endl;
	return success;
}

State const* ServerInterface::GetState() {
	zmq::message_t address;
	zmq::message_t state;
	m_state.recv(&address);
	m_state.recv(&state);
	std::string stateS (static_cast<char const*>(state.data()), state.size());
	return m_factory.ParseStateMessage(stateS);
}

