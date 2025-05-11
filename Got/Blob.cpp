#include "Blob.h"

void Blob::storeObject() const
{
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

std::string Blob::serialize() const
{
	// sha1 by the format: "type size\0content"
	std::string serialized = ObjectTypes::objectTypeToString(this->getType()) + " " + std::to_string(this->getSize()) + "\0" + this->getContent();
	return serialized;
}

std::string Blob::hash() const
{
	std::string serialized = this->serialize();
	Sha1 sha1;
	sha1.update(serialized);
	// get the hash
	std::string hash = sha1.final();
	// prepend the hash to the serialized
	return hash;
}
