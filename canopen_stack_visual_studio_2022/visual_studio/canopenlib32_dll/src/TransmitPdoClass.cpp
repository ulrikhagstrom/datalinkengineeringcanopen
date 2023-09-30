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

#include "TransmitPdoClass.h"
#include <stdio.h>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
TransmitPDO::TransmitPDO(void)
{
  this->remote_node_id = 0;
  this->can_interface = NULL;
  this->can_message_handler_index = -1;
  this->tPdo_periodic_transmission = false;
  //
  // Register a timer to be used for doing the node-guard polling.
  //
  timer = TimeClass::getTimeInterface();
  timer->registerPeriodicCallback((TimeClass::TimeHandlerFuncPtr)timerCallbackHandler,
    this, 1000, &periodic_timer_index);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
TransmitPDO :: ~TransmitPDO(void)
{
  timer->unregisterPeriodicCallback(this->periodic_timer_index);
  timer->removeTimeInterface();
  (void)this->canHardwareDisconnect();
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TransmitPDO::timerCallbackHandler(void* transmit_pdo_context)
{
  if (((TransmitPDO*)transmit_pdo_context)->can_hardware_is_initiated)
  {
    if (((TransmitPDO*)transmit_pdo_context)->tPdo_periodic_transmission)
    {
      ((TransmitPDO*)transmit_pdo_context)->transmitPdo();
    }
  }
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  TransmitPDO::setData(COBID cobid, u8* data, u8 len)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (len > 8 || len < 0)
  {
    ret = CANOPEN_ARG_ERROR;
  }
  else
  {
    this->tPdo_cobid = cobid;
    memcpy(this->tPdo_data, data, len);
    this->tPdo_len = len;
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  TransmitPDO::transmitPdo(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (this->can_hardware_is_initiated)
  {
    if (can_interface != NULL)
    {
      return can_interface->canWrite(this->tPdo_cobid, this->tPdo_data, this->tPdo_len, 0);
    }
    else
    {
      return CANOPEN_ERROR;
    }
  }
  return CANOPEN_ERROR_HW_NOT_CONNECTED;
  return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  TransmitPDO::periodicTransmission(bool value)
{
  canOpenStatus ret = CANOPEN_ERROR;
  this->tPdo_periodic_transmission = value;
  ret = CANOPEN_OK;
  return ret;
}



