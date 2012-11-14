#include <model/game_state.hpp>

GameState::GameState(std::vector<Tetromino> const& queue, Tetromino const& inPlay)
    : m_board(), m_pieceQueue(queue), m_depthInQueue(-1), 
        m_pieceInPlay(inPlay)
{ }

bool GameState::PushMove(Tetromino const& t) {
    if (t.GetType() != m_pieceInPlay.GetType())
        return false;
    return m_board.PushMove(t);
}

bool GameState::PopMove() {
    return m_board.PopMove();
}

bool GameState::PiecesLeftInQueue() const {
    return (m_depthInQueue < m_pieceQueue.size()-1);
}

Tetromino const& GameState::FeedFromQueue() {
    m_pieceInPlay = m_pieceQueue[m_depthInQueue+1];
    ++m_depthInQueue;
    return m_pieceInPlay;
}

GameBoard& GameState::GetBoard() {
    return m_board;
}

GameBoard const& GameState::GetBoard() const {
    return m_board;
}

