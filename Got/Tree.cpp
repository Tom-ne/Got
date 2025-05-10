#include "Tree.h"

void Tree::storeObject() const
{
}

std::string Tree::serialize() const
{
	// returns the contents that need to be stored in the object file
	std::vector<std::string> blobsContent;
	for (const auto& blob : blobs)
	{
		blobsContent.push_back(blob.serialize());
	}
}

std::string Tree::hash() const
{
	return std::string();
}
