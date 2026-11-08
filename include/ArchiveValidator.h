#ifndef ARCHIVEVALIDATOR_H
#define ARCHIVEVALIDATOR_H

#include <filesystem>

class ArchiveValidator
{
public:
    bool isValid(const std::filesystem::path& filePath);
};

#endif