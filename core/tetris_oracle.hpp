#pragma once

#include <util/vector.hpp>
#include <core/harmony.hpp>
#include <model/game_state.hpp>
#include <model/tetronimo.hpp>

Tetronimo const* FindBestMove(GameState& state, Harmony const& h);

std::vector<Tetronimo> const* FindPossibleMoves(GameState& state);

