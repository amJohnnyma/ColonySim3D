#include "InputManager.h"
#include "../game/World.h"
#include "../UI/UIManager.h"

void InputManager::processEvent(const sf::Event &event, sf::RenderWindow &window)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (!keyStates[event.key.code])
        {
            keysPressedOnce.insert(event.key.code);
        }
        keyStates[event.key.code] = true;
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        keyStates[event.key.code] = false;
        keysPressedOnce.erase(event.key.code);
    }


        // Start dragging
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Right)
    {
        rmbDragging = true;
        lastMousePos = sf::Mouse::getPosition(window);
    }

    // Stop dragging
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Right)
    {
        rmbDragging = false;
    }

    // Handle zooming
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.mouseWheelScroll.delta > 0)
            zoom *= 1.1f;
        else
            zoom /= 1.1f;

        zoom = std::clamp(zoom, 1.f, 10.f);
    }



        // If dragging, update rotation based on horizontal mouse movement
    if (rmbDragging)
    {
        sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
        int dx = currentMousePos.x - lastMousePos.x;
        int dy = currentMousePos.y - lastMousePos.y;

        // Compose the rotation with the current rotation
        // Apply rotation deltas as incremental transforms
        rotationX -= dy * rotationSpeed; // inverted for "grab" feel
        bool upsideDown = std::sin(rotationX) > 0;
        float effectiveDY = upsideDown ? -dx : dx;
        rotationY -= effectiveDY * rotationSpeed;

        rotationX = std::fmod(rotationX + 2 * M_PI,2 * M_PI);
        rotationY = std::fmod(rotationY + 2 * M_PI,2 * M_PI);

        lastMousePos = currentMousePos;
    }





    if (uiManager->isCheckboxChecked("selectMode"))
    {
        world->hoverEffect(sf::Mouse::getPosition(window));
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            mouseStart = sf::Mouse::getPosition(window);
            selecting = true;
        }
        else if (event.type == sf::Event::MouseButtonReleased &&
                 event.mouseButton.button == sf::Mouse::Left)
        {
            mouseEnd = sf::Mouse::getPosition(window);
            selecting = false;
            world->selectTiles(mouseStart, mouseEnd);
        }
    }
    world->updateView(rotationX, rotationY, zoom, window);
}

void InputManager::update(sf::RenderWindow &window)
{
    keysPressedOnce.clear();
    for (auto &[name, val] : uiManager->getCheckBoxMap())
    {
        world->updateCBValues(name, val);
    }
    for (auto &[name, val] : uiManager->getSliderMap())
    {
        world->updateSliderValues(name, val);
    }
}

void InputManager::draw(sf::RenderWindow &window)
{
}

bool InputManager::isKeyHeld(sf::Keyboard::Key key) const
{
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

bool InputManager::isKeyPressedOnce(sf::Keyboard::Key key)
{
    return keysPressedOnce.find(key) != keysPressedOnce.end();
}

bool InputManager::isMouseHeld(sf::Mouse::Button button) const
{
    auto it = mouseStates.find(button);
    return it != mouseStates.end() && it->second;
}

InputManager::InputManager(World *world, sf::RenderWindow &window)
{
    view = window.getDefaultView();
    this->world = world;
}

InputManager::~InputManager()
{
}
