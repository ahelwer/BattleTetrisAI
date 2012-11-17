#pragma once

#include <util/vector.hpp>
#include <model/tetromino.hpp>
#include <iostream>


typedef std::vector< std::vector<bool> > BoardDesc;

class GameBoard {
public:
    GameBoard();
    GameBoard(char const* desc);
    GameBoard& operator= (GameBoard const& o);
    ~GameBoard();
    bool ApplyMove(Tetromino const& t);
    bool PushMove(Tetromino const& t);
    bool PopMove();
    std::vector<int> const* ClearRows();
    void ClearBoard();
    BoardDesc& GetBoardDesc();
    BoardDesc const& GetBoardDesc() const;
    bool IsValidMove(Tetromino const& t) const;
    bool IsAtRest(Tetromino const& t) const;
    bool IsOccupied(int x, int y) const;
    bool InBounds(int x, int y) const;
    int WellDepth(int x) const;
    void Update(char const* desc);
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

