/**
 * @file sensor.cpp
 * @author csg83
 *
 * @brief Sensor-related functionality
 */

#include "sensor.hpp"
#include "pins.hpp"
#include "state.hpp"

Altimeter::Altimeter() : alt(I2C_PORT) {}

bool Altimeter::begin() {
    if (alt.begin()) {
        state::alt::status = VALID;
        // The first reading of the BMP388 is always garbage
        alt.read_pressure(&state::alt::ref_pressure);
        sleep_ms(100);
        alt.read_pressure(&state::alt::ref_pressure);
        return true;
    } else {
        state::flight::events.emplace_back(Event::alt_init_fail);
        return false;
    }
}

void Altimeter::update_ref_pressure() {
    if (alt.read_pressure(&pressure)) {
        state::alt::ref_pressure = alpha * pressure + (1 - alpha) * state::alt::ref_pressure;
        if (state::alt::status == INVALID) {
            state::alt::status = VALID;
        }
    } else {
        state::alt::failed_reads++;
        state::alt::status = INVALID;
        state::flight::events.emplace_back(Event::alt_read_fail);
        if (state::alt::failed_reads >= constants::max_failed_reads) {
            state::alt::status = OFF;
            // to_mode(state::flight::fault);
        }
    }
}

void Altimeter::read_altitude() {
    if (alt.read_data(&state::alt::altitude, &state::alt::temp, state::alt::ref_pressure)) {
        if (state::alt::status == INVALID) {
            state::alt::status = VALID;
        }
    } else {
        state::alt::failed_reads++;
        state::alt::status = INVALID;
        state::flight::events.emplace_back(Event::alt_read_fail);
        if (state::alt::failed_reads >= constants::max_failed_reads) {
            state::alt::status = OFF;
            // to_mode(state::flight::fault);
        }
    }
}

GPS::GPS() : gnss(I2C_PORT) {};

bool GPS::begin() {
    return true;
}

Accel::Accel() : accel(I2C_PORT) {}

bool Accel::begin() {
    if (accel.begin()) {
        state::accel::status = VALID;
        return true;
    } else {
        state::flight::events.emplace_back(Event::accel_init_fail);
        return false;
    }
}

void Accel::read_accel() {
    if (accel.read_accel(
            &state::accel::accel_x,
            &state::accel::accel_y,
            &state::accel::accel_z
        )) {
        if (state::accel::status == INVALID) {
            state::accel::status = VALID;
        }
    } else {
        state::accel::failed_reads++;
        state::accel::status = INVALID;
        state::flight::events.emplace_back(Event::accel_read_fail);
        if (state::accel::failed_reads >= constants::max_failed_reads) {
            state::accel::status = OFF;
        }
    }
}

IMU::IMU() : imu(I2C_PORT) {}

bool IMU::begin() {
    if (imu.begin()) {
        state::imu::status = VALID;
        return true;
    } else {
        state::flight::events.emplace_back(Event::imu_init_fail);
        return false;
    }
}

void IMU::read_gyro() {
    if (imu.read_gyro(
            &state::imu::gyro_x,
            &state::imu::gyro_y,
            &state::imu::gyro_z
        )) {
    }
}

void IMU::read_accel() {
    if (imu.read_accel(
            &state::imu::accel_x,
            &state::imu::accel_y,
            &state::imu::accel_z
        )) {
    }
}

void IMU::read_orientation() {
    if (imu.read_orientation(
            &state::imu::orientation_x,
            &state::imu::orientation_y,
            &state::imu::orientation_z
        )) {
    }
}

void IMU::read_gravity() {
    if (imu.read_gravity(
            &state::imu::gravity_x,
            &state::imu::gravity_y,
            &state::imu::gravity_z
        )) {
    }
}

ADC::ADC() : adc(I2C_PORT) {}

bool ADC::begin() {
    if (adc.begin()) {
        state::adc::status = VALID;
        return true;
    } else {
        state::flight::events.emplace_back(Event::adc_init_fail);
        return false;
    }
}

void ADC::read_data() {
    if (adc.read_data(channels, sizeof(channels), data)) {
        state::adc::pressure_pt3 = ((float)data[0]) * 1000 / 1667;
        state::adc::pressure_pt4 = ((float)data[1]) * 1000 / 1667;
        state::adc::temp_rtd = data[2];
    } else {
        state::adc::failed_reads++;
        state::adc::status = INVALID;
        state::flight::events.emplace_back(Event::adc_read_fail);
        if (state::adc::failed_reads >= constants::max_failed_reads) {
            state::adc::status = OFF;
        }
    }
}