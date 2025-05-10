#include "Object.h"
#include "Sha1.h"
#include <iostream>
#include "Constants.h"
#include "FilesHelper.h"

std::string Object::getContent() const
{
	return this->content;
}

ObjectTypes::ObjectType Object::getType() const
{
	return this->type;
}

size_t Object::getSize() const
{
	return this->size;
}
