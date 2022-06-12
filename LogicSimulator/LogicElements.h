#pragma once

#include "Object.h"
#include "Pin.h"
class LogicElement : public Object {
protected: // these are protected to reach the objects easier in the gates 
    enum  pinType { INPUT, OUTPUT };
    enum  pinState { LOW, HIGH, HIGHZ };
    Pin pins[4];
    int numPins;
public:
    using Object::Object;  // uses the Objects constructors
    void object_clone(Object* obj); // object clone method for which we are creating the new dynamic objects
    virtual void create_texture(std::string gate_name) { // this is for initial objects (ones that are locked), linking the png files to the textures
        if (!textures[0].loadFromFile("../assets/" + gate_name + ".png"))
        {
            std::cout << "Failed to load texture" + gate_name + ".png!" << std::endl;
        }
    }
    void set_pins_wire_positions(sf::Vector2f* old_pins); // this is checking when the logic element is moving, it checks the position of every pin with wires connected to it and changing the position of wire.
    int get_numPins(); // gets the NumPins from the logicelement and returns it
    void draw_object(); // draws the logic element
    void del_obj_list_wires(Object* a); // deletes the specified object pointed by the a, while deleting the logicelement it deletes the wires it is connected to also updates its own pin attributes and other pin that was connected to otherend of wire
    int get_Connections(int idx); // returns the number of connections of the pins[idx]
    sf::Vector2f get_pin_position(int i); // returns the position of the pins[idx]
    void* get_pinaddr(int idx);  // returns the pin address of pin[idx], it returns as void* because in the object class we had problems returning Pin*. In C terminology this is done heavily. We typecast to Pin* when we call this function.
};

class AndGate : public LogicElement {

public:
    using LogicElement::LogicElement; // uses the LogicElements constructors, also while creating a new AndGate its pins are in HIGHZ mode (done in the Pin class)

    void create_initial_objects(); // calls the create_texture from the logicelement function with its own png file, and places the initial (locked) andgate object to its own position, sets the origin to the middle point of texture and sets the numPins 
    void set_pin_positions();      // sets the pin positions relative to the origin, so positions can be easily changed while creating or moving object.
    void set_pin_attributes();     // sets the pin attributes as input,output. pins[0] and pins[1] input, pins[2] output.
    Object* create_obj_addr();     // creates a new address for AndGate, used while creating a new dynamic object in the workbench.
    void change_lgc_state();       // changes the output pin regarding to the state of input pins (pins[2] = pins[0] & pins[1])
};

class OrGate : public LogicElement {  // this class has the same attributes as AndGate only minor things change such as while creating the initial object its initial position is different
public:                               // it gives OrGate address in the create_obj addr, in logic state changing (pins[2] = pins[0] | pins[1])
    using LogicElement::LogicElement;

    void create_initial_objects();
    void set_pin_positions();
    void set_pin_attributes();
    Object* create_obj_addr();
    void change_lgc_state();
};

class NotGate : public LogicElement { // this class has the same attributes as AndGate only minor things change such as while creating the initial object its initial position is different
                                      // it gives NotGate address in the create_obj addr, while creating it creates 2 numPins. in logic state changing (pins[1] = !pins[0])
public:
    using LogicElement::LogicElement;

    void create_initial_objects();
    void set_pin_positions();
    void set_pin_attributes();
    Object* create_obj_addr();
    void change_lgc_state();

};

class XorGate : public LogicElement {// this class has the same attributes as AndGate only minor things change such as while creating the initial object its initial position is different
                                     // it gives XorGate address in the create_obj addr,  in logic state changing (pins[2] = pins[0] ^ pins[1])
public:
    using LogicElement::LogicElement;

    void create_initial_objects();
    void set_pin_positions();
    void set_pin_attributes();
    Object* create_obj_addr();
    void change_lgc_state();

};

class DFlipFlop : public LogicElement {// this class has the same attributes as AndGate only minor things change such as while creating the initial object its initial position is different,
                                       // also in this one its size (png bit size) is different, it has 4 pins 2 input and 2 output. 2 input pins are pins[0] = D and pins[1] = clk
                                       // pins[2] = Q, pins[3] = !Q. 
                                       // it gives DFlipFlop address in the create_obj addr,  in logic state changing (if clock is 1, then Q output takes the value of D input)
public:
    using LogicElement::LogicElement;

    void create_initial_objects();
    void set_pin_positions();
    void set_pin_attributes();
    Object* create_obj_addr();
    void change_lgc_state();

};

class Logic0 : public LogicElement { // this class has the same attributes as AndGate only minor things change such as while creating the initial object its initial position is different, its size is different
                                     // it gives Logic0 address in the create_obj addr,  there is no logic state changing because this is a source (Ground or Logic0)
                                     // in set pin attributes its pin is initiliazed with LOW, and it is not changing throughout the simulation or with any attribute.
                                     // It also has one pin and that pin is output pins[0].
public:
    using LogicElement::LogicElement;

    void create_initial_objects();
    void set_pin_positions();
    void set_pin_attributes();
    Object* create_obj_addr();
};

class Logic1 : public LogicElement { // this class has the same attributes as AndGate only minor things change such as while creating the initial object its initial position is different, its size is different
                                     // it gives Logic1 address in the create_obj addr,  there is no logic state changing because this is a source (VDD or Logic1)
                                     // in set pin attributes its pin is initiliazed with HIGH, and it is not changing throughout the simulation or with any attribute.
                                     // It also has one pin and that pin is output pins[0].
public:
    using LogicElement::LogicElement;

    void create_initial_objects();
    void set_pin_positions();
    void set_pin_attributes();
    Object* create_obj_addr();
};

class Clock : public LogicElement { // this class has the same attributes as AndGate only minor things change such as while creating the initial object its initial position is different, its size is different
                                    // it gives Clock address in the create_obj addr, it is quite similar to Logic1 or Logic0. It is a source gate however its pins state is changing in every second.
                                    // in set pin attributes its pin is initiliazed with LOW, and it is not changing throughout the simulation or with any attribute.
                                    // It also has one pin and that pin is output pins[0].
public:
    using LogicElement::LogicElement;

    void create_initial_objects();
    void set_pin_positions();
    void set_pin_attributes();
    Object* create_obj_addr();
    void change_clk_state();


};

class LED : public LogicElement { // this class has the same attributes as AndGate only minor things change such as while creating the initial object its initial position is different, its size is different
                                  // it gives LED address in the create_obj addr. This is just for observing the states of the wires.
                                  // It has two pin and both of them are input. If one of them is HIGHZ or states of the pins are equal to each other LED will not change its texture.

public:
    using LogicElement::LogicElement;

    void create_initial_objects();
    void set_pin_positions();
    void set_pin_attributes();
    Object* create_obj_addr();
    void change_lgc_state();
};

