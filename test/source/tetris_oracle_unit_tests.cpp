#include <test/tetris_oracle_unit_tests.hpp>
#include <core/game_state_eval.hpp>
#include <core/tetris_oracle.hpp>
#include <model/game_state.hpp>
#include <model/tetromino.hpp>
#include <util/constants.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(TetrisOracleUnitTests);

void TetrisOracleUnitTests::TestSquareBlockOnEmptyGrid() {
    std::vector<Tetromino> queue;
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
    std::vector<Tetromino> queue;
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
    std::vector<Tetromino> queue;
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

int TetrisOracleUnitTests::SimulatePath(GameState const& state,
                                            PathSequence const* path,
                                            Tetromino const& source,
                                            Tetromino const& target) {
    GameBoard const& board = state.GetBoard();
    if (source == target) {
        return path->size();
    }
    Tetromino c = source;
    int pastCount = -1;
    bool hitTarget = false;
    for (int i = 0; i < path->size(); ++i) {
        enum Tetromino::Move move = path->at(i);
        if (move == Tetromino::left)
            c.ShiftLeft();
        else if (move == Tetromino::right)
            c.ShiftRight();
        else if (move == Tetromino::down)
            c.ShiftDown();
        else if (move == Tetromino::lrotate)
            c.RotateLeft();
        else if (move == Tetromino::rrotate)
            c.RotateRight();
        else if (move == Tetromino::drop) {
            while (!board.IsAtRest(c) && board.IsValidMove(c))
                c.ShiftDown();
        }
        if (!board.IsValidMove(c))
            return -1;
        if (c == target) {
            int pathLeft = path->size() - i - 1;
            if (pathLeft == 1 && path->back() == Tetromino::drop)
                return 0;
            return (path->size() - i - 1);
        }
    }
    return -1;
}

void TetrisOracleUnitTests::TestFindPath() {
    GameState state;
    Tetromino source ('Z', 1, 5, 1);
    Tetromino target ('Z', 0, COLS-2, ROWS-2);
    PathSequence const* path = FindPath(state, source, target);
    CPPUNIT_ASSERT(path != NULL);
    CPPUNIT_ASSERT_EQUAL(0, SimulatePath(state, path, source, target));
    delete path;
}

void TetrisOracleUnitTests::TestSourceTargetEqual() {
    GameState state;
    Tetromino t ('S', 0, 1, ROWS-2);
    PathSequence const* path = FindPath(state, t, t);
    CPPUNIT_ASSERT(path != NULL);
    int pathSize = path->size();
    CPPUNIT_ASSERT_EQUAL(0, pathSize);
    delete path;
}

void TetrisOracleUnitTests::TestFindPathUnderOverhang() {
    GameState state;
    Tetromino j ('J', 1, 3, ROWS-2);
    state.SetPieceInPlay(&j);
    CPPUNIT_ASSERT(state.ApplyMove(j) != -1);
    Tetromino i ('I', 0, 6, ROWS-4);
    state.SetPieceInPlay(&i);
    CPPUNIT_ASSERT(state.ApplyMove(i) != -1);
    Tetromino oS ('O', 0, 5, 1);
    Tetromino oT ('O', 0, 5, ROWS-2);
    PathSequence const* path = FindPath(state, oS, oT);
    CPPUNIT_ASSERT(path != NULL);
    CPPUNIT_ASSERT_EQUAL(0, SimulatePath(state, path, oS, oT));
    delete path;
}

void TetrisOracleUnitTests::TestFindPathSlideIn() {
    GameState state;
    Tetromino init ('S', 0, 1, ROWS-2);
    state.SetPieceInPlay(&init);
    CPPUNIT_ASSERT(state.ApplyMove(init) != -1);
    init.ShiftUp();
    init.ShiftUp();
    init.ShiftRight();
    state.SetPieceInPlay(&init);
    CPPUNIT_ASSERT(state.ApplyMove(init) != -1);
    Tetromino source ('J', 2, 1, 1);
    Tetromino target ('J', 3, 3, ROWS-2);
    PathSequence const* path = FindPath(state, source, target);
    CPPUNIT_ASSERT(path != NULL);
    CPPUNIT_ASSERT_EQUAL(0, SimulatePath(state, path, source, target));
    delete path;
}

void TetrisOracleUnitTests::TestNoPath() {

}

