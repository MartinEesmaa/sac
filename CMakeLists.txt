cmake_minimum_required(VERSION 3.10)
project(sac)

# Making sure C++11 or later is used
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Verify that the compiler supports C++11 or later
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++11 COMPILER_SUPPORTS_CXX11)
if (NOT COMPILER_SUPPORTS_CXX11)
    message(FATAL_ERROR "Compiler does not support C++11 or later. Please use a compatible compiler.")
endif()

# Include directories
include_directories(
    src/common/
    src/file/
    src/libsac/
    src/model/
    src/opt/
    src/pred/
)

# Source files
set(SOURCE_FILES
    src/main.cpp
    src/cmdline.cpp
    src/common/md5.cpp
    src/common/utils.cpp
    src/file/file.cpp
    src/file/sac.cpp
    src/file/wav.cpp
    src/libsac/libsac.cpp
    src/libsac/map.cpp
    src/libsac/pred.cpp
    src/libsac/profile.cpp
    src/libsac/vle.cpp
    src/model/range.cpp
    src/pred/rls.cpp
)

# Microsoft Visual Studio is not supported due to syntax error of vle.cpp
if (MSVC)
    message(FATAL_ERROR "MSVC is not supported due to syntax error of vle.cpp")
endif()

# Create executable
add_executable(sac ${SOURCE_FILES})

# Set compiler flags based on the compiler being used
target_compile_options(sac PRIVATE -Wall -O3 -std=c++11)

# Link libc++
target_link_libraries(sac stdc++)
