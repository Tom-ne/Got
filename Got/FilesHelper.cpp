#include "FilesHelper.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "FailedToCreateFolderException.h"
#include "FailedToCreateFileException.h"
#include <vector>

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

bool FilesHelper::isDirectory(const std::string& path)
{
	return fs::exists(path) && fs::is_directory(path);
}

bool FilesHelper::isRegularFile(const std::string& path)
{
	// check if the path is a regular file
	return fs::exists(path) && fs::is_regular_file(path);
}

std::string FilesHelper::getFileMode(const std::string& filePath)
{
	if (!fs::exists(filePath)) {
		throw std::runtime_error("File does not exist: " + filePath);
	}

	if (isDirectory(filePath)) {
		return "40000";
	} else if (isRegularFile(filePath)) {
		return "100644";
	} else {
		throw std::runtime_error("Unknown file type: " + filePath);
	}
}

std::vector<std::string> FilesHelper::getFilesInDirectory(const std::string& directoryPath)
{
	std::vector<std::string> contents;
	if (!FilesHelper::isDirectory(directoryPath)) {
		return contents; // Return empty vector if not a directory
	}

	// should return the files and directories inside the directoryPath
	for (const auto& entry : fs::directory_iterator(directoryPath)) {
		if (entry.is_regular_file()) {
			contents.push_back(entry.path().string());
		}
		else if (entry.is_directory()) {
			contents.push_back(entry.path().string());
		}
	}

	return contents;
}

std::string FilesHelper::getFileContent(const std::string& filePath)
{
	// return the contents of a file
	std::ifstream file(filePath);
	if (!file) {
		throw std::runtime_error("Failed to open file: " + filePath);
	}
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return content;
}
