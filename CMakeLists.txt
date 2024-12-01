cmake_minimum_required(VERSION 3.15)  # Minimum requis pour vcpkg
project(MyGLProject VERSION 1.0)

# Configuration de vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Path to vcpkg toolchain file")

# Ajout des options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Trouver les packages nécessaires
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

# Ajouter l'exécutable
add_executable(MyApp main.cpp)

# Lier les bibliothèques
target_link_libraries(MyApp PRIVATE glfw glad OpenGL::GL)
