#pragma once

#include <string>
#include <iostream>
#include "my_vector.hpp"
#include "tetronimo.hpp"

class State {
public:
    State();
	State(int sequence, double timestamp);
	virtual ~State() { };
	int GetSequence() const;
	double GetTimestamp() const;
	virtual void ExecuteUpdates() const = 0;
	friend std::ostream& operator<< (std::ostream& out, State const& s);
protected:
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
	friend std::ostream& operator<< (std::ostream& out, GameBoardState const& s);
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
					Tetronimo* myTet, Tetronimo* theirTet,
					std::vector<Tetronimo>* queue);	
	~GamePieceState();
	virtual void ExecuteUpdates() const;
	friend std::ostream& operator<< (std::ostream& out, GamePieceState const& s);
private:
	Tetronimo* m_pMyTet;
	Tetronimo* m_pTheirTet;
	std::vector<Tetronimo>* m_pQueue;
};

class GameEnd : public State {
public:
	GameEnd(int sequence, double timestamp, bool won,
			int myScore, int theirScore);
	virtual void ExecuteUpdates() const;
	friend std::ostream& operator<< (std::ostream& out, GameEnd const& s);
private:
	bool m_won;
	int m_myScore;
	int m_theirScore;
};

class MatchEnd : public State {
public:
	virtual void ExecuteUpdates() const;
	friend std::ostream& operator<< (std::ostream& out, MatchEnd const& s);
};

class ErrorState : public State {
public:
	ErrorState(char const* errorMessage);
	virtual void ExecuteUpdates() const;
	friend std::ostream& operator<< (std::ostream& out, ErrorState const& s);
private:
	std::string m_errorMessage;
};

