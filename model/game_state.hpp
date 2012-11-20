#pragma once

#include <iostream>
#include <util/vector.hpp>
#include <model/game_board.hpp>
#include <model/tetromino.hpp>

/* *
 * Class GameState
 *
 * Internal representation of entire game state.
 * */
class GameState {
public:
    GameState();
    // Takes piece queue and current piece in play
    GameState(std::vector<Tetromino> const& queue, Tetromino const& inPlay);
    GameState(GameState const& o);
    GameState& operator= (GameState const& state);
    ~GameState();

    // Applies move to self
    int ApplyMove(Tetromino const& t);

    // Piece-related methods
    // Sets current piece in play
    void SetPieceInPlay(Tetromino const* t);
    // Gets current piece in play
    Tetromino const* GetPieceInPlay() const;
    // Gets last piece played on board before current
    Tetromino const& LastPiecePlayed() const;
    // Sets current piece sequence number
    void SetCurrentPieceNumber(int n);
    // Gets current piece sequence number
    int GetCurrentPieceNumber() const;
    // Determines if a new piece has come on the board
    bool PieceHasChanged();

    // Piece Queue related methods
    // Gets number of queued pieces
    int QueuedPieceCount() const;
    // Sets current queue in play
    void SetQueueInPlay(std::vector<Tetromino> const& queue);
    // Feeds current game piece in from the piece queue
    bool FeedFromQueue(int feedCount);

    // Cleared rows related methods
    // Gets last cleared rows
    std::vector<int> const& LastClearedRows() const;
    // Sets last cleared rows
    void SetLastClearedRows(std::vector<int> const& cleared);
    // Registers a row clearing event (necessitates a re-search)
    void SetRowClearEvent();
    // Determines if a row clearing even occurred
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

