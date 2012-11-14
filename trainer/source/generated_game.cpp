#include <trainer/generated_game.hpp>
#include <cstdlib>

GeneratedGame::GeneratedGame(int gameLength)
    : m_gameLength(gameLength), m_currentIdx(0)
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

Tetronimo const& GeneratedGame::GetNext() {
    Tetronimo* ret = m_sequence.at(m_currentIdx);
    ++m_currentIdx;
    return (*ret);
}

bool GeneratedGame::HasNext() const {
    return (m_currentIdx < m_gameLength-1);
}

Tetronimo const& GeneratedGame::PeekNext() const {
    Tetronimo* ret = m_sequence.at(m_currentIdx+1);
    return (*ret);
}

void GeneratedGame::Reset() {
    m_currentIdx = 0;
}

