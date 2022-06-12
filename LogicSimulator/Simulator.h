#pragma once
#include "Object.h"
#include "Pin.h"
#include "Wire.h"
#include "LogicElements.h"

class Simulator {
private:
    sf::RenderWindow* window;
    Object* objects;
public:
    Simulator() {
        window = new sf::RenderWindow(sf::VideoMode(1440, 900), "Logic Simulator");
        objects = nullptr;
    }
    ~Simulator() = default;
    void simulator_finish();
    void initial_sim();
    void start_sim();
    
};
