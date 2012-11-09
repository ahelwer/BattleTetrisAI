#pragma once

#include <string>

class Tetronimo {
public:
	Tetronimo(char type, int orient = 0, int col = 4, int row = 0);
private:
	int m_orient;
	int m_col;
	int m_row;
};

