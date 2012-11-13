#pragma once

#include <util/vector.hpp>
#include <model/tetronimo.hpp>
#include <iostream>


typedef std::vector< std::vector<bool> > BoardDesc;

class GameBoard {
public:
    GameBoard();
    GameBoard(char const* desc);
    ~GameBoard();
    // Applies move to board
    bool PushMove(Tetronimo const& tet);
    // Reverts last move to board
    bool PopMove();
    BoardDesc const& GetBoardDesc() const;
    bool IsOccupied(int x, int y) const;
    bool InBounds(int x, int y) const;
    int WellDepth(int x) const;
    void Update(char const* desc);
    bool HasChanged(char const* desc) const;
    friend std::ostream& operator<< (std::ostream& out, GameBoard const& gb);
private:
    void Translate(char const* desc);
    BoardDesc m_board;
    char* m_pDesc;
    std::vector<Tetronimo> m_moveStack;
};

