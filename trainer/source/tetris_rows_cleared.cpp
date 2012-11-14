#include <trainer/tetris_rows_cleared.hpp>

TetrisRowsCleared::TetrisRowsCleared(GeneratedGame const& generator)
    : m_generator(generator)
{ }

float TetrisRowsCleared::operator() (Harmony const& h) const {
    float rowsCleared = 0.0;
    int gameLength = m_generator.GameLength();
    for (int i = 0; i < gameLength; ++i ) {
        Tetronimo const& tCurrent = m_generator.GetPiece(i);
        Tetronimo const& tNext = m_generator.GetPiece(i+1);
        // Evaluate & pick moves with weights, record cleared rows 
    }
    return rowsCleared;
}
