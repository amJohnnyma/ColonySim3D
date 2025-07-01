#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <SFML/Graphics.hpp>
#include "../structs/biomDef.h"

namespace conf
{
    extern const int cellSize;
    extern const int antSize;
    extern double maxPheromone;
    extern const sf::Vector2f window_size;
    extern const sf::Vector2f window_size_f;
    extern int worldSize;
    extern const uint32_t max_framerate;
    extern const float dt;
    extern float calcDT;
    extern const int numAnts;
    extern const int numBases;
    extern const int numLocations;
    extern const int antPathDrawLength;
    extern int Q ;
    extern double pF ; //pheremone influnce
    extern double hF ; // heuristic influence
    extern double pheremoneEvap ;
    extern unsigned int timestep;
    extern double locationFoodRegenerationRate;
    extern float antSpeed;
    extern float terrainDifficultyScale;
    extern int baseSeparationDistance;
    extern int chunkSize;
    extern float perlinSmoothness;
    extern unsigned int seed;
    extern int perlinLayers;
    extern double perlinFlatness;
    constexpr int biomeSize = 5;
    extern const BiomeData biomeInfo[biomeSize];
    extern int simulationSpeed;
    extern double baseSpawnChance; //0 - 1
    extern int defaultNumAntsPerBase;
    extern double numAntsToSpawn; //0.5 - 1.5
    extern double locationSpawnChance; //0 - 1
    extern int numberOfTeams; // >0
    extern double chosenTeam; //0 - 1 (chosen * num teams = team)
    extern float worldRadius;
    extern const std::map<std::pair<int, int>, int> faceLookup;

}


#endif // GLOBALVARS_H
