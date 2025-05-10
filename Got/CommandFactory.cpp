#include "CommandFactory.h"

CommandFactory& CommandFactory::instance()
{
    static CommandFactory factory;
    return factory;
}

void CommandFactory::runCommand(const std::string& commandName, const std::vector<std::string>& args)
{
    auto it = commandMap.find(commandName);
    if (it != commandMap.end()) {
        std::unique_ptr<Command> command = it->second();
        command->execute(args);
    }
    else {
        std::cerr << "Error: Command '" << commandName << "' not found!" << std::endl;
    }
}

void CommandFactory::registerCommand(const std::string& commandName, std::function<std::unique_ptr<Command>()> commandCreator)
{
    commandMap[commandName] = commandCreator;
}
