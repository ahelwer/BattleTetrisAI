#pragma once

#include <vector>

class GameBoard {
public:
    GameBoard();
    ~GameBoard();
    bool HasChanged(char const* desc) const;
	void Translate(char const* desc);
private:
    char* m_desc;
    std::vector< std::vector<bool> > m_board;
};

