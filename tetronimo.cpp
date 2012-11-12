#include "tetronimo.hpp"

Tetronimo::Tetronimo(char type, int orient, int col, int row)
	: m_type(type), m_orient(orient), m_col(col), m_row(row)
{ }

std::ostream& operator<< (std::ostream& out, Tetronimo const& t) {
	out << t.m_type;
	return out;
}

