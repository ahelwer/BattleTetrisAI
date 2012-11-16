#include <test/tetris_oracle_unit_tests.hpp>
#include <core/game_state_eval.hpp>
#include <core/tetris_oracle.hpp>
#include <model/game_state.hpp>
#include <model/tetromino.hpp>
#include <util/constants.hpp>
#include <queue>

CPPUNIT_TEST_SUITE_REGISTRATION(TetrisOracleUnitTests);

void TetrisOracleUnitTests::TestSquareBlockOnEmptyGrid() {
    std::queue<Tetromino> queue;
    Tetromino t ('O', 0, 5, 1);
    GameState state (queue, t);
    std::vector<Tetromino> const* found = FindPossibleMoves(state);
    size_t expectedSize = COLS-1;
    CPPUNIT_ASSERT_EQUAL(expectedSize, found->size());
    int colPos[COLS];
    for (int i = 0; i < COLS; ++i)
        colPos[i] = 0;
    for (int i = 0; i < found->size(); ++i) {
        Tetromino const& t = found->at(i);
        colPos[t.GetX()] += 1;
        CPPUNIT_ASSERT_EQUAL(ROWS-2, t.GetY());
    }
    CPPUNIT_ASSERT_EQUAL(0, colPos[0]);
    for (int i = 1; i < COLS; ++i) 
        CPPUNIT_ASSERT_EQUAL(1, colPos[i]);
    delete found;
}

void TetrisOracleUnitTests::TestLineBlockOnEmptyGrid() {
    std::queue<Tetromino> queue;
    Tetromino t ('I', 0, 5, 1);
    GameState state (queue, t);
    std::vector<Tetromino> const* found = FindPossibleMoves(state);
    size_t expectedSize = COLS + COLS-3; 
    CPPUNIT_ASSERT_EQUAL(expectedSize, found->size());
    for (int i = 0; i < found->size(); ++i) {
        Tetromino const& t = found->at(i);
        CPPUNIT_ASSERT((t.GetY() == ROWS-1) || (t.GetY() == ROWS-3));
    }
    delete found;
}

void TetrisOracleUnitTests::TestFindBestMoveSquareBlockEmptyGrid() {
    std::queue<Tetromino> queue;
    Tetromino t ('O', 0, 5, 1);
    GameState state (queue, t);
    Harmony h;
    int varCount = GetVarCount();
    for (int i = 0; i < varCount; ++i)
        h.push_back(1.0);
    Tetromino const* best = FindBestMove(state, h);
    //std::cout << std::endl << *best << std::endl;
    CPPUNIT_ASSERT(best != NULL);
    delete best;
}

