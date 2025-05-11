#include <iostream>
#include "CommandFactory.h"
#include "Tree.h"
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>

int main() {
    std::cout << "Welcome to the Command Line Interface!" << std::endl;

    // Initialize the command factory
    CommandFactory::instance();

    // Infinite loop to read commands until "exit" is typed
    std::string command;
    while (true) {
        std::cout << "> "; // Show prompt
        std::getline(std::cin, command); // Read user input

        // Exit if the user types "exit"
        if (command == "exit") {
            std::cout << "Exiting program..." << std::endl;
            break;
        }

        try {
            // Split the command into command name and arguments
            std::stringstream ss(command);
            std::string cmd;
            std::vector<std::string> args;

            ss >> cmd;  // Get the command name
            std::string arg;
            while (ss >> arg) {
                args.push_back(arg);  // Collect arguments
            }

            // Execute the command
            CommandFactory::instance().runCommand(cmd, args);
        }
        catch (const CommandNotFoundException& e) {
            std::cerr << "Command not found: " << e.what() << std::endl;
        }
    }

    return 0;
}



//#include <iostream>
//#include "CommandFactory.h"
//#include "Tree.h"
//#include <filesystem>
//
//int main(int argc, char* argv[]) {
//	std::cout << "Hello, World!" << std::endl;
//
//	// Initialize the command factory
//	CommandFactory::instance();
//	try {
//		CommandFactory::instance().runCommand(argv[1], std::vector<std::string>());
//		// run the Add command with the argument "Command.h"
//		CommandFactory::instance().runCommand("add", { std::filesystem::current_path().string()});
//		CommandFactory::instance().runCommand("commit", { "Initial commit", "Tom" });
//	}
//	catch (const CommandNotFoundException& e) {
//		std::cerr << "Command not found: " << e.what() << std::endl;
//		return 1;
//	}
//
//	return 0;
//}