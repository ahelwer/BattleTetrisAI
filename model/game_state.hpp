#pragma once

#include <iostream>
#include <util/vector.hpp>
#include <model/game_board.hpp>
#include <model/tetromino.hpp>

class GameState {
public:
    GameState();
    GameState(std::vector<Tetromino> const& queue, Tetromino const& inPlay);
    ~GameState();
    int ApplyMove(Tetromino const& t);
    bool PushMove(Tetromino const& t);
    bool PopMove();
    bool PiecesLeftInQueue() const;
    void SetPieceInPlay(Tetromino* t);
    Tetromino const* GetPieceInPlay() const;
    std::vector<int> const& LastClearedRows() const;
    int GetCurrentPieceNumber() const;
    void RegisterLastClearedRows(std::vector<int> const& cleared);
    void RegisterCurrentPieceNumber(int n);
    void RegisterRowClearEvent();
    bool WasRowClearEvent();
    Tetromino const& LastPiecePlayed() const;
    GameBoard& GetBoard();
    GameBoard const& GetBoard() const;
    friend std::ostream& operator<< (std::ostream& out, GameState const& state);
private:
    GameBoard m_board;
    std::vector<Tetromino> m_pieceQueue;
    std::vector<std::vector<int> const*> m_rowClearedStack;
    std::vector<Tetromino> m_playedStack;
    int m_depthInQueue;
    Tetromino* m_pPieceInPlay;
    int m_pieceNumber;
    bool m_rowsCleared;
};

