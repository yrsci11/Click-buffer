cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    geode
    GIT_REPOSITORY https://github.com/geode-sdk/geode.git
    GIT_TAG main
)
FetchContent_MakeAvailable(geode)

project(ClickDelayBuffer VERSION 1.0.0)

setup_geode_mod(
    NAME "Click Delay Buffer"
    ID "yrrcsi.click_delay_buffer"
)

add_subdirectory(src)
