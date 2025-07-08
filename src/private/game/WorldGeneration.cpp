#include "WorldGeneration.h"
#include <unordered_set>

#include "World.h"
#include "ChunkManager.h"
WorldGeneration::WorldGeneration(unsigned int seed, ChunkManager* cm, int cellSize, World* world)
{

    if (seed == 0)
    {
        seed = std::random_device{}(); // non-deterministic seed
        std::cout << seed << std::endl;
        conf::seed = seed;
    }

    this->seed = conf::seed;

    perlinNoise = new PerlinNoise(conf::seed);

    this->cellSize = cellSize;
    this->cm = cm;
    //std::cout << "Gen t s" << std::endl;
   // generateTerrain(world); 
    //std::cout << "Gen t d" << std::endl;
    // std::cout << "Gen e s" << std::endl;
    // generateEntities(conf::numAnts,conf::numBases);
    // std::cout << "Gen e d" << std::endl;
    // std::cout << "Gen l s" << std::endl;
    // generateLocations(conf::numLocations);
    // std::cout << "Gen l d" << std::endl;
    // std::cout << "Gen te s" << std::endl;
    // assignTextures();
    // std::cout << "Gen te d" << std::endl;
    //cm->ensureChunksAround(1,1,conf::worldSize.x);
   // logAllEntities();

}

WorldGeneration::~WorldGeneration()
{
    delete perlinNoise;
}

std::unordered_map<std::pair<int, int>, Chunk *, pair_hash> WorldGeneration::getResult() {

    //ask chunk manager for results (i.e. world must ask chunk manager not worldgen)
    std::unordered_map<std::pair<int, int>, Chunk*, pair_hash> result;


    return result;
}

pheromone WorldGeneration::createPheromones(int x, int y)
{
    pheromone p1;

    p1.pheromoneMap[0] = 1;
    
    return p1;
}

double WorldGeneration::generateDifficulty()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.001, 1.0);
    return dis(gen);
}

//these will probably end up using some complex texturing else there will be thousands of draw calls
//just using a rectangle with color for now

std::pair<std::unique_ptr<Cell>, sf::Color> WorldGeneration::createCell(int x, int y, int face, float noise)
{
  //  std::cout << "X<Y<PERLIN\t" << x << ", " << y << ", " << noise << std::endl; 
    auto cell = std::make_unique<Cell>();
    BiomeData cellBiome = getBiome(noise);

    CellData cd;
    cd.type = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    cd.difficulty = noise;
    cd.biomeinfo = cellBiome;

    auto pheromones = createPheromones(x, y);
    cd.p = pheromones;


    sf::Color biomec = colorFromBiome(cellBiome.biome);
  //  std::cout << "Noise: " << noise << std::endl;
  //  std::cout << "colorFromBiome returns: " << (int)biomec.r << ", " << (int)biomec.g << ", " << (int)biomec.b << std::endl;
    double val = std::max(0.2, cd.difficulty);
    biomec.a = static_cast<sf::Uint8>(255 * val);

    cell->x = x;
    cell->y = y;
    cell->face = face;
    cell->data = std::move(cd);
    cell->color = cellBiome.color;

    return {std::move(cell), biomec};
}
std::pair<int, int> getUnfoldedCoords(int face, int globalX, int globalY) {
    // Global offset per face
    switch(face) {
        case 0: return {1 * conf::worldSize + globalX, 1 * conf::worldSize + globalY};
        case 1: return {3 * conf::worldSize + globalX, 1 * conf::worldSize + globalY};
        case 2: return {4 * conf::worldSize + globalX, 1 * conf::worldSize + globalY};
        case 3: return {2 * conf::worldSize + globalX, 1 * conf::worldSize + globalY};
        case 4: return {1 * conf::worldSize + globalX, 2 * conf::worldSize + globalY};
        case 5: return {1 * conf::worldSize + globalX, 3 * conf::worldSize + globalY};
        default: return {0,0}; // Should not happen
    }
}
std::vector<sf::Color> WorldGeneration::createChunk(int face,int gridSize, World* world)
{     
    std::vector<sf::Color> colors;
    int chunksPerAxis = gridSize / conf::chunkSize;
    
   for (int chunkY = 0; chunkY < chunksPerAxis; ++chunkY) {
        for (int chunkX = 0; chunkX < chunksPerAxis; ++chunkX) {
          //  std::cout << "Chunk: Face " << face << ", (" << chunkX << ", " << chunkY << ")\n";

            auto chunk = std::make_unique<Chunk>(chunkX, chunkY, conf::chunkSize);
            std::tuple<int, int,int> key = {face, chunkX , chunkY}; 

            for (int localX = 0; localX < conf::chunkSize; localX++) {
                for (int localY = 0; localY < conf::chunkSize; localY++) {
                    int globalX = chunkX * conf::chunkSize + localX;
                    int globalY = chunkY * conf::chunkSize + localY;
  //                  int globalX = localX * (chunkX+1) * face;
//                    int globalY = localY * (chunkY+1) * face;
                        auto [wx, wy] = getUnfoldedCoords(face, globalX, globalY);
                        float fx = static_cast<float>(wx) * conf::perlinSmoothness;
                        float fy = static_cast<float>(wy) * conf::perlinSmoothness;

            //        float fx = static_cast<float>(globalX) * conf::perlinSmoothness;
              //      float fy = static_cast<float>(globalY) * conf::perlinSmoothness;


                    // will need a map for storing faces and their adjacent. will be useful for pathfinding aswell
                    float val = perlinNoise->val(
                        fx,
                        fy
                    );

                    auto [cell, biomeC] = createCell(globalX, globalY, face, val);

                    chunk->push_back(std::move(cell));
                    colors.push_back(biomeC);
                }
            }

            cm->addChunk(key, {std::move(chunk), state::AVAILABLE});
            chunkCount++;
        }
    }

    return colors;

}


