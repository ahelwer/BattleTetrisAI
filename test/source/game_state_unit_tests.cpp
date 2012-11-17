#include <test/game_state_unit_tests.hpp>
#include <model/tetromino.hpp>
#include <util/constants.hpp>

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

void GameStateUnitTests::TestAssignment() {
    GameState a;
    Tetromino t ('O', 0, 5, COLS-2);
    a.SetPieceInPlay(&t);
    std::vector<Tetromino> queue;
    queue.push_back(t);
    a.SetQueueInPlay(queue);
    std::vector<int> cleared;
    cleared.push_back(0);
    a.SetLastClearedRows(cleared);
    a.SetCurrentPieceNumber(4);
    GameState b = a;

    CPPUNIT_ASSERT(b == a);
}

void GameStateUnitTests::TestApplyMove() {
    Tetromino t ('I', 0, 2, ROWS-1);
    m_pState->SetPieceInPlay(&t);
    CPPUNIT_ASSERT_EQUAL(0, m_pState->ApplyMove(t));
    t.ShiftUp();
    t.ShiftUp();
    m_pState->SetPieceInPlay(&t);
    CPPUNIT_ASSERT_EQUAL(0, m_pState->ApplyMove(t));
    t.ShiftDown();
    t.ShiftDown();
    t.ShiftRight();
    t.ShiftRight();
    t.ShiftUp();
    m_pState->SetPieceInPlay(&t);
    CPPUNIT_ASSERT_EQUAL(0, m_pState->ApplyMove(t));
    t.ShiftUp();
    t.ShiftUp();
    m_pState->SetPieceInPlay(&t);
    CPPUNIT_ASSERT_EQUAL(0, m_pState->ApplyMove(t));
    t.ShiftDown();
    t.ShiftDown();
    t.ShiftDown();
    t.ShiftRight();
    t.ShiftRight();
    m_pState->SetPieceInPlay(&t);
    CPPUNIT_ASSERT_EQUAL(0, m_pState->ApplyMove(t));
    t.ShiftUp();
    t.ShiftUp();
    m_pState->SetPieceInPlay(&t);
    CPPUNIT_ASSERT_EQUAL(0, m_pState->ApplyMove(t));
    t.ShiftDown();
    t.ShiftDown();
    t.ShiftRight();
    t.ShiftRight();
    t.RotateRight();
    t.ShiftUp();
    t.ShiftUp();
    m_pState->SetPieceInPlay(&t);
    CPPUNIT_ASSERT_EQUAL(0, m_pState->ApplyMove(t));
    t.ShiftRight();
    m_pState->SetPieceInPlay(&t);
    CPPUNIT_ASSERT_EQUAL(2, m_pState->ApplyMove(t));
    std::vector<int> const& cleared = m_pState->LastClearedRows();
    CPPUNIT_ASSERT_EQUAL(ROWS-3, cleared.at(0));
    CPPUNIT_ASSERT_EQUAL(ROWS-1, cleared.at(1));
    
}

void GameStateUnitTests::TestFeedFromQueue() {

}

void GameStateUnitTests::TestSetMethods() {

}

void GameStateUnitTests::TestEventMethods() {

}

