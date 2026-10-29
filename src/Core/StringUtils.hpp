#pragma once

#include <string_view>

namespace Reaper::Core
{
struct CaseInsensitiveCompare
{
    using is_transparent = void; // NOLINT(readability-identifier-naming)

    [[nodiscard]] static bool Equals(std::wstring_view lhs, std::wstring_view rhs) noexcept;

    [[nodiscard]] static bool operator()(std::wstring_view lhs, std::wstring_view rhs) noexcept;
};
} // namespace Reaper::Core
