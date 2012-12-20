#include <core/tetris_oracle.hpp>
#include <core/game_state_eval.hpp>
#include <cfloat>
#include <deque>
#include <string>
#include <algorithm>

float BestScore(GameState& state, Harmony const& h) {
    std::vector<Tetromino> const* possible = FindPossibleMoves(state);
    float maxScore = -1.0*FLT_MAX;
    for (unsigned i = 0; i < possible->size(); ++i) {
        GameState next = state;
        int cleared = next.ApplyMove(possible->at(i));
        if (cleared == -1) {
            continue;
        }
        float result = EvaluateMove(next, h);
        maxScore = std::max(result, maxScore);
    }
    if (possible != NULL) {
        delete possible;
        possible = NULL;
    }
    return maxScore;
}

Tetromino const* FindBestMove(GameState& state, Harmony const& h) {
    std::vector<Tetromino> const* possible = FindPossibleMoves(state);
    if (possible == NULL) {
        return NULL;
    }
    int maxIdx = -1;
    float maxScore = -1.0*FLT_MAX;
    for (unsigned i = 0; i < possible->size(); ++i) {
        GameState next = state;
        int cleared = next.ApplyMove(possible->at(i));
        if (cleared == -1) {
            continue;
        }
        float result = EvaluateMove(next, h);
        /*
        int feedSize = std::min(next.QueuedPieceCount(), 2);
        for (unsigned i = 0; i < feedSize; ++i) {
            bool success = next.FeedFromQueue(i);
            if (success) {
                result += ((BestScore(next, h) / (float)feedSize));
            }
        }
        */
        if (result > maxScore) {
            maxIdx = i;
            maxScore = result;
        }
    }
    Tetromino* ret = NULL;
    if (maxIdx != -1)
        ret = new Tetromino(possible->at(maxIdx));
    if (possible != NULL) {
        delete possible;
        possible = NULL;
    }
    return ret;
}

std::vector<Tetromino> const* Neighbours(Tetromino const& current) {
    std::vector<Tetromino>* adj = new std::vector<Tetromino>();

    // shift right
    Tetromino n = current;
    n.ShiftRight();
    adj->push_back(n);

    // shift left
    n = current;
    n.ShiftLeft();
    adj->push_back(n);
    
    // rotate left
    n = current;
    n.RotateLeft();
    adj->push_back(n);
    
    // rotate right
    n = current;
    n.RotateRight();
    adj->push_back(n);

    // shift down
    n = current;
    n.ShiftDown();
    adj->push_back(n);

    return adj;
}

std::vector<Tetromino> const* FindPossibleMoves(GameState& state) {
    std::vector<Tetromino>* moves = new std::vector<Tetromino>();
    GameBoard& board = state.GetBoard();
    Tetromino const* inPlay = state.GetPieceInPlay();
    if (inPlay == NULL || !board.IsValidMove(*inPlay))
        return moves;

    // Initializes breadth-first search
    // visited [Column][Row][Rotation]
    bool v[10][20][4];
    bool* vRaw = &(v[0][0][0]);
    for (unsigned i = 0; i < 10*20*4; ++i) {
        vRaw[i] = false;
    }
    std::deque<Tetromino> Q;
    Tetromino s (*(state.GetPieceInPlay()));
    Q.push_back(s);
    v[s.GetX()][s.GetY()][s.GetOrient()] = true;

    // Begins breadth-first search
    while (!Q.empty()) {
        Tetromino c = Q.front();
        Q.pop_front();

        // Check adjacent
        std::vector<Tetromino> const* adj = Neighbours(c);
        for (unsigned i = 0; i < adj->size(); ++i) {
            Tetromino n = adj->at(i);
            if (board.IsValidMove(n) && !v[n.GetX()][n.GetY()][n.GetOrient()]) {
                v[n.GetX()][n.GetY()][n.GetOrient()] = true;
                Q.push_back(n);
                if (board.IsAtRest(n))
                    moves->push_back(n);
            }
        }
        if (adj != NULL) {
            delete adj;
            adj = NULL;
        }
    }

    return moves;
}

enum Tetromino::Move GetTransition(Tetromino const& source, Tetromino const& target) {
    // shift right?
    Tetromino n = source;
    n.ShiftRight();
    if (n == target)
        return Tetromino::right;

    // shift left?
    n = source;
    n.ShiftLeft();
    if (n == target)
        return Tetromino::left;
    
    // shift down?
    n = source;
    n.ShiftDown();
    if (n == target)
        return Tetromino::down;
    
    // rotate left?
    n = source;
    n.RotateLeft();
    if (n == target)
        return Tetromino::lrotate;

    // rotate right?
    n = source;
    n.RotateRight();
    if (n == target)
        return Tetromino::rrotate;

