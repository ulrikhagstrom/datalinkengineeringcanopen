/*             _____        _        _      _       _
              |  __ \      | |      | |    (_)     | |
              | |  | | __ _| |_ __ _| |     _ _ __ | | __
              | |  | |/ _` | __/ _` | |    | | '_ \| |/ /
              | |__| | (_| | || (_| | |____| | | | |   <
              |_____/ \__,_|\__\__,_|______|_|_| |_|_|\_\
         ______             _                      _
        |  ____|           (_)                    (_)
        | |__   _ __   __ _ _ _ __   ___  ___ _ __ _ _ __   __ _
        |  __| | '_ \ / _` | | '_ \ / _ \/ _ \ '__| | '_ \ / _` |
        | |____| | | | (_| | | | | |  __/  __/ |  | | | | | (_| |
        |______|_| |_|\__, |_|_| |_|\___|\___|_|  |_|_| |_|\__, |
                       __/ |                                __/ |
                      |___/                                |___/

      Web: http://www.datalink.se E-mail: ulrik.hagstrom@datalink.se

    *******************************************************************
    *    CANopen API (C++/C#) distributed by Datalink Enginnering.    *
    *             Copyright (C) 2009-2013 Ulrik Hagström.             *
    *******************************************************************
*/

#ifndef LSS_DEFINES_H
#define LSS_DEFINES_H

#define   LSS_CMD_SWITCH_STATE_GLOBAL 0x04
#define   LSS_CMD_SWITCH_SELECTIVE_VENDOR 64
#define   LSS_CMD_SWITCH_SELECTIVE_PRODUCT 65
#define   LSS_CMD_SWITCH_SELECTIVE_REVISION 66
#define   LSS_CMD_SWITCH_SELECTIVE_SERIAL 67
#define   LSS_CMD_SWITCH_SELECTIVE_CONFIRMATION 68

#define   LSS_CMD_CONFIGURE_NODE_ID 17
#define   LSS_CMD_CONFIGURE_TIMING_PARAMETERS 19
#define   LSS_CMD_ACTIVATE_TIMING_PARAMETERS 21
#define   LSS_CMD_STORE_CONFIGURATION 23

#define   LSS_MODE_WAITING_STATE 0
#define   LSS_MODE_CONFIGURATION_STATE 1

#define   LSS_TIMING_ERROR_OK 0
#define   LSS_TIMING_ERROR_UNSUPPORTED_BITRATE 1
#define   LSS_TIMING_ERROR_IMPLEMENTATION_SPESIFIC 0xFF

#define   LSS_TIMING_TABLE0_1000K 0
#define   LSS_TIMING_TABLE0_800K 1
#define   LSS_TIMING_TABLE0_500K 2
#define   LSS_TIMING_TABLE0_250K 3
#define   LSS_TIMING_TABLE0_125K 4
// LSS_TIMING_TABLE0_RESERVED 5

#define   LSS_TIMING_TABLE0_50K 6
#define   LSS_TIMING_TABLE0_20K 7
#define   LSS_TIMING_TABLE0_10K 8
// LSS_TIMING_TABLE0_AUTOMATIC_BITRATE_DETECTION 9 not supported

#endif //LSS_DEFINES_H
