#pragma once

#include <util/vector.hpp>
#include <core/harmony.hpp>
#include <model/game_state.hpp>
#include <model/tetromino.hpp>
#include <string>

/* *
 * Function FindPath
 *
 * Returns sequence of moves to get from source position to target.
 * */
PathSequence const* FindPath(GameState const& state, Tetromino const& source, 
                                Tetromino const& target);

/* *
 * Function FindBestMove
 *
 * Evaluates game state and returns best move.
 * */
Tetromino const* FindBestMove(GameState& state, Harmony const& h);

/* *
 * Function FindPossibleMoves
 *
 * Finds all moves available to the piece currently in play.
 * */
std::vector<Tetromino> const* FindPossibleMoves(GameState& state);

