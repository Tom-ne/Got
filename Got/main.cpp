#include <iostream>
#include "CommandFactory.h"
#include "Object.h"
#include "ObjectTypes.h"

int main(int argc, char* argv[]) {
	std::cout << "Hello, World!" << std::endl;

	// Initialize the command factory
	CommandFactory::instance();
	try {
		CommandFactory::instance().runCommand(argv[1], std::vector<std::string>());
	}
	catch (const CommandNotFoundException& e) {
		std::cerr << "Command not found: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}