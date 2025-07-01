#include "Circle.h"

Circle::Circle(int xg, int yg, double radius, int resolution)
: Shape(xg, yg), radius(radius)
{
    float centerX = xg * conf::cellSize + conf::cellSize / 2;
    float centerY = yg * conf::cellSize + conf::cellSize / 2;

        getVA().setPrimitiveType(sf::TriangleFan);

        // Add center vertex
        getVA().append(sf::Vertex(sf::Vector2f(centerX, centerY)));

        // Add surrounding points on the circle
        for (int i = 0; i <= resolution; ++i) {
            float angle = static_cast<float>(i) / resolution * 2 * M_PI;
            float x = centerX + radius * std::cos(angle);
            float y = centerY + radius * std::sin(angle);
            getVA().append(sf::Vertex(sf::Vector2f(x, y)));
        }

}

void Circle::setFillColor(sf::Color col) {
    this->col = col;
    for (size_t i = 0; i < getVA().getVertexCount(); ++i) {
        getVA()[i].color = col;  // Change color of all vertices in the shape
    }
}