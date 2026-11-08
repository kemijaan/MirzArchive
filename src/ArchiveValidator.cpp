#include "ArchiveValidator.h"
#include "ArchiveFormat.h"
#include <fstream>

bool ArchiveValidator::isValid(const std::filesystem::path& filePath)
{
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
    return false;
    }

    std::array<std::uint8_t, 4> fileSignature;
    file.read(reinterpret_cast<char*>(fileSignature.data()), fileSignature.size());

    return fileSignature == ArchiveFormat::signature;
}

