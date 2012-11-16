#include <control/control.hpp>
#include <model/game_state.hpp>
#include <core/game_state_eval.hpp>
#include <core/tetris_oracle.hpp>
#include <core/harmony.hpp>
#include <server/state.hpp>
#include <algorithm>
#include <queue>

Control::Control(zmq::context_t&, ServerInterface& si)
    : m_si(si)
{ }

void Control::Execute() {
    int weightCount = GetVarCount();
    float weights[] = {0.413627, -0.723112, -0.214118, 0.0572252, 
                        0.666599, -0.758947, 0.297272, 0.112221, 
                        -0.571268, -0.255358, -0.63998, -0.572694, 
                        0.870288, -0.777475, -0.13282, -0.329776, 
                        0.285539, -0.739775, -0.163327};
    Harmony h;
    for (int i = 0; i < weightCount; ++i)
        h.push_back(weights[i]);

    bool gameOver = false;
    GameState game;
    Tetromino const* best = NULL;
    bool inMoveSequence = false;
    std::vector<enum Tetromino::Move> const* sequence = NULL;
    while (!gameOver) {
        if (best != NULL && game.GetPieceInPlay() != NULL && inMoveSequence) {
            if (sequence != NULL) {
                delete sequence;
                sequence = NULL;
            }
            sequence = FindPath(game, *(game.GetPieceInPlay()), *best);
			if (sequence->size() > 0)
				ExecuteSequence(*sequence, game.GetCurrentPieceNumber());
        }
        std::queue<State const*> messages;
        State const* s = m_si.GetState();
        while (s != NULL) {
            messages.push(s);
            s = m_si.GetState();
        }
        while (!messages.empty()) {
            gameOver = s->ExecuteUpdates(game);
            delete s;
            messages.pop();
        }
        if (game.WasRowClearEvent() || game.PieceHasChanged()) {
            inMoveSequence = false;
        }
        if (game.GetPieceInPlay() != NULL && !inMoveSequence) {
            if (best != NULL) {
                delete best;
                best = NULL;
            }
            best = FindBestMove(game, h);
            GameState next = game;
            next.ApplyMove(*best);
            std::cout << next << std::endl;
            if (best != NULL) {
                inMoveSequence = true;
            }
        }
    }
}

void Control::ExecuteSequence(std::vector<enum Tetromino::Move> const& sequence,
                               int pieceId) {
    int sequenceSize = sequence.size();
    int executeCount = std::min(sequenceSize, 10);
    for (int i = 0; i < executeCount; ++i) {
        enum Tetromino::Move move = sequence.at(i);
        bool success = m_si.SendMove(move, pieceId);
        if (!success)
            return;
    }
}

