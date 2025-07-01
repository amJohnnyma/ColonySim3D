#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "../game/GlobalVars.h"

class Shape //abstract
{
    protected:
        int xg,yg; //gridspace
        int xr, yr; // window space
        sf::VertexArray va;
        sf::Color col;

    public:
        Shape(int xg, int yg):xg(xg),yg(yg) {xr = xg * conf::cellSize; yr = xg * conf::cellSize;}
            virtual ~Shape() = default;  // Virtual destructor
      //  virtual std::vector<sf::Vertex> & getVertexArray() = 0;
        virtual void setFillColor(sf::Color col) = 0;
        void draw(sf::RenderTarget& target) const;


        //Getters
        int getXG() {return xg;}
        int getYG(){ return yg;}
        int getXR() {return xr;}
        int getYR() {return yr;}
        sf::VertexArray & getVA() {return va;}
        sf::Color getColor() { return this->col;}
        //Setters
        void setXG(int xg) {this->xg = xg;}
        void setYG(int yg) {this->yg = yg;}
        void setXR(int xr) {this->xr = xr;}
        void setYR(int yr) {this->yr = yr;}
        void setVA(sf::VertexArray va) {this->va = va;}
        void setPos(int xr, int xy);


        
};


#endif