#pragma once
#include "Vector2D.h"
#include <vector>
#include <string>

namespace Physica {

enum class ShapeType {
    Circle,
    Box
};

enum class IntegrationMethod {
    Euler,
    SemiImplicitEuler,
    Verlet
};

class PhysicsObject {
public:
    // Basic properties
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    Vector2D previousPosition; // For Verlet integration
    
    float mass;
    float radius; // For circles
    float width, height; // For boxes
    
    // Physics properties
    float restitution; // Coefficient of restitution (bounciness)
    float friction;
    bool isStatic;
    
    ShapeType shape;
    
    // Visual properties
    float colorR, colorG, colorB;
    std::string label;
    
    // Force accumulator
    Vector2D forceAccumulator;
    
    PhysicsObject(Vector2D pos, float mass, ShapeType shape = ShapeType::Circle)
        : position(pos), velocity(0, 0), acceleration(0, 0), previousPosition(pos),
          mass(mass), radius(20.0f), width(40.0f), height(40.0f),
          restitution(0.8f), friction(0.1f), isStatic(false),
          shape(shape), colorR(0.3f), colorG(0.7f), colorB(1.0f),
          forceAccumulator(0, 0) {}
    
    // Add force to object
    void addForce(const Vector2D& force) {
        forceAccumulator += force;
    }
    
    // Clear forces (call after each integration step)
    void clearForces() {
        forceAccumulator = Vector2D(0, 0);
    }
    
    // Get kinetic energy
    float getKineticEnergy() const {
        if (isStatic) return 0.0f;
        return 0.5f * mass * velocity.magnitudeSquared();
    }
    
    // Get potential energy (relative to y-position)
    float getPotentialEnergy(float gravity) const {
        if (isStatic) return 0.0f;
        return mass * gravity * position.y;
    }
    
    // Get inverse mass (useful for collision response)
    float getInverseMass() const {
        return isStatic ? 0.0f : 1.0f / mass;
    }
};

} // namespace Physica
