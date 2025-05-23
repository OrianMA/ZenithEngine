cmake_minimum_required(VERSION 3.1])

set(CMAKE_TOOLCHAIN_FILE "../vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Path to vcpkg toolchain file")
include(../vcpkg/scripts/buildsystems/vcpkg.cmake)
include_directories("../vcpkg/installed/x64-windows/include")

project(ZenithEngine VERSION 1.0 LANGUAGES CXX)

set(glad_DIR "../vcpkg/installed/x64-windows/share/glad")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)

#set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Path to vcpkg toolchain file")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ..${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/share/)
cmake_minimum_required(VERSION 3.20)


set(glad_DIR "../vcpkg/installed/x64-windows/share/glad")
# Ajout des dépendances (vcpkg gère tout ça)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)   # Glad
find_package(nlohmann_json 3.11.3 REQUIRED)
message(STATUS "glad found at: ${glad_DIR}")

# Ajoute les fichiers source de ton projet
add_executable(ZenithEngine
    ZenithEngine/ZenithEngine.cpp
)
add_library(foo ...)

# Lie les bibliothèques à ton exécutable
target_link_libraries(ZenithEngine PRIVATE glfw glad)
target_link_libraries(foo PRIVATE nlohmann_json::nlohmann_json)

# Inclure les headers des bibliothèques
target_include_directories(ZenithEngine PRIVATE
    ${GLFW3_INCLUDE_DIRS}
    ${GLAD_INCLUDE_DIRS}
)
