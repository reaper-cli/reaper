#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "ProjectRule.hpp"

namespace Reaper::Config
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
    std::vector<std::string> ExcludedEcosystems;
    std::vector<std::wstring> ExcludedDirectories;
};

struct ScanFilters
{
    std::optional<std::chrono::seconds> OlderThan;
    std::optional<uint64_t> MinSizeBytes;
    std::optional<int32_t> MaxDepth;
};

struct CliOverrides
{
    std::filesystem::path TargetPath;
    std::optional<unsigned int> Threads;
    std::optional<std::filesystem::path> ConfigFile;

    ExecutionOptions Execution{};
    FilterOptions Filters{};
};

enum class ConfigErrorCode : uint8_t
{
    FileNotFound,
    AccessDenied,
    ParseError,
    InvalidValue
};

struct ConfigError
{
    ConfigErrorCode Code{};
    std::string Message;
    std::filesystem::path FilePath;
    size_t Line{0};
    size_t Column{0};
};

struct ResolvedConfig
{
    std::filesystem::path TargetPath;
    unsigned int Threads{1};

    ExecutionOptions Execution{};
    ScanFilters Filters{};

    std::vector<Core::ProjectRule> Rules;
};
} // namespace Reaper::Config
