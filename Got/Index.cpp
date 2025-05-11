#include "Index.h"
#include "FilesHelper.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "Constants.h"
#include "Object.h"
#include "Blob.h"
#include "Tree.h"

Index& Index::instance()
{
    static Index instance;
    return instance;
}

void Index::add(const Object* entry)
{
    IndexEntry indexEntry;
    indexEntry.mode = (entry->getType() == ObjectTypes::ObjectType::BLOB)
        ? Constants::instance().getFileMode()
        : Constants::instance().getFolderMode();
    indexEntry.hash = entry->hash();
    indexEntry.path = entry->getPath();

    entries.push_back(indexEntry);
}

void Index::save()
{
    if (this->entries.empty())
    {
        std::cerr << "Nothing to save in the index. The index is empty.\n" << std::endl;
        return; // nothing to save
    }

    // clear the index file before writing
    this->clearFile();

    // write to the index file.
    // the writing format should be: <mode> <hash> <path>
    std::string content;
    for (const auto& entry : entries)
    {
        content += entry.mode + " " + entry.hash + " " + entry.path + "\n";
    }
    FilesHelper::writeToFile(this->indexFilePath, content);
}

void Index::load()
{
    entries.clear(); // clear existing entries to avoid duplication

    std::ifstream file(this->indexFilePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open index file");
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        IndexEntry entry;
        if (!(iss >> entry.mode >> entry.hash >> entry.path)) {
            continue; // skip invalid lines
        }
        entries.push_back(entry);
    }

    file.close();
}

std::vector<Object*> Index::toObjects() const
{
    std::vector<Object*> objects;
    for (const auto& entry : entries)
    {
        Object* object = nullptr;

        // Check if it's a file or directory
        if (entry.mode == Constants::instance().getFileMode()) {
            // Create a Blob object for a file
            Blob* blob = new Blob(FilesHelper::getFileContent(entry.path));
            objects.push_back(blob);
        }
        else if (entry.mode == Constants::instance().getFolderMode()) {
            // Create a Tree object for a directory
            Tree* tree = new Tree();
            addEntriesToTree(entries, tree, entry.path);  // Recursively add directory entries
            objects.push_back(tree);
        }
        else {
            std::cerr << "Unknown mode for entry: " << entry.path << std::endl;
        }
    }
    return objects;
}

Tree* Index::toObjectTree(const std::string& rootPath) const
{
    Tree* rootTree = new Tree();
    rootTree->setPath(rootPath);

    // Recursively add entries from the index to the root tree
    addEntriesToTree(entries, rootTree, rootPath);

    return rootTree;
}

void Index::addEntriesToTree(const std::vector<IndexEntry>& entries, Tree* parentTree, const std::string& parentPath) const
{
    for (const auto& entry : entries)
    {
        std::string fullPath = parentPath + "/" + entry.path;

        if (entry.mode == Constants::instance().getFileMode()) {
            // If it's a file, create a Blob and add it to the tree
            Blob* blob = new Blob(FilesHelper::getFileContent(entry.path));
            parentTree->addObject(entry.path, blob);
            std::cout << "Blob added: " << fullPath << std::endl;
        }
        else if (entry.mode == Constants::instance().getFolderMode()) {
            // If it's a folder, create a new Tree object and add its entries recursively
            Tree* subTree = new Tree();
            subTree->setPath(fullPath);  // Set the correct path for the subtree

            // Collect all entries under the directory
            std::vector<IndexEntry> subEntries;
            for (const auto& subEntry : entries) {
                if (subEntry.path.find(fullPath) == 0 && subEntry.path != fullPath) {
                    subEntries.push_back(subEntry);  // Add entries that are under the current directory
                }
            }

            // Recursively add subdirectory entries to the tree
            addEntriesToTree(subEntries, subTree, fullPath);
            parentTree->addObject(entry.path, subTree);
            std::cout << "Tree added: " << fullPath << std::endl;
        }
        else {
            std::cerr << "Unknown entry mode for: " << fullPath << std::endl;
        }
    }
}

void Index::clear()
{
    entries.clear();
    // clear the index file
    this->clearFile();
}

void Index::clearFile()
{
    std::ofstream ofs(this->indexFilePath, std::ofstream::out | std::ofstream::trunc);
    if (!ofs.is_open())
    {
        throw std::runtime_error("Could not open index file for clearing");
    }
    ofs.close();
    std::cout << "Index cleared." << std::endl;
}

const std::vector<IndexEntry>& Index::getEntries() const
{
    return this->entries;
}
