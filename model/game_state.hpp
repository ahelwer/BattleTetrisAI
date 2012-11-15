#pragma once

#include <util/vector.hpp>
#include <model/game_board.hpp>
#include <model/tetromino.hpp>

class GameState {
public:
    GameState(std::vector<Tetromino> const& queue, Tetromino const& inPlay);
    ~GameState();
    int ApplyMove(Tetromino const& t);
    bool PushMove(Tetromino const& t);
    bool PopMove();
    bool PiecesLeftInQueue() const;
    void SetPieceInPlay(Tetromino const& t);
    Tetromino const& GetPieceInPlay() const;
    Tetromino const& FeedFromQueue();
    std::vector<int> const& LastClearedRows() const;
    Tetromino const& LastPiecePlayed() const;
    GameBoard& GetBoard();
    GameBoard const& GetBoard() const;
private:
    GameBoard m_board;
    std::vector<Tetromino> m_pieceQueue;
    std::vector<std::vector<int> const*> m_rowClearedStack;
    std::vector<Tetromino> m_playedStack;
    int m_depthInQueue;
    Tetromino m_pieceInPlay;
};

