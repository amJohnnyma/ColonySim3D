#include "GlobalVars.h"
#include <cmath>
// put in wrapper to modify vars at runtime
namespace conf
{
    const int cellSize = 50;
    const int antSize = 50;
    double maxPheromone = 20;
    const sf::Vector2f window_size = {1000, 1000};
    const sf::Vector2f window_size_f = static_cast<sf::Vector2f>(window_size);
    // default - 512
    int worldSize = 4; // per square face
    int chunkSize = std::clamp((worldSize / 4), 1, 128);
    /*
    16,16 = 1x1
    32,32 = 2x2
    ...
    n,n = (n/chunksize)^2

    */
    const uint32_t max_framerate = 60;
    const float dt = 1.0f / static_cast<float>(max_framerate);
    float calcDT = 0;
    const int numAnts = 100; // per base
    const int numBases = 4;  // one base per team
    const int numLocations = 3;
    const int antPathDrawLength = 10;
    int Q = 1;
    double pF = 0.5;  // pheremone influnce
    double hF = 20.8; // heuristic influence //higher because terrain difficulty lowers it heavily
    double pheremoneEvap = 0.005;
    unsigned int timestep = 500;
    double locationFoodRegenerationRate = 1000;
    float antSpeed = 250; // 250 / log(simSpeed + 0.1) + 1 -> decent scaling
    float terrainDifficultyScale = 0.05f;
    int baseSeparationDistance = 5;
    float perlinSmoothness = 1.f / 16.f;
    unsigned int seed = 0;
    int perlinLayers = 5;
    double perlinFlatness = 0.9f;
    const BiomeData biomeInfo[biomeSize] = {
        {0.35f, false, sf::Color(0, 0, 255), WATER},     // WATER
        {0.41f, true, sf::Color(255, 255, 0), BEACH},    // BEACH
        {0.74f, true, sf::Color(0, 255, 0), FOREST},     // FOREST
        {0.95f, true, sf::Color(100, 255, 100), JUNGLE}, // JUNGLE
        {1.0f, false, sf::Color(150, 75, 0), WOODS}      // WOODS
    };
    int simulationSpeed = 10;
    double baseSpawnChance = 0.001; // 0 - 1
    int defaultNumAntsPerBase = 200;
    double numAntsToSpawn = 0.5;        // 0.5 - 1.5
    double locationSpawnChance = 0.001; // 0 - 1
    int numberOfTeams = 16;             // >0
    double chosenTeam;                  // 0 - 1 (chosen * num teams = team)
    float worldRadius = 250;

    //a flag to know if i must change the rotation of x,y ?
    const std::map<std::pair<int, int>, int> faceLookup = { // for when looking up x,y coords
        //X = 0
        {{0, 0}, 0},
        {{0, 1}, 5},
        {{0, 2}, 4},
        //X = 1
        {{1, 0}, 3},
        {{1, 1}, 5},
        {{1, 2}, 4},
        //X = 2
        {{2, 0}, 1},
        {{2, 1}, 3},
        {{2, 2}, 5},
        //X = 3
        {{3, 0}, 2},
        {{3, 1}, 5},
        {{3, 2}, 4}};

    float distance = worldRadius * 2.f;       
}