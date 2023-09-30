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

#ifndef CANOPEN_LIB_HELPER_H
#define CANOPEN_LIB_HELPER_H

#include "CANopenLibDefines.h"

typedef enum {
  canAdapter_DEFAULT = 0,
  canAdapter_EMS = 1,
  canAdapter_IXXAT = 2,
  canAdapter_KVASER = 3,
  canAdapter_CAN232 = 4,
  canAdapter_CANUSB = 5,
  canAdapter_CASTOR = 6,
  canAdapter_PCAN = 7,
  canAdapter_TINYCAN = 8,
  canAdapter_ZANTHIC = 9
} canAdapter;



void CANOPENDLL_API setAdapter(canAdapter userAdapter);
canAdapter CANOPENDLL_API getAdapter();

#define DLL_DEVELOPMENT 1

#ifndef __cplusplus
#define FALSE   0 
#define TRUE    1
#define NULL    0
typedef unsigned int bool;
#endif


typedef unsigned char       u8;
typedef signed char         s8;
typedef unsigned short int  u16;
typedef signed short int    s16;
typedef unsigned int        u32;
typedef signed int          s32;

#ifdef HAS_LONG_LONG
typedef signed long long    s64;
typedef unsigned long long  u64;
#endif

typedef float               f32;
typedef double              f64;

typedef u32     COBID;
typedef u32     CanOpenErrorCode;

#ifdef HAS_LONG_LONG
void val2buf(u64 val, u8* buf, u8 len);
#endif
void val2buf(u32 val, u8* buf, u8 len);
u32 buf2val(u8* buf, u8 len);
u16 getU16Val(u8* p, u8 offset);
u32 getU32Val(u8* p, u8 offset);
#ifdef HAS_LONG_LONG
u64 getU64Val(u8* p, u8 offset);
#endif
void setU16Val(u16 val, u8* p, u8 offset);
void setU32Val(u32 val, u8* p, u8 offset);
#ifdef HAS_LONG_LONG
void setU64Val(u64 val, u8* p, u8 offset);
#endif

u8 getU8Val(u8* p, u8 offset);
void setU8Val(u8 val, u8* p, u8 offset);
u32 getU24Val(u8* p, u8 offset);
void setU24Val(u32 val, u8* p, u8 offset);


typedef canOpenStatus(*DispatcherCanFuncPtr)(
  void* context,
  unsigned long id,
  unsigned char* data,
  unsigned int dlc,
  unsigned int flags);
typedef canOpenStatus(*ProtocolImplementationStateMachineFuncPtr)(void* context); // Callback for helping the transfer, will be called in same thread as "canFrameConsumer".

void DebugLogToFile(char* string);
canOpenStatus DebugExitErrorValueLogToFile(canOpenStatus code);

#endif



#define   INVALID_NODE_ID   (u8)-1;

