#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H


#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "biomDef.h"
#include "unordered_map"

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>()(p.first);
        auto h2 = std::hash<T2>()(p.second);
        return h1 ^ (h2 << 1);
    }
};

template<>
struct std::hash<std::tuple<int, int, int>> {
    std::size_t operator()(const std::tuple<int, int, int>& t) const {
        std::size_t h1 = std::hash<int>{}(std::get<0>(t));
        std::size_t h2 = std::hash<int>{}(std::get<1>(t));
        std::size_t h3 = std::hash<int>{}(std::get<2>(t));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

struct GridObject
{
    virtual ~GridObject() = default;
};

struct perTeamPheromone
{

};

//make a class
struct pheromone
{

    std::map<int, double> pheromoneMap;

};


struct CellData
{
    std::string type = "Default";
    std::string terrain ="flat";
    double difficulty = 0;
    pheromone p; 
    BiomeData biomeinfo;
    

        // Move-only
    CellData() = default;
    CellData(CellData&&) = default;
    CellData& operator=(CellData&&) = default;

    // Disable copy
    CellData(const CellData&) = delete;
    CellData& operator=(const CellData&) = delete;
};






#endif