#include "LogicElements.h"
// LOGIC ELEMENT
int LogicElement::get_numPins() {
    return numPins;
}
void LogicElement::set_pins_wire_positions(sf::Vector2f* old_pins) { 
    // we are getting a old_pins pointer and checking which pins of wire is connected to it because it can either be pins[0] or pins[1]
    // and we are changing the wire position to the new pin position, so we can move the object and wires will move with it.
    // there is 2 loops inside of this because one checks the every pin of logic element and other one checks the wires that pin is connected to.                                                                   
    for (int k = 0; k < numPins; k++) {
        for (int l = 0; l < pins[k].get_numConnections(); l++)
        {
            if (fabsf(pins[k].get_wire(l)->get_wire_position(0).x - old_pins[k].x) <= 5.f && fabsf(pins[k].get_wire(l)->get_wire_position(0).y - old_pins[k].y) <= 5.f)
                pins[k].get_wire(l)->set_wire_position(0, pins[k].get_pin_position()); // change pin 0
            else if (fabsf(pins[k].get_wire(l)->get_wire_position(1).x - old_pins[k].x) <= 5.f && fabsf(pins[k].get_wire(l)->get_wire_position(1).y - old_pins[k].y) <= 5.f)
                pins[k].get_wire(l)->set_wire_position(1, pins[k].get_pin_position()); // change pin 1
        }
    }
}
int LogicElement::get_Connections(int idx) {
    return pins[idx].get_numConnections(); 
}
void LogicElement::object_clone(Object* obj) // in this one while cloning object, for extra we get the pin positions, pin attributes and then add them to the list.
{
    //std::cout << __FUNCSIG__ << std::endl;
    window = obj->get_window();
    next = nullptr;
    textures[0] = obj->get_texture(0);
    textures[1] = obj->get_texture(1);
    sprite = obj->get_sprite();
    locked = false; // false because new object is created
    selected = false;
    state = false;
    numPins = obj->get_numPins();
    set_pin_positions();
    set_pin_attributes();
    add_obj_list();
}
void LogicElement::draw_object() {
    window->draw(sprite); 
};
sf::Vector2f LogicElement::get_pin_position(int i) {
    return pins[i].get_pin_position();
}
void* LogicElement::get_pinaddr(int idx)
{
    return pins + idx;
}
void LogicElement::del_obj_list_wires(Object* a) { 
    int p = get_numPins(); 
    for (int k = 0; k < p; k++)
    {
        int b = get_Connections(k);
        for (int l = 0; l < b; l++) 
        {
            if(pins[k].get_wire(l))
                 a->del_obj_list(pins[k].get_wire(l));
        }
    }
}
// AND GATE
void AndGate::create_initial_objects()
{
    this->create_texture("AND");
    if (locked) {
        this->arrange_position_origin(100.f, 50.f, 49.5f, 30.5f);
        this->sprite.setTexture(this->textures[0]);
    }
    numPins = 3;
}
void AndGate::set_pin_positions() {
    pins[0].set_position(get_position().x - get_size().x / 2 + 5, get_position().y - get_size().y / 2 + 15);
    pins[1].set_position(get_position().x - get_size().x / 2 + 5, get_position().y - get_size().y / 2 + 45);
    pins[2].set_position(get_position().x - get_size().x / 2 + 93, get_position().y - get_size().y / 2 + 30);
}
void AndGate::set_pin_attributes() {
    pins[0].set_type(0);
    pins[1].set_type(0);
    pins[2].set_type(1);
}
Object* AndGate::create_obj_addr() {
    Object* a = new AndGate;
    return a;
}
void AndGate::change_lgc_state() {
    if (pins[0].get_state() == HIGHZ || pins[1].get_state() == HIGHZ)
        pins[2].set_pin_state(HIGHZ);
    else
        pins[2].set_pin_state(pins[0].get_state() && pins[1].get_state());
}
// OR GATE
void OrGate::create_initial_objects()
{
    this->create_texture("OR");
    if (locked) {
        this->arrange_position_origin(100.f, 135.f, 49.5f, 30.5f);
        this->sprite.setTexture(this->textures[0]);
    }
    numPins = 3;
}
void OrGate::set_pin_positions() {
    pins[0].set_position(get_position().x - get_size().x / 2 + 5, get_position().y - get_size().y / 2 + 15);
    pins[1].set_position(get_position().x - get_size().x / 2 + 5, get_position().y - get_size().y / 2 + 45);
    pins[2].set_position(get_position().x - get_size().x / 2 + 93, get_position().y - get_size().y / 2 + 30);
}
void OrGate::set_pin_attributes() {
    pins[0].set_type(0);
    pins[1].set_type(0);
    pins[2].set_type(1);
}
Object* OrGate::create_obj_addr() {
    Object* a = new OrGate;
    return a;
}
void OrGate::change_lgc_state()
{
    if (pins[0].get_state() == HIGHZ || pins[1].get_state() == HIGHZ)
        pins[2].set_pin_state(HIGHZ);
    else
        pins[2].set_pin_state(pins[0].get_state() || pins[1].get_state());
}
// NOT GATE
void NotGate::create_initial_objects()
{
    this->create_texture("NOT");
    if (locked) {
        this->arrange_position_origin(100.f, 220.f, 49.5f, 30.5f);
        this->sprite.setTexture(this->textures[0]);
    }
    numPins = 2;
}
void NotGate::set_pin_positions() {
    pins[0].set_position(get_position().x - get_size().x / 2 + 5, get_position().y - get_size().y / 2 + 30);
    pins[1].set_position(get_position().x - get_size().x / 2 + 93, get_position().y - get_size().y / 2 + 30);
}
void NotGate::set_pin_attributes() {
    pins[0].set_type(0);
    pins[1].set_type(1);
}
Object* NotGate::create_obj_addr() {
    Object* a = new NotGate;
    return a;
}
void NotGate::change_lgc_state()
{
    if (pins[0].get_state() == HIGHZ)
        pins[1].set_pin_state(HIGHZ);
    else
        pins[1].set_pin_state(!pins[0].get_state());
}
// XOR GATE
void XorGate::create_initial_objects()
{
    this->create_texture("XOR");
    if (locked) {
        this->arrange_position_origin(100.f, 305.f, 49.5f, 30.5f);
        this->sprite.setTexture(this->textures[0]);
    }
    numPins = 3;
}
void XorGate::set_pin_positions() {
    pins[0].set_position(get_position().x - get_size().x / 2 + 5, get_position().y - get_size().y / 2 + 15);
    pins[1].set_position(get_position().x - get_size().x / 2 + 5, get_position().y - get_size().y / 2 + 45);
    pins[2].set_position(get_position().x - get_size().x / 2 + 93, get_position().y - get_size().y / 2 + 30);
}
void XorGate::set_pin_attributes() {
    pins[0].set_type(0);
    pins[1].set_type(0);
    pins[2].set_type(1);
}
Object* XorGate::create_obj_addr() {
    Object* a = new XorGate;
    return a;
}
void XorGate::change_lgc_state()
{
    if (pins[0].get_state() == HIGHZ || pins[1].get_state() == HIGHZ)
        pins[2].set_pin_state(HIGHZ);
    else
        pins[2].set_pin_state(pins[0].get_state() ^ pins[1].get_state());
}
// D Flip Flop
void DFlipFlop::create_initial_objects()
{
    this->create_texture("DFF");
    if (locked) {
        this->arrange_position_origin(100.f, 400.f, 49.5f, 40.5f);
        this->sprite.setTexture(this->textures[0]);
    }
    numPins = 4;
}
void DFlipFlop::set_pin_positions() {
    pins[0].set_position(get_position().x - get_size().x / 2 + 5, get_position().y - get_size().y / 2 + 20);
    pins[1].set_position(get_position().x - get_size().x / 2 + 5, get_position().y - get_size().y / 2 + 40);
    pins[2].set_position(get_position().x - get_size().x / 2 + 93, get_position().y - get_size().y / 2 + 20);
    pins[3].set_position(get_position().x - get_size().x / 2 + 93, get_position().y - get_size().y / 2 + 60);
}
void DFlipFlop::set_pin_attributes() {
    pins[0].set_type(0);
    pins[1].set_type(0);
    pins[2].set_type(1);
    pins[3].set_type(1);
}
Object* DFlipFlop::create_obj_addr() {
    Object* a = new DFlipFlop;
    return a;
}
void DFlipFlop::change_lgc_state()
{
    if (pins[0].get_state() == HIGHZ || pins[1].get_state() == HIGHZ)
    {
        pins[2].set_pin_state(LOW);
        pins[3].set_pin_state(HIGH);
    }
    else {
        if (pins[1].get_state())
        {
            pins[2].set_pin_state(pins[0].get_state());
            pins[3].set_pin_state(!pins[0].get_state());
        }
    }
}
// Logic0
void Logic0::create_initial_objects()
{
    this->create_texture("GND");
    if (locked) {
        this->arrange_position_origin(100.f, 700.f, 23.f, 19.f);
        this->sprite.setTexture(this->textures[0]);
    }
    numPins = 1;

}
void Logic0::set_pin_positions() {
    pins[0].set_position(get_position().x - get_size().x / 2 + 22, get_position().y - get_size().y / 2 + 5);
}
void Logic0::set_pin_attributes() {
    pins[0].set_pin_state(0);
    pins[0].set_type(1);
}
Object* Logic0::create_obj_addr() {
    Object* a = new Logic0;
    return a;
}
// Logic1
void Logic1::create_initial_objects()
{
    this->create_texture("VDD");
    if (locked) {
        this->arrange_position_origin(100.f, 600.f, 30.f, 30.f);
        this->sprite.setTexture(this->textures[0]);
    }
    numPins = 1;
    pins[0].set_pin_state(1);
}
void Logic1::set_pin_positions() {
    pins[0].set_position(get_position().x - get_size().x / 2 + 30, get_position().y - get_size().y / 2 + 54);
}
void Logic1::set_pin_attributes() {
    pins[0].set_pin_state(1);
    pins[0].set_type(1);
}
Object* Logic1::create_obj_addr() {
    Object* a = new Logic1;
    return a;
}
// Clock
void Clock::create_initial_objects()
{
    this->create_texture("CLOCK");
    if (locked) {
        this->arrange_position_origin(100.f, 500.f, 40.f, 30.5f);
        this->sprite.setTexture(this->textures[0]);
    }
    numPins = 1;
}
void Clock::set_pin_positions() {
    pins[0].set_position(get_position().x - get_size().x / 2 + 74, get_position().y - get_size().y / 2 + 30);
}
void Clock::set_pin_attributes() {
    pins[0].set_pin_state(0);
    pins[0].set_type(1);
}
Object* Clock::create_obj_addr() {
    Object* a = new Clock;
    return a;
}
void Clock::change_clk_state() {
    pins[0].state_toggle();
}
// LED
void LED::create_initial_objects()
{
    this->create_texture("LEDOFF");
    textures[1].loadFromFile("../assets/LEDON.png");
    if (locked) {
        this->arrange_position_origin(100.f, 780.f, 20.f, 39.f);
        this->sprite.setTexture(this->textures[0]);
    }
    numPins = 2;
}
void LED::set_pin_positions() {
    pins[0].set_position(get_position().x - get_size().x / 2 + 13, get_position().y - get_size().y / 2 + 72);
    pins[1].set_position(get_position().x - get_size().x / 2 + 29, get_position().y - get_size().y / 2 + 72);
}
void LED::set_pin_attributes() {
    pins[0].set_type(0);
    pins[1].set_type(0);
}
Object* LED::create_obj_addr() {
    Object* a = new LED;
    return a;
}
void LED::change_lgc_state() {
    if (pins[0].get_state() == pins[1].get_state() || pins[0].get_state() == HIGHZ || pins[1].get_state() == HIGHZ)
    {
        sprite.setTexture(textures[0]);
    }
    else
    {
        sprite.setTexture(textures[1]);
    }
}
