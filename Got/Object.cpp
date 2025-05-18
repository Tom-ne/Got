#include "Object.h"

std::string Object::hash() const
{
	return Sha1::hash(serialize());
}
