#include <core/tetris_oracle.hpp>
#include <core/game_state_eval.hpp>
#include <cfloat>

Tetromino const* FindBestMove(GameState& state, Harmony const& h) {
    std::vector<Tetromino> const* possible = FindPossibleMoves(state);
    if (possible->size() == 0)
        return NULL;
    int maxIdx = -1;
    float maxScore = FLT_MIN;
    for (int i = 0; i < possible->size(); ++i) {
        state.PushMove(possible->at(i));
        float result = EvaluateMove(state, h);
        state.PopMove();
        if (result > maxScore) {
            maxIdx = i;
            maxScore = result;
        }
    }
    Tetromino* ret = NULL;
    if (maxIdx != -1)
        ret = new Tetromino(possible->at(maxIdx));
    delete possible;
    return ret;
}

std::vector<Tetromino> const* FindPossibleMoves(GameState& state) {
    std::vector<Tetromino>* moves = new std::vector<Tetromino>();
    return moves;
}

