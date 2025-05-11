#pragma once

#include "IndexEntry.h"
#include <vector>
#include "Constants.h"
#include "Object.h"
#include "Tree.h"
#include "Blob.h"

class Index
{
public:
    static Index& instance();

    void add(const Object* entry);

    void save();

    void load();

    std::vector<Object*> toObjects() const;

    Tree* toObjectTree(const std::string& rootPath = ".") const;

    const std::vector<IndexEntry>& getEntries() const;

    void clear();

    void clearFile();

private:
    Index() = default;
    ~Index() = default;
    Index(const Index&) = delete;
    Index& operator=(const Index&) = delete;
    std::vector<IndexEntry> entries;
    const std::string indexFilePath = Constants::instance().getIndexFilePath();

    void addEntriesToTree(const std::vector<IndexEntry>& entries, Tree* parentTree, const std::string& parentPath) const;
};
