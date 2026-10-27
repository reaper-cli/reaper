#include "ConfigLoader.hpp"

#include <expected>
#include <filesystem>

#include "ConfigTypes.hpp"

namespace Reaper::Config
{
std::expected<ResolvedConfig, ConfigError> ConfigLoader::Load([[maybe_unused]] const CliOverrides& overrides)
{
    // TODO: Implement
    return {};
}

std::expected<void, ConfigError>
ConfigLoader::GenerateInitFile([[maybe_unused]] const std::filesystem::path& destination, [[maybe_unused]] bool force)
{
    // TODO: Implement
    return {};
}
} // namespace Reaper::Config
