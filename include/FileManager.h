#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <filesystem>
#include<vector>

class FileManager
{
public:
    FileManager();

std::vector<std::filesystem::directory_entry> listDirectory(const std::filesystem::path& path);
};

#endif