#pragma once

#include <expected>

#include "ConfigTypes.hpp"

namespace Reaper::CLI
{
[[nodiscard]] std::expected<Config::CliOverrides, int> ParseCommandLine(int argc, wchar_t** argv);
} // namespace Reaper::CLI
