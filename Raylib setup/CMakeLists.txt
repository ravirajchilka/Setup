# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(RaylibPractice)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Raylib root directory
set(RAYLIB_DIR "C:/raylib/raylib/src")

# Include Raylib headers
include_directories(${RAYLIB_DIR})

# Set source files for your project
set(SOURCES
    src/main.cpp  # Add more source files as needed
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link Raylib library (Make sure Raylib is built and the `.lib` or `.a` files are present)
target_link_libraries(${PROJECT_NAME} 
    ${RAYLIB_DIR}/lib/libraylib.a  # Adjust if your lib file has a different name
    -lopengl32 -lgdi32 -lwinmm  # Add other libraries required by Raylib
)

# Set the output directory for executable (optional)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
