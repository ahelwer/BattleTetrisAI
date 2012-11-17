#include <test/game_board_unit_tests.hpp>
#include <model/tetromino.hpp>
#include <util/constants.hpp>

#include <util/vector.hpp>
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(GameBoardUnitTests);

void GameBoardUnitTests::setUp() {
    m_pBoard = new GameBoard();
}

void GameBoardUnitTests::tearDown() {
    if (m_pBoard != NULL) {
        delete m_pBoard;
        m_pBoard = NULL;
    }
}

void GameBoardUnitTests::TestInit() {
    BoardDesc const& desc = m_pBoard->GetBoardDesc();
    for (int j = 0; j < ROWS; ++j) {
        for (int i = 0; i < COLS; ++i) {
            CPPUNIT_ASSERT(m_pBoard->InBounds(i, j));
            CPPUNIT_ASSERT_EQUAL(m_pBoard->WellDepth(i), ROWS);
            CPPUNIT_ASSERT(!desc[i][j]);
        }
    }
}

void GameBoardUnitTests::TestTranslateEmpty() {
    BoardDesc const& desc = m_pBoard->GetBoardDesc();
    char* shortDesc = new char[BOARD_DESC_SIZE];
    for (int i = 0; i < BOARD_DESC_SIZE; ++i)
        shortDesc[i] = '0';
    CPPUNIT_ASSERT(!m_pBoard->HasChanged(shortDesc));
    m_pBoard->Update(shortDesc);
    for (int j = 0; j < ROWS; ++j) {
        for (int i = 0; i < COLS; ++i) {
            CPPUNIT_ASSERT(!desc[i][j]);
        }
    }
    CPPUNIT_ASSERT(!m_pBoard->HasChanged(shortDesc));
    delete[] shortDesc;
}

void GameBoardUnitTests::TestTranslateFull() {
    BoardDesc const& desc = m_pBoard->GetBoardDesc();
    char* shortDesc = new char[BOARD_DESC_SIZE];
    for (int i = 0; i < BOARD_DESC_SIZE; ++i)
        shortDesc[i] = 'F';
    CPPUNIT_ASSERT(m_pBoard->HasChanged(shortDesc));
    m_pBoard->Update(shortDesc);
    for (int j = 0; j < ROWS; ++j) {
        for (int i = 0; i < COLS; ++i) {
            CPPUNIT_ASSERT_EQUAL(m_pBoard->WellDepth(i), 0);
            CPPUNIT_ASSERT(desc[i][j]);
        }
    }
    CPPUNIT_ASSERT(!m_pBoard->HasChanged(shortDesc));
    delete[] shortDesc;
}

void GameBoardUnitTests::TestTranslateComplex() {
    BoardDesc const& desc = m_pBoard->GetBoardDesc();
    char* shortDesc = new char[BOARD_DESC_SIZE];
    for (int i = 0; i < BOARD_DESC_SIZE; ++i)
        shortDesc[i] = 'A';
    CPPUNIT_ASSERT(m_pBoard->HasChanged(shortDesc));
    m_pBoard->Update(shortDesc);
    for (int j = 0; j < ROWS; ++j) {
        for (int i = 0; i < COLS; ++i) {
            bool expected = (i%2);
            CPPUNIT_ASSERT_EQUAL(expected*ROWS, m_pBoard->WellDepth(i));
            CPPUNIT_ASSERT_EQUAL(!expected, desc[i][j]);
        }
    }
    CPPUNIT_ASSERT(!m_pBoard->HasChanged(shortDesc));
    delete[] shortDesc;
}

