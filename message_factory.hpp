#pragma once

#include <string>
#include <json/json.h>
#include "state.hpp"

class MessageFactory {
public:
    MessageFactory();
    ~MessageFactory();
    std::string const* CreateInitMessage(std::string const& matchToken) const;
    bool ParseInitReply(std::string const& reply);
    std::string const* CreateMoveMessage(std::string const& moveS) const;
    bool ParseMoveReply(std::string const& reply) const;
    State const* ParseStateMessage(std::string const& stateS) const;
private:
    State const* ParseGameBoardStateMessage(Json::Value const& root) const;
    State const* ParseGamePieceStateMessage(Json::Value const& root) const;
    State const* ParseMatchEndStateMessage(Json::Value const& root) const;
    State const* ParseGameEndStateMessage(Json::Value const& root) const;
    std::string m_clientToken;
};

