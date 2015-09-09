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
#include "PCANBasic.h"
//#include "Pcan_2usb.h"
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
  int bitrate;
  CRITICAL_SECTION rx_queue_cs;
  TPCANMsg rx_queue[RX_QUEUE_SIZE];
  int rx_queue_put_pos;
  int rx_queue_get_pos;
  bool can_bus_on;
  int port;
} CanPortDataStruct;

static CanPortDataStruct can_port_data_devices[ MAX_CAN_DEVICES ];





BOOL APIENTRY DllMain( HMODULE hModule,
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

canOpenStatus putCanMessageInQueue (CanPortDataStruct *p,  TPCANMsg *can_frame)
{
  canOpenStatus res = CANOPEN_OK;

  EnterCriticalSection( &p->rx_queue_cs ); // We need to imlplement a "faked" echo mechanism.

  int put_pos = p->rx_queue_put_pos;
  
  if ( ++put_pos >= RX_QUEUE_SIZE )
    put_pos = 0;

  if (put_pos != p->rx_queue_get_pos) {
    memcpy( &(p->rx_queue[ p->rx_queue_put_pos ]), can_frame, sizeof(TPCANMsg) );
    p->rx_queue_put_pos = put_pos;
    res = CANOPEN_OK;
  } else {
    res = CANOPEN_ERROR_CAN_LAYER_OVRFLOW;
  }

  LeaveCriticalSection( &p->rx_queue_cs );

  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus getCanMessageFromQueue (CanPortDataStruct *p,  
                                      TPCANMsg *can_frame)
{
  canOpenStatus res;
  int get_pos = p->rx_queue_get_pos;

  if (get_pos != p->rx_queue_put_pos ) {
    memcpy( can_frame, &(p->rx_queue[ p->rx_queue_get_pos ]), sizeof(TPCANMsg) );
    if ( ++get_pos >= RX_QUEUE_SIZE )
      get_pos = 0;
    p->rx_queue_get_pos = get_pos;
    res = CANOPEN_OK;
  } else {
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

	int handle = -1; // the handle obtained for the opened channel

  // Make sure that all CAN ports are available.
  for ( int i = 0; i < MAX_CAN_DEVICES; i++ ) {
    can_port_data_devices[i].in_use = false;
    can_port_data_devices[i].echo_enabled = false;
    can_port_data_devices[i].can_bus_on = false;
    InitializeCriticalSection( &can_port_data_devices[i].rx_queue_cs );    
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortOpen( int port, canPortHandle *handle )
{
  canOpenStatus canopen_res = CANOPEN_ERROR_HW_NOT_CONNECTED;
  if ( port >= 0 && port < MAX_CAN_DEVICES ) {
    *handle = port;
	can_port_data_devices[port].port = port + 0x51U;
    canopen_res = CANOPEN_OK;
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortClose( canPortHandle handle )
{
  canOpenStatus ret = CANOPEN_ERROR_CAN_LAYER ;
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortBitrateSet( canPortHandle handle, int bitrate )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  DWORD res;
  switch ( bitrate ) 
  {
    case 5000:
      can_port_data_devices[handle].bitrate = PCAN_BAUD_5K;
      canopen_res = CANOPEN_OK;
      break;
    case 10000:
      can_port_data_devices[handle].bitrate = PCAN_BAUD_10K;
      canopen_res = CANOPEN_OK;
      break;
    case 20000:
      can_port_data_devices[handle].bitrate = PCAN_BAUD_20K;
      canopen_res = CANOPEN_OK;
      break;
    case 50000:
      can_port_data_devices[handle].bitrate = PCAN_BAUD_50K;
      canopen_res = CANOPEN_OK;
      break;
    case 100000:
      can_port_data_devices[handle].bitrate = PCAN_BAUD_100K;
      canopen_res = CANOPEN_OK;
      break;
    case 125000:
      can_port_data_devices[handle].bitrate = PCAN_BAUD_125K;
      canopen_res = CANOPEN_OK;
      break;
    case 250000:
      can_port_data_devices[handle].bitrate = PCAN_BAUD_250K;
      canopen_res = CANOPEN_OK;
      break;
    case 500000:
      can_port_data_devices[handle].bitrate = PCAN_BAUD_500K;
      canopen_res = CANOPEN_OK;
      break;
    case 1000000:
      can_port_data_devices[handle].bitrate = PCAN_BAUD_1M;
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

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortEcho( canPortHandle handle, bool enabled )
{
  canOpenStatus canopen_res = CANOPEN_OK;
  can_port_data_devices[handle].echo_enabled = enabled;
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOn( canPortHandle handle )
{
	canOpenStatus canopen_res = CANOPEN_ERROR; 
	TPCANStatus res = CAN_Initialize(
		(TPCANHandle)can_port_data_devices[ handle ].port,
		(TPCANBaudrate)can_port_data_devices[ handle ].bitrate,
		(TPCANType)PCAN_USB,
		0,
		0);

	if ( res == PCAN_ERROR_OK )
	{
		can_port_data_devices[ handle ].can_bus_on = true;
		canopen_res = CANOPEN_OK;
	}
	return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOff( canPortHandle handle )
{
  canOpenStatus canopen_res = CANOPEN_ERROR; 
  DWORD res = CAN_Uninitialize((TPCANHandle)can_port_data_devices[ handle ].port);
  if ( res == PCAN_ERROR_OK ) {
    can_port_data_devices[ handle ].can_bus_on = false;
    canopen_res = CANOPEN_OK;
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortWrite(canPortHandle handle,
                                        long id,
                                        void *msg,
                                        unsigned int dlc,
                                        unsigned int flags)
{
  canOpenStatus canopen_res = CANOPEN_CAN_LAYER_FAILED;
  TPCANMsg frame;
  frame.ID = id;
  frame.LEN = dlc;
  frame.MSGTYPE = 0; // Clear all flags.
  if ( flags & CAN_MSG_EXT ) 
    frame.MSGTYPE = PCAN_MESSAGE_EXTENDED;
  if ( flags & CAN_MSG_RTR ) 
    frame.MSGTYPE = PCAN_MESSAGE_RTR;
  else 
    memcpy( frame.DATA, msg, dlc);

  if ( can_port_data_devices[ handle ].echo_enabled ) {
    (void)putCanMessageInQueue( &can_port_data_devices[ handle ], &frame);
  }
  DWORD res = CAN_Write((TPCANHandle)can_port_data_devices[ handle ].port, &frame );
  if ( res == PCAN_ERROR_OK ) {
    canopen_res = CANOPEN_OK;
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortRead(canPortHandle handle,
                                        long *id,
                                        void *msg,
                                        unsigned int *dlc,
                                        unsigned int *flags)
{
  canOpenStatus canopen_res = CANOPEN_CAN_LAYER_FAILED;
  TPCANMsg frame;
  DWORD res;
  TPCANTimestamp dummy;
  *flags = 0;

  // First check if there are any echos avilable.
  canopen_res = getCanMessageFromQueue( &can_port_data_devices[ handle ], &frame );  
  if ( canopen_res != CANOPEN_OK ) {
    TPCANMsg tmp_frame;
    res = CAN_Read((TPCANHandle)can_port_data_devices[ handle ].port, &tmp_frame, &dummy);
    if ( res == PCAN_ERROR_OK && !( tmp_frame.MSGTYPE & PCAN_MESSAGE_STATUS ) ) {
      (void)putCanMessageInQueue( &can_port_data_devices[ handle ], &tmp_frame);
    } else {
      res = CANOPEN_ERROR_NO_MESSAGE;
    }
  }  else {
    res = CAN_Read(1, &frame, &dummy );
    if ( res == PCAN_ERROR_OK ) {
      if  ( frame.MSGTYPE & PCAN_MESSAGE_STATUS  ) {
        res = CANOPEN_ERROR_NO_MESSAGE;
      } else {
        canopen_res = CANOPEN_OK;
      }
    }
  }
  if ( canopen_res == CANOPEN_OK  ) {
    *id = (long)frame.ID;
    *dlc = frame.LEN;
    if ( frame.MSGTYPE & PCAN_MESSAGE_RTR ) 
      *flags = CAN_MSG_RTR;
    if ( frame.MSGTYPE & PCAN_MESSAGE_EXTENDED ) 
      *flags |= CAN_MSG_EXT;
    memcpy( msg, frame.DATA, *dlc);
    canopen_res = CANOPEN_OK;
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus  __stdcall canPortGetSerialNumber(canPortHandle handle, char *buffer, int bufferLen)
{
  return CANOPEN_ERROR;
}