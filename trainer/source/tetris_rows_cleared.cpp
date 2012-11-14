#include <trainer/tetris_rows_cleared.hpp>
#include <model/game_state.hpp>

TetrisRowsCleared::TetrisRowsCleared(GeneratedGame const& generator)
    : m_generator(generator)
{ }

float TetrisRowsCleared::operator() (Harmony const& h) const {
    float rowsCleared = 0.0;
    GameState state;
    int gameLength = m_generator.GameLength();
    for (int i = 0; i < gameLength; ++i ) {
        Tetronimo const& tCurrent = m_generator.GetPiece(i);
        //Tetronimo const& tNext = m_generator.GetPiece(i+1);
        // Tetronimo const* best = FindBestMove(state, h);
    }
    return rowsCleared;
}
