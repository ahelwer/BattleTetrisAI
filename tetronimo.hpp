#pragma once

#include <string>
#include <iostream>

class Tetronimo {
public:
	Tetronimo(char type, int orient = 0, int col = 4, int row = 0);
	friend std::ostream& operator<< (std::ostream& out, Tetronimo const& t);
private:
	int m_orient;
	int m_col;
	int m_row;
};

