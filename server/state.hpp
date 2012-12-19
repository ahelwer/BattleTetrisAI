#pragma once

#include <string>
#include <iostream>
#include <util/vector.hpp>
#include <model/tetromino.hpp>
#include <model/game_state.hpp>

class State {
public:
    State();
    State(int sequence, double timestamp);
    virtual ~State() { };
    int GetSequence() const;
    double GetTimestamp() const;
    virtual bool ExecuteUpdates(GameState& game) const = 0;
    virtual bool ShouldTerminate() const = 0;
    friend std::ostream& operator<< (std::ostream& out, State const& s);
protected:
    int m_sequence;
    double m_timestamp;
};

class GameBoardState : public State {
public:
    GameBoardState(int sequence, double timestamp,
                    char const* board, int piece,
                    std::vector<int>* cleared);
    ~GameBoardState();
    virtual bool ExecuteUpdates(GameState& game) const;
    virtual bool ShouldTerminate() const;
    friend std::ostream& operator<< (std::ostream& out, GameBoardState const& s);
private:
    char const* m_pBoard;
    int m_piece;
    std::vector<int>* m_pCleared;
};

class GamePieceState : public State {
public:
    GamePieceState(int sequence, double timestamp,
                    Tetromino* tet, int number,
                    std::vector<Tetromino>* queue);    
    ~GamePieceState();
    virtual bool ExecuteUpdates(GameState& game) const;
    virtual bool ShouldTerminate() const;
    friend std::ostream& operator<< (std::ostream& out, GamePieceState const& s);
private:
    Tetromino* m_pTet;
    int m_number;
    std::vector<Tetromino>* m_pQueue;
};

class GameEnd : public State {
public:
    GameEnd(int sequence, double timestamp, bool won, int myScore);
    virtual bool ExecuteUpdates(GameState& game) const;
    virtual bool ShouldTerminate() const;
    friend std::ostream& operator<< (std::ostream& out, GameEnd const& s);
private:
    bool m_won;
    int m_myScore;
};

class MatchEnd : public State {
public:
    virtual bool ExecuteUpdates(GameState& game) const;
    virtual bool ShouldTerminate() const;
    friend std::ostream& operator<< (std::ostream& out, MatchEnd const& s);
};

class ErrorState : public State {
public:
    ErrorState(char const* errorMessage);
    virtual bool ExecuteUpdates(GameState& game) const;
    virtual bool ShouldTerminate() const;
    friend std::ostream& operator<< (std::ostream& out, ErrorState const& s);
private:
    std::string m_errorMessage;
};

