﻿cmake_minimum_required (VERSION 3.28)

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

option(NYAN_UTIL_BUILD_TESTS "Build tests" ON)
option(NYAN_UTIL_BUILD_SAMPLES "Build samples" ON)

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------

add_subdirectory(src)

# ---------------------------------------------------------------------------
# Testing
# ---------------------------------------------------------------------------

if (NYAN_UTIL_BUILD_TESTS)
    add_subdirectory(tests)
endif ()

# ---------------------------------------------------------------------------
# Samples
# ---------------------------------------------------------------------------

if (NYAN_UTIL_BUILD_SAMPLES)
    add_subdirectory(samples)
endif ()