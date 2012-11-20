#pragma once

#include <core/harmony.hpp>
#include <trainer/generated_game.hpp>

/* *
 * Class TetrisPointsEarned
 *
 * Objective function used to train Tetris AI.
 * */
class TetrisPointsEarned : public ObjectiveFunction {
public:
    TetrisPointsEarned(GeneratedGame const& generator);
    float operator() (Harmony const& h) const;
private:
    GeneratedGame const& m_generator;
};

