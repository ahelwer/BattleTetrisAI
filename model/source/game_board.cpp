#include <model/game_board.hpp>
#include <util/constants.hpp>
#include <cstring>
#include <algorithm>

GameBoard::GameBoard()
	: m_board(COLS, std::vector<bool>(ROWS, false)), 
		m_pDesc(new char[BOARD_DESC_SIZE])
{ 
	for (int i = 0; i < BOARD_DESC_SIZE; ++i)
		m_pDesc[i] = '0';
}

GameBoard::GameBoard(char const* desc)
	: m_board(COLS, std::vector<bool>(ROWS, false)), 
    	m_pDesc(new char[BOARD_DESC_SIZE])
{
	for (int i = 0; i < BOARD_DESC_SIZE; ++i)
		m_pDesc[i] = '0';
	Translate(desc);
}

GameBoard::~GameBoard() {
	if (m_pDesc != NULL) {
		delete[] m_pDesc;
		m_pDesc = NULL;
	}
}

bool GameBoard::PushMove(Tetronimo const& tet) {
	Tetronimo t = tet;
	bool const* desc = t.GetDesc();
	int x = t.GetX();
	int pivotX = 2;
	int pivotY = 1;

	// Checks that tetronimo is in bounds along x axis
	bool inBounds = true;
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i <= 4; ++i) {
			bool isSet = desc[j*4+i];
			if (isSet) {
				int boardX = (x + i - pivotX);
				if (!InBounds(boardX, 5)) {
					inBounds = false;	
				}
			}
		}
	}
	if (!inBounds) {
		return false;
	}

	// Finds resting place of pivot
	int rest = ROWS-1;
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i <= 4; ++i) {
			bool isSet = desc[j*4+i];
			if (isSet) {
				int boardX = (x + i - pivotX);
				int depth = WellDepth(boardX);
				rest = std::min(depth-j, rest);
			}
		}
	}

	// Checks that tetronimo is in bounds along y axis
	inBounds = true;
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i <= 4; ++i) {
			bool isSet = desc[j*4+i];
			if (isSet) {
				int boardY = (rest + j - pivotY);
				if (!InBounds(5, boardY)) {
					inBounds = false;	
				}
			}
		}
	}
	if (!inBounds) {
		return false;
	}

	// Apply tetronimo to board
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i <= 4; ++i) {
			bool isSet = desc[j*4+i];
			if (isSet) {
				int boardX = (x + i - pivotX);
				int boardY = (rest + j - pivotY);
				m_board[boardX][boardY] = true;;
			}
		}
	}

	t.SetY(rest);
	m_moveStack.push_back(t);

	return true;
}

bool GameBoard::PopMove() {
	if (m_moveStack.size() == 0)
		return false;
	Tetronimo last = m_moveStack.back();
	bool const* desc = last.GetDesc();
	int x = last.GetX();
	int y = last.GetY();
	int pivotX = 2;
	int pivotY = 1;

	// De-apply move to board
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i <= 4; ++i) {
			bool isSet = desc[j*4+i];
			if (isSet) {
				int boardX = (x + i - pivotX);
				int boardY = (y + j - pivotY);
				m_board[boardX][boardY] = false;
			}
		}
	}

	m_moveStack.pop_back();
	return true;
}

BoardDesc const& GameBoard::GetBoardDesc() const {
	return m_board;
}

bool GameBoard::IsOccupied(int x, int y) const {
	if (InBounds(x, y))
		return m_board[x][y];
	else
		return false;
}

bool GameBoard::InBounds(int x, int y) const {
	return (x >= 0 && x < COLS && y >= 0 && y < ROWS);
}

int GameBoard::WellDepth(int x) const {
	if (!InBounds(x, 5))
		return -1;
	int depth = 0;
	for (int i = 0; i < ROWS; ++i) {
		if (m_board[x][i])
			break;
		++depth;
	}
	return depth;
}

void GameBoard::Update(char const* desc) {
	Translate(desc);
	if (m_pDesc != desc) // Overlapped memcpy is undefined
		memcpy((void*)m_pDesc, (void*)desc, BOARD_DESC_SIZE);
}

bool GameBoard::HasChanged(char const* desc) const {
	if (m_pDesc == desc)
		return false;
    return (memcmp((void*)desc, (void*)m_pDesc, BOARD_DESC_SIZE) != 0);
}

void GameBoard::Translate(char const* desc) {
	for (int i = 0; i < BOARD_DESC_SIZE; ++i) {
		char hex = desc[i];
		char byte = 0;
		if (hex > 64) // hex > 64, so is letter
			byte = (hex - 55); // A is char code 65, add 10
		else
			byte = (hex - 48); // 0 is char code 48
		// Reads hex bitmask into board
		int x = i*4 % COLS;
		int y = i*4 / COLS;
		m_board[x][y] = (byte & 1);
		x = (i*4+1) % COLS;
		y = (i*4+1) / COLS;
		m_board[x][y] = (byte & 2);
		x = (i*4+2) % COLS;
		y = (i*4+2) / COLS;
		m_board[x][y] = (byte & 4);
		x = (i*4+3) % COLS;
		y = (i*4+3) / COLS;
		m_board[x][y] = (byte & 8);
	}
}

std::ostream& operator<< (std::ostream& out, GameBoard const& gb) {
	out << gb.m_board;
	return out;
}

