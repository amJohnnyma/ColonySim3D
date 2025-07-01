#include "Triangle.h"

Triangle::Triangle(int xg, int yg, double size) 
: Shape(xg, yg), size(size)
{
    float centerX = xg * conf::cellSize + conf::cellSize / 2;
    float centerY = yg * conf::cellSize + conf::cellSize / 2;

    float height = static_cast<float>(size); // vertical distance from center to top
    float halfBase = height / std::sqrt(3.0f); // base/2 for equilateral triangle

    // 3 vertices (top, bottom left, bottom right)
    sf::Vector2f top(centerX, centerY - height / 2);
    sf::Vector2f bl(centerX - halfBase, centerY + height / 2);
    sf::Vector2f br(centerX + halfBase, centerY + height / 2);

    getVA().append(sf::Vertex(top));
    getVA().append(sf::Vertex(bl));
    getVA().append(sf::Vertex(br));

}

void Triangle::setFillColor(sf::Color col) {
    this->col = col;
    for (size_t i = 0; i < getVA().getVertexCount(); ++i) {
        getVA()[i].color = col;  // Change color of all vertices in the shape
    }
}