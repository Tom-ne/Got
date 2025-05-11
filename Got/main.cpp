#include <iostream>
#include "CommandFactory.h"
#include "Tree.h"
#include <filesystem>
#include "Index.h"

int main(int argc, char* argv[]) {
	std::cout << "Hello, World!" << std::endl;

	// Initialize the command factory
	CommandFactory::instance();
	try {
		Index& index = Index::instance();
		index.load();
		CommandFactory::instance().runCommand(argv[1], std::vector<std::string>());
		//CommandFactory::instance().runCommand("add", { std::filesystem::current_path().string()});
		//CommandFactory::instance().runCommand("commit", { "Initial commit", "Tom" });
		CommandFactory::instance().runCommand("log", {});
	}
	catch (const CommandNotFoundException& e) {
		std::cerr << "Command not found: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}