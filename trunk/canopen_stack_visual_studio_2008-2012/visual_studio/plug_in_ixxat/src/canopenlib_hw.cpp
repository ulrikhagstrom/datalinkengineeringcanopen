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
    *             Copyright (C) 2009-2010 Ulrik Hagström.             *
    *******************************************************************
    
    CANopen API (C++/C#) is available under a dual license: 
    
    1. GNU General Public License:
    ------------------------------

    Copyright (C) 2009-2010 Ulrik Hagström.

    This program is free software; you can redistribute it and/or modify 
    it under the terms of the GNU General Public License as published by 
    the Free Software Foundation; either version 3 of the License, 
    or (at your option) any later version.

    This program is distributed in the hope that it will be useful, 
    but WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
    See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License 
    along with this program; if not, see <http://www.gnu.org/licenses>.

    Additional permission under GNU GPL version 3 section 7:

    If you modify this Program, or any covered work, by linking or combining 
    it with 'CAN-dispatcher Library' (can_dispatcher.lib) (or a modified 
    version of that library), containing parts covered by the terms of 
    "CANopen API (C++/C#) Commercial License", the licensors 
    of this Program grant you additional permission to convey the resulting 
    work. {Corresponding Source for a non-source form of such a combination 
    shall include the source code for the parts of 'CAN-dispatcher Library' 
    used as well as that of the covered work.}


    2. CANopen API (C++/C#) Commercial License:
    -------------------------------------------

    This license allows for the use of CANopen API (C++/C#) 
    in closed-source projects. This license is available for a very reasonable 
    fee and also includes technical support. For details, please read the 
    CANopen API (C++/C#) Commercial License (see license.txt).
*/


// canlib32.cpp : Defines the entry point for the DLL application.
//
                                                                                                      
#include "windows.h"
#include "stdio.h"
#include "canopenlib_hw.h"

#include <vcinpl.h>

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
#define RX_QUEUE_SIZE       100

// CAN Message
//
typedef struct {
    DWORD ID;        // 11/29 bit identifier
    BYTE  MSGTYPE;   // Bits from MSGTYPE_*
    BYTE  LEN;       // Data Length Code of the Msg (0..8)
    BYTE  DATA[8];   // Data 0 .. 7
} TPCANMsg;

typedef struct 
{
  bool in_use;
  int echo_enabled;
  int bitrate[2];
  HANDLE hDevice;
  LONG   lCtrlNo;       // controller number
  HANDLE hCanCtl;       // controller handle
  HANDLE hCanChn;       // channel handle
  TPCANMsg rx_queue[RX_QUEUE_SIZE];
  int rx_queue_put_pos;
  int rx_queue_get_pos;
  bool can_bus_on;
  CRITICAL_SECTION rx_queue_cs;
  CRITICAL_SECTION tx_message_queue_cs;
  CRITICAL_SECTION usb_handle_cs;
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
  canopen_res = CANOPEN_OK;
  return canopen_res;

}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortOpen( int port, canPortHandle *handle )
{
  EnterCriticalSection( &can_port_data_devices[ port ].usb_handle_cs );
  
  canOpenStatus canopen_res = CANOPEN_ERROR;

  HRESULT hResult; // error code
	HANDLE hDevice;       // device handle
	HANDLE        hEnum;   // enumerator handle
	VCIDEVICEINFO sInfo;   // device info

	//
	// open the device list
	//
	hResult = vciEnumDeviceOpen(&hEnum);

	////
	//// retrieve information about the first
	//// device within the device list
	////
	if (hResult == VCI_OK)
	{
		for(int i=0;i<=port;i++) 
		{
			hResult = vciEnumDeviceNext(hEnum, &sInfo);
			if (VCI_OK != hResult)
			{
				break;
			}
		}
	}

	////
	//// close the device list (no longer needed)
	////
	vciEnumDeviceClose(hEnum);

	//
	// open the device
	//
	if (hResult == VCI_OK)
	{
		canopen_res = CANOPEN_OK;
		hResult = vciDeviceOpen(sInfo.VciObjectId, &hDevice);
		can_port_data_devices[port].hDevice = hDevice;
		// always select controller 0
		can_port_data_devices[port].lCtrlNo = 0;
		*handle = (canPortHandle)port;
	}
	else
	{
		canopen_res = CANOPEN_ERROR_HW_NOT_CONNECTED;
	}

  LeaveCriticalSection( &can_port_data_devices[ port ].usb_handle_cs );
  
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortClose( canPortHandle handle )
{
  canOpenStatus ret = CANOPEN_ERROR_HW_NOT_CONNECTED;
  if (0 < handle)
  {
	  EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
	  //TCanMsg can_frame;
	  ret = CANOPEN_ERROR_CAN_LAYER ;

	  HRESULT hResult; // error code
	  HANDLE hDevice = can_port_data_devices[(int)handle].hDevice;       // device handle

	  hResult = vciDeviceClose(hDevice);

	  if (hResult == VCI_OK)
	  {
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
  int port = (int)handle;
  switch ( bitrate ) 
  {
    case 10000:  // 10k
	  can_port_data_devices[port].bitrate[0] = CAN_BT0_10KB;
	  can_port_data_devices[port].bitrate[1] = CAN_BT1_10KB;
      canopen_res = CANOPEN_OK;
      break;
    case 20000:  // 20k
	  can_port_data_devices[port].bitrate[0] = CAN_BT0_20KB;
	  can_port_data_devices[port].bitrate[1] = CAN_BT1_20KB;
      canopen_res = CANOPEN_OK;
      break;
    case 50000:  // 50k
	  can_port_data_devices[port].bitrate[0] = CAN_BT0_50KB;
	  can_port_data_devices[port].bitrate[1] = CAN_BT1_50KB;
      canopen_res = CANOPEN_OK;
      break;
    case 100000: // 100k
	  can_port_data_devices[port].bitrate[0] = CAN_BT0_100KB;
	  can_port_data_devices[port].bitrate[1] = CAN_BT1_100KB;
      canopen_res = CANOPEN_OK;
      break;
    case 125000: // 125k
	  can_port_data_devices[port].bitrate[0] = CAN_BT0_125KB;
	  can_port_data_devices[port].bitrate[1] = CAN_BT1_125KB;
      canopen_res = CANOPEN_OK;
      break;
    case 250000: // 250k
	  can_port_data_devices[port].bitrate[0] = CAN_BT0_250KB;
	  can_port_data_devices[port].bitrate[1] = CAN_BT1_250KB;
      canopen_res = CANOPEN_OK;
      break;
    case 500000: // 500k
	  can_port_data_devices[port].bitrate[0] = CAN_BT0_500KB;
	  can_port_data_devices[port].bitrate[1] = CAN_BT1_500KB;
      canopen_res = CANOPEN_OK;
      break;
    case 1000000: // 1M
	  can_port_data_devices[port].bitrate[0] = CAN_BT0_1000KB;
	  can_port_data_devices[port].bitrate[1] = CAN_BT1_1000KB;
      canopen_res = CANOPEN_OK;
      break;
    default:
      canopen_res = CANOPEN_UNSUPPORTED_BITRATE;
      break;
  }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortEcho( canPortHandle handle, bool enabled )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  // Ixxat CAN doesn't support echo therefore we
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
  //EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  
  canOpenStatus canopen_res = CANOPEN_ERROR;

  HRESULT hResult;
  HANDLE hCanChn;
  HANDLE hCanCtl;

  int port = (int)handle;
  HANDLE hDevice = can_port_data_devices[port].hDevice;       // device handle
  int lCtrlNo = can_port_data_devices[port].lCtrlNo;

  //
  // create a message channel
  //
  if (hDevice != NULL)
  {
    //
    // create and initialize a message channel
    //
    hResult = canChannelOpen(hDevice, lCtrlNo, FALSE, &hCanChn);

    //
    // initialize the message channel
    //
    if (hResult == VCI_OK)
    {
	  can_port_data_devices[(int)handle].hCanChn = hCanChn;
      UINT16 wRxFifoSize  = 1024;
      UINT16 wRxThreshold = 1;
      UINT16 wTxFifoSize  = 128;
      UINT16 wTxThreshold = 1;

      hResult = canChannelInitialize( hCanChn,
                                      wRxFifoSize, wRxThreshold,
                                      wTxFifoSize, wTxThreshold);
    }

    //
    // activate the CAN channel
    //
    if (hResult == VCI_OK)
    {
      hResult = canChannelActivate(hCanChn, TRUE);
    }

    //
    // open the CAN controller
    //
    if (hResult == VCI_OK)
    {
      hResult = canControlOpen(hDevice, lCtrlNo, &hCanCtl);
      // this function fails if the controller is in use
      // by another application.
    }

    //
    // initialize the CAN controller
    //
    if (hResult == VCI_OK)
    { 
		can_port_data_devices[(int)handle].hCanCtl = hCanCtl;
		hResult = canControlInitialize(hCanCtl, CAN_OPMODE_STANDARD | CAN_OPMODE_ERRFRAME,
                                     can_port_data_devices[port].bitrate[0], can_port_data_devices[port].bitrate[1]);
    }

    //
    // set the acceptance filter
    //
    if (hResult == VCI_OK)
    { 
       hResult = canControlSetAccFilter( hCanCtl, FALSE,
                                         CAN_ACC_CODE_ALL, CAN_ACC_MASK_ALL);
    }

    //
    // start the CAN controller
    //
    if (hResult == VCI_OK)
    {
      hResult = canControlStart(hCanCtl, TRUE);
    }
  }
  else
  {
    hResult = VCI_E_INVHANDLE;
  }

  // TODO 
  //DisplayError(hResult);

  if (hResult == VCI_OK)
    {
      canopen_res = CANOPEN_OK;
    }
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOff( canPortHandle handle )
{
  //EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  
  canOpenStatus canopen_res = CANOPEN_ERROR;

  canControlReset(can_port_data_devices[handle].hCanCtl);
  canChannelClose(can_port_data_devices[handle].hCanChn);
  canControlClose(can_port_data_devices[handle].hCanCtl);
  vciDeviceClose(can_port_data_devices[handle].hDevice);
  canopen_res = CANOPEN_OK;
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
  //EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  canOpenStatus canopen_res = CANOPEN_CAN_LAYER_FAILED;
  HRESULT hResult;
  CANMSG  sCanMsg;

  sCanMsg.dwTime   = 0;
  sCanMsg.dwMsgId  = (UINT32)id;    // CAN message identifier

  sCanMsg.uMsgInfo.Bytes.bType  = CAN_MSGTYPE_DATA;
  sCanMsg.uMsgInfo.Bytes.bFlags = CAN_MAKE_MSGFLAGS(dlc,0,0,flags & CAN_MSG_RTR,flags & CAN_MSG_EXT);
  sCanMsg.uMsgInfo.Bits.srr     = 1;
  sCanMsg.uMsgInfo.Bits.dlc     = (UINT32)dlc;

  if (msg != NULL)
  {
	memcpy( sCanMsg.abData, msg, dlc );
  }

  // write the CAN message into the transmit FIFO
  hResult = canChannelSendMessage(can_port_data_devices[handle].hCanChn, INFINITE, &sCanMsg);
  if (hResult == VCI_OK)
	{
		canopen_res = CANOPEN_OK;
	}
  //LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
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
  //EnterCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  canOpenStatus canopen_res = CANOPEN_ERROR_NO_MESSAGE;  

  HRESULT hResult;
  CANMSG  sCanMsg;
  
  // read a CAN message from the receive FIFO
    hResult = canChannelReadMessage(can_port_data_devices[handle].hCanChn, 100, &sCanMsg);
	if (hResult == VCI_OK)
    {
      
		canopen_res = CANOPEN_OK;

		*id = sCanMsg.dwMsgId;
		*dlc = sCanMsg.uMsgInfo.Bits.dlc;
		unsigned int flagTemp = 0;
		if ( (sCanMsg.uMsgInfo.Bytes.bFlags & CAN_MSGFLAGS_RTR) == CAN_MSGFLAGS_RTR)
		{
			flagTemp |= CAN_MSG_RTR;
		}
		if ( (sCanMsg.uMsgInfo.Bytes.bFlags & CAN_MSGFLAGS_EXT) == CAN_MSGFLAGS_EXT)
		{
			flagTemp |= CAN_MSG_EXT;
		}
		*flags = flagTemp;
		memcpy( msg, sCanMsg.abData, sCanMsg.uMsgInfo.Bits.dlc );
    }
  //LeaveCriticalSection( &can_port_data_devices[ handle ].usb_handle_cs );
  return canopen_res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus  __stdcall canPortGetSerialNumber(canPortHandle handle, char *buffer, int bufferLen)
{
  return CANOPEN_ERROR;
}