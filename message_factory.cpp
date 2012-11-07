#include "message_factory.hpp"
#include <json/json.h>

MessageFactory::MessageFactory()
{ }

MessageFactory::~MessageFactory()
{ }

std::string& MessageFactory::CreateInitMessage(std::string const& matchToken) {

}

bool MessageFactory::ParseInitReply(std::string const& reply) {

}

std::string& MessageFactory::CreateMoveMessage(std::string const& move) {

}

bool MessageFactory::ParseMoveReply(std::string const& reply) {

}

