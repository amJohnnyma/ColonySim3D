#include "UIManager.h"
#include <vector>
#include <memory>
UIManager::UIManager(World* world, sf::RenderWindow &window)
{       
    width = conf::window_size.x;
    height = conf::window_size.y;
    midScreen = {
        (conf::window_size.x / conf::cellSize) / 2,
        (conf::window_size.y / conf::cellSize) / 2
    };


    gui* mainGUI = new gui(
        "Hello SFML-GUI!", //title
        sf::Vector2f(conf::window_size.x / 4, conf::window_size.y/2), //size
        sf::Vector2f(conf::window_size.x / 4 / 2, conf::window_size.y/2 / 2), //position
        sf::Color(41, 39, 39, 255), //background col
        sf::Color(217, 126, 95), //top bar col
        sf::Color(46, 45, 46)); //text col
    CheckBox* checkBox = new CheckBox(mainGUI->getGUIColor(), "This is A CheckBox!", sf::Color(255, 255, 255));
    text* Text = new text(mainGUI->getGUI(), "This is Some Text!", sf::Color(255, 255, 255), window);
    Slider<int, 1>* newSlider = new Slider<int,1>(mainGUI->getGUIColor().getFillColor(), sf::Color(255, 255, 255), 200.0f, mainGUI->getFont(), "This is a Slider!", 10, 900);
   

    guis.push_back({mainGUI, "main"});
    checkboxes.push_back({checkBox, "main"});
    texts.push_back({Text,"main"});
    sliders.push_back({newSlider,"main"});
}

UIManager::~UIManager()
{
    for (auto& elem : guis)        
        delete elem.first;
}

void UIManager::update(sf::RenderWindow& window, sf::Event& event)
{    
    for(auto& elem: guis)
    {
        elem.first->UPDATE_GUI(window);       
        
    }

    for(auto& elem:buttons)
    {
        if(elem.first->isButtonClicked(window))
        {
            
        }
    }

    
}

void UIManager::draw(sf::RenderWindow& window)
{
    int slot = 1;

    sf::View origView = window.getView();
    window.setView(window.getDefaultView());
    for(auto& elem : guis)
    {
        elem.first->DRAW_GUI(window);
        for(auto& b: buttons)
        {
            if(elem.second == b.second)
            {
                b.first->Draw(window,elem.first->getGUI(),elem.first->getGUIColor(),slot);
                slot++;
            }
        }
        for(auto& b: checkboxes)
        {
            if(elem.second == b.second)
            {
                bool tt = true;
                b.first->Draw(window,slot,elem.first->getGUI(), tt);
                slot++;
            }

        }
        for(auto& b: listBoxes)
        {
            if(elem.second == b.second)
            {
              //  b.first->updateListBox(elem.first->getGUI(),slot,window);
               // slot++;
            }

        }
        for(auto& b: sliders)
        {
            if(elem.second == b.second)
            {
                b.first->draw(window,slot,elem.first->getGUI(),&slot);
                slot++;
            }
            
        }
        for(auto& b: texts)
        {
            if(elem.second == b.second)
            {
                b.first->Draw(window,slot,elem.first->getGUI());
                slot++;
            }

        }

    }   

    window.setView(origView);
}

void UIManager::setVisibilityForState(State gameState)
{

    switch (gameState){
    case State::RUNNING: {

        break;
    }

    case State::PAUSED: {

        break;
    }

    case State::IDLE: {

        break;
    }

    case State::STOPPED: {

        break;
    }
}

}
