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

bool GameBoardState::ExecuteUpdates() const {
	std::cout << (*this) << std::endl;
	return false;
}

std::ostream& operator<< (std::ostream& out, GameBoardState const& s) {
	out << "My cleared: " << *(s.m_pMyCleared) << ", ";
	out << "Their cleared: " << *(s.m_pTheirCleared);
	return out;
}

GamePieceState::GamePieceState(int sequence, double timestamp,
								Tetronimo* myTet, 
								Tetronimo* theirTet, 
								std::vector<Tetronimo>* queue)
	: State(sequence, timestamp), m_pMyTet(myTet), 
        m_pTheirTet(theirTet), m_pQueue(queue)
{ }

GamePieceState::~GamePieceState() {
	if (m_pMyTet != NULL)
		delete m_pMyTet;
	if (m_pTheirTet != NULL)
		delete m_pTheirTet;
	if (m_pQueue != NULL)
		delete m_pQueue;
}

bool GamePieceState::ExecuteUpdates() const {
	std::cout << (*this) << std::endl;
	return false;
}

std::ostream& operator<< (std::ostream& out, GamePieceState const& s) {
	out << "My tetronimo: ";
	if (s.m_pMyTet != NULL)
		out << *(s.m_pMyTet);
	else
		out << "NULL";
	out << ", ";
	out << "Their tetronimo: ";
	if (s.m_pTheirTet != NULL)
		out << *(s.m_pTheirTet);
	else
		out << "NULL";
	out << ", ";
	out << "Queue: " << *(s.m_pQueue);
	return out;
}

GameEnd::GameEnd(int sequence, double timestamp, bool won,
					int myScore, int theirScore) 
	: State(sequence, timestamp), m_won(won), m_myScore(myScore),
		m_theirScore(theirScore)
{ }

bool GameEnd::ExecuteUpdates() const {
    std::cout << (*this) << std::endl;
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

bool MatchEnd::ExecuteUpdates() const {
    std::cout << (*this) << std::endl;
	return true;
}

std::ostream& operator<< (std::ostream& out, MatchEnd const& s) {
	out << "Game is over.";
	return out;
}

ErrorState::ErrorState(char const* errorMessage)
	: State(0, 0.0), m_errorMessage(errorMessage)
{ }

bool ErrorState::ExecuteUpdates() const {
	std::cout << (*this) << std::endl;
	return false;
}

std::ostream& operator<< (std::ostream& out, ErrorState const& s) {
	out << "ERROR: " << s.m_errorMessage;
	return out;
}

