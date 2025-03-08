#include "sim_data.hpp"

float SimData::get_alt() {
    if (alt_i == 2431) {
        return -2;
    }
    return alt_sim[alt_i++];
}