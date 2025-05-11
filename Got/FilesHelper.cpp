#include "FilesHelper.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include "FailedToCreateFolderException.h"
#include "FailedToCreateFileException.h"

namespace fs = std::filesystem;

bool FilesHelper::doesFileExist(const std::string& filePath) {
    fs::path path(filePath);
    return fs::exists(path) && fs::is_regular_file(path);
}

void FilesHelper::createFolder(const std::string& folderPath) {
    fs::path path(folderPath);

    try {
        if (fs::exists(path)) {
            if (fs::is_directory(path)) {
                std::cerr << "Folder already exists: " << path << std::endl;
                return;
            }
            throw FailedToCreateFolderException(path.string() + " exists but is not a folder.");
        }

        if (!fs::create_directories(path)) {
            throw FailedToCreateFolderException("Unknown failure for: " + path.string());
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        throw;
    }
}

void FilesHelper::createFile(const std::string& filePath) {
    fs::path path(filePath);

    if (fs::exists(path)) {
        if (fs::is_regular_file(path)) {
            std::cerr << "File already exists: " << path << std::endl;
            return;
        }
        throw FailedToCreateFileException(path.string() + " already exists but is not a regular file.");
    }

    fs::create_directories(path.parent_path());

    std::ofstream file(path);
    if (!file) {
        throw FailedToCreateFileException(path.string());
    }
}

void FilesHelper::writeToFile(const std::string& filePath, const std::string& content) {
    fs::path path(filePath);

    fs::create_directories(path.parent_path());

    std::ofstream file(path);
    if (!file) {
        throw FailedToCreateFileException(path.string());
    }

    file << content;
    file.close();
}

bool FilesHelper::isDirectory(const std::string& path) {
    return fs::is_directory(fs::path(path));
}

bool FilesHelper::isRegularFile(const std::string& path) {
    return fs::is_regular_file(fs::path(path));
}

std::string FilesHelper::getFileMode(const std::string& filePath) {
    fs::path path(filePath);

    if (!fs::exists(path)) {
        throw std::runtime_error("File does not exist: " + path.string());
    }

    if (fs::is_directory(path)) {
        return "40000";
    }
    else if (fs::is_regular_file(path)) {
        return "100644";
    }
    else {
        throw std::runtime_error("Unknown file type: " + path.string());
    }
}

std::vector<std::string> FilesHelper::getFilesInDirectory(const std::string& directoryPath) {
    std::vector<std::string> files;
    fs::path dirPath(directoryPath);

    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        return files;
    }

    for (const auto& entry : fs::directory_iterator(dirPath)) {
        files.push_back(entry.path().string());
    }

    return files;
}

std::string FilesHelper::getFileContent(const std::string& filePath) {
    fs::path path(filePath);
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path.string());
    }

    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::string FilesHelper::getRelativePath(const std::string& path) {
    fs::path absolute = fs::absolute(path);
    fs::path current = fs::current_path();
    return fs::relative(absolute, current).string();
}

std::string FilesHelper::getCurrentDirectory() {
    try {
        return fs::current_path().string();
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error getting current directory: " << e.what() << std::endl;
        return "";
    }
}
