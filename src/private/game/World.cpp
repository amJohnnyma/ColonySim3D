#include "World.h"
#include "GlobalVars.h"
#include <SFML/OpenGL.hpp>
#include "../utils/math.h"

World::World(sf::RenderWindow &window)
{

    // trackedVars = new TrackedVariables();
}

void World::Init()
{
    center = {conf::window_size.x/2, conf::window_size.y/2};
    std::cout << "Creating shader" << std::endl;
    if (!terrainShader.loadFromFile("terrain.vert", "terrain.frag"))
    {
        std::cerr << "Failed to load shaders\n";
    }

    terrainShader.setUniform("radius", conf::worldRadius);
    terrainShader.setUniform("screenCenter", sf::Glsl::Vec2(center.x, center.y));
    terrainShader.setUniform("zoom", 1.f);
    terrainShader.setUniform("distance", conf::distance);
    terrainShader.setUniform("screenSize", conf::window_size);
    terrainShader.setUniform("faceSize", 20.f);
    

    std::cout << "Creating vertices" << std::endl;
    vertices = sf::VertexArray(sf::Quads, totalVertices);
    if (!chunkManager)
    {
        chunkManager = std::make_unique<ChunkManager>(this);
        chunkManager.get()->createTempTerrain(vertices, conf::worldSize); // world size is actually * 6
    }

    std::cout << "Creating billboard shader" << std::endl;
    if (!billBoardShader.loadFromFile("billboard.vert", "billboard.frag"))
    {
        std::cerr << "Failed to load billboard shaders\n";
    }
    // sf::Texture billboardTexture;
    // if(!billboardTexture.loadFromFile("ant-top-down.png"))
    // {
    //     std::cerr << "Failed to load billboard sprite\n";
    // }
    // billboardTexture.setSmooth(true);

    billBoardShader.setUniform("radius", conf::worldRadius);
    billBoardShader.setUniform("screenCenter", sf::Glsl::Vec2(center.x, center.y));
    billBoardShader.setUniform("zoom", 1.f);
    //  billBoardShader.setUniform("texture", billboardTexture);
    billBoardShader.setUniform("distance", 256.f);
    billBoardShader.setUniform("screenSize", conf::window_size);
    billBoardShader.setUniform("faceSize", 200.f);

    billBoards = sf::VertexArray(sf::Quads, 4 * 6 * 9); // just want to test one at each midpoint
    // std::cout << "Bill" << billBoards.getVertexCount() << std::endl;
    // std::cout << "Mid" << (middle.size()*4) << std::endl;

    std::vector<std::pair<int, int>> middle =
        {

            {0, 0},
            {8, 0},
            {15, 0},
            {0, 8},
            {0, 15},
            {8, 8},
            {8, 15},
            {15, 8},
            {15, 15}, // 0
            {16, 0},
            {24, 0},
            {31, 0},
            {16, 8},
            {24, 8},
            {31, 8},
            {16, 15},
            {24, 15},
            {31, 15}, // 3
            {32, 0},
            {40, 0},
            {47, 0},
            {32, 8},
            {40, 8},
            {47, 8},
            {32, 15},
            {40, 15},
            {47, 15}, // 1
            {48, 0},
            {56, 0},
            {63, 0},
            {48, 8},
            {56, 8},
            {63, 8},
            {48, 15},
            {56, 15},
            {63, 15}, // 2
            {0, 16},
            {0, 24},
            {0, 31},
            {8, 16},
            {8, 24},
            {8, 31},
            {15, 16},
            {15, 24},
            {15, 31}, // 4
            {0, 32},
            {0, 40},
            {0, 47},
            {8, 32},
            {8, 40},
            {8, 47},
            {15, 32},
            {15, 40},
            {15, 47} // 5
        };

    sf::Color colors[6] = {
        sf::Color::Yellow,  // idx 0
        sf::Color::Magenta, // 1
        sf::Color::Cyan,    // 2
        sf::Color::Red,     // 3
        sf::Color::Green,   // 4
        sf::Color::Black    // 5
    };

    //      CYAN(4)
    // BLUE(2)-RED(0)-YELLOW(3)-GREEN(1)
    //      MAGENTA(5)

    int idx = 0;
    for (auto mid : middle)
    {
        // std::cout << "Checking: " << mid.first << ", " << mid.second << std::endl;
        Cell *cell = globalat(mid.first, mid.second);
        if (!cell)
        {
            std::cout << "Not cell" << std::endl;
            continue;
        }
        //   std::cout << "Face: " << cell->face << std::endl;
        // World-to-face mapping
        int fx = mid.first / conf::worldSize;
        int fy = mid.second / conf::worldSize;

        // Face origin in world space
        int faceStartX = fx * conf::worldSize;
        int faceStartY = fy * conf::worldSize;

        // Coordinates within the face
        int localX = mid.first - faceStartX;
        int localY = mid.second - faceStartY;
        float step = 2.f / float(conf::worldSize);

        int quadIndex = (cell->face * conf::worldSize * conf::worldSize + localY * conf::worldSize + localX) * 4;
        int bIndex = idx * 4;
        //  std::cout << "Index: " << quadIndex << std::endl;
        if (quadIndex + 3 < vertices.getVertexCount())
        {
            float x0 = -1.f + localX * step;
            float y0 = -1.f + localY * step;
            float x1 = x0 + step;
            float y1 = y0 + step;

            std::array<sf::Vector2f, 4> corners = {
                sf::Vector2f(x0, y0),
                sf::Vector2f(x0, y1),
                sf::Vector2f(x1, y1),
                sf::Vector2f(x1, y0)};

            //    std::cout << "BINDEX: " << bIndex << std::endl;
            for (int k = 0; k < 4; ++k)
            {
                vertices[quadIndex + k].color = colors[idx / 9];

                billBoards[bIndex + k].position = corners[k];
                billBoards[bIndex + k].color = sf::Color(120, 120, 120);
                billBoards[bIndex + k].texCoords = sf::Vector2f(cell->face, 0);
            }
            //    billBoards[bIndex + 0].texCoords = sf::Vector2f(0.f, 0.f);
            //    billBoards[bIndex + 1].texCoords = sf::Vector2f(1.f, 0.f);
            //    billBoards[bIndex + 2].texCoords = sf::Vector2f(1.f, 1.f);
            //    billBoards[bIndex + 3].texCoords = sf::Vector2f(0.f, 1.f);
        }
        idx++;
    }
    ////////////////////////////

    std::cout << "world init fin" << std::endl;
}

