#pragma once
#include <string>

struct IndexEntry {
	std::string mode; // File mode (e.g., "100644" for regular files and "40000" for folders)
	std::string hash; // Hash of the object (SHA-1)
	std::string path; // Relative path to the file or directory
};