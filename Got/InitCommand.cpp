#include "initCommand.h"
#include "CommandFactory.h"
#include "Constants.h"
#include "FilesHelper.h"

namespace {
    bool initRegistered = []() {
        CommandFactory::instance().registerCommand("init", []() -> std::unique_ptr<Command> {
            return std::make_unique<InitCommand>();
            });
        return true;
        }();
}

InitCommand::InitCommand()
{
}

void InitCommand::execute(const std::vector<std::string>& args)
{
    // check if the folder exists
    if (FilesHelper::doesFileExist(Constants::instance().getFolderPath()))
	{
		std::cout << "Folder already exists: " << Constants::instance().getFolderPath() << std::endl;
		return;
	}

    // create the folders
	createFolders();

	// create the files
	createFiles();

	std::cout << "Initialized repository at " << Constants::instance().getFolderPath() << std::endl;
}

void InitCommand::createFolders()
{
    FilesHelper::createFolder(Constants::instance().getFolderPath());
    FilesHelper::createFolder(Constants::instance().getObjectsPath());
    FilesHelper::createFolder(Constants::instance().getRefsPath());
    FilesHelper::createFolder(Constants::instance().getHeadsPath());
}

void InitCommand::createFiles()
{
    FilesHelper::createFile(Constants::instance().getHeadPath());

    FilesHelper::writeToFile(Constants::instance().getHeadPath(), "ref: refs/heads/master");
}
