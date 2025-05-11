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
	Object* clone() const override
	{
		return new Blob(*this);
	}
	
	std::string serialize() const override;
	std::string hash() const override;


};

