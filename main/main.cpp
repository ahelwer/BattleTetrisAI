#include <zmq.hpp>
#include <string>
#include <iostream>

#include <util/constants.hpp>
#include <server/server_interface.hpp>
#include <control/control.hpp>

int main(int argc, char* argv[]) {
    /*
    if (argc != 3) {
        std::cout << "ERROR: Usage <server> <match token>" << std::endl;
        return 0;
    }
    */

    // Parses command line arguments
    std::string protocol (PROTOCOL);
    std::string commandPort (COMMAND_PORT);
    std::string statePort (STATE_PORT);
    std::string ip (argv[1]);
    std::string matchToken (argv[2]);
    std::string commandServer = protocol + ip + commandPort;
    std::string stateServer = protocol + ip + statePort;
    std::string opponent = "Team " + std::string(argv[3]);
    std::cout << opponent << std::endl;

    // Create zmq context
    zmq::context_t context(1);

    // Create and initializes server interface
    ServerInterface si (commandServer, stateServer, matchToken, opponent);
    Control top (context, si);
    top.Execute();

    return 0;
}

