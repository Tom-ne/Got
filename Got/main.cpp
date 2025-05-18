#include <iostream>
#include "CommandFactory.h"
#include "Tree.h"
#include <filesystem>
#include "Index.h"

int main(int argc, char* argv[]) {
	// Initialize the command factory
	CommandFactory::instance();
	try {
		CommandFactory::instance().runCommand(argv[1], std::vector<std::string>());
		CommandFactory::instance().runCommand("add", { std::filesystem::current_path().string() + "/testProject"});
		CommandFactory::instance().runCommand("commit", { "Initial commit", "Tom" });
		CommandFactory::instance().runCommand("log", {});
	}
	catch (const CommandNotFoundException& e) {
		std::cerr << "Command not found: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}