#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <chrono>
#include <thread>

class Object {
protected: // these are protected to reach the objects easier in the logic elements or wires
    static Object* head;     // holds the initial object created in the whole simulation
    static Object* dyn_head; // hold the final object created in the beginning of simulation, so in some occasions it wont have to go through every element in list
    Object* next;
    sf::RenderWindow* window;
    sf::Texture textures[2];
    sf::Sprite sprite;
    bool locked;
    bool state;
    bool selected;
public:
    //constructors
    Object() {
        next = nullptr;
        window = nullptr;
        locked = false;
        state = false;
        selected = false;
    };
    Object(sf::RenderWindow* window, bool state = false, bool selected = false, bool locked = true) : next(NULL), window(window), state(state), selected(selected), locked(locked)
    {
        add_obj_list();
    }
    Object(sf::RenderWindow* window, sf::Texture* textures, sf::Sprite sprite, bool state = false, bool selected = false, bool locked = true) :sprite(sprite), next(NULL), window(window), selected(selected), locked(locked)
    {
        this->state = state;
        this->textures[0] = textures[0];
        this->textures[1] = textures[1];
        this->sprite.setTexture(this->textures[0]);
        add_obj_list();
    }
    // destructor
    ~Object() = default;
    // 
    void traverse_list_for_clk(); // changes the clocks pin state, it traverses through the list and when it finds the clock calls the change_clk_state for it. (for objects and other elements it does nothing)
    void traverse_list_for_logic(); // changes the logic elements pin state (except the ones that are source, it does nothing),   it traverses through the list and when it finds the logic element calls the change_lgc_state for it. (for other objects it does nothing)
    bool is_button_hold(std::chrono::milliseconds a); // waits for a specified time amount in milliseconds and then it returns true (if button is still hold) or false (if button is not hold)
    bool is_inside_workbench(); // checks the if object is inside the workbench, if it is not inside of the workbench it returns false, if it is it returns true
    int obj_spec_choose(bool sim); // checks if simulation should start, end, create new dynamic element, move dynamic element, create wire, select wire (while in simulation cant do create, move or select) 
    void add_obj_list(); // adds the object that is calling this function to the end of the list.
    void drw_rctngle_lgc_elmnt(float lower_x, float lower_y, float upper_x, float upper_y); // for logic elements it draws an rectangle around them, while they are selected
    void arrange_position_origin(float width, float height, float center_x, float center_y); // arranges the position of object, arranges the origin of object
    void draw_window(); // draws window (first clear, then draw_list, and display it to the window)
    bool check_pin_positions(float mouse_x, float mouse_y, int& idx); // checks the pins[idx]'s position with the mouse positions
    virtual void del_obj_list_wires(Object* a){}; // polymorphic function, while deleting logic element it deletes the wires and while deleting wires it changes the pins attributes 
    void del_new_wire(Object* a) { // if new wire is being deleted then del_obj_list_wires should not be called, so this function is created for that.
        // because while deleting there will be problem, because pins are yet to be set and it will give memory access error
       // std::cout << __FUNCSIG__ << std::endl;
        using namespace std::this_thread; // sleep_for, sleep_until
        using namespace std::chrono; // nanoseconds, system_clock, seconds
        Object* obj;
        Object* obj_prev;
        if (head)
        {
            obj_prev = head;
            for (obj = obj_prev->next; obj != a; (obj_prev = obj, obj = obj->next))
                ; //null statement
            a = obj_prev;
            a->next = obj->next;
            delete obj;
            sleep_for(120ms);
        }
    }
    void del_obj_list(Object* a) { // deletes the object specified in a, while doing it checks if object is wire or logic element.
        // also it changes the next of the object which is previous of deleted
        // and to not have any memory access errors it changes the a object (in the simulator always objects will call it)
       // std::cout << __FUNCSIG__ << std::endl;
        Object* obj;
        Object* obj_prev;
        if (head)
        {
            obj_prev = head;
            for (obj = obj_prev->next; obj != a; (obj_prev = obj, obj = obj->next))
                ; //null statement
            obj->del_obj_list_wires(a);
            a = obj_prev;
            a->next = obj->next;
            delete obj;
        }
        print_addresses();
    }
    virtual sf::Vector2f get_wire_position(int idx) { // gets the pins[idx] wire position, this is virtual in order to reach pins
        return sf::Vector2f(0.f,0.f); // if calling function is object or any other thing it returns 0.f,0.f positions
    }
    virtual void set_wire_position(int idx, sf::Vector2f ax) { // virtual function for wire
    }
    virtual void set_pins_wire_positions(sf::Vector2f* old_pins) {} // virtual function to change position of wires while moving
    virtual void incr_numCon() {} // virtual function, it that will increase numConnections in Pin class
    virtual void set_wire_pin_states() {} // virtual function to change the state of pins that are connected to wire
    virtual void set_wire_start_attributes() {} // while creating a wire it changes the type, isSrc and sets the initial states from the pins.
    virtual void change_lgc_state() {} // virtual function for changing logic states
    virtual void change_clk_state() {} // virtual function for changing clock states
    virtual void object_clone(Object* obj) // object clone method, it is virtual because it changes if object is Logicelement
    {
       // std::cout << __FUNCSIG__ << std::endl;
        window = obj->window;
        next = nullptr;
        textures[0] = obj->textures[0];
        textures[1] = obj->textures[1];
        sprite = obj->sprite;
        locked = false;
        selected = false;
        state = false;
        add_obj_list();
    }
    virtual void draw_selected(Object* obj) // either change color of wire or draw rectangle around the logic element
    {                                       // getting positions, adding them a little offset
                                            // if delete button is pressed it deletes the object
                                            // if right mouse button or escape button from keyboard is pressed it releases the object
        //std::cout << __FUNCSIG__ << std::endl;
        float lower_x;
        float upper_x;
        float lower_y;
        float upper_y;
        get_corner_positions_obj(obj, &lower_x, &lower_y, &upper_x, &upper_y);
        lower_x -= 10;
        upper_x += 10;
        lower_y -= 10;
        upper_y += 10;
        drw_rctngle_lgc_elmnt(lower_x, lower_y, upper_x, upper_y);
        while (1)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Delete))
            {
                obj->del_obj_list(obj);
                break;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                break;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
                break;
        }
    }
    virtual void create_initial_objects() {} // virtual function for creating initial objects
    virtual void draw_object() { // draws object, it is virtual because drawing wire is different from drawing object
        window->draw(sprite);
    };
    virtual void get_corner_positions_obj(Object* obj, float* lower_x, float* lower_y, float* upper_x, float* upper_y) {
        // gets the corner position of object, because origin is middle it can be done easily by adding or subtracting half values of x and y positions
        // it is virtual because in wire it is different from the normal objects
        *lower_x = (obj->get_position().x) - (obj->get_size().x / 2.f) ;
        *upper_x = (obj->get_position().x) + (obj->get_size().x / 2.f) ;
        *lower_y = (obj->get_position().y) - (obj->get_size().y / 2.f) ;
        *upper_y = (obj->get_position().y) + (obj->get_size().y / 2.f) ;
    }
    virtual void set_pin_positions() {} // for setting pin positions
    virtual void set_pin_attributes() {} // for setting pin attributes
    virtual void wire_state_update() {} // for keeping the pins states same on the wire
    virtual int add_new_wire(float mouse_x, float mouse_y, int k, Object* o) { return 0; }
    virtual Object* create_obj_addr() { // it creates an dynamic memory for object and returns its address, it is virtual because its different for each class
        Object* a = new Object;
        return a;
    }
    virtual bool is_mouse_inside_obj(float mouse_x, float mouse_y, float lower_x, float lower_y, float upper_x, float upper_y) { 
        // checks if mouse is inside the specified object, if it is returns true if not returns false
        // virtual because on wire it is quite different from normal objects
        return (mouse_x >= lower_x && mouse_x <= upper_x && mouse_y >= lower_y && mouse_y <= upper_y) ? true : false;
    }
    static void set_dyn_head(Object* obj) { // sets the dynamic object head
        dyn_head = obj;
    }
    static void set_static_head(Object* obj) { // sets the static object head (normal head)
        head = obj;
    }
    // 
    int element_count_list(); // added to see how many elements there are in the list, returns that value
    void draw_list(); // draws the list, traversing from the head to the end of the list
    Object* get_last(); // gets the last object address, in the simulator class, objects is updated with this in order to prevent errors
    void get_mouse_positions(float* mouse_x, float* mouse_y); // returns current mouse positions to the mouse_x and mouse_y pointers
    void print_addresses(); // prints the list, in order to see if list is working
    virtual int get_numPins() { // virtual function for getting numPins
        return 3;
    } 
    virtual void* get_pinaddr(int idx) { // virtual function for getting Pin addr.
        return nullptr;
    }
    virtual sf::Vector2f get_pin_position(int i) { // virtual function for getting pin position
        return sf::Vector2f(0, 0);
    }
    virtual sf::Vector2f get_size() { //gets the size it is always texture 0
        // even if led changes its texture its size is same
        // virtual because in wire calculation is different
        return static_cast<sf::Vector2f>(textures[0].getSize());
    }
    virtual sf::Vector2f get_position() { // returns the position of sprite for logic elements
        // virtual because in wire it is different
        return sprite.getPosition();
    }
    sf::Sprite get_sprite(); 
    sf::Texture get_texture(int idx);
    sf::RenderWindow* get_window();
    Object* get_next();
    static Object* get_head() {
        return head;
    }
    static Object* get_dyn_head() {
        return dyn_head;
    }
};
class BackGround : public Object { // created in order to prevent errors, this is Background class
public: // does not have any attributes and it is the first  one on the list, so other objects can be paint on it
    using Object::Object;
};