#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <model/game_state.hpp>

class GameStateUnitTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(GameStateUnitTests);
    CPPUNIT_TEST(TestInit);
    CPPUNIT_TEST(TestAssignment);
    CPPUNIT_TEST(TestApplyMove);
    CPPUNIT_TEST(TestFeedFromQueue);
    CPPUNIT_TEST(TestEventMethods);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
    void TestInit();
    void TestAssignment();
    void TestApplyMove();
    void TestFeedFromQueue();
    void TestEventMethods();
private:
    GameState* m_pState;
};

