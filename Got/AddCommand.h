#pragma once
#include "Command.h"
#include "Object.h"

class AddCommand : public Command
{
public:
	AddCommand();

	void execute(const std::vector<std::string>& args) override;
private:
	const std::string commandName = "add";

	void addToIndex(std::string path, ObjectTypes::ObjectType type, Object* object);
};

