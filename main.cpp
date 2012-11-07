#include <zmq.hpp>
#include <string>
#include <iostream>
#include "server_interface.hpp"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "ERROR: Usage <server> <match token>" << std::endl;
		return 0;
	}

    // Parses command line arguments
	std::string protocol ("tcp://");
	std::string commandPort (":5557");
	std::string statePort (":5556");
	std::string ip (argv[1]);
	std::string matchToken (argv[2]);
	std::string commandServer = protocol + ip + commandPort;
	std::string stateServer = protocol + ip + statePort;

    // Creates zmq context
	zmq::context_t context(1);

    // Creates and initializes server interface
    ServerInterface si (context, commandServer, stateServer, matchToken);
    si.Initialize();

	return 0;
}

