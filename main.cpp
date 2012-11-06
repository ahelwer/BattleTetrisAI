#include <zmq.hpp>
#include <string>
#include <iostream>
#include <json/json.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "ERROR: Usage <server> <match token>" << std::endl;
		return 0;
	}

	zmq::context_t context(1);

	std::string protocol ("tcp://");
	std::string commandPort (":5557");
	std::string statePort (":5556");
	std::string ip (argv[1]);
	std::string matchToken (argv[2]);
	std::string commandServer = protocol + ip + commandPort;
	std::string stateServer = protocol + ip + statePort;

	std::cout << "Connecting to command server " << commandServer << std::endl;
	zmq::socket_t command (context, ZMQ_REQ);
	command.connect(commandServer.c_str());

	zmq::message_t reply;
	command.recv(&reply);
	std::string token ((char*)reply.data());
	std::cout << "Received token " << token << std::endl;

	return 0;
}

