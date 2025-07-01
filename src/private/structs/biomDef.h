#ifndef BIOM_DEF_H
#define BIOM_DEF_H

#include <SFML/Graphics.hpp>

enum Biome
{
    WATER,
    BEACH,
    FOREST,
    JUNGLE,
    WOODS
};

struct BiomeData {
    float threshold;     // max elevation value for this biome
    bool passable;       // is this biome passable?
    sf::Color color;     // visualization color
    Biome biome; //the actual biom
};

#endif