#include "Commit.h"
#include <string>

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
    // if there are no parent hashes, we should add a newline
    if (this->parentHashes.empty())
        serialized += "\n";
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
    // Step 1: Read the HEAD reference
    std::string headContent = FilesHelper::getFileContent(Constants::instance().getHeadPath());
    std::string refPath = headContent.substr(5); // Skip "ref: "

    // Step 2: Get full path to the branch file (e.g., .got/refs/heads/master)
    std::string branchName = refPath.substr(refPath.find_last_of("/") + 1);
    std::string branchFilePath = Constants::instance().getHeadsPath() + "/" + branchName;

    // Step 3: Check if this branch file exists — if yes, add it as parent
    std::string existingCommitHash = FilesHelper::getFileContent(branchFilePath);
    if (!existingCommitHash.empty())
    {
        // Remove trailing newline if present
        if (!existingCommitHash.empty() && existingCommitHash.back() == '\n')
            existingCommitHash.pop_back();

        this->parentHashes.push_back(existingCommitHash);
    }

    // Step 4: Serialize and hash the commit
    std::string serialized = serialize();
    std::string hash = this->hash();

    // Step 5: Write the object to .got/objects/xx/yyyy...
    std::string firstChars = hash.substr(0, Constants::instance().getHashCount());
    std::string restOfHash = hash.substr(Constants::instance().getHashCount());
    std::string objectDir = Constants::instance().getObjectsPath() + "/" + firstChars;
    std::string objectFilePath = objectDir + "/" + restOfHash;

    FilesHelper::createFolder(objectDir);
    FilesHelper::createFile(objectFilePath);
    FilesHelper::writeToFile(objectFilePath, serialized);

    // Step 6: Update branch reference to point to this commit
    FilesHelper::createFile(branchFilePath);
    FilesHelper::writeToFile(branchFilePath, hash);
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
