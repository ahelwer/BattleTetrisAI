#pragma once

#include <core/harmony.hpp>
#include <model/game_state.hpp>

/* *
 * Function GetVarCount
 *
 * Returns the number of weights in the Tetris feature harmony.
 * */
unsigned GetVarCount();

/* *
 * Function GetRanges
 *
 * Returns the range of each weight in the Tetris feature harmony.
 * */
HarmonyRanges const* GetRanges();

/* *
 * Function GetBestHarmony
 *
 * Returns current best harmony.
 * */
Harmony const* GetBestHarmony();

/* *
 * function EvaluateMove
 *
 * Returns the state score for the given Tetris feature harmony.
 * */
float EvaluateMove(GameState const& state, Harmony const& h);

// Feature functions. See implementation for details.
int PileHeight(GameState const& state);
int Holes(GameState const& state);
int ConnectedHoles(GameState const& state);
int RemovedRows(GameState const& state); 
int AltitudeDifference(GameState const& state);
int MaxWellDepth(GameState const& state);
int SumOfAllWells(GameState const& state); 
int LandingHeight(GameState const& state);
int Blocks(GameState const& state);
int WeightedBlocks(GameState const& state);
int RowTransitions(GameState const& state);
int ColTransitions(GameState const& state);
int HighestHole(GameState const& state);
int BlocksAboveHighestHole(GameState const& state);
int PotentialRows(GameState const& state);
int Smoothness(GameState const& state);
int ErodedPieces(GameState const& state);
int RowHoles(GameState const& state);
int HoleDepth(GameState const& state);

