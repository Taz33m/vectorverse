#pragma once
#include "PhysicsObject.h"
#include "PhysicsEngine.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace Physica {

class Renderer {
public:
    Renderer(sf::RenderWindow& window);
    
    void render(const std::vector<std::shared_ptr<PhysicsObject>>& objects);
    void renderObject(const PhysicsObject& obj);
    void renderVectors(const PhysicsObject& obj, bool showVelocity, bool showForce);
    void renderTrajectory(const std::vector<Vector2D>& trail);
    void renderGrid(float spacing);
    
    // Settings
    bool showVelocityVectors = true;
    bool showForceVectors = false;
    bool showLabels = true;
    bool showGrid = false;
    float vectorScale = 0.1f;
    
private:
    sf::RenderWindow& window;
    sf::Font font;
    bool fontLoaded;
    
    void drawArrow(const Vector2D& start, const Vector2D& end, const sf::Color& color);
    void drawCircle(const Vector2D& position, float radius, const sf::Color& color);
    void drawBox(const Vector2D& position, float width, float height, const sf::Color& color);
};

} // namespace Physica
