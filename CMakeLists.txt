cmake_minimum_required(VERSION 3.16)
project(ECE250)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the build output directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

# Set output directories inside build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)    # Executables go here
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)    # Shared libraries go here (if any)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)    # Static libraries go here (if any)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/country.cpp
    ${CMAKE_SOURCE_DIR}/src/time_series.cpp
)

# Create the executable
add_executable(main ${SOURCES})

# Enable warnings
target_compile_options(main PRIVATE -Wall -Wextra -pedantic -fno-implicit-modules -fno-implicit-module-maps)
