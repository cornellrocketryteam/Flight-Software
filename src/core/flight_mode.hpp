#ifndef FLIGHT_MODE_HPP_
#define FLIGHT_MODE_HPP_

#include "modules.hpp"
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
     * Checks the return result from a sensor reading and updates state on
     * failure, if necessary.
     * @param sensor The enum describing which sensor is being checked
     * @param ret The return result from the sensor
     */
    void check_sensor(enum Sensor sensor, bool ret);

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

    void run_filter();

    int next_alt;

    double altitude_sum;

    double filtered_alt1 = 0;
    double filtered_alt2 = 0;
    double filtered_alt3 = 0;
};

/**
 * Flight mode representing the period after drogue parachute deployment.
 */
class DrogueDeployedMode : public FlightMode {
public:
    /**
     * Checks ematch on-time in addition to sensor reading.
     */
    void execute() override;
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
     * Checks e-match on-time in addition to sensor reading.
     */
    void execute() override;

        /**
     * Final mode. No transition criteria.
     */
    void transition() {};

    int id() { return 4; }
    std::string name() { return "Main Deployed"; };
};

/**
 * Flight mode representating situations where flight functions are not able to proceed.
 */
class FaultMode : public FlightMode {
public:
    /**
     * Flight functionality is disabled. No transition criteria.
     */
    void transition() {};

    int id() { return 5; };
    std::string name() { return "Fault"; };
};

#endif // FLIGHT_MODE_HPP_