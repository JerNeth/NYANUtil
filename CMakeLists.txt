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

#Project Wide Settings
#All Options which change the ABI come here
message("Detected ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
#if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
#    string(REGEX REPLACE "/W[3|4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#    add_compile_options(/wd26812)
#    add_compile_options(/DELAY:NOBIND)
#    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#endif()

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.36" AND CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )

    set(VCTOOLS_INSTALL_DIR ${CMAKE_CXX_COMPILER})
    cmake_path(REMOVE_FILENAME VCTOOLS_INSTALL_DIR)
    cmake_path(GET VCTOOLS_INSTALL_DIR PARENT_PATH tmp)
    cmake_path(GET tmp PARENT_PATH VCTOOLS_INSTALL_DIR)
    cmake_path(GET VCTOOLS_INSTALL_DIR PARENT_PATH tmp)
    cmake_path(GET tmp PARENT_PATH VCTOOLS_INSTALL_DIR)

    message("VCTOOLS_INSTALL_DIR: ${VCTOOLS_INSTALL_DIR}")

    include(FetchContent)
    FetchContent_Declare(
        std
        URL "file://${VCTOOLS_INSTALL_DIR}/modules"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        SYSTEM
    )
    FetchContent_MakeAvailable(std)

    add_library(std STATIC)
    target_sources(std PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${std_SOURCE_DIR}
        FILES
            ${std_SOURCE_DIR}/std.ixx
            ${std_SOURCE_DIR}/std.compat.ixx
    )

    target_compile_features(std PRIVATE cxx_std_23)
    target_compile_features(std PUBLIC cxx_std_20)

    get_target_property(std_msvc_runtime std MSVC_RUNTIME_LIBRARY)
    message(STATUS "Runtime library for std: ${std_msvc_runtime}")

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