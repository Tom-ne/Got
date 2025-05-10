#pragma once
#include <stdexcept>
#include <string>
#include <iostream>
class FailedToCreateFileException : public std::runtime_error
{
	public:
	FailedToCreateFileException(const std::string& filePath)
		: std::runtime_error("Failed to create file: " + filePath) {
		std::cerr << "Error: " << what() << std::endl;
	}
};