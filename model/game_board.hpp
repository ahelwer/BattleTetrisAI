#pragma once

#include <util/vector.hpp>
#include <model/tetromino.hpp>
#include <iostream>


typedef std::vector< std::vector<bool> > BoardDesc;

/* *
 * Class GameBoard
 *
 * Internal representation of the Tetris game board.
 * */
class GameBoard {
public:
    GameBoard();
    // Takes description of board compressed as hex string
    GameBoard(char const* desc);
    GameBoard& operator= (GameBoard const& o);
    ~GameBoard();
    // Applies move to game board
    bool ApplyMove(Tetromino const& t);
    // Copies current board onto stack, applies move to self
    bool PushMove(Tetromino const& t);
    // Pops board off stack and sets to current
    bool PopMove();
    // Performs row clearing operation, returning list of rows cleared
    std::vector<int> const* ClearRows();
    // Wipes game board in case of game end
    void ClearBoard();
    // Gets raw 2D boolean vector representation of board
    BoardDesc& GetBoardDesc();
    BoardDesc const& GetBoardDesc() const;
    // Determines if given move is valid in current board
    bool IsValidMove(Tetromino const& t) const;
    // Determines if given Tetromino is at rest in current board
    bool IsAtRest(Tetromino const& t) const;
    // Determines if specified grid cell is full
    bool IsOccupied(int x, int y) const;
    // Determines if specified grid cell is within game board bounds
    bool InBounds(int x, int y) const;
    // Determines depth of well in given column
    int WellDepth(int x) const;
    // Updates game board from compressed hex string
    void Update(char const* desc);
    // Determines if update is required based on compressed hex string
    bool HasChanged(char const* desc) const;
    bool operator== (GameBoard const& o) const;
    bool operator!= (GameBoard const& o) const;
    friend std::ostream& operator<< (std::ostream& out, GameBoard const& gb);
private:
    void ApplyMoveToBoard(Tetromino const& t);
    void Translate(char const* desc);
    std::vector<BoardDesc> m_boardStack;
    char* m_pDesc;
};

