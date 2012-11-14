#include <core/game_state_eval.hpp>
#include <util/constants.hpp>

// Declarations of all feature functions
int PileHeight(GameState const& state);
int Holes(GameState const& state);
int ConnectedHoles(GameState const& state);
int RemovedRows(GameState const& state); // TODO
int AltitudeDifference(GameState const& state);
int MaxWellDepth(GameState const& state);
int SumOfAllWells(GameState const& state);  // TODO
int LandingHeight(GameState const& state);  // TODO
int Blocks(GameState const& state);
int WeightedBlocks(GameState const& state);
int RowTransitions(GameState const& state); // TODO
int ColTransitions(GameState const& state); // TODO
int HighestHole(GameState const& state);
int BlocksAboveHighestHole(GameState const& state);
int PotentialRows(GameState const& state);  // TODO
int Smoothness(GameState const& state);
int ErodedPieces(GameState const& state);   // TODO
int RowHoles(GameState const& state);   // TODO
int HoleDepth(GameState const& state);  // TODO


int GetVarCount() {
    return 19;
}

HarmonyRanges const* GetRanges() {
    HarmonyRanges* ranges = new HarmonyRanges();
    for (int i = 0; i < GetVarCount(); ++i)
        ranges->push_back(std::pair<float, float>(-100.0, 100.0));
    return ranges;
}

float EvaluateMove(GameState const& state, Harmony const& h) {
    return 0.0;
}

/* *
 * Function PileHeight
 *
 * Row of the topmost occupied square on the board.
 * */
int PileHeight(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    int minWellDepth = ROWS;
    for (int i = 0; i < COLS; ++i)
        minWellDepth = std::min(minWellDepth, board.WellDepth(i));
    return (ROWS - minWellDepth);
}

/* *
 * Function Holes
 *
 * The number of gaps with at least one occupied cell above them.
 * */
int Holes(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    BoardDesc const& desc = board.GetBoardDesc();
    int holeCount = 0;
    for (int i = 0; i < COLS; ++i) {
        bool hitTop = false;
        for (int j = 0; j < ROWS; ++j) {
            if (desc[i][j] && !hitTop)
                hitTop = true;
            else if (!desc[i][j] && hitTop)
                ++holeCount;
        }
    }
    return holeCount;
}

/* *
 * Function ConnectedHoles
 *
 * The number of connected gaps with at least one occupied cell above them.
 * */
int ConnectedHoles(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    BoardDesc const& desc = board.GetBoardDesc();
    int holeCount = 0;
    for (int i = 0; i < COLS; ++i) {
        bool hitTop = false;
        bool inHole = false;
        for (int j = 0; j < ROWS; ++j) {
            if (desc[i][j] && !hitTop)
                hitTop = true;
            else if (desc[i][j] && hitTop && inHole)
                inHole = false;
            else if (!desc[i][j] && hitTop && !inHole) {
                ++holeCount;
                inHole = true;
            }
        }
    }
    return holeCount;
}

/* *
 * Function RemovedRows
 *
 * The number of rows cleared by the last step.
 * */
int RemovedRows(GameState const& state) {
    std::vector<int> const& cleared = state.LastClearedRows();
    return cleared.size();
}

int AltitudeDifference(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    int minWellDepth = ROWS;
    int maxWellDepth = 0;
    for (int i = 0; i < COLS; ++i) {
        int wellDepth = board.WellDepth(i);
        minWellDepth = std::min(minWellDepth, wellDepth);
        maxWellDepth = std::min(maxWellDepth, wellDepth);
    }
    return ((ROWS - minWellDepth) - (ROWS - maxWellDepth));
}

int MaxWellDepth(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    int maxWellDepth = 0;
    for (int i = 0; i < COLS; ++i)
        maxWellDepth = std::min(maxWellDepth, board.WellDepth(i));
    return (ROWS - maxWellDepth);
}

int SumOfAllWells(GameState const& state) {
    // Sum of all wells based on adjacent columns
    return 0;
}

int LandingHeight(GameState const& state) {
    // Height at which last tetronimo was placed
    return 0;
}

int Blocks(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    BoardDesc const& desc = board.GetBoardDesc();
    int blockCount = 0;    
    for (int j = 0; j < ROWS; ++j) {
        for (int i = 0; i < COLS; ++i) {
            if (desc[i][j])
                ++blockCount;
        }
    }
    return blockCount;
}

int WeightedBlocks(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    BoardDesc const& desc = board.GetBoardDesc();
    int weighted = 0;    
    for (int j = 0; j < ROWS; ++j) {
        for (int i = 0; i < COLS; ++i) {
            if (desc[i][j])
                weighted += (ROWS-j);    
        }
    }
    return weighted;
}

int RowTransitions(GameState const& state) {
    // Counts row transitions
    return 0;
}

int ColTransitions(GameState const& state) {
    // Counts col transitions
    return 0;
}

int HighestHole(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    BoardDesc const& desc = board.GetBoardDesc();
    int highest = 0;
    for (int i = 0; i < COLS; ++i) {
        bool hitTop = false;
        for (int j = 0; j < ROWS; ++j) {
            if (desc[i][j] && !hitTop)
                hitTop = true;
            if (!desc[i][j] && hitTop) {
                highest = std::max(highest, ROWS-j);
                break;
            }
        }
    }
    return highest;
}

int BlocksAboveHighestHole(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    BoardDesc const& desc = board.GetBoardDesc();
    int highest = 0;
    int blocksAboveCount = 0;
    for (int i = 0; i < COLS; ++i) {
        bool hitTop = false;
        int blocksHit = 0;
        for (int j = 0; j < ROWS; ++j) {
            if (desc[i][j])
                ++blocksHit;
            if (desc[i][j] && !hitTop)
                hitTop = true;
            if (!desc[i][j] && hitTop) {
                if ((ROWS-j) == highest && blocksHit > blocksAboveCount) {
                    blocksAboveCount = blocksHit; 
                }
                if ((ROWS-j) > highest) {
                    highest = (ROWS-j);
                    blocksAboveCount = blocksHit;
                }
                break;
            }
        }
    }
    return blocksAboveCount;
}

int PotentialRows(GameState const& state) {
    return 0;
}

int Smoothness(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    int smoothness = 0;
    for (int i = 0; i < COLS-1; ++i)
        smoothness += abs(board.WellDepth(i)-board.WellDepth(i+1));
    return smoothness;
}

int ErodedPieces(GameState const& state) {
    return 0;
}

int RowHoles(GameState const& state) {
    return 0;
}

int HoleDepth(GameState const& state) {
    return 0;
}

