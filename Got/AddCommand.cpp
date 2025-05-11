#include "AddCommand.h"
#include "CommandFactory.h"
#include "FilesHelper.h"
#include "Blob.h"
#include "Tree.h"
#include "Index.h"
#include "IndexEntry.h"
#include <filesystem>

namespace fs = std::filesystem;

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
	if (args.empty()) {
		std::cerr << "No arguments provided." << std::endl;
		return;
	}

	std::string filePath = args[0];

	if (FilesHelper::isInGot(filePath)) {
		std::cerr << "Cannot add files from inside .got directory." << std::endl;
		return;
	}

	Index& index = Index::instance();

	if (FilesHelper::isDirectory(filePath)) {
		// Create tree from folder
		Tree tree;
		tree.setPath(FilesHelper::getRelativePath(filePath));
		tree.mapObjects(filePath);  // recursively map contents
		tree.storeObject();

		// Store all child objects from mapObjects
		for (const auto& [path, object] : tree.getObjects()) {
			object->storeObject();
			index.add(object);
		}

		// Add root tree itself to index
		index.add(&tree);
	}
	else if (FilesHelper::isRegularFile(filePath)) {
		Blob blob(FilesHelper::getFileContent(filePath));
		blob.storeObject();
		index.add(&blob);
	}
	else {
		std::cerr << "Unknown file type: " << filePath << std::endl;
		return;
	}

	index.save();
}
