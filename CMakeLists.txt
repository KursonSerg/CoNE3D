# set(CMAKE_C_COMPILER "clang")
# set(CMAKE_CXX_COMPILER "clang++")

project(CoNE3D)
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    # Require at least GCC 5.0
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 5.0)
        message(FATAL_ERROR "GCC version must be at least 5.0!")
    endif()
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    # Require at least Clang 3.4
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.4)
        message(FATAL_ERROR "Clang version must be at least 3.4!")
    endif()
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    # Require at least MSVC 19.0
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 19.0)
        message(FATAL_ERROR "MSVC version must be at least 19.0 (Visual Studio 14 2015)!")
    endif()
endif()

# Version
set(CONE3D_VERSION_MAJOR    0)
set(CONE3D_VERSION_MINOR    0)
set(CONE3D_VERSION_REVISION 1)

# Configure a header file to pass CMake settings to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/Version.h.in"
  "${PROJECT_BINARY_DIR}/include/Version.h"
  )

# Add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}/include")

# Add glfw library
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
include_directories("${PROJECT_SOURCE_DIR}/deps/glfw/include")
add_subdirectory("${PROJECT_SOURCE_DIR}/deps/glfw")

# Add glad library
include_directories("${PROJECT_BINARY_DIR}/deps/glad/include")
add_subdirectory("${PROJECT_SOURCE_DIR}/deps/glad")

# Add glm library
include_directories("${PROJECT_SOURCE_DIR}/deps/glm")

# Add gli library
# include_directories("${PROJECT_SOURCE_DIR}/deps/gli")

# List includes
set(INCS include/Common.h
         include/CFormat.h
         include/CLogger.h
         include/CWindow.h
         include/CShader.h
         include/CObject.h
         include/CMesh.h
         include/CCamera.h
         include/CTexture.h)

# List sources
set(SRCS src/Main.cpp
         src/Common.cpp
         src/CFormat.cpp
         src/CLogger.cpp
         src/CWindow.cpp
         src/CShader.cpp
         src/CObject.cpp
         src/CMesh.cpp
         src/CCamera.cpp
         src/CTexture.cpp)

# Compiler flags
if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  set(CMAKE_C_CXX_FLAGS "/W4 /MD /Ox /Ob2 /Oi /Ot /Oy /GL /GF")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "/NOLOGO /INCREMENTAL:NO /OPT:REF /LTCG")
else()
  set(CMAKE_C_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -O3")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_CXX_FLAGS} -std=c11")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_CXX_FLAGS} -std=c++11")
endif()

# Add the executable
include_directories("${PROJECT_SOURCE_DIR}/include")
add_executable(${PROJECT_NAME} ${INCS} ${SRCS})
if(WIN32)
  target_link_libraries(${PROJECT_NAME} glad glfw OpenGL32)
else()
  target_link_libraries(${PROJECT_NAME} glad glfw GL X11 Xi Xrandr Xxf86vm Xinerama Xcursor pthread)
endif()
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${PROJECT_BINARY_DIR})
