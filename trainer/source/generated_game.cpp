#include <trainer/generated_game.hpp>
#include <algorithm>
#include <cstdlib>

GeneratedGame::GeneratedGame(int gameLength)
    : m_gameLength(gameLength)
{ 
    for (int i = 0; i < m_gameLength; ++i)
        m_sequence.push_back(NULL);
    GenerateNewGame();
}

GeneratedGame::~GeneratedGame() {
    for (int i = 0; i < m_gameLength; ++i) {
        if (m_sequence.at(i) != NULL) {
            delete m_sequence.at(i);
            m_sequence[i] = NULL;
        }
    }
}

void GeneratedGame::GenerateNewGame() {
    std::string tetTypes ("OISZLJT");
    for (int i = 0; i < m_gameLength; ++i) {
        char type = tetTypes.at(rand() % tetTypes.size());
        Tetronimo* newTet = new Tetronimo(type);
        if (m_sequence.at(i) != NULL) {
            delete m_sequence.at(i);
        }
        m_sequence[i] = newTet;
    }
}

int GeneratedGame::GameLength() const {
    return m_gameLength;
}

Tetronimo const& GeneratedGame::GetPiece(int idx) const {
    Tetronimo* ret = m_sequence.at(idx);
    return (*ret);
}

std::vector<Tetronimo> const* GeneratedGame::GetQueue(int idx) const {
    std::vector<Tetronimo>* queue = new std::vector<Tetronimo>();
    for (int i = idx+1; i < std::min(m_gameLength, i+5); ++i)
        queue->push_back(*m_sequence.at(i));
    return queue;
}

