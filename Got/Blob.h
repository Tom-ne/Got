#pragma once
#include "Object.h"
class Blob : public Object
{
public:
	Blob(const std::string& content)
		: Object(content, ObjectTypes::ObjectType::BLOB)
	{
	}

	void storeObject() const override;
	
	std::string serialize() const override;
	std::string hash() const override;


};

