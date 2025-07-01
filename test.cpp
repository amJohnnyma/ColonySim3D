#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <array>

struct Rectangle
{
    int x, y;   // grid coords
    float size; // size in grid space (uniform squares)

    Rectangle(int x_, int y_, float size_) : x(x_), y(y_), size(size_) {}
};

// Simple 3D vector struct
struct Vec3
{
    float x, y, z;

    // Constructor
    Vec3(float x_ = 0.f, float y_ = 0.f, float z_ = 0.f) : x(x_), y(y_), z(z_) {}

    // Operator - for vector subtraction
    Vec3 operator-(const Vec3 &other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    // Unary minus (invert)
    Vec3 operator-() const {
        return Vec3{-x, -y, -z};
    }


        // Addition operator overload
    Vec3 operator+(const Vec3& other) const {
        return Vec3{
            x + other.x,
            y + other.y,
            z + other.z
        };
    }

    // Optional: Addition assignment operator
    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

        // Scalar multiplication: Vec3 * float
    Vec3 operator*(float scalar) const {
        return Vec3{x * scalar, y * scalar, z * scalar};
    }

    // Optional scalar multiplication assignment
    Vec3& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }
};
// Dot product
inline float dot(const Vec3 &a, const Vec3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Cross product
inline Vec3 cross(const Vec3 &a, const Vec3 &b)
{
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

// Normalize vector
inline Vec3 normalize(const Vec3 &v)
{
    float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0.f)
        return Vec3(0.f, 0.f, 0.f);
    return Vec3(v.x / length, v.y / length, v.z / length);
}

// Convert spherical coords (u,v normalized) to 3D cartesian
Vec3 sphericalToCartesian(float u_norm, float v_norm, float radius)
{
    float theta = u_norm * 2.f * M_PI; // 0 to 2pi
    float phi = v_norm * M_PI;         // 0 to pi

    return {
        radius * std::sin(phi) * std::cos(theta),
        radius * std::cos(phi),
        radius * std::sin(phi) * std::sin(theta)};
}

// Rotate point around Y axis by angle (radians)
Vec3 rotateY(const Vec3 &p, float angle)
{
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    return {
        p.x * cosA + p.z * sinA,
        p.y,
        -p.x * sinA + p.z * cosA};
}

Vec3 rotateX(const Vec3 &v, float angle)
{
    float cosa = std::cos(angle);
    float sina = std::sin(angle);
    return {
        v.x,
        v.y * cosa - v.z * sina,
        v.y * sina + v.z * cosa};
}

Vec3 projectToSphere(float x, float y, float z, float radius)
{
    Vec3 v = {x, y, z};
    v = normalize(v);
    return {v.x * radius, v.y * radius, v.z * radius};
}


Vec3 applyRotation(const Vec3 &v, float rotX, float rotY)
{
    return rotateY(rotateX(v, rotX), rotY);
}
// Project 3D point to 2D screen coords (simple perspective)
sf::Vector2f projectTo2D(float x, float y, float z, sf::Vector2f center)
{
    float distance = 256.f;
    float scale = distance / (distance + z);
    return sf::Vector2f(center.x + x * scale, center.y - y * scale);
}

sf::Vector2f projectAndZoom(float x, float y, float z, sf::Vector2f center, float zoom)
{
    sf::Vector2f p = projectTo2D(x, y, z, center);
    // Move point relative to center, scale, then move back
    return center + (p - center) * zoom;
}

// Create rectangles in grid (u,v) coords with fixed size
std::vector<Rectangle> createRectangles(int width, int height, float size)
{
    std::vector<Rectangle> rects;
    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
            rects.emplace_back(x, y, size);
    return rects;
}

// Face index from 0 to 5
// i,j in [0, gridSize - 1]
// Returns 4 corners of the quad projected on sphere, CCW order
std::array<Vec3, 4> getCubeFaceQuadCorners(
    int faceIndex,
    int i, int j,
    int gridSize,
    float radius)
{
    float step = 2.0f / gridSize;
    float x0 = -1.0f + i * step;
    float x1 = x0 + step;
    float y0 = -1.0f + j * step;
    float y1 = y0 + step;

    Vec3 corners[4];

    switch (faceIndex)
    { //front, left, top
    case 0: // front  Z=+1
        corners[0] = {x0, y0, 1.f};
        corners[1] = {x0, y1, 1.f};
        corners[2] = {x1, y1, 1.f};
        corners[3] = {x1, y0, 1.f};
        break;
    case 1: // back  Z=-1
        corners[0] = {-x1, y0, -1.f};
        corners[1] = {-x0, y0, -1.f};
        corners[2] = {-x0, y1, -1.f};
        corners[3] = {-x1, y1, -1.f};
        break;
    case 2: // left  X=-1
        corners[0] = {-1.f, y0, x1};
        corners[1] = {-1.f, y0, x0};
        corners[2] = {-1.f, y1, x0};
        corners[3] = {-1.f, y1, x1};
        break;
    case 3: // right  X=+1
        corners[0] = {1.f, y1, -x0};
        corners[1] = {1.f, y1, -x1};
        corners[2] = {1.f, y0, -x1};
        corners[3] = {1.f, y0, -x0};
        break;
    case 4: // top   Y=+1
        corners[0] = {x0, 1.f, -y1};
        corners[1] = {x1, 1.f, -y1};
        corners[2] = {x1, 1.f, -y0};
        corners[3] = {x0, 1.f, -y0};
        break;
    case 5: // bottom  Y=-1
        corners[0] = {x0, -1.f, y1};
        corners[1] = {x1, -1.f, y1};
        corners[2] = {x1, -1.f, y0};
        corners[3] = {x0, -1.f, y0};
        break;
    }

    std::array<Vec3, 4> projected;
    for (int k = 0; k < 4; ++k)
    {
        projected[k] = projectToSphere(corners[k].x, corners[k].y, corners[k].z, radius);
    }

    return projected;
}
sf::VertexArray createSphereMesh(int gridSize, float radius, std::array<sf::Color,6> faceColors) {
    sf::VertexArray vertices(sf::Quads);
    vertices.resize(gridSize * gridSize * 6 * 4);

    int idx = 0;
    for (int face=0; face<6; ++face) {
        for (int i=0; i<gridSize; ++i) {
            for (int j=0; j<gridSize; ++j) {
                auto quad = getCubeFaceQuadCorners(face, i, j, gridSize, radius);

                // Compute face normal for lighting (cross product of edges)
                Vec3 edge1 = quad[1] - quad[0];
                Vec3 edge2 = quad[3] - quad[0];
                Vec3 normal = normalize(cross(edge1, edge2));

                for (int k=0; k<4; ++k) {
                    vertices[idx].position = sf::Vector2f(quad[k].x, quad[k].y); // z is handled in shader
                    // Pack normal in color to pass to shader as normalized floats
                    // (We'll pass real normal via vertex attribute in shader later)
                    vertices[idx].color = faceColors[face];
                    idx++;
                }
            }
        }
    }
    return vertices;
}
    struct RectToDraw
    {
        float depth;
        sf::VertexArray quad;
        sf::VertexArray outline;
    };

    inline float length(const Vec3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
void renderCubeSphere(
    sf::RenderWindow &window,
    int gridSize,
    float radius,
    float rotationX,
    float rotationY,
    sf::Vector2f screenCenter,
    float zoom)
{
// Vec3 baseLightDir = {0.f, 0.f, 1.f}; // Light comes from viewer's direction
// Vec3 lightDir = applyRotation(baseLightDir, -rotationX, -rotationY);
// lightDir = normalize(lightDir);
    Vec3 lightDir = normalize({0.f, 0.f, 1.f});
// Vec3 baseLightDir = {0.f, 0.f, 1.f}; // Light coming from the camera/view
// Vec3 lightDir = applyRotation(baseLightDir, -rotationX, -rotationY);
// lightDir = normalize(lightDir);
    sf::Color faceColors[6] = {
    sf::Color::Red,     // Front
    sf::Color::Green,   // Back
    sf::Color::Blue,    // Left
    sf::Color::Yellow,  // Right
    sf::Color::Cyan,    // Top
    sf::Color::Magenta  // Bottom
};

    auto lighting = [&](Vec3 pos, Vec3 normal, sf::Color baseColor) -> sf::Color {
    float lightIntensity = std::max(0.f, dot(normal, lightDir));

    // Measure how far from center the point is on the sphere surface
    float dist = length(pos); // should be ~1.0 for points on surface

    // Get radial distance from center of screen (use only x and y to control hemisphere fade)
    float radialFalloff = length({pos.x, pos.y, 0.f}) / radius;

    // Apply stronger falloff — you can tweak the exponent (try 2.0, 3.0, etc.)
    float falloff = std::pow(1.0f - radialFalloff, 3.0f); // cube falloff for stronger effect
    falloff = std::clamp(falloff, 0.f, 1.f);

    float ambient = 0.2f;
    float finalIntensity = lightIntensity * falloff * 0.8f + ambient;

    return sf::Color(
        static_cast<sf::Uint8>(baseColor.r * finalIntensity),
        static_cast<sf::Uint8>(baseColor.g * finalIntensity),
        static_cast<sf::Uint8>(baseColor.b * finalIntensity)
    );
};


    std::vector<RectToDraw> drawList;

    const sf::View &view = window.getView();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    sf::FloatRect viewRectWorld(
        viewCenter.x - viewSize.x / 2.f,
        viewCenter.y - viewSize.y / 2.f,
        viewSize.x,
        viewSize.y);

    float margin = 20.f;
    viewRectWorld.left -= margin;
    viewRectWorld.top -= margin;
    viewRectWorld.width += 2 * margin;
    viewRectWorld.height += 2 * margin;

    for (int face = 0; face < 6; ++face)
    {
        for (int i = 0; i < gridSize; ++i)
        {
            for (int j = 0; j < gridSize; ++j)
            {
                auto quadCorners = getCubeFaceQuadCorners(face, i, j, gridSize, radius);


                // Apply rotation
                Vec3 c0 = applyRotation(quadCorners[0], rotationX, rotationY);
                Vec3 c1 = applyRotation(quadCorners[1], rotationX, rotationY);
                Vec3 c2 = applyRotation(quadCorners[2], rotationX, rotationY);
                Vec3 c3 = applyRotation(quadCorners[3], rotationX, rotationY);

                Vec3 edge1 = c1 - c0;
                Vec3 edge2 = c3 - c0;
                Vec3 normal = cross(edge1, edge2);
                normal = normalize(normal);


                float dotProd = dot(normal, lightDir);
                if (dotProd < 0)
                    continue; // backface cull

                float depth = (c0.z + c1.z + c2.z + c3.z) / 4.f;

                sf::Vector2f p0 = projectAndZoom(c0.x, c0.y, c0.z, screenCenter, zoom);
                sf::Vector2f p1 = projectAndZoom(c1.x, c1.y, c1.z, screenCenter, zoom);
                sf::Vector2f p2 = projectAndZoom(c2.x, c2.y, c2.z, screenCenter, zoom);
                sf::Vector2f p3 = projectAndZoom(c3.x, c3.y, c3.z, screenCenter, zoom);

                // Culling
                bool visible =
                    viewRectWorld.contains(p0) ||
                    viewRectWorld.contains(p1) ||
                    viewRectWorld.contains(p2) ||
                    viewRectWorld.contains(p3);

                if (!visible)
                    continue;

                sf::VertexArray quad(sf::Quads, 4);
                quad[0].position = p0;
                quad[1].position = p1;
                quad[2].position = p2;
                quad[3].position = p3;

                sf::Color baseColor = faceColors[face];

                quad[0].color = lighting(c0, normal,  baseColor);
                quad[1].color = lighting(c1, normal,  baseColor);
                quad[2].color = lighting(c2, normal,  baseColor);
                quad[3].color = lighting(c3, normal,  baseColor);

                sf::VertexArray lines(sf::Lines, 8);
                lines[0].position = p0;
                lines[0].color = sf::Color::Black;
                lines[1].position = p1;
                lines[1].color = sf::Color::Black;

                lines[2].position = p1;
                lines[2].color = sf::Color::Black;
                lines[3].position = p2;
                lines[3].color = sf::Color::Black;

                lines[4].position = p2;
                lines[4].color = sf::Color::Black;
                lines[5].position = p3;
                lines[5].color = sf::Color::Black;

                lines[6].position = p3;
                lines[6].color = sf::Color::Black;
                lines[7].position = p0;
                lines[7].color = sf::Color::Black;

                drawList.push_back({depth, quad, lines});
            }
        }
    }

    // Sort farthest to nearest (descending depth)
    std::sort(drawList.begin(), drawList.end(),
              [](const RectToDraw &a, const RectToDraw &b)
              {
                  return a.depth > b.depth;
              });

    // Draw in batches
    const int batchSize = 100;
    int total = (int)drawList.size();

    for (int start = 0; start < total; start += batchSize)
    {
        sf::VertexArray batchQuads(sf::Quads);
        sf::VertexArray batchOutlines(sf::Lines);

        int end = std::min(start + batchSize, total);
        for (int i = start; i < end; ++i)
        {
            const auto &item = drawList[i];
            for (int v = 0; v < 4; ++v)
                batchQuads.append(item.quad[v]);
            for (int v = 0; v < 8; ++v)
                batchOutlines.append(item.outline[v]);
        }

        window.draw(batchQuads);
        window.draw(batchOutlines);
    }
    std::cout << "Total: " << total << std::endl;
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Sphere Rectangles");
    window.setFramerateLimit(60);

    int gridWidth = 200;
    int gridHeight = 200;
    float rectSize = 10.f;
    float sphereRadius = 200.f;
    sf::Vector2f center(400.f, 400.f);

    //std::vector<Rectangle> rectangles = createRectangles(gridWidth, gridHeight, rectSize);

    float rotation = 0.f;
    float zoom = 1.f; // zoom factor, 1.0 = no zoom
    sf::Clock clock;

    bool dragging = false;
    sf::Vector2i lastMousePos;
    float rotationSpeed = 0.001f; // control sensitivity

    float rotationY = 0.f; // horizontal (left/right)
    float rotationX = 0.f; // vertical (up/down)

    // Add near the top of main(), before window loop:
sf::Font font;
if (!font.loadFromFile("pixel.ttf")) {
    std::cerr << "Failed to load font\n";
    return -1;
}
sf::Text fpsText;
fpsText.setFont(font);
fpsText.setCharacterSize(26);
fpsText.setFillColor(sf::Color::White);
fpsText.setPosition(10.f, 10.f);

// Variables for FPS calculation
sf::Clock fpsClock;
int frameCount = 0;
float fps = 0.f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Start dragging
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                dragging = true;
                lastMousePos = sf::Mouse::getPosition(window);
            }

            // Stop dragging
            if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                dragging = false;
            }

            // Handle zooming
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0)
                    zoom *= 1.1f;
                else
                    zoom /= 1.1f;

                zoom = std::clamp(zoom, 0.1f, 10.f);
            }
        }

        float dt = clock.restart().asSeconds();

        // If dragging, update rotation based on horizontal mouse movement
        if (dragging)
        {
            sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
            int dx = currentMousePos.x - lastMousePos.x;
            int dy = currentMousePos.y - lastMousePos.y;

            float deltaX = dx * rotationSpeed;
            float deltaY = dy * rotationSpeed;

            // Compose the rotation with the current rotation
            // Apply rotation deltas as incremental transforms
            rotationY += deltaX;
            rotationX += deltaY;

            // Clamp X rotation to avoid flipping
            float maxTilt = M_PI / 2 - 0.01f;
            rotationX = std::clamp(rotationX, -maxTilt, maxTilt);

            lastMousePos = currentMousePos;
        }

            // FPS calculation
    frameCount++;
    if (fpsClock.getElapsedTime().asSeconds() >= 1.f) {
        fps = frameCount / fpsClock.getElapsedTime().asSeconds();
        fpsText.setString("FPS: " + std::to_string(int(fps)));
        fpsClock.restart();
        frameCount = 0;
    }
        window.clear(sf::Color::Black);
        // renderRectanglesAsSphere(window, rectangles, gridWidth, gridHeight,
        //                          sphereRadius, rotationX, rotationY, center, zoom);
        renderCubeSphere(
            window,
            50, // gridSize: number of subdivisions per cube face axis
            sphereRadius,
            rotationX, // in radians
            rotationY, // in radians
            sf::Vector2f(400.f, 400.f),
            zoom);
            window.draw(fpsText);
        window.display();
    }

    return 0;
}
    
