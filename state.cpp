#include "state.hpp"
#include <iostream>

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

void GameBoardState::ExecuteUpdates() const {
	// Do stuff here
    std::cout << "Board" << std::endl;
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

void GamePieceState::ExecuteUpdates() const {
	// Do stuff here
    std::cout << "Piece" << std::endl;
}

GameEnd::GameEnd(int sequence, double timestamp, bool won,
					int myScore, int theirScore) 
	: State(sequence, timestamp), m_won(won), m_myScore(myScore),
		m_theirScore(theirScore)
{ }

void GameEnd::ExecuteUpdates() const {
	// Do stuff here
    std::cout << "Game end" << std::endl;
}

void MatchEnd::ExecuteUpdates() const {
	// Do stuff here
    std::cout << "Match end" << std::endl;
}

ErrorState::ErrorState(char const* errorMessage)
	: State(0, 0.0), m_errorMessage(errorMessage)
{ }

void ErrorState::ExecuteUpdates() const {
	std::cout << "ERROR: " << m_errorMessage << std::endl;
}

