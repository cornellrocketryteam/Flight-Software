#include "sim_data.hpp"

float SimData::get_alt() {
    return alt_sim[alt_i++];
}

float SimData::get_accel() {
    return accel_z_sim[accel_i++];
}