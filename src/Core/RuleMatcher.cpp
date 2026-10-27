#include "RuleMatcher.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "DirectoryEntry.hpp"
#include "ProjectRule.hpp"
#include "StringUtils.hpp"

namespace Reaper::Core
{
namespace
{
struct RuleProgress
{
    uint32_t DirectoriesFound{0};
    uint32_t FilesFound{0};
    bool ExtensionMatched{false};
};

void DeduplicateCaseInsensitive(std::vector<std::wstring>& items)
{
    std::erase_if(items, [](std::wstring_view item) noexcept { return item.empty(); });
    std::ranges::sort(items, CaseInsensitiveCompare{});

    const auto [first, last] = std::ranges::unique(items, CaseInsensitiveCompare::Equals);
    items.erase(first, last);
}
} // namespace

void RuleMatcher::AddExcludedDirectory(std::wstring directoryName)
{
    if (!directoryName.empty())
    {
        m_excludedDirectories.insert(std::move(directoryName));
    }
}

void RuleMatcher::AddRule(ProjectRule rule)
{
    for (auto& extension : rule.AnyFileExtensions)
    {
        if (!extension.empty() && !extension.starts_with(L'.'))
        {
            extension.insert(extension.begin(), L'.');
        }
    }

    std::erase_if(rule.AnyFileExtensions, [](std::wstring_view extension) noexcept { return extension == L"."; });

    DeduplicateCaseInsensitive(rule.RequiredDirectoryNames);
    DeduplicateCaseInsensitive(rule.RequiredFileNames);
    DeduplicateCaseInsensitive(rule.AnyFileExtensions);
    rule.ArtifactDirectories.erase(L"");

    if ((rule.RequiredDirectoryNames.empty() && rule.RequiredFileNames.empty() && rule.AnyFileExtensions.empty()) ||
        rule.ArtifactDirectories.empty())
    {
        return;
    }

    const size_t ruleIndex = m_rules.size();

    for (const auto& directoryName : rule.RequiredDirectoryNames)
    {
        m_directoryIndex[directoryName].push_back(ruleIndex);
    }
    for (const auto& fileName : rule.RequiredFileNames)
    {
        m_fileIndex[fileName].push_back(ruleIndex);
    }
    for (const auto& extension : rule.AnyFileExtensions)
    {
        m_extensionIndex[extension].push_back(ruleIndex);
    }

    m_rules.push_back(std::move(rule));
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
const ProjectRule* RuleMatcher::Identify(std::span<const DirectoryEntry> entries) const
{
    if (m_rules.empty() || entries.empty())
    {
        return nullptr;
    }

    thread_local std::vector<RuleProgress> ruleProgress;
    ruleProgress.assign(m_rules.size(), RuleProgress{});

    auto processDirectory = [this](std::wstring_view directoryName)
    {
        if (const auto it = m_directoryIndex.find(directoryName); it != m_directoryIndex.end())
        {
            for (const size_t ruleIndex : it->second)
            {
                ruleProgress[ruleIndex].DirectoriesFound++;
            }
        }
    };

    auto processFile = [this](std::wstring_view fileName)
    {
        if (const auto it = m_fileIndex.find(fileName); it != m_fileIndex.end())
        {
            for (const size_t ruleIndex : it->second)
            {
                ruleProgress[ruleIndex].FilesFound++;
            }
        }

        const size_t dotPosition = fileName.rfind(L'.');

        if (dotPosition == std::wstring_view::npos || dotPosition == 0)
        {
            return;
        }

        const std::wstring_view extension = fileName.substr(dotPosition);
        if (const auto it = m_extensionIndex.find(extension); it != m_extensionIndex.end())
        {
            for (const size_t ruleIndex : it->second)
            {
                ruleProgress[ruleIndex].ExtensionMatched = true;
            }
        }
    };

    for (const auto& entry : entries)
    {
        if (entry.IsDirectory)
        {
            processDirectory(entry.Name);
        }
        else
        {
            processFile(entry.Name);
        }
    }

    for (const auto& [rule, progress] : std::views::zip(m_rules, ruleProgress))
    {
        if (progress.DirectoriesFound == rule.RequiredDirectoryNames.size() &&
            progress.FilesFound == rule.RequiredFileNames.size() &&
            (rule.AnyFileExtensions.empty() || progress.ExtensionMatched))
        {
            return &rule;
        }
    }

    return nullptr;
}
} // namespace Reaper::Core
