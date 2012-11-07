#pragma once

#include <string>

class MessageFactory {
public:
    MessageFactory();
    ~MessageFactory();
    std::string& CreateInitMessage(std::string const& matchToken);
    bool ParseInitReply(std::string const& reply);
    std::string& CreateMoveMessage(std::string const& move);
    bool ParseMoveReply(std::string const& reply);
private:
    std::string m_clientToken;
};