Cell *World::globalat(int x, int y)
{
    const int totalWidth = conf::worldSize * 4;
    const int totalHeight = conf::worldSize * 3;

    // Wrap coordinates within cube map bounds
    int wrappedX = (x % totalWidth + totalWidth) % totalWidth;
    int wrappedY = (y % totalHeight + totalHeight) % totalHeight;

    // Determine which unfolded face (fx, fy) the coordinates are on
    int fx = wrappedX / conf::worldSize;
    int fy = wrappedY / conf::worldSize;

    // Compute local face coordinates
    int faceStartX = fx * conf::worldSize;
    int faceStartY = fy * conf::worldSize;

    int localX = wrappedX - faceStartX;
    int localY = wrappedY - faceStartY;

    int chunkX = localX / conf::chunkSize;
    int chunkY = localY / conf::chunkSize;

    // std::cout << "FX,FY: " << fx << ", " << fy << std::endl;
    // std::cout << "CX,CY: " << chunkX << ", " << chunkY << std::endl;
    auto it = conf::faceLookup.find({fx, fy});
    if (it == conf::faceLookup.end())
    {
        std::cerr << "Invalid face mapping for fx=" << fx << ", fy=" << fy << std::endl;
        return nullptr;
    }
    int face = it->second;

    // If invalid face mapping
    if (face < 0 || face > 5)
    {
        std::cerr << "Invalid face mapping for (x=" << x << ", y=" << y
                  << ") → (fx=" << fx << ", fy=" << fy << ")\n";
        return nullptr;
    }
    //   std::cout << "Looking for face " << face << ", chunk (" << chunkX << ", " << chunkY << ")" << std::endl;
    // Retrieve chunk
    auto chunk = chunkManager->getChunk(chunkX, chunkY, face);
    if (!chunk)
    {
        //   std::cerr << "Missing chunk at face " << face << ", chunk (" << chunkX << ", " << chunkY << ")\n";
        return nullptr;
    }

    int lx = localX % conf::chunkSize;
    int ly = localY % conf::chunkSize;
    // std::cout << "LocalXY: " << lx << ", " << ly << std::endl;
    return chunk->at(lx, ly);
}

