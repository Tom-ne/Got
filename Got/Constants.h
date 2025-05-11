#pragma once

#include <string>

class Constants {
public:
	static Constants& instance();

	const std::string& getFolderPath() const;
	const std::string& getObjectsPath() const;
	const std::string& getRefsPath() const;
	const std::string& getHeadsPath() const;
	const std::string& getHeadPath() const;

	const std::string& getInitialRef() const;

	const std::string& getIndexFilePath() const;

	const std::string& getFolderMode() const;
	const std::string& getFileMode() const;
};