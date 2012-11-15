#include <core/tetris_oracle.hpp>
#include <core/game_state_eval.hpp>
#include <cfloat>
#include <deque>
#include <string>

Tetromino const* FindBestMove(GameState& state, Harmony const& h) {
    std::vector<Tetromino> const* possible = FindPossibleMoves(state);
    int maxIdx = -1;
    float maxScore = -1.0*FLT_MAX;
    //std::cout << std::endl << "found: " << possible->size() << std::endl;
    for (int i = 0; i < possible->size(); ++i) {
        state.PushMove(possible->at(i));
        float result = EvaluateMove(state, h);
        //std::cout << result << " " << maxScore << std::endl;
        state.PopMove();
        if (result > maxScore) {
            maxIdx = i;
            maxScore = result;
        }
    }
    //std::cout << std::endl;
    Tetromino* ret = NULL;
    if (maxIdx != -1)
        ret = new Tetromino(possible->at(maxIdx));
    delete possible;
    return ret;
}

std::vector<Tetromino> const* Neighbours(Tetromino const& current) {
    std::vector<Tetromino>* adj = new std::vector<Tetromino>();

    // right
    Tetromino n = current;
    n.ShiftRight();
    for (int i = 0; i < 4; ++i) {
        n.RotateRight();
        adj->push_back(n);
    }

    // down
    n = current;
    n.ShiftDown();
    for (int i = 0; i < 4; ++i) {
        n.RotateRight();
        adj->push_back(n);
    }

    // left
    n = current;
    n.ShiftLeft();
    for (int i = 0; i < 4; ++i) {
        n.RotateRight();
        adj->push_back(n);
    }
    
    return adj;
}

std::vector<Tetromino> const* FindPossibleMoves(GameState& state) {
    std::vector<Tetromino>* moves = new std::vector<Tetromino>();
    GameBoard& board = state.GetBoard();

    // Initializes breadth-first search
    // visited [Column][Row][Rotation]
    bool v[10][20][4];
    bool* vRaw = &(v[0][0][0]);
    for (int i = 0; i < 10*20*4; ++i)
        vRaw[i] = false;
    std::deque<Tetromino> Q;
    Tetromino s (state.GetPieceInPlay());
    Q.push_back(s);
    v[s.GetX()][s.GetY()][s.GetOrient()] = true;

    // Begins breadth-first search
    while (!Q.empty()) {
        Tetromino c = Q.front();
        Q.pop_front();

        // Check adjacent
        std::vector<Tetromino> const* adj = Neighbours(c);
        for (int i = 0; i < adj->size(); ++i) {
            Tetromino n = adj->at(i);
            if (board.IsValidMove(n) && !v[n.GetX()][n.GetY()][n.GetOrient()]) {
                v[n.GetX()][n.GetY()][n.GetOrient()] = true;
                Q.push_back(n);
                if (board.IsAtRest(n))
                    moves->push_back(n);
            }
        }
        delete adj;
    }

    return moves;
}