Cell *World::cellat(int face, int x, int y)
{
        // Validate face index [0..5]
    if (face < 0 || face > 5) {
        std::cerr << "cellat: invalid face index " << face << std::endl;
        return nullptr;
    }

    // Validate local coordinates within face bounds
    if (x < 0 || x >= conf::worldSize || y < 0 || y >= conf::worldSize) {
        std::cerr << "cellat: coordinates out of bounds on face " << face
                  << ": (" << x << ", " << y << ")" << std::endl;
        return nullptr;
    }

    // Convert local coords to chunk coords and local offsets
    int chunkX = x / conf::chunkSize;
    int chunkY = y / conf::chunkSize;

    int localX = x % conf::chunkSize;
    int localY = y % conf::chunkSize;

    // Retrieve chunk
    auto chunk = chunkManager->getChunk(chunkX, chunkY, face);
    if (!chunk) {
        // Chunk missing, possibly not loaded
        // std::cerr << "cellat: chunk missing at face " << face
        //           << ", chunk (" << chunkX << ", " << chunkY << ")\n";
        return nullptr;
    }


    // Return the cell at local coords within chunk
    return chunk->at(localX, localY);
}

Chunk *World::getChunkAt(int x, int y)
{
    const int totalWidth = conf::worldSize * 4;
    const int totalHeight = conf::worldSize * 3;

    // Wrap coordinates within cube map bounds
    int wrappedX = (x % totalWidth + totalWidth) % totalWidth;
    int wrappedY = (y % totalHeight + totalHeight) % totalHeight;

    // Determine which unfolded face (fx, fy) the coordinates are on
    int fx = wrappedX / conf::worldSize;
    int fy = wrappedY / conf::worldSize;

    // Compute local face coordinates
    int faceStartX = fx * conf::worldSize;
    int faceStartY = fy * conf::worldSize;

    int localX = wrappedX - faceStartX;
    int localY = wrappedY - faceStartY;

    int chunkX = localX / conf::chunkSize;
    int chunkY = localY / conf::chunkSize;

    // std::cout << "FX,FY: " << fx << ", " << fy << std::endl;
    // std::cout << "CX,CY: " << chunkX << ", " << chunkY << std::endl;
    auto it = conf::faceLookup.find({fx, fy});
    if (it == conf::faceLookup.end())
    {
        std::cerr << "Invalid face mapping for fx=" << fx << ", fy=" << fy << std::endl;
        return nullptr;
    }
    int face = it->second;

    // If invalid face mapping
    if (face < 0 || face > 5)
    {
        std::cerr << "Invalid face mapping for (x=" << x << ", y=" << y
                  << ") → (fx=" << fx << ", fy=" << fy << ")\n";
        return nullptr;
    }
    //   std::cout << "Looking for face " << face << ", chunk (" << chunkX << ", " << chunkY << ")" << std::endl;
    // Retrieve chunk
    auto chunk = chunkManager->getChunk(chunkX, chunkY, face);
    if (!chunk)
    {
        //   std::cerr << "Missing chunk at face " << face << ", chunk (" << chunkX << ", " << chunkY << ")\n";
        return nullptr;
    }

    return chunk;
}



