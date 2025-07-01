#ifndef ROUNDED_RECTANGLE_H
#define ROUNDED_RECTANGLE_H
#include "Shape.h"
#include <iostream>
class RoundedRectangle : public Shape // will be a circle with four corners split up
{
    private:
        int width, height, resolution, radius;
    public:
        RoundedRectangle(int xr, int yr, double radius, int resolution, int width, int height);
        ~RoundedRectangle() {};

    public:
        void setFillColor(sf::Color col) override;

        void drawCorner(float cx, float cy, float startAngleDeg, float endAngleDeg);
        void buildShape();
        void moveTo(int xg, int yg);
};

#endif

