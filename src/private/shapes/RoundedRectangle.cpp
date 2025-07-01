#include "RoundedRectangle.h"
//#include "RoundedRectangle.h"

RoundedRectangle::RoundedRectangle(int xg, int yg, double radius, int resolution, int width, int height)
: Shape(xg,yg), width(width), height(height), resolution(resolution), radius(radius) 
{
    buildShape();

}
void RoundedRectangle::setFillColor(sf::Color col) {
    this->col = col;
    for (size_t i = 0; i < getVA().getVertexCount(); ++i) {
        getVA()[i].color = col;  // Change color of all vertices in the shape
    }
}

void RoundedRectangle::drawCorner(float cx, float cy, float startAngleDeg, float endAngleDeg) {
    sf::Vector2f center(cx, cy);
    for (int i = 0; i < resolution; ++i) {
        float a1 = startAngleDeg + (endAngleDeg - startAngleDeg) * (i / (float)resolution);
        float a2 = startAngleDeg + (endAngleDeg - startAngleDeg) * ((i + 1) / (float)resolution);

        float rad1 = a1 * M_PI / 180.f;
        float rad2 = a2 * M_PI / 180.f;

        sf::Vector2f p1(cx + radius * std::cos(rad1), cy + radius * std::sin(rad1));
        sf::Vector2f p2(cx + radius * std::cos(rad2), cy + radius * std::sin(rad2));

        getVA().append(sf::Vertex(center, sf::Color::White));
        getVA().append(sf::Vertex(p1, sf::Color::White));
        getVA().append(sf::Vertex(p2, sf::Color::White));
    }
}

void RoundedRectangle::buildShape() {
    getVA().clear();
    getVA().setPrimitiveType(sf::Triangles);

    float left = xg * conf::cellSize;
    float top = yg * conf::cellSize;
    float right = left + width;
    float bottom = top + height;

    float r = static_cast<float>(radius);

    auto addTriangle = [&](sf::Vector2f a, sf::Vector2f b, sf::Vector2f c) {
        getVA().append(sf::Vertex(a, sf::Color::White));
        getVA().append(sf::Vertex(b, sf::Color::White));
        getVA().append(sf::Vertex(c, sf::Color::White));
    };

    auto drawCorner = [&](float cx, float cy, float startDeg, float endDeg) {
        sf::Vector2f center(cx, cy);
        for (int i = 0; i < resolution; ++i) {
            float a1 = startDeg + (endDeg - startDeg) * (i / (float)resolution);
            float a2 = startDeg + (endDeg - startDeg) * ((i + 1) / (float)resolution);

            float rad1 = a1 * M_PI / 180.f;
            float rad2 = a2 * M_PI / 180.f;

            sf::Vector2f p1(cx + r * std::cos(rad1), cy + r * std::sin(rad1));
            sf::Vector2f p2(cx + r * std::cos(rad2), cy + r * std::sin(rad2));

            addTriangle(center, p1, p2);
        }
    };

    // Draw 4 corner arcs
    drawCorner(right - r, top + r,   270, 360);  // Top-right
    drawCorner(left + r,  top + r,   180, 270);  // Top-left
    drawCorner(left + r,  bottom - r, 90, 180);  // Bottom-left
    drawCorner(right - r, bottom - r, 0, 90);    // Bottom-right

    // Fill side rectangles
    // Top side (between top-left and top-right arcs)
    addTriangle(
        sf::Vector2f(left + r, top),
        sf::Vector2f(right - r, top),
        sf::Vector2f(left + r, top + r));
    addTriangle(
        sf::Vector2f(right - r, top),
        sf::Vector2f(right - r, top + r),
        sf::Vector2f(left + r, top + r));

    // Bottom side
    addTriangle(
        sf::Vector2f(left + r, bottom - r),
        sf::Vector2f(right - r, bottom - r),
        sf::Vector2f(left + r, bottom));
    addTriangle(
        sf::Vector2f(right - r, bottom - r),
        sf::Vector2f(right - r, bottom),
        sf::Vector2f(left + r, bottom));

    // Left side
    addTriangle(
        sf::Vector2f(left, top + r),
        sf::Vector2f(left + r, top + r),
        sf::Vector2f(left, bottom - r));
    addTriangle(
        sf::Vector2f(left + r, top + r),
        sf::Vector2f(left + r, bottom - r),
        sf::Vector2f(left, bottom - r));

    // Right side
    addTriangle(
        sf::Vector2f(right - r, top + r),
        sf::Vector2f(right, top + r),
        sf::Vector2f(right - r, bottom - r));
    addTriangle(
        sf::Vector2f(right, top + r),
        sf::Vector2f(right, bottom - r),
        sf::Vector2f(right - r, bottom - r));

    // Fill center rectangle
    addTriangle(
        sf::Vector2f(left + r, top + r),
        sf::Vector2f(right - r, top + r),
        sf::Vector2f(left + r, bottom - r));
    addTriangle(
        sf::Vector2f(right - r, top + r),
        sf::Vector2f(right - r, bottom - r),
        sf::Vector2f(left + r, bottom - r));
}

void RoundedRectangle::moveTo(int xg, int yg)
{
    this->xg = xg;
    this->yg=yg;
    buildShape();
}
