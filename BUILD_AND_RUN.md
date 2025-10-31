# Build and Run Instructions

Quick guide to build and run vectorverse on your system.

## ⚡ Quick Start

### Option 1: Automated Setup (Recommended)
```bash
cd insert directory
./setup.sh
```

The script will:
1. Check for dependencies
2. Offer to install missing ones
3. Build the project
4. Provide run instructions

---

### Option 2: Manual Build

#### Step 1: Install Dependencies

**macOS:**
```bash
brew install sfml cmake
```

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install cmake g++ libsfml-dev
```

**Fedora:**
```bash
sudo dnf install cmake gcc-c++ SFML-devel
```

**Arch Linux:**
```bash
sudo pacman -S cmake sfml
```

#### Step 2: Build
```bash
cd /Users/tazeemmahashin/CascadeProjects/Physica
mkdir build
cd build
cmake ..
cmake --build .
```

#### Step 3: Run
```bash
./bin/Physica
```

---

## 🎮 First Launch

When you run Physica, you'll see:
1. **Window**: 1280x720 dark window
2. **Objects**: A few demo circles falling
3. **Energy Graph**: Top-right corner
4. **Controls**: Keyboard and mouse ready

### Try This First:
1. **Click** anywhere to create an object
2. **Drag** an object to move it
3. Press **SPACE** to pause
4. Press **G** to toggle gravity
5. Press **V** to see velocity vectors

---

## 🔧 Troubleshooting

### Build Fails: "SFML not found"

**macOS:**
```bash
brew reinstall sfml
```

**Linux:**
```bash
sudo apt install --reinstall libsfml-dev
```

### Build Fails: "CMake version too old"

**Upgrade CMake:**
```bash
# macOS
brew upgrade cmake

# Ubuntu (via snap)
sudo snap install cmake --classic
```

### Runtime: Font not found (warnings)

This is non-critical. Labels won't show but simulation works.

**Fix (Ubuntu):**
```bash
sudo apt install fonts-dejavu
```

### Runtime: Window doesn't appear

1. Check graphics drivers are updated
2. Try running from terminal to see errors
3. Verify SFML installation:
   ```bash
   # macOS
   brew list sfml
   
   # Ubuntu
   dpkg -l | grep sfml
   ```

---

## 🚀 Advanced Build Options

### Release Build (Faster)
```bash
mkdir build-release
cd build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

### Debug Build (For Development)
```bash
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

### Parallel Build (Faster Compilation)
```bash
# macOS
cmake --build . -j$(sysctl -n hw.ncpu)

# Linux
cmake --build . -j$(nproc)
```

---

## 📁 Build Outputs

After building, you'll have:
```
build/
├── bin/
│   └── Physica          # Executable
├── CMakeFiles/          # Build artifacts
└── Makefile            # Build rules
```

---

## 🎯 Verification

### Test 1: Launch
```bash
cd build
./bin/Physica
```
**Expected**: Window appears with falling objects

### Test 2: Create Object
- Click anywhere
**Expected**: New circle appears

### Test 3: Pause
- Press SPACE
**Expected**: Objects freeze

### Test 4: Gravity Toggle
- Press G
**Expected**: Objects stop falling (or start if off)

### Test 5: Module Load
- Press 2
**Expected**: Projectile motion demo loads

If all tests pass: ✅ Installation successful!

---

## 🖥️ System Requirements

### Minimum
- **OS**: macOS 10.12+, Ubuntu 18.04+, Windows 10+
- **CPU**: Dual-core 1.5 GHz
- **RAM**: 512 MB
- **Graphics**: OpenGL 2.0 compatible

### Recommended
- **OS**: macOS 12+, Ubuntu 22.04+, Windows 11
- **CPU**: Quad-core 2.5 GHz
- **RAM**: 2 GB
- **Graphics**: Dedicated GPU

---


## 🔄 Rebuild After Changes

If you modify source files:

```bash
cd build
cmake --build .
```

If you modify CMakeLists.txt:

```bash
cd build
cmake ..
cmake --build .
```

Clean rebuild:

```bash
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```

---