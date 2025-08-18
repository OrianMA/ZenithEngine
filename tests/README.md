# ZenithEngine Unit Tests Documentation

This document explains how to use and run the unit tests for the ZenithEngine project.

## 📋 Overview

The project uses **Google Test (GTest)** framework for unit testing, configured through CMake and vcpkg. All test files are located in the `tests/` directory.

## 🏗️ Project Structure

```
tests/
├── README.md              # This documentation
├── CMakeLists.txt         # Test-specific CMake configuration
├── main_test.cpp          # Main test suite (Shader, Camera, Math, File, String tests)
├── simple_test.cpp        # Basic test examples
├── model_test.cpp         # Model-specific tests
├── run_tests.bat          # Windows batch script for running tests
├── run_tests.ps1          # Windows PowerShell script for running tests
├── run_tests.sh           # Linux/macOS shell script for running tests
└── .vscode/
    └── tasks.json         # VS Code task configurations
```

## 🚀 Quick Start

### Prerequisites

**All Platforms:**
- CMake 3.20 or higher
- vcpkg package manager (already configured)

**Windows:**
- Visual Studio 2022 or compatible compiler
- PowerShell (for PowerShell script) or Command Prompt (for batch script)

**Linux/macOS:**
- GCC 9+ or Clang 10+ compiler
- Bash shell (for shell script)

### Building and Running Tests

1. **Navigate to the project root:**
   ```bash
   cd ZenithEngine
   ```

2. **Create and enter build directory:**
   ```bash
   mkdir build
   cd build
   ```

3. **Configure the project:**
   ```bash
   cmake ..
   ```

4. **Build the project and tests:**
   ```bash
   cmake --build . --config Release
   ```

5. **Run all tests:**
   ```bash
   # Windows
   .\Release\ZenithEngineTests.exe
   
   # Linux/macOS
   ./Release/ZenithEngineTests
   ```

### Using the Test Runner Scripts

For easier test execution, use the provided scripts:

**Windows (Batch):**
```bash
# Navigate to tests directory
cd tests

# Run all tests
.\run_tests.bat

# Run specific test suite
.\run_tests.bat -Filter SimpleTest.*

# Run with verbose output
.\run_tests.bat -Verbose -Color

# Build and run tests
.\run_tests.bat -Build
```

**Windows (PowerShell):**
```powershell
# Navigate to tests directory
cd tests

# Run all tests
.\run_tests.ps1

# Run specific test suite
.\run_tests.ps1 -Filter "SimpleTest.*"

# Run with verbose output
.\run_tests.ps1 -Verbose -Color

# Build and run tests
.\run_tests.ps1 -Build
```

**Linux/macOS:**
```bash
# Navigate to tests directory
cd tests

# Make script executable (first time only)
chmod +x run_tests.sh

# Run all tests
./run_tests.sh

# Run specific test suite
./run_tests.sh -f "SimpleTest.*"

# Run with verbose output
./run_tests.sh -v -c

# Build and run tests
./run_tests.sh -b
```

## 🧪 Test Commands

### Basic Commands

```bash
# Run all tests
# Windows
.\Release\ZenithEngineTests.exe
# Linux/macOS
./Release/ZenithEngineTests

# Run with verbose output
# Windows
.\Release\ZenithEngineTests.exe --gtest_verbose
# Linux/macOS
./Release/ZenithEngineTests --gtest_verbose

# Run with colored output
# Windows
.\Release\ZenithEngineTests.exe --gtest_color=yes
# Linux/macOS
./Release/ZenithEngineTests --gtest_color=yes

# Run tests and show timing
# Windows
.\Release\ZenithEngineTests.exe --gtest_print_time=1
# Linux/macOS
./Release/ZenithEngineTests --gtest_print_time=1
```

### Filtering Tests

