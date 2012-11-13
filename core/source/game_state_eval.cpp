#include <core/game_state_eval.hpp>
#include <util/constants.hpp>

GameStateEval::GameStateEval(GameState& state) 
    : m_state(state)
{ }

float GameStateEval::Evaluate(Harmony const& h) const {
    return 0.0;
}

int GameStateEval::PileHeight() const {
    GameBoard& board = m_state.GetBoard();
    int minWellDepth = ROWS;
    for (int i = 0; i < COLS; ++i)
        minWellDepth = std::min(minWellDepth, board.WellDepth(i));
    return (ROWS - minWellDepth);
}

int GameStateEval::Holes() const {
    GameBoard& board = m_state.GetBoard();
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

int GameStateEval::ConnectedHoles() const {
    GameBoard& board = m_state.GetBoard();
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

int GameStateEval::RemovedRows() const {
    // Number of rows removed in last step
    return 0;
}

int GameStateEval::AltitudeDifference() const {
    GameBoard& board = m_state.GetBoard();
    int minWellDepth = ROWS;
    int maxWellDepth = 0;
    for (int i = 0; i < COLS; ++i) {
        int wellDepth = board.WellDepth(i);
        minWellDepth = std::min(minWellDepth, wellDepth);
        maxWellDepth = std::min(maxWellDepth, wellDepth);
    }
    return ((ROWS - minWellDepth) - (ROWS - maxWellDepth));
}

int GameStateEval::MaxWellDepth() const {
    GameBoard& board = m_state.GetBoard();
    int maxWellDepth = 0;
    for (int i = 0; i < COLS; ++i)
        maxWellDepth = std::min(maxWellDepth, board.WellDepth(i));
    return (ROWS - maxWellDepth);
}

int GameStateEval::SumOfAllWells() const {
    // Sum of all wells based on adjacent columns
    return 0;
}

int GameStateEval::LandingHeight() const {
    // Height at which last tetronimo was placed
    return 0;
}

int GameStateEval::Blocks() const {
    GameBoard& board = m_state.GetBoard();
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

int GameStateEval::WeightedBlocks() const {
    GameBoard& board = m_state.GetBoard();
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

int GameStateEval::RowTransitions() const {
    // Counts row transitions
    return 0;
}

int GameStateEval::ColTransitions() const {
    // Counts col transitions
    return 0;
}

int GameStateEval::HighestHole() const {
    GameBoard& board = m_state.GetBoard();
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

int GameStateEval::BlocksAboveHighestHole() const {
    GameBoard& board = m_state.GetBoard();
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

int GameStateEval::PotentialRows() const {
    return 0;
}

int GameStateEval::Smoothness() const {
    GameBoard& board = m_state.GetBoard();
    int smoothness = 0;
    for (int i = 0; i < COLS-1; ++i)
        smoothness += abs(board.WellDepth(i)-board.WellDepth(i+1));
    return smoothness;
}

int GameStateEval::ErodedPieces() const {
    return 0;
}

int GameStateEval::RowHoles() const {
    return 0;
}

int GameStateEval::HoleDepth() const {
    return 0;
}

