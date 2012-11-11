#include "message_factory.hpp"
#include "main.hpp"
#include <cstdio>
#include <json/json.h>

MessageFactory::MessageFactory()
{ }

MessageFactory::~MessageFactory()
{ }

std::string const* MessageFactory::CreateInitMessage(std::string const& matchToken) const {
    Json::Value root;
    Json::FastWriter writer;

    Json::Value comm_type ("MatchConnect");
    Json::Value match_token (matchToken);
    Json::Value team_name (USERNAME);
    Json::Value password (PASSWORD);

    root["comm_type"] = comm_type;
    root["match_token"] = match_token;
    root["team_name"] = team_name;
    root["password"] = password;

    std::string const* serialized = new std::string(writer.write(root));
    return serialized;
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

std::string const* MessageFactory::CreateMoveMessage(std::string const& moveS) const {
    Json::Value root;
    Json::FastWriter writer;

    Json::Value comm_type ("GameMove");
    Json::Value client_token (m_clientToken);
    Json::Value move (moveS);

    root["comm_type"] = comm_type;
    root["client_token"] = client_token;
    root["move"] = move;

    std::string const* serialized = new std::string(writer.write(root));
    return serialized;
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

State const* MessageFactory::ParseStateMessage(std::string const& stateS) const { // Parse message
    Json::Value root;
    Json::Reader reader;
    reader.parse(stateS, root); 

	State* newState = NULL;

    // Extract comm type
    std::string commType = root.get("comm_type", "not found").asString();
    if (commType.compare("GameBoardState") == 0) {
		int sequence = root.get("sequence", -1).asInt();
		int timestamp = root.get("timestamp", 0.0).asDouble();
        Json::Value states = root.get("states", "not found");

        Json::Value mine = states.get("Team 148", "not found");
		int myPiece = mine.get("piece_number", -1).asInt();
		char const* myBoard = mine.get("board_state", "0").asCString();
		Json::Value myClearedV = mine.get("cleared_rows", "not found");
		std::vector<int>* myCleared = new std::vector<int>();
		for (unsigned i = 0; i < myClearedV.size(); ++i)
			myCleared->push_back(myClearedV[i].asInt());

        Json::Value theirs = states.get("Test Client", "not found");
		int theirPiece = theirs.get("piece_number", -1).asInt();
		char const* theirBoard = theirs.get("board_state", "0").asCString();
		Json::Value theirClearedV = theirs.get("cleared_rows", "not found");
		std::vector<int>* theirCleared = new std::vector<int>();
		for (unsigned i = 0; i < theirClearedV.size(); ++i)
			theirCleared->push_back(theirClearedV[i].asInt());

		newState = new GameBoardState(sequence, timestamp, myBoard, theirBoard,
										myPiece, theirPiece, myCleared, theirCleared);
    }
    else if (commType.compare("GamePieceState") == 0) {
        std::cout << "Received GamePieceState" << std::endl;
        std::cout << "Sequence: " << root.get("sequence", -1).asInt() << std::endl;
        std::cout << "Timestamp: " << root.get("timestamp", 0.0).asDouble() << std::endl;
        Json::Value queue = root.get("queue", "not found");
        std::cout << "Queue: " ;
        for (unsigned i = 0; i < queue.size(); ++i) {
            std::cout << queue[i].asString() << " ";
        }
        std::cout << std::endl << std::endl;
    }
    else if (commType.compare("MatchEnd") == 0) {
        std::cout << "Match ended." << std::endl;
        std::cout << "Match name: " << root.get("match_name", "not found").asString() << std::endl;
    }
    else if (commType.compare("GameEnd") == 0) {
        std::cout << "Game ended." << std::endl;
        std::cout << "Game name: " << root.get("game_name", "not found").asString() << std::endl;
        std::cout << "Winner: " << root.get("winner", "not found").asString() << std::endl;
        Json::Value scores = root.get("scores", "not found");
        std::cout << "Scores:" << std::endl;
        std::cout << "  Team 148: " << scores.get("Team 148", -1).asInt() << std::endl;
        std::cout << "  Test Client: " << scores.get("Test Client", -1).asInt() << std::endl;
    }

	if (newState != NULL)
		return newState;
	else
		return (new ErrorState("Could not match message state type."));
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

