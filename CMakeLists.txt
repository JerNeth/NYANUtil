cmake_minimum_required (VERSION 3.30)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
  # This specific value changes as experimental support evolves. See
  # `Help/dev/experimental.rst` in the CMake source corresponding to
  # your CMake build for the exact value to use.
  "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")

include(CheckIPOSupported)



cmake_policy(SET CMP0091 NEW)
project(NYAN
        VERSION 0.0.1
        LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 23)
       
#set(CPPSTANDARDMODULE_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/EnableStandardLibraryModule.cmake")

#if(NOT (EXISTS ${CPPSTANDARDMODULE_DOWNLOAD_LOCATION}))
#    message(STATUS "Downloading EnableStandardLibraryModule.cmake")
#    file(DOWNLOAD https://raw.githubusercontent.com/stripe2933/CppStandardLibraryModule/main/cmake/EnableStandardLibraryModule.cmake ${CPPSTANDARDMODULE_DOWNLOAD_LOCATION})
#endif()
#include(${CPPSTANDARDMODULE_DOWNLOAD_LOCATION})

#Project Wide Settings
#All Options which change the ABI come here
message("Detected ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    string(REGEX REPLACE "/W[3|4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    add_compile_options(/wd26812)
    add_compile_options(/DELAY:NOBIND)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
endif()

set(CMAKE_CXX_MODULE_STD 1)
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
    set(CPM_DOWNLOAD_VERSION 0.40.2) 
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

    if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
        message(STATUS "Downloading CPM.cmake")
        file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
    endif()
    include(${CPM_DOWNLOAD_LOCATION})

    enable_testing()
    add_subdirectory(tests)
endif ()

# ---------------------------------------------------------------------------
# Samples
# ---------------------------------------------------------------------------

if (NYAN_UTIL_BUILD_SAMPLES)
    add_subdirectory(samples)
endif ()