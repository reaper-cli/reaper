add_library(Reaper.Core STATIC 
    DirectoryEntry.hpp
    ProjectRule.hpp
    RuleMatcher.hpp
    RuleMatcher.cpp
    StringUtils.hpp
    StringUtils.cpp
)

add_library(Reaper::Core ALIAS Reaper.Core)

target_include_directories(Reaper.Core 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(Reaper.Core 
    PRIVATE 
        reaper_compiler_flags
)
