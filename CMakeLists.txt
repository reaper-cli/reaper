cmake_minimum_required(VERSION 3.28)
project(Reaper VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

include(CTest)
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_library(reaper_compiler_flags INTERFACE)
if(MSVC)
    target_compile_options(reaper_compiler_flags INTERFACE 
        /W4 /WX /permissive- 
        /Zc:__cplusplus 
        /Zc:preprocessor 
        /utf-8 
        /Gw
        /EHsc
        /wd4324
    )    
    target_compile_definitions(reaper_compiler_flags INTERFACE 
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
endif()

add_subdirectory(src)
