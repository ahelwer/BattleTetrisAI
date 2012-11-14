#pragma once

#include <trainer/generated_game.hpp>
#include <core/harmony.hpp>

class Trainer {
public:
    Trainer(int gameLength, int iterations);
    Harmony* Execute();
private:
    int m_gameLength;
    int m_iterations;
    GeneratedGame m_generator;
};

