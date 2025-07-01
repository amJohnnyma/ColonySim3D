#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm> 
#include <random>
#include <array>
std::random_device rd;
std::mt19937 gen(rd());
void rebuildVertices(sf::VertexArray& vertices, int gridSize) {
    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(6 * gridSize * gridSize * 4);

    for (int face = 0; face < 6; ++face) {
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                int quadIndex = (face * gridSize * gridSize + i * gridSize + j) * 4;
                float step = 2.f / float(gridSize);
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

                std::uniform_int_distribution<> dis(0, (6*gridSize*gridSize*4) - 1);
                int randomQuad = dis(gen);

                int startVertex = randomQuad;

                // Generate a random color
                std::uniform_int_distribution<> colorDis(0, 255);
                sf::Color randomColor(colorDis(gen), colorDis(gen), colorDis(gen));


                for (int k = 0; k < 4; ++k) {
                    vertices[quadIndex + k].position = corners[k];
                    vertices[quadIndex + k].color = randomColor; // default
                    vertices[quadIndex + k].texCoords = sf::Vector2f(face, 0);  // Pass face index

                    }
            }
        }
    }
}

int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 800), "Shader Cube Sphere");
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);
    glDepthFunc(GL_LESS);
   // window.setFramerateLimit(60);

    int gridSize = 1;
    float radius = 200.f;

    sf::Shader shader;
    if (!shader.loadFromFile("sphere.vert", "sphere.frag"))
    {
        std::cerr << "Failed to load shaders\n";
        return -1;
    }

    shader.setUniform("radius", radius);
    shader.setUniform("screenCenter", sf::Glsl::Vec2(400.f, 400.f));
    shader.setUniform("zoom", 1.f);

    // Total vertices for quads
    int totalVertices = 6 * gridSize * gridSize * 4;
    sf::VertexArray vertices(sf::Quads, totalVertices);
    for (int face = 0; face < 6; ++face) {
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                int quadIndex = (face * gridSize * gridSize + i * gridSize + j) * 4;
                float step = 2.f / float(gridSize);
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

                    std::uniform_int_distribution<> dis(0, totalVertices - 1);
                    int randomQuad = dis(gen);

                    int startVertex = randomQuad;

                    // Generate a random color
                    std::uniform_int_distribution<> colorDis(0, 255);
                    sf::Color randomColor(colorDis(gen), colorDis(gen), colorDis(gen));


                    for (int k = 0; k < 4; ++k) {
                        vertices[quadIndex + k].position = corners[k];
                        vertices[quadIndex + k].color = randomColor; // default
                        vertices[quadIndex + k].texCoords = sf::Vector2f(face, 0);  // Pass face index

                    }
            }
        }
    }


    // Prepare line vertices for grid lines (currently unused)
    int totalQuads = 6 * gridSize * gridSize * 4;
    int totalLineVertices = totalQuads * 8; // 4 edges * 2 vertices each
    sf::VertexArray lineVertices(sf::Lines, totalLineVertices);


    float rotationX = 0.f;
    float rotationY = 0.f;
    float zoom = 1.f;

    bool dragging = false;
    sf::Vector2i lastMousePos;
    float rotationSpeed = 0.0005f;

    // FPS tracking
    sf::Font font;
    if (!font.loadFromFile("pixel.ttf"))
    {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(26);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(10.f, 10.f);

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

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                dragging = true;
                lastMousePos = sf::Mouse::getPosition(window);
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                dragging = false;
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0)
                    zoom *= 1.1f;
                else
                    zoom /= 1.1f;
                zoom = std::clamp(zoom, 0.1f, 10.f);
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space) {
                    gridSize *= 2;
                    gridSize = std::min(gridSize, 2048);
                    totalQuads = 6 * gridSize * gridSize * 4;
                    rebuildVertices(vertices, gridSize);  // call this
                    std::cout << "Grid size doubled to " << gridSize << std::endl;
                }
                else if (event.key.code == sf::Keyboard::C)
                {
                    
                    std::uniform_int_distribution<> dis(0, totalQuads - 1);
                    int randomQuad = dis(gen);

                    int startVertex = randomQuad;

                    // Generate a random color
                    std::uniform_int_distribution<> colorDis(0, 255);
                    sf::Color randomColor(colorDis(gen), colorDis(gen), colorDis(gen));

                    for (int i = 0; i < 4; ++i)
                    {
                        vertices[startVertex + i].color = randomColor;
                    }
                    std::cout << "Changed color of quad " << randomQuad << " to (" 
                            << (int)randomColor.r << ", " << (int)randomColor.g << ", " << (int)randomColor.b << ")" << std::endl;
                   // rebuildVertices(vertices,gridSize);
                }
            }
        }

        if (dragging)
        {
            sf::Vector2i cur = sf::Mouse::getPosition(window);
            int dx = cur.x - lastMousePos.x;
            int dy = cur.y - lastMousePos.y;

            rotationY += dx * rotationSpeed;
            rotationX += dy * rotationSpeed;
            //rotationX = std::clamp(rotationX, -3.14f / 2.f, 3.14f / 2.f);
            lastMousePos = cur;
        }

        shader.setUniform("rotX", rotationX);
        shader.setUniform("rotY", rotationY);
        shader.setUniform("zoom", zoom);


        frameCount++;
        float elapsed = fpsClock.getElapsedTime().asSeconds();
        if (elapsed >= 1.f)
        {
            fps = frameCount / elapsed;
            fpsText.setString("FPS: " + std::to_string(int(fps)));
            fpsClock.restart();
            frameCount = 0;
        }


        window.clear(sf::Color::Black);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glClear(GL_DEPTH_BUFFER_BIT);

        if (vertices.getVertexCount() == 0)
            std::cout << "No vertices to draw!\n";
    //shader.setUniform("drawLines", false);
    //  window.draw(vertices);
        window.draw(vertices, &shader);

     //   shader.setUniform("drawLines", true);
      //  window.draw(lineVertices, &shader);
        glDisable(GL_CULL_FACE);
        window.draw(fpsText);
        window.display();
    }

    return 0;
}
