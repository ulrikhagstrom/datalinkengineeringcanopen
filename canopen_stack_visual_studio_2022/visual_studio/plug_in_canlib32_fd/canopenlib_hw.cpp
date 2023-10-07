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

// canlib32.cpp : Defines the entry point for the DLL application.
//

#include "windows.h"
#include "stdio.h"
#include "canlib.h"
#include "canopenlib_hw.h"
#include "CANopenLibHelper.h"



#include <time.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <conio.h>
#include <shlwapi.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif


#ifdef _MANAGED
#pragma managed(pop)
#endif

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
u32 buf2val(u8* buf, u8 len) {
  u32 val = 0;
  buf += len; /* Points to MSB+1 (we use predecrement or CodeGuard will complain on buffer underrun) */
  while (len--)
    val = (val << 8) + *--buf;
  return val;
}


BOOL APIENTRY DllMain(HMODULE hModule,
  DWORD  ul_reason_for_call,
  LPVOID lpReserved
)
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    break;
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}


CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortLibraryInit(void)
{

  /*
  *	Do the initialzation!
  */
  canInitializeLibrary();
  return CANOPEN_OK;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortOpen(int port, canPortHandle* handle)
{
  int kv_handle;

  kv_handle = canOpenChannel(port, canOPEN_ACCEPT_VIRTUAL | canOPEN_CAN_FD);
  if (kv_handle >= 0)
  {
    *handle = kv_handle;
    return CANOPEN_OK;
  }
  else
    return CANOPEN_ERROR_HW_UNDEFINED;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortClose(canPortHandle handle)
{
  canStatus kv_status = canClose((int)handle);
  if (kv_status == canOK)
  {
    return CANOPEN_OK;
  }
  else
    return CANOPEN_ERROR_HW_UNDEFINED;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortBitrateSet(canPortHandle handle, int bitrate)
{
  canStatus kv_status = canERR_INTERNAL;

  switch (bitrate) {
  case 1000000:
    kv_status = canSetBusParamsFd(handle, canFD_BITRATE_1M_80P, 0, 0, 0);
    break;
  case 500000:
    kv_status = canSetBusParamsFd(handle, canFD_BITRATE_500K_80P, 0, 0, 0);
    break;
  case 250000:
    kv_status = canSetBitrate(handle, 250);
    break;
  case 125000:
    kv_status = canSetBitrate(handle, 125000);
    break;
  }

  if (kv_status == canOK)
    return CANOPEN_OK;
  else
    return CANOPEN_ERROR_HW_UNDEFINED;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortEcho(canPortHandle handle, bool enabled)
{
  DWORD flag = enabled ? 1 : 0;
  canStatus kv_status = canIoCtl((int)handle, canIOCTL_SET_TXACK, &flag, sizeof(DWORD));
  if (kv_status == canOK)
    return CANOPEN_OK;
  else
    return CANOPEN_ERROR_HW_UNDEFINED;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOn(canPortHandle handle)
{
  canStatus kv_status = canBusOn((int)handle);
  if (kv_status == canOK)
    return CANOPEN_OK;
  else
    return CANOPEN_ERROR;

}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOff(canPortHandle handle)
{
  canStatus kv_status = canBusOff((int)handle);
  if (kv_status == canOK)
    return CANOPEN_OK;
  else
    return CANOPEN_ERROR;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortWrite(canPortHandle handle,
  long id,
  void* msg,
  unsigned int dlc,
  unsigned int flags)
{

  unsigned int kv_flags = 0;
  if (flags & CAN_MSG_EXT)
    kv_flags |= canMSG_EXT;
  if (flags & CAN_MSG_RTR)
    kv_flags |= canMSG_RTR;

  canStatus kv_status = canWrite((int)handle,
    id,
    msg,
    dlc,
    kv_flags);
  if (kv_status == canOK)
    return CANOPEN_OK;
  else
    return CANOPEN_ERROR;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortRead(canPortHandle handle,
  long* id,
  void* msg,
  unsigned int* dlc,
  unsigned int* flags)
{

  unsigned int kv_flags = 0;
  DWORD kv_time = 0;

  canStatus kv_status = canRead((int)handle,
    id,
    msg,
    dlc,
    &kv_flags,
    &kv_time);

  if (kv_status == canOK)
  {
    if (kv_flags & canMSG_ERROR_FRAME)
    {
      return CANOPEN_ERROR_NO_MESSAGE;
    }
    *flags = 0;
    if (kv_flags & canMSG_EXT)
      *flags |= CAN_MSG_EXT;
    if (kv_flags & canMSG_RTR)
      *flags |= CAN_MSG_RTR;
    return CANOPEN_OK;
  }
  else if (kv_status == canERR_NOMSG)
  {
    return CANOPEN_ERROR_NO_MESSAGE;
  }
  else
  {
    return CANOPEN_ERROR;
  }
}

CANOPENLIB_HW_API   canOpenStatus  __stdcall canPortGetSerialNumber(canPortHandle handle, char* buffer, int bufferLen)
{
  memset(buffer, 0, bufferLen);
  canStatus kv_status = canGetChannelData((int)handle, canCHANNELDATA_CARD_SERIAL_NO, buffer, bufferLen);
  u32 serial_number = buf2val((u8*)buffer, 2);
  sprintf_s(buffer, bufferLen, "%d", serial_number);
  if (kv_status == canOK)
    return CANOPEN_OK;
  else
    return CANOPEN_ERROR;
}