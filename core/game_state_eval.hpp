#pragma once

#include <core/harmony.hpp>
#include <model/game_state.hpp>

/* *
 * Function GetVarCount
 *
 * Returns the number of weights in the Tetris feature harmony.
 * */
int GetVarCount();

/* *
 * Function GetRanges
 *
 * Returns the range of each weight in the Tetris feature harmony.
 * */
HarmonyRanges const* GetRanges();

/* *
 * function EvaluateMove
 *
 * Returns the state score for the given Tetris feature harmony.
 * */
float EvaluateMove(GameState const& state, Harmony const& h);

