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

#include "NMTSlaveClass.h"
#include <stdio.h>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
NMTSlave :: NMTSlave (void)
{
  this->node_id = -1;
  this->node_state = 0;
  this->toggle = 0;
  this->local_node_operational_state_change_callback = NULL;
  this->can_hardware_is_initiated = false;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
NMTSlave :: ~NMTSlave (void)
{
  (void)this->canHardwareDisconnect();
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus NMTSlave :: nodeSetId(u8 node_id)
{
  this->node_id = node_id;
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus NMTSlave :: nodeSetState(u8 node_state)
{
  this->node_state = node_state;
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  NMTSlave :: canHardwareConnect(u8 port, u32 bitrate)
{
  return CanConnection::canHardwareInit(port,
                                        bitrate,
                                        canFrameConsumerW,
                                        transferHelperW);
}

//------------------------------------------------------------------------
// Wrapper for being able to setup callbacks to non-static funcs.
//------------------------------------------------------------------------
canOpenStatus NMTSlave :: canFrameConsumerW(void *nmt_slave_context, 
  unsigned long id, unsigned char *data, unsigned int dlc, unsigned int flags)
{
  NMTSlave *nmt_slave = (NMTSlave*) nmt_slave_context;
  return nmt_slave->canFrameConsumer(id, data, dlc, flags);
}

//------------------------------------------------------------------------
// Wrapper for being able to setup callbacks to non-static funcs.
//------------------------------------------------------------------------
canOpenStatus NMTSlave :: transferHelperW(void *nmt_slave_context)
{
  canOpenStatus ret = CANOPEN_ERROR;
  NMTSlave *nmt_slave = (NMTSlave*)nmt_slave_context;
  ret = nmt_slave->transferHelper();
  return ret;
}

//------------------------------------------------------------------------
// Callback for processing CAN messages that has been received by the interface.
//------------------------------------------------------------------------
canOpenStatus  NMTSlave :: canFrameConsumer(unsigned long id, unsigned char *data, 
  unsigned int dlc, unsigned int flags)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if ( (id == (0x700 + this->node_id)) && (flags & CAN_MSG_RTR))
  {
    // Heartbeat poll message received.
    u8 canData = toggle | this->node_state;
    if (can_interface->canWrite((0x700 + this->node_id), &canData, 1, 0) == CANOPEN_OK)
    {
      toggle ^= 0x80;  //XOR
      ret = CANOPEN_OK;
    }
  } 
  else if ( id == 0x0 && dlc == 2 )
  {
    // Module Control Protocol
    if ( data[1] == this->node_id || data[1] == 0 )    //patch to handle 0 for reset all nodes
    {
      this->node_state = data[0];   // qqq needs to be improved.
      if (this->local_node_operational_state_change_callback != NULL)
      {
        this->local_node_operational_state_change_callback( 
          this->local_node_operational_state_change_callback_context, this->node_state);
      }
      ret = CANOPEN_OK;
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
canOpenStatus  NMTSlave :: transferHelper(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  return ret;
}

//------------------------------------------------------------------------
// Method to send can frames through NMT
//------------------------------------------------------------------------
canOpenStatus NMTSlave :: writeMessage(long id, void* msg, unsigned int dlc, unsigned int flags)
{
    canOpenStatus ret = CANOPEN_ERROR;

    ret = this->can_interface->canWrite(id, msg, dlc, flags);

    return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  NMTSlave :: registerLocalNodeStateChangeCallback(
  NMTLocalNodeOperationalStateFunPtr state_change_callback_function, void *context)
{
  canOpenStatus ret = CANOPEN_ERROR;
  this->local_node_operational_state_change_callback = state_change_callback_function;
  this->local_node_operational_state_change_callback_context = context;
  ret = CANOPEN_OK;
  return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
u8 NMTSlave :: getNodeState( void )
{
  return this->node_state;
}
