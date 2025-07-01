#ifndef CELL_H
#define CELL_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../structs/worldObjects.h"


class Cell {
    public:
        int x, y, face;
        CellData data;


    public:
        Cell() {}
        Cell(int x, int y, int face, CellData &&data);
        // Cell(const Cell& other);
        // Cell& operator=(const Cell& other);
        ~Cell();

        // Getters
        int getX() const { return x; }
        int getY() const { return y; }
        const CellData& getData() const { return data; }


};

#endif