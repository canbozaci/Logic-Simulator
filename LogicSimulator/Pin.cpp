#include "Pin.h"
void Pin::set_position(float x, float y) // set pin pos while creating a dynamic object or moving it.
{
    pos = sf::Vector2f(x, y);
}
void Pin::set_pin_state(int a) { //  a = 0 LOW , a = 1 HIGH, a = 2 HIGHZ
    a == 2 ? (state = HIGHZ) : (a == 1 ? state = HIGH : state = LOW);
}
void Pin::set_type(int a) { // a = 0 INPUT, a = 1 OUTPUT
    a == 0 ? (type = INPUT) : (type = OUTPUT);
}
void Pin::incr_numConnections() {
    numConnections++;
}
void Pin::dec_numConnections() {
    numConnections--;
}
int Pin::get_numConnections() {
    return numConnections;
}
void Pin::decrease_specs(int i) { 
    // if we are deleting wire and a pin connected to it is holding its value in array 
    // less than numConnections there will be errors regarding to the deleted content
    // to prevent it we move every attribute to the one before so that it will not give any error
    for (int k = i; k < numConnections; k++) {
        connectedTo[i] = connectedTo[i + 1];
        isSrc[i] = isSrc[i + 1];
        wires[i] = wires[i + 1];
    }
}
void Pin::delete_last() { // deletes the last contents of pins
    connectedTo[numConnections] = nullptr;
    wires[numConnections] = nullptr;
}
void Pin::state_toggle() { // toggle the state of pin, it is used only in Clock
    if (state == HIGHZ)
        state = LOW;
    else if (state == LOW)
        state = HIGH;
    else if (state == HIGH)
        state = LOW;
}
void Pin::set_isSrc(bool a) {
    isSrc[numConnections] = a;
}
void Pin::set_wire(Object* wr) {
    wires[numConnections] = wr;
}
void Pin::set_connectedTo(Pin* pn) {
    connectedTo[numConnections] = pn;
}
sf::Vector2f Pin::get_pin_position()
{
    return pos;
}