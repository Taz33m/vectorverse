#include "Application.h"
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <optional>
#include <cstdint>

namespace Physica {

Application::Application()
    : window(sf::VideoMode({1280, 720}), "Vectorverse - Educational Physics Sandbox"),
      isPaused(false), isStepping(false), simulationSpeed(1.0f),
      timeAccumulator(0.0f), fixedTimeStep(1.0f / 60.0f), elapsedTime(0.0f),
      isDragging(false), maxEnergyHistory(300), showUI(true),
      showEnergyGraph(true), currentModule(SimulationModule::Sandbox) {
    
    window.setFramerateLimit(60);
    
    physicsEngine = std::make_unique<PhysicsEngine>();
    renderer = std::make_unique<Renderer>(window);
    
    loadSandbox();
}

Application::~Application() = default;

void Application::run() {
    sf::Clock clock;
    
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        dt = std::min(dt, 0.1f); // Cap dt to prevent spiral of death
        
        processEvents();
        
        if (!isPaused || isStepping) {
            update(dt * simulationSpeed);
            isStepping = false;
        }
        
        render();
    }
}

void Application::processEvents() {
    while (std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePress->button == sf::Mouse::Button::Left) {
                // Left click only creates objects
                Vector2D pos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                createObject(pos, 10.0f);
            }
            else if (mousePress->button == sf::Mouse::Button::Right) {
                // Right click for slingshot mechanic
                handleMousePress(sf::Mouse::getPosition(window));
            }
        }
        else if (const auto* mouseRelease = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseRelease->button == sf::Mouse::Button::Right) {
                handleMouseRelease();
            }
        }
        else if (event->is<sf::Event::MouseMoved>()) {
            handleMouseMove(sf::Mouse::getPosition(window));
        }
        else if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
            handleKeyPress(keyPress->code);
        }
    }
}

void Application::update(float dt) {
    timeAccumulator += dt;
    
    while (timeAccumulator >= fixedTimeStep) {
        physicsEngine->update(fixedTimeStep);
        physicsEngine->handleBoundaryCollisions(1280, 720);
        
        elapsedTime += fixedTimeStep;
        updateEnergyTracking();
        
        timeAccumulator -= fixedTimeStep;
    }
}

void Application::render() {
    window.clear(sf::Color(20, 20, 30));
    
    renderer->render(physicsEngine->getObjects());
    
    if (showEnergyGraph) {
        renderEnergyGraph();
    }
    
    if (isDragging) {
        renderTrajectory();
    }
    
    window.display();
}

void Application::handleMousePress(const sf::Vector2i& mousePos) {
    Vector2D pos(mousePos.x, mousePos.y);
    selectedObject = getObjectAtPosition(pos);
    
    if (selectedObject && !selectedObject->isStatic) {
        isDragging = true;
        dragStartPos = selectedObject->position; // Store object's original position
        predictedTrajectory.clear();
    }
}

void Application::handleMouseRelease() {
    if (isDragging && selectedObject) {
        // Launch object with calculated velocity (already set in handleMouseMove)
        // No need to modify velocity here - it's already correct
    }
    isDragging = false;
    selectedObject = nullptr;
    predictedTrajectory.clear();
}

void Application::handleMouseMove(const sf::Vector2i& mousePos) {
    if (isDragging && selectedObject) {
        Vector2D currentMousePos(mousePos.x, mousePos.y);
        
        // Angry Birds style: pull back from object position
        // The vector FROM mouse TO object is the pull direction
        Vector2D pullVector = dragStartPos - currentMousePos;
        
        // Velocity is proportional to pull distance (scaled for gameplay)
        // The further you pull, the faster it goes
        selectedObject->velocity = pullVector * 3.0f;
        
        // Object stays at original position until release
        selectedObject->position = dragStartPos;
        
        // Calculate predicted trajectory in LAUNCH direction (opposite of pull)
        calculateTrajectory(dragStartPos, selectedObject->velocity, selectedObject->mass);
    }
}

void Application::handleKeyPress(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Key::Space) {
        isPaused = !isPaused;
    }
    else if (key == sf::Keyboard::Key::S) {
        isStepping = true;
    }
    else if (key == sf::Keyboard::Key::R) {
        loadModule(currentModule);
    }
    else if (key == sf::Keyboard::Key::C) {
        physicsEngine->clearObjects();
        energyHistory.clear();
    }
    else if (key == sf::Keyboard::Key::G) {
        physicsEngine->gravityEnabled = !physicsEngine->gravityEnabled;
    }
    else if (key == sf::Keyboard::Key::V) {
        renderer->showVelocityVectors = !renderer->showVelocityVectors;
    }
    else if (key == sf::Keyboard::Key::Num1) {
        loadModule(SimulationModule::Sandbox);
    }
    else if (key == sf::Keyboard::Key::Num2) {
        loadModule(SimulationModule::ProjectileMotion);
    }
    else if (key == sf::Keyboard::Key::Num3) {
        loadModule(SimulationModule::ElasticCollisions);
    }
}

