add_library(Reaper.Config STATIC 
    ConfigLoader.hpp
    ConfigLoader.cpp
    ConfigTypes.hpp
)

add_library(Reaper::Config ALIAS Reaper.Config)

target_include_directories(Reaper.Config 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(Reaper.Config 
    PUBLIC 
        Reaper::Core
    PRIVATE 
        reaper_compiler_flags
)
