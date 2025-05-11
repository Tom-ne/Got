#pragma once
// every object has a type, size and content.
// every object should have a serialize method.
#include <string>
#include "ObjectTypes.h"
#include "Constants.h"
#include "FilesHelper.h"
#include "Sha1.h"

class Object {
public:
    Object(std::string content, ObjectTypes::ObjectType type, std::string path = "")
        : content(std::move(content)), type(type), path(std::move(path)) {}

    virtual ~Object() = default;

    virtual std::string serialize() const = 0;
    virtual std::string hash() const = 0;
    virtual void storeObject() const = 0;
    virtual Object* clone() const = 0;

    const std::string& getContent() const { return content; }
    ObjectTypes::ObjectType getType() const { return type; }
    const std::string& getPath() const { return path; }

protected:
    std::string content;
    ObjectTypes::ObjectType type;
    std::string path; 
};
