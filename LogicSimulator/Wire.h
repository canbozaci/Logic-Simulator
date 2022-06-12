#pragma once
#include "Object.h"
#include "Pin.h"
class Wire : public Object {
private:
    enum  pinType { INPUT, OUTPUT };
    enum  pinState { LOW, HIGH, HIGHZ };
    sf::Vertex line[2];
    Pin* pins[2];
public:
    using Object::Object;
    Wire() = default;
    Wire(const Wire& a) { 
        // copy constructor to copy attributes of one to the other directly
        // and add to the list in the fianl step
       // std::cout << __FUNCSIG__ << std::endl;
        line[0] = a.line[0];
        line[1] = a.line[1];
        pins[1] = a.pins[1];
        pins[0] = a.pins[0];
        window = a.window;
        next = nullptr;
        locked = false;
        selected = false;
        state = false;
        add_obj_list();
    };
    Object* create_obj_addr(); //creates wire object addr
    bool is_mouse_inside_obj(float mouse_x, float mouse_y, float lower_x, float lower_y, float upper_x, float upper_y); // we use the formula distance from a point to the line
    void get_corner_positions_obj(Object* obj, float* lower_x, float* lower_y, float* upper_x, float* upper_y); // returns line[0] and line[1] positions
    void draw_object(); // draws the line
    void draw_selected(Object* obj); // here changes the color of wire if selected
    void del_obj_list_wires(Object* a) { //delete the pins attributes while deleting the wire
        int pin0_numconn = pins[0]->get_numConnections();
        int pin1_numconn = pins[1]->get_numConnections();
    
        for (int i = 0; i < pin0_numconn; i++) {
            if (pins[0]->get_wire(i) == this) // compares the wire address pin[0] holding with this wire
            {
                if (i < pin0_numconn)
                    pins[0]->decrease_specs(i); //if it is not the last one connected, then update the attributes accordingly
                pins[0]->delete_last(); // delete last one
            }
        }
        for (int i = 0; i < pin1_numconn; i++) {
            if (pins[1]->get_wire(i) == this)  // compares the wire address pin[0] holding with this wire
            {
                if (i < pin1_numconn)
                    pins[1]->decrease_specs(i); //if it is not the last one connected, then update the attributes accordingly
                pins[1]->delete_last(); // delete last one
            }
        }
        pins[0]->dec_numConnections(); // decrease the number of conn pin[0]
        pins[1]->dec_numConnections(); // decrease the number of conn pin[1]

    }
    int add_new_wire(float mouse_x, float mouse_y, int k, Object* o); // adding new wire to the window
    sf::Vertex* get_line();
    sf::Vector2f get_position();
    sf::Vector2f get_size();
    int wire_create();
    void set_wire_position(int idx, sf::Vector2f ax) {
        line[idx].position = ax;
    }
    sf::Vector2f get_wire_position(int idx) {
        return line[idx].position;
    }
    void set_wire_pin_states();
    void set_wire_start_attributes();
    void wire_state_update();
    void incr_numCon();
};

