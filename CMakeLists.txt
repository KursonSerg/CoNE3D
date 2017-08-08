# set(CMAKE_C_COMPILER "clang")
# set(CMAKE_CXX_COMPILER "clang++")

project(CoNE3D)
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    # Require at least GCC 5.0
    if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 5.0)
        message(FATAL_ERROR "GCC version must be at least 5.0!")
    endif()
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    # Require at least Clang 3.4
    if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.4)
        message(FATAL_ERROR "Clang version must be at least 3.4!")
    endif()
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    # Require at least MSVC 19.0
    if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 19.0)
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

# Add assimp library
option(ASSIMP_BUILD_ASSIMP_TOOLS "Build the supplementary tools" OFF)
option(ASSIMP_BUILD_TESTS "Build the test suite" OFF)
option(ASSIMP_NO_EXPORT "Disable export functionality" ON)

option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT "Default value for all importers" OFF)
option(ASSIMP_BUILD_OBJ_IMPORTER "Value for obj importer" ON)

include_directories("${PROJECT_SOURCE_DIR}/deps/assimp/include")
include_directories("${PROJECT_BINARY_DIR}/deps/assimp/include")
add_subdirectory("${PROJECT_SOURCE_DIR}/deps/assimp")

# Add stb libraries collection (include only)
include_directories("${PROJECT_SOURCE_DIR}/deps/stb")

set(BUFFERS
    include/Buffers/VertexArray.h
    include/Buffers/VertexBuffer.h
    include/Buffers/IndexBuffer.h
    include/Buffers/UniformBuffer.h
    include/Buffers/CameraBuffer.h
    src/Buffers/VertexArray.cpp
    src/Buffers/VertexBuffer.cpp
    src/Buffers/IndexBuffer.cpp
    src/Buffers/UniformBuffer.cpp
    src/Buffers/CameraBuffer.cpp)

set(DRAWABLES
    include/Drawables/Drawable.h
    include/Drawables/IndexedDrawable.h
    src/Drawables/Drawable.cpp
    src/Drawables/IndexedDrawable.cpp)

set(SHADERS
    include/Shaders/Shader.h
    include/Shaders/ShaderParser.h
    include/Shaders/Program.h
    src/Shaders/Shader.cpp
    src/Shaders/ShaderParser.cpp
    src/Shaders/Program.cpp)

# List includes
set(INCS
    include/Common.h
    include/CFormat.h
    include/CLogger.h
    include/CWindow.h
    include/CObject.h
    include/CModel.h
    include/CCamera.h
    include/CTexture.h)

# List sources
set(SRCS
    src/Main.cpp
    src/Common.cpp
    src/CFormat.cpp
    src/CLogger.cpp
    src/CWindow.cpp
    src/CObject.cpp
    src/CModel.cpp
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
add_executable(${PROJECT_NAME} ${INCS} ${SRCS} ${BUFFERS} ${DRAWABLES} ${SHADERS})
if(WIN32)
    target_link_libraries(${PROJECT_NAME} glfw glad assimp OpenGL32)
else()
    target_link_libraries(${PROJECT_NAME} glfw glad assimp GL X11 Xi Xrandr Xxf86vm Xinerama Xcursor pthread)
endif()

# Copy all assets
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${PROJECT_BINARY_DIR})
