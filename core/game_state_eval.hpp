#pragma once

#include <core/harmony.hpp>
#include <model/game_state.hpp>


int GetVarCount();
HarmonyRanges const* GetRanges();

float EvaluateMove(GameState const& state, Harmony const& h);

int PileHeight(GameState const& state);
int Holes(GameState const& state);
int ConnectedHoles(GameState const& state);
int RemovedRows(GameState const& state); // TODO
int AltitudeDifference(GameState const& state);
int MaxWellDepth(GameState const& state);
int SumOfAllWells(GameState const& state);  // TODO
int LandingHeight(GameState const& state);  // TODO
int Blocks(GameState const& state);
int WeightedBlocks(GameState const& state);
int RowTransitions(GameState const& state); // TODO
int ColTransitions(GameState const& state); // TODO
int HighestHole(GameState const& state);
int BlocksAboveHighestHole(GameState const& state);
int PotentialRows(GameState const& state);  // TODO
int Smoothness(GameState const& state);
int ErodedPieces(GameState const& state);   // TODO
int RowHoles(GameState const& state);   // TODO
int HoleDepth(GameState const& state);  // TODO

