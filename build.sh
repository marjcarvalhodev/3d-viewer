#!/bin/bash
set -e

# Default build type
BUILD_TYPE="Debug"
REPLACE_ASSETS=false

# Parse arguments
while getopts "ra" opt; do
    case $opt in
        r)
            BUILD_TYPE="Release"
            ;;
        a)
            REPLACE_ASSETS=true
            ;;
        *)
            echo "Usage: $0 [-r] [-a]"
            echo "  -r    Build in Release mode"
            echo "  -a    Replace assets in the build directory"
            exit 1
            ;;
    esac
done

# Define project directories
BUILD_DIR="build"
ASSETS_DIR="assets"

# Ensure the build directory exists
mkdir -p $BUILD_DIR

# Optionally replace assets
if [ "$REPLACE_ASSETS" = true ]; then
    echo "Replacing assets in the build directory..."
    rm -rf "$BUILD_DIR/$ASSETS_DIR"
    cp -r "$ASSETS_DIR" "$BUILD_DIR"
fi

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
