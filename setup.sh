#!/bin/bash

# Physica Setup Script
# Automates installation and building for macOS and Linux

set -e  # Exit on error

echo "╔═══════════════════════════════════════╗"
echo "║   Physica - Physics Simulation Setup ║"
echo "╚═══════════════════════════════════════╝"
echo ""

# Detect OS
OS="$(uname -s)"
echo "Detected OS: $OS"
echo ""

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Install dependencies based on OS
install_dependencies() {
    echo "Installing dependencies..."
    
    case "$OS" in
        Darwin*)
            # macOS
            if ! command_exists brew; then
                echo "Homebrew not found. Installing Homebrew..."
                /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
            fi
            
            echo "Installing SFML and CMake via Homebrew..."
            brew install sfml cmake
            ;;
            
        Linux*)
            # Linux
            if command_exists apt-get; then
                # Debian/Ubuntu
                echo "Installing dependencies via apt..."
                sudo apt-get update
                sudo apt-get install -y cmake g++ libsfml-dev fonts-dejavu
            elif command_exists dnf; then
                # Fedora/RHEL
                echo "Installing dependencies via dnf..."
                sudo dnf install -y cmake gcc-c++ SFML-devel
            elif command_exists pacman; then
                # Arch Linux
                echo "Installing dependencies via pacman..."
                sudo pacman -S --noconfirm cmake sfml
            else
                echo "Error: Unsupported Linux distribution"
                echo "Please install cmake, g++, and SFML manually"
                exit 1
            fi
            ;;
            
        *)
            echo "Error: Unsupported operating system"
            exit 1
            ;;
    esac
    
    echo "✓ Dependencies installed successfully"
    echo ""
}

# Build project
build_project() {
    echo "Building Physica..."
    
    # Create build directory
    if [ -d "build" ]; then
        echo "Removing old build directory..."
        rm -rf build
    fi
    
    mkdir build
    cd build
    
    # Configure
    echo "Configuring with CMake..."
    cmake ..
    
    # Build
    echo "Compiling..."
    if [ "$OS" = "Darwin" ]; then
        cmake --build . -j$(sysctl -n hw.ncpu)
    else
        cmake --build . -j$(nproc)
    fi
    
    cd ..
    
    echo "✓ Build completed successfully"
    echo ""
}

# Main installation flow
main() {
    # Check for dependencies
    echo "Checking for required tools..."
    
    NEED_INSTALL=false
    
    if ! command_exists cmake; then
        echo "✗ CMake not found"
        NEED_INSTALL=true
    else
        echo "✓ CMake found"
    fi
    
    if ! command_exists g++ && ! command_exists clang++; then
        echo "✗ C++ compiler not found"
        NEED_INSTALL=true
    else
        echo "✓ C++ compiler found"
    fi
    
    echo ""
    
    # Install if needed
    if [ "$NEED_INSTALL" = true ]; then
        read -p "Install missing dependencies? (y/n) " -n 1 -r
        echo ""
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            install_dependencies
        else
            echo "Skipping dependency installation."
            echo "Note: Build may fail without required dependencies."
            echo ""
        fi
    else
        echo "All dependencies found!"
        echo ""
    fi
    
    # Build
    read -p "Build Physica now? (y/n) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        build_project
        
        echo "╔═══════════════════════════════════════╗"
        echo "║         Setup Complete! 🎉            ║"
        echo "╚═══════════════════════════════════════╝"
        echo ""
        echo "To run Physica:"
        echo "  cd build"
        echo "  ./bin/Physica"
        echo ""
        echo "Controls:"
        echo "  SPACE   - Pause/Resume"
        echo "  S       - Step simulation"
        echo "  R       - Reset"
        echo "  C       - Clear objects"
        echo "  G       - Toggle gravity"
        echo "  V       - Toggle velocity vectors"
        echo "  1-3     - Load modules"
        echo "  Mouse   - Click to create/drag objects"
        echo ""
    else
        echo "Build skipped. Run './setup.sh' again to build."
    fi
}

# Run main function
main
