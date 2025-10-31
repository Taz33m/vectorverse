#pragma once
#include "PhysicsEngine.h"
#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <deque>

namespace Physica {

struct EnergyData {
    float time;
    float kinetic;
    float potential;
    float total;
};

enum class SimulationModule {
    Sandbox,
    ProjectileMotion,
    ElasticCollisions,
    HarmonicMotion,
    InclinedPlane
};

class Application {
public:
    Application();
    ~Application();
    
    void run();
    
private:
    // Window and rendering
    sf::RenderWindow window;
    std::unique_ptr<Renderer> renderer;
    
    // Physics
    std::unique_ptr<PhysicsEngine> physicsEngine;
    
    // Simulation state
    bool isPaused;
    bool isStepping;
    float simulationSpeed;
    float timeAccumulator;
    float fixedTimeStep;
    float elapsedTime;
    
    // User interaction
    std::shared_ptr<PhysicsObject> selectedObject;
    Vector2D mouseOffset;
    bool isDragging;
    Vector2D dragStartPos;
    std::vector<Vector2D> predictedTrajectory;
    
    // Energy tracking
    std::deque<EnergyData> energyHistory;
    size_t maxEnergyHistory;
    
    // UI state
    bool showUI;
    bool showEnergyGraph;
    SimulationModule currentModule;
    
    // Methods
    void processEvents();
    void update(float dt);
    void render();
    void renderUI();
    void renderEnergyGraph();
    
    // Input handling
    void handleMousePress(const sf::Vector2i& mousePos);
    void handleMouseRelease();
    void handleMouseMove(const sf::Vector2i& mousePos);
    void handleKeyPress(sf::Keyboard::Key key);
    
    // Object selection
    std::shared_ptr<PhysicsObject> getObjectAtPosition(const Vector2D& pos);
    
    // Module loading
    void loadModule(SimulationModule module);
    void loadSandbox();
    void loadProjectileMotion();
    void loadElasticCollisions();
    void loadHarmonicMotion();
    void loadInclinedPlane();
    
    // Helpers
    void createObject(const Vector2D& position, float mass, const Vector2D& velocity = Vector2D(0, 0));
    void updateEnergyTracking();
    void calculateTrajectory(const Vector2D& startPos, const Vector2D& velocity, float mass);
    void renderTrajectory();
};

} // namespace Physica
