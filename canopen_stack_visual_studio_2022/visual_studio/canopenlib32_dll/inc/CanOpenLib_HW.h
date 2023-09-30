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

#include "CANopenLibDefines.h"

#ifndef CANOPENLIB_HW_H
#define CANOPENLIB_HW_H

#ifdef CANLIB32_EXPORTS
#define CANOPENLIB_HW_API  /*extern "C"*/ __declspec(dllexport) 
#else
#define CANOPENLIB_HW_API __declspec(dllimport)
#endif




/***************
*
*   CANopen-lib hardware port plugin DLL (canopenlib_hw.dll).
*
****************/

typedef int      canPortHandle;



//
// CAN informaiton flags.
//
#define CAN_MSG_RTR              0x0001      // Message is a remote request
#define CAN_MSG_EXT              0x0002      // Message has a standard ID


CANOPENLIB_HW_API   canOpenStatus        __stdcall canPortLibraryInit(void);

CANOPENLIB_HW_API   canOpenStatus        __stdcall canPortOpen(int port, canPortHandle* handle);
CANOPENLIB_HW_API   canOpenStatus        __stdcall canPortClose(canPortHandle handle);

CANOPENLIB_HW_API   canOpenStatus        __stdcall canPortBitrateSet(canPortHandle handle, int bitrate);

CANOPENLIB_HW_API   canOpenStatus        __stdcall canPortEcho(canPortHandle handle, bool enabled);

CANOPENLIB_HW_API   canOpenStatus        __stdcall canPortGoBusOn(canPortHandle handle);
CANOPENLIB_HW_API   canOpenStatus        __stdcall canPortGoBusOff(canPortHandle handle);

CANOPENLIB_HW_API   canOpenStatus        __stdcall canPortWrite(canPortHandle handle,
  long id,
  void* msg,
  unsigned int dlc,
  unsigned int flags);

CANOPENLIB_HW_API   canOpenStatus        __stdcall canPortRead(canPortHandle handle,
  long* id,
  void* msg,
  unsigned int* dlc,
  unsigned int* flags);

CANOPENLIB_HW_API   canOpenStatus        __stdcall canPortGetSerialNumber(canPortHandle handle, char* buffer, int bufferLen);

#endif
