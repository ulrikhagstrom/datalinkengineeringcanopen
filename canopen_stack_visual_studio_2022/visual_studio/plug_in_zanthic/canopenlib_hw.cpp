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
#include "C4UFX_LowLevFunc.h"

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

#define RX_QUEUE_SIZE     100
#define INVALID_HANDLE    -1

typedef struct {
  long id;
  unsigned char data[8];
  unsigned int dlc;
  unsigned int flags;
} CanMessageStruct;

typedef struct 
{
  bool in_use;
  int can_port_index;
  int echo_enabled;
  int bitrate;
  CRITICAL_SECTION rx_queue_cs;
  CanMessageStruct rx_queue[RX_QUEUE_SIZE];
  int rx_queue_put_pos;
  int rx_queue_get_pos;
  bool can_usb_rx_thread_running;
  bool can_bus_on;
  HANDLE rx_thread_handle;
  DWORD can_usb_rx_thread_id;
  CRITICAL_SECTION tx_message_queue_cs;
  CRITICAL_SECTION usb_handle_cs;
  //HANDLE rx_event;
  int device_handle;
} CanPortDataStruct;



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

void __stdcall messageHandler ( int device_handle, const unsigned char *zanthic_msg )
{

// Byte 0=number of bytes in this record including this byte (min=7, max=16+4(timestamping))
// Byte 1=Control 1 (upper 4 bits is MO, lower four bits are data length)
// Byte 2=Control 2 (status bits IDE,RTR,Reserved,SRR,RXRTR,Filt2,Filt1,Filt0)
// Byte 3=ID1
// Byte 4=ID2
// Byte 5=ID3
// Byte 6=ID4
// Byte 7...=data as needed (0-8 bytes)


  CanMessageStruct can_message;

  can_message.dlc = zanthic_msg[0] - 7;
  can_message.dlc = zanthic_msg[1] & 0x0f;

  can_message.flags = 0;

  if ( zanthic_msg[2] & 0x80 ) 
    can_message.flags |= CAN_MSG_EXT;
  if ( zanthic_msg[2] & 0x40 ) 
    can_message.flags |= CAN_MSG_RTR;

  can_message.id = (zanthic_msg[3] << 3*8);
  can_message.id |= (zanthic_msg[4] << 2*8);
  can_message.id |= (zanthic_msg[5] << 1*8);
  can_message.id |= zanthic_msg[6];
  memcpy( can_message.data, &zanthic_msg[7], can_message.dlc);

  int index_for_handle = -1; 
  for ( int i = 0; i < MAX_CAN_DEVICES; i++ ) {
    if ( ( can_port_data_devices[i].in_use == true ) &&
         ( can_port_data_devices[i].device_handle == device_handle ) )
    {
      (void)putCanMessageInQueue( &can_port_data_devices[i], &can_message ); 
      //SetEvent( can_port_data_devices[i].rx_event );
      break;
    }
  }
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
DWORD WINAPI canUsbRxThread(void *can_port_data) 
{
  unsigned char can_message_buffer[20];
  CanPortDataStruct* p = (CanPortDataStruct*)can_port_data;
  while ( p->can_usb_rx_thread_running == true )
  {
    EnterCriticalSection( &p->usb_handle_cs );
    if ( p->in_use && p->can_bus_on ) 
    {
      short int res = GetCANMess( p->device_handle, 0, can_message_buffer);
      if (res == ACK)
      {
        messageHandler( p->device_handle, can_message_buffer);
      }
    }
    LeaveCriticalSection( &p->usb_handle_cs );
  }
  return 0;
}




//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortLibraryInit(void)
{
 canOpenStatus canopen_res = CANOPEN_OK;
	//int handle = -1; // the handle obtained for the opened channel

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
  unsigned char version[3];
  unsigned char manufacturer[20];
  unsigned char can_controllers[10];
  unsigned char feature;

  short int ret = 0;

  *handle = INVALID_HANDLE;

  int can_port_index = findFreeCanPortDataSlot();
  if ( can_port_index < 0 ) 
  {
    canopen_res = CANOPEN_OUT_OF_CAN_INTERFACES;
  } 
  else 
  {

    (void)ResetInterface( port, 0 );

    ret = GetInfo( port, 0, version, &feature, manufacturer, can_controllers );
    if (ret == ACK) 
    {
      canopen_res = CANOPEN_OK;
    }
    else
    {
      canopen_res = CANOPEN_ERROR_HW_NOT_CONNECTED;
    }
    if (canopen_res == CANOPEN_OK) 
    {
      EnterCriticalSection( &can_port_data_devices[ can_port_index ].usb_handle_cs );    
      *handle = (canPortHandle)can_port_index;
      can_port_data_devices[can_port_index].can_port_index = can_port_index;
      can_port_data_devices[can_port_index].device_handle = port;
      can_port_data_devices[can_port_index].in_use = true;
      //can_port_data_devices[can_port_index].rx_event = CreateEvent(NULL, TRUE, FALSE, NULL);


      // Start RX-thread
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
      LeaveCriticalSection( &can_port_data_devices[ can_port_index ].usb_handle_cs ); 
    }
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortClose( canPortHandle handle )
{
  canOpenStatus ret = CANOPEN_ERROR_CAN_LAYER ;

  if (handle != INVALID_HANDLE)
  {
    // Detroy the thread that is constatly sync device with PC.
    can_port_data_devices[handle].can_usb_rx_thread_running = false;
    DWORD dw_res = WaitForSingleObject( 
      can_port_data_devices[handle].rx_thread_handle,  INFINITE );

    EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );

    if ( ResetInterface( can_port_data_devices[handle].device_handle, 0 ) )
    {
      can_port_data_devices[handle].in_use = false;
      ret = CANOPEN_OK;
    }
    LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  }
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
      can_port_data_devices[handle].bitrate = 125;
      canopen_res = CANOPEN_OK;
      break;
    case 250000:
      can_port_data_devices[handle].bitrate = 250;
      canopen_res = CANOPEN_OK;
      break;
    case 500000:
      can_port_data_devices[handle].bitrate = 500;
      canopen_res = CANOPEN_OK;
      break;
    case 1000000:
      can_port_data_devices[handle].bitrate = 1000;
      canopen_res = CANOPEN_OK;
      break;
    default:
      canopen_res = CANOPEN_UNSUPPORTED_BITRATE;
      break;
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

  short int res = SetCANBaud(can_port_data_devices[handle].device_handle, 
    0, 
    can_port_data_devices[handle].bitrate);
  if (res == ACK) 
  {
    can_port_data_devices[ handle ].can_bus_on = true;
    canopen_res = CANOPEN_OK;
  }
  else
  {
    canopen_res = CANOPEN_UNSUPPORTED_BITRATE;
  }

  LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
	// now we init the CAN controller
  return canopen_res;}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOff( canPortHandle handle )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  short int res = 0;
  EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  if ( can_port_data_devices[handle].can_bus_on )
  {
    res = ResetInterface(can_port_data_devices[handle].device_handle, 0);
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

  if ( can_port_data_devices[ handle ].can_bus_on )
  {
    EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
    long zanthic_id = id;
    if ( flags & CAN_MSG_RTR ) 
    {
      zanthic_id |= 0x40000000; // Bit 30.
    }
    if ( flags & CAN_MSG_EXT ) 
    {
      zanthic_id |= 0x80000000; // Bit 31.
    }
    unsigned char zanthic_CB1 = 0;
    unsigned char zanthic_CB2 = dlc & 0x0f;
    unsigned char zanthic_CAN_data[8];
    if ((flags & CAN_MSG_RTR) == 0)
    {
      memcpy( zanthic_CAN_data, msg, dlc); // Workaround.
    }

    short res = SendCANMess( can_port_data_devices[handle].device_handle,
      0, zanthic_id, zanthic_CB1, zanthic_CB2, zanthic_CAN_data);
    if ( res = ACK )
    {
      canopen_res = CANOPEN_OK;
    }

    if ( canopen_res == CANOPEN_OK )
    {
      CanMessageStruct can_message;
      if ( can_port_data_devices[ handle ].echo_enabled )
      {
        can_message.id = id;
        can_message.dlc = dlc;
        can_message.flags = flags;
        if ( (msg != NULL) && ( (flags & CAN_MSG_RTR) == 0 ) ) 
        {
          memcpy( can_message.data, msg, dlc );
        }
        putCanMessageInQueue( &can_port_data_devices[ handle ], &can_message );
      }
    }

    LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
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
    //ResetEvent( can_port_data_devices[ handle ].rx_event );
    EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
    if ( can_port_data_devices[ handle ].in_use ) 
    {
      unsigned char can_message_buffer[20];
      short int res = GetCANMess( can_port_data_devices[ handle ].device_handle, 
        0, can_message_buffer);
      if (res == ACK)
      {
        messageHandler( can_port_data_devices[ handle ].device_handle, can_message_buffer);
        canopen_res = getCanMessageFromQueue(&can_port_data_devices[handle], &can_message );
      }
    }
    LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
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