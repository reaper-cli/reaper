#pragma once

#include <expected>
#include <filesystem>

#include "ConfigTypes.hpp"

namespace Reaper::Config
{
class ConfigLoader
{
public:
    ConfigLoader() = delete;

    [[nodiscard]] static std::expected<ResolvedConfig, ConfigError> Load(const CliOverrides& overrides);

    [[nodiscard]] static std::expected<void, ConfigError> GenerateInitFile(const std::filesystem::path& destination,
                                                                           bool force);
};
} // namespace Reaper::Config
