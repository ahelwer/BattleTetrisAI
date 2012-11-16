#pragma once

#include <iostream>
#include <util/vector.hpp>
#include <model/game_board.hpp>
#include <model/tetromino.hpp>
#include <queue>
#include <stack>

class GameState {
public:
    GameState();
    GameState(std::queue<Tetromino> const& queue, Tetromino const& inPlay);
    ~GameState();

    int ApplyMove(Tetromino const& t);
    bool PushMove(Tetromino const& t);
    bool PopMove();
    bool PushFeedFromQueue(int feedCount);
    bool PopFeedFromQueue();

    int QueuedPieces() const;
    void SetPieceInPlay(Tetromino const* t);
    void SetQueueInPlay(std::queue<Tetromino> const& queue);
    Tetromino const* GetPieceInPlay() const;
    std::vector<int> const& LastClearedRows() const;
    int GetCurrentPieceNumber() const;
    void RegisterLastClearedRows(std::vector<int> const& cleared);
    void RegisterCurrentPieceNumber(int n);
    void RegisterRowClearEvent();
    bool WasRowClearEvent();
    bool PieceHasChanged();
    Tetromino const& LastPiecePlayed() const;
    GameBoard& GetBoard();
    GameBoard const& GetBoard() const;
    friend std::ostream& operator<< (std::ostream& out, GameState const& state);
private:
    GameBoard m_board;
    std::stack<Tetromino> m_piecePlayedStack;
    std::stack<std::vector<int> > m_rowsClearedStack;
    std::stack<std::queue<Tetromino> > m_pieceQueueStack;
    Tetromino* m_pPieceInPlay;
    int m_pieceNumber;
    bool m_rowsCleared;
    bool m_pieceChanged;
};

