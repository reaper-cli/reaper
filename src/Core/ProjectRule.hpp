#pragma once

#include <flat_set>
#include <string>
#include <string_view>
#include <vector>

#include "StringUtils.hpp"

namespace Reaper::Core
{
struct ProjectRule
{
    std::string Ecosystem;
    std::vector<std::wstring> RequiredDirectoryNames;
    std::vector<std::wstring> RequiredFileNames;
    std::vector<std::wstring> AnyFileExtensions;
    std::flat_set<std::wstring, CaseInsensitiveCompare> ArtifactDirectories;

    [[nodiscard]] constexpr bool IsArtifact(std::wstring_view directoryName) const noexcept
    {
        return ArtifactDirectories.contains(directoryName);
    }
};
} // namespace Reaper::Core
