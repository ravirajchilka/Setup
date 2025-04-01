cmake_minimum_required(VERSION 3.10)
project(RaylibAndTorchProject CUDA CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Set CUDA Toolkit root directory
set(CUDA_TOOLKIT_ROOT_DIR "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.8")
find_package(CUDAToolkit 12.8 REQUIRED)

# Set Torch root directory
set(TORCH_ROOT "E:/Programs/libtorch_release_12p6_version_2p6p0/libtorch")
list(APPEND CMAKE_PREFIX_PATH ${TORCH_ROOT})

# Find Torch
find_package(Torch REQUIRED)

# cuDNN configuration
option(USE_CUDNN "Use cuDNN" ON)
if(USE_CUDNN)
  find_library(CUDNN_LIBRARY cudnn
               HINTS "${CUDA_TOOLKIT_ROOT_DIR}/lib/x64")
  if(CUDNN_LIBRARY)
    message(STATUS "cuDNN found: ${CUDNN_LIBRARY}")
    add_definitions(-DUSE_CUDNN)
    include_directories("${CUDA_TOOLKIT_ROOT_DIR}/include")
    list(APPEND TORCH_LIBRARIES ${CUDNN_LIBRARY})
  else()
    message(WARNING "cuDNN not found. Compiling without cuDNN support.")
    set(USE_CUDNN OFF)
  endif()
endif()

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
    ${TORCH_LIBRARIES}                  # Link Torch libraries
    CUDA::cudart                        # Link CUDA Runtime library
)

# Copy necessary DLLs after build for Torch and CUDA
if(MSVC)
  file(GLOB TORCH_DLLS "${TORCH_ROOT}/lib/*.dll")
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${TORCH_DLLS}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )
endif(MSVC)

# Output Directory Configuration
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
