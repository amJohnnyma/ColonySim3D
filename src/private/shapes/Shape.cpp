#include "Shape.h"


void Shape::draw(sf::RenderTarget &target) const
{
    target.draw(va);
}

void Shape::setPos(int xr, int xy)
{
    this->xr = xr;
    this->yr = xy;
}
