#include "Blob.h"

void Blob::storeObject() const
{
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

std::string Blob::serialize() const
{
	// sha1 by the format: "type size\0content"
	std::string serialized = ObjectTypes::objectTypeToString(this->getType()) + " " + std::to_string(this->getContent().size()) + "\0" + this->getContent();
	return serialized;
}
