#!/bin/bash
# ZenithEngine Test Runner Script (Linux/macOS)
# This script provides easy commands to run unit tests

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"
TEST_EXE="$BUILD_DIR/ZenithEngineTests"

# Default values
FILTER=""
VERBOSE=""
COLOR=""
BUILD=""
CLEAN=""
HELP=""

# Function to show help
show_help() {
    echo "ZenithEngine Test Runner"
    echo "======================="
    echo ""
    echo "Usage:"
    echo "  ./run_tests.sh [options]"
    echo ""
    echo "Options:"
    echo "  -f, --filter PATTERN    Run tests matching pattern (e.g., 'SimpleTest.*')"
    echo "  -v, --verbose          Run tests with verbose output"
    echo "  -c, --color            Run tests with colored output"
    echo "  -b, --build            Build project before running tests"
    echo "  --clean                Clean build directory before building"
    echo "  -h, --help             Show this help message"
    echo ""
    echo "Examples:"
    echo "  ./run_tests.sh                           # Run all tests"
    echo "  ./run_tests.sh -f 'SimpleTest.*'         # Run SimpleTest suite"
    echo "  ./run_tests.sh -v -c                     # Run with verbose and colored output"
    echo "  ./run_tests.sh -b                        # Build and run tests"
    echo "  ./run_tests.sh --clean -b                # Clean build and run tests"
}

# Function to test build directory
test_build_directory() {
    if [ ! -d "$BUILD_DIR" ]; then
        echo "Build directory not found. Creating..."
        mkdir -p "$BUILD_DIR"
        return 0
    fi
    return 1
}

# Function to invoke CMake configure
invoke_cmake_configure() {
    echo "Configuring project with CMake..."
    cd "$BUILD_DIR"
    cmake ..
    if [ $? -ne 0 ]; then
        echo "CMake configuration failed!"
        exit 1
    fi
    cd "$SCRIPT_DIR"
}

# Function to invoke build
invoke_build() {
    echo "Building project..."
    cd "$BUILD_DIR"
    cmake --build . --config Release
    if [ $? -ne 0 ]; then
        echo "Build failed!"
        exit 1
    fi
    cd "$SCRIPT_DIR"
}

# Function to invoke clean
invoke_clean() {
    echo "Cleaning build directory..."
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
    fi
}

# Function to invoke tests
invoke_tests() {
    echo "Running tests..."
    
    ARGS="--gtest_print_time=1"
    
    if [ -n "$FILTER" ]; then
        ARGS="$ARGS --gtest_filter=$FILTER"
    fi
    
    if [ -n "$VERBOSE" ]; then
        ARGS="$ARGS --gtest_verbose"
    fi
    
    if [ -n "$COLOR" ]; then
        ARGS="$ARGS --gtest_color=yes"
    fi
    
    echo "Command: $TEST_EXE $ARGS"
    
    cd "$BUILD_DIR"
    $TEST_EXE $ARGS
    EXIT_CODE=$?
    cd "$SCRIPT_DIR"
    
    if [ $EXIT_CODE -eq 0 ]; then
        echo ""
        echo "✅ All tests passed!"
    else
        echo ""
        echo "❌ Some tests failed!"
    fi
    
    return $EXIT_CODE
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -f|--filter)
            FILTER="$2"
            shift 2
            ;;
        -v|--verbose)
            VERBOSE="--gtest_verbose"
            shift
            ;;
        -c|--color)
            COLOR="--gtest_color=yes"
            shift
            ;;
        -b|--build)
            BUILD="1"
            shift
            ;;
        --clean)
            CLEAN="1"
            shift
            ;;
        -h|--help)
            HELP="1"
            shift
            ;;
        *)
            echo "Unknown option: $1"
            show_help
            exit 1
            ;;
    esac
done

# Main execution
if [ -n "$HELP" ]; then
    show_help
    exit 0
fi

# Check if test executable exists
if [ ! -f "$TEST_EXE" ]; then
    echo "Test executable not found. Building project..."
    BUILD="1"
fi

# Clean if requested
if [ -n "$CLEAN" ]; then
    invoke_clean
fi

# Build if requested or needed
if [ -n "$BUILD" ] || [ ! -f "$TEST_EXE" ]; then
    test_build_directory
    invoke_cmake_configure
    invoke_build
fi

# Run tests
invoke_tests
exit $?
