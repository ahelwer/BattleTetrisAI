#include <core/game_state_eval.hpp>
#include <util/constants.hpp>

int GetVarCount() {
    return 19;
}

HarmonyRanges const* GetRanges() {
    HarmonyRanges* ranges = new HarmonyRanges();
    for (int i = 0; i < GetVarCount(); ++i)
        ranges->push_back(std::pair<float, float>(-1.0, 1.0));
    return ranges;
}

Harmony const* GetBestHarmony() {
    int weightCount = GetVarCount();
    float weights[] = {0.831252, -0.833718, -0.484039, -0.669239, -0.74901, 
                        -0.445552, -0.129955, -0.0353312, 0.468689, -0.115297, 
                        -0.604853, -0.436222, 0.760341, -0.0589, -0.79345, 
                        0.102076, 0.941241, -0.154894, -0.437886};
    /*
    // Original weights
    float weights[] = {0.413627, -0.723112, -0.214118, 0.0572252, 
                        0.666599, -0.758947, 0.297272, 0.112221, 
                        -0.571268, -0.255358, -0.63998, -0.572694, 
                        0.870288, -0.777475, -0.13282, -0.329776, 
                        0.285539, -0.739775, -0.163327};
                        */
    Harmony* h = new Harmony();
    for (int i = 0; i < weightCount; ++i)
        h->push_back(weights[i]);
    return h;
}

float EvaluateMove(GameState const& state, Harmony const& h) {
    float result = 0.0;
    result += h.at(0)*PileHeight(state);
    result += h.at(1)*Holes(state);
    result += h.at(2)*ConnectedHoles(state);
    result += h.at(3)*RemovedRows(state);
    result += h.at(4)*AltitudeDifference(state);
    result += h.at(5)*MaxWellDepth(state);
    result += h.at(6)*SumOfAllWells(state);
    result += h.at(7)*LandingHeight(state);
    result += h.at(8)*Blocks(state);
    result += h.at(9)*WeightedBlocks(state);
    result += h.at(10)*RowTransitions(state);
    result += h.at(11)*ColTransitions(state);
    result += h.at(12)*HighestHole(state);
    result += h.at(13)*BlocksAboveHighestHole(state);
    result += h.at(14)*PotentialRows(state);
    result += h.at(15)*Smoothness(state);
    result += h.at(16)*ErodedPieces(state);
    result += h.at(17)*RowHoles(state);
    result += h.at(18)*HoleDepth(state);
    return result;
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

/* *
 * Function AltitudeDifference
 *
 * The height difference between the lowest and highest reachable blocks.
 * */
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

/* *
 * Function MaxWellDepth
 *
 * Depth of the deepest well on the board.
 * */
int MaxWellDepth(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    int maxWellDepth = 0;
    int wells[COLS];
    for (int i = 0; i < COLS; ++i)
        wells[i] = board.WellDepth(i);
    if (wells[0] < wells[1]) {
        int depth = wells[1]-wells[0];
        maxWellDepth = std::max(maxWellDepth, depth);
    }
    for (int i = 1; i < COLS-1; ++i) {
        if (wells[i] < wells[i-1] && wells[i] < wells[i+1]) {
            int depth = std::min(wells[i-1]-wells[i], wells[i+1]-wells[i]);
            maxWellDepth = std::max(maxWellDepth, depth);
        }
    }
    if (wells[COLS-1] < wells[COLS-2]) {
        int depth = wells[COLS-2]-wells[COLS-1];
        maxWellDepth = std::max(maxWellDepth, depth);
    }
    return maxWellDepth;
}

/* *
 * Function SumOfAllWells
 *
 * Sum of the depths of all wells on the board.
 * */
int SumOfAllWells(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    int totalWellDepth = 0;
    int wells[COLS];
    for (int i = 0; i < COLS; ++i)
        wells[i] = board.WellDepth(i);
    if (wells[0] < wells[1]) {
        int depth = wells[1]-wells[0];
        totalWellDepth += depth;
    }
    for (int i = 1; i < COLS-1; ++i) {
        if (wells[i] < wells[i-1] && wells[i] < wells[i+1]) {
            int depth = std::min(wells[i-1]-wells[i], wells[i+1]-wells[i]);
            totalWellDepth += depth;
        }
    }
    if (wells[COLS-1] < wells[COLS-2]) {
        int depth = wells[COLS-2]-wells[COLS-1];
        totalWellDepth += depth;
    }
    return totalWellDepth;
}

/* *
 * Function LandingHeight
 *
 * Height at which the last tetronimo was placed
 * */
int LandingHeight(GameState const& state) {
    Tetromino const& last = state.LastPiecePlayed();
    int height = last.GetY();
    bool const* desc = last.GetDesc();
    for (int i = 0; i < 4; ++i) {
        if (desc[i]) {
            --height;
            break;
        }
    }
    return (ROWS-height);
}

/* *
 * Function Blocks
 *
 * The number of blocks on the board.
 * */
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

/* *
 * Function WeightedBlocks
 *
 * Blocks on the board, weighted by height.
 * */
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

/* *
 * Function RowTransitions
 *
 * The sum of all occupied to unoccupied or vice versa row transitions.
 * */
int RowTransitions(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    BoardDesc const& desc = board.GetBoardDesc();
    int transCount = 0;
    for (int j = 0; j < ROWS; ++j) {
        bool validRow = false;
        bool inBlock = true;
        int rowTransCount = 0;
        for (int i = 0; i < COLS; ++i) {
            if (desc[i][j] && !inBlock) {
                ++rowTransCount;
                inBlock = true;
                validRow = true;
            }
            if (!desc[i][j] && inBlock) {
                ++rowTransCount;
                inBlock = false;
            }
        }
        if (!inBlock)
            ++rowTransCount;
        if (validRow)
            transCount += rowTransCount;
    }
    return transCount;
}

/* *
 * Function ColTransitions
 *
 * The sum of all occupied to unoccupied or vice versa col transitions.
 * */
int ColTransitions(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    BoardDesc const& desc = board.GetBoardDesc();
    int transCount = 0;
    for (int i = 0; i < COLS; ++i) {
        bool validCol = false;
        bool inBlock = true;
        int colTransCount = 0;
        for (int j = ROWS-1; j >= 0; --j) {
            if (desc[i][j] && !inBlock) {
                ++colTransCount;
                inBlock = true;
                validCol = true;
            }
            if (!desc[i][j] && inBlock) {
                ++colTransCount;
                inBlock = false;
            }
        }
        if (validCol)
            transCount += colTransCount;
    }
    // Discount all non-well column tops
    int wells[COLS];
    int wellCount = 0;
    for (int i = 0; i < COLS; ++i)
        wells[i] = board.WellDepth(i);
    if (wells[0] < wells[1])
        ++wellCount;
    for (int i = 1; i < COLS-1; ++i) {
        if (wells[i] < wells[i-1] && wells[i] < wells[i+1])
            ++wellCount;
    }
    if (wells[COLS-1] < wells[COLS-2])
        ++wellCount;
    transCount -= (COLS-wellCount);
    return transCount;
}

/* *
 * Function HighestHole
 *
 * Height of the topmost hole on the board.
 * */
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
    return (ROWS-highest);
}

