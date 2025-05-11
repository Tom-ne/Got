#include "LogCommand.h"
#include "CommandFactory.h"
#include "Constants.h"
#include <filesystem>
#include "FilesHelper.h"
#include <iostream>
#include "Commit.h"

namespace {
	bool addRegistered = []() {
		CommandFactory::instance().registerCommand("log", []() -> std::unique_ptr<Command> {
			return std::make_unique<LogCommand>();
			});
		return true;
		}();
}

LogCommand::LogCommand()
{
}

void LogCommand::execute(const std::vector<std::string>& args)
{
    // Step 1: Check if the repository is initialized
    if (!std::filesystem::exists(Constants::instance().getFolderPath())) {
        std::cerr << "Repository not initialized. Please run 'init' command." << std::endl;
        return;
    }

    // Step 2: Check if HEAD exists and read its content
    std::string headContent = FilesHelper::getFileContent(Constants::instance().getHeadPath());
    if (headContent.empty()) {
        std::cerr << "No commits found. Please run 'commit' command." << std::endl;
        return;
    }

    // Step 3: Get the commit hash from HEAD (reference path)
    std::string refPath = headContent.substr(5); // Skip "ref: "
    std::string branchName = refPath.substr(refPath.find_last_of("/") + 1);
    std::string branchFilePath = Constants::instance().getHeadsPath() + "/" + branchName;
    std::string commitHash = FilesHelper::getFileContent(branchFilePath);
    if (commitHash.empty()) {
        std::cerr << "No commits found. Please run 'commit' command." << std::endl;
        return;
    }

    // Step 4: Start recursion for commit log
    printCommitLog(commitHash);
}

void LogCommand::printCommitLog(const std::string& commitHash)
{
    // Step 5: Read the commit object (hash)
    std::string firstChars = commitHash.substr(0, Constants::instance().getHashCount());
    std::string restOfHash = commitHash.substr(Constants::instance().getHashCount());
    std::string objectDir = Constants::instance().getObjectsPath() + "/" + firstChars;
    std::string objectFilePath = objectDir + "/" + restOfHash;

    std::string commitContent = FilesHelper::getFileContent(objectFilePath);
    if (commitContent.empty()) {
        std::cerr << "Commit not found: " << commitHash << std::endl;
        return;
    }

    // Step 6: Parse the commit data
    std::string treeHash = commitContent.substr(commitContent.find("tree ") + 5, commitContent.find("\n", commitContent.find("tree ")) - commitContent.find("tree ") - 5);
    std::string author = commitContent.substr(commitContent.find("author ") + 7, commitContent.find("\n", commitContent.find("author ")) - commitContent.find("author ") - 7);
    std::string message = commitContent.substr(commitContent.find("message ") + 8, commitContent.find("\n", commitContent.find("message ")) - commitContent.find("message ") - 8);

    std::cout << "Commit: " << commitHash << std::endl;
    std::cout << "Author: " << author << std::endl;
    std::cout << "Message: " << message << std::endl;
    std::cout << "---------------------------------" << std::endl;

    // Step 7: Handle parent commit (if any)
    std::string parentHash = getParentHash(commitContent);
    if (!parentHash.empty()) {
        // Recursively call for the parent commit
        printCommitLog(parentHash);
    }
}

std::string LogCommand::getParentHash(const std::string& commitContent)
{
    // Extract the first parent hash (if available)
    size_t parentPos = commitContent.find("parent ");
    if (parentPos == std::string::npos) {
        return ""; // No parent found
    }

    size_t parentEndPos = commitContent.find("\n", parentPos);
    return commitContent.substr(parentPos + 7, parentEndPos - parentPos - 7); // Extract the hash
}
