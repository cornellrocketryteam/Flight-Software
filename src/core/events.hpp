

#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <stdint.h>
enum class Event : uint8_t {
    alt_armed = 0,

    alt_init_fail = 1,
    alt_read_fail = 2,

    gps_init_fail = 3,
    gps_read_fail = 4,

    imu_init_fail = 5,
    imu_read_fail = 6,

    accel_init_fail = 7,
    accel_read_fail = 8,

    adc_init_fail = 9,
    adc_read_fail = 10,

    fram_init_fail = 11,
    fram_read_fail = 12,
    fram_write_fail = 13,

    sd_init_fail = 14,
    sd_write_fail = 15,

    mav_actuated = 16,
    sv_actuated = 17,

    main_deploy_wait_end = 18,
    main_log_shutoff = 19,
    cycle_overflow = 20,

    unknown_command_received = 21,
    launch_command_received = 22,
    mav_command_received = 23,
    sv_command_received = 24,
    safe_command_received = 25,
    reset_card_command_received = 26,
    reset_fram_command_received = 27,
    state_change_command_received = 28,

    umbilical_disconnected = 29
};

class EventBuffer {
public:
    void push(Event event);
    uint32_t get();
    void clear();
    bool is_empty();

private:
    uint32_t buffer;
};

#endif //EVENTS_HPP