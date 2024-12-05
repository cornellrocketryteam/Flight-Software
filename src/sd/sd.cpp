#include "sd.hpp"
#include "../constants.hpp"
#include "../core/state.hpp"
#include <cstdio>
#include <sstream>

bool SD::begin() {
    FRESULT fr = f_mount(&fs, "", 1);

    if (fr != FR_OK) {
        logf("SD mount: %s (%d)\n", FRESULT_str(fr), fr);
        return false;
    }

    FRESULT exists = f_stat(constants::boot_filename, NULL);
    fr = f_open(&boot_file, constants::boot_filename, FA_OPEN_ALWAYS | FA_READ);

    if (exists == FR_OK) {
        char buffer[8];
        (void)f_gets(buffer, sizeof(buffer), &boot_file);

        std::string buffer_str(buffer);
        std::stringstream ss(buffer_str);

        ss >> state::flight::boot_count;
        state::flight::boot_count++;
        ss.ignore();
        ss >> state::flight::old_mode;
    }

    fr = f_close(&boot_file);
    if (FR_OK != fr) {
        logf("SD boot close: %s (%d)\n", FRESULT_str(fr), fr);
        return false;
    }

    return true;
}

bool SD::log() {

    if (writes_count == constants::file_writes_threshold) {
        state::sd::current_file++;
        writes_count = 0;
    }

    std::string log;

    // clang-format off
    log += std::to_string(state::flight::timestamp) + ","
        + std::to_string(state::flight::cycle_count) + ","
        + std::to_string(state::flight::mode->id()) + ","

        + std::to_string(state::alt::status) + ","
        + std::to_string(state::alt::altitude) + ","
        + std::to_string(state::alt::pressure) + ","

        + std::to_string(state::gps::status) + ","
        + std::to_string(state::gps::data.latitude) + ","
        + std::to_string(state::gps::data.longitude) + ","

        + std::to_string(state::imu::status) + ","
        + std::to_string(state::imu::gyro_x) + ","
        + std::to_string(state::imu::gyro_y) + ","
        + std::to_string(state::imu::gyro_z) + ","
        + std::to_string(state::imu::accel_x) + ","
        + std::to_string(state::imu::accel_y) + ","
        + std::to_string(state::imu::accel_z) + ","
        + std::to_string(state::imu::orientation_x) + ","
        + std::to_string(state::imu::orientation_y) + ","
        + std::to_string(state::imu::orientation_z) + ","
        + std::to_string(state::imu::gravity_x) + ","
        + std::to_string(state::imu::gravity_y) + ","
        + std::to_string(state::imu::gravity_z) + ","

        + std::to_string(state::accel::status) + ","
        + std::to_string(state::accel::accel_x) + ","
        + std::to_string(state::accel::accel_y) + ","
        + std::to_string(state::accel::accel_z) + ","

        + std::to_string(state::therm::status) + ","
        + std::to_string(state::therm::temp) + ","

        + std::to_string(state::blims::motor_position) + ","

        + std::to_string(state::rfm::init) + ",";
    // clang-format on

    for (Event event : state::flight::events) {
        log += std::to_string(static_cast<uint8_t>(event)) + ",";
    }

    char filename[10];
    sprintf(filename, "%d_%d.csv", state::flight::boot_count, state::sd::current_file);

    FRESULT fr = f_open(&log_file, (const char *)filename, FA_OPEN_APPEND | FA_WRITE);

    if (fr != FR_OK && fr != FR_EXIST) {
        logf("SD log open: %s (%d)\n", FRESULT_str(fr), fr);
        return false;
    }

    if (f_printf(&log_file, "%s\n", log.c_str()) < 0) {
        logf("SD log print: %s (%d)\n", FRESULT_str(fr), fr);
        return false;
    }

    fr = f_close(&log_file);
    if (FR_OK != fr) {
        logf("SD log close: %s (%d)\n", FRESULT_str(fr), fr);
        return false;
    }

    writes_count++;

    logf("SD: Log success\n");

    return true;
}

bool SD::write_mode() {
    FRESULT fr = f_open(&boot_file, constants::boot_filename, FA_OPEN_ALWAYS | FA_WRITE);

    if (fr != FR_OK && fr != FR_EXIST) {
        logf("SD boot open: %s (%d)\n", FRESULT_str(fr), fr);
        return false;
    }

    std::string boot_str = std::to_string(state::flight::boot_count) + "," + std::to_string(state::flight::mode->id());
    if (f_puts(boot_str.c_str(), &boot_file) < 0) {
        logf("SD boot write: %s (%d)\n", FRESULT_str(fr), fr);
        return false;
    }

    fr = f_close(&boot_file);
    if (FR_OK != fr) {
        logf("SD boot close: %s (%d)\n", FRESULT_str(fr), fr);
        return false;
    }

    return true;
}