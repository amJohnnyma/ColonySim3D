#ifndef WORLDGENERATION_H
#define WORLDGENERATION_H

#include <iostream>
#include <array>
#include <random>

#include "../structs/worldObjects.h"
#include "../game/Cell.h"
#include "../shapes/Shape.h"
#include "../shapes/Triangle.h"
#include "../shapes/Circle.h"
#include "../shapes/RoundedRectangle.h"
#include "../shapes/Rectangle.h"
#include "PerlinNoise.h"

class Chunk;
class ChunkManager;
class World;
class WorldGeneration
{
    private:
        PerlinNoise* perlinNoise;
        unsigned int seed;
        //std::map<std::pair<int, int>, std::unique_ptr<Chunk>> grid;
        ChunkManager* cm;
        uint32_t chunkCount = 0;
        int cellSize;
        int curTeam = 0;

    public:
        WorldGeneration(unsigned int seed, ChunkManager* cm, int cellSize, World* world);
        ~WorldGeneration();
        std::unordered_map<std::pair<int, int>, Chunk*, pair_hash> getResult();
        const std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, pair_hash> &getGridRef() const;
        std::vector<sf::Color> createChunk(int face, int gridSize, World *world);

    private:
        pheromone createPheromones(int x, int y);
        double generateDifficulty();
        std::unique_ptr<Rectangle> createCellShape(int x, int y, float size);
        std::pair<std::unique_ptr<Cell>, sf::Color> createCell(int x, int y, int face, float noise);
        void generateTerrain(World *world);
        std::unique_ptr<sf::Sprite> createAntShape(sf::Color fillColor, int x, int y, float cellSize);
        std::unique_ptr<sf::Sprite> createBaseShape(sf::Color fillColor, int x, int y, float cellSize);
        void logAllEntities();
        void generateEntities(int num, int col);
        void assignTextures();
        std::unique_ptr<sf::Sprite> createLocationShape(int x, int y, float cellSize, double difficulty);
        void generateLocations(int num);
        float getDifficulty(int x, int y);
        void createBuilding(int x, int y, std::string type);
        BiomeData getBiome(float val);
        sf::Color colorFromBiome(Biome b);
};

#endif

