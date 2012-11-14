#include <model/game_state.hpp>

GameState::GameState(std::vector<Tetronimo> const& queue, Tetronimo const& inPlay)
    : m_board(), m_pieceQueue(queue), m_depthInQueue(-1), 
        m_pieceInPlay(inPlay)
{ }

bool GameState::PushMove(int x, int y, int orient) {
    Tetronimo t (m_pieceInPlay.GetType(), orient, x, y);
    return m_board.PushMove(t);
}

bool GameState::PopMove() {
    return m_board.PopMove();
}

bool GameState::PiecesLeftInQueue() const {
    return (m_depthInQueue < m_pieceQueue.size()-1);
}

Tetronimo const& GameState::FeedFromQueue() {
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

