cmake_minimum_required (VERSION 3.28)

include(CheckIPOSupported)

cmake_policy(SET CMP0091 NEW)
project(NYAN
        VERSION 0.0.1
        LANGUAGES CXX)

if(EXISTS "UserSettings.txt")
    include("UserSettings.txt")
endif()

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------

option(BUILD_TESTS "Build tests" ON)
option(BUILD_SAMPLES "Build samples" ON)

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------

add_subdirectory(src)

# ---------------------------------------------------------------------------
# Testing
# ---------------------------------------------------------------------------

if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()

# ---------------------------------------------------------------------------
# Samples
# ---------------------------------------------------------------------------

if (BUILD_SAMPLES)
    add_subdirectory(samples)
endif ()