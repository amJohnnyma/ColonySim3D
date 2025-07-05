#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include "../structs/worldObjects.h"
#include "WorldGeneration.h"
#include <array>
#include <algorithm>
#include "Chunk.h"
#include "ChunkManager.h"
//#include "../../controller/InputManager.h"


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
        //WorldGeneration* gen;
        std::unique_ptr<ChunkManager> chunkManager;
        std::vector<Cell*> raw_goals;
        sf::Shader terrainShader, billBoardShader;


        int totalVertices = 6 * conf::worldSize * conf::worldSize * 4;
        sf::VertexArray vertices;
        sf::VertexArray billBoards;
        float rotationX = 0.f;
        float rotationY = 0.f;
        float zoom = 1.f;

        std::unordered_map<std::string, bool> worldCBValues; // Checkbox values for world state

    public:
    private:
        void drawGrid(sf::RenderWindow & window);
        void drawTerrain(sf::RenderWindow & window);
        void drawEntities(sf::RenderWindow & window);


    public:
        World(sf::RenderWindow& window);
        ~World();
        void Init();
    public:
        int getWidth();
        int getHeight();
        Cell *globalat(int x, int y);
        Chunk *getChunkAt(int globalx, int globaly);
        void update();
        void render(sf::RenderWindow & window);
        void updateView(float rotationX, float rotationY, float zoom, sf::RenderWindow &window);
        void updateCBValues(const std::string& name, bool value) {
            worldCBValues[name] = value;
        }
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
};


#endif