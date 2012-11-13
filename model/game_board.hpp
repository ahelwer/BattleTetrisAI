#pragma once

#include <util/vector.hpp>
#include <model/tetronimo.hpp>
#include <iostream>

class GameBoard {
public:
    GameBoard();
    GameBoard(char const* desc);
    ~GameBoard();
	// Applies move to board, takes ownership of Tetronimo object
	bool PushMove(Tetronimo const* t);
	// Reverts last move to board and deletes Tetronimo object
	bool PopMove();
	bool IsOccupied(unsigned x, unsigned y) const;
	void Update(char const* desc);
    bool HasChanged(char const* desc) const;
	friend std::ostream& operator<< (std::ostream& out, GameBoard const& gb);
private:
	void Translate(char const* desc);
    char* m_pDesc;
    std::vector< std::vector<bool> > m_board;
	std::vector<Tetronimo const*> m_moveStack;
};

