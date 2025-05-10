#pragma once
#include "Object.h"
#include "Blob.h"
#include <vector>

class Tree : public Object
{
public:
	Tree(const std::string& content)
		: Object(content, ObjectTypes::ObjectType::TREE)
	{
	}

	void storeObject() const override;

	std::string serialize() const override;
	std::string hash() const override;
private:
	std::vector<Blob> blobs;
};
