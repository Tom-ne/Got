#pragma once
#include "Object.h"
#include "FilesHelper.h"

class Blob : public Object
{
public:
	Blob(const std::string& path)
		: Object(FilesHelper::getFileContent(path), ObjectTypes::ObjectType::BLOB, path) {}


	void storeObject() const override;
	Object* clone() const override
	{
		return new Blob(*this);
	}
	
	std::string serialize() const override;
	std::string hash() const override;
};

