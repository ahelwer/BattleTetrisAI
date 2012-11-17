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

    // Pieces
    void SetPieceInPlay(Tetromino const* t);
    Tetromino const* GetPieceInPlay() const;
    Tetromino const& LastPiecePlayed() const;
    void SetCurrentPieceNumber(int n);
    int GetCurrentPieceNumber() const;
    bool PieceHasChanged();

    // Piece Queue
    int QueuedPieceCount() const;
    void SetQueueInPlay(std::vector<Tetromino> const& queue);
    bool FeedFromQueue(int feedCount);

    // Cleared rows
    std::vector<int> const& LastClearedRows() const;
    void SetLastClearedRows(std::vector<int> const& cleared);
    void SetRowClearEvent();
    bool WasRowClearEvent();

    // Game board
    GameBoard& GetBoard();
    GameBoard const& GetBoard() const;
    bool operator== (GameState const& o) const;
    bool operator!= (GameState const& o) const;
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

