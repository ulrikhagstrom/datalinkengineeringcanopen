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

#include "LSSSlaveClass.h"
#include <stdio.h>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
LSSSlave :: LSSSlave (void)
{
  this->local_node_lss_callback = NULL;
  this->local_node_lss_activate_bit_timing_paramters = NULL;
  this->local_node_lss_configure_bit_timing_parameters = NULL;
  this->local_node_lss_configure_node_id_callback = NULL;
  this->local_node_lss_store_configuration = NULL;

  this->can_hardware_is_initiated = false;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
LSSSlave :: ~LSSSlave (void)
{
  (void)this->canHardwareDisconnect();
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  LSSSlave :: canHardwareConnect(u8 port, u32 bitrate)
{
  return CanConnection::canHardwareInit(port,
                                        bitrate,
                                        canFrameConsumerW,
                                        transferHelperW);
}

//------------------------------------------------------------------------
// Wrapper for being able to setup callbacks to non-static funcs.
//------------------------------------------------------------------------
canOpenStatus LSSSlave :: canFrameConsumerW(void *nmt_slave_context, 
  unsigned long id, unsigned char *data, unsigned int dlc, unsigned int flags)
{
  LSSSlave *nmt_slave = (LSSSlave*) nmt_slave_context;
  return nmt_slave->canFrameConsumer(id, data, dlc, flags);
}

//------------------------------------------------------------------------
// Wrapper for being able to setup callbacks to non-static funcs.
//------------------------------------------------------------------------
canOpenStatus LSSSlave :: transferHelperW(void *nmt_slave_context)
{
  canOpenStatus ret = CANOPEN_ERROR;
  LSSSlave *nmt_slave = (LSSSlave*)nmt_slave_context;
  ret = nmt_slave->transferHelper();
  return ret;
}

//------------------------------------------------------------------------
// Callback for processing CAN messages that has been received by the interface.
//------------------------------------------------------------------------
canOpenStatus  LSSSlave :: canFrameConsumer(unsigned long id, unsigned char *data, 
  unsigned int dlc, unsigned int flags)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (id == 0x7e5 && dlc == 8)
  {
      u8 cs = data[0];
      if (cs == 0x04)
      {
          this->lss_mode = data[1];
          if (this->local_node_lss_callback != NULL)
          {
            this->local_node_lss_callback(this->local_lss_callback_context, this->lss_mode);
          }
          ret = CANOPEN_OK;
      } 
      else if ((cs == 64 && this->vendor_id == ::getU32Val(data, 1)) ||
               (cs == 65 && this->product_code == ::getU32Val(data, 1)) ||
               (cs == 66 && this->revision_number == ::getU32Val(data, 1)) ||
               (cs == 67 && this->serial_number == ::getU32Val(data, 1)))
      {
          if (this->lss_mode == 0)
          {
              this->lss_mode = 1;
          }
          else
          {
              this->lss_mode = 0;
          }
          u8 canData[8] = { 68, this->lss_mode, 0, 0, 0, 0, 0, 0 };
          this->can_interface->canWrite(0x7e4, canData, 8, 0);
          if (this->local_node_lss_callback != NULL)
          {
            
            this->local_node_lss_callback(this->local_lss_callback_context, this->lss_mode);
          }
      }
      else if (cs == 17)
      {
          u8 errorCode = 0;
          u8 specError = 0;
          u8 nodeId = data[1];
          if (this->local_node_lss_configure_node_id_callback != NULL)
          {
              this->local_node_lss_configure_node_id_callback(this->local_node_lss_configure_node_id_callback_context, nodeId, &errorCode, &specError);
          }
          u8 canData[8] = { 17, errorCode, specError, 0, 0, 0, 0, 0 };
          this->can_interface->canWrite(0x7e4, canData, 8, 0);
      }
      else if (cs == 19)
      {
          u8 errorCode = 0;
          u8 specError = 0;
          u8 tableSelector = data[1];
          u8 tableIndex = data[2];

          if (this->local_node_lss_configure_bit_timing_parameters != NULL)
          {
              this->local_node_lss_configure_bit_timing_parameters(this->local_node_lss_configure_bit_timing_parameters_context, tableSelector, tableIndex, &errorCode, &specError);
          }
          u8 canData[8] = { 19, errorCode, specError, 0, 0, 0, 0, 0 };
          this->can_interface->canWrite(0x7e4, canData, 8, 0);
      }
      else if (cs == 21)
      {
          u8 errorCode = 0;
          u8 specError = 0;
          u16 switchDelay = ::buf2val(&data[1], 2);

          if (this->local_node_lss_activate_bit_timing_paramters != NULL)
          {
              this->local_node_lss_activate_bit_timing_paramters(this->local_node_lss_activate_bit_timing_paramters, switchDelay, &errorCode, &specError);
          }
          u8 canData[8] = { 21, errorCode, specError, 0, 0, 0, 0, 0 };
          this->can_interface->canWrite(0x7e4, canData, 8, 0);
      }
      else if (cs == 23)
      {
          u8 errorCode = 0;
          u8 specError = 0;
          if (this->local_node_lss_store_configuration != NULL)
          {
              this->local_node_lss_store_configuration(this->local_node_lss_store_configuration_context, &errorCode, &specError);
          }
          u8 canData[8] = { 23, errorCode, specError, 0, 0, 0, 0, 0 };
          this->can_interface->canWrite(0x7e4, canData, 8, 0);	  }
  }


  return ret;
}

//------------------------------------------------------------------------
// Callback for processing CAN messages that has been received by the interface.
//------------------------------------------------------------------------
canOpenStatus  LSSSlave :: transferHelper(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  LSSSlave :: registerLocalNodeStateChangeCallback(
  LSSReqestFunPtr state_change_callback_function, void *context)
{
  canOpenStatus ret = CANOPEN_ERROR;
  this->local_node_lss_callback = state_change_callback_function;
  this->local_lss_callback_context = context;
  ret = CANOPEN_OK;
  return ret;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------

void LSSSlave :: setDeviceParameters(u32 vendorId, u32 productCode, u32 revisionNumber, u32 serialNumber)
{
    this->vendor_id = vendorId;
    this->product_code = productCode;
    this->revision_number = revisionNumber;
    this->serial_number = serialNumber;
}