#include <server/state.hpp>

State::State()
    : m_sequence(-1), m_timestamp(0.0)
{ }

State::State(int sequence, double timestamp)
    : m_sequence(sequence), m_timestamp(timestamp)
{ }

int State::GetSequence() const {
    return m_sequence;
}

double State::GetTimestamp() const {
    return m_timestamp;
}

std::ostream& operator<< (std::ostream& out, State const& s) {
    out << "Sequence: " << s.m_sequence << std::endl;
    out << "Timestamp: " << s.m_timestamp << std::endl;
    return out;
}

GameBoardState::GameBoardState(int sequence, double timestamp,
                                const char* board, int piece,
                                std::vector<int>* cleared)
    : State(sequence, timestamp), m_pBoard(board), 
        m_piece(piece), m_pCleared(cleared)
{ }

GameBoardState::~GameBoardState() {
    if (m_pBoard != NULL) {
        delete[] m_pBoard;
        m_pBoard = NULL;
    }
    if (m_pCleared != NULL) {
        delete m_pCleared;
        m_pCleared = NULL;
    }
}

bool GameBoardState::ExecuteUpdates(GameState& game) const {
    GameBoard& board = game.GetBoard();
    if (m_pBoard != NULL && board.HasChanged(m_pBoard)) {
        board.Update(m_pBoard);
    }
    if (m_pCleared != NULL && m_pCleared->size() > 0) {
        game.SetLastClearedRows(*m_pCleared);
    }
    //std::cout << (*this) << std::endl;
    return false;
}

bool GameBoardState::ShouldTerminate() const {
    return false;
}

std::ostream& operator<< (std::ostream& out, GameBoardState const& s) {
    out << "My cleared: " << *(s.m_pCleared);
    return out;
}

GamePieceState::GamePieceState(int sequence, double timestamp,
                                Tetromino* tet, int number,
                                std::vector<Tetromino>* queue)
    : State(sequence, timestamp), m_pTet(tet), 
        m_number(number), m_pQueue(queue)
{ }

GamePieceState::~GamePieceState() {
    if (m_pTet != NULL) {
        delete m_pTet;
        m_pTet = NULL;
    }
    if (m_pQueue != NULL) {
        delete m_pQueue;
        m_pQueue = NULL;
    }
}

bool GamePieceState::ExecuteUpdates(GameState& game) const {
    game.SetPieceInPlay(m_pTet);
    game.SetCurrentPieceNumber(m_number);
    //std::cout << (*this) << std::endl;
    return false;
}

bool GamePieceState::ShouldTerminate() const {
    return false;
}

std::ostream& operator<< (std::ostream& out, GamePieceState const& s) {
    out << "My tetronimo: ";
    if (s.m_pTet != NULL)
        out << s.m_pTet->GetType();
    else
        out << "NULL";
    out << ", ";
    out << "Queue: ";
    if (s.m_pQueue != NULL) {
        out << *(s.m_pQueue);
    }
    return out;
}

GameEnd::GameEnd(int sequence, double timestamp, bool won, int myScore) 
    : State(sequence, timestamp), m_won(won), m_myScore(myScore)
{ }

bool GameEnd::ExecuteUpdates(GameState& game) const {
    GameBoard& board = game.GetBoard();
    board.ClearBoard();
    game.SetPieceInPlay(NULL);
    game.SetCurrentPieceNumber(-1);
    std::cout << (*this) << std::endl;
    return false;
}

bool GameEnd::ShouldTerminate() const {
    return false;
}

std::ostream& operator<< (std::ostream& out, GameEnd const& s) {
    if (s.m_won)
        out << "We won! ";
    else
        out << "Opponent won. ";
    out << "Score: " << s.m_myScore;
    return out;
}

bool MatchEnd::ExecuteUpdates(GameState&) const {
    std::cout << (*this) << std::endl;
    return true;
}

bool MatchEnd::ShouldTerminate() const {
    return true;
}

std::ostream& operator<< (std::ostream& out, MatchEnd const&) {
    out << "Game is over.";
    return out;
}

ErrorState::ErrorState(char const* errorMessage)
    : State(0, 0.0), m_errorMessage(errorMessage)
{ }

bool ErrorState::ExecuteUpdates(GameState&) const {
    std::cout << (*this) << std::endl;
    return false;
}

bool ErrorState::ShouldTerminate() const {
    return false;
}

std::ostream& operator<< (std::ostream& out, ErrorState const& s) {
    out << "ERROR: " << s.m_errorMessage;
    return out;
}

