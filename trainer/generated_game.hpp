#pragma once

#include <model/tetromino.hpp>
#include <util/vector.hpp>

/* *
 * Class GeneratedGame
 *
 * Generates a random Tetris game of the specified size.
 * */
class GeneratedGame {
public:
    GeneratedGame(int gameLength);
    ~GeneratedGame();
    // Generates new random game
    void GenerateNewGame();
    // Get Tetromino at specified index in sequence
    Tetromino const& GetPiece(int idx) const; 
    // Gets piece queue at specified index in sequence
    std::vector<Tetromino> const* GetQueue(int idx) const;
    // Returns game length
    int GameLength() const;
private:
    int m_gameLength;
    std::vector<Tetromino*> m_sequence;
};

