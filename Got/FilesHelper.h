#pragma once
#include <string>
#include <vector>

class FilesHelper
{
public:
	static bool doesFileExist(const std::string& filePath);
	static void createFolder(const std::string& folderPath);
	static void createFile(const std::string& filePath);

	static void writeToFile(const std::string& filePath, const std::string& content);

	static bool isDirectory(const std::string& path);
	static bool isRegularFile(const std::string& path);

	static std::string getFileMode(const std::string& filePath);

	static std::vector<std::string> getFilesInDirectory(const std::string& directoryPath);
	static std::string getFileContent(const std::string& filePath);

	static std::string getRelativePath(const std::string& path);
	static std::string getCurrentDirectory();
};