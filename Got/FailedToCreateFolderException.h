#pragma once
#include <stdexcept>
#include <string>
#include <iostream>
class FailedToCreateFolderException : public std::runtime_error
{
public:
	FailedToCreateFolderException(const std::string& folderPath)
		: std::runtime_error("Failed to create folder: " + folderPath) {
		std::cerr << "Error: " << what() << std::endl;
	}
};