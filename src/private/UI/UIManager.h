#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include "../game/GlobalVars.h"
#include "../structs/enums.h"
#include "sfml-gui/sfml-gui.hpp"


class World;

class UIManager
{
    private:
            std::vector<std::pair<gui*, std::string>> guis; //actual object, group
            std::vector<std::pair<CheckBox*, std::string>> checkboxes;
            std::vector<std::pair<Button*,std::string>> buttons;
            std::vector<std::pair<ListBox*,std::string>> listBoxes;
            std::vector<std::pair<text*,std::string>> texts;
            std::vector<std::pair<Slider<int>*, std::string>> sliders;
            int width, height;
            std::pair<int,int> midScreen;
    public:
        UIManager(World* world, sf::RenderWindow& window);
        ~UIManager();
        void draw(sf::RenderWindow& window);
        void setVisibilityForState(State gameState);
        void update(sf::RenderWindow &window, sf::Event& event);
        void addElement(gui* elem, std::string group) {
            guis.push_back({elem,group});
        }

    /*
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        for (auto& elem : elements)
            elem->handleEvent(event, window);
    }*/

};

#endif