#include "StringUtils.hpp"

#include <Windows.h>

#include <string_view>

namespace Reaper::Core
{
bool CaseInsensitiveCompare::Equals(std::wstring_view lhs, std::wstring_view rhs) noexcept
{
    if (lhs.size() != rhs.size())
    {
        return false;
    }

    if (lhs.empty())
    {
        return true;
    }

    return ::CompareStringOrdinal(
               lhs.data(), static_cast<int>(lhs.size()), rhs.data(), static_cast<int>(rhs.size()), TRUE) == CSTR_EQUAL;
}

bool CaseInsensitiveCompare::operator()(std::wstring_view lhs, std::wstring_view rhs) noexcept
{
    if (lhs.empty() || rhs.empty())
    {
        return lhs.size() < rhs.size();
    }

    return ::CompareStringOrdinal(
               lhs.data(), static_cast<int>(lhs.size()), rhs.data(), static_cast<int>(rhs.size()), TRUE) ==
           CSTR_LESS_THAN;
}
} // namespace Reaper::Core
