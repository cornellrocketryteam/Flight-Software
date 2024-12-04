#ifndef UBLOX_DEFS_HPP
#define UBLOX_DEFS_HPP

// TODO: Change representation of UTC time
typedef struct {
    uint32_t utc_time;
    float latitude;
    float longitude;
    float altitude;
    int num_satellites;

    bool valid;
} gnss_data_t;

struct UBXFrame {
    uint8_t sync_1 = 0xB5;
    uint8_t sync_2 = 0x62;
    uint8_t msg_class;
    uint8_t msg_id;
    uint16_t len;
    uint8_t *payload;
    uint8_t ck_a;
    uint8_t ck_b;
};

// Ublox classes
#define UBX_CLASS_ACK (0x05)
#define UBX_CLASS_CFG (0x06)

// Ublox IDs
#define UBX_ACK_ACK (0x01)
#define UBX_ACK_NAK (0x00)

#define UBX_CFG_VALSET (0x8a)

// NMEA message IDs
#define NMEA_STANDARD_GGA (0x00)
#define NMEA_STANDARD_GLL (0x01)
#define NMEA_STANDARD_GSA (0x02)
#define NMEA_STANDARD_GSV (0x03)
#define NMEA_STANDARD_RMC (0x04)
#define NMEA_STANDARD_VTG (0x05)

// Message output configuration
#define CFG_MSGOUT_NMEA_ID_DTM_I2C (0x209100a6)
#define CFG_MSGOUT_NMEA_ID_GBS_I2C (0x209100dd)
#define CFG_MSGOUT_NMEA_ID_GGA_I2C (0x209100ba)
#define CFG_MSGOUT_NMEA_ID_GLL_I2C (0x209100c9)
#define CFG_MSGOUT_NMEA_ID_GNS_I2C (0x209100b5)
#define CFG_MSGOUT_NMEA_ID_GRS_I2C (0x209100ce)
#define CFG_MSGOUT_NMEA_ID_GSA_I2C (0x209100bf)
#define CFG_MSGOUT_NMEA_ID_GST_I2C (0x209100d3)
#define CFG_MSGOUT_NMEA_ID_GSV_I2C (0x209100c4)
#define CFG_MSGOUT_NMEA_ID_RLM_I2C (0x20910400)
#define CFG_MSGOUT_NMEA_ID_RMC_I2C (0x209100ab)
#define CFG_MSGOUT_NMEA_ID_VLW_I2C (0x209100e7)
#define CFG_MSGOUT_NMEA_ID_VTG_I2C (0x209100b0)
#define CFG_MSGOUT_NMEA_ID_ZDA_I2C (0x209100d8)

#endif // UBLOX_DEFS_HPP