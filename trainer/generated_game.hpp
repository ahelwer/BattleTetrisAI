#pragma once

#include <model/tetronimo.hpp>
#include <util/vector.hpp>

class GeneratedGame {
public:
    GeneratedGame(int gameLength);
    ~GeneratedGame();
    void GenerateNewGame();
    Tetronimo const& GetNext(); 
    bool HasNext() const;
    Tetronimo const& PeekNext() const;
private:
    int m_gameLength;
    int m_currentIdx;
    std::vector<Tetronimo*> m_sequence;
};

