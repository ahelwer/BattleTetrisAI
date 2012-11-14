#pragma once

#include <string>
#include <iostream>

class Tetronimo {
public:
    Tetronimo(char type, int orient = 0, 
                int col = 5, int row = 1);
    bool const* GetDesc() const;
    char GetType() const;
    int GetX() const;
    int GetY() const;
    void SetX(int x);
    void SetY(int y);
    void SetOrient(int orient);
    void RotateLeft();
    void RotateRight();
    friend std::ostream& operator<< (std::ostream& out, Tetronimo const& t);
private:
    bool const* GetDesc(char type, int orient) const;
    char m_type;
    int m_orient;
    int m_col;
    int m_row;
    bool const* m_pDesc;
};

