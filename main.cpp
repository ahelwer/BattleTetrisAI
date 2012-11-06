#include <zmq.hpp>
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cout << "ERROR: Usage <server> <token>." << std::endl;
		return 0;
	}
	// Connects to and authenticates with gameserver
	std::string server = std::string(argv[1]) + ":5557";
	std::string matchToken (argv[2]);
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REQ);
	std::cout << "Connecting to server " << server << std::endl;
	socket.connect(server.c_str());
	std::cout << "Sending token " << matchToken << std::endl;
	zmq::message_t request (matchToken.size()+1);
	memcpy((void*)request.data(), matchToken.c_str(), matchToken.size());
	socket.send(request);

	std::cout << "Waiting for reply." << std::endl;
	zmq::message_t reply;
	socket.recv(&reply);
	std::cout << reply.data() << std::endl;

	return 0;
}

