#pragma once
#include <cmath>

namespace Physica {

struct Vector2D {
    float x, y;

    Vector2D() : x(0.0f), y(0.0f) {}
    Vector2D(float x, float y) : x(x), y(y) {}

    // Vector operations
    Vector2D operator+(const Vector2D& v) const {
        return Vector2D(x + v.x, y + v.y);
    }

    Vector2D operator-(const Vector2D& v) const {
        return Vector2D(x - v.x, y - v.y);
    }

    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D operator/(float scalar) const {
        return Vector2D(x / scalar, y / scalar);
    }

    Vector2D& operator+=(const Vector2D& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2D& operator-=(const Vector2D& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2D& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Dot product
    float dot(const Vector2D& v) const {
        return x * v.x + y * v.y;
    }

    // Magnitude
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    float magnitudeSquared() const {
        return x * x + y * y;
    }

    // Normalization
    Vector2D normalized() const {
        float mag = magnitude();
        if (mag > 0.0001f) {
            return *this / mag;
        }
        return Vector2D(0, 0);
    }

    void normalize() {
        float mag = magnitude();
        if (mag > 0.0001f) {
            x /= mag;
            y /= mag;
        }
    }

    // Distance between two vectors
    static float distance(const Vector2D& a, const Vector2D& b) {
        return (a - b).magnitude();
    }
};

// Scalar * Vector
inline Vector2D operator*(float scalar, const Vector2D& v) {
    return v * scalar;
}

} // namespace Physica
