#pragma once

#include <model/game_board.hpp>

class GameState {
public:
	GameState(GameBoard& board);
	GameBoard& GetBoard();
	GameBoard const& GetBoard() const;
private:
	GameBoard& m_board;
};

