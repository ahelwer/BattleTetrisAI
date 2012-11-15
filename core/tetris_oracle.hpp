#pragma once

#include <util/vector.hpp>
#include <core/harmony.hpp>
#include <model/game_state.hpp>
#include <model/tetromino.hpp>
#include <string>

std::vector<enum Tetromino::Move> const* FindPath(GameState const& state, Tetromino const& source, Tetromino const& target);

Tetromino const* FindBestMove(GameState& state, Harmony const& h);

std::vector<Tetromino> const* FindPossibleMoves(GameState& state);

