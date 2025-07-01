#include "game/Game.h"
#include "game/GlobalVars.h"
#include <filesystem>

int main()
{
    Game::init(conf::window_size.x,conf::window_size.y, conf::worldSize,conf::worldSize);
    Game& game = Game::getInstance();
    game.run();
 //   game.fixedrun();
    Game::destroy();
//    game.fixedrun(); //fixed run to help with memory leaks


    return 0;
}

// #include <SFML/Graphics.hpp>
// #include "UI/sfml-gui/sfml-gui.hpp"
// int main()
// {
//     sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
//     gui NewGui("Hello SFML-GUI!", sf::Vector2f(600, 400), sf::Vector2f(300, 300), sf::Color(41, 39, 39, 128), sf::Color(217, 126, 95), sf::Color(46, 45, 46));
//     CheckBox checkBox(NewGui.getGUIColor(), "This is A CheckBox!", sf::Color(255, 255, 255));
//     text Text(NewGui.getGUI(), "This is Some Text!", sf::Color(255, 255, 255), window);
//     Slider<int, 1> newSlider(NewGui.getGUIColor().getFillColor(), sf::Color(255, 255, 255), 200.0f, NewGui.getFont(), "This is a Slider!", 10, 900);
//     bool testBoolean;
//     int testSliderValue;
//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//             if (event.type == sf::Event::Closed)
//                 window.close();        
//         window.clear(sf::Color(33, 32, 32));
//         NewGui.UPDATE_GUI(window);
//         NewGui.DRAW_GUI(window);
//         Text.Draw(window, 1, NewGui.getGUI());
//         checkBox.Draw(window, 2, NewGui.getGUI(), testBoolean);
//         newSlider.draw(window, 3, NewGui.getGUI(), &testSliderValue);
//         window.display();
//     }
//     return 0;
// }