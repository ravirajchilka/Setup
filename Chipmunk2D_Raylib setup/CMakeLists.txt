cmake_minimum_required(VERSION 3.10)
project(RaylibPractice)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Raylib Configuration
set(RAYLIB_INCLUDE_DIR "E:/Programs/Raylib/raylib/src")  # Path to Raylib headers
set(RAYLIB_LIB_DIR "E:/Programs/Raylib/raylib/projects/VS2022/build/raylib/bin/x64/Debug") # Path to Raylib .lib files
include_directories(${RAYLIB_INCLUDE_DIR})
link_directories(${RAYLIB_LIB_DIR})

# Chipmunk2D Configuration
set(CHIPMUNK2D_DIR "E:/Programs/Chipmunk2D")  # Change this to your Chipmunk2D path
include_directories(${CHIPMUNK2D_DIR}/include/chipmunk)

set(CHIPMUNK2D_LIB_DIR "E:/Programs/Chipmunk2D/msvc/VS2015/chipmunk/x64/Debug") # Path to Chipmunk2D .lib files
link_directories(${CHIPMUNK2D_LIB_DIR})

# Source files
set(SOURCES src/main.cpp)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link Raylib and Chipmunk2D libraries
target_link_libraries(${PROJECT_NAME}
    raylib.lib                          # Link Raylib .lib file
    ${CHIPMUNK2D_LIB_DIR}/chipmunk.lib   # Link Chipmunk2D .lib file
    opengl32                            # OpenGL library (required by Raylib)
    gdi32                               # Windows graphics library (required by Raylib)
    winmm                               # Windows multimedia library (required by Raylib)
    kernel32                            # Windows kernel library (standard)
    user32                              # Windows user interface library (standard)
    shell32                             # Windows shell library (standard)
    ole32                               # Object Linking and Embedding library
    oleaut32                            # OLE Automation library
    uuid                                # UUID management library
    comdlg32                            # Common dialog box library
    advapi32                            # Advanced Windows API library
)

# Output Directory Configuration
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
