cmake_minimum_required(VERSION 3.20)

# Utilisation du toolchain vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Path to vcpkg toolchain file")

project(ZenithEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ajout des dépendances (vcpkg gère tout ça)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

# Récupérer tous les fichiers source du projet
file(GLOB_RECURSE ZENITHENGINE_SOURCES
    "src/ZenithEngine/*.cpp"
)

add_executable(ZenithEngine ${ZENITHENGINE_SOURCES})

# Lie les bibliothèques à l'exécutable
 target_link_libraries(ZenithEngine PRIVATE glfw glad::glad nlohmann_json::nlohmann_json glm::glm)

# Inclure les headers du projet et des dépendances
 target_include_directories(ZenithEngine PRIVATE
    src/ZenithEngine
    ${Stb_INCLUDE_DIR}
 )

target_compile_definitions(ZenithEngine PRIVATE GLM_ENABLE_EXPERIMENTAL)
