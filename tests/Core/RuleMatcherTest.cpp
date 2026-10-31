#include "RuleMatcher.hpp"

#include <gtest/gtest.h>

#include <array>
#include <span>
#include <string_view>

#include "DirectoryEntry.hpp"
#include "ProjectRule.hpp"

namespace Reaper::Core
{
namespace
{
[[nodiscard]] DirectoryEntry MakeDir(std::wstring_view name)
{
    return DirectoryEntry{.Name = std::wstring{name}, .IsDirectory = true};
}

[[nodiscard]] DirectoryEntry MakeFile(std::wstring_view name)
{
    return DirectoryEntry{.Name = std::wstring{name}, .IsDirectory = false};
}

void ExpectMatch(const ProjectRule* rule, std::string_view expectedEcosystem)
{
    ASSERT_NE(rule, nullptr);
    EXPECT_EQ(rule->Ecosystem, expectedEcosystem);
}

class PopulatedRuleMatcherTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_matcher.AddRule(ProjectRule{
            .Ecosystem = "RuleA",
            .RequiredDirectoryNames = {L"dir_a", L"dir_b"},
            .RequiredFileNames = {L"file.txt"},
            .AnyFileExtensions = {L".ext"},
            .ArtifactDirectories = {L"out"},
        });
        m_matcher.AddRule(ProjectRule{
            .Ecosystem = "RuleB",
            .AnyFileExtensions = {L".ext1", L".ext2"},
            .ArtifactDirectories = {L"out"},
        });
    }

    RuleMatcher m_matcher;
};
} // namespace

TEST(RuleMatcherTest, AddExcludedDirectoryIgnoresEmpty)
{
    RuleMatcher matcher;
    matcher.AddExcludedDirectory(L"");

    EXPECT_FALSE(matcher.IsExcluded(L""));
}

TEST(RuleMatcherTest, IsExcludedIgnoresCase)
{
    RuleMatcher matcher;
    matcher.AddExcludedDirectory(L"dir");

    EXPECT_TRUE(matcher.IsExcluded(L"DIR"));
}

TEST(RuleMatcherTest, AddRuleNormalizesExtensions)
{
    RuleMatcher matcher;
    matcher.AddRule(ProjectRule{
        .Ecosystem = "Test",
        .AnyFileExtensions = {L"ext"},
        .ArtifactDirectories = {L"out"},
    });

    const std::array entries{MakeFile(L"app.ext")};
    ExpectMatch(matcher.Identify(entries), "Test");
}

TEST(RuleMatcherTest, AddRuleIgnoresInvalidRules)
{
    RuleMatcher matcher;
    matcher.AddRule(ProjectRule{.Ecosystem = "Empty"});
    matcher.AddRule(ProjectRule{.Ecosystem = "NoReqs", .ArtifactDirectories = {L"out"}});
    matcher.AddRule(ProjectRule{.Ecosystem = "NoArtifacts", .RequiredFileNames = {L"file.txt"}});

    const std::array entries{MakeDir(L"out"), MakeFile(L"file.txt")};
    EXPECT_EQ(matcher.Identify(entries), nullptr);
}

TEST(RuleMatcherTest, IdentifyFollowsRegistrationOrder)
{
    RuleMatcher matcher;
    matcher.AddRule(
        ProjectRule{.Ecosystem = "First", .RequiredFileNames = {L"file.txt"}, .ArtifactDirectories = {L"out"}});
    matcher.AddRule(
        ProjectRule{.Ecosystem = "Second", .RequiredFileNames = {L"file.txt"}, .ArtifactDirectories = {L"out"}});

    const std::array entries{MakeFile(L"file.txt")};
    ExpectMatch(matcher.Identify(entries), "First");
}

TEST_F(PopulatedRuleMatcherTest, IdentifyMatchesAllRequirements)
{
    const std::array missingDir{MakeDir(L"dir_a"), MakeFile(L"file.txt"), MakeFile(L"app.ext")};
    EXPECT_EQ(m_matcher.Identify(missingDir), nullptr);

    const std::array missingFile{MakeDir(L"dir_a"), MakeDir(L"dir_b"), MakeFile(L"app.ext")};
    EXPECT_EQ(m_matcher.Identify(missingFile), nullptr);

    const std::array missingExt{MakeDir(L"dir_a"), MakeDir(L"dir_b"), MakeFile(L"file.txt")};
    EXPECT_EQ(m_matcher.Identify(missingExt), nullptr);

    const std::array complete{MakeDir(L"dir_a"), MakeDir(L"dir_b"), MakeFile(L"file.txt"), MakeFile(L"app.ext")};
    ExpectMatch(m_matcher.Identify(complete), "RuleA");
}

TEST_F(PopulatedRuleMatcherTest, IdentifyIgnoresCase)
{
    const std::array uppercase{MakeDir(L"DIR_A"), MakeDir(L"DIR_B"), MakeFile(L"FILE.TXT"), MakeFile(L"APP.EXT")};
    ExpectMatch(m_matcher.Identify(uppercase), "RuleA");
}

TEST_F(PopulatedRuleMatcherTest, IdentifyMatchesAnyExtension)
{
    const std::array firstExt{MakeFile(L"a.ext1")};
    ExpectMatch(m_matcher.Identify(firstExt), "RuleB");

    const std::array secondExt{MakeFile(L"b.ext2")};
    ExpectMatch(m_matcher.Identify(secondExt), "RuleB");
}

TEST_F(PopulatedRuleMatcherTest, IdentifyReturnsNullWhenEmpty)
{
    const RuleMatcher emptyMatcher;
    const std::array entries{MakeFile(L"a.ext1")};
    EXPECT_EQ(emptyMatcher.Identify(entries), nullptr);

    constexpr std::span<const DirectoryEntry> emptyEntries;
    EXPECT_EQ(m_matcher.Identify(emptyEntries), nullptr);
}

TEST_F(PopulatedRuleMatcherTest, IdentifyEnforcesEntryTypes)
{
    const std::array swappedTypes{
        MakeFile(L"dir_a"),
        MakeFile(L"dir_b"),
        MakeDir(L"file.txt"),
        MakeDir(L"app.ext"),
        MakeDir(L"a.ext1"),
    };
    EXPECT_EQ(m_matcher.Identify(swappedTypes), nullptr);
}
} // namespace Reaper::Core
