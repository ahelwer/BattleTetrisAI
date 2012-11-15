#include <model/game_state.hpp>

GameState::GameState()
    : m_depthInQueue(-1), m_pPieceInPlay(NULL), m_pieceNumber(-1),
        m_rowsCleared(false)
{ }

GameState::GameState(std::vector<Tetromino> const& queue, Tetromino const& inPlay)
    : m_pieceQueue(queue), m_depthInQueue(-1), 
        m_pPieceInPlay(new Tetromino(inPlay)), m_pieceNumber(-1),
        m_rowsCleared(false)
{ 
    m_rowClearedStack.push_back(new std::vector<int>());
}

GameState::~GameState() {
    for (int i = 0; i < m_rowClearedStack.size(); ++i) {
        if (m_rowClearedStack.at(i) != NULL) {
            delete m_rowClearedStack.at(i);
            m_rowClearedStack[i] = NULL;
        }
    }
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
        m_playedStack.push_back(t);
        std::vector<int> const* cleared = m_board.ClearRows();
        m_rowClearedStack.push_back(cleared);
    }
    return success;
}

bool GameState::PopMove() {
    bool success = m_board.PopMove();
    if (success) {
        m_playedStack.pop_back();
        std::vector<int> const* top = m_rowClearedStack.back();
        m_rowClearedStack.pop_back();
        delete top;
    }
    return success;
}

bool GameState::PiecesLeftInQueue() const {
    return (m_depthInQueue < m_pieceQueue.size()-1);
}

void GameState::SetPieceInPlay(Tetromino* t) {
    if (m_pPieceInPlay != NULL) {
        delete m_pPieceInPlay;
        m_pPieceInPlay = NULL;
    }
    if (t == NULL) {
        m_pPieceInPlay = NULL;
    }
    else {
        m_pPieceInPlay = t;
    }
}

Tetromino const* GameState::GetPieceInPlay() const {
    return m_pPieceInPlay;
}

std::vector<int> const& GameState::LastClearedRows() const {
    return *(m_rowClearedStack.back());
}

void GameState::RegisterLastClearedRows(std::vector<int> const& cleared) {
    std::vector<int> const* top = m_rowClearedStack.back();
    m_rowClearedStack[m_rowClearedStack.size()-1] = new std::vector<int>(cleared);
    delete top;
}

int GameState::GetCurrentPieceNumber() const {
    return m_pieceNumber;
}

void GameState::RegisterCurrentPieceNumber(int n) {
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

Tetromino const& GameState::LastPiecePlayed() const {
    return m_playedStack.back();
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

