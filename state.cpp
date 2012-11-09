#include "state.hpp"
#include <iostream>

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
	delete[] m_pMyBoard;
	delete[] m_pTheirBoard;
	delete m_pMyCleared;
	delete m_pTheirCleared;
}

void GameBoardState::ExecuteUpdates() const {
	// Do stuff here
}

GamePieceState::GamePieceState(int sequence, double timestamp,
								Tetronimo* tet, 
								std::vector<Tetronimo>* queue)
	: State(sequence, timestamp), m_pTet(tet), m_pQueue(queue)
{ }

GamePieceState::~GamePieceState() {
	delete m_pTet;
	delete m_pQueue;
}

void GamePieceState::ExecuteUpdates() const {
	// Do stuff here
}

GameEnd::GameEnd(int sequence, double timestamp, bool won,
					int myScore, int theirScore) 
	: State(sequence, timestamp), m_won(won), m_myScore(myScore),
		m_theirScore(theirScore)
{ }

void GameEnd::ExecuteUpdates() const {
	// Do stuff here
}

void MatchEnd::ExecuteUpdates() const {
	// Do stuff here
}

ErrorState::ErrorState(char const* errorMessage)
	: State(0, 0.0), m_errorMessage(errorMessage)
{ }

void ErrorState::ExecuteUpdates() const {
	std::cout << "ERROR: " << m_errorMessage << std::endl;
}

