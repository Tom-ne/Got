#pragma once
#include <stdexcept>
#include <string>
#include <iostream>
class CommandNotFoundException : public std::runtime_error
{
	public:
	CommandNotFoundException(const std::string& commandName)
		: std::runtime_error("Command not found: " + commandName) {
		std::cerr << "Error: " << what() << std::endl;
	}
};