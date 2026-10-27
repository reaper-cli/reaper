#pragma once

#include <algorithm>
#include <string_view>

namespace Reaper::Core
{
struct CaseInsensitiveCompare
{
    using is_transparent = void; // NOLINT(readability-identifier-naming)

    [[nodiscard]] static constexpr wchar_t ToLower(wchar_t ch) noexcept
    {
        return (ch >= L'A' && ch <= L'Z') ? static_cast<wchar_t>(ch + (L'a' - L'A')) : ch;
    }

    [[nodiscard]] static constexpr bool Equals(std::wstring_view lhs, std::wstring_view rhs) noexcept
    {
        return std::ranges::equal(
            lhs, rhs, [](wchar_t lhsChar, wchar_t rhsChar) noexcept { return ToLower(lhsChar) == ToLower(rhsChar); });
    }

    [[nodiscard]] static constexpr bool operator()(std::wstring_view lhs, std::wstring_view rhs) noexcept
    {
        return std::ranges::lexicographical_compare(
            lhs, rhs, [](wchar_t lhsChar, wchar_t rhsChar) noexcept { return ToLower(lhsChar) < ToLower(rhsChar); });
    }
};
} // namespace Reaper::Core
