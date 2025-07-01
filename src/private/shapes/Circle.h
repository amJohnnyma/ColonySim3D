#ifndef CIRCLE_H
#define CIRCE_H

#include "Shape.h"

class Circle : public Shape
{
    private:
        double radius;
        int resolution = 8;
    public:
        Circle(int xg, int yg, double radius, int resolution);
        void setFillColor(sf::Color col) override;

};

#endif