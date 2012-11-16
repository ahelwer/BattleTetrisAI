#pragma once

#include <string>
#include <json/json.h>
#include <server/state.hpp>
#include <model/tetromino.hpp>

class MessageFactory {
public:
    MessageFactory();
    ~MessageFactory();
    std::string const* CreateInitMessage(std::string const& matchToken) const;
    bool ParseInitReply(std::string const& reply) const;
    std::string const* CreateMoveMessage(enum Tetromino::Move move, int pieceId) const;
    bool ParseMoveReply(std::string const& reply) const;
    State const* ParseStateMessage(std::string const& stateS) const;
private:
    State const* ParseGameBoardStateMessage(Json::Value const& root) const;
    State const* ParseGamePieceStateMessage(Json::Value const& root) const;
    State const* ParseMatchEndStateMessage() const;
    State const* ParseGameEndStateMessage(Json::Value const& root) const;
    mutable std::string m_clientToken;
};

