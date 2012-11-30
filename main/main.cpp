#include <zmq.hpp>
#include <string>
#include <iostream>

#include <util/constants.hpp>
#include <server/server_interface.hpp>
#include <control/control.hpp>
#include <core/game_state_eval.hpp>

int main(int argc, char* argv[]) {
	std::string opponent;
    if (argc == 3) {
		opponent = std::string("Test Client");
    }
	else if (argc == 4) {
		opponent = "Team " + std::string(argv[3]);
	}

    // Parses command line arguments
    std::string protocol (PROTOCOL);
    std::string commandPort (COMMAND_PORT);
    std::string statePort (STATE_PORT);
    std::string ip (argv[1]);
    std::string matchToken (argv[2]);
    std::string commandServer = protocol + ip + commandPort;
    std::string stateServer = protocol + ip + statePort;
    std::cout << opponent << std::endl;

    // Initializes and launches main loop
    zmq::context_t context (1);
    ServerInterface si (commandServer, stateServer, matchToken, opponent);
    Harmony const* weights = GetBestHarmony();
    Control top (context, si, *weights);
    std::cout << "Entering main loop." << std::endl;
    top.Execute();
    delete weights;

    return 0;
}