```bash
# Run specific test suite
# Windows
.\Release\ZenithEngineTests.exe --gtest_filter=SimpleTest.*
# Linux/macOS
./Release/ZenithEngineTests --gtest_filter=SimpleTest.*

# Run specific test
# Windows
.\Release\ZenithEngineTests.exe --gtest_filter=SimpleTest.BasicAssertion
# Linux/macOS
./Release/ZenithEngineTests --gtest_filter=SimpleTest.BasicAssertion

# Run multiple test suites
# Windows
.\Release\ZenithEngineTests.exe --gtest_filter=SimpleTest.*:MathTest.*
# Linux/macOS
./Release/ZenithEngineTests --gtest_filter=SimpleTest.*:MathTest.*

# Exclude specific tests
# Windows
.\Release\ZenithEngineTests.exe --gtest_filter=-SimpleTest.BasicAssertion
# Linux/macOS
./Release/ZenithEngineTests --gtest_filter=-SimpleTest.BasicAssertion

# Run tests matching a pattern
# Windows
.\Release\ZenithEngineTests.exe --gtest_filter=*Math*
# Linux/macOS
./Release/ZenithEngineTests --gtest_filter=*Math*
```

### Test Output Options

```bash
# Generate XML report
# Windows
.\Release\ZenithEngineTests.exe --gtest_output=xml:test_results.xml
# Linux/macOS
./Release/ZenithEngineTests --gtest_output=xml:test_results.xml

# Generate JSON report
# Windows
.\Release\ZenithEngineTests.exe --gtest_output=json:test_results.json
# Linux/macOS
./Release/ZenithEngineTests --gtest_output=json:test_results.json

# Run tests in random order
# Windows
.\Release\ZenithEngineTests.exe --gtest_shuffle
# Linux/macOS
./Release/ZenithEngineTests --gtest_shuffle

# Set random seed for reproducible results
# Windows
.\Release\ZenithEngineTests.exe --gtest_shuffle --gtest_random_seed=42
# Linux/macOS
./Release/ZenithEngineTests --gtest_shuffle --gtest_random_seed=42
```

## 📊 Available Test Suites

### 1. SimpleTest
Basic test examples to verify Google Test is working correctly.
- `BasicAssertion`: Tests basic assertions (EXPECT_EQ, EXPECT_TRUE, EXPECT_FALSE)
- `MathOperations`: Tests mathematical operations
- `StringOperations`: Tests string manipulation

### 2. ShaderTest
Tests for shader-related functionality.
- `ShaderFileCreation`: Verifies shader file creation
- `ShaderCompilation`: Tests shader compilation logic

### 3. CameraTest
Tests for camera system.
- `CameraInitialization`: Tests camera initialization
- `CameraMatrix`: Tests camera matrix calculations

### 4. MathTest
Tests for mathematical operations using GLM.
- `VectorOperations`: Tests vector operations (addition, subtraction, scaling)
- `MatrixOperations`: Tests matrix operations (identity, translation, rotation)

### 5. FileTest
Tests for file operations.
- `FileExists`: Tests file existence checking
- `FileContent`: Tests file reading and writing

### 6. StringTest
Tests for string operations.
- `StringOperations`: Tests string concatenation, length, and search

### 7. ModelTest
Tests for 3D model functionality.
- `ModelInitialization`: Tests model initialization
- `ModelPathValidation`: Tests file path validation
- `ModelDataStructures`: Tests vertex and index data structures
- `ModelTransformation`: Tests model transformation matrices

### 8. ModelUtilityTest
Tests for model utility functions.
- `VertexDataValidation`: Tests vertex data validation
- `IndexDataValidation`: Tests index data validation

## 📝 Writing New Tests

### Test Structure

```cpp
#include <gtest/gtest.h>

// Simple test
TEST(TestSuiteName, TestName) {
    EXPECT_EQ(expected, actual);
    EXPECT_TRUE(condition);
    EXPECT_FALSE(condition);
}

// Test with setup/teardown
class MyTestClass : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialization before each test
    }
    
    void TearDown() override {
        // Cleanup after each test
    }
};

TEST_F(MyTestClass, TestName) {
    // Test with automatic setup
}
```

