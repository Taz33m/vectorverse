# Physica Quick Reference Card

A one-page reference for Physica controls, features, and formulas.

## 🎮 Keyboard Controls

| Key | Action |
|-----|--------|
| `SPACE` | Pause/Resume simulation |
| `S` | Step one frame (when paused) |
| `R` | Reset current module |
| `C` | Clear all objects |
| `G` | Toggle gravity on/off |
| `V` | Toggle velocity vectors |
| `1` | Load Sandbox module |
| `2` | Load Projectile Motion module |
| `3` | Load Elastic Collisions module |

## 🖱️ Mouse Controls

| Action | Effect |
|--------|--------|
| **Click** empty space | Create new object |
| **Click** object | Select object |
| **Drag** object | Move object and impart velocity |
| **Release** while dragging | Apply velocity based on drag speed |

## 📊 Visual Elements

| Element | Color | Meaning |
|---------|-------|---------|
| **Circle** | Random | Physics object |
| **Green Arrow** | Green | Velocity vector |
| **Red Arrow** | Red | Force vector |
| **Graph Line** | White | Total energy |
| **Graph Line** | Green | Kinetic energy |
| **Graph Line** | Red | Potential energy |
| **Dimmed Circle** | Gray | Static object (unmovable) |

## 📐 Physics Formulas

### Newton's Laws
```
F = ma        (Force = mass × acceleration)
```

### Energy
```
KE = ½mv²     (Kinetic energy)
PE = mgh      (Potential energy)
E = KE + PE   (Total energy, conserved)
```

### Momentum
```
p = mv        (Momentum = mass × velocity)
Σp_before = Σp_after  (Conservation of momentum)
```

### Collision (Elastic)
```
e = 1.0       (Coefficient of restitution)
Both KE and p conserved
```

### Collision (Inelastic)
```
e < 1.0       (Some energy lost)
Only p conserved
```

### Projectile Motion
```
x(t) = x₀ + v₀ₓt
y(t) = y₀ + v₀ᵧt - ½gt²
Range = v₀²sin(2θ)/g
Max height = v₀²sin²(θ)/(2g)
```

## 🔧 Default Values

| Property | Default Value |
|----------|---------------|
| **Mass** | 10 kg |
| **Radius** | 20 pixels |
| **Restitution** | 0.8 (80% bouncy) |
| **Friction** | 0.1 |
| **Gravity** | 980 pixels/s² (≈9.8 m/s²) |
| **Timestep** | 1/60 second |
| **Frame Rate** | 60 FPS |

## 🎯 Quick Start Scenarios

### Free Fall
1. Press `C`
2. Click at top of screen
3. Watch object fall

### Collision Test
1. Press `3`
2. Watch preset collision
3. Press `R` to repeat

### Custom Launch
1. Press `C`
2. Click at bottom-left
3. Drag up and right
4. Release to launch

### Zero Gravity
1. Press `G` to disable gravity
2. Create objects
3. Give them velocities
4. Watch inertial motion

## 📈 Energy Graph Reading

### Flat White Line
- ✓ Energy conserved
- System is ideal
- No losses

### Decreasing White Line
- Energy being lost
- Inelastic collisions
- Friction/air resistance

### Oscillating Lines
- Energy converting
- KE ↔ PE exchange
- Pendulum-like motion

### Vertical Drops
- Sudden energy loss
- Collision just occurred
- Inelastic impact

## 🎓 Common Experiments

### Test Conservation of Momentum
1. Press `3` for collisions
2. Note velocities before collision
3. Calculate: p = m₁v₁ + m₂v₂
4. Note velocities after
5. Calculate: p' = m₁v₁' + m₂v₂'
6. Verify: p = p'

### Test Energy Loss
1. Create object at height h
2. Drop it (PE = mgh)
3. Watch it bounce
4. Measure new height h'
5. Calculate energy lost: mg(h - h')
6. Calculate restitution: e = √(h'/h)

### Test Projectile Range
1. Press `2`
2. Observe landing position
3. Measure horizontal distance
4. Calculate: R = v₀²sin(2θ)/g
5. Verify with measurement

## 🐛 Troubleshooting

| Problem | Solution |
|---------|----------|
| Objects not moving | Press `SPACE` to unpause |
| No gravity | Press `G` to enable |
| Can't see vectors | Press `V` to toggle |
| Simulation too fast | Currently fixed at 1× (future feature) |
| Objects stuck together | They're colliding; separate them |
| Energy not conserved | Check restitution < 1.0 |

## 💡 Pro Tips

1. **Pause Before Creating**: Pause (`SPACE`) for precise object placement
2. **Step Through Collisions**: Use `S` to see collision frame-by-frame
3. **Watch Energy Graph**: Best indicator of physics accuracy
4. **Start Simple**: Begin with one or two objects
5. **Reset Often**: Press `R` to replay scenarios
6. **Experiment**: Try extreme values to understand limits

## 📚 Modules Explained

### Sandbox (1)
- Free experimentation
- Create any scenario
- All features available
- Best for learning controls

### Projectile Motion (2)
- Preset parabolic launch
- Demonstrates 2D motion
- Shows energy exchange
- Educational focus

### Elastic Collisions (3)
- Two objects colliding
- Perfect momentum transfer
- Energy conservation demo
- Reset and replay anytime

## 🔬 Physics Accuracy

### What's Accurate
- ✓ Newton's laws (F=ma)
- ✓ Energy conservation (ideal)
- ✓ Momentum conservation
- ✓ Projectile trajectories
- ✓ Elastic collisions

### Simplifications
- ⚠ 2D only (no z-axis)
- ⚠ Point masses (circles)
- ⚠ No rotation dynamics
- ⚠ Discrete timesteps
- ⚠ Simple air resistance

### Known Limitations
- High velocities cause tunneling
- Energy drift in long simulations
- No advanced constraints yet
- Box collisions not yet implemented

## 🎯 Learning Objectives

### Beginner
- Understand force and motion
- Observe energy conservation
- See collision dynamics
- Explore cause and effect

### Intermediate
- Calculate momentum
- Verify conservation laws
- Understand integration methods
- Analyze energy graphs

### Advanced
- Study numerical methods
- Explore stability
- Design experiments
- Investigate edge cases

## 📝 Quick Reference: Units

| Quantity | Unit | Screen Value |
|----------|------|--------------|
| **Position** | pixels | 0-1280 (x), 0-720 (y) |
| **Velocity** | pixels/s | Typical: 0-500 |
| **Acceleration** | pixels/s² | Gravity: 980 |
| **Mass** | arbitrary | Default: 10 |
| **Force** | mass·pixels/s² | Varies |
| **Energy** | mass·pixels²/s² | Varies |
| **Time** | seconds | Real-time |

## 🚀 Getting Started (30 seconds)

1. **Launch**: Run `./bin/Physica`
2. **Create**: Click anywhere
3. **Drag**: Click and drag object
4. **Observe**: Watch physics in action
5. **Experiment**: Try different scenarios
6. **Learn**: Read energy graph

## 📖 For More Information

- **User Guide**: See `USER_GUIDE.md`
- **Examples**: See `EXAMPLES.md`
- **Developer Guide**: See `DEVELOPER_GUIDE.md`
- **Installation**: See `INSTALL.md`

---

**Print this page for quick reference while using Physica!**
