#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Shape.h"
class Triangle : public Shape
{
    private:
        double size;
    public:
        Triangle(int xg, int yg, double size);
        void setFillColor(sf::Color col) override;
};

#endif