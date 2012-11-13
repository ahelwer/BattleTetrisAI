#include <test/game_board_unit_tests.hpp>

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

void GameBoardUnitTests::TestTranslateEmpty() {

}

void GameBoardUnitTests::TestTranslateFull() {

}

void GameBoardUnitTests::TestTranslateComplex() {

}

void GameBoardUnitTests::TestApplyPiece() {

}

void GameBoardUnitTests::TestApplyMultiplePieces() {

}

