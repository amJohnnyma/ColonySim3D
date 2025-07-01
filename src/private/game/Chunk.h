#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <memory>
#include "../structs/worldObjects.h"
#include "../game/Cell.h"

class Chunk {
private:
    int chunkX, chunkY; // chunk position in world
    int chunkSize; // e.g., 16x16 cells
    std::vector<std::unique_ptr<Cell>> cells;

public:
    Chunk(int cx, int cy, int size);
    Cell* at(int localX, int localY);
    void update();
    void render(sf::RenderWindow& window);
    std::vector<std::unique_ptr<Cell>>& getCells();
    void push_back(std::unique_ptr<Cell> cell);
    ~Chunk();
};

#endif
