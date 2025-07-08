#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include "../structs/worldObjects.h"
#include "WorldGeneration.h"
#include <array>
#include <algorithm>
#include "Chunk.h"
#include "ChunkManager.h"
#include "../utils/math.h"
// #include "../../controller/InputManager.h"

class InputManager;
class ACO;
class World
{
private:
    int temp = 0;
    float angle = 0;

    float baseSpeed = 5.0f;
    float speed;
    bool running = true;
    sf::Clock antClock;
    // WorldGeneration* gen;
    std::unique_ptr<ChunkManager> chunkManager;
    std::vector<Cell *> raw_goals;
    sf::Shader terrainShader, billBoardShader;

    int totalVertices = 6 * conf::worldSize * conf::worldSize * 4;
    sf::VertexArray vertices;
    sf::VertexArray billBoards;
    float rotationX = 0.f;
    float rotationY = 0.f;
    float zoom = 1.f;
    sf::Vector2f center;
    sf::CircleShape outline;
    float projectedRadius;

    std::unordered_map<std::string, bool> worldCBValues; // Checkbox values for world state
    std::unordered_map<std::string, int> worldSliderValues; // Checkbox values for world state

    std::vector<math::GridCoord> lastHoverTiles;
    std::vector<math::GridCoord> curHoverTiles;

    //temp
    sf::CircleShape dot;
    sf::Vector2f screenPos;
    math::DebugVariables debug;

public:
private:
    void drawGrid(sf::RenderWindow &window);
    void resetCellColor(const math::GridCoord &coord);
    void drawTerrain(sf::RenderWindow &window);
    void drawEntities(sf::RenderWindow &window);

public:
    World(sf::RenderWindow &window);
    ~World();
    void Init();

public:
    int getWidth();
    int getHeight();
    Cell *globalat(int x, int y);
    Cell *cellat(int face, int x, int y);
    Chunk *getChunkAt(int globalx, int globaly);
    void update();
    void render(sf::RenderWindow &window);
    void updateView(float rotationX, float rotationY, float zoom, sf::RenderWindow &window);

public: //input stuff from UI and inputManager
    void updateCBValues(const std::string &name, bool value)
    {
        worldCBValues[name] = value;        
    }
    void updateSliderValues(const std::string &name, int value);

    bool getCBValues(std::string name)
    {
        if (worldCBValues.find(name) != worldCBValues.end())
            return worldCBValues[name];
        else
        {
            worldCBValues[name] = false;
            return false;
        }
    }

    int getSliderValues(std::string name)
    {
        if (worldSliderValues.find(name) != worldSliderValues.end())
            return worldSliderValues[name];
        else
        {
            worldSliderValues[name] = 0;
            return 0;
        }
    }
    void selectTiles(sf::Vector2i start, sf::Vector2i end);
    bool isOverWorld(sf::Vector2i pos);
    void hoverEffect(sf::Vector2i mousePos);
    void highlightCell(const math::GridCoord &coord, sf::Color color);
};

#endif