#include "math.h"
#include <cmath>


sf::Vector3f Math::projectToTrackball(const sf::Vector2i& pos, const sf::Vector2u& winSize) {
    float x = (2.0f * pos.x - winSize.x) / winSize.x;
    float y = (winSize.y - 2.0f * pos.y) / winSize.y;
    float z2 = 1.0f - x * x - y * y;
    float z = (z2 > 0.0f) ? std::sqrt(z2) : 0.0f;
    return sf::Vector3f(x, y, z);
}

sf::Vector3f Math::normalize(const sf::Vector3f& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len == 0.0f) return v;
    return v / len;
}

float Math::dot(const sf::Vector3f& a, const sf::Vector3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

sf::Vector3f Math::cross(const sf::Vector3f& a, const sf::Vector3f& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

sf::Transform Math::createRotationMatrix(const sf::Vector3f& axis, float angle) {
    sf::Vector3f a = normalize(axis);
    float c = std::cos(angle);
    float s = std::sin(angle);
    float t = 1 - c;

    float x = a.x, y = a.y, z = a.z;

    return sf::Transform(
        t * x * x + c,     t * x * y - s * z, t * x * z + s * y,
        t * x * y + s * z, t * y * y + c,     t * y * z - s * x,
        t * x * z - s * y, t * y * z + s * x, t * z * z + c
    );
}