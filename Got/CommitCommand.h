#pragma once
#include "Command.h"
class CommitCommand : public Command
{
public:
	CommitCommand();

	void execute(const std::vector<std::string>& args) override;
private:
	const std::string commandName = "commit";
};

