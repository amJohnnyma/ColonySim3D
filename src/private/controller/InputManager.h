#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include "../game/GlobalVars.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <map>
#include <set>

class Controller;
class World;
class InputManager {
private:
    std::map<sf::Keyboard::Key, bool> keyStates;
    std::set<sf::Keyboard::Key> keysPressedOnce;
    std::map<sf::Mouse::Button, bool> mouseStates;

    sf::Vector2i mouseStart;
    sf::Vector2i mouseEnd;
    bool selecting = false;
    float currentZoom = 1.0f;
    float zoomFactor = 1.1f;
    sf::View view;
    World* world;
    bool dragging = false;
    sf::Vector2i lastMousePos;
    float zoom = 1.f;
    float rotationX, rotationY;
    float rotationSpeed = 0.0005f;
    sf::Vector2i currentMousePos;

public:
    void processEvent(const sf::Event& event, sf::RenderWindow& window);
    void update(sf::RenderWindow& window);

    bool isKeyHeld(sf::Keyboard::Key key) const;
    bool isKeyPressedOnce(sf::Keyboard::Key key);
    bool isMouseHeld(sf::Mouse::Button button) const;
    void draw(sf::RenderWindow& window);
    float getCurrentZoom(){return currentZoom;}
    sf::View& getView() { return view; }
    void setView(const sf::View& v) { view = v; }


    InputManager(World* world, sf::RenderWindow& window);
    ~InputManager();

};

#endif