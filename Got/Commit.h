#pragma once
#include "Object.h"

class Commit : public Object 
{
public:
	Commit(const std::string& treeHash, const std::vector<std::string>& parentHashes, const std::string& author, const std::string& message) :
		Object("", ObjectTypes::ObjectType::COMMIT),
		treeHash(treeHash),
		parentHashes(parentHashes),
		author(author),
		message(message)
	{
	}

	std::string serialize() const override;
	std::string hash() const override;
	void storeObject() const override;

	Object* clone() const override
	{
		return new Commit(*this);
	}

	std::string getTreeHash() const;
	std::vector<std::string> getParentHashes() const;
	std::string getAuthor() const;
	std::string getMessage() const;
private:
	std::string treeHash;
	std::vector<std::string> parentHashes;
	std::string author;
	std::string message;
};
