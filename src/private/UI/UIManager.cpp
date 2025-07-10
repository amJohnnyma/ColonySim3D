#include "UIManager.h"
#include <vector>
#include <memory>
UIManager::UIManager(World *world, sf::RenderWindow &window)
{
    width = conf::window_size.x;
    height = conf::window_size.y;
    midScreen = {
        (conf::window_size.x / conf::cellSize) / 2,
        (conf::window_size.y / conf::cellSize) / 2};

    gui *mainGUI = new gui(
        "Hello SFML-GUI!",                                                      // title
        sf::Vector2f(conf::window_size.x / 4, conf::window_size.y / 2),         // size
        sf::Vector2f(conf::window_size.x / 4 / 2, conf::window_size.y / 2 / 2), // position
        sf::Color(41, 39, 39, 255),                                             // background col
        sf::Color(217, 126, 95),                                                // top bar col
        sf::Color(46, 45, 46));                                                 // text col
    CheckBox *selectModeCB = new CheckBox(mainGUI->getGUIColor(), "SelectMode", sf::Color(255, 255, 255));
    CheckBox *showFlatMapCB = new CheckBox(mainGUI->getGUIColor(), "ShowFlatMap", sf::Color(255, 255, 255));
    CheckBox *showSphere = new CheckBox(mainGUI->getGUIColor(), "ShowSphere", sf::Color(255, 255, 255));
    //  CheckBox* checkBox = new CheckBox(mainGUI->getGUIColor(), "SelectMode", sf::Color(255, 255, 255));
    //   text* Text = new text(mainGUI->getGUI(), "This is Some Text!", sf::Color(255, 255, 255), window);
    Slider<int, 1>* radius = new Slider<int,1>(mainGUI->getGUIColor().getFillColor(), sf::Color(255, 255, 255), 200.0f, mainGUI->getFont(), "Radius", 0, 50);
    Slider<int, 1>* flatSize = new Slider<int,1>(mainGUI->getGUIColor().getFillColor(), sf::Color(255, 255, 255), 200.0f, mainGUI->getFont(), "FlatMapSize", 10, 900);

    Button *highlightWorld = new Button(mainGUI->getGUI(), mainGUI->getGUI(), 250.f, sf::Color(0,0,0), "highlightEntireWorld", "1");
    Button *resetWorld = new Button(mainGUI->getGUI(),mainGUI->getGUI(), 250.f,sf::Color(0,0,0), "resetEntireWorld", "2");

    Slider<int, 1>* xTest = new Slider<int,1>(mainGUI->getGUIColor().getFillColor(), sf::Color(255, 255, 255), 200.0f, mainGUI->getFont(), "xTest", 0, (conf::worldSize * 4));
    Slider<int, 1>* yTest = new Slider<int,1>(mainGUI->getGUIColor().getFillColor(), sf::Color(255, 255, 255), 200.0f, mainGUI->getFont(), "yTest", 0, (conf::worldSize*3));
    // guis.push_back({mainGUI, "main"});
    // checkboxes.push_back({checkBox, "main"});
    // texts.push_back({Text,"main"});
    // sliders.push_back({newSlider,"main"});
    addGUI("main", mainGUI);
    addCheckBox("selectMode", selectModeCB);
    addCheckBox("showFlatMap", showFlatMapCB);
    addCheckBox("showSphere", showSphere);
    addSlider("radius", radius);
    addSlider("flatSize", flatSize);
    addSlider("xTest", xTest);
    addSlider("yTest",yTest);
    // addCheckBox("selectMode", selectModeCB);


    //for testing purposes
    Slider<int, 1>* A = new Slider<int,1>(mainGUI->getGUIColor().getFillColor(), sf::Color(255, 255, 255), 600.0f, mainGUI->getFont(), "A", -100, 100, -1);
    addSlider("A",A);
    Slider<int, 1>* originZ = new Slider<int,1>(mainGUI->getGUIColor().getFillColor(), sf::Color(255, 255, 255), 600.0f, mainGUI->getFont(), "originZ", -500, 500, 256);
    addSlider("originZ",originZ);

    addButton("highlightWorld", highlightWorld);
    addButton("resetWorld", resetWorld);
}

UIManager::~UIManager()
{
    for (auto &[_, g] : guis)
        delete g;

    for (auto &[_, cb] : checkBoxes)
        delete cb;

    for (auto &[_, s] : sliders)
        delete s;

    for (auto &[_, b] : buttons)
        delete b;

}

void UIManager::update(sf::RenderWindow &window, sf::Event &event)
{
    for (auto &[_, g] : guis)
        g->UPDATE_GUI(window);
    
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        for(auto& [name, b] : buttons)
        {
            buttonMap[name] = b->isButtonClicked(window);
            
        }

    }
    
}
void UIManager::draw(sf::RenderWindow &window)
{
    int slot = 1;

    sf::View origView = window.getView();
    window.setView(window.getDefaultView());
    for (auto &[_, g] : guis)
    {
        g->DRAW_GUI(window);
        slot++;
    }

    for (auto &[name, cb] : checkBoxes)
    {
        cb->Draw(window, slot, guis["main"]->getGUI(), checkBoxMap[name]);
        slot++;
    }

    for (auto& [name, s] : sliders)
    {
    s->draw(window, slot,guis["main"]->getGUI(), &sliderMap[name]);
    slot++;

    }
    for (auto& [name, b] : buttons)
    {
    b->Draw(window,guis["main"]->getGUI(), guis["main"]->getGUIColor(), slot);
    slot++;
    }
    //   std::cout << std::to_string(checkBoxMap["selectMode"]) << std::endl;

    window.setView(origView);
}

void UIManager::setVisibilityForState(State gameState)
{

    switch (gameState)
    {
    case State::RUNNING:
    {

        break;
    }

    case State::PAUSED:
    {

        break;
    }

    case State::IDLE:
    {

        break;
    }

    case State::STOPPED:
    {

        break;
    }
    }
}
void UIManager::addGUI(const std::string &name, gui *g)
{
    guis[name] = g;
}
void UIManager::addCheckBox(const std::string &name, CheckBox *cb)
{
    checkBoxes[name] = cb;
}
void UIManager::addSlider(const std::string &name, Slider<int, 1> *s)
{
    sliders[name] = s;
}

void UIManager::addButton(const std::string &name, Button *b)
{
    buttons[name] = b;
}
