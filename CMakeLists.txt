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
find_package(GTest CONFIG REQUIRED)

# Récupérer tous les fichiers source du projet (sauf le main)
file(GLOB_RECURSE ZENITHENGINE_SOURCES
    "src/ZenithEngine/*.cpp"
)

# Exclure le fichier principal du projet
list(REMOVE_ITEM ZENITHENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/ZenithEngine/ZenithEngine.cpp")

# Créer une bibliothèque pour les sources communes
add_library(ZenithEngineLib ${ZENITHENGINE_SOURCES})

# Lier les bibliothèques à la bibliothèque
target_link_libraries(ZenithEngineLib PRIVATE glfw glad::glad nlohmann_json::nlohmann_json glm::glm)

# Inclure les headers du projet et des dépendances
target_include_directories(ZenithEngineLib PRIVATE
    src/ZenithEngine
    ${Stb_INCLUDE_DIR}
)

target_compile_definitions(ZenithEngineLib PRIVATE GLM_ENABLE_EXPERIMENTAL)

# Créer l'exécutable principal
add_executable(ZenithEngine src/ZenithEngine/ZenithEngine.cpp)
target_link_libraries(ZenithEngine PRIVATE ZenithEngineLib)
target_include_directories(ZenithEngine PRIVATE
    src/ZenithEngine
    ${Stb_INCLUDE_DIR}
)
target_compile_definitions(ZenithEngine PRIVATE GLM_ENABLE_EXPERIMENTAL)

# Configuration des tests unitaires
enable_testing()

# Récupérer tous les fichiers de test
file(GLOB_RECURSE TEST_SOURCES
    "tests/*.cpp"
)

# Créer l'exécutable de test
add_executable(ZenithEngineTests ${TEST_SOURCES})

# Lier les bibliothèques nécessaires pour les tests
target_link_libraries(ZenithEngineTests PRIVATE 
    ZenithEngineLib
    GTest::gtest 
    GTest::gtest_main
)

# Inclure les headers pour les tests
target_include_directories(ZenithEngineTests PRIVATE
    src/ZenithEngine
    ${Stb_INCLUDE_DIR}
)

target_compile_definitions(ZenithEngineTests PRIVATE GLM_ENABLE_EXPERIMENTAL)

# Ajouter les tests à CTest
add_test(NAME ZenithEngineTests COMMAND ZenithEngineTests)
