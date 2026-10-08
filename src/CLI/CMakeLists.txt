find_package(CLI11 CONFIG REQUIRED)

add_executable(Reaper.CLI 
    main.cpp
    CliParser.hpp
    CliParser.cpp
)

set_target_properties(Reaper.CLI PROPERTIES OUTPUT_NAME "rpr")

target_link_libraries(Reaper.CLI PRIVATE 
    Reaper::Core
    reaper_compiler_flags
    CLI11::CLI11
)