/* *
 * Function BlocksAboveHighestHole
 *
 * The number of blocks on top of the highest hole.
 * */
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
                // Tie-breaker: greatest number of blocks hit
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

/* *
 * Function PotentialRows
 *
 * Number of rows located above highest hole in use by more than 8 cells.
 * */
int PotentialRows(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    BoardDesc const& desc = board.GetBoardDesc();
    int highest = (ROWS-HighestHole(state));
    int potentialCount = 0;
    for (int j = 0; j < highest; ++j) {
        int setCount = 0;
        for (int i = 0; i < COLS; ++i) {
            if (desc[i][j])
                ++setCount;
        }
        if (setCount >= 8)
            ++potentialCount;
    }
    return potentialCount;
}

/* *
 * Function Smoothness
 *
 * Sum of differences in heights between adjacent columns.
 * */
int Smoothness(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    int smoothness = 0;
    for (int i = 0; i < COLS-1; ++i)
        smoothness += abs(board.WellDepth(i)-board.WellDepth(i+1));
    return smoothness;
}

/* *
 * Function ErodedPieces
 *
 * Number of rows cleared * number of cells of last piece eliminated.
 * */
int ErodedPieces(GameState const& state) {
    std::vector<int> const& cleared = state.LastClearedRows();
    Tetromino const& last = state.LastPiecePlayed();
    bool const* desc = last.GetDesc();
    int erodedCount = 0;
    for (int r = 0; r < cleared.size(); ++r) {
        int tetRow = cleared.at(r) - last.GetY() + 1;
        if (tetRow >= 0 && tetRow < 4) {
            int blockCount = 0;
            for (int i = 0; i < 4; ++i) {
                if (desc[tetRow*4+i])
                    ++blockCount;
            }
            erodedCount += blockCount;
        }
    }
    return (erodedCount * cleared.size());
}

/* *
 * Function RowHoles
 *
 * The number of rows with at least one hole.
 * */
int RowHoles(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    BoardDesc const& desc = board.GetBoardDesc();
    int holeCount[ROWS];
    for (int i = 0; i < ROWS; ++i)
        holeCount[i] = 0;
    for (int i = 0; i < COLS; ++i) {
        bool hitTop = false;
        for (int j = 0; j < ROWS; ++j) {
            if (desc[i][j] && !hitTop)
                hitTop = true;
            else if (!desc[i][j] && hitTop)
                holeCount[j] += 1;
        }
    }
    int rowHoleCount = 0;
    for (int i = 0; i < ROWS; ++i) {
        if (holeCount[i] > 0)
            ++rowHoleCount;
    }
    return rowHoleCount;
}

/* *
 * Function HoleDepth
 *
 * The number of filled cells on top of each hole.
 * */
int HoleDepth(GameState const& state) {
    GameBoard const& board = state.GetBoard();
    BoardDesc const& desc = board.GetBoardDesc();
    int cellCount = 0;
    for (int i = 0; i < COLS; ++i) {
        bool hitHole = false;
        bool inHole = false;
        for (int j = ROWS-1; j >= 0; --j) {
            if (desc[i][j] && inHole) {
                inHole = false;
            }
            if (desc[i][j] && hitHole) {
                ++cellCount;
            }
            if (!desc[i][j] && !inHole) {
                inHole = true;
                hitHole = true;
            }
        }
    }
    return cellCount;
}

