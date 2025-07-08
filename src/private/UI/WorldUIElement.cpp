/*#include "WorldUIElement.h"
#include <iostream>
#include <unordered_map>
WorldUIElement::WorldUIElement(World* world, int x, int y, int resolution, int radius, int width, int height, std::string function, std::string textArea)
 : UIElement(world), shape(nullptr), button(nullptr), updateFunc(nullptr)
{

   // std::cout << "rect" << std::endl;
    shape = new RoundedRectangle(x,y,radius,resolution,width, height);
    shape->setFillColor(sf::Color::White);
    this->function = function;
    this->args = FunctionArgs{};

    if (function != "null")
    {

        auto &map = getFunctionMap();
        auto it = map.find(function);
        if (it != map.end())
        {
            auto func = it->second;
            if (function.find("update") == std::string::npos)
            {
                button = new Button(
                    x * conf::cellSize,
                    y * conf::cellSize,
                    width,
                    height,
                    [this, func, world]()
                    {
                        func(world, this->args);
                    });
            }
            else
            {
                this->updateFunc = func;
            }
        }
    }
    if(textArea != "")
    {

        if (!font.loadFromFile("src/fonts/pixel.ttf")) {
            std::cerr << "Could not load font\n";
            return;
        }
        text.setFont(this->font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f{x * conf::cellSize, y*conf::cellSize});
        text.setString(textArea);
    }

}

WorldUIElement::~WorldUIElement()
{
    updateFunc = nullptr;
    args.element = std::nullopt; // reset optional safely
    if (shape) {
        delete shape;
        shape = nullptr;
    }
    if (button) {
        delete button;
        button = nullptr;
    }

}

void WorldUIElement::draw(sf::RenderWindow& window)
{
    // Draw in default view so it overlays UI properly
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());
    shape->draw(window);
    if(text.getString() != "")
    window.draw(text);
    window.setView(originalView);
}



void WorldUIElement::update(sf::RenderWindow& window, sf::Event &event)
{
    if(button && isVisible())
    {
        if(button->isClicked(window))
        {
            onClick(); //selected = true
        }
    }
    if(updateFunc && this->args.element)
    {
     //   std::cout << "updatefunc" << std::endl;
        updateFunc(this->world, this->args);
    }
}

void WorldUIElement::onClick()
{
    std::cout << "Rect clicked" << std::endl;
   //selected = true;
}

//needs to use player-controller such that enemy AI has access to the same commands
const std::unordered_map<std::string, std::function<void(World*, const FunctionArgs&)>>& WorldUIElement::getFunctionMap() {
   static const std::unordered_map<std::string, std::function<void(World*, const FunctionArgs&)>> functionMap = {
        {"testClick", [](World* w, const FunctionArgs&) {
            w->testClick();
        }},
        {"spawnEntity", [](World* w, const FunctionArgs& args) {
            if (args.name && args.count) {
                w->spawn(args.name.value(), args.count.value());
            }
        }},
        {"toggleSimState", [](World* w, const FunctionArgs& args){
            switch (Game::getInstance().getState())
            {
                case State::PAUSED:
                    Game::getInstance().handleEvent(Event::START);                    
                    break;
                case State::RUNNING:
                //dont change state
                   // Game::getInstance().handleEvent(Event::START);
                    break;
                case State::IDLE:
                    Game::getInstance().handleEvent(Event::START);
                    break;
                case State::STOPPED:
                    Game::getInstance().handleEvent(Event::STOP);
                    break;
            }
            w->toggleSimState();            
          //  std::cout << "Calling setColor on UIElement at " << *args.element << std::endl;
            bool running = w->isRunning();            
            if (args.element.has_value() && args.element.value() != nullptr) {
                UIElement* elem = args.element.value();
             //   std::cout << "Lambda called with element pointer: " << elem << std::endl;
                if (elem) {
                    std::string text = elem->getText();
                    elem->setColor(running ? sf::Color::Green : sf::Color::Red);
                    elem->setText(text == "S" ? "P" : "S");
                } else {
                    std::cout << "element pointer is null\n";
                }
            } else {
                std::cout << "args.element not set\n";
            }
        }},
        {"updateWorldStats", [](World* w, const FunctionArgs& args){
          //  std::cout << "Getting tracked vars.." << std::endl;
            TrackedVariables tv = *w->getWorldStats();
          //  std::cout << "Checking element validitry" <<std::endl;
            if (args.element.has_value() && args.element.value() != nullptr) {
                UIElement* elem = args.element.value();
            //    std::cout << "Making magic" << std::endl;
                if (elem) {
              //      std::cout << "Magic trying" << std::endl;
              std::string basesInfo = tv.getBasesInfo();
                    elem->setText("Stats: \n"
                        + basesInfo 
                         +"\nPF: " + std::to_string(tv.getPF())
                         +"\nHF: " + std::to_string(tv.getHF())
                        );
              //      std::cout << "Made magic" << std::endl;
                } else {
                    std::cout << "No magic today" << std::endl;
                  elem->setText("Stats:\nerror");
                }
            } else {
                std::cout << "args.element not set\n";
            }
        }},
        {"incrementPheremone", [](World* w, const FunctionArgs& args){
            w->changePF(0.1);
        }},
        {"decrementPheremone", [](World* w, const FunctionArgs& args){
            w->changePF(-0.1);
        }},
        {"incrementHeuristic", [](World* w, const FunctionArgs& args){
            w->changeHF(0.1);
        }},
        {"decrementHeuristic", [](World* w, const FunctionArgs& args){
            w->changeHF(-0.1);
        }},
        {"openBuildMenu", [](World* w, const FunctionArgs& args){

                if(args.name.has_value())
                {
                    w->getWorldStats()->setBuildingMode(true);
                    std::string name = args.name.value_or("Unnamed"); 
                    w->buildBuilding(name);            
                    w->getWorldStats()->setBuildingMode(false);
                }

            
        }},
        {"deleteBuilding", [](World* w, const FunctionArgs& args){

                if(args.name.has_value())
                {
                    w->getWorldStats()->setBuildingMode(true);
                    std::string name = args.name.value_or("Unnamed"); 
                    w->destroyBuilding("");            
                    w->getWorldStats()->setBuildingMode(false);
                }

            
        }},
        {"pauseGame", [](World* w, const FunctionArgs& args){
            switch (Game::getInstance().getState())
            {
                case State::PAUSED:
                    Game::getInstance().handleEvent(Event::UNPAUSE);                    
                    break;
                case State::RUNNING:
                    Game::getInstance().handleEvent(Event::PAUSE);
                    break;
                case State::IDLE:
                    Game::getInstance().handleEvent(Event::PAUSE);
                    break;
                case State::STOPPED:
                    Game::getInstance().handleEvent(Event::STOP);
                    break;
            }

        }},
        {"trackInput", [](World* w, const FunctionArgs& args){
            if (args.element.has_value() && args.element.value() != nullptr) {
                std::cout << "Selected but " << std::to_string(args.element.value()->isSelected()) << std::endl;
                args.element.value()->setSelected(true);
            }
        }}

    };
    return functionMap;
}

void WorldUIElement::setColor(sf::Color col)
{
    shape->setFillColor(col);
}

void WorldUIElement::setText(std::string text)
{
    this->text.setString(text);
}

std::string WorldUIElement::getText()
{
    return this->text.getString();
}

void WorldUIElement::setFontSize(int size)
{
    this->text.setCharacterSize(size);
}
void WorldUIElement::move(int x, int y) {
    shape->moveTo(x,y);
    text.setPosition(x*conf::cellSize, y*conf::cellSize);
    button->setPosition(x*conf::cellSize, y*conf::cellSize);
}
*/