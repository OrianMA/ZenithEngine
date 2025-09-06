cmake_minimum_required(VERSION 3.20)

# Use the vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Path to vcpkg toolchain file")

project(ZenithEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies resolved via vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Collect all project source files (except the main executable source)
file(GLOB_RECURSE ZENITHENGINE_SOURCES
    "src/ZenithEngine/*.cpp"
)

# Exclude app/entry sources from the library
list(REMOVE_ITEM ZENITHENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ZenithEngine/ZenithEngine.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ZenithEngine/ZenithApp.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ZenithEngine/ImGuiLayer.cpp"
)

# Create a library for shared sources
add_library(ZenithEngineLib ${ZENITHENGINE_SOURCES})

# Link required libraries to the shared library
target_link_libraries(ZenithEngineLib PRIVATE glfw glad::glad nlohmann_json::nlohmann_json glm::glm assimp::assimp imgui::imgui)

# Include project headers and dependencies
target_include_directories(ZenithEngineLib PRIVATE
    src/ZenithEngine
    ${Stb_INCLUDE_DIR}
)

target_compile_definitions(ZenithEngineLib PRIVATE GLM_ENABLE_EXPERIMENTAL)

# Create the main executable (hosts the app + ImGui layer)
add_executable(ZenithEngine
    src/ZenithEngine/ZenithEngine.cpp
    src/ZenithEngine/ZenithApp.cpp
    src/ZenithEngine/ImGuiLayer.cpp
)
target_link_libraries(ZenithEngine PRIVATE ZenithEngineLib imgui::imgui)
target_include_directories(ZenithEngine PRIVATE
    src/ZenithEngine
    ${Stb_INCLUDE_DIR}
)
target_compile_definitions(ZenithEngine PRIVATE GLM_ENABLE_EXPERIMENTAL IMGUI_IMPL_OPENGL_LOADER_GLAD)

# Unit test configuration
enable_testing()

# Collect tests
file(GLOB_RECURSE TEST_SOURCES
    "tests/*.cpp"
)

# Create test executable
add_executable(ZenithEngineTests ${TEST_SOURCES})

# Link dependencies for tests
target_link_libraries(ZenithEngineTests PRIVATE
    ZenithEngineLib
    GTest::gtest
    GTest::gtest_main
    assimp::assimp
)

# Include headers for tests
target_include_directories(ZenithEngineTests PRIVATE
    src/ZenithEngine
    ${Stb_INCLUDE_DIR}
)

target_compile_definitions(ZenithEngineTests PRIVATE GLM_ENABLE_EXPERIMENTAL)

# Register tests with CTest
add_test(NAME ZenithEngineTests COMMAND ZenithEngineTests)
