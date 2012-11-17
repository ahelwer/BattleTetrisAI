#include <model/game_board.hpp>
#include <util/constants.hpp>
#include <cstring>
#include <algorithm>

GameBoard::GameBoard()
    : m_boardStack(1, std::vector< std::vector<bool> >
                    (COLS, std::vector<bool>(ROWS, false))), 
        m_pDesc(new char[BOARD_DESC_SIZE])
{ 
    for (int i = 0; i < BOARD_DESC_SIZE; ++i)
        m_pDesc[i] = '0';
}

GameBoard::GameBoard(char const* desc)
    : m_boardStack(1, std::vector< std::vector<bool> >
                    (COLS, std::vector<bool>(ROWS, false))), 
        m_pDesc(new char[BOARD_DESC_SIZE])
{
    for (int i = 0; i < BOARD_DESC_SIZE; ++i)
        m_pDesc[i] = '0';
    Translate(desc);
}

GameBoard& GameBoard::operator= (GameBoard const& o) {
    if (this == &o)
        return (*this);

    m_boardStack = o.m_boardStack;
    if (m_pDesc != NULL) {
        delete[] m_pDesc;
        m_pDesc = NULL;
    }
    if (o.m_pDesc == NULL) {
        m_pDesc = NULL;
    }
    else {
        m_pDesc = new char[BOARD_DESC_SIZE];
        memcpy((void*)m_pDesc, (void*)o.m_pDesc, BOARD_DESC_SIZE);
    }
    return (*this);
}

GameBoard::~GameBoard() {
    if (m_pDesc != NULL) {
        delete[] m_pDesc;
        m_pDesc = NULL;
    }
}

bool GameBoard::ApplyMove(Tetromino const& t) {
    if (!IsValidMove(t))
        return false;
    ApplyMoveToBoard(t);
    return true;
}

bool GameBoard::PushMove(Tetromino const& t) {
    if (!IsValidMove(t))
        return false;

    // Saves current board on stack
    BoardDesc& board = GetBoardDesc();
    m_boardStack.push_back(board);

    // Apply tetronimo to board
    ApplyMoveToBoard(t);
    return true;
}

bool GameBoard::PopMove() {
    if (m_boardStack.size() == 1)
        return false;
    m_boardStack.pop_back();
    return true;
}

std::vector<int> const* GameBoard::ClearRows() {
    std::vector<int>* cleared = new std::vector<int>();
    BoardDesc& desc = GetBoardDesc();
    for (int j = 0; j < ROWS; ++j) {
        bool isCleared = true;
        for (int i = 0; i < COLS; ++i) {
            if (!desc[i][j]) {
                isCleared = false;
                break;
            }
        }
        if (isCleared)
            cleared->push_back(j);
    }
    for (int r = 0; r < cleared->size(); ++r) {
        int rowCleared = cleared->at(r);
        for (int j = rowCleared; j > 0; --j) {
            for (int i = 0; i < COLS; ++i) {
                desc[i][j] = desc[i][j-1];
            }
        }
    }
    return cleared;
}

void GameBoard::ClearBoard() {
    BoardDesc& desc = GetBoardDesc();
    for (int j = 0; j < ROWS; ++j) {
        for (int i = 0; i < COLS; ++i) {
            desc[i][j] = false;
        }
    }
    if (m_pDesc != NULL) {
        for (int i = 0; i < BOARD_DESC_SIZE; ++i) {
            m_pDesc[i] = '0';
        }
    }
}

