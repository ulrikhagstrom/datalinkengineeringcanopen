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
#include "SyncServerClass.h"
#include <stdio.h>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
SyncServer :: SyncServer (void)
{
  this->sync_messagae_callback = NULL;
  this->can_interface = NULL;
  this->can_message_handler_index = -1;
  this->COBID_Sync = 0x80;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
SyncServer :: ~SyncServer (void)
{
  (void)this->canHardwareDisconnect();
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus SyncServer :: canHardwareConnect(u8 port, u32 bitrate)
{
  return CanConnection::canHardwareInit(port,
                                        bitrate,
                                        canFrameConsumerW,
                                        NULL);
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus SyncServer :: canFrameConsumerW(void *sync_server_object, 
  unsigned long id, unsigned char *data, unsigned int dlc, unsigned int flags)
{
  SyncServer *syncServer = (SyncServer*) sync_server_object;
  return syncServer->canFrameConsumer(id, data, dlc, flags);
}

//------------------------------------------------------------------------
// Callback for processing CAN messages that has been received by the interface.
//------------------------------------------------------------------------
canOpenStatus  SyncServer :: canFrameConsumer(unsigned long id, 
  unsigned char *data, unsigned int dlc, unsigned int flags)
{
  canOpenStatus ret = CANOPEN_MSG_NOT_PROCESSED;
  if (sync_messagae_callback != NULL) // Perform callback if configured to do so.
  {
    if (id == this->COBID_Sync)
    {
      this->sync_messagae_callback(this->context);
    }
  }
  return ret;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  SyncServer :: registerSyncMessageCallBack(void *context, SyncMessageFunPtr fp)
{
  this->sync_messagae_callback = fp;
  this->context = context;
  return CANOPEN_OK;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  SyncServer :: reconfigureSyncCOBID(u32 COBID)
{
  this->COBID_Sync = COBID;
  return CANOPEN_OK;
}