#include "ChunkManager.h"
#include "WorldGeneration.h"
#include <climits>


ChunkManager::ChunkManager(World* world)
: worldGen(std::make_unique<WorldGeneration>(conf::seed, this, conf::cellSize, world)), world(world) 
{
    
}


ChunkManager::~ChunkManager()
{
}

bool ChunkManager::isAvailable(std::string type)
{
    return false;
}

void ChunkManager::addChunk(std::tuple<int, int,int> key, ChunkWrapper newChunk)
{
    grid[key] = std::move(newChunk);
   // std::cout << "Chunk at: " << key.first << ", " << key.second << std::endl;
}

Chunk* ChunkManager::getChunk(int x, int y, int face) {
    if (!this) {
    std::cout << "chunkManager is null!" << std::endl;
    return nullptr;
    }
//std::cout << "grid size: " << grid.size() << std::endl;
    //std::cout << "Looking chunk CM" << std::endl;
    auto it = grid.find({face, x, y});
    if (it != grid.end() && it->second.chunkState == state::AVAILABLE) {
       // std::cout << "Found chunk CM" << std::endl;
        return it->second.chunk.get();
    }
    return nullptr;
}

bool ChunkManager::canWrite()
{
    return false;
}

bool ChunkManager::canRead()
{
    return false;
}

bool ChunkManager::canSerialize()
{
    return false;
}

void ChunkManager::ensureChunksAround(int centerChunkX, int centerChunkY, int radius) {

}


void ChunkManager::unloadDistantChunks(int playerChunkX, int playerChunkY, int radius)
{

}
bool ChunkManager::hasLoaded(int x, int y) const {
    return true;
   // return grid.find({x, y}) != grid.end();
}


void ChunkManager::updateWorldSize() {

}

void ChunkManager::createTempTerrain(sf::VertexArray& vertices, int gridSize)
{
    int totalCells = gridSize * gridSize;
    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(6 * gridSize * gridSize * 4);
  //  std::vector<int> faces = {0,4,5,3,1,2};
    std::vector<int> faces = {0,1,2,3,4,5};

    for (auto& face : faces) {
        std::vector<sf::Color> colors = worldGen.get()->createChunk(face, gridSize, world);
        float step = 2.f / float(gridSize);

        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                int quadIndex = (face * gridSize * gridSize + j * gridSize + i) * 4;
                float x0 = -1.f + i * step;
                float y0 = -1.f + j * step;
                float x1 = x0 + step;
                float y1 = y0 + step;

                std::array<sf::Vector2f, 4> corners = {
                    sf::Vector2f(x0, y0),
                    sf::Vector2f(x0, y1),
                    sf::Vector2f(x1, y1),
                    sf::Vector2f(x1, y0)
                };

                for (int k = 0; k < 4; ++k) {
                    vertices[quadIndex + k].position = corners[k];
                    vertices[quadIndex + k].color = colors[j * gridSize + i];
                    vertices[quadIndex + k].texCoords = sf::Vector2f(face, 0);
                }

            }
        }
    }
}
