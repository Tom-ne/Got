#include "CommitCommand.h"
#include "Constants.h"
#include "CommandFactory.h"
#include "Commit.h"
#include "Index.h"
#include "Tree.h"
#include "IndexEntry.h"
#include "FilesHelper.h"

namespace {
	bool initRegistered = []() {
		CommandFactory::instance().registerCommand("commit", []() -> std::unique_ptr<Command> {
			return std::make_unique<CommitCommand>();
			});
		return true;
		}();
}

CommitCommand::CommitCommand()
{
}

void CommitCommand::execute(const std::vector<std::string>& args)
{
	// step 1: get the commit messsage from args
	if (args.empty()) {
		std::cerr << "No commit message provided." << std::endl;
		return;
	}

	std::string commitMessage = args[0];

	// step 2: get the author from args
	if (args.size() < 2) {
		std::cerr << "No author provided." << std::endl;
		return;
	}

	std::string author = args[1];

	// step 3: parents hashes, empty for now
	std::vector<std::string> parentHashes;

	// step 4: get the entries from the index
	// get the index
	Index& index = Index::instance();
	// get the index entries
	std::vector<IndexEntry> indexEntries = index.getEntries();

	// step 5: create tree members.
	// iterate over the index entries, and create blobs and trees for each entry.
	// add the blobs and trees to the tree.

	// create a tree
	Tree tree;
	tree.setPath(".");
	for (auto entry : indexEntries) {
		Object* object;
		std::string name = entry.path;
		// if entry.path contains the constants::getFolderPath, continue
		if (entry.path.find(Constants::instance().getFolderPath()) != std::string::npos) {
			std::cerr << "Cannot add a folder inside the repository." << std::endl;
			continue;
		}

		// check if the entry is a folder or a file
		if (entry.mode == Constants::instance().getFolderMode()) {
			// create a tree and store it.
			Tree childTree;
			childTree.mapObjects(entry.path);
			childTree.storeObject();
			childTree.setPath(entry.path); // set the path of the tree
			std::cout << "Tree stored: " << childTree.hash() << std::endl;

			object = childTree.clone();
		}
		else if (entry.mode == Constants::instance().getFileMode()) {
			// create a blob and store it.
			Blob blob(FilesHelper::getFileContent(entry.path));
			blob.storeObject();
			std::cout << "Blob stored: " << blob.hash() << std::endl;

			// add the blob to the tree
			object = blob.clone();
		}
		else {
			std::cerr << "Unknown file type: " << entry.path << std::endl;
			return;
		}

		// add the object to the tree
		tree.addObject(name, object);
	}

	// step 6: store the tree
	tree.storeObject();

	// step 7: create the commit
	Commit commit(tree.hash(), parentHashes, author, commitMessage);
	commit.storeObject();
}
