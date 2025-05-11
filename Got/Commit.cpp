#include "Commit.h"

std::string Commit::serialize() const
{
    // should return in the form of:
    // tree <treeHash>
    // parent <parentHash1>
    // author <author>
    // message <message>
    std::string serialized = "tree " + this->treeHash + "\n";
    serialized += "parent ";
    for (const auto& parentHash : this->parentHashes)
	{
		serialized += parentHash + "\n";
	}
    serialized += "author " + this->author + "\n";
	serialized += "message " + this->message + "\n";
	return serialized;
}

std::string Commit::hash() const
{
    // should hash the following format:
    // commit <size>\0<serialized>
    std::string serialized = this->serialize();
    std::string header = "commit " + std::to_string(serialized.size()) + "\0";
    std::string fullSerialized = header + serialized;
    Sha1 sha1;
    sha1.update(fullSerialized);
    // get the hash
    std::string hash = sha1.final();
    return hash;
}

void Commit::storeObject() const
{
    // step 1: serialize the object
    std::string serialized = this->serialize();

    // step 2: hash the object
    std::string hash = this->hash();

    // step 3: store the object in the object dir
    std::string firstChars = hash.substr(0, Constants::instance().getHashCount());
    std::string restOfHash = hash.substr(Constants::instance().getHashCount());
    std::string objectsPath = Constants::instance().getObjectsPath();
    std::string objectPath = objectsPath + "/" + firstChars;
    FilesHelper::createFolder(objectPath);
    std::string filePath = objectPath + "/" + restOfHash;
    FilesHelper::createFile(filePath);
    // write the content to the file.
    FilesHelper::writeToFile(filePath, serialized);

    // step 4: read the path from HEAD
    std::string content = FilesHelper::getFileContent(Constants::instance().getHeadPath());
    // this is how the content looks like:
    // ref: refs/heads/master
    // we need to get the path from it.
    std::string path = content.substr(4); // remove the "ref: " part

    // step 5: go the the path specified in HEAD and write the hash to the file
    std::string refsPath = Constants::instance().getRefsPath();
    // create the file with the name at the end of path (eg. master here)
    std::string fileName = path.substr(path.find_last_of("/") + 1);
    std::string filePath2 = refsPath + "/" + fileName;
    FilesHelper::createFile(filePath2);
    // write the hash to the file
    FilesHelper::writeToFile(filePath2, hash);
}

std::string Commit::getTreeHash() const
{
    return this->treeHash;
}

std::vector<std::string> Commit::getParentHashes() const
{
    return this->parentHashes;
}

std::string Commit::getAuthor() const
{
    return this->author;
}

std::string Commit::getMessage() const
{
    return this->message;
}
