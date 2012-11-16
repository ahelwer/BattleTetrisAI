#include <server/message_factory.hpp>
#include <util/constants.hpp>
#include <model/tetromino.hpp>
#include <cstdio>
#include <cstring>

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

bool MessageFactory::ParseInitReply(std::string const& reply) const {
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
    if (commType.compare("ErrorResp") == 0) {
        std::string errorMessage = root.get("message", "not found").asString();
        errorMessage = "Move command rejected: " + errorMessage;
        return new ErrorState(errorMessage.c_str());
    }
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

std::string const* MessageFactory::CreateMoveMessage(enum Tetromino::Move move, int pieceId) const {
    Json::Value root;
    Json::FastWriter writer;

    Json::Value comm_type ("GameMove");
    Json::Value client_token (m_clientToken);

    root["comm_type"] = comm_type;
    root["client_token"] = client_token;
    if (move == Tetromino::left) {
        Json::Value moveV ("left");
        root["move"] = moveV;
    }
    else if (move == Tetromino::right) {
        Json::Value moveV ("right");
        root["move"] = moveV;
    }
    else if (move == Tetromino::down) {
        Json::Value moveV ("down");
        root["move"] = moveV;
    }
    else if (move == Tetromino::lrotate) {
        Json::Value moveV ("lrotate");
        root["move"] = moveV;
    }
    else if (move == Tetromino::rrotate) {
        Json::Value moveV ("rrotate");
        root["move"] = moveV;
    }
    else if (move == Tetromino::drop) {
        Json::Value moveV ("drop");
        root["move"] = moveV;
    }

    Json::Value number (pieceId);
    root["piece_number"] = number;

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
    //if (commType.compare("ErrorResp") == 0)
        //PrintErrorMessage(reply);
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
    if (!reader.parse(stateS, root))
        return new ErrorState("Parse failure on state message.");

    // Extract comm type
    std::string commType = root.get("comm_type", "not found").asString();
    if (commType.compare("GameBoardState") == 0)
        return ParseGameBoardStateMessage(root);
    else if (commType.compare("GamePieceState") == 0)
        return ParseGamePieceStateMessage(root);
    else if (commType.compare("MatchEnd") == 0)
        return ParseMatchEndStateMessage();
    else if (commType.compare("GameEnd") == 0)
        return ParseGameEndStateMessage(root);
    else
        return new ErrorState("Could not match state message type.");
}

State const* MessageFactory::ParseGameBoardStateMessage(Json::Value const& root) const {
    int sequence = root.get("sequence", -1).asInt();
    double timestamp = root.get("timestamp", 0.0).asDouble();
    Json::Value states = root.get("states", "not found");

    Json::Value mine = states.get(USERNAME, "not found");
    int myPiece = mine.get("piece_number", -1).asInt();
    Json::Value myBoardV = mine.get("board_state", "0");
    if (myBoardV.asString().size() != BOARD_DESC_SIZE)
        return new ErrorState("Invalid board description.");
    char* myBoard = new char[BOARD_DESC_SIZE];
    memcpy((void*)myBoard, (void*)myBoardV.asCString(), BOARD_DESC_SIZE);
    Json::Value myClearedV = mine.get("cleared_rows", "not found");
    std::vector<int>* myCleared = new std::vector<int>();
    for (unsigned i = 0; i < myClearedV.size(); ++i)
        myCleared->push_back(myClearedV[i].asInt());

    Json::Value theirs = states.get("Test Client", "not found");
    int theirPiece = theirs.get("piece_number", -1).asInt();
    Json::Value theirBoardV = theirs.get("board_state", "0");
    if (theirBoardV.asString().size() != BOARD_DESC_SIZE)
        return new ErrorState("Invalid board description.");
    char* theirBoard = new char[BOARD_DESC_SIZE];
    memcpy((void*)theirBoard, (void*)theirBoardV.asCString(), BOARD_DESC_SIZE);
    Json::Value theirClearedV = theirs.get("cleared_rows", "not found");
    std::vector<int>* theirCleared = new std::vector<int>();
    for (unsigned i = 0; i < theirClearedV.size(); ++i)
        theirCleared->push_back(theirClearedV[i].asInt());

    return new GameBoardState(sequence, timestamp, myBoard, 
                                theirBoard, myPiece, theirPiece, 
                                myCleared, theirCleared);
}

State const* MessageFactory::ParseGamePieceStateMessage(Json::Value const& root) const {
    int sequence = root.get("sequence", -1).asInt();
    double timestamp = root.get("timestamp", 0.0).asDouble();
    Json::Value states = root.get("states", "not found");

    Json::Value queueV = root.get("queue", "not found");
    std::vector<Tetromino>* queue = new std::vector<Tetromino>();
    for (unsigned i = 0; i < queueV.size(); ++i) {
        std::string piece = queueV[i].asString();
        if (piece.size() != 1)
            return new ErrorState("Invalid piece descriptor.");
        Tetromino t (piece.at(0));
        queue->push_back(t);
    }

    Tetromino* myTet = NULL;
    int myNumber = -1;
    Json::Value mine = states.get(USERNAME, "not found");
    if (!mine.isNull() && !mine.isInt()) {
        std::string myPieceS = mine.get("piece", "not found").asString();
        if (myPieceS.size() != 1)
            return new ErrorState("Invalid piece descriptor.");
        char myPiece = myPieceS.at(0);
        int myOrient = mine.get("orient", -1).asInt();
        int myCol = mine.get("col", -1).asInt();
        int myRow = mine.get("row", -1).asInt();
        myNumber = mine.get("number", -1).asInt();
        myTet = new Tetromino(myPiece, myOrient, myCol, myRow);
    }

    Tetromino* theirTet = NULL;
    int theirNumber = -1;
    Json::Value theirs = states.get("Test Client", "not found");
    if (!theirs.isNull() && !theirs.isInt()) {
        std::string theirPieceS = theirs.get("piece", "not found").asString();
        if (theirPieceS.size() != 1)
            return new ErrorState("Invalid piece descriptor.");
        char theirPiece = theirPieceS.at(0);
        int theirOrient = theirs.get("orient", -1).asInt();
        int theirCol = theirs.get("col", -1).asInt();
        int theirRow = theirs.get("row", -1).asInt();
        theirNumber = theirs.get("number", -1).asInt();
        theirTet = new Tetromino(theirPiece, theirOrient, theirCol, theirRow);
    }

    return new GamePieceState(sequence, timestamp, myTet, theirTet,
                                myNumber, theirNumber, queue);
}

State const* MessageFactory::ParseMatchEndStateMessage() const {
    return new MatchEnd();
}

State const* MessageFactory::ParseGameEndStateMessage(Json::Value const& root) const {
    int sequence = root.get("sequence", -1).asInt();
    double timestamp = root.get("timestamp", 0.0).asDouble();

    std::string winner = root.get("winner", "not found").asString();
    bool won = false;
    if (winner.compare(USERNAME) == 0)
        won = true;

    Json::Value scores = root.get("scores", "not found");
    int myScore = scores.get(USERNAME, -1).asInt();
    int theirScore = scores.get("Test Client", -1).asInt();

    return new GameEnd(sequence, timestamp, won, myScore, theirScore);
}

