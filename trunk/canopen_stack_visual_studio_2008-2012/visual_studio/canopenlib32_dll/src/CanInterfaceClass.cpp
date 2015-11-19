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

#include <stdio.h>
#include <conio.h>
#include "CanInterfaceClass.h"
#include "TimerClass.h"
#include "canopenlibdefines.h"
#include "CanOpenDispatcher.h"
#include "CanopenLibHelper.h"

//
// CAN informaiton flags.
//
#define CAN_MSG_RTR              0x0001      // Message is a remote request
#define CAN_MSG_EXT              0x0002      // Message has a standard ID

typedef canOpenStatus (__stdcall *canPortLibraryInitFP)(void);
typedef canOpenStatus (__stdcall *canPortOpenFP)(int, canPortHandle *);
typedef canOpenStatus (__stdcall *canPortCloseFP)(canPortHandle handle);
typedef canOpenStatus (__stdcall *canPortBitrateSetFP)(canPortHandle, int );
typedef canOpenStatus (__stdcall *canPortEchoFP)(canPortHandle, bool );
typedef canOpenStatus (__stdcall *canPortGoBusOnFP)( canPortHandle );
typedef canOpenStatus (__stdcall *canPortGoBusOffFP)(canPortHandle );
typedef canOpenStatus (__stdcall *canPortWriteFP)(canPortHandle,
                                            long,
                                            void *,
                                            unsigned int,
                                            unsigned int);

typedef canOpenStatus (__stdcall *canPortReadFP)(canPortHandle,
                                          long *,
                                          void *,
                                          unsigned int *,
                                          unsigned int *);

typedef canOpenStatus (__stdcall *canPortGetSerialNumberFP)(canPortHandle handle, char *buffer, int bufferLen);


canPortLibraryInitFP canPortLibraryInit = NULL;
canPortOpenFP canPortOpen = NULL;
canPortCloseFP canPortClose = NULL;
canPortBitrateSetFP canPortBitrateSet = NULL;
canPortEchoFP canPortEcho = NULL;
canPortGoBusOnFP canPortGoBusOn = NULL;
canPortGoBusOffFP canPortGoBusOff = NULL;
canPortWriteFP canPortWrite = NULL;
canPortReadFP canPortRead = NULL;
canPortGetSerialNumberFP canPortGetSerialNumber = NULL;

// Init of the parameters.
CanInterface* CanInterface :: canInterfaceSingleton[MAX_CAN_INTERFACES] = 
  { NULL, NULL, NULL, NULL };

bool CanInterface :: driver_licensed = false;

