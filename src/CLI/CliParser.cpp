#include "CliParser.hpp"

#include <CLI/CLI.hpp>

#include <algorithm>
#include <expected>
#include <filesystem>
#include <thread>

namespace Reaper::CLI
{
std::expected<CliOptions, int> ParseCommandLine(int argc, wchar_t** argv)
{
    CliOptions options{};
    options.TargetPath = std::filesystem::current_path();
    options.Threads = std::max(1U, std::thread::hardware_concurrency());

    ::CLI::App app{"Reaper - A swift scythe for project artifacts"};
    app.set_version_flag("-V,--version", "0.1.0");

    app.add_option("path", options.TargetPath, "Set the target directory to scan");

    app.add_option("-j,--threads", options.Threads, "Set the number of worker threads")->check(::CLI::PositiveNumber);
    app.add_option("-c,--config", options.ConfigFile, "Provide a path to a custom configuration file");

    auto* execGroup = app.add_option_group("Execution");
    execGroup->add_flag("-y,--yes", options.Execution.Yes, "Skip prompts and execute automatically");
    execGroup->add_flag("-n,--dry-run", options.Execution.DryRun, "Scan and evaluate only; do not delete files");
    execGroup->add_flag("--headless", options.Execution.Headless, "Run without the TUI");
    execGroup->add_flag("-f,--force", options.Execution.Force, "Force the deletion of artifacts, ignoring warnings");

    auto* filterGroup = app.add_option_group("Filters");
    filterGroup->add_option(
        "--older-than", options.Filters.OlderThan, "Filter projects untouched for a duration (e.g., 30d, 2w)");
    filterGroup->add_option(
        "--min-size", options.Filters.MinSize, "Filter projects with artifacts larger than a size (e.g., 500MB, 2GB)");
    filterGroup->add_option("-d,--max-depth", options.Filters.MaxDepth, "Limit the maximum directory depth");

    filterGroup
        ->add_option("-e,--ecosystem", options.Filters.Ecosystems, "Target specific ecosystems (e.g., unity, unreal)")
        ->delimiter(',');
    filterGroup->add_option("-E,--exclude-ecosystem", options.Filters.ExcludeEcosystems, "Exclude specific ecosystems")
        ->delimiter(',');
    filterGroup->add_option("--exclude-dir", options.Filters.ExcludeDirectories, "Exclude specific directories")
        ->delimiter(',');

    try
    {
        app.parse(argc, argv);
    }
    catch (const ::CLI::ParseError& e)
    {
        return std::unexpected(app.exit(e));
    }

    return options;
}
} // namespace Reaper::CLI
