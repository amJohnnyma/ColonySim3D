#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include "../game/GlobalVars.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <map>
#include <set>

class Controller;
class World;
class UIManager;

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
    bool rmbDragging = false;
    sf::Vector2i lastMousePos;
    float zoom = 5.f;
    float rotationX = 0.f, rotationY = 0.f;
    float rotationSpeed = 0.001f;
    sf::Vector2i currentMousePos;
    UIManager* uiManager;

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
    void setUIManager(UIManager* ui) { uiManager = ui; }


    InputManager(World* world, sf::RenderWindow& window);
    ~InputManager();

};

#endif