// Constructor that is cleaning up the callbacks pointers that are being
// used to send objects the received frames.

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
CanInterface :: CanInterface ( int port_index )
{
  canOpenStatus ret = canLibraryInit();  // Also verifies that drivers are installed.
  if ( ret == CANOPEN_OK)
  {
    this->drivers_initialized = true;
    this->is_can_dispatcher_thread_running = false;
    this->can_port_opened = false;
    this->can_port_bus_on = false;
    this->can_frame_dispatcher_thread_handle = NULL;
    this->can_message_dispatcher_mutex = CreateMutex( NULL, FALSE, NULL);
    this->port_mutex = CreateMutex( NULL, FALSE, NULL);
	this->can_frame_dispatcher_thread_mutex = CreateMutex( NULL, FALSE, NULL);

    for (int i=0; i < MAX_PROCESS_MSG_CALLBACKS; i++)
    {
      this->dispatcherConfiguration[i].can_consumer_callback            = NULL;
      this->dispatcherConfiguration[i].protocol_state_machine_callback  = NULL;
      this->dispatcherConfiguration[i].context                          = NULL;
    }
    this->canHandle = -1;
    this->port_index = port_index; // To be able to cleanup later.
    this->port_users = 0; // Number of users of this port set to zero.
  }
  else
  {
    this->drivers_initialized = false;
  }
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
CanInterface :: ~CanInterface()
{
  this->stopDispatcherThread();   // qqq change so it is one thread per port.;
  WaitForSingleObject(this->can_message_dispatcher_mutex, INFINITE);
  if (hCanLib != NULL)
  {
      FreeLibrary(hCanLib);
      hCanLib = NULL;
  }
  CloseHandle(this->can_message_dispatcher_mutex);
  CloseHandle(this->can_frame_dispatcher_thread_mutex);
  CloseHandle(this->port_mutex);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: unlockCanopenLibrary(char *path_and_license_file, 
                                                   char *unlock_code )
{
  canOpenStatus res = CANOPEN_OK; //::unlockCanopenLibrary( path_and_license_file, 
    //unlock_code, strlen(unlock_code));
  if ( res == CANOPEN_OK )
    driver_licensed = true;
  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: registerCanMessageHandler(
  void *context, 
  DispatcherCanFuncPtr can_consumer_callback, 
  ProtocolImplementationStateMachineFuncPtr protocol_state_machine_callback,
  int *can_message_handler_index)
{
  canOpenStatus ret = CANOPEN_ERROR;

  WaitForSingleObject( this->can_message_dispatcher_mutex, INFINITE);
  for (int i=0; i < MAX_PROCESS_MSG_CALLBACKS; i++)
  {
    if (this->dispatcherConfiguration[i].can_consumer_callback == NULL)
    {
      this->dispatcherConfiguration[i].protocol_state_machine_callback = 
        protocol_state_machine_callback;
      this->dispatcherConfiguration[i].context = context;
      this->dispatcherConfiguration[i].can_consumer_callback = 
        can_consumer_callback;
      *can_message_handler_index = i;
      ret = CANOPEN_OK;
      break;
    }
  }
  ReleaseMutex( this->can_message_dispatcher_mutex );
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: unregisterCanMessageHandler( int can_message_handler_index )
{
  canOpenStatus ret = CANOPEN_ERROR;
  WaitForSingleObject( this->can_message_dispatcher_mutex, INFINITE);
  if ( can_message_handler_index >= 0 && 
       can_message_handler_index < MAX_PROCESS_MSG_CALLBACKS)
  {
      this->dispatcherConfiguration[can_message_handler_index].protocol_state_machine_callback = NULL;
      this->dispatcherConfiguration[can_message_handler_index].context = NULL;
      this->dispatcherConfiguration[can_message_handler_index].can_consumer_callback = NULL;
      ret = CANOPEN_OK;
  }
  ReleaseMutex( this->can_message_dispatcher_mutex );
  return ret;
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
DWORD WINAPI  CanInterface :: canFrameDispatcherThread(PVOID p)
{
  return canopenDispatcher(p);
}

// Pass the can-message as a call to everybody that is looking for this "event".
//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  CanInterface :: canFrameDispatcher(unsigned long id, 
                                                  unsigned char *data, 
                                                  unsigned int dlc, 
                                                  unsigned int flags)
{
  canOpenStatus ret = CANOPEN_ERROR;

  WaitForSingleObject( this->can_message_dispatcher_mutex , INFINITE);
  for (int i=0; i < MAX_PROCESS_MSG_CALLBACKS; i++)
  {
    // Check if there is a callback function available.
    if (this->dispatcherConfiguration[i].can_consumer_callback != NULL)
    {
      void *context = this->dispatcherConfiguration[i].context;
      ret = this->dispatcherConfiguration[i].can_consumer_callback( 
        context, id, data, dlc, flags );
      if (ret != CANOPEN_MSG_NOT_PROCESSED)
      {
        //break;  // Only one object can consume a message. 
                //This needs to be removed if spy is implemented.
      }
    }
  }
  ReleaseMutex( this->can_message_dispatcher_mutex );
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  CanInterface :: protocolImplementationDispatcher(DWORD ticks)
{
  canOpenStatus ret = CANOPEN_ERROR;

  WaitForSingleObject( this->can_message_dispatcher_mutex , INFINITE );
  for (int i=0; i < MAX_PROCESS_MSG_CALLBACKS; i++)
  {
    // Check if there is a callback function available.
    if (this->dispatcherConfiguration[i].protocol_state_machine_callback != NULL)
    {
      void *context = this->dispatcherConfiguration[i].context;
      ret = this->dispatcherConfiguration[i].protocol_state_machine_callback( context ); 
    }
  }
  ReleaseMutex( this->can_message_dispatcher_mutex );
  return ret;
}

//------------------------------------------------------------------------
// Gets the specific can interface for the given port.
//------------------------------------------------------------------------
CanInterface* CanInterface :: getCanInterface( int port)
{
  if ( port >= 0 && port < MAX_CAN_INTERFACES )
  {
    if ( canInterfaceSingleton[port] == NULL )
    {
      canInterfaceSingleton[port] = new CanInterface( port );
    }
	WaitForSingleObject(canInterfaceSingleton[port]->port_mutex, INFINITE);
    canInterfaceSingleton[port]->port_users++; // Set to at least 1.
	ReleaseMutex(canInterfaceSingleton[port]->port_mutex);
    return canInterfaceSingleton[port];
  }
  else
  {
    return NULL;
  }
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface::removeCanInterface(void)
{
  canOpenStatus ret = CANOPEN_OK;

  WaitForSingleObject(this->port_mutex, INFINITE);  
  this->port_users--;
  if (port_users <= 0)
  {
    canOpenStatus retPortClose = CANOPEN_OK;
    if (this->can_port_opened)
    {
      this->stopDispatcherThread();
	  this->can_port_opened = false;
	  this->can_port_bus_on = false;
      retPortClose = ::canPortClose(this->canHandle);
    }
    if (retPortClose != CANOPEN_OK)
    {
      ret = retPortClose;
    }
  }
  ReleaseMutex(this->port_mutex);

  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
CanInterface* CanInterface :: getCanInterfaceNoCreate(int port)
{
  return canInterfaceSingleton[port];
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: canRead(long * id, void * msg, 
  unsigned int * dlc,  unsigned int * flags)
{  
  return ::canPortRead( this->canHandle, id, msg, dlc, flags);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: canWrite(long id, void * msg, 
  unsigned int dlc, unsigned int flags)
{
  return ::canPortWrite( this->canHandle, id, msg, dlc, flags );
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: canOpenPort( int port, int bitrate )
{
  // Check if the port is already opened.
  canOpenStatus ret = CANOPEN_OK;

  if ( this->can_port_opened == false )
  {
    ret = this->canOpenHardwarePort( port );
    if (ret == CANOPEN_OK)
    {
      ret = this->canSetBitrate( bitrate );
      if ( ret != CANOPEN_OK )
      {
        (void) this->canCloseHardwarePort( port );
      }
    }
    if (ret == CANOPEN_OK) 
    {
      this->can_port_opened = true;
      this->startDispatcherThread();
    }
  }
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: canClosePort(void)
{  
  canOpenStatus ret = CANOPEN_ERROR_CAN_LAYER;
  if ( this->can_port_opened == true )
  {
    ret = this->removeCanInterface(); // Close the current port.
  }
  return ret;
}
    
//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: canGoBusOn(void)
{
  canOpenStatus res = CANOPEN_ERROR;
  if ( this->can_port_bus_on == false ) {
    res = ::canPortGoBusOn( this->canHandle );
    if ( res == CANOPEN_OK ) {
      this->can_port_bus_on = true;
    }
  } else {
    res = CANOPEN_OK;
  }
  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: canGoBusOff(void)
{
  canOpenStatus res;
  if ( this->can_port_bus_on == true )
  {
    res = canPortGoBusOff( this->canHandle );
    if (res == CANOPEN_OK) {
      this->can_port_bus_on = false;
    }
  } else {
    res = CANOPEN_OK;
  }
  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: canLibraryInit(void)
{
  switch (getAdapter())
  {
#ifdef WIN64
  case canAdapter_DEFAULT:
    hCanLib = LoadLibrary(TEXT("canopenlib64_hw.dll"));
    break;
  case canAdapter_IXXAT:
    hCanLib = LoadLibrary(TEXT("canopenlib64_hw_ixxat.dll"));
    break;
  case canAdapter_KVASER:
    hCanLib = LoadLibrary(TEXT("canopenlib64_hw_kvaser.dll"));
    break;
  default:
    hCanLib = LoadLibrary(TEXT("canopenlib64_hw.dll"));
    break;
#else
  case canAdapter_DEFAULT:
    hCanLib = LoadLibrary(TEXT("canopenlib32_hw.dll"));
    break;
  case canAdapter_IXXAT:
    hCanLib = LoadLibrary(TEXT("canopenlib32_hw_ixxat.dll"));
    break;
  case canAdapter_KVASER:
    hCanLib = LoadLibrary(TEXT("canopenlib32_hw_kvaser.dll"));
    break;
  case canAdapter_CAN232:
    hCanLib = LoadLibrary(TEXT("canopenlib32_hw_can232.dll"));
    break;
  case canAdapter_CANUSB:
    hCanLib = LoadLibrary(TEXT("canopenlib32_hw_canusb.dll"));
    break;
  case canAdapter_EMS:
    hCanLib = LoadLibrary(TEXT("canopenlib32_hw_ems.dll"));
    break;
  case canAdapter_PCAN:
    hCanLib = LoadLibrary(TEXT("canopenlib32_hw_pcan.dll"));
    break;
  case canAdapter_TINYCAN:
    hCanLib = LoadLibrary(TEXT("canopenlib32_hw_tinycan.dll"));
    break;
  case canAdapter_ZANTHIC:
    hCanLib = LoadLibrary(TEXT("canopenlib32_hw_zanthic.dll"));
    break;
default:
    hCanLib = LoadLibrary(TEXT("canopenlib32_hw.dll"));
    break;
#endif
  }


    /*
    *	Load proper CANLIB.DLL
    */
//#ifdef WIN64
//    hCanLib = LoadLibrary(TEXT("canopenlib64_hw.dll"));
//#else
//    hCanLib = LoadLibrary(TEXT("canopenlib32_hw.dll"));
//#endif
  if (hCanLib == NULL)
  {
#ifdef CANOPENDLL_BUILT_IN
    MessageBox(NULL, ((LPCTSTR)"CAN hardware layer DLL (canopenlib32_hw.dll) was not found!"), ((LPCTSTR)"Missing file!"), MB_OK);
#else
#ifdef WIN64
    MessageBox(NULL, "CAN hardware layer DLL (canopenlib64_hw.dll) was not found!","Missing file!", MB_OK);MessageBox(NULL, "CAN hardware layer DLL (canopenlib32_hw.dll) was not found!","Missing file!", MB_OK);
#else
    MessageBox(NULL, "CAN hardware layer DLL (canopenlib32_hw.dll) was not found!","Missing file!", MB_OK);MessageBox(NULL, "CAN hardware layer DLL (canopenlib32_hw.dll) was not found!","Missing file!", MB_OK);
#endif
#endif
    return CANOPEN_ERROR_DRIVER;
  }
    /*
    *	Connect to the functions in the DLL.
    */
  canPortLibraryInit = (canPortLibraryInitFP)GetProcAddress(hCanLib, "canPortLibraryInit");
  canPortOpen = (canPortOpenFP)GetProcAddress(hCanLib, "canPortOpen");
  canPortClose = (canPortCloseFP)GetProcAddress(hCanLib, "canPortClose");
  canPortBitrateSet = (canPortBitrateSetFP)GetProcAddress(hCanLib, "canPortBitrateSet");
  canPortEcho = (canPortEchoFP)GetProcAddress(hCanLib, "canPortEcho");
  canPortGoBusOn = (canPortGoBusOnFP)GetProcAddress(hCanLib, "canPortGoBusOn");
  canPortGoBusOff = (canPortGoBusOffFP)GetProcAddress(hCanLib, "canPortGoBusOff");
  canPortWrite = (canPortWriteFP)GetProcAddress(hCanLib, "canPortWrite");
  canPortRead = (canPortReadFP)GetProcAddress(hCanLib, "canPortRead");
  canPortGetSerialNumber = (canPortGetSerialNumberFP)GetProcAddress(hCanLib, "canPortGetSerialNumber");

  if (canPortLibraryInit == NULL
    || canPortOpen == NULL
    || canPortClose == NULL
    || canPortBitrateSet == NULL
    || canPortEcho == NULL
    || canPortGoBusOn == NULL
    || canPortGoBusOff == NULL
    || canPortWrite == NULL
    || canPortRead == NULL
    || canPortGetSerialNumber == NULL)
  {
    return CANOPEN_ERROR_CAN_LAYER;
  }

	return ::canPortLibraryInit();
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: canOpenHardwarePort(int port)
{
  canOpenStatus ret = ::canPortOpen( port, &this->canHandle );
  if (ret == CANOPEN_OK)
  {
    ret = ::canPortEcho( this->canHandle, true);
  }
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: canCloseHardwarePort(int port)
{
  return ::canPortClose( this->canHandle );
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: canSetBitrate(int bitrate)
{
  return ::canPortBitrateSet( this->canHandle, bitrate );
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanInterface :: canGetSerialNumber(char *buffer, int bufferLen)
{
    return ::canPortGetSerialNumber(
       this->canHandle,
       buffer, 
       bufferLen);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void CanInterface :: startDispatcherThread(void)
{
  DWORD tid;
  if( !this->is_can_dispatcher_thread_running ) 
  { 
    this->can_frame_dispatcher_thread_handle = CreateThread(NULL
        , 0
        , &CanInterface::canFrameDispatcherThread
        , (LPVOID)this
        , CREATE_SUSPENDED
        , &tid);
    is_can_dispatcher_thread_running = TRUE;
    /* Start thread. Only suspend at shutdown. */
    (void)ResumeThread( this->can_frame_dispatcher_thread_handle );   
  } 
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void CanInterface :: stopDispatcherThread(void)
{
  this->is_can_dispatcher_thread_running = FALSE;
  // Waits for the thread to terminate.
  WaitForSingleObject(this->can_frame_dispatcher_thread_mutex, INFINITE );
  ReleaseMutex(this->can_frame_dispatcher_thread_mutex);
}

