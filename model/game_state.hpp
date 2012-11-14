#pragma once

#include <util/vector.hpp>
#include <model/game_board.hpp>

class GameState {
public:
    GameState(std::vector<Tetronimo> const& queue, Tetronimo const& inPlay);
    bool PushMove(int x, int y, int orient);
    bool PopMove();
    bool PiecesLeftInQueue() const;
    Tetronimo const& FeedFromQueue();
    GameBoard& GetBoard();
    GameBoard const& GetBoard() const;
private:
    GameBoard m_board;
    std::vector<Tetronimo> m_pieceQueue;
    int m_depthInQueue;
    Tetronimo m_pieceInPlay;
};

