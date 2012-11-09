#include "game_board.hpp"
#include "main.hpp"
#include <cstring>

GameBoard::GameBoard()
    : m_desc(new char[BOARD_DESC_SIZE]), 
        m_board(COLS, std::vector<bool>(ROWS, false))
{ }



GameBoard::~GameBoard() {
    delete[] m_desc;
}

bool GameBoard::HasChanged(char const* desc) const {
    return (memcmp((void*)desc, (void*)m_desc, BOARD_DESC_SIZE) == 0);
}

void GameBoard::Translate(char const* desc) {
	for (int i = 0; i < BOARD_DESC_SIZE; ++i) {
		char hex = desc[i];
		char byte = 0;
		if (hex & (1 << 5)) // hex > 64, so is letter
			byte = (hex - 55); // A is char code 65, add 10
		else
			byte = (hex - 48); // 0 is char code 48
		// Reads hex bitmask into board
		int x = i*4 % COLS;
		int y = i*4 / COLS;
		m_board[x][y] = (byte & 1);
		x = (i*4+1) % COLS;
		y = (i*4+1) / COLS;
		m_board[x][y] = (byte & 2);
		x = (i*4+2) % COLS;
		y = (i*4+2) / COLS;
		m_board[x][y] = (byte & 4);
		x = (i*4+3) % COLS;
		y = (i*4+3) / COLS;
		m_board[x][y] = (byte & 8);
	}
}

