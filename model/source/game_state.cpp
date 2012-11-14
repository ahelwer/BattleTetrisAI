#include <model/game_state.hpp>

GameState::GameState(std::vector<Tetromino> const& queue, Tetromino const& inPlay)
    : m_pieceQueue(queue), m_depthInQueue(-1), 
        m_pieceInPlay(inPlay)
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
}

bool GameState::PushMove(Tetromino const& t) {
    if (t.GetType() != m_pieceInPlay.GetType())
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

Tetromino const& GameState::GetPieceInPlay() {
    return m_pieceInPlay;
}

Tetromino const& GameState::FeedFromQueue() {
    m_pieceInPlay = m_pieceQueue[m_depthInQueue+1];
    ++m_depthInQueue;
    return m_pieceInPlay;
}

std::vector<int> const& GameState::LastClearedRows() const {
    return *m_rowClearedStack.back(); 
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

