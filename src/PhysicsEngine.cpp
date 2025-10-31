#include "PhysicsEngine.h"
#include <cmath>
#include <algorithm>

namespace Physica {

PhysicsEngine::PhysicsEngine()
    : gravity(0, 980.0f), // 980 pixels/s^2 (simulating 9.8 m/s^2)
      integrationMethod(IntegrationMethod::SemiImplicitEuler) {
}

void PhysicsEngine::update(float dt) {
    // Apply forces
    if (gravityEnabled) {
        applyGravity();
    }
    
    applyFriction();
    applyAirResistance(airResistanceCoefficient);
    
    // Integrate physics
    for (auto& obj : objects) {
        if (!obj->isStatic) {
            // Calculate acceleration from forces
            obj->acceleration = obj->forceAccumulator * obj->getInverseMass();
            
            // Integrate based on selected method
            switch (integrationMethod) {
                case IntegrationMethod::Euler:
                    integrateEuler(*obj, dt);
                    break;
                case IntegrationMethod::SemiImplicitEuler:
                    integrateSemiImplicitEuler(*obj, dt);
                    break;
                case IntegrationMethod::Verlet:
                    integrateVerlet(*obj, dt);
                    break;
            }
            
            obj->clearForces();
        }
    }
    
    // Handle collisions
    if (collisionsEnabled) {
        handleCollisions();
    }
}

void PhysicsEngine::reset() {
    objects.clear();
}

void PhysicsEngine::addObject(std::shared_ptr<PhysicsObject> object) {
    objects.push_back(object);
}

void PhysicsEngine::removeObject(size_t index) {
    if (index < objects.size()) {
        objects.erase(objects.begin() + index);
    }
}

void PhysicsEngine::clearObjects() {
    objects.clear();
}

void PhysicsEngine::applyGravity() {
    for (auto& obj : objects) {
        if (!obj->isStatic) {
            obj->addForce(gravity * obj->mass);
        }
    }
}

void PhysicsEngine::applyFriction() {
    for (auto& obj : objects) {
        if (!obj->isStatic && obj->friction > 0.0f) {
            Vector2D frictionForce = obj->velocity * (-obj->friction);
            obj->addForce(frictionForce);
        }
    }
}

void PhysicsEngine::applyAirResistance(float coefficient) {
    for (auto& obj : objects) {
        if (!obj->isStatic && coefficient > 0.0f) {
            float speedSquared = obj->velocity.magnitudeSquared();
            if (speedSquared > 0.0001f) {
                Vector2D dragDirection = obj->velocity.normalized() * -1.0f;
                Vector2D dragForce = dragDirection * (coefficient * speedSquared);
                obj->addForce(dragForce);
            }
        }
    }
}

void PhysicsEngine::integrateEuler(PhysicsObject& obj, float dt) {
    obj.velocity += obj.acceleration * dt;
    obj.position += obj.velocity * dt;
}

void PhysicsEngine::integrateSemiImplicitEuler(PhysicsObject& obj, float dt) {
    obj.velocity += obj.acceleration * dt;
    obj.position += obj.velocity * dt;
}

void PhysicsEngine::integrateVerlet(PhysicsObject& obj, float dt) {
    Vector2D newPosition = obj.position * 2.0f - obj.previousPosition + obj.acceleration * (dt * dt);
    obj.previousPosition = obj.position;
    obj.velocity = (newPosition - obj.position) / dt;
    obj.position = newPosition;
}

void PhysicsEngine::handleCollisions() {
    // Check all pairs of objects
    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            if (objects[i]->shape == ShapeType::Circle && 
                objects[j]->shape == ShapeType::Circle) {
                if (checkCircleCircleCollision(*objects[i], *objects[j])) {
                    resolveCircleCircleCollision(*objects[i], *objects[j]);
                }
            }
        }
    }
}

void PhysicsEngine::handleBoundaryCollisions(float width, float height) {
    if (!boundaryEnabled) return;
    
    for (auto& obj : objects) {
        if (obj->isStatic) continue;
        
        if (obj->shape == ShapeType::Circle) {
            // Left boundary
            if (obj->position.x - obj->radius < 0) {
                obj->position.x = obj->radius;
                obj->velocity.x *= -obj->restitution;
            }
            // Right boundary
            if (obj->position.x + obj->radius > width) {
                obj->position.x = width - obj->radius;
                obj->velocity.x *= -obj->restitution;
            }
            // Top boundary
            if (obj->position.y - obj->radius < 0) {
                obj->position.y = obj->radius;
                obj->velocity.y *= -obj->restitution;
            }
            // Bottom boundary
            if (obj->position.y + obj->radius > height) {
                obj->position.y = height - obj->radius;
                obj->velocity.y *= -obj->restitution;
                
                // Apply resting friction
                if (std::abs(obj->velocity.y) < 10.0f) {
                    obj->velocity.x *= 0.95f;
                }
            }
        }
    }
}

bool PhysicsEngine::checkCircleCircleCollision(const PhysicsObject& a, const PhysicsObject& b) {
    float distance = Vector2D::distance(a.position, b.position);
    return distance < (a.radius + b.radius);
}

void PhysicsEngine::resolveCircleCircleCollision(PhysicsObject& a, PhysicsObject& b) {
    Vector2D normal = (b.position - a.position).normalized();
    
    // Separate objects
    float overlap = (a.radius + b.radius) - Vector2D::distance(a.position, b.position);
    if (overlap > 0) {
        float totalInvMass = a.getInverseMass() + b.getInverseMass();
        if (totalInvMass > 0.0001f) {
            Vector2D separation = normal * (overlap / totalInvMass);
            if (!a.isStatic) a.position -= separation * a.getInverseMass();
            if (!b.isStatic) b.position += separation * b.getInverseMass();
        }
    }
    
    // Calculate relative velocity
    Vector2D relativeVelocity = b.velocity - a.velocity;
    float velocityAlongNormal = relativeVelocity.dot(normal);
    
    // Don't resolve if objects are separating
    if (velocityAlongNormal > 0) return;
    
    // Calculate restitution
    float e = std::min(a.restitution, b.restitution);
    
    // Calculate impulse scalar
    float j = -(1 + e) * velocityAlongNormal;
    j /= a.getInverseMass() + b.getInverseMass();
    
    // Apply impulse
    Vector2D impulse = normal * j;
    if (!a.isStatic) a.velocity -= impulse * a.getInverseMass();
    if (!b.isStatic) b.velocity += impulse * b.getInverseMass();
}

float PhysicsEngine::getTotalKineticEnergy() const {
    float total = 0.0f;
    for (const auto& obj : objects) {
        total += obj->getKineticEnergy();
    }
    return total;
}

float PhysicsEngine::getTotalPotentialEnergy() const {
    float total = 0.0f;
    float g = gravity.magnitude();
    for (const auto& obj : objects) {
        total += obj->getPotentialEnergy(g);
    }
    return total;
}

float PhysicsEngine::getTotalEnergy() const {
    return getTotalKineticEnergy() + getTotalPotentialEnergy();
}

} // namespace Physica