void WorldGeneration::generateTerrain(World* world)
{

}















const std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, pair_hash>& WorldGeneration::getGridRef() const {
    //return grid;
}



int getEdgeBiased(int max) {
    static std::mt19937 rng(conf::seed);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float r = dist(rng);

    // Skew toward edges
    float edgeBias = std::pow(std::abs(r - 0.5f) * 2, 1.5f); // 1.5 = strength of bias
    bool leftSide = (dist(rng) < 0.5f); // flip a coin

    if (leftSide) {
        return static_cast<int>(edgeBias * (max));
    } else {
        return static_cast<int>((1.0f - edgeBias) * (max));
    }
}


void WorldGeneration::assignTextures()
{
}

// Helper to create a random engine once
std::mt19937& getRandomEngine() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

// Hash function for pairs so we can use unordered_set
struct pair_hash_vis {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1,T2>& p) const {
        return std::hash<T1>()(p.first) ^ (std::hash<T2>()(p.second) << 1);
    }
};





float WorldGeneration::getDifficulty(int x, int y)
{    
    int localX = x % conf::chunkSize;
    int localY = y % conf::chunkSize;   
    int chunkX = x / conf::chunkSize;
    int chunkY = y / conf::chunkSize; 
  //  double val = static_cast<double>(grid[{chunkX, chunkY}]->at(localX,localY)->data.difficulty);
   // return val;
   return 0.f;
}

void WorldGeneration::createBuilding(int x, int y, std::string type)
{
        int localX = x % conf::chunkSize;
        int localY = y % conf::chunkSize;   
        int chunkX = x / conf::chunkSize;
        int chunkY = y / conf::chunkSize; 

        //if type == building
      //  Cell* cell = cell = grid[{chunkX, chunkY}].get()->at(localX,localY);
      //  if(cell != nullptr && cell->data.entities.empty())  
        // {
        // std::cout << "Building building at " << x << ", " << y << std::endl;
        // auto building = std::make_unique<BuildingLocation>(x, y, "Building");
        // cell->data.entities.push_back(std::unique_ptr<Entity>(building.release()));
        // }     

}

BiomeData WorldGeneration::getBiome(float e)
{
    for (int i = 0; i < conf::biomeSize; ++i)
    {
        if (e < conf::biomeInfo[i].threshold)
            return conf::biomeInfo[i];
    }
    return conf::biomeInfo[conf::biomeSize-1]; // Fallback (should not reach here if thresholds are valid)
}

sf::Color WorldGeneration::colorFromBiome(Biome b)
{
    if (b >= 0 && b < conf::biomeSize)
        return conf::biomeInfo[b].color;

    return sf::Color(10, 10, 10); // Default/fallback
}
