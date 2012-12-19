#include <zmq.hpp>
#include <string>
#include <iostream>

#include <util/constants.hpp>
#include <server/server_interface.hpp>
#include <control/control.hpp>
#include <core/game_state_eval.hpp>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: <server ip> <match token>" << std::endl;
        return -1;
    }
    // Parses command line arguments
    std::string protocol (PROTOCOL);
    std::string commandPort (COMMAND_PORT);
    std::string statePort (STATE_PORT);
    std::string ip (argv[1]);
    std::string matchToken (argv[2]);
    std::string commandServer = protocol + ip + commandPort;
    std::string stateServer = protocol + ip + statePort;

    // Initializes and launches main loop
    zmq::context_t context (1);
    ServerInterface si (commandServer, stateServer, matchToken);
    Harmony const* weights = GetBestHarmony();
    Control top (context, si, *weights);
    std::cout << "Entering main loop." << std::endl;
    top.Execute();
    delete weights;

    return 0;
}

