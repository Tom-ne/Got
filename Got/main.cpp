#include <iostream>
#include "CommandFactory.h"
#include "Tree.h"
#include <filesystem>

int main(int argc, char* argv[]) {
	std::cout << "Hello, World!" << std::endl;

	// Initialize the command factory
	CommandFactory::instance();
	try {
		CommandFactory::instance().runCommand(argv[1], std::vector<std::string>());
		// run the Add command with the argument "Command.h"
		CommandFactory::instance().runCommand("add", { std::filesystem::current_path().string()});
		CommandFactory::instance().runCommand("commit", { "Initial commit", "Tom" });
	}
	catch (const CommandNotFoundException& e) {
		std::cerr << "Command not found: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}