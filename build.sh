#!/bin/bash
set -e

# Default build type
PROJECT_NAME="3D-Viewer"
BUILD_TYPE="Debug"
BUILD_WASM=false
REPLACE_ASSETS=false
SERVE=false

# Parse arguments
while getopts "raws" opt; do
    case $opt in
        r)
            BUILD_TYPE="Release"
            ;;
        a)
            REPLACE_ASSETS=true
            ;;
        w)
            BUILD_WASM=true
            ;;
        s)
            SERVE=true
            ;;
        *)
            echo "Usage: $0 [-r] [-a] [-w] [-s]"
            echo "  -r    Build in Release mode"
            echo "  -a    Replace assets in the build directory"
            echo "  -w    Build for WebAssembly"
            echo "  -s    Serve the build and open it in a browser"
            exit 1
            ;;
    esac
done

# Define project directories
BUILD_DIR="build"
BUILD_WASM_DIR="build-wasm"
ASSETS_DIR="assets"
DOCS_DIR="docs"

# Clean and recreate the build directory
if [ "$BUILD_WASM" = true ]; then
    echo "Cleaning WebAssembly build directory..."
    rm -rf "$BUILD_WASM_DIR"
    mkdir -p "$BUILD_WASM_DIR"
else
    echo "Cleaning native build directory..."
    rm -rf "$BUILD_DIR"
    mkdir -p "$BUILD_DIR"
fi

# Optionally replace assets
if [ "$REPLACE_ASSETS" = true ]; then
    echo "Replacing assets in the build directory..."
    rm -rf "$BUILD_DIR/$ASSETS_DIR" "$BUILD_WASM_DIR/$ASSETS_DIR"
    cp -r "$ASSETS_DIR" "$BUILD_DIR" 2>/dev/null || true
    cp -r "$ASSETS_DIR" "$BUILD_WASM_DIR" 2>/dev/null || true
fi

# Build for WebAssembly
if [ "$BUILD_WASM" = true ]; then
    echo "Building for WebAssembly..."
    cd "$BUILD_WASM_DIR"
    emcmake cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..
    emmake make

    # echo "Copying WebAssembly build artifacts to docs directory..."
    # rm -rf "../$DOCS_DIR"
    # mkdir -p "../$DOCS_DIR"
    # cp *.data *.html *.js *.wasm "../$DOCS_DIR/"

    # echo "Copying assets directory to docs..."
    # cp -r ../assets "../$DOCS_DIR/"

    # echo "WebAssembly build successful. Artifacts are in the '$DOCS_DIR' directory."
else
    # Build for native
    echo "Configuring the project with CMake (Build Type: $BUILD_TYPE)..."
    cd "$BUILD_DIR"
    cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..
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
fi

# Serve and open in browser
if [ "$SERVE" = true ]; then
    echo "Starting HTTP server and opening browser..."
    cd "../$DOCS_DIR"
    python3 -m http.server 8000 & # Start server in the background
    SERVER_PID=$!
    sleep 2 # Give the server time to start
    echo "Press Ctrl+C to stop the server."
    wait $SERVER_PID # Wait for the server process
fi