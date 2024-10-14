cmake_minimum_required (VERSION 3.28)

#set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
  # This specific value changes as experimental support evolves. See
  # `Help/dev/experimental.rst` in the CMake source corresponding to
  # your CMake build for the exact value to use.
 # "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")

include(CheckIPOSupported)

cmake_policy(SET CMP0091 NEW)
project(NYAN
        VERSION 0.0.1
        LANGUAGES CXX)

if(EXISTS "UserSettings.txt")
    include("UserSettings.txt")
endif()

#set(CMAKE_CXX_MODULE_STD 1)
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