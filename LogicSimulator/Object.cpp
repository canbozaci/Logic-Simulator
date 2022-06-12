#include "Object.h"
#include "Wire.h"
Object* Object::head{ nullptr };
Object* Object::dyn_head{ nullptr };


// change member functions
void Object::traverse_list_for_clk() {
    Object* obj;
    {
        for (obj = dyn_head; obj != nullptr; obj = obj->next)
            obj->change_clk_state();
    }
}
void Object::traverse_list_for_logic() {
    Object* obj;
    {
        for (obj = dyn_head; obj != nullptr; obj = obj->next)
            obj->change_lgc_state();
    }
}
bool Object::is_button_hold(std::chrono::milliseconds a) {
    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds
    sleep_for(a);
    return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}
bool Object::is_inside_workbench() {
    float mouse_x, mouse_y;
    get_mouse_positions(&mouse_x, &mouse_y);
    if (!is_mouse_inside_obj(mouse_x, mouse_y, 230.f, 150.f, 1390.f, 850.f)) // this positions are the background image positions calculated correctly
        return false; 
    else
        return true;
}
bool Object::check_pin_positions(float mouse_x, float mouse_y, int& idx) {
    // checks the pin positions and if the index is found it changes the idx to the found one and returns true
    int a = get_numPins();
    for (int k = 0; k < a; k++)
    { 
        float lower_x = (get_pin_position(k).x) - 5;
        float upper_x = (get_pin_position(k).x) + 5;
        float lower_y = (get_pin_position(k).y) - 5;
        float upper_y = (get_pin_position(k).y) + 5;
        if (is_mouse_inside_obj(mouse_x, mouse_y, lower_x, lower_y, upper_x, upper_y))
        {
            idx = k;
            return true;
        }
    }
    return false;
}
int Object::obj_spec_choose(bool sim) { // burda basili tutmak gerekse daha iyi olabilir, suan tek týklayýnca da yaratýyor nasýl olur idk
    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds
    bool button_hold = is_button_hold(120ms);
    Object* obj;
    int idx;
    float lower_x, lower_y, upper_x, upper_y;
    float mouse_x, mouse_y;
    {
        for (obj = head; obj != nullptr; obj = obj->next)
        {
            obj->get_corner_positions_obj(obj, &lower_x, &lower_y, &upper_x, &upper_y);
            get_mouse_positions(&mouse_x, &mouse_y);
            if (obj->is_mouse_inside_obj(mouse_x, mouse_y, lower_x, lower_y, upper_x, upper_y))
            {
                if (strcmp(typeid(*obj).name(), "class Object") == 0) // to check if pressed object is start or stop button
                { // it compares the name of object with class Object, if true and state of object is true it means start simulation 
                    // so returns 1, if it state is false it means stop simulation and returns 2
                    if (obj->state == true)
                        return 1;
                    return 2;
                }
                else if (obj->locked == true && strcmp(typeid(*obj).name(), "class Object") && strcmp(typeid(*obj).name(), "class BackGround") && button_hold == true && !sim)   
                {
                    // Creating a new dynamic object, if object is locked and class is not Object and Background, and button_hold is true and while not in simulation
                    // it creates a new dynamic object.
                    Object* new_obj = obj->create_obj_addr();
                    new_obj->object_clone(obj);

                    while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        new_obj->sprite.setPosition(static_cast<float>(sf::Mouse::getPosition(*window).x), static_cast<float>(sf::Mouse::getPosition(*window).y)); // to change position while dragging mouse
                        new_obj->draw_window(); // refresh the screen
                    }
                    if (!new_obj->is_inside_workbench()) // if object is not inside the workbench just delete the object
                        new_obj->del_obj_list(new_obj);
                    else
                    {
                        new_obj->set_pin_positions(); // if it is set the pin positions accordingly
                    }
                    return 0; // just to return something, 0 means succesfull for this
                }
                else if (obj->check_pin_positions(mouse_x,mouse_y,idx)) // Drawing wire by checking the pin positions with wire
                    { 
                        // creates a new Wire object, adds a new wire, if the wire is not connected properly then delete the wire
                        Object* temp_obj = new Wire(window);
                        int a = temp_obj->add_new_wire(mouse_x, mouse_y, idx,obj);
                        if (a == -1)
                            temp_obj->del_new_wire(temp_obj);
                        return 0; // just to return something, 0 means succesfull for this
                }
                else if (obj->locked == false && button_hold == true && !sim) // move the dynamic objects
                {
                    sf::Vector2f old_position_pin_holder[4]; // to hold old pin positions
                    while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        if (obj->is_inside_workbench()) // to not move the object outside the workbench we check while moving
                            obj->sprite.setPosition(static_cast<float>(sf::Mouse::getPosition(*window).x), static_cast<float>(sf::Mouse::getPosition(*window).y));
                        obj->draw_window(); // refresh the screen
                        for (int m = 0; m < obj->get_numPins(); m++)
                            old_position_pin_holder[m] = obj->get_pin_position(m); // save the old pin positions
                        obj->set_pin_positions(); // update the pin positions
                        obj->set_pins_wire_positions(old_position_pin_holder); // update the wire positions by checking with old pin positions and changing their positions with new pin positions
                        
                    }
                    return 0; // just to return something, 0 means succesfull for this
                }
                else if (button_hold == false && obj->locked == false && !sim) // select the dynamic object
                {
                    obj->draw_selected(obj); // select the object
                    return 0; // just to return something, 0 means succesfull for this
                }
            }
        }
        return -1; // just to return something, this means it did not do anything
    }
}
int Object::element_count_list() { // to delete every created new dynamic object hold the number of created objects
    int k = 0;
    if (head)
    {
        Object* obj;
        {
            for (obj = head; obj != nullptr; obj = obj->next)
                k += 1;
        }
    }
    return k;
}
void Object::draw_list()
{
    if (head)
    {
        Object* obj;
        {
            for (obj = head; obj != nullptr; obj = obj->next)
                obj->draw_object(); // draw_object is polymorphic
        }
    }
}
void Object::add_obj_list() {
    std::cout << __FUNCSIG__ << std::endl;
    Object* obj;
    if (head)
    {
        for (obj = head; obj->next != nullptr; obj = obj->next)
            ; // null statement
        obj->next = this;
        this->next = nullptr;
    }
    else  // if the list is empty update the head 
    {
        head = this;
    }
    print_addresses(); // to see if working properly, good demonstration
}
void Object::drw_rctngle_lgc_elmnt(float lower_x, float lower_y, float upper_x, float upper_y)
{
    sf::Vertex line[5]; // 5 lines because they must be connected to each other
    for (int i = 0; i < 5; i++)
        line[i].color = sf::Color::Red;
    line[0].position = sf::Vector2f(lower_x, upper_y);
    line[1].position = sf::Vector2f(lower_x, lower_y);
    line[2].position = sf::Vector2f(upper_x, lower_y);
    line[3].position = sf::Vector2f(upper_x, upper_y);
    line[4].position = sf::Vector2f(lower_x, upper_y);
    window->clear(); // here we do not call draw_window method because lines must be drawed too
    draw_list(); 
    window->draw(line, 5, sf::LinesStrip); // we draw lines
    window->display();
}
void Object::arrange_position_origin(float width, float height, float center_x, float center_y)
{
    sprite.setOrigin(center_x, center_y); // set origin positions of sprite
    sprite.setPosition(width, height); // set position of sprite
};
void Object::draw_window() { 
    window->clear();
    draw_list();
    window->display();
}
// get member functions
Object* Object::get_last() { //set_last fonksiyonu yaz orda direk hep en sonuncuyu da tutsun, veya static object yaz her listeye ekleme yaptýðýnda en son olarak onu yap.
    if (head)
    {
        Object* obj;
        {
            for (obj = head; obj->next != nullptr; obj = obj->next)
                ; //null statement
        }
        return obj;
    }
}
void Object::get_mouse_positions(float* mouse_x, float* mouse_y) {
    *mouse_x = static_cast<float>(sf::Mouse::getPosition(*window).x);
    *mouse_y = static_cast<float>(sf::Mouse::getPosition(*window).y);
}
void Object::print_addresses() {
    if (head)
    {
        Object* obj;
        {
            int x = 1;
            for (obj = head; obj != nullptr; obj = obj->next)
            {

                std::cout << "element \t" << x++ << "current address is: " << obj << "   next address is" << obj->next << std::endl;

            }
            std::cout << "*****************************************************************" << std::endl;
        }
    }
}
sf::Sprite Object::get_sprite() {
    return sprite;
}
sf::Texture Object::get_texture(int idx) {
    return textures[idx];
}
sf::RenderWindow* Object::get_window() {
    return window;
}
Object* Object::get_next() {
    return next;
}