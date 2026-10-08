#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Reaper::CLI
{
struct ExecutionOptions
{
    bool Yes{false};
    bool DryRun{false};
    bool Headless{false};
    bool Force{false};
};

struct FilterOptions
{
    std::optional<std::string> OlderThan;
    std::optional<std::string> MinSize;
    std::optional<int32_t> MaxDepth;

    std::vector<std::string> Ecosystems;
    std::vector<std::string> ExcludeEcosystems;
    std::vector<std::string> ExcludeDirectories;
};

struct CliOptions
{
    std::filesystem::path TargetPath;
    unsigned int Threads{1};
    std::optional<std::filesystem::path> ConfigFile;

    ExecutionOptions Execution{};
    FilterOptions Filters{};
};

[[nodiscard]] std::expected<CliOptions, int> ParseCommandLine(int argc, wchar_t** argv);
} // namespace Reaper::CLI
