# Installation Guide for vectorverse

This guide provides detailed installation instructions for different platforms.

## Table of Contents
1. [macOS Installation](#macos-installation)
2. [Linux Installation](#linux-installation)
3. [Windows Installation](#windows-installation)
4. [Troubleshooting](#troubleshooting)

---

## macOS Installation

### Step 1: Install Homebrew (if not installed)
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### Step 2: Install Dependencies
```bash
brew install sfml cmake
```

### Step 3: Clone or Navigate to Project
```bash
cd /path
```

### Step 4: Build
```bash
mkdir build && cd build
cmake ..
cmake --build . -j$(sysctl -n hw.ncpu)
```

### Step 5: Run
```bash
./bin/__
```

---

## Linux Installation

### Ubuntu/Debian

#### Step 1: Update Package Manager
```bash
sudo apt update
```

#### Step 2: Install Dependencies
```bash
sudo apt install cmake g++ libsfml-dev
```

#### Step 3: Build
```bash
cd /path/to/vectorverse
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)
```

#### Step 4: Run
```bash
./bin/vectorverse
```

### Fedora/RHEL

#### Step 1: Install Dependencies
```bash
sudo dnf install cmake gcc-c++ SFML-devel
```

#### Step 2: Build and Run
Follow the same build steps as Ubuntu.

### Arch Linux

#### Step 1: Install Dependencies
```bash
sudo pacman -S cmake sfml
```

#### Step 2: Build and Run
Follow the same build steps as Ubuntu.

---

## Windows Installation

### Using Visual Studio

#### Step 1: Install Visual Studio
- Download and install [Visual Studio 2019 or later](https://visualstudio.microsoft.com/)
- Include "Desktop development with C++" workload

#### Step 2: Install CMake
- Download and install [CMake](https://cmake.org/download/)
- Add CMake to PATH during installation

#### Step 3: Install SFML
Option A: Using vcpkg (Recommended)
```cmd
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
.\vcpkg install sfml:x64-windows
```

Option B: Manual Installation
1. Download SFML from [sfml-dev.org](https://www.sfml-dev.org/download.php)
2. Extract to `C:\SFML`
3. Set environment variable: `SFML_DIR=C:\SFML`

#### Step 4: Build
```cmd
cd path\to\vectorverse
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release
```

#### Step 5: Run
```cmd
.\bin\Release\vectorverse.exe
```

### Using MinGW

#### Step 1: Install MSYS2
- Download from [msys2.org](https://www.msys2.org/)
- Follow installation instructions

#### Step 2: Install Dependencies
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-sfml
```

#### Step 3: Build
```bash
cd /path/to/vectorverse
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

---

## Troubleshooting

### SFML Not Found

**macOS/Linux:**
```bash
# Verify SFML installation
brew list sfml  # macOS
dpkg -l | grep sfml  # Ubuntu/Debian

# If not found, reinstall
brew reinstall sfml  # macOS
sudo apt install --reinstall libsfml-dev  # Ubuntu
```

**Windows:**
- Ensure SFML_DIR environment variable is set
- Verify vcpkg installation: `vcpkg list`

### Font Loading Issues

The application tries to load system fonts. If you encounter font errors:

**macOS:**
- Default font path: `/System/Library/Fonts/Helvetica.ttc`

**Linux:**
- Default font path: `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf`
- Install fonts: `sudo apt install fonts-dejavu`

**Windows:**
- Font loading may fail silently (labels won't appear but app runs)

### CMake Version Too Old

```bash
# macOS
brew upgrade cmake

# Ubuntu (if version < 3.15)
sudo apt remove cmake
sudo snap install cmake --classic

# Or build from source
wget https://github.com/Kitware/CMake/releases/download/v3.27.0/cmake-3.27.0.tar.gz
tar -xzf cmake-3.27.0.tar.gz
cd cmake-3.27.0
./bootstrap && make && sudo make install
```

### Compilation Errors

**C++17 not supported:**
- Ensure compiler version:
  - GCC >= 7
  - Clang >= 5
  - MSVC >= 2017

**Missing OpenGL:**
- macOS: Should be included
- Linux: `sudo apt install libgl1-mesa-dev`
- Windows: Included with GPU drivers

### Performance Issues

If simulation runs slowly:
1. Reduce number of objects
2. Check if VSync is causing issues
3. Ensure you're running Release build:
   ```bash
   cmake --build . --config Release
   ```

### Runtime Errors

**Segmentation fault / Access violation:**
- Check SFML library compatibility
- Ensure all shared libraries are in PATH/LD_LIBRARY_PATH

**Window doesn't appear:**
- Verify graphics drivers are up to date
- Check display settings (resolution, scaling)

---

## Verification

After installation, you should be able to:
1. Launch the application
2. See a window with physics objects
3. Click to create/drag objects
4. Press keys to control simulation

If any step fails, refer to the troubleshooting section or open an issue.

---

## Quick Start Commands

**macOS:**
```bash
brew install sfml cmake && cd vectorverse && mkdir build && cd build && cmake .. && cmake --build . && ./bin/vectorverse
```

**Ubuntu/Debian:**
```bash
sudo apt install cmake g++ libsfml-dev && cd vectorverse && mkdir build && cd build && cmake .. && cmake --build . && ./bin/vectorverse
```

---

For additional help, please check the README.md or submit an issue.
