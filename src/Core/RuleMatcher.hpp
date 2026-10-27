#pragma once

#include <cstddef>
#include <flat_map>
#include <flat_set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "DirectoryEntry.hpp"
#include "ProjectRule.hpp"
#include "StringUtils.hpp"

namespace Reaper::Core
{
class RuleMatcher
{
public:
    void AddExcludedDirectory(std::wstring directoryName);
    void AddRule(ProjectRule rule);

    [[nodiscard]] const ProjectRule* Identify(std::span<const DirectoryEntry> entries) const;

    [[nodiscard]] constexpr bool IsExcluded(std::wstring_view directoryName) const noexcept
    {
        return m_excludedDirectories.contains(directoryName);
    }

private:
    std::vector<ProjectRule> m_rules;
    std::flat_set<std::wstring, CaseInsensitiveCompare> m_excludedDirectories;

    std::flat_map<std::wstring, std::vector<size_t>, CaseInsensitiveCompare> m_directoryIndex;
    std::flat_map<std::wstring, std::vector<size_t>, CaseInsensitiveCompare> m_fileIndex;
    std::flat_map<std::wstring, std::vector<size_t>, CaseInsensitiveCompare> m_extensionIndex;
};
} // namespace Reaper::Core
