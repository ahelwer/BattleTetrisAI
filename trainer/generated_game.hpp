#pragma once

#include <model/tetromino.hpp>
#include <util/vector.hpp>
#include <queue>

class GeneratedGame {
public:
    GeneratedGame(int gameLength);
    ~GeneratedGame();
    void GenerateNewGame();
    Tetromino const& GetPiece(int idx) const; 
    std::queue<Tetromino> const* GetQueue(int idx) const;
    int GameLength() const;
private:
    int m_gameLength;
    std::vector<Tetromino*> m_sequence;
};

