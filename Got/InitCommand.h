#pragma once
#include "Command.h"
#include <iostream>

class InitCommand : public Command
{
	public :
		InitCommand();

		void execute(const std::vector<std::string>& args) override;
private:
	const std::string commandName = "init";
	void createFolders();
	void createFiles();
};

