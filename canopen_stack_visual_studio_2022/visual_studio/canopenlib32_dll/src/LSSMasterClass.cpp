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

#include "LSSMasterClass.h"
#include <stdio.h>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
LSSMaster :: LSSMaster (void)
{
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
LSSMaster :: ~LSSMaster (void)
{
  timer->unregisterPeriodicCallback( this->periodic_timer_index );
  timer->removeTimeInterface(); 
  (void)this->canHardwareDisconnect();
}

//------------------------------------------------------------------------
// CiA DSP-305 v.1.0: 3.9.1 Switch Mode Global
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: switchModeGlobal(u8 mode)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if ( can_interface != NULL )
  {
    u8 canData[8] = {LSS_CMD_SWITCH_STATE_GLOBAL, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    setU8Val(mode, canData, 1);

    ret = can_interface->canWrite(0x7e5, canData, 8, 0); 
    if (ret != CANOPEN_OK)
    {
      DebugLogToFile("canWrite failed\n");
    }
  }
  return ret;
}


//------------------------------------------------------------------------
// Generic switch mode helper without timeout.
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: switchModeSelectiveGeneric(u8 cs, u32 param)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if ( can_interface != NULL )
  {
    u8 canData[8] = {cs, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    setU32Val(param, canData, 1);

    ret = can_interface->canWrite(0x7e5, canData, 8, 0); 
    if (ret != CANOPEN_OK)
    {
      DebugLogToFile("canWrite failed\n");
    }
  }
  return ret;}


//------------------------------------------------------------------------
// CiA DSP-305 v.2.2.17: 7.4.2 Switch Mode Selective
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: switchModeSelectiveSyncResponse(u8 cs, u32 parameter)
{
    unsigned long now = TimeClass::readTimer();
    unsigned long timeout = now + 3000;
    this->wait_lss_mode = true;
    canOpenStatus res = switchModeSelectiveGeneric(cs, parameter);
    if (res == CANOPEN_OK)
    {
        while (this->wait_lss_mode)
        {
            now = TimeClass::readTimer();
            if (timeout < now)
            {
                res = CANOPEN_TIMEOUT;
                break;
            }
        }
    }
    return res;
}

//------------------------------------------------------------------------
// CiA DSP-305 v.2.2.17: 7.4.2 Switch Mode Selective
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: switchModeSelectiveVendorId(u32 vendorId)
{
    canOpenStatus res = switchModeSelectiveGeneric(LSS_CMD_SWITCH_SELECTIVE_VENDOR, vendorId);
    return res;
}

//------------------------------------------------------------------------
// CiA DSP-305 v.2.2.17: 7.4.2 Switch Mode Selective
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: switchModeSelectiveProductCode(u32 productCode)
{
    canOpenStatus res = switchModeSelectiveGeneric(LSS_CMD_SWITCH_SELECTIVE_PRODUCT, productCode);
    return res;
}


//------------------------------------------------------------------------
// CiA DSP-305 v.2.2.17: 7.4.2 Switch Mode Selective
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: switchModeSelectiveRevisionNumber(u32 revisionNumber)
{
    canOpenStatus res = switchModeSelectiveGeneric(LSS_CMD_SWITCH_SELECTIVE_REVISION, revisionNumber);
    return res;
}

//------------------------------------------------------------------------
// CiA DSP-305 v.2.2.17: 7.4.2 Switch Mode Selective
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: switchModeSelectiveSerialNumber(u32 serialNumber)
{
    canOpenStatus res = switchModeSelectiveSyncResponse(LSS_CMD_SWITCH_SELECTIVE_SERIAL, serialNumber);
    return res;
}

//------------------------------------------------------------------------
// CiA DSP-305 v.1.0: 3.10.1 Configure Node-ID Protocol
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: configureNodeId(u8 nodeId, u8 *errorCode, u8 *specificErrorCode)
{
    return this->configureGenericSyncResponse(LSS_CMD_CONFIGURE_NODE_ID, nodeId, 0, errorCode, specificErrorCode);
}

//------------------------------------------------------------------------
// CiA DSP-305 v.1.0: 3.10.2 Configure Bit Timing Parameters Protocol
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: configureBitTimingParamteres(u8 tableSelector, u8 tableIndex, u8 *errorCode, u8 *specificErrorCode)
{
    return this->configureGenericSyncResponse(LSS_CMD_CONFIGURE_TIMING_PARAMETERS, tableSelector, tableIndex, errorCode, specificErrorCode);
}

//------------------------------------------------------------------------
// CiA DSP-305 v.2.2.7: 7.5.3 Activate Bit Timing Parameters Protocol
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: activateBitTimingParameters(u16 switchDelay)
{
    canOpenStatus ret = CANOPEN_ERROR;
    if (can_interface != NULL)
    {
        u8 canData[8] = { LSS_CMD_ACTIVATE_TIMING_PARAMETERS, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

        setU16Val(switchDelay, canData, 1);

        ret = can_interface->canWrite(0x7e5, canData, 8, 0);
        if (ret != CANOPEN_OK)
        {
            DebugLogToFile("canWrite failed\n");
        }
    }
    return ret;
}

//------------------------------------------------------------------------
// CiA DSP-305 v.1.0: 3.10.4 Store Configuration Protocol
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: storeConfiguration(u8 *errorCode, u8 *specificErrorCode)
{
    return this->configureGenericSyncResponse(LSS_CMD_STORE_CONFIGURATION, 0, 0, errorCode, specificErrorCode);
}

//------------------------------------------------------------------------
// Generic configuration protocol helper.
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: configureGenericSyncResponse(u8 cs, u8 param1, u8 param2, u8 *errorCode, u8 *specificError)
{
    canOpenStatus res = CANOPEN_ERROR;

    if (this->wait_configure == true)
        return CANOPEN_INTERNAL_STATE_ERROR; // In use, waiting for response.

    unsigned long now = TimeClass::readTimer();
    unsigned long timeout = now + 3000;
    this->wait_configure = true;
    u8 canData[8] = {cs, param1, param2, 0x00, 0x00, 0x00, 0x00, 0x00};


    res = can_interface->canWrite(0x7e5, canData, 8, 0); 
    if (res != CANOPEN_OK)
    {
      DebugLogToFile("canWrite failed\n");
    }
    if (res == CANOPEN_OK)
    {
        while (this->wait_configure)
        {
            now = TimeClass::readTimer();
            if (timeout < now)
            {
                res = CANOPEN_TIMEOUT;
                break;
            }
        }
        if (this->wait_configure == false)
        {
            *errorCode = this->configure_error_code;
            *specificError = this->configure_specific_error;
            if (this->configure_error_code != (u8)0)
            {
                res = CANOPEN_ERROR;
            }
        }
    }
    return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void LSSMaster :: timerCallbackHandler(void *nmt_master_context)
{
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: canHardwareConnect(u8 port, u32 bitrate)
{
  return (this->canHardwareInit(port, 
            bitrate, 
            (DispatcherCanFuncPtr)canFrameConsumerW, 
            (ProtocolImplementationStateMachineFuncPtr)transferHelperW));
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: canFrameConsumerW(void *lss_master_object, 
  unsigned long id, unsigned char *data, unsigned int dlc, unsigned int flags)
{
  LSSMaster *lssMaster = (LSSMaster*) lss_master_object;
  return lssMaster->canFrameConsumer(id, data, dlc, flags);
}

//------------------------------------------------------------------------
// Wrapper for being able to setup callbacks to non-static funcs.
//------------------------------------------------------------------------
canOpenStatus LSSMaster :: transferHelperW(void *lss_master_object)
{
  canOpenStatus ret = CANOPEN_ERROR;
  LSSMaster *lssMaster = (LSSMaster*)lss_master_object;
  ret = lssMaster->transferHelper();
  return ret;
}

//------------------------------------------------------------------------
// Callback for processing CAN messages that has been received by the interface.
//------------------------------------------------------------------------
canOpenStatus  LSSMaster :: canFrameConsumer(unsigned long id, 
  unsigned char *data, unsigned int dlc, unsigned int flags)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (id == 0x7e4 && dlc == 8)
  {
      if (data[0] == LSS_CMD_SWITCH_SELECTIVE_CONFIRMATION) // Switch mode response.
      {
          this->wait_lss_mode = false;
          ret = CANOPEN_OK;
      }
      else if (data[0] == LSS_CMD_CONFIGURE_NODE_ID || // Configure node id.
          data[0] == LSS_CMD_CONFIGURE_TIMING_PARAMETERS || // Configure bit timing.
          data[0] == LSS_CMD_STORE_CONFIGURATION)   // Store configuration.
      {
          this->configure_error_code = data[1];
          this->configure_specific_error = data[2];
          this->wait_configure = false;
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
canOpenStatus  LSSMaster :: transferHelper(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  return ret;
}

