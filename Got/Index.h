#pragma once
#include "IndexEntry.h"
#include <vector>
#include "Constants.h"

class Index
{
public:
	static Index& instance();

	void add(const IndexEntry& entry);
	void save() const;
	void load();
	const std::vector<IndexEntry>& getEntries() const;
	void clear();

private:
	Index();
	~Index() = default;
	Index(const Index&) = delete;
	Index& operator=(const Index&) = delete;

	std::vector<IndexEntry> entries;
	const std::string indexFilePath = Constants::instance().getIndexFilePath();
};