std::shared_ptr<PhysicsObject> Application::getObjectAtPosition(const Vector2D& pos) {
    for (auto& obj : physicsEngine->getObjects()) {
        float dist = Vector2D::distance(pos, obj->position);
        if (obj->shape == ShapeType::Circle && dist < obj->radius) {
            return obj;
        }
    }
    return nullptr;
}

void Application::createObject(const Vector2D& position, float mass, const Vector2D& velocity) {
    auto obj = std::make_shared<PhysicsObject>(position, mass);
    obj->velocity = velocity;
    obj->colorR = 0.3f + (rand() % 100) / 300.0f;
    obj->colorG = 0.3f + (rand() % 100) / 300.0f;
    obj->colorB = 0.6f + (rand() % 100) / 300.0f;
    physicsEngine->addObject(obj);
}

void Application::updateEnergyTracking() {
    EnergyData data;
    data.time = elapsedTime;
    data.kinetic = physicsEngine->getTotalKineticEnergy();
    data.potential = physicsEngine->getTotalPotentialEnergy();
    data.total = data.kinetic + data.potential;
    
    energyHistory.push_back(data);
    if (energyHistory.size() > maxEnergyHistory) {
        energyHistory.pop_front();
    }
}

void Application::renderEnergyGraph() {
    if (energyHistory.empty()) return;
    
    float graphX = 900.0f;
    float graphY = 20.0f;
    float graphW = 350.0f;
    float graphH = 150.0f;
    
    // Background
    sf::RectangleShape bg(sf::Vector2f(graphW, graphH));
    bg.setPosition({graphX, graphY});
    bg.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(bg);
    
    // Find max energy for scaling
    float maxE = 1.0f;
    for (const auto& d : energyHistory) {
        maxE = std::max(maxE, d.total);
    }
    
    // Draw energy lines
    if (energyHistory.size() > 1) {
        float xStep = graphW / maxEnergyHistory;
        
        // Total energy (white)
        sf::VertexArray totalLine(sf::PrimitiveType::LineStrip);
        // Kinetic energy (green)
        sf::VertexArray kineticLine(sf::PrimitiveType::LineStrip);
        // Potential energy (red)
        sf::VertexArray potentialLine(sf::PrimitiveType::LineStrip);
        
        for (size_t i = 0; i < energyHistory.size(); ++i) {
            float x = graphX + i * xStep;
            float yTotal = graphY + graphH - (energyHistory[i].total / maxE) * graphH;
            float yKinetic = graphY + graphH - (energyHistory[i].kinetic / maxE) * graphH;
            float yPotential = graphY + graphH - (energyHistory[i].potential / maxE) * graphH;
            
            totalLine.append(sf::Vertex{{x, yTotal}, sf::Color::White});
            kineticLine.append(sf::Vertex{{x, yKinetic}, sf::Color::Green});
            potentialLine.append(sf::Vertex{{x, yPotential}, sf::Color::Red});
        }
        
        window.draw(kineticLine);
        window.draw(potentialLine);
        window.draw(totalLine);
    }
}

void Application::loadModule(SimulationModule module) {
    currentModule = module;
    physicsEngine->clearObjects();
    energyHistory.clear();
    elapsedTime = 0.0f;
    
    switch (module) {
        case SimulationModule::Sandbox:
            loadSandbox();
            break;
        case SimulationModule::ProjectileMotion:
            loadProjectileMotion();
            break;
        case SimulationModule::ElasticCollisions:
            loadElasticCollisions();
            break;
        case SimulationModule::HarmonicMotion:
            loadHarmonicMotion();
            break;
        case SimulationModule::InclinedPlane:
            loadInclinedPlane();
            break;
    }
}

void Application::loadSandbox() {
    // Create a few demo objects
    createObject(Vector2D(200, 200), 15.0f);
    createObject(Vector2D(400, 150), 20.0f);
    createObject(Vector2D(600, 250), 10.0f);
}

void Application::loadProjectileMotion() {
    auto obj = std::make_shared<PhysicsObject>(Vector2D(100, 600), 10.0f);
    obj->velocity = Vector2D(300, -400);
    obj->colorR = 1.0f;
    obj->colorG = 0.5f;
    obj->colorB = 0.0f;
    physicsEngine->addObject(obj);
}

