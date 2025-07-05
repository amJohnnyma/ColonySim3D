#include "InputManager.h"
#include "../game/World.h"
#include "../UI/UIManager.h"

 void InputManager::processEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::KeyPressed) {
        if (!keyStates[event.key.code]) {
            keysPressedOnce.insert(event.key.code);
        }
        keyStates[event.key.code] = true;
    } else if (event.type == sf::Event::KeyReleased) {
        keyStates[event.key.code] = false;
        keysPressedOnce.erase(event.key.code);
    }
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

     // If dragging, update rotation based on horizontal mouse movement
    if (dragging)
    {
        sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
        int dx = currentMousePos.x - lastMousePos.x;
        int dy = currentMousePos.y - lastMousePos.y;

        // Compose the rotation with the current rotation
        // Apply rotation deltas as incremental transforms
        rotationX -= dy * rotationSpeed;  // inverted for "grab" feel
        rotationY -= dx * rotationSpeed;
        if (rotationY > 2 * M_PI) rotationY -= 2 * M_PI;
        if (rotationY < 0) rotationY += 2 * M_PI;

        lastMousePos = currentMousePos;
    }
    world->updateView(rotationX,rotationY,zoom,window);

}


void InputManager::update(sf::RenderWindow& window) {
    keysPressedOnce.clear();
    if(uiManager->isCheckboxChecked("selectMode"))
    {
        std::cout << "Select mode is active!" << std::endl;
    }
}

void InputManager::draw(sf::RenderWindow& window) {
}

bool InputManager::isKeyHeld(sf::Keyboard::Key key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

bool InputManager::isKeyPressedOnce(sf::Keyboard::Key key) {
    return keysPressedOnce.find(key) != keysPressedOnce.end();
}

bool InputManager::isMouseHeld(sf::Mouse::Button button) const {
    auto it = mouseStates.find(button);
    return it != mouseStates.end() && it->second;
}


InputManager::InputManager(World *world, sf::RenderWindow& window)
{
    view = window.getDefaultView();
    this->world = world;
}

InputManager::~InputManager()
{
}
