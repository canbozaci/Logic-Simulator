// Includes
#include "Simulator.h"
// Defines


int main()
{
    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds
    Simulator sim1;
    sim1.initial_sim();
    sim1.start_sim();
    sim1.simulator_finish();
  /* Simulator sim2;
    sim2.initial_sim();
    sim2.start_sim();
    sim2.simulator_finish();
    Simulator sim3;
    sim3.initial_sim();
    sim3.start_sim();
    sim3.simulator_finish();*/
    return 0;
}