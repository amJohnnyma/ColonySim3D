#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>

struct keyboardInput
{
    std::array<bool, sf::Keyboard::KeyCount> keyStates {};

    keyboardInput()
    {

    }

    void update(const sf::Event &e)
    {
        if(e.type == sf::Event::KeyPressed)
        {
            if(e.key.code >= 0 && e.key.code < sf::Keyboard::KeyCount)
                keyStates[e.key.code] = true;
        }
        else if (e.type == sf::Event::KeyReleased)
        {
            if(e.key.code >= 0 && e.key.code < sf::Keyboard::KeyCount)
                keyStates[e.key.code] = false;
        }
    }

    bool isKeyPressed(sf::Keyboard::Key key) const
    {
        return keyStates[key];
    }
};

struct mouseInput
{
    sf::Event event; 
    bool leftClick = false;
    bool rightClick = false;
    bool scrollIn = false;
    bool scrollOut = false;

    mouseInput()
    {

    }

    void update(const sf::Event &e)
    {
        event = e;

        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
        {
            leftClick = true;
        }
        else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
        {
            leftClick = false;
        }
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Right)
        {
            rightClick = true;
        }
        else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Right)
        {
            rightClick = false;
        }
    }

};

struct inputs
{
    keyboardInput ki;
    mouseInput mi;

    void update(const sf::Event &e)
    {
        ki.update(e);
     //   mi.update(e);
    }
};



#endif