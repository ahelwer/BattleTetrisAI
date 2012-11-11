#include "control.hpp"
#include "state.hpp"

Control::Control(zmq::context_t& context, ServerInterface& si)
	: m_si(si)
{ }

void Control::Execute() {
    while (true) {
        State const* s = m_si.GetState();
        s->ExecuteUpdates();
        delete s;
    }
}

