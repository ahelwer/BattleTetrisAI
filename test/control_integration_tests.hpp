#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <server/server_interface.hpp>
#include <server/state.hpp>
#include <control/control.hpp>
#include <queue>
#include <utility>
#include <pthread.h>
#include <zmq.h>

/* *
 * Class TestServerInterface
 *
 * Pretends to be a working interface to a gameserver; in actuality
 * logs and adds messages according to the whim of the owner, presumably
 * the test code contained within this module.
 * */
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
    mutable pthread_mutex_t m_moveMutex;
    mutable pthread_cond_t m_messagesAvailable;
    mutable pthread_cond_t m_movesAvailable;
    mutable bool m_commandConnected;
    mutable bool m_stateConnected;
};

/* *
 * class InterceptInternalState
 *
 * Intercepts reference to internal game state, pointing provided 
 * pointer variable to it. Hooray for testable program design!
 * */
class InterceptInternalState : public State {
public:
    InterceptInternalState(GameState*& gameStatePointer,
                            pthread_mutex_t& pointerMutex,
                            pthread_cond_t& pointerSet);
    virtual bool ExecuteUpdates(GameState& game) const;
    virtual bool ShouldTerminate() const;
private:
    mutable GameState*& m_gameStatePointer;
    mutable pthread_mutex_t& m_pointerMutex;
    mutable pthread_cond_t& m_pointerSet;
};

/* *
 * class SynchronizeMessage
 *
 * Used to synchronize testing code with control loop code by
 * sending a signal to the provided condition variable.
 * */
class SynchronizeMessage : public State {
public: 
    SynchronizeMessage(pthread_mutex_t& signalMutex, 
                        pthread_cond_t& signal);
    virtual bool ExecuteUpdates(GameState& game) const;
    virtual bool ShouldTerminate() const;
private:
    mutable pthread_mutex_t& m_signalMutex;
    mutable pthread_cond_t& m_signal;
};

class ControlIntegrationTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ControlIntegrationTests);
    CPPUNIT_TEST(TestPlacePiece);
    CPPUNIT_TEST_SUITE_END();
public:
    void TestPlacePiece();
};

