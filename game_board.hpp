#pragma once

class GameBoard {
public:
    GameBoard();
    ~GameBoard();
    bool HasChanged(char const* desc) const;
private:
    char* m_desc;
    std::vector< std::vector<bool> > m_board;
};

