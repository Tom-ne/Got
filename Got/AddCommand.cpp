#include "AddCommand.h"
#include "CommandFactory.h"
#include "FilesHelper.h"
#include "Blob.h"
#include "Tree.h"
#include "Index.h"
#include "IndexEntry.h"

namespace {
	bool addRegistered = []() {
		CommandFactory::instance().registerCommand("add", []() -> std::unique_ptr<Command> {
			return std::make_unique<AddCommand>();
		});
		return true;
		}();
}

AddCommand::AddCommand()
{
}

void AddCommand::execute(const std::vector<std::string>& args)
{
	// steps:
	// 1. get the first argument given
	
	// check if the argument is empty
	if (args.empty()) {
		std::cerr << "No arguments provided." << std::endl;
		return;
	}
	
	std::string filePath = args[0];

	// check if the file exists
	if (!FilesHelper::doesFileExist(filePath)) {
		std::cerr << "File does not exist: " << filePath << std::endl;
		return;
	}

	// 2. check if it is a folder or a file
	// if it is a folder, create a tree and store it.
	// if it is a file, create a blob and store it.
	Index& index = Index::instance();
	IndexEntry entry;

	if (FilesHelper::isDirectory(filePath)) {
		// create a tree and store it.
		Tree tree;
		tree.mapObjects(filePath);
		tree.storeObject();
		std::cout << "Tree stored: " << tree.hash() << std::endl;

		// add the tree to the index
		entry.mode = Constants::instance().getFolderMode();
		entry.hash = tree.hash();
		entry.path = filePath;
	}
	else if (FilesHelper::isRegularFile(filePath)) {
		// create a blob and store it.
		Blob blob(FilesHelper::getFileContent(filePath));
		blob.storeObject();
		std::cout << "Blob stored: " << blob.hash() << std::endl;

		// add the blob to the index
		entry.mode = Constants::instance().getFileMode();
		entry.hash = blob.hash();
		entry.path = filePath;
	}
	else {
		std::cerr << "Unknown file type: " << filePath << std::endl;
		return;
	}
	// 3. add the entry to the index
	std::cout << Constants::instance().getFileMode() << std::endl;
	std::cout << Constants::instance().getFolderMode() << std::endl;
	std::cout << entry.mode << " " << entry.hash << " " << entry.path << std::endl;
	index.add(entry);
	index.save();
	
}
