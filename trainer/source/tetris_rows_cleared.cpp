#include <trainer/tetris_rows_cleared.hpp>
#include <core/tetris_oracle.hpp>
#include <model/game_state.hpp>

TetrisRowsCleared::TetrisRowsCleared(GeneratedGame const& generator)
    : m_generator(generator)
{ }

float TetrisRowsCleared::operator() (Harmony const& h) const {
    float rowsCleared = 0.0;
    Tetronimo const& tetInPlay = m_generator.GetPiece(0);
    std::vector<Tetronimo> const* queue = m_generator.GetQueue(0);
    GameState state(*queue, tetInPlay);
    delete queue;
    int gameLength = m_generator.GameLength();
    for (int i = 0; i < gameLength; ++i ) {
        Tetronimo const* best = FindBestMove(state, h);
        if (best != NULL) {
            // Apply move to state
            delete best;
        }
    }
    return rowsCleared;
}
