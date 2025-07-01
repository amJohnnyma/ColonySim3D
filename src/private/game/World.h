#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include "../structs/worldObjects.h"
#include "WorldGeneration.h"
#include <array>
#include <algorithm>
#include "TrackedVariables.h"
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
        TrackedVariables* trackedVars;
        sf::Clock antClock;
        //WorldGeneration* gen;
        std::unique_ptr<ChunkManager> chunkManager;
        std::vector<Cell*> raw_goals;
        sf::Shader terrainShader, billBoardShader;

        //temp

        float radius = 150.f;
        int totalVertices = 6 * conf::worldSize * conf::worldSize * 4;
        sf::VertexArray vertices;
        sf::VertexArray billBoards;
        float rotationX = 0.f;
        float rotationY = 0.f;
        float zoom = 1.f;
        const std::map<std::pair<int, int>, int> faceLookup = { //for when looking up x,y coords
            {{0,0},0},
            {{0,1},4},
            {{0,2},5},
            {{1,0},3},
            {{1,1},4},
            {{1,2},5},
            {{2,0},1},
            {{2,1},4},
            {{2,2},5},
            {{3,0},2},
            {{3,1},4},
            {{3,2},5}
        };
        //temp "ant"
        //global x,y
        int x = 0, y = 0;

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
        std::vector<std::unique_ptr<Cell>> getGrid();
        std::vector<Cell> getBase();
        Cell* at(int x, int y);
        Cell *globalat(int x, int y);
        Chunk *getChunkAt(int chunkX, int chunkY);
        void update();
        void render(sf::RenderWindow & window);
        //temp input
        void toggleSimState();



        void testClick() { std::cout << "Clicked in the world!" << std::endl;}
        void spawn(std::string name, int count) { std::cout << "Spawning: " << count << " " << name << "'s. " << " with " << trackedVars->getNumAnts()  << std::endl;}
        TrackedVariables* getWorldStats() { return trackedVars; }
        void changePF(double amnt) {conf::pF += amnt;}
        void changeHF(double amnt) {conf::hF += amnt;}        
};


#endif