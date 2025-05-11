#include "Tree.h"
#include <stdexcept>
#include <iostream>

Tree::~Tree()
{
	for (const auto& object : objects)
	{
		delete object.second; // delete the object
	}
	objects.clear(); // clear the map
}

void Tree::storeObject() const
{
	// call storeObject on every object in the tree.
	for (const auto& object : objects)
	{
		object.second->storeObject(); // store the object
	}

	std::string hash = this->hash();
	// get the first two characters of the hash
	std::string firstTwoChars = hash.substr(0, 4);
	// get the rest of the hash
	std::string restOfHash = hash.substr(4);

	// create the directory inside objects.
	std::string objectsPath = Constants::instance().getObjectsPath();
	std::string objectPath = objectsPath + "/" + firstTwoChars;
	FilesHelper::createFolder(objectPath);

	// create the file inside the directory.
	std::string filePath = objectPath + "/" + restOfHash;
	FilesHelper::createFile(filePath);
	// write the content to the file.
	FilesHelper::writeToFile(filePath, this->serialize());
}

std::string Tree::serialize() const
{
	// returns the contents that need to be stored in the object file
	std::vector<std::string> objectsContents;
	// for every object, check if it a blob or a tree, if it is a tree, then the mode should be 40000, if it is a blob, then the mode should be 100644.
	// for every object, store in the following format:
	// mode + space + name + \0 + hash
	for (const auto& object : objects)
	{
		std::string mode;
		if (object.second->getType() == ObjectTypes::ObjectType::BLOB)
		{
			mode = Constants::instance().getFileMode();
		}
		else if (object.second->getType() == ObjectTypes::ObjectType::TREE)
		{

			mode = Constants::instance().getFolderMode();
		}
		else
		{
			continue; // skip if not a blob or tree
		}

		// for the name of the object, remove the path to the object.
		std::string name = object.first;
		std::string::size_type pos = name.find_last_of("/\\");
		if (pos != std::string::npos)
		{
			name = name.substr(pos + 1); // get the name of the object
		}

		std::string content = mode + " " + name + "\0" + object.second->hash();
		objectsContents.push_back(content);
	}

	std::string serializedContent = "";
	for (const auto& content : objectsContents)
	{
		serializedContent += content;
		serializedContent += "\n";
	}
	serializedContent += "\0"; // add a null terminator at the end
	// the header should be tree <name>\0<size>\0
	std::string header = "tree " + this->treePath + "\0" + std::to_string(serializedContent.size()) + "\0";
	return header + serializedContent;
}

std::string Tree::hash() const
{
	std::string serialized = this->serialize();
	Sha1 sha1;
	sha1.update(serialized);
	// get the hash
	std::string hash = sha1.final();
	// prepend the hash to the serialized
	return hash;
}

void Tree::mapObjects(const std::string& treePath)
{
	// a method responsible for adding objects to the tree.
	// it should take a path to a directory and add all the files and directories inside it to the tree.
	// it should also add the files and directories inside the directories to the tree.
	//if (!FilesHelper::doesFileExist(treePath)) {
	//	throw std::runtime_error("Path does not exist: " + treePath);
	//}

	if (!FilesHelper::isDirectory(treePath)) {
		throw std::runtime_error("Path is not a directory: " + treePath);
	}

	this->treePath = treePath;
	std::vector<std::string> contents = FilesHelper::getFilesInDirectory(treePath);
	for (const auto& content : contents) {
		if (FilesHelper::isDirectory(content)) {
			Tree* tree = new Tree();
			std::cout << "Mapping objects in directory: " << content << std::endl;
			tree->mapObjects(content);
			objects[content] = tree;
		}
		else {
			Blob* blob = new Blob(content);
			objects[content] = blob;
		}
	}
}
