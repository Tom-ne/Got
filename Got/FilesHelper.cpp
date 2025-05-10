#include "FilesHelper.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "FailedToCreateFolderException.h"
#include "FailedToCreateFileException.h"

namespace fs = std::filesystem;

bool FilesHelper::doesFileExist(const std::string& filePath)
{
	return fs::exists(filePath) && fs::is_regular_file(filePath);
}

void FilesHelper::createFolder(const std::string& folderPath)
{
	namespace fs = std::filesystem;
	try {
		fs::path path(folderPath);

		if (fs::exists(path)) {
			if (fs::is_directory(path)) {
				std::cerr << "Folder already exists: " << folderPath << std::endl;
				return;
			}
			throw FailedToCreateFolderException(folderPath + " exists but is not a folder.");
		}

		if (!fs::create_directories(path)) {
			throw FailedToCreateFolderException("Unknown failure for: " + folderPath);
		}
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "Filesystem error: " << e.what() << std::endl;
		throw;
	}
}

void FilesHelper::createFile(const std::string& filePath)
{
	fs::path path(filePath);

	if (fs::exists(path)) {
		if (fs::is_regular_file(path)) {
			std::cerr << "File already exists: " << filePath << std::endl;
			return;
		}
		throw FailedToCreateFileException(filePath + " already exists but is not a regular file.");
	}

	// Ensure the parent directory exists
	fs::create_directories(path.parent_path());

	std::ofstream file(path);
	if (!file) {
		throw FailedToCreateFileException(filePath);
	}
}

void FilesHelper::writeToFile(const std::string& filePath, const std::string& content)
{
	fs::path path(filePath);

	// Ensure the parent directory exists
	fs::create_directories(path.parent_path());

	std::ofstream file(path);
	if (!file) {
		throw FailedToCreateFileException(filePath);
	}
	file << content;
	file.close();
}
