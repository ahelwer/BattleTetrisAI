#include <test/game_state_unit_tests.hpp>
#include <model/tetromino.hpp>

#include <util/vector.hpp>
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(GameStateUnitTests);

void GameStateUnitTests::setUp() {
    m_pState = new GameState();
}

void GameStateUnitTests::tearDown() {
    if (m_pState != NULL) {
        delete m_pState;
        m_pState = NULL;
    }
}

void GameStateUnitTests::TestInit() {
    CPPUNIT_ASSERT(m_pState->GetPieceInPlay() == NULL);
    std::vector<int> const& last = m_pState->LastClearedRows();
    int lastSize = last.size();
    CPPUNIT_ASSERT_EQUAL(0, lastSize);
    CPPUNIT_ASSERT_EQUAL(-1, m_pState->GetCurrentPieceNumber());
    CPPUNIT_ASSERT(!m_pState->WasRowClearEvent());
    CPPUNIT_ASSERT(!m_pState->PieceHasChanged());
}

void GameStateUnitTests::TestApplyMove() {
    //Tetromino t ('O', o
}

void GameStateUnitTests::TestFeedFromQueue() {

}

void GameStateUnitTests::TestSetMethods() {

}

void GameStateUnitTests::TestEventMethods() {

}

