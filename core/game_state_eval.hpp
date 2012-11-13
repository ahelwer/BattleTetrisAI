#pragma once

#include <core/harmony.hpp>
#include <model/game_state.hpp>

/* *
 * Evaluates current game state and recommends best move
 * */
class GameStateEval {
public:
	GameStateEval(GameState& state);
	float Evaluate(Harmony const& h) const;
private:
	int PileHeight() const;
	int Holes() const;
	int ConnectedHoles() const;
	int RemovedRows() const;	// TODO
	int AltitudeDifference() const;
	int MaxWellDepth() const;
	int SumOfAllWells() const;	// TODO
	int LandingHeight() const;	// TODO
	int Blocks() const;
	int WeightedBlocks() const;
	GameState& m_state;
};

