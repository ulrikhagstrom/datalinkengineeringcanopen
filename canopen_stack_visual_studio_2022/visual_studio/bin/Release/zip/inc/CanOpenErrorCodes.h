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

#ifndef CANOPEN_ERROR_CODES_H
#define CANOPEN_ERROR_CODES_H

/******************
* CANopen Errors, p.9-27, DS301.
*******************/

#define   TOGGLE_BIT_NOT_ALTERED            0x05030000  
#define   SDO_PROTOCOL_TIMED_OUT            0x05040000  
#define   COMMAND_SPECIFIER_UNKNOWN         0x05040001  
#define   INVALID_BLOCK_SIZE                0x05040002  
#define   INVALID_SEQUENCE_NUMBER           0x05040003  
#define   CRC_ERROR                         0x05040004  
#define   OUT_OF_MEMORY                     0x05040005  
#define   UNSUPPORTED_ACCESS_TO_AN_OBJECT   0x06010000  
#define   ATTEMPT_TO_READ_A_WRITE_ONLY_OBJ  0x06010001  
#define   ATTEMPT_TO_WRITE_A_READ_ONLY_OBJ  0x06010002
#define   SUBINDEX_DOES_NOT_EXIST           0x06090011
#define   DATA_CAN_NOT_BE_STORED            0x08000020


#endif




