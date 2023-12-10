#ifndef FLIGHT_MODE_
#define FLIGHT_MODE_

#include <string>

class FlightMode {
public:
    virtual void execute();
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

    int id() { return 1; }
    std::string name() { return "Standby"; };
};

class PoweredAscentMode : public FlightMode {
public:
    void transition();

    int id() { return 2; }
    std::string name() { return "Powered Ascent"; };
};

class CoastingMode : public FlightMode {
public:
    void transition();

    int id() { return 3; }
    std::string name() { return "Coasting"; };

private:
    bool apogee_detected();
};

class DrogueDeployedMode : public FlightMode {
public:
    void transition();

    int id() { return 4; }
    std::string name() { return "Drogue Deployed"; };
};

class MainDeployedMode : public FlightMode {
public:
    void transition() {};

    int id() { return 5; }
    std::string name() { return "Main Deployed"; };
};

#endif // FLIGHT_MODE_