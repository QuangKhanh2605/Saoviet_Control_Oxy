

#ifndef USER_OBIS_SV_H
#define USER_OBIS_SV_H

#include "user_util.h"


/*======== Define =================*/

#define OBIS_TIME_DEVICE    0x01
#define OBIS_TIME_EMET      0x02
#define OBIS_MODE           0x03
#define OBIS_DEV_VOL1       0x04
#define OBIS_DEV_VOL2       0x05
#define OBIS_RSSI_1         0x06
#define OBIS_RSSI_2         0x07
#define OBIS_FREQ_SEND      0x08
#define OBIS_SERI_DEV       0x09
#define OBIS_SERI_SENSOR    0x0A
#define OBIS_SERI_SIM       0x0B
#define OBIS_SER_ADD        0x0C
#define OBIS_GPS_LOC        0x0D

#define OBIS_EMET_VOL               0x20
#define OBIS_EMET_VOL_A             0x21
#define OBIS_EMET_VOL_B             0x22
#define OBIS_EMET_VOL_C             0x23

#define OBIS_EMET_CUR               0x24
#define OBIS_EMET_CUR_A             0x25
#define OBIS_EMET_CUR_B             0x26
#define OBIS_EMET_CUR_C             0x27

#define OBIS_EMET_FREQ              0x28

#define OBIS_EMET_FACTOR            0x29
#define OBIS_EMET_FACTOR_A          0x2A
#define OBIS_EMET_FACTOR_B          0x2B
#define OBIS_EMET_FACTOR_C          0x2C

#define OBIS_EMET_ACT_W             0x2D
#define OBIS_EMET_ACT_W_A           0x2E
#define OBIS_EMET_ACT_W_B           0x2F
#define OBIS_EMET_ACT_W_C           0x30

#define OBIS_EMET_ACT_Wh            0x31
#define OBIS_EMET_ACT_Wh_A          0x32
#define OBIS_EMET_ACT_Wh_B          0x33
#define OBIS_EMET_ACT_Wh_C          0x34


#define OBIS_WM_PULSE               0x60
#define OBIS_WM_FLOW                0x61
#define OBIS_WM_PRESSURE            0x62

#define OBIS_WM_LEVEL_UNIT          0x63
#define OBIS_WM_LEVEL_VAL_SENSOR    0x64
#define OBIS_WM_LEVEL_VAL_REAL      0x65


#define OBIS_ENVI_TEMP_1            0x70
#define OBIS_ENVI_TEMP_2            0x71
#define OBIS_ENVI_HUMI_1            0x72
#define OBIS_ENVI_HUMI_2            0x6E

/*================ Obis Control Oxy ================*/
#define OBIS_ENVI_STATE_MACHINE     0xB4

#define OBIS_ENVI_OXY_TEMPERATURE   0x70
#define OBIS_ENVI_PH_WATER          0x73
#define OBIS_ENVI_OXY_MG_L          0x74
#define OBIS_ENVI_SALINITY          0x75
#define OBIS_ENVI_OXY_PERCENT       0x8B


#define OBIS_ENVI_SET_OXY_UPPER     0xB2
#define OBIS_ENVI_SET_OXY_LOWER     0xB3

#define OBIS_WARNING_OXY_LOWER      0xD1
#define OBIS_WARNING_POWER          0xD3
#define OBIS_WARNING_CONNECT_SENSOR 0xD4

#define OBIS_STATE_OXY_1            0x90
#define OBIS_STATE_OXY_2            0x91
#define OBIS_STATE_OXY_3            0x92
#define OBIS_STATE_OXY_4            0x93

#define OBIS_ENVI_ID_SERVER         0xE1


#endif