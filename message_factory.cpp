#include "message_factory.hpp"
#include <json/json.h>

MessageFactory::MessageFactory()
{ }

MessageFactory::~MessageFactory()
{ }

std::string const& MessageFactory::CreateInitMessage(std::string const& matchToken) const {
    Json::Value root;
    Json::StyledWriter writer;

    Json::Value comm_type ("MatchConnect");
    Json::Value match_token (matchToken);
    Json::Value team_name ("Team 148");
    Json::Value password ("agrajabber");

    root["comm_type"] = comm_type;
    root["match_token"] = match_token;
    root["team_name"] = team_name;
    root["password"] = password;

    std::string const* serialized = new std::string(writer.write(root));
    return (*serialized);
}

bool MessageFactory::ParseInitReply(std::string const& reply) {
    bool success = true;

    // Parse message
    Json::Value root;
    Json::Reader reader;
    success = reader.parse(reply, root);
    if (!success)
        return success;

    // Extract comm type (expect "MatchConnectResp")
    std::string commType = root.get("comm_type", "not found").asString();
    success = (commType.compare("MatchConnectResp") == 0);
    if (commType.compare("ErrorResp") == 0)
        PrintErrorMessage(reply);
    if (!success)
        return success;

    // Extract resp (expect "ok")
    std::string resp = root.get("resp", "not ok").asString();
    success = (resp.compare("ok") == 0);
    if (!success)
        return success;

    // Extract client token (expect some value)
    std::string token = root.get("client_token", "not found").asString();
    success = (token.compare("not found") != 0);
    if (!success)
        return success;

    // Record client token, return success
    m_clientToken = token;
    return success;
}

std::string const& MessageFactory::CreateMoveMessage(std::string const& move) const {

}

bool MessageFactory::ParseMoveReply(std::string const& reply) const {

}

std::string const& MessageFactory::GetClientToken() const {
    return m_clientToken;
}

void MessageFactory::PrintErrorMessage(std::string const& message) {
    Json::Value root;
    Json::Reader reader;
    reader.parse(message, root);
    std::string error = root.get("error", "not found").asString();
    std::string desc = root.get("message", "not found").asString();
    std::cout << error << std::endl;
    std::cout << desc << std::endl;
}

