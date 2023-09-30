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

#include "EmcyServerClass.h"
#include <stdio.h>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
EmcyServer::EmcyServer(void)
{
  this->emergency_messagae_callback = NULL;
  this->can_interface = NULL;
  this->can_message_handler_index = -1;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
EmcyServer :: ~EmcyServer(void)
{
  (void)this->canHardwareDisconnect();
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus EmcyServer::canHardwareConnect(u8 port, u32 bitrate)
{
  return CanConnection::canHardwareInit(port, bitrate, canFrameConsumerW, NULL);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus EmcyServer::canFrameConsumerW(void* emcy_server,
  unsigned long id, unsigned char* data, unsigned int dlc, unsigned int flags)
{
  EmcyServer* emcyServer = (EmcyServer*)emcy_server;
  return emcyServer->canFrameConsumer(id, data, dlc, flags);
}

//------------------------------------------------------------------------
// Callback for processing CAN messages that has been received by the interface.
//------------------------------------------------------------------------
canOpenStatus  EmcyServer::canFrameConsumer(unsigned long id,
  unsigned char* data, unsigned int dlc, unsigned int flags)
{
  canOpenStatus ret = CANOPEN_MSG_NOT_PROCESSED;
  if (emergency_messagae_callback != NULL) // Perform callback if configured to do so.
  {
    if (id > 0x80 && id < 0xff)
    {
      u16 emcyCode;
      u8 nodeId;
      u8 errRegister;
      u8 manufSpecErrorField[5];

      nodeId = (u8)(id - 0x80);
      emcyCode = getU16Val(data, 0);
      errRegister = data[2];
      for (int i = 0; i < 5; i++)
        manufSpecErrorField[i] = data[3 + i];

      this->emergency_messagae_callback(
        this->context,
        nodeId,
        emcyCode,
        errRegister,
        manufSpecErrorField);
    }
  }
  return ret;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  EmcyServer::registerEmergencyMessageCallBack(void* context, EmcyMessageFunPtr fp)
{
  this->emergency_messagae_callback = fp;
  this->context = context;
  return CANOPEN_OK;
}
