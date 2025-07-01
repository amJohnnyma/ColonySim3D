#ifndef GAME_H
#define GAME_H

#include "World.h"
#include "../structs/gfx.h"
#include "GlobalVars.h"
#include "../structs/enums.h"
#include "../controller/InputManager.h"

#include <thread>
#include <chrono>



class Game
{
    private:
        static Game* instance;
        World* world;
        window* wind;        
        State currentState = State::PAUSED;
        InputManager* inputManager;

    private:
        Game(int windowWidth, int windowHeight, int worldWidth, int worldHeight);
        ~Game();
        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;

    public:
        void fixedrun();
        void run();
        void renderFrame();
        void idleState();
        void runningState();
        void pausedState();
        void stoppedState();
        void handleEvent(Event event);
    public:
    static void init(int windowWidth, int windowHeight, int worldWidth, int worldHeight) {
        if (!instance)
            instance = new Game(windowWidth, windowHeight, worldWidth, worldHeight);
    }

    static Game& getInstance() {
        if (!instance) {
            throw std::runtime_error("Game not initialized. Call Game::init() first.");
        }
        return *instance;
    }

    static void destroy() {
        delete instance;
        instance = nullptr;
    }
    State getState() {return currentState;}
};

#endif