### Common Assertions

```cpp
// Equality
EXPECT_EQ(expected, actual);
ASSERT_EQ(expected, actual);  // Stops execution if fails

// Boolean
EXPECT_TRUE(condition);
EXPECT_FALSE(condition);

// Floating point
EXPECT_FLOAT_EQ(expected, actual);
EXPECT_DOUBLE_EQ(expected, actual);
EXPECT_NEAR(value, expected, tolerance);

// Strings
EXPECT_STREQ(expected, actual);
EXPECT_STRNE(expected, actual);

// Containers
EXPECT_THAT(container, ::testing::ElementsAre(1, 2, 3));
```

### Adding New Test Files

1. Create a new `.cpp` file in the `tests/` directory
2. Include the necessary headers and Google Test
3. Write your tests
4. Reconfigure CMake to detect the new file:
   ```bash
   cd build
   cmake ..
   cmake --build . --config Release
   ```

## 🔧 Troubleshooting

### Common Issues

1. **Tests not found after adding new files:**
   - Run `cmake ..` again to detect new files
   - Rebuild the project

2. **Compilation errors:**
   - Check that all necessary headers are included
   - Verify that the project builds successfully before running tests

3. **Test failures:**
   - Check the test output for specific error messages
   - Use `--gtest_verbose` for more detailed output

### Debug Mode

```bash
# Build in debug mode
cmake --build . --config Debug

# Run debug tests
# Windows
.\Debug\ZenithEngineTests.exe
# Linux/macOS
./Debug/ZenithEngineTests
```

## 🔧 IDE Integration

### Visual Studio Code

The project includes VS Code task configurations for easy test execution:

1. **Open the project in VS Code**
2. **Open the Command Palette** (`Ctrl+Shift+P` / `Cmd+Shift+P`)
3. **Select "Tasks: Run Task"**
4. **Choose from available tasks:**
   - `Build Tests`: Build the test executable
   - `Run All Tests`: Run all tests
   - `Run Tests with Filter`: Run tests with a specific filter
   - `Run Tests Verbose`: Run tests with verbose output
   - `Clean and Build`: Clean build and rebuild

**Keyboard Shortcuts:**
- `Ctrl+Shift+P` → "Tasks: Run Task" → "Run All Tests"
- `Ctrl+Shift+P` → "Tasks: Run Task" → "Run Tests with Filter"

### Other IDEs

Most IDEs support CMake projects and can run the test executable directly:
- **Visual Studio**: Open the CMake project and run `ZenithEngineTests.exe`
- **CLion**: Configure the test executable as a run configuration
- **Qt Creator**: Add the test executable as a custom run configuration

## 📈 Continuous Integration

```bash
# Run tests and generate reports
# Windows
.\Release\ZenithEngineTests.exe --gtest_output=xml:test_results.xml
# Linux/macOS
./Release/ZenithEngineTests --gtest_output=xml:test_results.xml

# Check exit code for CI
if [ $? -eq 0 ]; then
    echo "All tests passed!"
else
    echo "Some tests failed!"
    exit 1
fi
```

## 🎯 Best Practices

1. **Test Naming**: Use descriptive names for test suites and tests
2. **One Assertion per Test**: Keep tests focused on one specific behavior
3. **Setup/Teardown**: Use `SetUp()` and `TearDown()` for common initialization
4. **Test Independence**: Tests should not depend on each other
5. **Mocking**: Use mocks for external dependencies when needed
6. **Coverage**: Aim for high test coverage of critical code paths

## 📚 Additional Resources

- [Google Test Documentation](https://google.github.io/googletest/)
- [CMake Testing Guide](https://cmake.org/cmake/help/latest/manual/ctest.1.html)
- [vcpkg Documentation](https://github.com/microsoft/vcpkg)

---

**Happy Testing! 🧪✨**