void GameBoardUnitTests::TestApplyPiece() {
    Tetromino t1 ('O', 0, 5, ROWS-2);
    CPPUNIT_ASSERT(m_pBoard->IsValidMove(t1));
    CPPUNIT_ASSERT(m_pBoard->PushMove(t1));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(3));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(4));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(5));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(6));
    m_pBoard->PopMove();

    Tetromino t2 ('O', 0, COLS-1, ROWS-2);
    CPPUNIT_ASSERT(m_pBoard->IsValidMove(t2));
    CPPUNIT_ASSERT(m_pBoard->PushMove(t2));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(COLS-3));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(COLS-2));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(COLS-1));
    m_pBoard->PopMove();

    Tetromino t3 ('O', 0, 1, ROWS-2);
    CPPUNIT_ASSERT(m_pBoard->IsValidMove(t3));
    CPPUNIT_ASSERT(m_pBoard->PushMove(t3));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(0));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(1));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(2));
    m_pBoard->PopMove();

    Tetromino t4 ('O', 0, 0, ROWS-2);
    CPPUNIT_ASSERT(!m_pBoard->IsValidMove(t4));
    CPPUNIT_ASSERT(!m_pBoard->PushMove(t4));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(0));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(1));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(2));

    Tetromino t5 ('O', 0, COLS, ROWS-2);
    CPPUNIT_ASSERT(!m_pBoard->IsValidMove(t5));
    CPPUNIT_ASSERT(!m_pBoard->PushMove(t5));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(COLS-1));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(COLS-2));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(COLS-3));
}

void GameBoardUnitTests::TestApplyMultiplePieces() {
    Tetromino t1 ('O', 0, 5, ROWS-2);
    CPPUNIT_ASSERT(m_pBoard->IsValidMove(t1));
    CPPUNIT_ASSERT(m_pBoard->PushMove(t1));
    Tetromino t2 ('O', 0, 4, ROWS-4);
    CPPUNIT_ASSERT(m_pBoard->IsValidMove(t2));
    CPPUNIT_ASSERT(m_pBoard->PushMove(t2));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(2));
    CPPUNIT_ASSERT_EQUAL(ROWS-4, m_pBoard->WellDepth(3));
    CPPUNIT_ASSERT_EQUAL(ROWS-4, m_pBoard->WellDepth(4));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(5));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(6));
    m_pBoard->PopMove();
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(3));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(4));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(5));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(6));

    Tetromino t3 ('J', 1, 3, ROWS-2);
    CPPUNIT_ASSERT(m_pBoard->IsValidMove(t3));
    CPPUNIT_ASSERT(m_pBoard->PushMove(t3));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(2));
    CPPUNIT_ASSERT_EQUAL(ROWS-3, m_pBoard->WellDepth(3));
    CPPUNIT_ASSERT_EQUAL(ROWS-3, m_pBoard->WellDepth(4));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(5));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(6));
    m_pBoard->PopMove();
    m_pBoard->PopMove();
}

void GameBoardUnitTests::TestClearAdjacentRows() {
    Tetromino t ('O', 0, 1, ROWS-2);
    for (int i = 0; i < (COLS)/2; ++i) {
        CPPUNIT_ASSERT(m_pBoard->IsValidMove(t));
        CPPUNIT_ASSERT(m_pBoard->IsAtRest(t));
        CPPUNIT_ASSERT(m_pBoard->ApplyMove(t) != -1);
        t.ShiftRight();
        t.ShiftRight();
    }
    Tetromino n ('I', 0, 5, ROWS-3);
    CPPUNIT_ASSERT(m_pBoard->IsValidMove(n));
    CPPUNIT_ASSERT(m_pBoard->IsAtRest(n));
    CPPUNIT_ASSERT(m_pBoard->ApplyMove(n) != -1);

    std::vector<int> const* cleared = m_pBoard->ClearRows();
    int clearCount = cleared->size();
    CPPUNIT_ASSERT_EQUAL(2, clearCount);
    CPPUNIT_ASSERT_EQUAL(ROWS-2, cleared->at(0));
    CPPUNIT_ASSERT_EQUAL(ROWS-1, cleared->at(1));
    delete cleared;
    BoardDesc const& desc = m_pBoard->GetBoardDesc();
    for(int i = 0; i < COLS; ++i) {
        for (int j = 0; j < ROWS-1; ++j) {
            CPPUNIT_ASSERT(!desc[i][j]);
        }
    }
    CPPUNIT_ASSERT(!desc[0][ROWS-1]);
    CPPUNIT_ASSERT(!desc[1][ROWS-1]);
    CPPUNIT_ASSERT(!desc[2][ROWS-1]);
    CPPUNIT_ASSERT(desc[3][ROWS-1]);
    CPPUNIT_ASSERT(desc[4][ROWS-1]);
    CPPUNIT_ASSERT(desc[5][ROWS-1]);
    CPPUNIT_ASSERT(desc[6][ROWS-1]);
    CPPUNIT_ASSERT(!desc[7][ROWS-1]);
    CPPUNIT_ASSERT(!desc[8][ROWS-1]);
    CPPUNIT_ASSERT(!desc[9][ROWS-1]);
}

