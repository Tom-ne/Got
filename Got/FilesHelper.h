#pragma once
#include <string>

class FilesHelper
{
public:
	static bool doesFileExist(const std::string& filePath);
	static void createFolder(const std::string& folderPath);
	static void createFile(const std::string& filePath);

	static void writeToFile(const std::string& filePath, const std::string& content);
};