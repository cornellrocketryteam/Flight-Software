#include "sim_data.hpp"

float SimData::get_alt() {
    if (alt_i == 1056) {
        return -2;
    }
    return alt_sim[alt_i++];
}

float SimData::get_accel() {
    if (accel_i == 1056) {
        return -2;
    }
    return accel_z_sim[accel_i++];
}