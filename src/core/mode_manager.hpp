
#include "flight_mode.hpp"

class ModeManager {
public:
    ModeManager();
    void to_mode(int new_mode);

    FlightMode *mode;

private:
    StartupMode startup_class;
    StandbyMode standby_class;
    AscentMode ascent_class;
    DrogueDeployedMode drogue_deployed_class;
    MainDeployedMode main_deployed_class;
    FaultMode fault_class;
};

extern ModeManager manager;