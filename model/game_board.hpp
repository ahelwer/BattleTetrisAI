#pragma once

#include <util/vector.hpp>
#include <iostream>

class GameBoard {
public:
    GameBoard();
    GameBoard(char const* desc);
    ~GameBoard();
	bool IsOccupied(int x, int y) const;
	void Update(char const* desc);
    bool HasChanged(char const* desc) const;
	friend std::ostream& operator<< (std::ostream& out, GameBoard const& gb);
private:
	void Translate(char const* desc);
    char* m_pDesc;
    std::vector< std::vector<bool> > m_board;
};

