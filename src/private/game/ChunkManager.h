#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <iostream>
#include "Chunk.h"


//This is the only place where chunks can be modified
//Every system must comply and work through chunk manager
// -> For thread safety later on
// -> For serializing data to load in and out of RAM


enum state //Manager stores chunks in a hash map and assigns a status to allow access
{
    IN_USE, //can probably be read with no issues (touchwood)
    AVAILABLE, //can have anything done
    QUEUED, //for save
    SERIALIZING // currently getting serialized


};
struct ChunkWrapper
{
    std::unique_ptr<Chunk> chunk;
    state chunkState;
};

class WorldGeneration;
class World;
class ChunkManager
{
    private:
        std::unordered_map<std::tuple<int, int,int>, ChunkWrapper> grid;
        std::unique_ptr<WorldGeneration> worldGen;        
        World* world;
    private:
        bool canWrite(/*Hash value?*/); //edit a chunk
        bool canRead(); //use a chunk
        bool canSerialize(); //serialize chunk
    public:
        ChunkManager(World* world);
        ~ChunkManager();
        bool isAvailable(std::string type);
        void addChunk(std::tuple<int, int,int> key, ChunkWrapper);
        void createChunk(int x, int y);
        Chunk* getChunk(int x, int y, int face);
        void saveInRam(int x, int y);
        void ensureChunksAround(int playerChunkX, int playerChunkY, int radius);
        void unloadDistantChunks(int playerChunkX, int playerChunkY, int radius);
        bool hasLoaded(int x, int y) const;
        void updateWorldSize();
        void createTempTerrain(sf::VertexArray &vertices, int gridSize);
        void createTempTerrain();
};

#endif