#pragma once
#include "Object.h"
#include "Blob.h"
#include <vector>
#include <map>

class Tree : public Object
{
public:
	Tree()
		: Object("", ObjectTypes::ObjectType::TREE), treePath("")
	{
	}

	~Tree() override;

	void storeObject() const override;

	std::string serialize() const override;
	std::string hash() const override;
	void mapObjects(const std::string& treePath);
private:
	std::map<std::string, Object*> objects;
	std::string treePath;
};
