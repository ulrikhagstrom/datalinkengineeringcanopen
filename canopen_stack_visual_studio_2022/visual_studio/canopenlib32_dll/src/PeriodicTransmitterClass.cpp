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

#include "PeriodicTransmitterClass.h"
#include <stdio.h>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
PeriodicTransmitter::PeriodicTransmitter(void)
{
  this->remote_node_id = 0;
  this->can_interface = NULL;
  this->can_message_handler_index = -1;
  this->tMsg_periodic_transmission = false;
  //
  // Register a timer to be used for doing the node-guard polling.
  //
  timer = TimeClass::getTimeInterface();
  timer->registerPeriodicCallback((TimeClass::TimeHandlerFuncPtr)timerCallbackHandler,
    this, 100, &periodic_timer_index);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
PeriodicTransmitter :: ~PeriodicTransmitter(void)
{
  timer->unregisterPeriodicCallback(this->periodic_timer_index);
  timer->removeTimeInterface();
  (void)this->canHardwareDisconnect();
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void PeriodicTransmitter::timerCallbackHandler(void* periodic_transmitter_context)
{
  if (((PeriodicTransmitter*)periodic_transmitter_context)->can_hardware_is_initiated)
  {
    if (((PeriodicTransmitter*)periodic_transmitter_context)->tMsg_periodic_transmission)
    {
      if ((((PeriodicTransmitter*)periodic_transmitter_context)->tMsg_time_stamp_last_tx +
        ((PeriodicTransmitter*)periodic_transmitter_context)->tMsg_period_time) < TimeClass::readTimer())
        ((PeriodicTransmitter*)periodic_transmitter_context)->transmitMsg();
    }
  }
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  PeriodicTransmitter::setData(COBID cobid, u8* data, u8 len)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (len > 8 || len < 0)
  {
    ret = CANOPEN_ARG_ERROR;
  }
  else
  {
    this->tMsg_cobid = cobid;
    if (data != NULL)
      memcpy(this->tMsg_data, data, len);
    this->tMsg_len = len;
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  PeriodicTransmitter::transmitMsg(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (this->can_hardware_is_initiated)
  {
    if (can_interface != NULL)
    {
      tMsg_time_stamp_last_tx = TimeClass::readTimer();
      return can_interface->canWrite(this->tMsg_cobid, this->tMsg_data, this->tMsg_len, 0);
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
canOpenStatus  PeriodicTransmitter::periodicTransmission(bool value)
{
  canOpenStatus ret = CANOPEN_ERROR;
  this->tMsg_periodic_transmission = value;

  if (value == TRUE)
    this->transmitMsg();

  ret = CANOPEN_OK;
  return ret;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  PeriodicTransmitter::setTransmissionPeriodTime(unsigned long sync_period_time)
{
  canOpenStatus ret = CANOPEN_OK;
  this->tMsg_period_time = sync_period_time;
  return ret;
}

