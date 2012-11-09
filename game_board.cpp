#include "game_board.hpp"
#include "main.hpp"
#include <string>

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

