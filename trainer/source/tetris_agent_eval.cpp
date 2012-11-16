#include <trainer/tetris_agent_eval.hpp>
#include <core/tetris_oracle.hpp>
#include <model/game_state.hpp>

TetrisPointsEarned::TetrisPointsEarned(GeneratedGame const& generator)
    : m_generator(generator)
{ }

float TetrisPointsEarned::operator() (Harmony const& h) const {
    float pointsEarned = 0.0;
    Tetromino const& tetInPlay = m_generator.GetPiece(0);
    std::queue<Tetromino> const* queue = m_generator.GetQueue(0);
    GameState state(*queue, tetInPlay);
    delete queue;
    int gameLength = m_generator.GameLength();
    for (int i = 0; i < gameLength; ++i ) {
        Tetromino const* best = FindBestMove(state, h);
        if (best != NULL) {
            int cleared = state.ApplyMove(*best);
            if (cleared != -1) {
                if (cleared == 1)
                    pointsEarned += 1.0;
                else if (cleared == 2)
                    pointsEarned += 2.0;
                else if (cleared == 3)
                    pointsEarned += 5.0;
                else if (cleared == 4)
                    pointsEarned += 8.0;
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
    return pointsEarned;
}
