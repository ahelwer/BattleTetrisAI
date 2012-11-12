#include <control/control.hpp>
#include <server/state.hpp>

Control::Control(zmq::context_t&, ServerInterface& si)
	: m_si(si)
{ }

void Control::Execute() {
	bool gameOver = false;
    while (!gameOver) {
        State const* s = m_si.GetState();
        gameOver = s->ExecuteUpdates();
        delete s;
    }
}

