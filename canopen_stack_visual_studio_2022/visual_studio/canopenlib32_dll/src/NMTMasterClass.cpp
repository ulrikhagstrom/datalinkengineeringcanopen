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

#include "NMTMasterClass.h"
#include <stdio.h>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
NMTMaster::NMTMaster(void)
{
  this->remote_node_id = 0;
  for (int i = 0; i < 128; i++)
  {
    this->nodeList[i].node_state = UNDEFINED;
    this->nodeList[i].node_guard_poll = false;
    this->nodeList[i].error_control = false;
    this->nodeList[i].time_stamp = TimeClass::readTimer();
  }
  this->operational_state_callback = NULL;
  this->can_interface = NULL;
  this->can_message_handler_index = -1;
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
NMTMaster :: ~NMTMaster(void)
{
  timer->unregisterPeriodicCallback(this->periodic_timer_index);
  timer->removeTimeInterface();
  (void)this->canHardwareDisconnect();
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void NMTMaster::timerCallbackHandler(void* nmt_master_context)
{
  if (((NMTMaster*)nmt_master_context)->can_hardware_is_initiated)
  {
    ((NMTMaster*)nmt_master_context)->sendNodeGuardMessages();
    ((NMTMaster*)nmt_master_context)->checkHeartbeatEvent();
  }
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus NMTMaster::canHardwareConnect(u8 port, u32 bitrate)
{
  return (this->canHardwareInit(port,
    bitrate,
    (DispatcherCanFuncPtr)canFrameConsumerW,
    (ProtocolImplementationStateMachineFuncPtr)transferHelperW));
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus NMTMaster::canFrameConsumerW(void* nmt_master_object,
  unsigned long id, unsigned char* data, unsigned int dlc, unsigned int flags)
{
  NMTMaster* nmtMaster = (NMTMaster*)nmt_master_object;
  return nmtMaster->canFrameConsumer(id, data, dlc, flags);
}

//------------------------------------------------------------------------
// Wrapper for being able to setup callbacks to non-static funcs.
//------------------------------------------------------------------------
canOpenStatus NMTMaster::transferHelperW(void* nmt_master_object)
{
  canOpenStatus ret = CANOPEN_ERROR;
  NMTMaster* nmtMaster = (NMTMaster*)nmt_master_object;
  ret = nmtMaster->transferHelper();
  return ret;
}

//------------------------------------------------------------------------
// Callback for processing CAN messages that has been received by the interface.
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::canFrameConsumer(unsigned long id,
  unsigned char* data, unsigned int dlc, unsigned int flags)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if ((id > 0x700) && (id <= 0x77f) && ((flags & CAN_MSG_RTR) == 0))
  {
    u8 node_id = id & 0x7f; /* 0 -> 127 */
    u8 state = (data[0] & 0x7f);  /* Bit 6..0 tells the state for node (DS301, p.9-48) */
    switch (state)
    {
    case BOOTUP_STATE:  /* Bootup, see 9-48. */
      this->nodeList[node_id].node_state = BOOTUP_STATE;
      this->nodeList[node_id].time_stamp = TimeClass::readTimer();;  // GetTickCount();
      break;
    case STOPPED_STATE:  /* Stopped, see 9-48. */
      this->nodeList[node_id].node_state = STOPPED_STATE;
      this->nodeList[node_id].time_stamp = TimeClass::readTimer();;
      break;
    case OPERATIONAL_STATE: /* Operational, see 9-48. */
      this->nodeList[node_id].node_state = OPERATIONAL_STATE;
      this->nodeList[node_id].time_stamp = TimeClass::readTimer();
      break;
    case RESET_STATE: /* PreOperational, see 9-48. */
      this->nodeList[node_id].node_state = RESET_STATE;
      this->nodeList[node_id].time_stamp = TimeClass::readTimer();
      break;
    default:
      this->nodeList[node_id].node_state = UNDEFINED;
      this->nodeList[node_id].time_stamp = TimeClass::readTimer();
      break;
    }
    if (operational_state_callback != NULL) // Perform callback if configured to do so.
    {
      this->operational_state_callback(this->context, node_id, state); // 
    }
  }
  else
  {
    ret = CANOPEN_MSG_NOT_PROCESSED;
  }
  return ret;
}

//------------------------------------------------------------------------
// Callback for processing CAN messages that has been received by the interface.
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::transferHelper(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  return ret;
}

//------------------------------------------------------------------------
// Method to connect to node...
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::connect(u8 node_id)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if ((node_id >= 0) && (node_id < 128))
  {
    this->remote_node_id = node_id;
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
// Set remote node state.
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodeSetState(SetNodeStateCmd node_stateCmd)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 canDataField[8];

  switch (node_stateCmd)
  {
  case START_CMD: /* Start Remote Node Protocol, p.9-44. */
    canDataField[0] = START_CMD;
    ret = CANOPEN_OK;
    break;
  case STOP_CMD: /* Stop Remote Node Protocol, p.9-44. */
    canDataField[0] = STOP_CMD;
    ret = CANOPEN_OK;
    break;
  case PRE_OPERATIONAL_CMD:  /* Enter Pre-Operational Protocol, p.9-45. */
    canDataField[0] = PRE_OPERATIONAL_CMD;
    ret = CANOPEN_OK;
    break;
  case RESET_CMD: /* Reset Node Protocol, p.9-45. */
    canDataField[0] = RESET_CMD;
    ret = CANOPEN_OK;
    break;
  case RESET_COMMUNCATION_CMD: /* Reset Communication Protocol, p.9-46. */
    canDataField[0] = RESET_COMMUNCATION_CMD;
    ret = CANOPEN_OK;
    break;
  default:
    ret = CANOPEN_ARG_ERROR;
    break;
  }
  if (ret == CANOPEN_OK)
  {
    canDataField[1] = this->remote_node_id;
    if (this->can_hardware_is_initiated)
      ret = can_interface->canWrite(0x0, canDataField, 2, 0);
    else
      ret = CANOPEN_ERROR_HW_NOT_CONNECTED;
  }
  return ret;
}

//------------------------------------------------------------------------
// 9.2.6.2.1 Module Control Protocols, Start Remote Node Protocol.
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodeStart(void)
{
  return (this->nodeSetState(START_CMD));
}

//------------------------------------------------------------------------
// 9.2.6.2.1 Module Control Protocols, Start Remote Node Protocol.
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodeStart(u8 node_id)
{
  canOpenStatus ret = this->connect(node_id);
  if (ret == CANOPEN_OK)
  {
    ret = this->nodeSetState(START_CMD);
  }
  return ret;
}

//------------------------------------------------------------------------
// 9.2.6.2.1 Module Control Protocols, Stop Remote Node Protocol.
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodeStop(void)
{
  return (this->nodeSetState(STOP_CMD));
}

//------------------------------------------------------------------------
// 9.2.6.2.1 Module Control Protocols, Stop Remote Node Protocol.
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodeStop(u8 node_id)
{
  canOpenStatus ret = this->connect(node_id);
  if (ret == CANOPEN_OK)
  {
    ret = this->nodeSetState(STOP_CMD);
  }
  return ret;
}

//------------------------------------------------------------------------
// 9.2.6.2.1 Module Control Protocols, Enter Pre-Operational Protocol.
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodePreoperational(void)
{
  return (this->nodeSetState(PRE_OPERATIONAL_CMD));
}

//------------------------------------------------------------------------
// 9.2.6.2.1 Module Control Protocols, Enter Pre-Operational Protocol.
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodePreoperational(u8 node_id)
{
  canOpenStatus ret = this->connect(node_id);
  if (ret == CANOPEN_OK)
  {
    ret = this->nodeSetState(PRE_OPERATIONAL_CMD);
  }
  return ret;
}

//------------------------------------------------------------------------
//  9.2.6.2.1 Module Control Protocols, Reset Node Protocol.
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodeReset(void)
{
  return (this->nodeSetState(RESET_CMD));
}

//------------------------------------------------------------------------
//  9.2.6.2.1 Module Control Protocols, Reset Node Protocol.
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodeReset(u8 node_id)
{
  canOpenStatus ret = this->connect(node_id);
  if (ret == CANOPEN_OK)
  {
    ret = this->nodeSetState(RESET_CMD);
  }
  return ret;
}

//------------------------------------------------------------------------
//  9.2.6.2.1 Module Control Protocols, Reset Communication Protocol
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodeResetCommunication(void)
{
  return (this->nodeSetState(RESET_COMMUNCATION_CMD));
}

//------------------------------------------------------------------------
//  9.2.6.2.1 Module Control Protocols, Reset Communication Protocol
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodeResetCommunication(u8 node_id)
{
  canOpenStatus ret = this->connect(node_id);
  if (ret == CANOPEN_OK)
  {
    ret = this->nodeSetState(RESET_COMMUNCATION_CMD);
  }
  return ret;
}

//------------------------------------------------------------------------
// Heartbeat Protocol, p.9-48.
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodeReadOperationalState(u8 node_id,
  unsigned long maxMsTimeout, NodeState* node_stateP)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (node_id > 0 && node_id < 128)
  {
    unsigned long now = GetTickCount();
    if ((now - this->nodeList[node_id].time_stamp) < maxMsTimeout)
    {
      *node_stateP = this->nodeList[node_id].node_state;
      ret = CANOPEN_OK;
    }
    else
    {
      *node_stateP = UNDEFINED;
    }
  }
  else
  {
    ret = CANOPEN_ARG_ERROR;
  }
  return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodeGuardPollStart(u8 node_id,
  unsigned long node_life_time_ms)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (this->can_hardware_is_initiated)
  {
    if (node_id > 0 && node_id < 128)
    {
      this->nodeList[node_id].time_stamp = TimeClass::readTimer();
      this->nodeList[node_id].node_guard_poll = true;
      this->nodeList[node_id].error_control = true;
      ret = CANOPEN_OK;
    }
    else
    {
      ret = CANOPEN_ARG_ERROR;
    }
  }
  else
  {
    ret = CANOPEN_ERROR_HW_NOT_CONNECTED;
  }
  return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::nodeGuardPollStop(u8 node_id)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (node_id > 0 && node_id < 128)
  {
    this->nodeList[node_id].error_control = false;
    this->nodeList[node_id].node_guard_poll = false;
    ret = CANOPEN_OK;
  }
  else
  {
    ret = CANOPEN_ARG_ERROR;
  }
  return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::heartbeatMonitorStart(u8 node_id,
  unsigned long heartbeat_consumer_time_ms)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (node_id > 0 && node_id < 128)
  {
    this->nodeList[node_id].time_stamp = TimeClass::readTimer();
    this->nodeList[node_id].node_guard_poll = false;
    this->nodeList[node_id].error_control = true;
    ret = CANOPEN_OK;
  }
  else
  {
    ret = CANOPEN_ARG_ERROR;
  }
  return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::heartbeatMonitorStop(u8 node_id)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (node_id > 0 && node_id < 128)
  {
    this->nodeList[node_id].error_control = false;
    this->nodeList[node_id].node_guard_poll = false;
    ret = CANOPEN_OK;
  }
  else
  {
    ret = CANOPEN_ARG_ERROR;
  }
  return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::sendNodeGuardMessages(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  for (int node_id = 0; node_id < 127; node_id++)
  {
    if (this->nodeList[node_id].error_control &&
      this->nodeList[node_id].node_guard_poll)
    {
      ret = can_interface->canWrite(0x700 + node_id, NULL, 2, CAN_MSG_RTR);
      if (ret != CANOPEN_OK)
        return ret;
    }
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void NMTMaster::checkHeartbeatEvent(void)
{
  if (can_interface != NULL)
  {
    unsigned long now = TimeClass::readTimer();
    for (int node_id = 0; node_id < 127; node_id++)
    {
      if (this->nodeList[node_id].error_control)
      {
        if ((now - this->nodeList[node_id].time_stamp) > 3000) // 3 sec.
        {
          this->nodeList[node_id].node_state = UNDEFINED; // Lost        
          if (operational_state_callback != NULL) // Perform callback if configured to do so.
          {
            this->operational_state_callback(this->context,
              node_id,
              nodeList[node_id].node_state); // Send of the undefined state. 
          }
        }
      }
    }
  }
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  NMTMaster::registerRemoteNodeStateCallback(NMTOperationalStateFunPtr fp, void* op)
{
  this->operational_state_callback = fp;
  this->context = op;
  return CANOPEN_OK;
}