void World::update()
{
    // createACO(); //    
//    terrainShader.setUniform("screenCenter", sf::Glsl::Vec2(conf::window_size.x / 2 + getSliderValues("globePositionX"), conf::window_size.y / 2 + getSliderValues("globePositionY")));
   // std::cout << "X: " << conf::window_size.x / 2 + getSliderValues("globePositionX") << ", Y: " << conf::window_size.y / 2 + getSliderValues("globePositionY") << std::endl;
    terrainShader.setUniform("radius", conf::worldRadius + getSliderValues("globePositionY"));
}

void World::render(sf::RenderWindow &window)
{

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glClear(GL_DEPTH_BUFFER_BIT);
    drawTerrain(window);
   // drawEntities(window);

    glDisable(GL_CULL_FACE);

    //testing

    if(worldCBValues["selectMode"])
    {
        float frontZ = -conf::worldRadius - getSliderValues("globePositionY");
        float scale = conf::distance / (conf::distance + frontZ);
        projectedRadius = (conf::worldRadius) * zoom * scale * 0.65;

        outline = sf::CircleShape(projectedRadius);
        outline.setOrigin(projectedRadius, projectedRadius);
        outline.setPosition(sf::Vector2f(center.x, center.y - (conf::window_size.y * 0.03)));
        outline.setOutlineColor(sf::Color::Red);
        outline.setOutlineThickness(1.f);
        outline.setFillColor(sf::Color::Transparent);
        window.draw(outline);
    }



}

void World::updateView(float rotationX, float rotationY, float zoom, sf::RenderWindow &window)
{
    this->zoom = zoom;
    this->rotationX = rotationX;
    this->rotationY = rotationY;
}

void World::selectTiles(sf::Vector2i start, sf::Vector2i end)
{

   //if world is within start and end
   //convert to the grid system
    //xy /  
    if(isOverWorld(start) && isOverWorld(end))
    {
        std::cout << "Selecting tiles from " << start.x << "," << start.y << " to " << end.x << "," << end.y << std::endl;
        sf::Vector2f mouseScreen(static_cast<float>(start.x), static_cast<float>(start.y));

        // Step 1: Undo zoom (reverse of shader logic)
        sf::Vector2f unzoomed = center + (mouseScreen - center) / zoom;

        // Step 2: Undo screenCenter projection -> get 2D coords in perspective-projected space
        sf::Vector2f projected = unzoomed - center;

        // Step 3: Create view-space direction
        float scale = 1.f; // because we want ray direction, not scaled position
        Vec3 viewDir(projected.x, -projected.y, 1.0f); // +Z is forward in shader

        // Step 4: Normalize direction
        float len = std::sqrt(viewDir.x*viewDir.x + viewDir.y*viewDir.y + viewDir.z*viewDir.z);
        viewDir.x /= len;
        viewDir.y /= len;
        viewDir.z /= len;

        Vec3 inverseRot = rotateX(rotateY(viewDir, -rotationY), -rotationX);

        Vec3 camPos(0.f, 0.f, -1.f);

        Vec3 origin = camPos;
        Vec3 dir = inverseRot; // already normalized
        float radius = conf::worldRadius;

        // Ray-sphere intersection (origin + t*dir) where length == radius
        // Solve: (o + td)² = r² → quadratic: At² + Bt + C = 0
        float A = 1.0f; // dir is normalized
        float B = 2.0f * (origin.x * dir.x + origin.y * dir.y + origin.z * dir.z);
        float C = origin.x*origin.x + origin.y*origin.y + origin.z*origin.z - radius*radius;

        float discriminant = B*B - 4*A*C;
        if (discriminant < 0) {
            // No intersection — mouse is outside sphere
            return;
        }

        float t = (-B - std::sqrt(discriminant)) / (2.0f * A); // use closer intersection
        Vec3 hitPoint = Vec3(
            origin.x + t * dir.x,
            origin.y + t * dir.y,
            origin.z + t * dir.z
        );

        Vec3 p = hitPoint.normalized(); // ensure it's unit length

        float absX = std::abs(p.x);
        float absY = std::abs(p.y);
        float absZ = std::abs(p.z);

        // Determine cube face (major axis)
        int face;
        float u, v;

        if (absZ >= absX && absZ >= absY) {
            if (p.z > 0) { // front
                face = 0;
                u =  p.x / absZ;
                v =  p.y / absZ;
            } else {       // back
                face = 1;
                u = -p.x / absZ;
                v =  p.y / absZ;
            }
        }
        else if (absX >= absY) {
            if (p.x > 0) { // right
                face = 3;
                u = -p.z / absX;
                v =  p.y / absX;
            } else {       // left
                face = 2;
                u =  p.z / absX;
                v =  p.y / absX;
            }
        }
        else {
            if (p.y > 0) { // top
                face = 4;
                u =  p.x / absY;
                v = -p.z / absY;
            } else {       // bottom
                face = 5;
                u =  p.x / absY;
                v =  p.z / absY;
            }
        }

        float step = 2.f / float(conf::worldSize); 
        int i = std::clamp(int((u + 1.f) / step), 0, conf::worldSize - 1);
        int j = std::clamp(int((v + 1.f) / step), 0, conf::worldSize - 1);

        std::cout << "Clicked face " << std::to_string(face) << " on " << std::to_string(i) << ", " << std::to_string(j) << std::endl;

       // Cell* cell = cellat(face,i,j);
        int quadIndex = (face * conf::worldSize * conf::worldSize + i * conf::worldSize + j) * 4;
        //  std::cout << "Index: " << quadIndex << std::endl;
        if (quadIndex + 3 < vertices.getVertexCount())
        {
            float x0 = -1.f + i * step;
            float y0 = -1.f + j * step;
            float x1 = x0 + step;
            float y1 = y0 + step;

            std::array<sf::Vector2f, 4> corners = {
                sf::Vector2f(x0, y0),
                sf::Vector2f(x0, y1),
                sf::Vector2f(x1, y1),
                sf::Vector2f(x1, y0)};

            //    std::cout << "BINDEX: " << bIndex << std::endl;
            for (int k = 0; k < 4; ++k)
            {
                vertices[quadIndex + k].color = sf::Color::White;
            }
        }

    }
}

