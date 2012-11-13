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

}

void GameBoardUnitTests::TestApplyMultiplePieces() {

}

