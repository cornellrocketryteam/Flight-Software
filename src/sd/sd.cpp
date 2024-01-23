#include "sd.hpp"
#include "../constants.hpp"
#include "../core/state.hpp"
#include <cstdio>

bool SD::begin() {
    time_init();

    pSD = sd_get_by_num(0);
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);

    if (fr != FR_OK) {
#ifdef VERBOSE
        printf("SD: %s (%d)\n", FRESULT_str(fr), fr);
#endif
        return false;
    }

    return true;
}

bool SD::log() {
    std::string log;

    // clang-format off
    log += std::to_string(state::flight::timestamp) + ","
        + std::to_string(state::flight::cycle_count) + ","
        + std::to_string(state::flight::mode->id()) + ","

        + std::to_string(state::alt::status) + ","
        + std::to_string(state::alt::altitude) + ","
        + std::to_string(state::alt::pressure) + ","

        + std::to_string(state::imu::status) + ","
        + std::to_string(state::imu::gyro_x) + ","
        + std::to_string(state::imu::gyro_y) + ","
        + std::to_string(state::imu::gyro_z) + ","
        + std::to_string(state::imu::mag_x) + ","
        + std::to_string(state::imu::mag_y) + ","
        + std::to_string(state::imu::mag_z) + ","

        + std::to_string(state::accel::status) + ","
        + std::to_string(state::accel::accel_x) + ","
        + std::to_string(state::accel::accel_y) + ","
        + std::to_string(state::accel::accel_z) + ","

        + std::to_string(state::therm::status) + ","
        + std::to_string(state::therm::temp) + ","
        + std::to_string(state::therm::humidity) + ","

        + std::to_string(state::rfm::init) + ",";
    // clang-format on

    for (std::string event : state::flight::events) {
        log += event + ",";
    }

    FRESULT fr = f_open(&file, constants::sd::filename, FA_OPEN_APPEND | FA_WRITE);

    if (fr != FR_OK && fr != FR_EXIST) {
#ifdef VERBOSE
        printf("SD: %s (%d)\n", FRESULT_str(fr), fr);
#endif
        return false;
    }

    if (f_printf(&file, "%s\n", log.c_str()) < 0) {
#ifdef VERBOSE
        printf("SD: %s (%d)\n", FRESULT_str(fr), fr);
#endif
        return false;
    }

    fr = f_close(&file);
    if (FR_OK != fr) {
#ifdef VERBOSE
        printf("SD: %s (%d)\n", FRESULT_str(fr), fr);
#endif

        return false;
    }
#ifdef VERBOSE
    printf("SD: Log success\n");
#endif

    return true;
}