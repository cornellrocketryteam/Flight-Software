/**
 * @file flight_mode.hpp
 * @author csg83, gb486
 *
 * @brief Flight mode logic and transitions
 */

#ifndef FLIGHT_MODE_HPP
#define FLIGHT_MODE_HPP

#include "modules.hpp"
#include <cstdint>
#include <string>

/**
 * Abstract flight mode class
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
     * Switches to the specified flight mode and updates
     * the boot file, if able.
     * @param mode The flight mode to switch to
     */
    void to_mode(FlightMode *mode);

    /**
     * A short integer ID for storage and transmission purposes.
     */
    virtual uint8_t id() = 0;

    /**
     * A verbose name for debugging purposes.
     */
    virtual std::string name() = 0;
};

/**
 * Flight mode representing the period before arming
 */
class StartupMode : public FlightMode {
public:
    /**
     * Attempts to begin all modules. Logs failures if necessary.
     */
    void execute();

    /**
     * Transition to standby mode if the arming key has been armed, or
     * if the old flight mode from a past boot was in-flight or fault.
     */
    void transition();

    uint8_t id() { return 0; }
    std::string name() { return "Startup"; };
};

/**
 * Flight mode representing the period before liftoff
 */
class StandbyMode : public FlightMode {
public:
    /**
     * Reads commands and sends telemetry over umbilical.
     */
    void execute();
    /**
     * Transitions to ascent mode upon acceleration of liftoff.
     */
    void transition();

    uint8_t id() { return 1; }
    std::string name() { return "Standby"; };
};

/**
 * Flight mode representing the period between liftoff and apogee
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

    uint8_t id() { return 2; }
    std::string name() { return "Ascent"; };

private:
    /**
     * Stores values for calculating the moving average.
     */
    float alt_buffer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    /**
     * Stores the sum of the current values in alt_buffer.
     */
    float alt_sum = 0;

    /**
     * The current index of accel_buffer.
     */
    int index = 0;

    /**
     * The current counter of data points in a sample.
     */
    uint8_t count = 0;

    /**
     * Stores points of moving averages of altitudes.
     */
    float filtered_alt[3] = {-1, -1, -1};
};

/**
 * Flight mode representing the period after drogue parachute deployment
 */
class DrogueDeployedMode : public FlightMode {
public:
    /**
     * Transition to main deployed mode if the proper altitude has been reached.
     */
    void transition();

    uint8_t id() { return 3; }
    std::string name() { return "Drogue Deployed"; };

private:
    /**
     * Tracks the number of cycles after drogue deployment.
     */
    uint16_t main_cycle_count = 0;
};

/**
 * Flight mode representing the period after main parachute deployment
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

    uint8_t id() { return 4; }
    std::string name() { return "Main Deployed"; };

private:
    /**
     * Tracks the number of cycles over which to log data in this mode.
     */
    uint16_t log_cycle_count = 0;

#ifdef USE_BLIMS
    // Data to send to the blims module
    BLIMSDataIn to_blims_data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
};

/**
 * Flight mode representating situations where flight functions are not able to proceed
 */
class FaultMode : public FlightMode {
public:
    /**
     * Checks for commands in addition to sensor reading.
     */
    void execute() override;

    /**
     * Flight functionality is disabled. No transition criteria.
     */
    void transition() {};

    uint8_t id() { return 5; };
    std::string name() { return "Fault"; };
};

#endif // FLIGHT_MODE_HPP