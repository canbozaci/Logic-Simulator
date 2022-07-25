# Logic-Simulator
Logic Simulator done in C++ with OOP Paradigm

Project is in master branch. This project was done in Visual Studio using SFML library. It implements a basic Logic Simulator including some primitive gates, clock, D Flip Flop, LED, VDD and GND. Singly Linked List is being used for controlling the elements.
 
Objects in toolbar can be moved to the workbench. If object is not dragged into workbench it will be deleted. By clicking into the ports of the object, a wire will appear if you click into the another objects wire than it will be connected to that object. Input to input or output to output wire connections are prevented. If a pin is clicked (wire being created) and if the user clicks into the space which is not suitable port than wire will be deleted.

Objects inside the workbench can be selected by clicking, if user presses delete button selected object will be deleted. If the object is logic element and it has wire connections than wires connected to it will be deleted. If wire is being deleted (also wire can be independently selected) than attributes of pins on the each end is being resetted.

If user wants to move a object than it can drag the object inside the workbench, if user tries to move the object outside the workbench it is not allowed. He can only move the object inside the workbench (if he reaches the limits of the workbench than object will not move outside the workbench). If object is logic element and it has wire connections, wires connected to it will move along with the logic element.

To start simulation, start button must be pressed. Once simulation is being started all the attributes talked above such as move, select, create or delete object can not be done at the simulation time. To stop simulation stop button must be pressed. 

Example instantiation of this is given in main.cpp file. Simulator class has all these objects mentioned above (composition). 

