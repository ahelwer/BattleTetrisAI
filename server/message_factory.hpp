#pragma once

#include <string>
#include <json/json.h>
#include <server/state.hpp>
#include <model/tetromino.hpp>

/* *
 * Class MessageFactory
 *
 * Parses JSON into State messages or creates JSON messages
 * to send to game server.
 * */
class MessageFactory {
public:
    MessageFactory();
    // Creates initial JSON handshake message with given match token
    std::string const* CreateInitMessage(std::string const& matchToken) const;
    // Parses initial handshake message reply
    bool ParseInitReply(std::string const& reply) const;
    // Creates JSON move command message
    std::string const* CreateMoveMessage(enum Tetromino::Move move, int pieceId) const;
    // Parses reply to move command message
    bool ParseMoveReply(std::string const& reply) const;
    // Parses JSON into State message
    State const* ParseStateMessage(std::string const& stateS) const;
private:
    State const* ParseGameBoardStateMessage(Json::Value const& root) const;
    State const* ParseGamePieceStateMessage(Json::Value const& root) const;
    State const* ParseMatchEndStateMessage() const;
    State const* ParseGameEndStateMessage(Json::Value const& root) const;
    mutable std::string m_clientToken;
};

