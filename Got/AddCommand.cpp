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
	//if (!FilesHelper::doesFileExist(filePath)) {
	//	std::cerr << "File does not exist: " << filePath << std::endl;
	//	return;
	//}

	// 2. check if it is a folder or a file
	// if it is a folder, create a tree and store it.
	// if it is a file, create a blob and store it.

	if (FilesHelper::isDirectory(filePath)) {
		// create a tree and store it.
		// if the file path contains constants::getFolderName, return
		if (filePath.find(Constants::instance().getFolderPath()) != std::string::npos) {
			//std::cerr << "Cannot add a folder inside the repository." << std::endl;
			return;
		}
		
		Tree tree;
		tree.mapObjects(filePath);
		tree.storeObject();
		std::cout << "Tree stored: " << tree.hash() << std::endl;

		// run addToIndex on everyfile in the tree
		for (const auto& object : tree.getObjects()) {
			addToIndex(object.first, object.second->getType(), object.second);
		}

		addToIndex(filePath, ObjectTypes::ObjectType::TREE, &tree); // add the tree to the index
	}
	else if (FilesHelper::isRegularFile(filePath)) {
		// create a blob and store it.
		Blob blob(FilesHelper::getFileContent(filePath));
		blob.storeObject();
		std::cout << "Blob stored: " << blob.hash() << std::endl;

		// add the blob to the index
		addToIndex(filePath, ObjectTypes::ObjectType::BLOB, &blob); // add the blob to the index
	}
	else {
		std::cerr << "Unknown file type: " << filePath << std::endl;
		return;
	}
	// 3. save the index
	Index& index = Index::instance();
	index.save();
}

void AddCommand::addToIndex(std::string path, ObjectTypes::ObjectType type, Object* object)
{
	Index& index = Index::instance();
	IndexEntry entry;
	if (type == ObjectTypes::ObjectType::BLOB) {
		entry.mode = Constants::instance().getFileMode();
	}
	else if (type == ObjectTypes::ObjectType::TREE) {
		entry.mode = Constants::instance().getFolderMode();
	}
	else {
		std::cerr << "Unknown object type: " << ObjectTypes::objectTypeToString(type) << std::endl;
		return;
	}
	entry.hash = object->hash();
	entry.path = entry.path = FilesHelper::getRelativePath(path);
	index.add(entry);
}
