#include "CommitCommand.h"
#include "Constants.h"
#include "CommandFactory.h"
#include "Commit.h"
#include "Index.h"
#include "Tree.h"
#include "FilesHelper.h"

namespace {
	bool initRegistered = []() {
		CommandFactory::instance().registerCommand("commit", []() -> std::unique_ptr<Command> {
			return std::make_unique<CommitCommand>();
			});
		return true;
		}();
}

CommitCommand::CommitCommand() {}

void CommitCommand::execute(const std::vector<std::string>& args)
{
	// Step 1: Check and get commit message
	if (args.empty()) {
		std::cerr << "No commit message provided." << std::endl;
		return;
	}
	std::string commitMessage = args[0];

	// Step 2: Check and get author
	if (args.size() < 2) {
		std::cerr << "No author provided." << std::endl;
		return;
	}
	std::string author = args[1];

	// Step 3: (Optional) get parent hashes (not implemented yet)
	std::vector<std::string> parentHashes;

	// Step 4: Convert index to a Tree object
	Index& index = Index::instance();

	Tree* rootTree = index.toObjectTree(".");
	if (!rootTree) {
		std::cerr << "Failed to convert index to tree." << std::endl;
		return;
	}

	// Step 5: Create commit and store it
	Commit commit(rootTree->hash(), parentHashes, author, commitMessage);
	commit.storeObject();

	std::cout << "Commit created: " << commit.hash() << std::endl;

	delete rootTree;
}
