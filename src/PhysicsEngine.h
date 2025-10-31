#pragma once
#include "PhysicsObject.h"
#include <vector>
#include <memory>

namespace Physica {

class PhysicsEngine {
public:
    PhysicsEngine();
    
    // Simulation control
    void update(float dt);
    void reset();
    
    // Object management
    void addObject(std::shared_ptr<PhysicsObject> object);
    void removeObject(size_t index);
    void clearObjects();
    std::vector<std::shared_ptr<PhysicsObject>>& getObjects() { return objects; }
    
    // Physics parameters
    void setGravity(const Vector2D& g) { gravity = g; }
    Vector2D getGravity() const { return gravity; }
    void setIntegrationMethod(IntegrationMethod method) { integrationMethod = method; }
    IntegrationMethod getIntegrationMethod() const { return integrationMethod; }
    
    // Force application
    void applyGravity();
    void applyFriction();
    void applyAirResistance(float coefficient);
    
    // Collision detection and response
    void handleCollisions();
    void handleBoundaryCollisions(float width, float height);
    
    // Energy tracking
    float getTotalKineticEnergy() const;
    float getTotalPotentialEnergy() const;
    float getTotalEnergy() const;
    
    // Settings
    bool gravityEnabled = true;
    bool collisionsEnabled = true;
    bool boundaryEnabled = true;
    float airResistanceCoefficient = 0.01f;
    
private:
    std::vector<std::shared_ptr<PhysicsObject>> objects;
    Vector2D gravity;
    IntegrationMethod integrationMethod;
    
    // Integration methods
    void integrateEuler(PhysicsObject& obj, float dt);
    void integrateSemiImplicitEuler(PhysicsObject& obj, float dt);
    void integrateVerlet(PhysicsObject& obj, float dt);
    
    // Collision helpers
    bool checkCircleCircleCollision(const PhysicsObject& a, const PhysicsObject& b);
    void resolveCircleCircleCollision(PhysicsObject& a, PhysicsObject& b);
};

} // namespace Physica
