#include "state.hpp"
#include "../pins.hpp"

#include <stdint.h>

class FlightMode;

namespace state {
    namespace flight {
        StartupMode startup_class;
        StandbyMode standby_class;
        PoweredAscentMode powered_ascent_class;
        CoastingMode coasting_class;
        DrogueDeployedMode drogue_deployed_class;
        MainDeployedMode main_deployed_class;

        FlightMode *startup = &startup_class;
        FlightMode *standby = &standby_class;
        FlightMode *powered_ascent = &powered_ascent_class;
        FlightMode *coasting = &coasting_class;
        FlightMode *drogue_deployed = &drogue_deployed_class;
        FlightMode *main_deployed = &main_deployed_class;

        FlightMode *mode = startup;

        bool key_armed = false;
        bool armed = false;
    }
    namespace altimeter {
        bool init = false;
        float pressure = -1;
        float altitude = -1;
    }
    namespace gps {
        bool init = false;
        float latitude = -1;
        float longitude = -1;
        float altitude = -1;
        uint8_t siv = -1;
    }
    namespace imu {
        bool init = false;
        float mag_x = -1;
        float mag_y = -1;
        float mag_z = -1;
        float gyro_x = -1;
        float gyro_y = -1;
        float gyro_z = -1;

        BNO055 imu(I2C_PORT);
    }
    namespace accel {
        bool init = false;
        float accel_x = -1;
        float accel_y = -1;
        float accel_z = -1;

        LIS3DH accel(I2C_PORT);
    }
    namespace therm {
        bool init = false;
        float temp = -1;
        float humidity = -1;

        Si7021 therm(I2C_PORT);
    }
    namespace sd {
        bool init = false;
    }
    namespace rfm {
        bool init = false;
    }
};
