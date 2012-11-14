#pragma once

#include <util/vector.hpp>
#include <core/harmony.hpp>
#include <model/game_state.hpp>
#include <model/tetromino.hpp>

Tetromino const* FindBestMove(GameState& state, Harmony const& h);

std::vector<Tetromino> const* FindPossibleMoves(GameState& state);

