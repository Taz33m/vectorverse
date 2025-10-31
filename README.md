# 🧩 Vectorverse - An Educational Physics Sandbox

An interactive, visual C++ application designed to help students learn fundamental physics concepts through real-time simulation and experimentation. Can also be used to learn intermediate C++.

## 🎯 Overview

VectorVerse is an educational physics simulation engine that allows users to:
- Observe and manipulate physical systems interactively
- Visualize forces, velocities, and energy in real-time
- Compare different numerical integration methods
- Experiment with Newton's laws, collisions, and energy conservation

## ✨ Features

### Core Simulation
- **2D Physics Engine**: Real-time simulation with adjustable speed
- **Multiple Bodies**: Support for circles with mass, velocity, and forces
- **Force Models**: 
  - Gravity (toggleable)
  - Friction and air resistance
  - Custom user-defined forces

### Physics Capabilities
- **Newtonian Motion**: F = ma
- **Integration Methods**: Euler, Semi-Implicit Euler, Verlet
- **Collision Response**: Elastic and inelastic collisions
- **Energy Tracking**: Real-time kinetic, potential, and total energy graphs

### Visualization
- Real-time display of force and velocity vectors
- Object trajectories and trails
- Energy graphs over time
- Adjustable visual settings

### Educational Modules
1. **Sandbox Mode**: Free experimentation
2. **Projectile Motion**: Parabolic trajectory visualization
3. **Elastic Collisions**: Momentum conservation demonstration
4. **Harmonic Motion**: Oscillation and energy exchange
5. **Inclined Plane**: Friction and normal forces

## 🎮 Controls

### Keyboard
- **SPACE**: Pause/Resume simulation
- **S**: Step through simulation (when paused)
- **R**: Reset current module
- **C**: Clear all objects
- **G**: Toggle gravity
- **V**: Toggle velocity vectors
- **1-3**: Load different educational modules
  - **1**: Sandbox
  - **2**: Projectile Motion
  - **3**: Elastic Collisions

### Mouse
- **Left Click**: Select and drag objects (or create new objects)
- **Drag**: Move objects and impart velocity

### Prerequisites to Build

#### macOS
```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install sfml cmake
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install cmake g++ libsfml-dev
```

#### Windows
1. Install [CMake](https://cmake.org/download/)
2. Install [SFML](https://www.sfml-dev.org/download.php)
3. Set up environment variables for SFML

### Build Instructions

```bash
# Navigate to project directory
cd vectorverse

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build .

# Run
./bin/vectorverse
```

## License

This project is educational software. Feel free to use, modify, and distribute for educational purposes.

## Acknowledgments

- **SFML**: Graphics and window management
- Built for undergraduate physics and computational physics courses
- Inspired by classical physics education needs

---

**Made with ❤️ for physics students and educators**