void GameBoardUnitTests::TestClearSpacedRows() {
    Tetromino t ('I', 0, 2, ROWS-1);
    CPPUNIT_ASSERT(m_pBoard->ApplyMove(t) != -1);
    t.ShiftUp();
    t.ShiftUp();
    CPPUNIT_ASSERT(m_pBoard->ApplyMove(t) != -1);
    t.ShiftDown();
    t.ShiftDown();
    t.ShiftRight();
    t.ShiftRight();
    t.ShiftUp();
    CPPUNIT_ASSERT(m_pBoard->ApplyMove(t) != -1);
    t.ShiftUp();
    t.ShiftUp();
    CPPUNIT_ASSERT(m_pBoard->ApplyMove(t) != -1);
    t.ShiftDown();
    t.ShiftDown();
    t.ShiftDown();
    t.ShiftRight();
    t.ShiftRight();
    CPPUNIT_ASSERT(m_pBoard->ApplyMove(t) != -1);
    t.ShiftUp();
    t.ShiftUp();
    CPPUNIT_ASSERT(m_pBoard->ApplyMove(t) != -1);
    t.ShiftDown();
    t.ShiftDown();
    t.ShiftRight();
    t.ShiftRight();
    t.RotateRight();
    t.ShiftUp();
    t.ShiftUp();
    CPPUNIT_ASSERT(m_pBoard->ApplyMove(t) != -1);
    std::vector<int> const* cleared = m_pBoard->ClearRows();
    int clearCount = cleared->size();
    CPPUNIT_ASSERT_EQUAL(0, clearCount);
    delete cleared;
    t.ShiftRight();
    CPPUNIT_ASSERT(m_pBoard->ApplyMove(t) != -1);
    cleared = m_pBoard->ClearRows();
    clearCount = cleared->size();
    CPPUNIT_ASSERT_EQUAL(2, clearCount);
    CPPUNIT_ASSERT_EQUAL(ROWS-3, cleared->at(0));
    CPPUNIT_ASSERT_EQUAL(ROWS-1, cleared->at(1));
    delete cleared;
    BoardDesc const& desc = m_pBoard->GetBoardDesc();
    for(int i = 0; i < COLS; ++i) {
        for (int j = 0; j < ROWS-2; ++j) {
            CPPUNIT_ASSERT(!desc[i][j]);
        }
    }
    CPPUNIT_ASSERT(!desc[0][ROWS-1]);
    CPPUNIT_ASSERT(!desc[1][ROWS-1]);
    CPPUNIT_ASSERT(desc[2][ROWS-1]);
    CPPUNIT_ASSERT(desc[3][ROWS-1]);
    CPPUNIT_ASSERT(desc[4][ROWS-1]);
    CPPUNIT_ASSERT(desc[5][ROWS-1]);
    CPPUNIT_ASSERT(!desc[6][ROWS-1]);
    CPPUNIT_ASSERT(!desc[7][ROWS-1]);
    CPPUNIT_ASSERT(desc[8][ROWS-1]);
    CPPUNIT_ASSERT(desc[9][ROWS-1]);
    CPPUNIT_ASSERT(!desc[0][ROWS-2]);
    CPPUNIT_ASSERT(!desc[1][ROWS-2]);
    CPPUNIT_ASSERT(desc[2][ROWS-2]);
    CPPUNIT_ASSERT(desc[3][ROWS-2]);
    CPPUNIT_ASSERT(desc[4][ROWS-2]);
    CPPUNIT_ASSERT(desc[5][ROWS-2]);
    CPPUNIT_ASSERT(!desc[6][ROWS-2]);
    CPPUNIT_ASSERT(!desc[7][ROWS-2]);
    CPPUNIT_ASSERT(desc[8][ROWS-2]);
    CPPUNIT_ASSERT(desc[9][ROWS-2]);
}

