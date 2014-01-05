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

#include "tiny_can_porting.h"

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

#define MAX_CAN_DEVICES     4

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

static int findFreeCanPortDataSlot( void )
{
  int com_port_data_index = -1; 
  for ( int i = 0; i < MAX_CAN_DEVICES; i++ ) {
    if ( can_port_data_devices[i].in_use == false ) {
      com_port_data_index  = i;
      break;
    }
  }
  return com_port_data_index;
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus putCanMessageInQueue (CanPortDataStruct *p,  CanMessageStruct *can_frame)
{
  canOpenStatus res = CANOPEN_OK;

  if ( p->can_bus_on )
  {
    EnterCriticalSection( &p->rx_queue_cs ); // We need to imlplement a "faked" echo mechanism.

    int put_pos = p->rx_queue_put_pos;
    
    if ( ++put_pos >= RX_QUEUE_SIZE )
      put_pos = 0;

    if (put_pos != p->rx_queue_get_pos) {
      memcpy( &(p->rx_queue[ p->rx_queue_put_pos ]), can_frame, sizeof(CanMessageStruct) );
      p->rx_queue_put_pos = put_pos;
      res = CANOPEN_OK;
    } else {
      res = CANOPEN_ERROR_CAN_LAYER_OVRFLOW;
    }

    LeaveCriticalSection( &p->rx_queue_cs );
  } 
  else
  {
    res = CANOPEN_ERROR_CAN_LAYER;
  }

  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus getCanMessageFromQueue (CanPortDataStruct *p,  
                                          CanMessageStruct *can_frame)
{
  canOpenStatus res;
  int get_pos = p->rx_queue_get_pos;

  if (get_pos != p->rx_queue_put_pos ) {
    memcpy( can_frame, &(p->rx_queue[ p->rx_queue_get_pos ]), sizeof(CanMessageStruct) );
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
  canOpenStatus canopen_res = CANOPEN_ERROR_DRIVER;

  // Make sure that all CAN ports are available.
  for ( int i = 0; i < MAX_CAN_DEVICES; i++ ) {
    can_port_data_devices[i].in_use = false;
    can_port_data_devices[i].echo_enabled = false;
    can_port_data_devices[i].can_bus_on = false;
    InitializeCriticalSection( &can_port_data_devices[i].rx_queue_cs );    
    InitializeCriticalSection( &can_port_data_devices[i].tx_message_queue_cs );
    InitializeCriticalSection( &can_port_data_devices[i].usb_handle_cs );
  }

  int err = LoadDriver( "mhstcan.dll" );
  if (err == 0)
  {
    int32_t res = CanInitDriver(NULL);
    if (res == 0) 
    {
      canopen_res = CANOPEN_OK;
    }
  }
  return canopen_res;

}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortOpen( int port, canPortHandle *handle )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;

  int can_port_index = findFreeCanPortDataSlot();
  if ( can_port_index < 0 ) 
  {
    canopen_res = CANOPEN_OUT_OF_CAN_INTERFACES;
  } 
  else 
  {       
    *handle = (canPortHandle)can_port_index;
    can_port_data_devices[can_port_index].can_port_index = can_port_index;
    can_port_data_devices[can_port_index].device_handle = port; 

    canopen_res = CANOPEN_OK;
  }
  if ( canopen_res == CANOPEN_OK )
  {
    // Perform device open.
    if ( CanDeviceOpen(can_port_data_devices[can_port_index].device_handle, NULL) == 0)
    {
      can_port_data_devices[can_port_index].in_use = true;
      can_port_data_devices[can_port_index].device_handle = port;
    }
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortClose( canPortHandle handle )
{
  TCanMsg can_frame;
  canOpenStatus ret = CANOPEN_ERROR_CAN_LAYER ;

  EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  if ( CanDeviceClose( can_port_data_devices[handle].device_handle ) == 0)
  {
    can_port_data_devices[handle].in_use = false;
    ret = CANOPEN_OK;
  }
  LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );

  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortBitrateSet( canPortHandle handle, int bitrate )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;

  EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  switch (bitrate)
  {
    case 125000:
      if ( CanSetSpeed( can_port_data_devices[handle].device_handle, 125) == 0)
      {
        canopen_res = CANOPEN_OK;
      }
      break;
    case 250000:
      if ( CanSetSpeed( can_port_data_devices[handle].device_handle, 250) == 0)
      {
        canopen_res = CANOPEN_OK;
      }
      break;
    case 500000:
      if ( CanSetSpeed( can_port_data_devices[handle].device_handle, 500) == 0)
      {
        canopen_res = CANOPEN_OK;
      }
      break;
    case 1000000:
      if ( CanSetSpeed( can_port_data_devices[handle].device_handle, 1000) == 0)
      {
        canopen_res = CANOPEN_OK;
      }
      break;
    default:
      canopen_res = CANOPEN_UNSUPPORTED_BITRATE;
      break;
  }
  LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortEcho( canPortHandle handle, bool enabled )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  // Tiny CAN doesn't support echo therefore we
  // need to fake this echo mechanism when sending the
  // frame.
  can_port_data_devices[handle].echo_enabled = enabled;
  canopen_res = CANOPEN_OK;
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOn( canPortHandle handle )
{
  canOpenStatus canopen_res = CANOPEN_ERROR; 
  EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  if (CanSetMode(handle, OP_CAN_RESET, 0) == 0)
  {
    canopen_res = CANOPEN_OK;
  }
  if (canopen_res == CANOPEN_OK)
  {
    if (CanSetMode(can_port_data_devices[handle].device_handle, OP_CAN_START, CAN_CMD_ALL_CLEAR) == 0)
    {
      can_port_data_devices[handle].can_bus_on = true;
      canopen_res = CANOPEN_OK;
    }
  }
  LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOff( canPortHandle handle )
{
  canOpenStatus canopen_res = CANOPEN_ERROR; 
  EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  if ( CanSetMode(can_port_data_devices[handle].device_handle, OP_CAN_STOP, CAN_CMD_NONE) == 0)
  {
    can_port_data_devices[handle].can_bus_on = true;
    canopen_res = CANOPEN_OK;
  }
  LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
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
  struct TCanMsg can_frame;
  canOpenStatus canopen_res = CANOPEN_CAN_LAYER_FAILED;

  EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );

  can_frame.MsgFlags = 0L; 
  can_frame.Id = id;
  can_frame.MsgLen = dlc;

  if (msg != NULL)
  {
    memcpy(can_frame.MsgData, msg, dlc);
  }
  if (flags & CAN_MSG_RTR)
  {
    can_frame.MsgRTR = 1;
  }
  if (flags & CAN_MSG_EXT)
  {
    can_frame.MsgEFF = 1;
  }

  if (CanTransmit(can_port_data_devices[handle].device_handle, &can_frame, 1) == 0)
  {
    canopen_res = CANOPEN_OK;
  } 
  else
  {
    canopen_res = CANOPEN_ERROR;
  }

  if ( canopen_res == CANOPEN_OK )
  {
    if ( can_port_data_devices[ handle ].echo_enabled == true )
    {
      CanMessageStruct can_message;
      can_message.id = id;
      can_message.dlc = dlc;
      can_message.flags = flags;
      if ( (msg != NULL) && ( (flags & CAN_MSG_RTR) == 0 ) ) {
        memcpy( can_message.data, msg, dlc );
      }
      putCanMessageInQueue( &can_port_data_devices[ handle ], &can_message );
    }
  }

  LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );

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
  // TODO: Use the callbacks and remove the polling of messages.

  CanMessageStruct can_message;
  canOpenStatus canopen_res = CANOPEN_ERROR_NO_MESSAGE;  

  EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );

  canopen_res = getCanMessageFromQueue(&can_port_data_devices[handle], &can_message );
  if ( canopen_res == CANOPEN_ERROR_NO_MESSAGE )
  {
    if ( can_port_data_devices[ handle ].in_use ) 
    {
      TCanMsg can_frame;
      int32_t tiny_can_res = -1;

      tiny_can_res = CanReceive(can_port_data_devices[handle].device_handle, &can_frame, 1);

      if ( tiny_can_res  == 0 )
      {
        canopen_res = CANOPEN_ERROR_NO_MESSAGE;
      }
      else if (tiny_can_res  < 0 )
      {
        canopen_res = CANOPEN_ERROR_CAN_LAYER;
      }
      else 
      {
        can_message.id = can_frame.Id;
        can_message.dlc = can_frame.MsgLen;
        memcpy( can_message.data, can_frame.MsgData, can_frame.MsgLen);
        can_message.flags = 0;
        if ( can_frame.MsgRTR == 1)
        {
          can_message.flags |= CAN_MSG_RTR;
        }
        if (can_frame.MsgEFF == 1)
        {
          can_message.flags |= CAN_MSG_EXT;
        }
        canopen_res = CANOPEN_OK;
      }
    }
  } 
  if ( canopen_res == CANOPEN_OK ) {
    *id = can_message.id;
    *dlc = can_message.dlc;
    *flags = can_message.flags;
    memcpy( msg, can_message.data, can_message.dlc );
  }
  LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus  __stdcall canPortGetSerialNumber(canPortHandle handle, char *buffer, int bufferLen)
{
  return CANOPEN_ERROR;
}