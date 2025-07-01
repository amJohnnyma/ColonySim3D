#include "Cell.h"

Cell::Cell(int x, int y, int face, CellData&& data)
    : x(x), y(y), face(face), data(std::move(data)) {}


// Cell::Cell(const Cell& other)
//     : x(other.x), y(other.y), data(other.data), originalColor(other.originalColor) {
//     if (other.cellShape)
//         cellShape = other.cellShape->clone();  // Requires `Shape::clone()`
// }

// Cell& Cell::operator=(const Cell& other) {
//     if (this == &other) return *this;
//     x = other.x;
//     y = other.y;
//     data = other.data;
//     originalColor = other.originalColor;
//     cellShape = other.cellShape ? other.cellShape->clone() : nullptr;
//     return *this;
// }

Cell::~Cell() {}

