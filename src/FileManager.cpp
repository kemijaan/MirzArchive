#include "FileManager.h"
#include <vector>

FileManager::FileManager()
{

}

std::vector<std::filesystem::directory_entry> FileManager::listDirectory(const std::filesystem::path& path)
{
    std::vector<std::filesystem::directory_entry> entries;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        entries.push_back(entry);
    }

    return entries;
}