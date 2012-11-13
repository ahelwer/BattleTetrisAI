#pragma once

#include <model/game_board.hpp>

class GameState {
public:
	GameState(GameBoard& board);
	GameBoard& GetBoard();
private:
	GameBoard& m_board;
};