    return Tetromino::nmoves;
}

Tetromino GetPrevious(Tetromino const& current, enum Tetromino::Move move) {
    // Was shifted right?
    if (move == Tetromino::right) {
        Tetromino n = current;
        n.ShiftLeft();
        return n;
    }

    // Was shifted left?
    if (move == Tetromino::left) {
        Tetromino n = current;
        n.ShiftRight();
        return n;
    }
    
    // Was shifted down?
    if (move == Tetromino::down) {
        Tetromino n = current;
        n.ShiftUp();
        return n;
    }

    // Was rotated left?
    if (move == Tetromino::lrotate) {
        Tetromino n = current;
        n.RotateRight();
        return n;
    }

    // Was rotated right?
    if (move == Tetromino::rrotate) {
        Tetromino n = current;
        n.RotateLeft();
        return n;
    }

    return current;
}

Tetromino const* BFS(GameState const& state, Tetromino const& s, 
                        Tetromino const& t, bool v[10][20][4], 
                        Tetromino::Move p[10][20][4], bool& reached) {
    GameBoard const& board = state.GetBoard();
    std::deque<Tetromino> Q;
    Q.push_back(s);
    v[s.GetX()][s.GetY()][s.GetOrient()] = true;

    // Begins breadth-first search
    while (!Q.empty()) {
        Tetromino c = Q.front();
        Q.pop_front();

        // Checks if can hot drop on target
        Tetromino dropped = c;
        while (!board.IsAtRest(dropped) && board.IsValidMove(dropped))
            dropped.ShiftDown();
        if (dropped == t) {
            reached = true;
            v[t.GetX()][t.GetY()][t.GetOrient()] = true;
            p[t.GetX()][t.GetY()][t.GetOrient()] = Tetromino::drop;
            return (new Tetromino(c));
        }

        // Check adjacent
        std::vector<Tetromino> const* adj = Neighbours(c);
        for (unsigned i = 0; i < adj->size(); ++i) {
            Tetromino n = adj->at(i);
            if (board.IsValidMove(n) && !v[n.GetX()][n.GetY()][n.GetOrient()]) {
                v[n.GetX()][n.GetY()][n.GetOrient()] = true;
                p[n.GetX()][n.GetY()][n.GetOrient()] = GetTransition(c, n);
                Q.push_back(n);
                
                // Checks if reached target naturally
                if (n == t) {
                    reached = true;
                    if (adj != NULL) {
                        delete adj;
                        adj = NULL;
                    }
                    return NULL;
                }
            }
        }
        if (adj != NULL) {
            delete adj;
            adj = NULL;
        }
    }
    reached = false;
    return NULL;
}

PathSequence const* FindPath(GameState const& state, 
                                Tetromino const& source, 
                                Tetromino const& target) {
    if (source == target) {
        return (new PathSequence());
    }

    GameBoard const& board = state.GetBoard();
    if (!board.IsValidMove(source) || !board.IsValidMove(target) ||
            source.GetType() != target.GetType())
        return NULL;

    // Initializes breadth-first search
    // visited [Column][Row][Rotation]
    bool v[10][20][4];
    Tetromino::Move p[10][20][4];
    bool* vRaw = &(v[0][0][0]);
    Tetromino::Move* pRaw = &(p[0][0][0]);
    for (int i = 0; i < 10*20*4; ++i) {
        vRaw[i] = false;
        pRaw[i] = Tetromino::nmoves;
    }

    bool reached = false;
    // last == NULL if no drop, otherwise set to state dropped from
    Tetromino const* last = BFS(state, source, target, v, p, reached);
    if (!reached) {
        if (last != NULL) {
            delete last;
            last = NULL;
        }
        //std::cout << "ERROR: Could not reach target" << std::endl;
        return NULL;
    }

    std::vector<enum Tetromino::Move> sequence;
    Tetromino c = target;
    // Pushes drop move to sequence and sets current to pre-drop
    if (last != NULL) {
        sequence.push_back(Tetromino::drop);
        c = *last;
        delete last;
        last = NULL;
    }
    while (c != source && v[c.GetX()][c.GetY()][c.GetOrient()]) {
        enum Tetromino::Move move = p[c.GetX()][c.GetY()][c.GetOrient()];
        sequence.push_back(move); 
        c = GetPrevious(c, move);
    }

    // Reverses order of sequence
    PathSequence* moves = new PathSequence();
    for (int i = sequence.size()-1; i >= 0; --i)
        moves->push_back(sequence.at(i));
    // Append drop on to end if last move is not drop
    if (moves->back() != Tetromino::drop)
        moves->push_back(Tetromino::drop);

    return moves;
}

