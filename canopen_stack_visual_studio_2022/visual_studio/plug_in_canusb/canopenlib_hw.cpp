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
#include "canopenlib_hw.h"
#include "lawicel_can.h"
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

#define MAX_CAN_DEVICES     2
#define RX_QUEUE_SIZE       100

typedef struct
{
  bool in_use;
  int echo_enabled;
  char bitrate[10];
  CRITICAL_SECTION rx_queue_cs;
  CANMsg rx_queue[RX_QUEUE_SIZE];
  int rx_queue_put_pos;
  int rx_queue_get_pos;
  bool can_bus_on;
  char serialNumber[20];
  bool adapterExists;
  CANHANDLE adapterHandle;
} CanPortDataStruct;

static CanPortDataStruct can_port_data_devices[MAX_CAN_DEVICES];
static bool libraryIsInitiated = false;

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

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus putCanMessageInQueue(CanPortDataStruct* p, CANMsg* can_frame)
{
  canOpenStatus res = CANOPEN_OK;

  EnterCriticalSection(&p->rx_queue_cs); // We need to imlplement a "faked" echo mechanism.

  int put_pos = p->rx_queue_put_pos;

  if (++put_pos >= RX_QUEUE_SIZE)
    put_pos = 0;

  if (put_pos != p->rx_queue_get_pos) {
    memcpy(&(p->rx_queue[p->rx_queue_put_pos]), can_frame, sizeof(CANMsg));
    p->rx_queue_put_pos = put_pos;
    res = CANOPEN_OK;
  }
  else {
    res = CANOPEN_ERROR_CAN_LAYER_OVRFLOW;
  }

  LeaveCriticalSection(&p->rx_queue_cs);

  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus getCanMessageFromQueue(CanPortDataStruct* p,
  CANMsg* can_frame)
{
  canOpenStatus res;
  int get_pos = p->rx_queue_get_pos;

  if (get_pos != p->rx_queue_put_pos) {
    memcpy(can_frame, &(p->rx_queue[p->rx_queue_get_pos]), sizeof(CANMsg));
    if (++get_pos >= RX_QUEUE_SIZE)
      get_pos = 0;
    p->rx_queue_get_pos = get_pos;
    res = CANOPEN_OK;
  }
  else {
    res = CANOPEN_ERROR_NO_MESSAGE;
  }
  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortLibraryInit(void)
{
  canOpenStatus canopen_res = CANOPEN_OK;
  if (libraryIsInitiated == FALSE)
  {
    int handle = -1; // the handle obtained for the opened channel
    int totalAdapters = 0;

    // Make sure that all CAN ports are available.
    for (int i = 0; i < MAX_CAN_DEVICES; i++) {
      can_port_data_devices[i].adapterExists = false;
      if (i == 0)
      {
        totalAdapters = canusb_getFirstAdapter(can_port_data_devices[i].serialNumber, sizeof(can_port_data_devices[i].serialNumber));
        can_port_data_devices[i].adapterExists = true;
      }
      else
      {
        if (totalAdapters > i)
        {
          canusb_getNextAdapter(can_port_data_devices[i].serialNumber, sizeof(can_port_data_devices[i].serialNumber));
          can_port_data_devices[i].adapterExists = true;
        }

      }
      can_port_data_devices[i].in_use = false;
      can_port_data_devices[i].echo_enabled = false;
      can_port_data_devices[i].can_bus_on = false;

      InitializeCriticalSection(&can_port_data_devices[i].rx_queue_cs);
    }
    libraryIsInitiated = TRUE;
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortOpen(int port, canPortHandle* handle)
{
  canOpenStatus canopen_res = CANOPEN_ERROR_HW_NOT_CONNECTED;
  if (port >= 0 && port < MAX_CAN_DEVICES && can_port_data_devices[port].adapterExists) {
    *handle = port;
    can_port_data_devices[port].in_use = true;
    canopen_res = CANOPEN_OK;
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortClose(canPortHandle handle)
{
  canOpenStatus ret = CANOPEN_ERROR_CAN_LAYER;
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortBitrateSet(canPortHandle handle, int bitrate)
{
  canOpenStatus canopen_res = CANOPEN_ERROR;

  switch (bitrate)
  {
  case 5000:
    sprintf(can_port_data_devices[handle].bitrate, "5");
    canopen_res = CANOPEN_OK;
    break;
  case 10000:
    sprintf(can_port_data_devices[handle].bitrate, "10");
    canopen_res = CANOPEN_OK;
    break;
  case 20000:
    sprintf(can_port_data_devices[handle].bitrate, "20");
    canopen_res = CANOPEN_OK;
    break;
  case 50000:
    sprintf(can_port_data_devices[handle].bitrate, "50");
    canopen_res = CANOPEN_OK;
    break;
  case 100000:
    sprintf(can_port_data_devices[handle].bitrate, "100");
    canopen_res = CANOPEN_OK;
    break;
  case 125000:
    sprintf(can_port_data_devices[handle].bitrate, "125");
    canopen_res = CANOPEN_OK;
    break;
  case 250000:
    sprintf(can_port_data_devices[handle].bitrate, "250");
    canopen_res = CANOPEN_OK;
    break;
  case 500000:
    sprintf(can_port_data_devices[handle].bitrate, "500");
    canopen_res = CANOPEN_OK;
    break;
  case 1000000:
    sprintf(can_port_data_devices[handle].bitrate, "1000");
    canopen_res = CANOPEN_OK;
    break;
  default:
    canopen_res = CANOPEN_ERROR;
    break;
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortEcho(canPortHandle handle, bool enabled)
{
  canOpenStatus canopen_res = CANOPEN_OK;
  can_port_data_devices[handle].echo_enabled = enabled;
  return canopen_res;
}

//------------------------------------------------------------------------
//can_port_data_devices[handle].serialNumber = 0x007a7935 "LWUN2UQV"
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOn(canPortHandle handle)
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  can_port_data_devices[handle].adapterHandle = canusb_Open(can_port_data_devices[handle].serialNumber,
    can_port_data_devices[handle].bitrate,
    CANUSB_ACCEPTANCE_CODE_ALL,
    CANUSB_ACCEPTANCE_MASK_ALL,
    0);
  if (can_port_data_devices[handle].adapterHandle != 0)
  {
    (void)canusb_Flush(can_port_data_devices[handle].adapterHandle, FLUSH_WAIT);
    can_port_data_devices[handle].can_bus_on = true;
    canopen_res = CANOPEN_OK;
  }

  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOff(canPortHandle handle)
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  if (can_port_data_devices[handle].can_bus_on)
  {
    if (canusb_Close(can_port_data_devices[handle].adapterHandle) <= 0)
    {
      can_port_data_devices[handle].can_bus_on = false;
      canopen_res = CANOPEN_OK;
    }
  }

  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortWrite(canPortHandle handle,
  long id,
  void* msg,
  unsigned int dlc,
  unsigned int flags)
{
  canOpenStatus canopen_res = CANOPEN_CAN_LAYER_FAILED;

  CANMsg frame;
  frame.id = id;
  frame.len = dlc;
  frame.flags = 0; // Fix this.
  if (flags & CAN_MSG_EXT)
    frame.flags = CANMSG_EXTENDED;
  if (flags & CAN_MSG_RTR)
    frame.flags |= CANMSG_RTR;

  if (msg != NULL)
    memcpy(frame.data, msg, dlc);

  int res = canusb_Write(can_port_data_devices[handle].adapterHandle, &frame);
  if (res == ERROR_CANUSB_OK) {
    if (can_port_data_devices[handle].echo_enabled) {
      (void)putCanMessageInQueue(&can_port_data_devices[handle], &frame); // Echo.
    }
    canopen_res = CANOPEN_OK;
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortRead(canPortHandle handle,
  long* id,
  void* msg,
  unsigned int* dlc,
  unsigned int* flags)
{
  canOpenStatus canopen_res = CANOPEN_CAN_LAYER_FAILED;
  CANMsg frame;
  int canusb_res;
  *flags = 0;
  // First check if there are any echos avilable.
  canopen_res = getCanMessageFromQueue(&can_port_data_devices[handle], &frame);
  if (canopen_res == CANOPEN_ERROR_NO_MESSAGE) {
    if (can_port_data_devices[handle].can_bus_on)
      canusb_res = canusb_Read(can_port_data_devices[handle].adapterHandle, &frame);
    else
      canusb_res = ERROR_CANUSB_NO_MESSAGE;
    if (canusb_res == ERROR_CANUSB_NO_MESSAGE)
    {
      canopen_res = CANOPEN_ERROR_NO_MESSAGE;
    }
    else if (canusb_res == ERROR_CANUSB_OK)
    {
      canopen_res = CANOPEN_OK;
    }
  }
  if (canopen_res == CANOPEN_OK)
  {
    if (frame.len > 8)
      canopen_res = CANOPEN_ERROR_NO_MESSAGE;
    else
    {
      *id = frame.id;
      *dlc = frame.len;
      if (!(frame.flags & CANMSG_RTR))
        memcpy(msg, frame.data, frame.len);
      else
        memset(msg, 0, frame.len);
      *flags = 0;
      if (frame.flags & CANMSG_EXTENDED)
        *flags = CAN_MSG_EXT;
      if (frame.flags & CANMSG_RTR)
        *flags |= CAN_MSG_RTR;
    }
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus  __stdcall canPortGetSerialNumber(canPortHandle handle, char* buffer, int bufferLen)
{
  strncpy(buffer, can_port_data_devices[handle].serialNumber, sizeof(can_port_data_devices[handle].serialNumber));
  return CANOPEN_ERROR;
}