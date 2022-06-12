#include "Simulator.h"

void Simulator::initial_sim() { // creating the inital objects by their png files and background
    sf::Texture a;
    sf::Sprite b;
    a.loadFromFile("../assets/background.png");
    b.setTexture(a);
    objects = new BackGround(window, &a, b);
    objects = new AndGate(window, false, false, true);
    objects->create_initial_objects();
    objects = new OrGate(window, false, false, true);
    objects->create_initial_objects();
    objects = new XorGate(window, false, false, true);
    objects->create_initial_objects();
    objects = new NotGate(window, false, false, true);
    objects->create_initial_objects();
    objects = new DFlipFlop(window, false, false, true);
    objects->create_initial_objects();
    objects = new Logic0(window, false, false, true);
    objects->create_initial_objects();
    objects = new Logic1(window, false, false, true);
    objects->create_initial_objects();
    objects = new Clock(window, false, false, true);
    objects->create_initial_objects();
    objects = new LED(window, false, false, true);
    objects->create_initial_objects();
    sf::Texture start_texture[2];
    sf::Texture stop_texture[2];
    sf::Sprite start_sprite;
    sf::Sprite stop_sprite;
    if (!start_texture[0].loadFromFile("../assets/START.png") || !stop_texture[0].loadFromFile("../assets/STOP.png"))
    {
        std::cout << "Failed to load texture Start.png!" << std::endl;
    }
    objects = new Object(window, start_texture, start_sprite, true);
    objects->arrange_position_origin(400.f, 50.f, 106.5f, 18.f);
    objects = new Object(window, stop_texture, stop_sprite, false);
    objects->arrange_position_origin(800.f, 50.f, 106.5f, 18.f);
    objects->set_dyn_head(objects);
}
void Simulator::start_sim() // simulation is started
{
    using namespace std;
    bool simulation_hold = false;
    int k;
    sf::Clock clock; // sfml clock defined to help us for timing
    sf::Time time = clock.getElapsedTime(); // time to get clock time
    while (window->isOpen())
    {
        sf::Event evnt;
        while (window->pollEvent(evnt))
        {
            switch (evnt.type)
            {
            case sf::Event::Closed:
                window->close();
                cout << "\nClosed the window" << endl;             
                break;
            case sf::Event::MouseButtonPressed:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    k = objects->obj_spec_choose(simulation_hold); // goes into obj_spec_choose with simulation_hold ( if simulation hold dont do move, create or select)
                    if (k == 2) //simulation stop
                    {
                        simulation_hold = false;
                        k = 0;
                    }
                    else if (k == 1) //simulation start
                    {
                        simulation_hold = true;
                        clock.restart(); // we restart the clock
                        time = clock.getElapsedTime(); // get the time
                        k = 0;
                    }
                    objects = objects->get_last(); // set object to the last one to prevent errors
                }
                break;
            }
        }

        if (simulation_hold == true) // 
        {
            time = clock.getElapsedTime();
            if (time.asSeconds() >= 1) // if time is greater then 1 it means clock is in rising edge
            {
                objects->traverse_list_for_clk(); // change clock state
                clock.restart(); // restart the clcok again
            }
            objects->traverse_list_for_logic(); // update logic element pin states
            objects->set_wire_pin_states(); // set the pins of wires connected to each other
        }
        objects->draw_window();
    }
}
void Simulator::simulator_finish() {
    int k = objects->element_count_list();
    for (int l = 0; l < k - 1; l++) {
        objects->del_obj_list(objects);
        objects = objects->get_last();
    }
    objects->set_dyn_head(nullptr);
    objects->set_static_head(nullptr);
    delete objects;
    delete window;
}