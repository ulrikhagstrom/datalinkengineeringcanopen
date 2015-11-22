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

#include "ReceivePDOClass.h"
#include <stdio.h>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
ReceivePDO :: ReceivePDO (void)
{
  this->receive_pdo_callback = NULL;
  this->can_interface = NULL;
  this->can_message_handler_index = -1;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
ReceivePDO :: ~ReceivePDO (void)
{
  (void)this->canHardwareDisconnect();
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus ReceivePDO :: canHardwareConnect(u8 port, u32 bitrate)
{
  return CanConnection::canHardwareInit(port, bitrate, canFrameConsumerW, NULL);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus ReceivePDO :: canFrameConsumerW(void *receive_pdo_object, 
  unsigned long id, unsigned char *data, unsigned int dlc, unsigned int flags)
{
  ReceivePDO *receivePDO = (ReceivePDO*) receive_pdo_object;
  return receivePDO->canFrameConsumer(id, data, dlc, flags);
}

//------------------------------------------------------------------------
// Callback for processing CAN messages that has been received by the interface.
//------------------------------------------------------------------------
canOpenStatus  ReceivePDO :: canFrameConsumer(unsigned long id, 
  unsigned char *data, unsigned int dlc, unsigned int flags)
{
  canOpenStatus ret = CANOPEN_MSG_NOT_PROCESSED;
  if (receive_pdo_callback != NULL) // Perform callback if configured to do so.
  {
    if ((u32)id == this->cobid)
    {
      char databuf[8];

      // In case the callback mess up the data contents.
      for (unsigned int i = 0; i < dlc; i++)
        databuf[i] = data[i];

      this->receive_pdo_callback(
        this->context,
        this->cobid,
        (u8*)databuf,
        dlc);
    }
  }
  return ret;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  ReceivePDO :: registerReceivePdoMessageCallBack(void *context, ReceivePdoFunPtr fp)
{
  this->receive_pdo_callback = fp;
  this->context = context;
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------

canOpenStatus ReceivePDO :: setCobid(COBID cobid)
{
  this->cobid = cobid;
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------

canOpenStatus ReceivePDO::requestPDO(u8 dlc)
{
	canOpenStatus ret = CANOPEN_ERROR;
	if (this->can_hardware_is_initiated)
	{
		if (can_interface != NULL)
		{
			return can_interface->canWrite(this->cobid, NULL, dlc, CAN_MSG_RTR);
		}
		else
		{
			return CANOPEN_ERROR;
		}
	}
	return CANOPEN_ERROR_HW_NOT_CONNECTED;
	return ret;
}
