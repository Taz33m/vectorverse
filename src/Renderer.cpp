#include "Renderer.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdint>

namespace Physica {

Renderer::Renderer(sf::RenderWindow& window)
    : window(window), fontLoaded(false) {
    // Try to load a system font (fallback to default if not found)
    fontLoaded = font.openFromFile("/System/Library/Fonts/Helvetica.ttc");
    if (!fontLoaded) {
        fontLoaded = font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    }
}

void Renderer::render(const std::vector<std::shared_ptr<PhysicsObject>>& objects) {
    if (showGrid) {
        renderGrid(50.0f);
    }
    
    for (const auto& obj : objects) {
        renderObject(*obj);
        renderVectors(*obj, showVelocityVectors, showForceVectors);
    }
}

void Renderer::renderObject(const PhysicsObject& obj) {
    sf::Color color(
        static_cast<std::uint8_t>(obj.colorR * 255),
        static_cast<std::uint8_t>(obj.colorG * 255),
        static_cast<std::uint8_t>(obj.colorB * 255)
    );
    
    // Dimmer color for static objects
    if (obj.isStatic) {
        color = sf::Color(color.r / 2, color.g / 2, color.b / 2);
    }
    
    if (obj.shape == ShapeType::Circle) {
        drawCircle(obj.position, obj.radius, color);
    } else if (obj.shape == ShapeType::Box) {
        drawBox(obj.position, obj.width, obj.height, color);
    }
    
    // Draw label
    if (showLabels && fontLoaded && !obj.label.empty()) {
        sf::Text text(font, obj.label, 12);
        text.setFillColor(sf::Color::White);
        text.setPosition({obj.position.x - 20, obj.position.y - obj.radius - 20});
        window.draw(text);
    }
}

void Renderer::renderVectors(const PhysicsObject& obj, bool showVelocity, bool showForce) {
    if (obj.isStatic) return;
    
    Vector2D start = obj.position;
    
    // Velocity vector (green)
    if (showVelocity && obj.velocity.magnitude() > 0.1f) {
        Vector2D end = start + obj.velocity * vectorScale;
        drawArrow(start, end, sf::Color::Green);
    }
    
    // Force vector (red)
    if (showForce && obj.forceAccumulator.magnitude() > 0.1f) {
        Vector2D end = start + obj.forceAccumulator * vectorScale * 0.01f;
        drawArrow(start, end, sf::Color::Red);
    }
}

void Renderer::renderTrajectory(const std::vector<Vector2D>& trail) {
    if (trail.size() < 2) return;
    
    sf::VertexArray lines(sf::PrimitiveType::LineStrip, trail.size());
    for (size_t i = 0; i < trail.size(); ++i) {
        lines[i].position = sf::Vector2f(trail[i].x, trail[i].y);
        // Fade color based on age
        std::uint8_t alpha = static_cast<std::uint8_t>(255 * (i + 1) / trail.size());
        lines[i].color = sf::Color(255, 255, 255, alpha);
    }
    window.draw(lines);
}

void Renderer::renderGrid(float spacing) {
    sf::Vector2u size = window.getSize();
    sf::Color gridColor(50, 50, 50);
    
    // Vertical lines
    for (float x = 0; x < size.x; x += spacing) {
        sf::Vertex line[] = {
            sf::Vertex{{x, 0}, gridColor},
            sf::Vertex{{x, static_cast<float>(size.y)}, gridColor}
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
    
    // Horizontal lines
    for (float y = 0; y < size.y; y += spacing) {
        sf::Vertex line[] = {
            sf::Vertex{{0, y}, gridColor},
            sf::Vertex{{static_cast<float>(size.x), y}, gridColor}
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

void Renderer::drawArrow(const Vector2D& start, const Vector2D& end, const sf::Color& color) {
    // Draw line
    sf::Vertex line[] = {
        sf::Vertex{{start.x, start.y}, color},
        sf::Vertex{{end.x, end.y}, color}
    };
    window.draw(line, 2, sf::PrimitiveType::Lines);
    
    // Draw arrowhead
    Vector2D direction = (end - start).normalized();
    Vector2D perpendicular(-direction.y, direction.x);
    
    float arrowSize = 8.0f;
    Vector2D tip = end;
    Vector2D left = tip - direction * arrowSize + perpendicular * (arrowSize * 0.5f);
    Vector2D right = tip - direction * arrowSize - perpendicular * (arrowSize * 0.5f);
    
    sf::ConvexShape arrow(3);
    arrow.setPoint(0, {tip.x, tip.y});
    arrow.setPoint(1, {left.x, left.y});
    arrow.setPoint(2, {right.x, right.y});
    arrow.setFillColor(color);
    window.draw(arrow);
}

void Renderer::drawCircle(const Vector2D& position, float radius, const sf::Color& color) {
    sf::CircleShape circle(radius);
    circle.setPosition({position.x - radius, position.y - radius});
    circle.setFillColor(color);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(2.0f);
    window.draw(circle);
}

void Renderer::drawBox(const Vector2D& position, float width, float height, const sf::Color& color) {
    sf::RectangleShape box(sf::Vector2f(width, height));
    box.setPosition({position.x - width / 2, position.y - height / 2});
    box.setFillColor(color);
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(2.0f);
    window.draw(box);
}

} // namespace Physica
