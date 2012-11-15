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
                                const char* myBoard, 
                                const char* theirBoard,
                                int myPiece, int theirPiece,
                                std::vector<int>* myCleared,
                                std::vector<int>* theirCleared)
    : State(sequence, timestamp), m_pMyBoard(myBoard), 
        m_pTheirBoard(theirBoard), m_myPiece(myPiece),
        m_theirPiece(theirPiece), m_pMyCleared(myCleared),
        m_pTheirCleared(theirCleared)
{ }

GameBoardState::~GameBoardState() {
    if (m_pMyBoard != NULL)
        delete[] m_pMyBoard;
    if (m_pTheirBoard != NULL)
        delete[] m_pTheirBoard;
    if (m_pMyCleared != NULL) 
        delete m_pMyCleared;
    if (m_pTheirCleared != NULL)
        delete m_pTheirCleared;
}

bool GameBoardState::ExecuteUpdates(GameState& game) const {
    GameBoard& board = game.GetBoard();
    if (board.HasChanged(m_pMyBoard)) {
        board.Update(m_pMyBoard);
    }
    if (m_pMyCleared->size() > 0) {
        game.RegisterLastClearedRows(*m_pMyCleared);
    }
    if (m_pTheirCleared->size() > 0) {
        game.RegisterRowClearEvent();
    }
    //std::cout << (*this) << std::endl;
    return false;
}

std::ostream& operator<< (std::ostream& out, GameBoardState const& s) {
    out << "My cleared: " << *(s.m_pMyCleared) << ", ";
    out << "Their cleared: " << *(s.m_pTheirCleared);
    return out;
}

GamePieceState::GamePieceState(int sequence, double timestamp,
                                Tetromino* myTet, 
                                Tetromino* theirTet, 
                                int myNumber, int theirNumber,
                                std::vector<Tetromino>* queue)
    : State(sequence, timestamp), m_pMyTet(myTet), 
        m_pTheirTet(theirTet), m_myNumber(myNumber),
        m_theirNumber(theirNumber), m_pQueue(queue)
{ }

GamePieceState::~GamePieceState() {
    if (m_pMyTet != NULL)
        delete m_pMyTet;
    if (m_pTheirTet != NULL)
        delete m_pTheirTet;
    if (m_pQueue != NULL)
        delete m_pQueue;
}

bool GamePieceState::ExecuteUpdates(GameState& game) const {
    game.SetPieceInPlay(m_pMyTet);
    game.RegisterCurrentPieceNumber(m_myNumber);
    //std::cout << (*this) << std::endl;
    return false;
}

std::ostream& operator<< (std::ostream& out, GamePieceState const& s) {
    out << "My tetronimo: ";
    if (s.m_pMyTet != NULL)
        out << s.m_pMyTet->GetType();
    else
        out << "NULL";
    out << ", ";
    out << "Their tetronimo: ";
    if (s.m_pTheirTet != NULL)
        out << s.m_pTheirTet->GetType();
    else
        out << "NULL";
    out << ", ";
    out << "Queue: [";
    for (int i = 0; i < s.m_pQueue->size(); ++i)
        out << s.m_pQueue->at(i).GetType() << ", ";
    out << "]";
    return out;
}

GameEnd::GameEnd(int sequence, double timestamp, bool won,
                    int myScore, int theirScore) 
    : State(sequence, timestamp), m_won(won), m_myScore(myScore),
        m_theirScore(theirScore)
{ }

bool GameEnd::ExecuteUpdates(GameState& game) const {
    GameBoard& board = game.GetBoard();
    board.ClearBoard();
    game.SetPieceInPlay(NULL);
    game.RegisterCurrentPieceNumber(-1);
    return false;
}

std::ostream& operator<< (std::ostream& out, GameEnd const& s) {
    if (s.m_won)
        out << "We won!";
    else
        out << "Opponent won.";
    out << " ";
    out << "Score: " << s.m_myScore << "-" << s.m_theirScore;
    return out;
}

bool MatchEnd::ExecuteUpdates(GameState& game) const {
    std::cout << (*this) << std::endl;
    return true;
}

std::ostream& operator<< (std::ostream& out, MatchEnd const&) {
    out << "Game is over.";
    return out;
}

ErrorState::ErrorState(char const* errorMessage)
    : State(0, 0.0), m_errorMessage(errorMessage)
{ }

bool ErrorState::ExecuteUpdates(GameState& game) const {
    std::cout << (*this) << std::endl;
    return false;
}

std::ostream& operator<< (std::ostream& out, ErrorState const& s) {
    out << "ERROR: " << s.m_errorMessage;
    return out;
}

