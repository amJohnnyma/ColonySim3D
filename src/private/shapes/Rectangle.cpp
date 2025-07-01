#include "Rectangle.h"

Rectangle::Rectangle(int xg, int yg, int width, int height)
: Shape(xg,yg), width(width), height(height)
{
    float centerX = xg * conf::cellSize + conf::cellSize / 2;
    float centerY = yg * conf::cellSize + conf::cellSize / 2;

    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    getVA().setPrimitiveType(sf::TriangleFan);

    getVA().append(sf::Vertex(sf::Vector2f(centerX, centerY))); // center (optional for triangle fan)

    getVA().append(sf::Vertex(sf::Vector2f(centerX - halfWidth, centerY - halfHeight))); // top-left
    getVA().append(sf::Vertex(sf::Vector2f(centerX + halfWidth, centerY - halfHeight))); // top-right
    getVA().append(sf::Vertex(sf::Vector2f(centerX + halfWidth, centerY + halfHeight))); // bottom-right
    getVA().append(sf::Vertex(sf::Vector2f(centerX - halfWidth, centerY + halfHeight))); // bottom-left
    getVA().append(sf::Vertex(sf::Vector2f(centerX - halfWidth, centerY - halfHeight))); // close loop (optional)

}


void Rectangle::setFillColor(sf::Color col) {
    this->col = col;
    for (size_t i = 0; i < getVA().getVertexCount(); ++i) {
        getVA()[i].color = col;  // Change color of all vertices in the shape
    }
}