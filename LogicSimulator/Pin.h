#pragma once
#define MAX_CONNECTIONS 300
#include "Object.h"
class Pin {
private:
    enum  pinType { INPUT, OUTPUT };
    enum  pinState { LOW, HIGH, HIGHZ };

    pinType type;
    pinState state;


    bool isSrc[MAX_CONNECTIONS];
    Pin* connectedTo[MAX_CONNECTIONS];
    Object* wires[MAX_CONNECTIONS];
    int numConnections;
    sf::Vector2f pos;
public:
    Pin()
    {
        state = HIGHZ;
        numConnections = 0;
    };
    ~Pin() = default;
    void set_position(float x, float y); // sets pin position
    void set_pin_state(int a); // sets pin position by looking at a
    void set_type(int a); // sets pins state by looking at a
    Object* get_wire(int idx) {
        return wires[idx];
    }
    void incr_numConnections();
    void dec_numConnections();
    void decrease_specs(int i);
    void delete_last();
    int get_numConnections();
    void state_toggle();
    void set_isSrc(bool a);
    void set_wire(Object* wr);
    void set_connectedTo(Pin* pn);
    pinState get_state() {
        return state;
    }
    pinType get_type() {
        return type;
    }
    sf::Vector2f get_pin_position();
};