#pragma once
#include "Object.h"
#include "Blob.h"
#include <vector>
#include <map>

class Tree : public Object
{
public:
    Tree(const std::string& treePath = "");

    ~Tree() override;

    void storeObject() const override;
    Object* clone() const override
    {
        return new Tree(*this);
    }

    std::string serialize() const override;
    std::string hash() const override;
    void mapObjects(const std::string& treePath);

    void addObject(const std::string& name, Object* object);

    void setPath(const std::string& treePath);
    std::map<std::string, Object*>& getObjects();

private:
    std::map<std::string, Object*> objects;
    std::string treePath;
};
