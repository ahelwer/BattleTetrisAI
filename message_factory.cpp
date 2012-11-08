#include "message_factory.hpp"
#include <cstdio>
#include <json/json.h>

MessageFactory::MessageFactory()
{ }

MessageFactory::~MessageFactory()
{ }

std::string const& MessageFactory::CreateInitMessage(std::string const& matchToken) const {
    Json::Value root;
    Json::FastWriter writer;

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

std::string const& MessageFactory::CreateMoveMessage(std::string const& moveS) const {
    Json::Value root;
    Json::FastWriter writer;

    Json::Value comm_type ("GameMove");
    Json::Value client_token (m_clientToken);
    Json::Value move (moveS);

    root["comm_type"] = comm_type;
    root["client_token"] = client_token;
    root["move"] = move;

    std::string const* serialized = new std::string(writer.write(root));
    return (*serialized);
}

bool MessageFactory::ParseMoveReply(std::string const& reply) const {
    bool success = true;

    // Parse message
    Json::Value root;
    Json::Reader reader;
    success = reader.parse(reply, root);
    if (!success)
        return success;

    // Extract comm type (expect "GameMoveResp")
    std::string commType = root.get("comm_type", "not found").asString();
    success = (commType.compare("GameMoveResp") == 0);
    if (commType.compare("ErrorResp") == 0)
        PrintErrorMessage(reply);
    if (!success)
        return success;
    
    // Extract resp (expect "ok")
    std::string resp = root.get("resp", "not ok").asString();
    success = (resp.compare("ok") == 0);

    return success;
}

bool MessageFactory::ParseStateMessage(std::string const& stateS) const {
    bool gameOver = false;
    // Parse message
    Json::Value root;
    Json::Reader reader;
    reader.parse(stateS, root); 

    // Extract comm type
    std::string commType = root.get("comm_type", "not found").asString();
    if (commType.compare("GameBoardState") == 0) {
        std::cout << "Received GameBoardState" << std::endl;
        std::cout << "Sequence: " << root.get("sequence", -1).asInt() << std::endl;
        std::cout << "Timestamp: " << root.get("timestamp", 0.0).asDouble() << std::endl;
        Json::Value states = root.get("states", "not found");
        Json::Value team = states.get("Team 148", "not found");
        std::cout << "Piece number: " << team.get("piece_number", -1).asInt() << std::endl;;
        std::cout << std::endl;
        gameOver = false;
    }
    else if (commType.compare("GamePieceState") == 0) {
        std::cout << "Received GamePieceState" << std::endl;
        std::cout << "Sequence: " << root.get("sequence", -1).asInt() << std::endl;
        std::cout << "Timestamp: " << root.get("timestamp", 0.0).asDouble() << std::endl;
        Json::Value queue = root.get("queue", "not found");
        std::cout << "Queue: " ;
        for (unsigned i = 0; i < queue.size(); ++i) {
            std::cout << queue[i].asString << " ";
        }
        std::cout << std::endl << std::endl;
        gameOver = false;
    }
    else if (commType.compare("MatchEnd") == 0) {
        std::cout << "Match ended." << std::endl;
        std::cout << "Match name: " << root.get("match_name", "not found").asString() << std::endl;
        return true;
    }
    else if (commType.compare("GameEnd") == 0) {
        std::cout << "Game ended." << std::endl;
        std::cout << "Game name: " << root.get("game_name", "not found").asString() << std::endl;
        std::cout << "Winner: " << root.get("winner", "not found").asString() << std::endl;
        Json::Value scores = root.get("scores", "not found");
        std::cout << "Scores:" << std::endl;
        std::cout << "  Team 148: " << scores.get("Team 148", -1).asInt() << std::endl;
        std::cout << "  Test Client: " << scores.get("Test Client", -1).asInt() << std::endl;
        gameOver = false;
    }
    return gameOver;
}

std::string const& MessageFactory::GetClientToken() const {
    return m_clientToken;
}

void MessageFactory::PrintErrorMessage(std::string const& message) const {
    Json::Value root;
    Json::Reader reader;
    reader.parse(message, root);
    std::string error = root.get("error", "not found").asString();
    std::string desc = root.get("message", "not found").asString();
    std::cout << error << std::endl;
    std::cout << desc << std::endl;
}

