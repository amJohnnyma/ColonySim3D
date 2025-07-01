#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(unsigned int seed)
{
    p.resize(255); //permutation table init
    std::iota(p.begin(),p.end(),0); // fill 0...255

    std::default_random_engine engine(seed);
    std::shuffle(p.begin(),p.end(),engine);

    //duplicate perm vector
    p.insert(p.end(), p.begin(),p.end());
}

PerlinNoise::~PerlinNoise()
{
}

//recursive function to apply elevation smoothing?
/*



*/
float PerlinNoise::noise(float nx, float ny)
{
    int xi = static_cast<int>(std::floor(nx)) & 255;
    int yi = static_cast<int>(std::floor(ny)) & 255;

    float xf = nx - std::floor(nx);
    float yf = ny - std::floor(ny);

    float u = fade(xf);
    float v = fade(yf);

    // Hash coordinates of the 4 square corners
    int aa = p[p[xi] + yi];
    int ab = p[p[xi] + yi + 1];
    int ba = p[p[xi + 1] + yi];
    int bb = p[p[xi + 1] + yi + 1];

    // Gradient and dot product at each corner
    float x1 = lerp(grad(aa, xf,     yf),     // Bottom-left
                    grad(ba, xf - 1, yf), u); // Bottom-right
    float x2 = lerp(grad(ab, xf,     yf - 1), // Top-left
                    grad(bb, xf - 1, yf - 1), u); // Top-right

    return (lerp(x1, x2, v) + 1.0f) / 2.0f; // Normalize to [0,1]
}

float PerlinNoise::val(float x, float y)
{    
    float e = elevation(x, y, conf::perlinLayers);
   // std::cout << "elevation: " << e << ", pow base before clamp" << std::endl;

    if (e < 0.0f) e = 0.0f;

   // std::cout << "elevation after clamp: " << e << std::endl;
    //std::cout << "exponent: " << conf::perlinFlatness << std::endl;

    float val = static_cast<float>(std::pow(e, conf::perlinFlatness));

    //std::cout << "pow result: " << val << std::endl;

    return val;
}

float PerlinNoise::elevation(float nx, float ny, int layers)
{
    float e = 0.0f;
    float maxAmpl = 0.0f;

    for (int i = 0; i < layers; i++)
    {
        int fac = 1 << i;                // frequency factor: 1, 2, 4, 8, ...
        double ampl = pow(0.5, i);       // amplitude: 1, 0.5, 0.25, ...

        e += amplitude(nx, ny, fac, ampl);
        maxAmpl += ampl;
    }

    return e / maxAmpl;
}

float PerlinNoise::amplitude(float nx, float ny, int fac, double amplitude)
{
    return amplitude * noise(fac * nx, fac * ny);
}
