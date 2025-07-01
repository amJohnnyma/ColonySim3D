#ifndef TRACKED_VARIABLES_H
#define TRACKED_VARIABLES_H

#include "../structs/worldObjects.h"


class TrackedVariables
{
    private:
        const std::vector<Cell*>* world = nullptr; //The entire world -> might be useful later
        int numAnts;
        std::vector<Cell*> selectedCells;
        bool inBuildingMode = false;
        sf::Vector2f viewCenter;
    public:
        TrackedVariables() {};
        ~TrackedVariables() {};
        void setWorld(const std::vector<Cell*>& w) {world = &w;}
        void setNumAnts(int na) {numAnts = na;}
        int getNumAnts() {return numAnts;}
    void setSelectedCells(std::vector<Cell*> cells)
    {
        selectedCells = cells;
    }
    std::vector<Cell*> getSelectedCells() { return selectedCells; }
    double getPF() {return conf::pF;}
    double getHF() { return conf::hF;}
    double isInBuildingMode() {return inBuildingMode;}
    void setBuildingMode(bool val) {inBuildingMode=val;}

    sf::Vector2f getViewCenter() const {
    return viewCenter;
    }

    void setViewCenter(const sf::Vector2f& center) {
        viewCenter = center;
    }

};

#endif
