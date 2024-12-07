#!/bin/bash
set -e

# Update and install dependencies
sudo apt update
sudo apt install -y build-essential clang libc++-dev libc++abi-dev libsdl2-dev libsdl2-ttf-dev \
                    libsdl2-image-dev libsdl2-mixer-dev libglew-dev libglm-dev mesa-utils cmake git

# Verify installations
echo "Installation completed. Verifying versions..."
clang --version
sdl2-config --version
glewinfo | grep -i glew
glxinfo | grep -i opengl

echo "All dependencies installed successfully with LLVM."