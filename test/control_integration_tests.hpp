#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <server/server_interface.hpp>
#include <server/state.hpp>
#include <control/control.hpp>
#include <queue>
#include <utility>
#include <pthread.h>
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
    void AddStateMessage(State const* message) const;
    std::pair<enum Tetromino::Move, int> GetNextMove() const;
private:
    mutable std::queue<State const*> m_messages;
    mutable std::queue< std::pair<enum Tetromino::Move, int> > m_moves;
    mutable pthread_mutex_t m_messageMutex;
    mutable pthread_cond_t m_messagesAvailable;
    mutable pthread_mutex_t m_moveMutex;
    mutable pthread_cond_t m_movesAvailable;
    mutable bool m_commandConnected;
    mutable bool m_stateConnected;
};

class ControlIntegrationTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ControlIntegrationTests);

    CPPUNIT_TEST_SUITE_END();
public:

};

