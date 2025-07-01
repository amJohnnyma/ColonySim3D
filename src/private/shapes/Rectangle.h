#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

class Rectangle : public Shape
{
    private:
        int width;
        int height;
    public:
        Rectangle(int xg, int yg, int width, int height);
        void setFillColor(sf::Color col) override;
};

#endif