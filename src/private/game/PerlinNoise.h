#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include "GlobalVars.h"
#include <iostream>
//https://www.redblobgames.com/maps/terrain-from-noise/
class PerlinNoise
{
    private:
        std::vector<int> p; //permutation table
    public:
        PerlinNoise(unsigned int seed = std::default_random_engine::default_seed);
        ~PerlinNoise();
        float noise(float x, float y);
        float val(float x, float y);
    private:
        static float fade(float t) {
            return t * t * t * (t * (t * 6 - 15) + 10); // 6t^5 - 15t^4 + 10t^3
        }

        static float lerp(float a, float b, float t) {
            return a + t * (b - a);
        }

        // Pseudo-random gradient vector dot product
        static float grad(int hash, float x, float y) {
            switch (hash & 3) { // Pick 1 of 4 directions
                case 0: return  x + y;
                case 1: return -x + y;
                case 2: return  x - y;
                case 3: return -x - y;
                default: return 0; // unreachable
            }
        }

        float elevation(float nx, float ny, int layers);
        float amplitude(float nx, float ny, int fac, double multFac);
};

#endif