find_package(GTest CONFIG REQUIRED)

add_executable(Reaper.Tests 
    main.cpp
    Core/RuleMatcherTest.cpp
    Core/StringUtilsTest.cpp
)

target_link_libraries(Reaper.Tests 
    PRIVATE 
        Reaper::Core
        reaper_compiler_flags
        GTest::gtest
        GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(Reaper.Tests)
