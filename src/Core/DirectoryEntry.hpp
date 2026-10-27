#pragma once

#include <cstdint>
#include <string>

namespace Reaper::Core
{
struct DirectoryEntry
{
    std::wstring Name;
    uint64_t Size{0};
    bool IsDirectory{false};
    bool IsReparsePoint{false};
    bool IsSystem{false};
    bool IsHidden{false};
};
} // namespace Reaper::Core
