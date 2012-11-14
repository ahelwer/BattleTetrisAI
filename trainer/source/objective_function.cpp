#include <trainer/objective_function.hpp>

TetrisRowsCleared::TetrisRowsCleared(GeneratedGame& generator)
    : m_generator(generator)
{ }

float TetrisRowsCleared::Execute(Harmony const& h) {
    float rowsCleared = 0.0;
    while (m_generator.HasNext()) {
        Tetronimo const& tCurrent = m_generator.GetNext();
        Tetronimo const& tNext = m_generator.PeekNext();
        // Evaluate & pick moves with weights, record cleared rows 
    }
    m_generator.Reset();
    return rowsCleared;
}
