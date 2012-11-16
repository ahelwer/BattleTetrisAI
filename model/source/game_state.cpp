#include <model/game_state.hpp>

GameState::GameState()
    : m_board(), m_lastRowsCleared(), m_pieceQueue(),
       m_pPieceInPlay(NULL), 
       m_lastPiecePlayed(Tetromino('O', 0, 5, 1)),
       m_pieceNumber(-1), m_rowsCleared(false), 
       m_pieceChanged(false)
{ }

GameState::GameState(std::vector<Tetromino> const& queue, 
                        Tetromino const& inPlay)
    : m_board(), m_lastRowsCleared(), m_pieceQueue(queue),
        m_pPieceInPlay(new Tetromino(inPlay)), 
        m_lastPiecePlayed(Tetromino('O', 0, 5, 1)),
        m_pieceNumber(-1), m_rowsCleared(false), 
        m_pieceChanged(false)
{ }

GameState::GameState(GameState const& o)
    : m_board(), m_lastRowsCleared(), m_pieceQueue(),
       m_pPieceInPlay(NULL), 
       m_lastPiecePlayed(Tetromino('O', 0, 5, 1)),
       m_pieceNumber(-1), m_rowsCleared(false),
       m_pieceChanged(false)
{
    (*this) = o;    
}

GameState& GameState::operator= (GameState const& o) {
    if (this == &o) { // detects self-assignment
        return *this;
    }

    m_board = o.m_board;
    m_lastRowsCleared = o.m_lastRowsCleared;

    if (m_pPieceInPlay != NULL) {
        delete m_pPieceInPlay;
        m_pPieceInPlay = NULL;
    }
    if (o.m_pPieceInPlay == NULL) {
        m_pPieceInPlay = NULL;
    }
    else {
        m_pPieceInPlay = new Tetromino(*(o.m_pPieceInPlay));
    }

    m_lastPiecePlayed = o.m_lastPiecePlayed;
    m_pieceNumber = o.m_pieceNumber;
    m_rowsCleared = o.m_rowsCleared;
    m_pieceChanged = o.m_pieceChanged;

    return (*this);
}

GameState::~GameState() {
    if (m_pPieceInPlay != NULL) {
        delete m_pPieceInPlay;
        m_pPieceInPlay = NULL;
    }
}

int GameState::ApplyMove(Tetromino const& t) {
    if (t.GetType() != m_pPieceInPlay->GetType())
        return -1;
    bool success = m_board.ApplyMove(t);
    if (success) {
        std::vector<int> const* cleared = m_board.ClearRows();
        int clearCount = cleared->size();
        m_lastRowsCleared = *cleared;
        delete cleared;
        if (m_pPieceInPlay != NULL) {
            m_lastPiecePlayed = *m_pPieceInPlay;
            delete m_pPieceInPlay;
            m_pPieceInPlay = NULL;
            m_pieceNumber = -1;
        }
        return clearCount;
    }
    else {
        return -1;
    }
}

bool GameState::FeedFromQueue(int feedCount) {
    if (feedCount >= m_pieceQueue.size())
        return false;
    if (m_pPieceInPlay != NULL) {
        delete m_pPieceInPlay;
        m_pPieceInPlay = NULL;
    }
    m_pPieceInPlay = new Tetromino(m_pieceQueue.at(feedCount));
    return true;
}

int GameState::QueuedPieceCount() const {
    return m_pieceQueue.size();
}

void GameState::SetPieceInPlay(Tetromino const* t) {
    if (m_pPieceInPlay != NULL) {
        delete m_pPieceInPlay;
        m_pPieceInPlay = NULL;
    }
    if (t == NULL) {
        m_pPieceInPlay = NULL;
        m_pieceNumber = -1;
    }
    else {
        m_pPieceInPlay = new Tetromino(*t);
        m_pieceNumber = -1;
    }
}

void GameState::SetQueueInPlay(std::vector<Tetromino> const& q) {
    m_pieceQueue = q;
}

Tetromino const* GameState::GetPieceInPlay() const {
    return m_pPieceInPlay;
}

std::vector<int> const& GameState::LastClearedRows() const {
    return m_lastRowsCleared;
}

Tetromino const& GameState::LastPiecePlayed() const {
    return m_lastPiecePlayed;
}

void GameState::SetLastClearedRows(std::vector<int> const& cleared) {
    m_lastRowsCleared = cleared;
}

int GameState::GetCurrentPieceNumber() const {
    return m_pieceNumber;
}

void GameState::SetCurrentPieceNumber(int n) {
    if (n != m_pieceNumber) {
        m_pieceChanged = true;
    }
    if (n == -1 && m_pPieceInPlay != NULL) {
        delete m_pPieceInPlay;
        m_pPieceInPlay = NULL;
    }
    m_pieceNumber = n;
}

void GameState::SetRowClearEvent() {
    m_rowsCleared = true;
}

bool GameState::WasRowClearEvent() {
    bool wasCleared = m_rowsCleared;
    m_rowsCleared = false;
    return wasCleared;
}

bool GameState::PieceHasChanged() {
    bool changed = m_pieceChanged;
    m_pieceChanged = false;
    return changed;
}

GameBoard& GameState::GetBoard() {
    return m_board;
}

GameBoard const& GameState::GetBoard() const {
    return m_board;
}

std::ostream& operator<< (std::ostream& out, GameState const& state) {
    out << state.GetBoard();
    return out;
}

