#include "Constants.h"
#include <filesystem>

Constants& Constants::instance() {
    static Constants instance;
    return instance;
}

const std::string& Constants::getFolderPath() const {
    static std::string path = std::filesystem::path(".got").string();
    return path;
}

const std::string& Constants::getObjectsPath() const {
    static std::string path = (std::filesystem::path(getFolderPath()) / "objects").string();
    return path;
}

const std::string& Constants::getRefsPath() const {
    static std::string path = (std::filesystem::path(getFolderPath()) / "refs").string();
    return path;
}

const std::string& Constants::getHeadsPath() const {
    static std::string path = (std::filesystem::path(getRefsPath()) / "heads").string();
    return path;
}

const std::string& Constants::getHeadPath() const {
    static std::string path = (std::filesystem::path(getFolderPath()) / "HEAD").string();
    return path;
}

const std::string& Constants::getInitialRef() const {
    static std::string ref = "ref: refs/heads/master";
    return ref;
}

const std::string& Constants::getIndexFilePath() const
{
    static std::string path = (std::filesystem::path(getFolderPath()) / "index").string();
	return path;
}

const std::string& Constants::getFolderMode() const
{
    static std::string mode = "40000"; // default folder mode
    return mode;
}

const std::string& Constants::getFileMode() const
{
    static std::string mode = "100644"; // default file mode
    return mode;
}
