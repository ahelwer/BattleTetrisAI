#pragma once

#include <core/harmony.hpp>
#include <trainer/generated_game.hpp>

class TetrisRowsCleared : public ObjectiveFunction {
public:
    TetrisRowsCleared(GeneratedGame const& generator);
    float operator() (Harmony const& h) const;
private:
    GeneratedGame const& m_generator;
};

