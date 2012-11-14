#include <test/game_board_unit_tests.hpp>
#include <model/tetronimo.hpp>
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
            bool expected = ((i+1)%2);
            CPPUNIT_ASSERT_EQUAL(expected*ROWS, m_pBoard->WellDepth(i));
            CPPUNIT_ASSERT_EQUAL(!expected, desc[i][j]);
        }
    }
    CPPUNIT_ASSERT(!m_pBoard->HasChanged(shortDesc));
    delete[] shortDesc;
}

void GameBoardUnitTests::TestApplyPiece() {
    Tetronimo t1 ('O', 0, 5, 1);
    CPPUNIT_ASSERT(m_pBoard->PushMove(t1));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(3));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(4));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(5));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(6));
    m_pBoard->PopMove();

    Tetronimo t2 ('O', 0, COLS-1, 1);
    CPPUNIT_ASSERT(m_pBoard->PushMove(t2));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(COLS-3));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(COLS-2));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(COLS-1));
    m_pBoard->PopMove();

    Tetronimo t3 ('O', 0, 1, 1);
    CPPUNIT_ASSERT(m_pBoard->PushMove(t3));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(0));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(1));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(2));
    m_pBoard->PopMove();

    Tetronimo t4 ('O', 0, 0, 1);
    CPPUNIT_ASSERT(!m_pBoard->PushMove(t4));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(0));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(1));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(2));

    Tetronimo t5 ('O', 0, COLS, 1);
    CPPUNIT_ASSERT(!m_pBoard->PushMove(t5));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(COLS-1));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(COLS-2));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(COLS-3));
}

void GameBoardUnitTests::TestApplyMultiplePieces() {
    Tetronimo t1 ('O', 0, 5, 1);
    CPPUNIT_ASSERT(m_pBoard->PushMove(t1));
    Tetronimo t2 ('O', 0, 4, 1);
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

    Tetronimo t3 ('J', 1, 3, 1);
    CPPUNIT_ASSERT(m_pBoard->PushMove(t3));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(2));
    CPPUNIT_ASSERT_EQUAL(ROWS-3, m_pBoard->WellDepth(3));
    CPPUNIT_ASSERT_EQUAL(ROWS-3, m_pBoard->WellDepth(4));
    CPPUNIT_ASSERT_EQUAL(ROWS-2, m_pBoard->WellDepth(5));
    CPPUNIT_ASSERT_EQUAL(ROWS, m_pBoard->WellDepth(6));
    m_pBoard->PopMove();
    m_pBoard->PopMove();
}

