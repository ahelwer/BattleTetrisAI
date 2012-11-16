#include <server/server_interface.hpp>
#include <iostream>

ServerInterface::ServerInterface(std::string const& commandServer,
                                    std::string const& stateServer,
                                    std::string const& matchToken)
    : m_commandServer(commandServer), m_stateServer(stateServer), 
        m_matchToken(matchToken)
{ }

bool ServerInterface::ConnectToCommandServer(zmq::socket_t& commandSocket) const {
    commandSocket.connect(m_commandServer.c_str());
    std::cout << "Connecting to command server... ";
    std::string const* requestS = m_factory.CreateInitMessage(m_matchToken);
    zmq::message_t request (requestS->size());
    memcpy(static_cast<void*>(request.data()), requestS->data(), requestS->size());
    delete requestS;
    commandSocket.send(request);

    zmq::message_t reply;
    commandSocket.recv(&reply);
    std::string replyS (static_cast<char const*>(reply.data()), reply.size());
    bool success = m_factory.ParseInitReply(replyS);
    if (success)
        std::cout << "Success!" << std::endl;
    else
        std::cout << "Failure." << std::endl;
    return success;
}

void ServerInterface::ConnectToStateServer(zmq::socket_t& stateSocket) const {
    std::cout << "Connecting to state server " << m_stateServer << std::endl;
    stateSocket.connect(m_stateServer.c_str());
    stateSocket.setsockopt(ZMQ_SUBSCRIBE, m_matchToken.c_str(), m_matchToken.size());
}

State const* ServerInterface::GetState(zmq::socket_t& stateSocket) const {
    zmq::message_t address;
    zmq::message_t state;
    stateSocket.recv(&address);
    stateSocket.recv(&state);
    std::string stateS (static_cast<char const*>(state.data()), state.size());
    return m_factory.ParseStateMessage(stateS);
}

bool ServerInterface::SendMove(enum Tetromino::Move move, int pieceId, zmq::socket_t& commandSocket) const {
    std::string const* moveCommandS = m_factory.CreateMoveMessage(move, pieceId);
    zmq::message_t moveCommand (moveCommandS->size());
    memcpy(static_cast<void*>(moveCommand.data()), moveCommandS->data(), 
                                moveCommandS->size());
    delete moveCommandS;
    commandSocket.send(moveCommand);

    zmq::message_t reply;
    commandSocket.recv(&reply);
    std::string replyS (static_cast<char const*>(reply.data()), reply.size());
    return m_factory.ParseMoveReply(replyS);
}

