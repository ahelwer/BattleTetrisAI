#pragma once

#include <core/harmony.hpp>
#include <model/game_state.hpp>

/* *
 * Evaluates current game state and recommends best move
 * */
class GameStateEval {
public:
	GameStateEval(GameState& state);
private:
	GameState& m_state;
};

