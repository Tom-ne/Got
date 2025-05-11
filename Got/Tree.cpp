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
		object.second->storeObject();
	}

	std::string hash = this->hash();
	// get the first two characters of the hash
	std::string firstChars = hash.substr(0, Constants::instance().getHashCount());
	// get the rest of the hash
	std::string restOfHash = hash.substr(Constants::instance().getHashCount());

	// create the directory inside objects.
	std::string objectsPath = Constants::instance().getObjectsPath();
	std::string objectPath = objectsPath + "/" + firstChars;
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
	// Ensure treePath is not empty
	if (treePath.empty()) {
		throw std::runtime_error("Path is empty");
	}

	// Handle the case where the treePath is "." (current directory)
	std::string currentDir = treePath;
	if (currentDir == ".") {
		currentDir = FilesHelper::getCurrentDirectory(); // Get the current working directory if it's "."
	}

	if (!FilesHelper::isDirectory(currentDir)) {
		std::cout << "Path is not a directory: " << currentDir << std::endl;
		throw std::runtime_error("Path is not a directory: " + currentDir);
	}

	this->treePath = currentDir;
	std::vector<std::string> contents = FilesHelper::getFilesInDirectory(currentDir);

	for (const auto& content : contents) {
		if (FilesHelper::isDirectory(content)) {
			// if the content contains the constants::getFolderPath, continue
			if (content.find(Constants::instance().getFolderPath()) != std::string::npos) {
				std::cerr << "Cannot add a folder inside the repository." << std::endl;
				continue;
			}
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

void Tree::addObject(const std::string& name, Object* object)
{
	// a method responsible for adding objects to the tree.
	// it should take a name and an object and add it to the tree.
	if (object == nullptr) {
		throw std::runtime_error("Object is null");
	}
	objects[name] = object;
}

void Tree::setPath(std::string treePath)
{
	this->treePath = treePath;
}

std::map<std::string, Object*>& Tree::getObjects()
{
	return objects;
}
