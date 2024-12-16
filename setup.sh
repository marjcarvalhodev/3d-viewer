#!/bin/bash
set -e

# Ensure script is run with sudo
if [ "$EUID" -ne 0 ]; then
    echo "Please run this script as root (e.g., using sudo)."
    exit 1
fi

echo "Updating package lists..."
sudo apt update

echo "Installing dependencies..."
sudo apt install -y build-essential clang libc++-dev libc++abi-dev libsdl2-dev \
                    libsdl2-ttf-dev libsdl2-image-dev libsdl2-mixer-dev \
                    libglew-dev libglm-dev mesa-utils cmake git

# Verify installations
echo "Verifying installations..."
{
    echo "Clang version:"
    clang --version
    echo "SDL2 version:"
    sdl2-config --version
    echo "GLEW version:"
    glewinfo | grep -i glew
    echo "OpenGL version:"
    glxinfo | grep -i opengl | grep "version"
} > setup_log.txt 2>&1

echo "Installation completed. Check 'setup_log.txt' for detailed versions and verification."
