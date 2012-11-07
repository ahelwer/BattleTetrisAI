#pragma once

#include <string>

class MessageFactory {
public:
    MessageFactory();
    ~MessageFactory();
    std::string const& CreateInitMessage(std::string const& matchToken) const;
    bool ParseInitReply(std::string const& reply);
    std::string const& CreateMoveMessage(std::string const& move) const;
    bool ParseMoveReply(std::string const& reply) const;
    std::string const& GetClientToken() const;
private:
    std::string m_clientToken;
};

