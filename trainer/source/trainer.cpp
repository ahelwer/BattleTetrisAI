#include <trainer/trainer.hpp>
#include <core/harmony_search.hpp>

Trainer::Trainer(int gameLength, int iterations)
    : m_gameLength(gameLength), m_iterations(iterations),
        m_generator(gameLength)
{ }

Harmony* Trainer::Execute() {
    //HarmonySearch search (
    for (int i = 0; i < m_iterations; ++i) {
        m_generator.GenerateNewGame();
    }
}

