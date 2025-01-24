cmake_minimum_required(VERSION 3.1])

project(ZenithEngine VERSION 1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Path to vcpkg toolchain file")

set(FICHIERS_SOURCES
	ZenithEngine/ZenithEngine.cpp
	ZenithEngine/VBO.cpp
	ZenithEngine/shaderClass.cpp
	ZenithEngine/EBO.cpp

	ZenithEngine/EBO.h
	ZenithEngine/shaderClass.h
	ZenithEngine/VBO.h
)
# Trouver les packages nécessaires
find_package(glad CONFIG REQUIRED)

option(BUILD_GLFW3 "Download and build GLFW3" OFF)

# Use GLFW from system
if(NOT BUILD_GLFW3)
    find_package(glfw3 3.2.1)
    if(glfw3_FOUND)
        find_path(GLFW3_INCLUDE_DIR "GLFW/glfw3.h")
        find_library(GLFW3_LIBRARY "libglfw3.a")
        if (GLFW3_INCLUDE_DIR)
            message(STATUS "Looking for GLFW3 headers: ${GLFW3_INCLUDE_DIR} - found")
        else()
            message(STATUS "Looking for GLFW3 headers - not found")
            set(BUILD_GLFW3 ON)
        endif()
        if (GLFW3_LIBRARY)
            message(STATUS "Found GLFW3: ${GLFW3_LIBRARY}")
        else()
            message(STATUS "GLFW3 static library - not found")
            set(BUILD_GLFW3 ON)
        endif()
    else(glfw3_FOUND)
        message(STATUS "GLFW3 package - not found")
        set(BUILD_GLFW3 ON)
    endif(glfw3_FOUND)
endif(NOT BUILD_GLFW3)

# Build GLFW3 locally into the binary directory
if(BUILD_GLFW3)
    message(STATUS "GLFW3 will be downloaded and built")
    set(GLFW3_PREFIX ${CMAKE_BINARY_DIR}/glfw3)
    set(GLFW3_INCLUDE_DIR ${GLFW3_PREFIX}/include)
    set(GLFW3_LIBRARY ${GLFW3_PREFIX}/lib/libglfw3.a)

    # Build settings
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")

    # Download and build
    ExternalProject_Add(glfw3
        URL "https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip"
        PREFIX ${GLFW3_PREFIX}
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${GLFW3_PREFIX}"
        UPDATE_COMMAND "")
endif(BUILD_GLFW3)

# Global Variables
set(GLFW3_INCLUDE_DIRS ${GLFW3_INCLUDE_DIR} CACHE STRING "GLFW3 Include directories")
set(GLFW3_LIBRARIES ${GLFW3_LIBRARY}
    X11
    Xrandr
    Xinerama
    Xxf86vm
    Xcursor
    CACHE STRING "GLFW3 Libraries"
)
set(GLFW3_DEFINITIONS -DGLFW_STATIC)

# Ajouter l'exécutable
#add_executable(ZenithEngine/ZenithEngine/ZenithEngine.cpp)
target_link_libraries(LearnGLSL ${OPENGL_gl_LIBRARY} glfw)
# Lier les bibliothèques
target_link_libraries(ZenithEngine PRIVATE glfw glad OpenGL::GL)

#add_executable(ZenithEngine ${FICHIERS_SOURCES})
