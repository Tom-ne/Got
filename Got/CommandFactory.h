#pragma once
#include "Command.h"
#include "InitCommand.h"
#include "CommandNotFoundException.h"
#include <functional>
#include <unordered_map>

class CommandFactory
{
public:
	using CreatorFunc = std::function<Command*()>;

	static CommandFactory& instance();

	void runCommand(const std::string& commandName, const std::vector<std::string>& args);

	void registerCommand(const std::string& commandName, std::function<std::unique_ptr<Command>()> commandCreator);
private:
	CommandFactory() = default;
	std::unordered_map<std::string, std::function<std::unique_ptr<Command>()>> commandMap;
};