bool World::isOverWorld(sf::Vector2i mousePos)
{
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    sf::Vector2f circleCenter = outline.getPosition(); // Or same as `center`


    float distX = mousePosF.x - circleCenter.x;
    float distY = mousePosF.y - circleCenter.y;
    float distSquared = distX * distX + distY * distY;

    bool isInside = distSquared <= projectedRadius * projectedRadius;

    return isInside;
}

// helper render
void World::drawTerrain(sf::RenderWindow &window)
{

    terrainShader.setUniform("rotX", rotationX);
    terrainShader.setUniform("rotY", rotationY);

    terrainShader.setUniform("zoom", zoom);

    if (vertices.getVertexCount() == 0)
        std::cout << "No vertices to draw!\n";
    terrainShader.setUniform("renderSphere", true); // Sphere mode
    window.draw(vertices, &terrainShader);
    if (getCBValues("showFlatMap"))
    {
        terrainShader.setUniform("renderSphere", false); // plane mode/
        window.draw(vertices, &terrainShader);
    }
}

void World::drawEntities(sf::RenderWindow &window)
{
    billBoardShader.setUniform("rotX", rotationX);
    billBoardShader.setUniform("rotY", rotationY);
    billBoardShader.setUniform("zoom", zoom);
    if (billBoards.getVertexCount() == 0)
        std::cout << "No billboards to draw!\n";

    billBoardShader.setUniform("renderSphere", true); // Sphere mode
    window.draw(billBoards, &billBoardShader);
    if (getCBValues("showFlatMap"))
    {
        billBoardShader.setUniform("renderSphere", false); // plane mode
        window.draw(billBoards, &billBoardShader);
    }
}

void World::drawGrid(sf::RenderWindow &window)
{
}

World::~World()
{
    //  delete trackedVars;
}

int World::getWidth()
{
    return 0;
}

int World::getHeight()
{
    return 0;
}