void Application::loadElasticCollisions() {
    auto obj1 = std::make_shared<PhysicsObject>(Vector2D(300, 360), 15.0f);
    obj1->velocity = Vector2D(200, 0);
    obj1->restitution = 1.0f;
    obj1->colorR = 0.2f;
    obj1->colorG = 0.8f;
    obj1->colorB = 1.0f;
    physicsEngine->addObject(obj1);
    
    auto obj2 = std::make_shared<PhysicsObject>(Vector2D(800, 360), 15.0f);
    obj2->velocity = Vector2D(-200, 0);
    obj2->restitution = 1.0f;
    obj2->colorR = 1.0f;
    obj2->colorG = 0.3f;
    obj2->colorB = 0.3f;
    physicsEngine->addObject(obj2);
}

void Application::loadHarmonicMotion() {
    // Simple pendulum-like motion
    auto obj = std::make_shared<PhysicsObject>(Vector2D(640, 200), 10.0f);
    obj->velocity = Vector2D(200, 0);
    physicsEngine->addObject(obj);
}

void Application::loadInclinedPlane() {
    // Create static inclined surface (simplified)
    createObject(Vector2D(300, 400), 10.0f, Vector2D(100, -50));
}

void Application::calculateTrajectory(const Vector2D& startPos, const Vector2D& velocity, float mass) {
    predictedTrajectory.clear();
    
    // Simulate the trajectory for a short time
    const int numPoints = 50;
    const float dt = 0.05f; // 50ms timesteps
    
    Vector2D pos = startPos;
    Vector2D vel = velocity;
    
    // Get gravity from physics engine
    Vector2D gravity = physicsEngine->getGravity();
    
    for (int i = 0; i < numPoints; ++i) {
        predictedTrajectory.push_back(pos);
        
        // Simple Euler integration for prediction
        Vector2D acceleration = gravity;
        vel += acceleration * dt;
        pos += vel * dt;
        
        // Stop if trajectory goes off screen or hits ground
        if (pos.x < 0 || pos.x > 1280 || pos.y > 720) {
            break;
        }
    }
}

void Application::renderTrajectory() {
    // Draw slingshot lines (like rubber bands)
    if (isDragging && selectedObject) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        Vector2D objectPos = dragStartPos;
        
        // Draw two lines from object to mouse (like slingshot bands)
        sf::Vertex leftBand[] = {
            sf::Vertex{{objectPos.x - 10, objectPos.y}, sf::Color(139, 69, 19, 200)}, // Brown
            sf::Vertex{{static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)}, sf::Color(139, 69, 19, 200)}
        };
        sf::Vertex rightBand[] = {
            sf::Vertex{{objectPos.x + 10, objectPos.y}, sf::Color(139, 69, 19, 200)},
            sf::Vertex{{static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)}, sf::Color(139, 69, 19, 200)}
        };
        
        window.draw(leftBand, 2, sf::PrimitiveType::Lines);
        window.draw(rightBand, 2, sf::PrimitiveType::Lines);
        
        // Draw a line showing pull direction and power
        sf::Vertex pullLine[] = {
            sf::Vertex{{objectPos.x, objectPos.y}, sf::Color(255, 100, 100, 150)},
            sf::Vertex{{static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)}, sf::Color(255, 100, 100, 150)}
        };
        window.draw(pullLine, 2, sf::PrimitiveType::Lines);
    }
    
    // Draw trajectory as dotted line with circles
    for (size_t i = 0; i < predictedTrajectory.size(); ++i) {
        // Draw a dot every few points
        if (i % 3 == 0) {
            const Vector2D& point = predictedTrajectory[i];
            
            // Fade alpha based on distance along trajectory
            std::uint8_t alpha = static_cast<std::uint8_t>(200 - (i * 150 / predictedTrajectory.size()));
            
            // Draw small circle
            sf::CircleShape dot(3.0f);
            dot.setPosition({point.x - 3.0f, point.y - 3.0f});
            dot.setFillColor(sf::Color(255, 255, 100, alpha)); // Yellow with fade
            window.draw(dot);
        }
    }
    
    // Also draw the curve as a line strip for smoothness
    sf::VertexArray curve(sf::PrimitiveType::LineStrip);
    for (size_t i = 0; i < predictedTrajectory.size(); ++i) {
        std::uint8_t alpha = static_cast<std::uint8_t>(180 - (i * 120 / predictedTrajectory.size()));
        curve.append(sf::Vertex{{predictedTrajectory[i].x, predictedTrajectory[i].y}, 
                                sf::Color(255, 220, 100, alpha)});
    }
    window.draw(curve);
}

} // namespace Physica (renamed to vectorverse)
