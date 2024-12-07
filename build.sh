#!/bin/bash
set -e

# Default build type
BUILD_TYPE="Release"

# Parse arguments
while getopts "d" opt; do
    case $opt in
        d)
            BUILD_TYPE="Debug"
            ;;
        *)
            echo "Usage: $0 [-d]"
            echo "  -d    Build in Debug mode"
            exit 1
            ;;
    esac
done

# Define project directories
BUILD_DIR="build"

# Ensure the build directory exists
mkdir -p $BUILD_DIR

# Navigate to the build directory
cd $BUILD_DIR

# Run CMake to configure the project with the specified build type
echo "Configuring the project with CMake (Build Type: $BUILD_TYPE)..."
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

# Build the project
echo "Building the project..."
make

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo "Build successful. Running the program..."
    # Run the program
    ./3D-Viewer
else
    echo "Build failed."
    exit 1
fi
