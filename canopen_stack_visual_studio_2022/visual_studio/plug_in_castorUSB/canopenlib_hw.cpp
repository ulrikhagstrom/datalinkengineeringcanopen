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
#include "castorDriver.h"


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


CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortLibraryInit(void)
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  castorStatus castor_res = canInitializeLib();  
  if ( castor_res == castorOK)
  {
    int channel_count;
    castor_res = canGetNumberOfChannels( &channel_count );
    if ( castor_res == castorOK )
    {
      canopen_res = CANOPEN_OK;
    }
  }
  else
  {
    canopen_res = CANOPEN_ERROR_DRIVER;
  }
  return canopen_res;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortOpen( int port, canPortHandle *handle )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  castorStatus castor_res = canOpenChannel( port, (castorUSBHandle*)handle ); // qqq... 10 ?? 
  if ( castor_res == castorOK || castor_res == castorERR_ALREADY_OPEN )
  {
    canopen_res = CANOPEN_OK;
  }
  else
  {
    canopen_res = CANOPEN_ERROR_DRIVER;
  }
  return canopen_res;

}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortClose( canPortHandle handle )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  castorStatus castor_res = canClose( (castorUSBHandle) handle ); // castorUSBHandle is also int.
  if ( castor_res == castorOK)
  {
    canopen_res = CANOPEN_OK;
  }
  else
  {
    canopen_res = CANOPEN_ERROR_DRIVER;
  }
  return canopen_res;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortBitrateSet( canPortHandle handle, int bitrate )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  int castor_predef_bitrate_code = 0;

  switch ( bitrate )
  {
    case 125000:
      castor_predef_bitrate_code = canBUSSPEED125K;
      break;
    case 250000:
      castor_predef_bitrate_code = canBUSSPEED250K;
      break;
    case 500000:
      castor_predef_bitrate_code = canBUSSPEED500K;
      break;
    case 1000000:
      castor_predef_bitrate_code = canBUSSPEED1000K;
      break;
    default:
      break;
  }
  if ( castor_predef_bitrate_code )
  {
    castorStatus castor_res = canSetBitRate( (castorUSBHandle) handle, 
      castor_predef_bitrate_code );
    if ( castor_res == castorOK )
    {
      canopen_res = CANOPEN_OK;
    }
    else
    {
      canopen_res = CANOPEN_ERROR_DRIVER;
    }
  }
  return canopen_res;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortEcho( canPortHandle handle, bool enabled )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;
  int flag = enabled ? 1 : 0;
  
  castorStatus castor_res = canIoCtl( (castorUSBHandle) handle, 
    canIOCTL_SET_TXACK,
    &flag,
    sizeof(flag));

  if ( castor_res == castorOK )
  {
    canopen_res = CANOPEN_OK;
  }
  else
  {
    canopen_res = CANOPEN_ERROR_DRIVER;
  }
  return canopen_res;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOn( canPortHandle handle )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;  
  castorStatus castor_res = canBusOn ( (castorUSBHandle) handle );
  if ( castor_res == castorOK )
  {
    canopen_res = CANOPEN_OK;
  }
  else
  {
    canopen_res = CANOPEN_ERROR_DRIVER;
  }
  return canopen_res;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOff( canPortHandle handle )
{
  canOpenStatus canopen_res = CANOPEN_ERROR;  
  castorStatus castor_res = canBusOff ( (castorUSBHandle) handle );
  if ( castor_res == castorOK )
  {
    canopen_res = CANOPEN_OK;
  }
  else
  {
    canopen_res = CANOPEN_ERROR_DRIVER;
  }
  return canopen_res;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortWrite(canPortHandle handle,
                                        long id,
                                        void *msg,
                                        unsigned int dlc,
                                        unsigned int flags)
{
  canOpenStatus canopen_res = CANOPEN_ERROR;  
  int castor_flags = 0;
  void *p_init = msg;
  unsigned char temp[8];

  if ( flags & CAN_MSG_RTR )
  {
    castor_flags |= canMSG_RTR_FLG;
  }
  if ( flags & CAN_MSG_EXT )
  {
    castor_flags |= canMSG_EXTENDED_IDENTIFIER_FLG;
  }
  else
  {
    castor_flags |= canMSG_STANDARD_IDENTIFIER_FLG;
  }

  if (msg == NULL) // qqq. driver crashes otherwise.
  {
    msg = temp;
  }
  castorStatus castor_res = canWrite ( 
    (castorUSBHandle)handle,
    id,
    (char*)msg,
    dlc,
    castor_flags);

  if (p_init)
  {
    msg = NULL;
  }
  if ( castor_res == castorOK )
  {
    canopen_res = CANOPEN_OK;
  }
  else
  {
    canopen_res = CANOPEN_ERROR_DRIVER;
  }
  return canopen_res;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortRead(canPortHandle handle,
                                        long *id,
                                        void *msg,
                                        unsigned int *dlc,
                                        unsigned int *flags)
{
  canOpenStatus canopen_res = CANOPEN_ERROR;  
  int canopen_flags = 0;
  unsigned int castor_flags = 0;
  unsigned long dummy_time = 0;

  castorStatus castor_res = canRead ( 
    (castorUSBHandle)handle,
    id,
    (char*)msg,
    dlc,
    &castor_flags,
    &dummy_time);

  if ( castor_res == castorOK )
  {
    if ( castor_flags & canMSG_RTR_FLG )
    {
      canopen_flags |= CAN_MSG_RTR;
    }
    if ( castor_flags & canMSG_EXTENDED_IDENTIFIER_FLG )
    {
      canopen_flags |= CAN_MSG_EXT;
    }
    *flags = canopen_flags;
    canopen_res = CANOPEN_OK;
  }
  else
  {
    canopen_res = CANOPEN_ERROR_DRIVER;
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