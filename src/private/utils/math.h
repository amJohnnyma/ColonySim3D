#ifndef MATH_H
#define MATH_H
#include <SFML/Graphics.hpp>

class Math {
public:
    // Projects 2D screen coordinates to a unit hemisphere
    static sf::Vector3f projectToTrackball(const sf::Vector2i& pos, const sf::Vector2u& winSize);

    // Creates a 3x3 rotation matrix from an axis-angle representation
    static sf::Transform createRotationMatrix(const sf::Vector3f& axis, float angle);

    // Normalize a vector
    static sf::Vector3f normalize(const sf::Vector3f& v);

    // Dot product
    static float dot(const sf::Vector3f& a, const sf::Vector3f& b);

    // Cross product
    static sf::Vector3f cross(const sf::Vector3f& a, const sf::Vector3f& b);
};
#endif