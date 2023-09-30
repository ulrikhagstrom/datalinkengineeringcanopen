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

#include "EmcyClientClass.h"
#include <stdio.h>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
EmcyClient::EmcyClient(void)
{
  this->node_id = -1;
  this->can_hardware_is_initiated = false;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
EmcyClient :: ~EmcyClient(void)
{
  (void)this->canHardwareDisconnect();
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus EmcyClient::nodeSetId(u8 node_id)
{
  this->node_id = node_id;
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
// Callback for processing CAN messages that has been received by the interface.
//------------------------------------------------------------------------
canOpenStatus  EmcyClient::canFrameConsumer(unsigned long id, unsigned char* data,
  unsigned int dlc, unsigned int flags)
{
  return CANOPEN_MSG_NOT_PROCESSED;
}

//------------------------------------------------------------------------
// Callback for processing CAN messages that has been received by the interface.
//------------------------------------------------------------------------
canOpenStatus  EmcyClient::transferHelper(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  EmcyClient::sendEmcyMessage(u8 nodeId, u16 emcy_error_code, u8 error_register, u8* manufSpecificErrorField)
{
  canOpenStatus ret = CANOPEN_ERROR_HW_NOT_CONNECTED;
  if (can_interface != NULL)
  {
    u8 canData[8];

    setU16Val(emcy_error_code, canData, 0);
    canData[2] = error_register;

    for (int i = 0; i < 5; i++)
    {
      if (manufSpecificErrorField != NULL)
        canData[3 + i] = manufSpecificErrorField[i];
      else
        canData[3 + i] = 0;
    }

    ret = can_interface->canWrite(0x80 + nodeId, canData, 8, 0);
    if (ret != CANOPEN_OK)
    {
      DebugLogToFile("writeToCanBus failed\n");
    }
  }
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  EmcyClient::sendEmcyMessage(u16 emcy_error_code, u8 error_register, u8* manufSpecificErrorField)
{
  return sendEmcyMessage(this->node_id, emcy_error_code, error_register, manufSpecificErrorField);
}

