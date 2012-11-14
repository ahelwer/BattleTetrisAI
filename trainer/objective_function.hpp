#pragma once

#include <core/harmony.hpp>
#include <trainer/generated_game.hpp>

class TetrisRowsCleared : public ObjectiveFunction {
public:
    TetrisRowsCleared(GeneratedGame& generator);
    float Execute(Harmony const& h);
private:
    GeneratedGame& m_generator;
};

