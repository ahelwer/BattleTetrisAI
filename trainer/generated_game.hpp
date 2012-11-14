#pragma once

#include <model/tetronimo.hpp>
#include <util/vector.hpp>

class GeneratedGame {
public:
    GeneratedGame(int gameLength);
    ~GeneratedGame();
    void GenerateNewGame();
    Tetronimo const& GetPiece(int idx) const; 
    int GameLength() const;
private:
    int m_gameLength;
    std::vector<Tetronimo*> m_sequence;
};

