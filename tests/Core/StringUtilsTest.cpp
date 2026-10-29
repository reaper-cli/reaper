#include "StringUtils.hpp"

#include <gtest/gtest.h>

namespace Reaper::Core
{
TEST(CaseInsensitiveCompareTest, EqualsIgnoresCase)
{
    EXPECT_TRUE(CaseInsensitiveCompare::Equals(L"", L""));
    EXPECT_TRUE(CaseInsensitiveCompare::Equals(L"Reaper", L"rEaPeR"));
}

TEST(CaseInsensitiveCompareTest, EqualsDetectsMismatches)
{
    EXPECT_FALSE(CaseInsensitiveCompare::Equals(L"", L"a"));
    EXPECT_FALSE(CaseInsensitiveCompare::Equals(L"Reaper", L"Reap"));
    EXPECT_FALSE(CaseInsensitiveCompare::Equals(L"a", L"B"));
}

TEST(CaseInsensitiveCompareTest, OperatorSortsCaseInsensitively)
{
    CaseInsensitiveCompare compare{};

    EXPECT_FALSE(compare(L"abc", L"ABC"));

    EXPECT_TRUE(compare(L"a", L"B"));
    EXPECT_FALSE(compare(L"B", L"a"));

    EXPECT_TRUE(compare(L"abc", L"abcd"));
    EXPECT_FALSE(compare(L"abcd", L"abc"));
}
} // namespace Reaper::Core
