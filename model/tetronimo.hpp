#pragma once

#include <string>
#include <iostream>

class Tetronimo {
public:
	Tetronimo(char type, int orient = 0, 
				int col = 4, int row = 0);
	bool const* GetDesc() const;
	int GetX() const;
	int GetY() const;
	void SetX(int x);
	void SetY(int y);
	friend std::ostream& operator<< (std::ostream& out, Tetronimo const& t);
private:
	bool const* SetDesc(char type, int orient) const;
	char m_type;
	int m_orient;
	int m_col;
	int m_row;
	bool const* m_pDesc;
};

