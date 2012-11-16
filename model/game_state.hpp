#pragma once

#include <iostream>
#include <util/vector.hpp>
#include <model/game_board.hpp>
#include <model/tetromino.hpp>

class GameState {
public:
    GameState();
    GameState(std::vector<Tetromino> const& queue, Tetromino const& inPlay);
    GameState(GameState const& o);
    GameState& operator= (GameState const& state);
    ~GameState();

    int ApplyMove(Tetromino const& t);
    bool FeedFromQueue(int feedCount);
    int QueuedPieceCount() const;

    void SetPieceInPlay(Tetromino const* t);
    void SetQueueInPlay(std::vector<Tetromino> const& queue);
    void SetLastClearedRows(std::vector<int> const& cleared);
    void SetCurrentPieceNumber(int n);
    void SetRowClearEvent();

    Tetromino const* GetPieceInPlay() const;
    std::vector<int> const& LastClearedRows() const;
    Tetromino const& LastPiecePlayed() const;
    int GetCurrentPieceNumber() const;
    bool WasRowClearEvent();
    bool PieceHasChanged();
    GameBoard& GetBoard();
    GameBoard const& GetBoard() const;
    friend std::ostream& operator<< (std::ostream& out, GameState const& state);
private:
    GameBoard m_board;
    std::vector<int> m_lastRowsCleared;
    std::vector<Tetromino> m_pieceQueue;
    Tetromino* m_pPieceInPlay;
    Tetromino m_lastPiecePlayed;
    int m_pieceNumber;
    bool m_rowsCleared;
    bool m_pieceChanged;
};

