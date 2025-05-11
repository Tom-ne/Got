#include "Index.h"
#include "FilesHelper.h"
#include <fstream>
#include <sstream>
#include <iostream>

Index& Index::instance()
{
    static Index instance;
    return instance;
}

Index::Index() {
    try {
        this->load();
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading index: " << e.what() << std::endl;
    }
}

void Index::add(const IndexEntry& entry)
{
    // print the entry to the console
    std::cout << "Adding entry to index: " << entry.mode << " " << entry.hash << " " << entry.path << std::endl;
    entries.push_back(entry);
}

void Index::save() const
{
    if (this->entries.empty())
	{
        std::cerr << "Nothing to save in the index. The index is empty.\n" << std::endl;
		return; // nothing to save
	}

    // write to the index file.
    // the writing format should be: <mode> <hash> <path>
    IndexEntry entry = entries.back();
    FilesHelper::writeToFile(this->indexFilePath, entry.mode + " " + entry.hash + " " + entry.path);
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

const std::vector<IndexEntry>& Index::getEntries() const
{
    return this->entries;
}

void Index::clear()
{
    entries.clear();
    // clear the index file
    std::ofstream ofs(this->indexFilePath, std::ofstream::out | std::ofstream::trunc);
    if (!ofs.is_open())
	{
		throw std::runtime_error("Could not open index file for clearing");
	}
    ofs.close();
	std::cout << "Index cleared." << std::endl;
    
}

