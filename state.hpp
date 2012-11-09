#pragma once

#include <vector>
#include <string>
#include "tetronimo.hpp"

class State {
public:
	State(int sequence, double timestamp);
	virtual ~State() { };
	int GetSequence() const;
	double GetTimestamp() const;
	virtual void ExecuteUpdates() const = 0;
private:
	int m_sequence;
	double m_timestamp;
};

class GameBoardState : public State {
public:
	GameBoardState(int sequence, double timestamp,
					char const* myBoard, char const* theirBoard,
					int myPiece, int theirPiece,
					std::vector<int>* myCleared,
					std::vector<int>* theirCleared);
	~GameBoardState();
	virtual void ExecuteUpdates() const;
private:
	char const* m_pMyBoard;
	char const* m_pTheirBoard;
	int m_myPiece;
	int m_theirPiece;
	std::vector<int>* m_pMyCleared;
	std::vector<int>* m_pTheirCleared;
};

class GamePieceState : public State {
public:
	GamePieceState(int sequence, double timestamp,
					Tetronimo* tet, 
					std::vector<Tetronimo>* queue);	
	~GamePieceState();
	virtual void ExecuteUpdates() const;
private:
	Tetronimo* m_pTet;
	std::vector<Tetronimo>* m_pQueue;
};

class GameEnd : public State {
public:
	GameEnd(int sequence, double timestamp, bool won,
			int myScore, int theirScore);
	virtual void ExecuteUpdates() const;
private:
	bool m_won;
	int m_myScore;
	int m_theirScore;
};

class MatchEnd : public State {
public:
	virtual void ExecuteUpdates() const;
};

class ErrorState : public State {
public:
	ErrorState(char const* errorMessage);
	virtual void ExecuteUpdates() const;
private:
	std::string m_errorMessage;
};

