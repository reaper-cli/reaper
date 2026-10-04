add_executable(Reaper.CLI 
    main.cpp
)

set_target_properties(Reaper.CLI PROPERTIES OUTPUT_NAME "rpr")

target_link_libraries(Reaper.CLI PRIVATE 
    Reaper::Core
    reaper_compiler_flags
)
