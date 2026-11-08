#ifndef ARCHIVEFORMAT_H
#define ARCHIVEFORMAT_H

#include <array>
#include <cstdint>

class ArchiveFormat
{
public:
    static constexpr std::array<std::uint8_t, 4> signature = {'M', 'I', 'R', 'Z'};
};

#endif