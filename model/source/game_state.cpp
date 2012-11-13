#include <model/game_state.hpp>

GameState::GameState(GameBoard& board)
	: m_board(board)
{ }

GameBoard& GameState::GetBoard() {
	return m_board;
}

GameBoard const& GameState::GetBoard() const {
	return m_board;
}

