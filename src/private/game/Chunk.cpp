#include "Chunk.h"

Chunk::Chunk(int cx, int cy, int size) : chunkX(cx), chunkY(cy), chunkSize(size) {
    cells.reserve(size * size);

}

Cell* Chunk::at(int localX, int localY) {
    return cells[localX * chunkSize + localY].get();
}

void Chunk::update() {
    // TODO: implement entity updates per chunk
}



void Chunk::render(sf::RenderWindow& window) {

}

std::vector<std::unique_ptr<Cell>>& Chunk::getCells() {
    return cells;
}

void Chunk::push_back(std::unique_ptr<Cell> cell)
{
    cells.push_back(std::move(cell));
}

Chunk::~Chunk()
{
}
