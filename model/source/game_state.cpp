#include <model/game_state.hpp>

GameState::GameState()
    : m_board(), m_piecePlayedStack(), m_rowsClearedStack(),
       m_pieceQueueStack(), m_pPieceInPlay(NULL), m_pieceNumber(-1),
       m_rowsCleared(false), m_pieceChanged(false)
{ }

GameState::GameState(std::queue<Tetromino> const& queue, Tetromino const& inPlay)
    : m_board(), m_piecePlayedStack(), m_rowsClearedStack(),
        m_pieceQueueStack(), m_pPieceInPlay(new Tetromino(inPlay)), 
        m_pieceNumber(-1), m_rowsCleared(false), m_pieceChanged(false)
{ 
    m_rowsClearedStack.push(std::vector<int>());
    m_pieceQueueStack.push(queue);
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
        delete cleared;
        return clearCount;
    }
    else {
        return -1;
    }
}

bool GameState::PushMove(Tetromino const& t) {
    if (t.GetType() != m_pPieceInPlay->GetType())
        return false;
    bool success = m_board.PushMove(t);
    if (success) {
        m_piecePlayedStack.push(t);
        std::vector<int> const* cleared = m_board.ClearRows();
        m_rowsClearedStack.push(*cleared);
        delete cleared;
    }
    return success;
}

bool GameState::PopMove() {
    bool success = m_board.PopMove();
    if (success) {
        m_piecePlayedStack.pop();
        m_rowsClearedStack.pop();
    }
    return success;
}

bool GameState::PushFeedFromQueue(int feedCount) {
    std::queue<Tetromino> next = m_pieceQueueStack.top();
    if (feedCount >= next.size())
        return false;
    if (m_pPieceInPlay != NULL) {
        delete m_pPieceInPlay;
        m_pPieceInPlay = NULL;
    }
    for (int i = 0; i < feedCount-1; ++i) {
        next.pop(); 
    }
    m_pPieceInPlay = new Tetromino(next.front());
    next.pop();
    m_pieceQueueStack.push(next);
    return true;
}

bool GameState::PopFeedFromQueue() {
    m_pieceQueueStack.pop();
    if (m_pPieceInPlay != NULL) {
        delete m_pPieceInPlay;
        m_pPieceInPlay = NULL;
    }
    m_pPieceInPlay = new Tetromino(m_piecePlayedStack.top());
    return true;
}

int GameState::QueuedPieces() const {
    return (m_pieceQueueStack.top().size());
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

void GameState::SetQueueInPlay(std::queue<Tetromino> const& q) {
    m_pieceQueueStack.top() = q;
}

Tetromino const* GameState::GetPieceInPlay() const {
    return m_pPieceInPlay;
}

std::vector<int> const& GameState::LastClearedRows() const {
    return m_rowsClearedStack.top();
}

void GameState::RegisterLastClearedRows(std::vector<int> const& cleared) {
    m_rowsClearedStack.push(cleared);
}

int GameState::GetCurrentPieceNumber() const {
    return m_pieceNumber;
}

void GameState::RegisterCurrentPieceNumber(int n) {
    if (n != m_pieceNumber) {
        m_pieceChanged = true;
    }
    if (n == -1 && m_pPieceInPlay != NULL) {
        delete m_pPieceInPlay;
        m_pPieceInPlay = NULL;
    }
    m_pieceNumber = n;
}

void GameState::RegisterRowClearEvent() {
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

Tetromino const& GameState::LastPiecePlayed() const {
    return m_piecePlayedStack.top();
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

