#pragma once
// every object has a type, size and content.
// every object should have a serialize method.
#include <string>
#include "ObjectTypes.h"
#include "Constants.h"
#include "FilesHelper.h"
#include "Sha1.h"

class Object
{
public:
	Object(const std::string& content, ObjectTypes::ObjectType type)
		: content(content), type(type)
	{
		size = content.size();
	}

	virtual ~Object() = default;

	virtual Object* clone() const = 0;

	virtual void storeObject() const = 0;

	virtual std::string serialize() const = 0;
	virtual std::string hash() const = 0;
	
	std::string getContent() const;
	ObjectTypes::ObjectType getType() const;
	size_t getSize() const;

protected:
	void updateContent(std::string& content) {
		this->content = content;
		this->size = content.size();
	}

private:
	std::string content;
	ObjectTypes::ObjectType type;
	size_t size;
};
