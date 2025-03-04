/**
 * @file sd.cpp
 * @author csg83
 *
 * @brief SD card-related functionality
 */

#include "sd.hpp"
#include "constants.hpp"
#include "state.hpp"
#include <cstdio>

void SD::begin() {
    fr = f_mount(&fs, "", 1);

    if (fr != FR_OK) {
        logf("SD Mount Error: %s (%d)\n", FRESULT_str(fr), fr);
        events.push(Event::sd_init_fail);
        return;
    }
    state::sd::init = true;
}

void SD::log() {
    if (writes_count == constants::file_writes_threshold) {
        state::sd::current_file++;
        writes_count = 0;
    }

    data.clear();

    // clang-format off
    data += std::to_string(state::flight::timestamp) + ","
         + std::to_string(state::flight::cycle_count) + ","
         + std::to_string(state::flight::mode->id()) + ","

         + std::to_string(state::alt::status) + ","
         + std::to_string(state::alt::altitude) + ","

         + std::to_string(state::gps::status) + ","
         + std::to_string(state::gps::data.lat) + ","
         + std::to_string(state::gps::data.lon) + ","

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

         + std::to_string(state::alt::temp) + ","

         + std::to_string(state::fram::init) + ",";
    // clang-format on

    uint32_t events_bitfield = events.get();
    for (uint i = 0; i < sizeof(events_bitfield) * 8; i++) {
        if (events_bitfield & (1u << i)) {
            data += std::to_string(i) + ",";
        }
    }

    char filename[10];
    sprintf(filename, "%d_%d.csv", state::flight::boot_count, state::sd::current_file);

    fr = f_open(&log_file, (const char *)filename, FA_OPEN_APPEND | FA_WRITE);

    if (fr != FR_OK && fr != FR_EXIST) {
        logf("SD Open Error: %s (%d)\n", FRESULT_str(fr), fr);
        state::sd::failed_writes++;
        events.push(Event::sd_write_fail);
        if (state::sd::failed_writes == constants::max_failed_writes) {
            state::sd::init = false;
        }
    }

    if (f_printf(&log_file, "%s\n", data.c_str()) < 0) {
        logf("SD Print Error: %s (%d)\n", FRESULT_str(fr), fr);
        state::sd::failed_writes++;
        events.push(Event::sd_write_fail);
        if (state::sd::failed_writes == constants::max_failed_writes) {
            state::sd::init = false;
        }
    }

    fr = f_close(&log_file);
    if (FR_OK != fr) {
        logf("SD Close Error: %s (%d)\n", FRESULT_str(fr), fr);
        state::sd::failed_writes++;
        events.push(Event::sd_write_fail);
        if (state::sd::failed_writes == constants::max_failed_writes) {
            state::sd::init = false;
        }
    }

    writes_count++;
}

bool SD::clear_card() {
    DIR dir;
    FILINFO fno;

    fr = f_opendir(&dir, "/");
    if (fr != FR_OK && fr != FR_EXIST) {
        logf("SD Open Error: %s (%d)\n", FRESULT_str(fr), fr);
        events.push(Event::sd_write_fail);
        return false;
    }

    while (true) {
        fr = f_readdir(&dir, &fno);
        if (fr != FR_OK || fno.fname[0] == 0) break;
        if (fno.fname[0] == '.') continue;

        fr = f_unlink(fno.fname);
        if (fr != FR_OK) {
            logf("SD Delete Error: Failed to delete %s\n", fno.fname);
            events.push(Event::sd_write_fail);
            return false;
        }
    }

    return true;
}