bool GameBoard::IsValidMove(Tetromino const& t) const {
    BoardDesc const& board = GetBoardDesc();
    bool const* desc = t.GetDesc();
    int x = t.GetX();
    int y = t.GetY();
    int pivotX = 2;
    int pivotY = 1;

    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 4; ++i) {
            bool isSet = desc[j*4+i];
            if (isSet) {
                int boardX = (x + i - pivotX);
                int boardY = (y + j - pivotY);
                if (!InBounds(boardX, boardY)) {
                    return false;
                }
                else {
                    if(board[boardX][boardY]) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool GameBoard::IsAtRest(Tetromino const& t) const {
    if (!IsValidMove(t))
        return false;
    Tetromino n = t;
    n.ShiftDown();
    return !IsValidMove(n);
}

BoardDesc& GameBoard::GetBoardDesc() {
    return m_boardStack.back();
}

BoardDesc const& GameBoard::GetBoardDesc() const {
    return m_boardStack.back();
}

bool GameBoard::IsOccupied(int x, int y) const {
    BoardDesc const& board = GetBoardDesc();
    if (InBounds(x, y))
        return board[x][y];
    else
        return false;
}

bool GameBoard::InBounds(int x, int y) const {
    return (x >= 0 && x < COLS && y >= 0 && y < ROWS);
}

int GameBoard::WellDepth(int x) const {
    BoardDesc const& board = GetBoardDesc();
    if (!InBounds(x, 5))
        return -1;
    int depth = 0;
    for (int i = 0; i < ROWS; ++i) {
        if (board[x][i])
            break;
        ++depth;
    }
    return depth;
}

void GameBoard::Update(char const* desc) {
    Translate(desc);
    if (m_pDesc != desc) // Overlapped memcpy is undefined
        memcpy((void*)m_pDesc, (void*)desc, BOARD_DESC_SIZE);
}

bool GameBoard::HasChanged(char const* desc) const {
    if (m_pDesc == desc) return false;
    return (memcmp((void*)desc, (void*)m_pDesc, BOARD_DESC_SIZE) != 0);
}

void GameBoard::ApplyMoveToBoard(Tetromino const& t) {
    BoardDesc& board = GetBoardDesc();
    bool const* desc = t.GetDesc();
    int x = t.GetX();
    int y = t.GetY();
    int pivotX = 2;
    int pivotY = 1;

    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 4; ++i) {
            bool isSet = desc[j*4+i];
            if (isSet) {
                int boardX = (x + i - pivotX);
                int boardY = (y + j - pivotY);
                board[boardX][boardY] = true;
            }
        }
    }
}

void GameBoard::Translate(char const* desc) {
    BoardDesc& board = GetBoardDesc();
    for (int i = 0; i < BOARD_DESC_SIZE; ++i) {
        char hex = desc[i];
        char byte = 0;
        if (hex > 64) // hex > 64, so is letter
            byte = (hex - 55); // A is char code 65, add 10
        else
            byte = (hex - 48); // 0 is char code 48
        // Reads hex bitmask into board
        int x = i*4 % COLS;
        int y = i*4 / COLS;
        board[x][y] = (byte & 8);
        x = (i*4+1) % COLS;
        y = (i*4+1) / COLS;
        board[x][y] = (byte & 4);
        x = (i*4+2) % COLS;
        y = (i*4+2) / COLS;
        board[x][y] = (byte & 2);
        x = (i*4+3) % COLS;
        y = (i*4+3) / COLS;
        board[x][y] = (byte & 1);
    }
}

bool GameBoard::operator== (GameBoard const& o) const {
    if (this == &o)
        return true;

    bool descEqual = (memcmp((void*)m_pDesc, (void*)o.m_pDesc, 
                        BOARD_DESC_SIZE) == 0);

    bool boardEqual = true;
    if (m_boardStack.size() == o.m_boardStack.size()) {
        for (int k = 0; k < m_boardStack.size(); ++k) {
            BoardDesc const& mine = m_boardStack.at(k);
            BoardDesc const& theirs = o.m_boardStack.at(k);
            for (int i = 0; i < COLS; ++i) {
                for (int j = 0; j < ROWS; ++j) {
                    if (mine[i][j] != theirs[i][j])
                        boardEqual = false;
                }
            }
        }
    }

    return (descEqual && boardEqual);

}

bool GameBoard::operator!= (GameBoard const& o) const {
    return !(*this == o);
}

std::ostream& operator<< (std::ostream& out, GameBoard const& gb) {
    BoardDesc const& board = gb.GetBoardDesc();
    out << board;
    return out;
}

