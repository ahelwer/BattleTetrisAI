#include <model/game_state.hpp>

GameState::GameState()
    : m_board()
{ }

GameBoard& GameState::GetBoard() {
    return m_board;
}

GameBoard const& GameState::GetBoard() const {
    return m_board;
}

