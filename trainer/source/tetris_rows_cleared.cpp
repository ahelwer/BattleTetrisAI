#include <trainer/tetris_rows_cleared.hpp>
#include <core/tetris_oracle.hpp>
#include <model/game_state.hpp>

TetrisRowsCleared::TetrisRowsCleared(GeneratedGame const& generator)
    : m_generator(generator)
{ }

float TetrisRowsCleared::operator() (Harmony const& h) const {
    float rowsCleared = 0.0;
    Tetromino const& tetInPlay = m_generator.GetPiece(0);
    std::vector<Tetromino> const* queue = m_generator.GetQueue(0);
    GameState state(*queue, tetInPlay);
    delete queue;
    int gameLength = m_generator.GameLength();
    for (int i = 0; i < gameLength; ++i ) {
        Tetromino const* best = FindBestMove(state, h);
        if (best != NULL) {
            int cleared = state.ApplyMove(*best);
            if (cleared != -1) {
                rowsCleared += static_cast<float>(cleared);
            }
            else {
                std::cout << "ERROR: best move failed." << std::endl;
            }
            delete best;
        }
        else {
            break;
        }
        if (i < gameLength-1) {
            Tetromino nextTet = m_generator.GetPiece(i+1);
            state.SetPieceInPlay(&nextTet);
        }
    }
    return rowsCleared;
}
