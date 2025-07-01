#ifndef GFX_H
#define GFX_H

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>


struct window
{
    unsigned int windowWidth, windowHeight;
    int grids;
    sf::RenderWindow* wndw;
    sf::Font font;
    int frameCount = 0;
    float elapsedTime = 0.f;
    sf::Text fpsText;



    window(int w, int h) : windowWidth(w), windowHeight(h)
    {
        wndw = new sf::RenderWindow(sf::VideoMode({windowWidth,windowHeight}), "Window"/*, sf::Style::Fullscreen*/);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glClearDepth(1.f);
        glDepthFunc(GL_LESS);
      //  wndw->setFramerateLimit(conf::max_framerate);
      if (!font.loadFromFile("src/fonts/pixel.ttf")) {
        std::cerr << "Could not load font. Using default font." << std::endl; }

        fpsText.setFont(font);
        fpsText.setCharacterSize(24);
        fpsText.setFillColor(sf::Color::Red);
        fpsText.setPosition(10.f, 10.f);
    } 


    ~window()
    {
        wndw->close();
        delete wndw;
    }

    void updatefps(sf::Clock fpsClock)
    {
        elapsedTime += fpsClock.restart().asSeconds();
        frameCount++;
        if (elapsedTime >= 1.0f) {

            float fps = frameCount / elapsedTime;
            std::cout << "Fps: " << fps << std::endl;
            fpsText.setString("FPS: " + std::to_string((fps)));

            frameCount = 0;
            elapsedTime = 0.f;
        }
        wndw->draw(fpsText);
        wndw->display();
    }


};



#endif