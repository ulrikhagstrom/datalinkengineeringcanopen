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

#include "CanMonitorClass.h"

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
CanMonitor :: CanMonitor()
{
  this->rx_tx_mutex = rx_tx_mutex = CreateMutex(NULL, FALSE, NULL);
  this->can_hardware_is_initiated = false;
  this->application_can_receive_callback = NULL;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
CanMonitor :: ~CanMonitor()
{
  WaitForSingleObject(this->rx_tx_mutex, INFINITE);
  this->application_can_receive_callback = NULL;
  this->canHardwareDisconnect();
  CloseHandle(this->rx_tx_mutex);
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanMonitor :: canFrameConsumerW(void *can_monitor_object, 
  unsigned long id, unsigned char *data, unsigned int  dlc, unsigned int flags)
{
  CanMonitor *can_monitor = (CanMonitor*) can_monitor_object;
  (void)can_monitor->canFrameConsumer(id, data, dlc, flags);
  return CANOPEN_MSG_NOT_PROCESSED; // Should never consume a CAN frame.
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  CanMonitor :: canFrameConsumer( u32 id, u8 *data, u8 dlc,  u32 flags)
{
  WaitForSingleObject(this->rx_tx_mutex, INFINITE);
  if (this->application_can_receive_callback != NULL)
  {
    (void)this->application_can_receive_callback( 
      this->application_context, id, data, dlc, flags);
  }
  ReleaseMutex(this->rx_tx_mutex);
  return CANOPEN_MSG_NOT_PROCESSED;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  CanMonitor :: canHardwareConnect(u8 port, u32 bitrate)
{
  return CanConnection :: canHardwareInit(port, bitrate, canFrameConsumerW, NULL);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  CanMonitor :: canHardwareDisconnect(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if ( this->can_hardware_is_initiated == false )
  {
    ret = CANOPEN_ERROR_HW_NOT_CONNECTED;
  }
  else
  {
    if ( this->can_interface != NULL )
    {
      can_interface->unregisterCanMessageHandler( can_message_handler_index );
      ret = can_interface->canClosePort();
    }
    this->can_message_handler_index = - 1;
    this->can_hardware_is_initiated = false;
    this->can_interface = NULL;
    ret = CANOPEN_OK;
  }  
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  CanMonitor :: registerCanReceiveCallback( void *context, RawCanReceiveFunPtr raw_can_receive_callback )
{
  this->application_context = context;
  this->application_can_receive_callback = raw_can_receive_callback;
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  CanMonitor :: canWrite( u32 id, u8 *data, u8 dlc, u32 flags )
{
  if ( this->can_hardware_is_initiated )
  {
    if ( can_interface != NULL )
    {
      return can_interface->canWrite( id, data, dlc, flags );
    } 
    else
    {
      return CANOPEN_ERROR;
    }
  }
  return CANOPEN_ERROR_HW_NOT_CONNECTED;
}