#ifndef FLIGHT_MODE_ENUM_
#define FLIGHT_MODE_ENUM_

#include <string>

class FlightMode {
public:
    void execute();
    virtual void transition() = 0;
    virtual int id() = 0;
    virtual std::string name() = 0;
};

class StartupMode : public FlightMode {
public:
    void execute();
    void transition();

    int id() { return 0; }
    std::string name() { return "Startup"; };
};

class StandbyMode : public FlightMode {
public:
    void transition();

    int id() { return 0; }
    std::string name() { return "Standby"; };
};

class PoweredAscentMode : public FlightMode {
public:
    void transition();

    int id() { return 0; }
    std::string name() { return "Powered Ascent"; };
};

class CoastingMode : public FlightMode {
public:
    void transition();

    int id() { return 0; }
    std::string name() { return "Coasting"; };
private:
    bool apogee_detected();
    void deploy_drogue();
};

class DrogueDeployedMode : public FlightMode {
public:
    void transition();

    int id() { return 0; }
    std::string name() { return "Drogue Deployed"; };
private:
    void deploy_main();
};

class MainDeployedMode : public FlightMode {
public:
    void transition() {};

    int id() { return 0; }
    std::string name() { return "Main Deployed"; };
};

#endif