#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <server/server_interface.hpp>
#include <server/state.hpp>
#include <control/control.hpp>
#include <queue>
#include <zmq.h>

class TestServerInterface : public ServerInterface {
public:
    TestServerInterface(std::queue<State const*>& messages);
    ~TestServerInterface(); 
    virtual bool ConnectToCommandServer(zmq::socket_t& commandSocket) const;
    virtual void ConnectToStateServer(zmq::socket_t& stateSocket) const;
    virtual State const* GetState(zmq::socket_t& stateSocket) const;
    virtual bool SendMove(enum Tetromino::Move move, int pieceId, 
                            zmq::socket_t& commandSocket) const;
private:
    mutable std::queue<State const*>& m_messages;
    mutable bool m_commandConnected;
    mutable bool m_stateConnected;
};

class ControlIntegrationTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ControlIntegrationTests);

    CPPUNIT_TEST_SUITE_END();
public:

};

