#include "Tree.h"
#include "Constants.h"
#include "FilesHelper.h"
#include "Sha1.h"
#include <stdexcept>
#include <iostream>

Tree::Tree(const std::string& treePath)
    : Object("", ObjectTypes::ObjectType::TREE, treePath), treePath(treePath)
{
}

Tree::~Tree()
{
    for (const auto& object : objects)
    {
        delete object.second;
    }
    objects.clear();
}

void Tree::storeObject() const
{
    for (const auto& object : objects)
    {
        object.second->storeObject();
    }

    std::string hash = this->hash();
    std::string firstChars = hash.substr(0, Constants::instance().getHashCount());
    std::string restOfHash = hash.substr(Constants::instance().getHashCount());

    std::string objectsPath = Constants::instance().getObjectsPath();
    std::string objectPath = objectsPath + "/" + firstChars;
    FilesHelper::createFolder(objectPath);

    std::string filePath = objectPath + "/" + restOfHash;
    FilesHelper::createFile(filePath);
    FilesHelper::writeToFile(filePath, this->serialize());
}

std::string Tree::serialize() const
{
    std::vector<std::string> objectsContents;

    for (const auto& object : objects)
    {
        std::string mode;
        if (object.second->getType() == ObjectTypes::ObjectType::BLOB)
        {
            mode = Constants::instance().getFileMode();
        }
        else if (object.second->getType() == ObjectTypes::ObjectType::TREE)
        {
            mode = Constants::instance().getFolderMode();
        }
        else
        {
            continue;
        }

        std::string name = object.first;
        std::string::size_type pos = name.find_last_of("/\\");
        if (pos != std::string::npos)
        {
            name = name.substr(pos + 1);
        }

        std::string content = mode + " " + name + "\0" + object.second->hash();
        objectsContents.push_back(content);
    }

    std::string serializedContent;
    for (const auto& content : objectsContents)
    {
        serializedContent += content + "\n";
    }
    serializedContent += "\0";

    std::string header = "tree " + this->treePath + "\0" + std::to_string(serializedContent.size()) + "\0";
    return header + serializedContent;
}

std::string Tree::hash() const
{
    std::string serialized = this->serialize();
    Sha1 sha1;
    sha1.update(serialized);
    return sha1.final();
}

void Tree::mapObjects(const std::string& treePath)
{
    if (treePath.empty())
    {
        throw std::runtime_error("Path is empty");
    }

    std::string currentDir = treePath;
    if (currentDir == ".")
    {
        currentDir = FilesHelper::getCurrentDirectory();
    }

    if (!FilesHelper::isDirectory(currentDir))
    {
        throw std::runtime_error("Path is not a directory: " + currentDir);
    }

    this->treePath = currentDir;
    this->path = currentDir; // update base Object path too

    std::vector<std::string> contents = FilesHelper::getFilesInDirectory(currentDir);

    for (auto& content : contents)
    {
        if (FilesHelper::isDirectory(content))
        {
            if (FilesHelper::isInGot(std::ref(content)))
            {
                std::cerr << "Cannot add a folder inside the repository." << std::endl;
                continue;
            }
            Tree* tree = new Tree(content);
            std::cout << "Mapping objects in directory: " << content << std::endl;
            tree->mapObjects(content);
            objects[content] = tree;
        }
        else
        {
            Blob* blob = new Blob(content);
            objects[content] = blob;
        }
    }
}

void Tree::addObject(const std::string& name, Object* object)
{
    if (!object)
    {
        throw std::runtime_error("Object is null");
    }
    objects[name] = object;
}

void Tree::setPath(const std::string& treePath)
{
    this->treePath = treePath;
    this->path = treePath; // keep base class in sync
}

std::map<std::string, Object*>& Tree::getObjects()
{
    return objects;
}
