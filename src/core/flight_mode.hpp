#ifndef FLIGHT_MODE_HPP_
#define FLIGHT_MODE_HPP_

#include <string>

/**
 * Abstract flight mode class.
 */
class FlightMode {
public:
    /**
     * Updates the current state by polling all sensors. Writes that state to
     * the SD card and transmits it over radio.
     */
    virtual void execute();

    /**
     * Checks for valid transition criteria and updates the current flight mode
     * in state, if necessary.
     */
    virtual void transition() = 0;

    /**
     * A short integer ID for storage and transmission purposes.
     */
    virtual int id() = 0;

    /**
     * A verbose name for debugging purposes.
     */
    virtual std::string name() = 0;
};

/**
 * Flight mode representing the period before arming.
 */
class StartupMode : public FlightMode {
public:
    /**
     * Attempts to begin all modules. Logs failures if necessary.
     */
    void execute();

    /**
     * Transition to standby mode if the arming key has been armed.
     */
    void transition();

    int id() { return 0; }
    std::string name() { return "Startup"; };
};

/**
 * Flight mode representing the period before liftoff.
 */
class StandbyMode : public FlightMode {
public:
    /**
     * Transitions to ascent mode upon acceleration of liftoff.
     */
    void transition();

    int id() { return 1; }
    std::string name() { return "Standby"; };
};

/**
 * Flight mode representing the period between liftoff and apogee.
 */
class AscentMode : public FlightMode {
public:
    /**
     * Checks arming altitude in addition to sensor reading.
     */
    void execute() override;

    /**
     * Transition to drogue deployed mode if apogee has been reached.
     */
    void transition();

    int id() { return 2; }
    std::string name() { return "Ascent"; };

private:
    /**
     * Detects apogee with a moving average filter. Must be above the arming altitude.
     * @return True if apogee has been detected, false otherwise.
     */
    bool apogee_detected();
};

/**
 * Flight mode representing the period after drogue parachute deployment.
 */
class DrogueDeployedMode : public FlightMode {
public:
    /**
     * Transition to main deployed mode if the proper altitude has been reached.
     */
    void transition();

    int id() { return 3; }
    std::string name() { return "Drogue Deployed"; };
};

/**
 * Flight mode representing the period after main parachute deployment.
 */
class MainDeployedMode : public FlightMode {
public:
    /**
     * Final mode. No transition criteria.
     */
    void transition() {};

    int id() { return 4; }
    std::string name() { return "Main Deployed"; };
};

#endif // FLIGHT_MODE_HPP_