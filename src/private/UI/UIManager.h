#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include "../game/GlobalVars.h"
#include "../structs/enums.h"
#include "sfml-gui/sfml-gui.hpp"
#include <unordered_map>
#include <iostream>

class World;

class UIManager
{
    private:
            std::unordered_map<std::string, gui*> guis;
            std::unordered_map<std::string, CheckBox*> checkBoxes;
            std::unordered_map<std::string, Slider<int, 1>*> sliders;
            int width, height;
            std::pair<int,int> midScreen;
            std::unordered_map<std::string, bool> checkBoxMap; // Values for each checkbox "name" -> value
    public:
        UIManager(World* world, sf::RenderWindow& window);
        ~UIManager();
        void draw(sf::RenderWindow& window);
        void setVisibilityForState(State gameState);
        void update(sf::RenderWindow &window, sf::Event& event);
        void addGUI(const std::string& name, gui* g);
        void addCheckBox(const std::string& name, CheckBox* cb);
        void addSlider(const std::string& name, Slider<int, 1>* s);
        bool isCheckboxChecked(std::string name) {
            return checkBoxMap[name];
        }
        std::unordered_map<std::string, bool>& getCheckBoxMap() {
            return checkBoxMap;
        }


    /*
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        for (auto& elem : elements)
            elem->handleEvent(event, window);
    }*/

};

#endif