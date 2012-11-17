#include <trainer/tetris_agent_eval.hpp>
#include <core/tetris_oracle.hpp>
#include <model/game_state.hpp>

TetrisPointsEarned::TetrisPointsEarned(GeneratedGame const& generator)
    : m_generator(generator)
{ }

float TetrisPointsEarned::operator() (Harmony const& h) const {
    float pointsEarned = 0.0;
    int rowsCleared = 0;
    Tetromino const& tetInPlay = m_generator.GetPiece(0);
    std::vector<Tetromino> const* queue = m_generator.GetQueue(0);
    GameState state(*queue, tetInPlay);
    delete queue;
    int gameLength = m_generator.GameLength();
    int survivalCount = 0;
    bool died = false;
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
                rowsCleared += cleared;
            }
            else {
                std::cout << "ERROR: best move failed." << std::endl;
            }
            delete best;
        }
        else {
            died = true;
            break;
        }
        ++survivalCount;
        int chance = rand() % 2;
        int nextIdx = i+1+chance;
        if (nextIdx < gameLength) {
            Tetromino nextTet = m_generator.GetPiece(nextIdx);
            state.SetPieceInPlay(&nextTet);
            queue = m_generator.GetQueue(nextIdx);
            state.SetQueueInPlay(*queue);
            delete queue;
        }
    }
    // death penalty
    if (died)
        pointsEarned /= 2.0;
    std::cout << "Rows cleared: " << rowsCleared << ", ";
    if (died)
        std::cout << "did not survive." << std::endl;
    else
        std::cout << "survived." << std::endl;
    return pointsEarned;
}
