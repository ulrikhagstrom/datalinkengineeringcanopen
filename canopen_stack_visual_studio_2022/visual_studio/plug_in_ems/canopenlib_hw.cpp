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
#include "cpc.h"
#include "cpclib.h"
#include "ems_porting.h"

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
DWORD WINAPI canUsbRxThread(void *can_port_data) 
{
  CanPortDataStruct* p = (CanPortDataStruct*)can_port_data;
  while ( p->can_usb_rx_thread_running == true )
  {
    EnterCriticalSection( &p->usb_handle_cs );
    if ( p->in_use ) 
    {
      (void)CPC_Handle( p->device_handle );
    }
    LeaveCriticalSection( &p->usb_handle_cs );
  }
  return 0;
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

void __stdcall messageHandler ( int device_handle, const CPC_MSG_T *ems_msg )
{
  if ( ems_msg != NULL )
  {
    CanMessageStruct can_message;
    if ( ems_msg->type == CPC_MSG_T_CAN || 
         ems_msg->type == CPC_MSG_T_RTR ||
         ems_msg->type == CPC_MSG_T_XCAN ||
         ems_msg->type == CPC_MSG_T_XRTR )
    {
      const CPC_CAN_MSG_T *ems_can_msg = &ems_msg->msg.canmsg;
      can_message.id = ems_can_msg->id;
      can_message.dlc = ems_can_msg->length;
      if ( ems_msg->type == CPC_MSG_T_CAN || 
           ems_msg->type == CPC_MSG_T_XCAN )
      {
        memcpy( &can_message.data, ems_can_msg->msg, ems_can_msg->length);
      }
      switch ( ems_msg->type )
      {
        case CPC_MSG_T_CAN:
          can_message.flags = 0;
          break;
        case CPC_MSG_T_RTR:
          can_message.flags = CAN_MSG_RTR;
          break;
        case CPC_MSG_T_XCAN:
          can_message.flags = CAN_MSG_EXT;
          break;
        case CPC_MSG_T_XRTR:
          can_message.flags = CAN_MSG_RTR || CAN_MSG_EXT;
          break;
        default:
          break;
      }
      // Find index for device handle.
      int index_for_handle = -1; 
      for ( int i = 0; i < MAX_CAN_DEVICES; i++ ) {
        if ( ( can_port_data_devices[i].in_use == true ) &&
             ( can_port_data_devices[i].device_handle == device_handle ) )
        {
          (void)putCanMessageInQueue( &can_port_data_devices[i], &can_message ); 
          SetEvent( can_port_data_devices[i].rx_event );
          break;
        }
      }
    }
  }
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
    InitializeCriticalSection( &can_port_data_devices[i].tx_message_queue_cs );
    InitializeCriticalSection( &can_port_data_devices[i].usb_handle_cs );
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortOpen( int port, canPortHandle *handle )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  char channel_str[7];

  int can_port_index = findFreeCanPortDataSlot();
  if ( can_port_index < 0 ) 
  {
    canopen_res = CANOPEN_OUT_OF_CAN_INTERFACES;
  } 
  else 
  {
    EnterCriticalSection( &can_port_data_devices[ can_port_index ].usb_handle_cs );    
    *handle = (canPortHandle)can_port_index;
    can_port_data_devices[can_port_index].can_port_index = can_port_index;
    canopen_res = CANOPEN_OK;
  }
  if ( canopen_res == CANOPEN_OK )
  {
    sprintf( channel_str, "CHAN%02d", port );
    can_port_data_devices[can_port_index].device_handle = 
      CPC_OpenChannel( channel_str );
    if( can_port_data_devices[can_port_index].device_handle < 0 ) 
    {
      canopen_res = CANOPEN_ERROR_HW_NOT_CONNECTED;
    } 
    else 
    {
      if ( CPC_AddHandler( can_port_data_devices[can_port_index].device_handle, 
        messageHandler ) == 0 ) 
      {
        can_port_data_devices[can_port_index].in_use = true;
        can_port_data_devices[can_port_index].rx_event = CreateEvent(NULL, TRUE, FALSE, NULL);

        // Create thread that is doing the sync calls required.
        can_port_data_devices[can_port_index].rx_thread_handle =
          CreateThread(NULL,                                              // lpThreadAttributes
            0,                                                            // dwStackSize
            canUsbRxThread,                                               // lpStartAddress
            &can_port_data_devices[can_port_index],                       // lpParameter
            CREATE_SUSPENDED,                                             // wCreationFlags
            &can_port_data_devices[can_port_index].can_usb_rx_thread_id); // pThreadId
        
        can_port_data_devices[can_port_index].can_usb_rx_thread_running = true;
        SetThreadPriority(can_port_data_devices[can_port_index].rx_thread_handle, 
          THREAD_PRIORITY_BELOW_NORMAL);
        ResumeThread(can_port_data_devices[can_port_index].rx_thread_handle);

        canopen_res = CANOPEN_OK;
      }
    }
  }
  LeaveCriticalSection( &can_port_data_devices[ can_port_index ].usb_handle_cs );    

  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortClose( canPortHandle handle )
{
  canOpenStatus ret = CANOPEN_ERROR_CAN_LAYER ;

  // Detroy the thread that is constatly sync device with PC.
  can_port_data_devices[handle].can_usb_rx_thread_running = false;
  DWORD dw_res = WaitForSingleObject( 
    can_port_data_devices[handle].rx_thread_handle,  INFINITE );

  EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );

  if ( CPC_RemoveHandler( can_port_data_devices[handle].device_handle, 
    messageHandler ) == 0 ) 
  {
    if ( CPC_CloseChannel( can_port_data_devices[ handle ].device_handle ) == 0 )
    {
      can_port_data_devices[handle].in_use = false;
      ret = CANOPEN_OK;
    }
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
  can_port_data_devices[ handle ].device_params = 
    CPC_GetInitParamsPtr( can_port_data_devices[ handle ].device_handle );

  if ( can_port_data_devices[ handle ].device_params  != NULL ) {
	  // check the controller type
    if( can_port_data_devices[ handle ].device_params->canparams.cc_type == SJA1000 ) {
	    // we change the data rate to 100kbps
      switch (bitrate)
      {
        case 125000:
	        can_port_data_devices[ handle ].device_params->canparams.cc_params.sja1000.btr0 = 0x03;
          can_port_data_devices[ handle ].device_params->canparams.cc_params.sja1000.btr1 = 0x1c;
          canopen_res = CANOPEN_OK;
          break;
        case 250000:
	        can_port_data_devices[ handle ].device_params->canparams.cc_params.sja1000.btr0 = 0x01;
          can_port_data_devices[ handle ].device_params->canparams.cc_params.sja1000.btr1 = 0x1c;
          canopen_res = CANOPEN_OK;
          break;
        case 500000:
	        can_port_data_devices[ handle ].device_params->canparams.cc_params.sja1000.btr0 = 0x00;
          can_port_data_devices[ handle ].device_params->canparams.cc_params.sja1000.btr1 = 0x1c;
          canopen_res = CANOPEN_OK;
          break;
        case 1000000:
	        can_port_data_devices[ handle ].device_params->canparams.cc_params.sja1000.btr0 = 0x00;
          can_port_data_devices[ handle ].device_params->canparams.cc_params.sja1000.btr1 = 0x14;
          canopen_res = CANOPEN_OK;
          break;
        default:
          canopen_res = CANOPEN_UNSUPPORTED_BITRATE;
          break;
      }
      if ( canopen_res == CANOPEN_OK )
      {
        if ( CPC_CANInit( can_port_data_devices[ handle ].device_handle, 0 ) != 0 )
        {
          canopen_res = CANOPEN_CAN_LAYER_FAILED;
        }
      }
    }
  }

  LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
	// now we init the CAN controller
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortEcho( canPortHandle handle, bool enabled )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  // EMS doesn't support echo therefore we
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
  if ( can_port_data_devices[handle].can_bus_on  == false ) {
    if ( can_port_data_devices[handle].device_params != NULL )
    {
      can_port_data_devices[handle].device_params->canparams.cc_params.sja1000.outp_contr = 0xDA;
      if ( CPC_CANInit( can_port_data_devices[ handle ].device_handle, 0 ) == 0 ) {
        can_port_data_devices[handle].can_bus_on = true;
        if ( CPC_Control( can_port_data_devices[ handle ].device_handle, 
          (CONTR_CAN_Message | CONTR_CONT_ON)) == 0 ) 
        {
          canopen_res = CANOPEN_OK; 
        }
      } else {
        canopen_res = CANOPEN_CAN_LAYER_FAILED;
      }
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
  if ( can_port_data_devices[handle].device_params != NULL )
  {
    can_port_data_devices[handle].device_params->canparams.cc_params.sja1000.outp_contr = 0x00; // qqq: Bus-off ?
    if ( CPC_CANInit( can_port_data_devices[ handle ].device_handle, 0 ) == 0 )
    {
      if ( CPC_Control( can_port_data_devices[ handle ].device_handle, 
        (CONTR_CAN_Message | CONTR_CONT_OFF)) == 0 ) 
      {
        canopen_res = CANOPEN_OK; 
      }
    } else {
      canopen_res = CANOPEN_CAN_LAYER_FAILED;
    }
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
  canOpenStatus canopen_res = CANOPEN_CAN_LAYER_FAILED;
  CanMessageStruct can_message;
  CPC_CAN_MSG_T ems_msg;
  ems_msg.id = id;
  ems_msg.length = dlc;
  if ( msg != NULL ) {
    memcpy ( ems_msg.msg, msg, dlc);
  }

  EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  if ( ((flags & CAN_MSG_RTR) != 0) && ((flags & CAN_MSG_EXT) != 0) ) 
  {
    if ( CPC_SendXRTR( can_port_data_devices[handle].device_handle, 0, &ems_msg ) == 0 )
    {
      canopen_res = CANOPEN_OK;
    }
  } 
  else  if ( flags & CAN_MSG_EXT ) 
  {
    if ( CPC_SendXMsg( can_port_data_devices[handle].device_handle, 0, &ems_msg ) == 0 )
    {
      canopen_res = CANOPEN_OK;
    }
  }
  else  if ( flags & CAN_MSG_RTR ) 
  {
    if ( CPC_SendRTR( can_port_data_devices[handle].device_handle, 0, &ems_msg ) == 0 )
    {
      canopen_res = CANOPEN_OK;
    }
  }
  else 
  {
    if ( CPC_SendMsg( can_port_data_devices[handle].device_handle, 0, &ems_msg ) == 0 )
    {
      canopen_res = CANOPEN_OK;
    }
  }
  LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  if ( canopen_res == CANOPEN_OK )
  {
    if ( can_port_data_devices[ handle ].echo_enabled == true )
    {
      can_message.id = id;
      can_message.dlc = dlc;
      can_message.flags = flags;
      if ( (msg != NULL) && ( (flags & CAN_MSG_RTR) == 0 ) ) {
        memcpy( can_message.data, msg, dlc );
      }
      putCanMessageInQueue( &can_port_data_devices[ handle ], &can_message );
    }
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
  CanMessageStruct can_message;
  canOpenStatus canopen_res = CANOPEN_ERROR_NO_MESSAGE;  

  canopen_res = getCanMessageFromQueue(&can_port_data_devices[handle], &can_message );
  if ( canopen_res == CANOPEN_ERROR_NO_MESSAGE )
  {
    ResetEvent( can_port_data_devices[ handle ].rx_event );
    EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
    if ( can_port_data_devices[ handle ].in_use ) {
      (void)CPC_Handle( can_port_data_devices[handle].device_handle );
    }
    LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
    DWORD res = WaitForSingleObject( can_port_data_devices[ handle ].rx_event, 100 );
    if ( res == WAIT_OBJECT_0 )
    {
      canopen_res = getCanMessageFromQueue(&can_port_data_devices[handle], &can_message );
    }
  } 
  if ( canopen_res == CANOPEN_OK ) {
    *id = can_message.id;
    *dlc = can_message.dlc;
    *flags = can_message.flags;
    memcpy( msg, can_message.data, can_message.dlc );
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