#pragma once
#include "Command.h"
class LogCommand : public Command
{
public:
	LogCommand();

	void execute(const std::vector<std::string>& args) override;
private:
	const std::string commandName = "log";

	void printCommitLog(const std::string& commitHash);
	std::string getParentHash(const std::string& commitContent);
};

