#include "Wire.h"

Object* Wire::create_obj_addr() { // create new dynamic wire addr return wire addr
    Object* a = new Wire;
    return a;
}
bool Wire::is_mouse_inside_obj(float mouse_x, float mouse_y, float lower_x, float lower_y, float upper_x, float upper_y) {
    // here we use the formula distance from a point to the line
    // to do that we create a equation of the line and use the formula distance from a point to the line
    // if it is smaller then 10 we say wire is selected.
    //std::cout << __FUNCSIG__ << std::endl;
    float temp_upper_y,temp_upper_x,temp_lower_y,temp_lower_x; // to understand if mouse is inside these wire lines
    if (upper_y > lower_y) {// to understand if mouse is inside these wire lines
        temp_upper_y = upper_y; 
        temp_lower_y = lower_y;
    }
    else {
        temp_upper_y = lower_y;
        temp_lower_y = upper_y;
    }
    if (upper_x > lower_x) {
        temp_upper_x = upper_x;
        temp_lower_x = lower_x;
    }
    else {
        temp_upper_x = lower_x;
        temp_lower_x = upper_x;
    }
    float a = (upper_y - lower_y);
    float b = (lower_x - upper_x);
    float c = -(a * upper_x + b * upper_y);
    float h = fabsf(a * mouse_x + b * mouse_y + c) / sqrt(powf(a, 2) + powf(b, 2)); // formula for distance from a point to the line
    if (mouse_y <= temp_upper_y && mouse_y >= temp_lower_y && mouse_x <= temp_upper_x && mouse_x >= temp_lower_x) // to understand if mouse is inside these wire lines
        return (h < 10.0f);
    return false;
}
void Wire::get_corner_positions_obj(Object* obj, float* lower_x, float* lower_y, float* upper_x, float* upper_y) {
    //std::cout << __FUNCSIG__ << std::endl;
    *lower_x = (obj->get_position().x); // line[0].x position
    *upper_x = (obj->get_position().x) + (obj->get_size().x); // Line[1].x position
    *lower_y = (obj->get_position().y); // line[0].y position
    *upper_y = (obj->get_position().y) + (obj->get_size().y); // line[1].y position
}
void Wire::draw_object() {
    get_window()->draw(line, 2, sf::LinesStrip); // drawing line is different from drawing sprites so we override it
}
void Wire::draw_selected(Object* obj) { // change the color of the wire to the red
    line[0].color = line[1].color = sf::Color::Red;
    obj->draw_window();
    bool deleted = false;
    while (1)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Delete))
        {
            obj->del_obj_list(obj);
            deleted = true;
            break;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            break;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
            break;
    }
    if (!deleted)
        line[0].color = line[1].color = sf::Color::Blue;
}
int Wire::wire_create() { //creating a new wire
    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds
    Object* obj;
    int l;
    float mouse_x, mouse_y;
    get_mouse_positions(&mouse_x, &mouse_y);
    for (obj = obj->get_dyn_head(); obj != nullptr; obj = obj->get_next()) // starts to look from dynamic head, because locked objects can not create a wire
    {
        if (obj->check_pin_positions(mouse_x, mouse_y, l))
        {
            pins[1] = static_cast<Pin*>(obj->get_pinaddr(l));         //  pin address it is holding should be same as pin it is being connected to
            line[1].position = sf::Vector2f(obj->get_pin_position(l));// position of line[1] attached to pin it is being connected to
            
            if (pins[1]->get_type() == pins[0]->get_type()) { // if the types are same dont create wire, such as connecting INPUT to INPUT or OUTPUT to OUTPUT
                sleep_for(150ms); // to prevent missclicks
                return -1; // returning -1 means wire should be deleted, 
            }
            if (pins[0]->get_type() == 0) // if it is input pin[0] 
            {
                if(pins[0]->get_numConnections() == 1) // if input pin[0] has connections, we can not connect to it
                {
                sleep_for(150ms); // to prevent missclicks
                return -1; // returning -1 means wire should be deleted, 
                }
            }
            if (pins[1]->get_type() == 0)// if it is input pin[1] 
            {
                if (pins[1]->get_numConnections() == 1) // if input pin[1] has connections, we can not connect to it
                {
                    sleep_for(150ms); // to prevent missclicks
                    return -1; // returning -1 means wire should be deleted, 
                }
            }
            set_wire_start_attributes(); // set the attributes of wire
            incr_numCon(); // increase the number of connections
            locked = false; // it is not important but to be meaningful it is false
            sleep_for(150ms); // to prevent missclicks
            return 1; // it means succesfully created wire and it should not be deleted
        }
    }
    return -1; // returning -1 means wire should be deleted, 
}
int  Wire::add_new_wire(float mouse_x, float mouse_y, int k, Object* o)
{
    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds
   // std::cout << __FUNCSIG__ << std::endl;
    line[0].color = line[1].color = sf::Color::Blue;
    pins[0] = static_cast<Pin*>(o->get_pinaddr(k));          //  pin address it is holding should be same as pin it is being connected to
    line[0].position = sf::Vector2f(o->get_pin_position(k)); // position of line[0] attached to pin it is being connected to
    sleep_for(120ms);
    sf::Event event;
    while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        line[1].position = sf::Vector2f(sf::Mouse::getPosition(*(o->get_window()))); // line[1] position is same as mouse position in the window
        o->get_window()->clear();
        o->draw_list();
        o->get_window()->display();
        o->get_window()->pollEvent(event); // to get the events
        switch (event.type)
        {
        case sf::Event::MouseButtonReleased:
            return wire_create(); // if released goes into wire_create
        case sf::Event::MouseButtonPressed:
            {
                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    return wire_create(); // if left mouse button is pressed it goes into wire_create
            }
        }
    }
    return -1; // means delete the function because it is not found
}
sf::Vertex*  Wire::get_line() {
    return line;
}
sf::Vector2f Wire::get_position() {
    return line->position;
}
sf::Vector2f Wire::get_size() { // size is just difference betweens lines
    return (sf::Vector2f((line[1].position.x - line[0].position.x), (line[1].position.y - line[0].position.y)));
}
void Wire::set_wire_pin_states() { // updates the state of wires 
    Object* obj;
    {
        for (obj = dyn_head; obj != nullptr; obj = obj->get_next())
        {
                obj->wire_state_update(); 
        }
    }

}
void Wire::wire_state_update() { 
    if (pins[0]->get_type() == OUTPUT) // if pins[0] is output, means pins[1] input and it should get state from pin[0]
    {
        int a = pins[0]->get_state();
        pins[1]->set_pin_state(a);

    }
    if (pins[1]->get_type() == OUTPUT) // if pins[1] is output, means pins[0] input and it should get state from pin[0]
    {
        int a = pins[1]->get_state();
        pins[0]->set_pin_state(a);
    }
}
void Wire::set_wire_start_attributes() { 
    if (pins[0]->get_type() == OUTPUT) // arranges the isSrc, and states initially
    {
        pins[0]->set_isSrc(true);  // if it is output then it should be source
        pins[1]->set_isSrc(false); // it is input it should not be source
        int a = pins[0]->get_state();
        pins[1]->set_pin_state(a);

    }
    if (pins[1]->get_type() == OUTPUT)  // arranges the isSrc, and states initially
    {
        pins[1]->set_isSrc(true);    // if it is output then it should be source
        pins[0]->set_isSrc(false);      // it is input it should not be source
        int a = pins[1]->get_state();
        pins[0]->set_pin_state(a);
    }
    pins[0]->set_wire(this); // sets the pins pointer array to the this wire
    pins[1]->set_wire(this); // sets the pins pointer array to the this wire
    pins[0]->set_connectedTo(pins[1]); // pin 0 is connected to pin1
    pins[1]->set_connectedTo(pins[0]); // pins1 is connected to pin0
}
void Wire::incr_numCon() {
    pins[0]->incr_numConnections();
    pins[1]->incr_numConnections();
}