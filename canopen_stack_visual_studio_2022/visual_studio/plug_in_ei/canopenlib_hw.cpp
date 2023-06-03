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
                                                                                                      

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <conio.h>
#include <shlwapi.h>
#include "canopenlib_hw.h"
#include "eican.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif


#ifdef _MANAGED
#pragma managed(pop)
#endif


#define MAX_EICAN_OBJS   50

// Allocated EIcan objects
static EIcan *cardObj[ MAX_EICAN_OBJS ];
static CRITICAL_SECTION objListCS;


#ifdef ENABLE_LOGGING
#include <stdarg.h>
static FILE *fp;
static void Log( const char *fmt, ... )
{
   if( !fp )
   {
      fp = fopen("eilog.txt", "w" );
      if( !fp ) return;
   }
   va_list ap;
   va_start( ap, fmt );
   vfprintf( fp, fmt, ap );
   va_end(ap);
   fflush(fp);
}
#else
static void Log( const char *fmt, ... ){}
#endif


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
Log( "DllMain %d\n", ul_reason_for_call );
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
   InitializeCriticalSection( &objListCS );
   for( int i=0; i<MAX_EICAN_OBJS; i++ )
      cardObj[i] = 0;
Log( "canPortLibInit\n" );

   return CANOPEN_OK;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortOpen( int port, canPortHandle *handle )
{

Log( "open %d\n", port );
   // Allocate an object to represent the CAN port
   // and try to open it.
   // Note, EI port numbers start from zero
   EIcan *card = new EIcan();
   int err = card->Open( port-1 );
   if( err )
   {
Log( "open failed with err %d\n", err );
      delete card;
      return CANOPEN_ERROR_DRIVER;
   }

   // Find an unused location in our object list
   // and associate it with this card
   EnterCriticalSection( &objListCS );

   for( int i=0; i < MAX_EICAN_OBJS; i++ )
   {
      if( cardObj[i] ) continue;

Log( "open success, handle %d\n", i );
      cardObj[i] = card;
      *handle = i; 
      LeaveCriticalSection( &objListCS );
      return CANOPEN_OK;
   }

   LeaveCriticalSection( &objListCS );
Log( "open no handles\n" );
   return CANOPEN_ERROR_DRIVER;
}

static EIcan *FindCard( int handle )
{
   if( handle < 0 || handle >= MAX_EICAN_OBJS )
      return 0;
   return cardObj[ handle ];
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortClose( canPortHandle handle )
{
   // Find the object associated with this handle
   EnterCriticalSection( &objListCS );
Log( "closing handle %d\n", handle );

   EIcan *card = FindCard( handle );
   if( card )
   {
      // Close the handle and delete the object
      card->Close();
      cardObj[ handle ] = 0;
      delete card;
   }
   LeaveCriticalSection( &objListCS );

   return CANOPEN_OK;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortBitrateSet( canPortHandle handle, int bitrate )
{
   EIcan *card = FindCard( handle );
   if( !card ) return CANOPEN_ERROR_DRIVER;

   int err = card->SetBitRate( bitrate );

Log( "Set bit rate %d, err %d\n", bitrate, err );
   if( err ) return CANOPEN_ERROR_DRIVER;

   return CANOPEN_OK;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortEcho( canPortHandle handle, bool enabled )
{
Log( "Echo\n" );
   // TODO
   return CANOPEN_OK;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOn( canPortHandle handle )
{
   EIcan *card = FindCard( handle );
Log( "Online %d (%p)\n", handle, card );
   if( !card ) return CANOPEN_ERROR_DRIVER;

   int err = card->Online();
Log( "online err: %d\n", err );
   if( err )
      return CANOPEN_ERROR_DRIVER;
   else
      return CANOPEN_OK;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOff( canPortHandle handle )
{
   EIcan *card = FindCard( handle );
Log( "Offline %d (%p)\n", handle, card );
   if( !card ) return CANOPEN_ERROR_DRIVER;

   int err = card->Offline();
Log( "offline err: %d\n", err );
   if( err )
      return CANOPEN_ERROR_DRIVER;
   else
      return CANOPEN_OK;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortWrite(canPortHandle handle,
                                        long id,
                                        void *msg,
                                        unsigned int dlc,
                                        unsigned int flags)
{
   if( dlc > 8 ) 
      return CANOPEN_ERROR_DRIVER;

   CanFrame frame;

   if( flags & CAN_MSG_RTR )
      frame.type = CAN_FRAME_REMOTE;
   else
      frame.type = CAN_FRAME_DATA;

   if( flags & CAN_MSG_EXT )
      frame.id = id | 0x20000000;

   else
      frame.id = id & 0x000007ff;

   frame.length = dlc;
   memcpy( frame.data, msg, dlc );

   EIcan *card = FindCard( handle );
Log( "Write %d (%p)\n", handle, card );
   if( !card ) return CANOPEN_ERROR_DRIVER;

   int err = card->Xmit( &frame, -1 );
Log( "Xmit err %d\n", err );
   if( err )
      return CANOPEN_ERROR_DRIVER;
   else
      return CANOPEN_OK;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortRead(canPortHandle handle,
                                        long *id,
                                        void *msg,
                                        unsigned int *dlc,
                                        unsigned int *flags)
{

   EIcan *card = FindCard( handle );
Log( "Read %d (%p)\n", handle, card );
   if( !card ) return CANOPEN_ERROR_DRIVER;

   CanFrame frame;
   while( true )
   {
      int err = card->Recv( &frame, -1 );
Log( "Recv err %d\n", err );
      if( err ) return CANOPEN_ERROR_DRIVER;

      switch( frame.type )
      {
         case CAN_FRAME_DATA:
            *flags = 0;
            break;

         case CAN_FRAME_REMOTE:
            *flags = CAN_MSG_RTR;
            break;

         // No way to return error info?
         case CAN_FRAME_ERROR:
         default:
            continue;
      }

      if( frame.id & 0x20000000 )
      {
         *id = frame.id & 0x1fffffff;
         *flags |= CAN_MSG_EXT;
      }
      else
         *id = frame.id & 0x7ff;

      *dlc = frame.length;

      memcpy( msg, frame.data, frame.length );
      break;
   }

   return CANOPEN_OK;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus  __stdcall canPortGetSerialNumber(canPortHandle handle, char *buffer, int bufferLen)
{
   Log( "Get Serial\n" );
  return CANOPEN_ERROR;
}
