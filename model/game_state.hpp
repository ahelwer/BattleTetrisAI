#pragma once

#include <util/vector.hpp>
#include <model/game_board.hpp>
#include <model/tetromino.hpp>

class GameState {
public:
    GameState(std::vector<Tetromino> const& queue, Tetromino const& inPlay);
    bool PushMove(Tetromino const& t);
    bool PopMove();
    bool PiecesLeftInQueue() const;
    Tetromino const& FeedFromQueue();
    GameBoard& GetBoard();
    GameBoard const& GetBoard() const;
private:
    GameBoard m_board;
    std::vector<Tetromino> m_pieceQueue;
    int m_depthInQueue;
    Tetromino m_pieceInPlay;
};

