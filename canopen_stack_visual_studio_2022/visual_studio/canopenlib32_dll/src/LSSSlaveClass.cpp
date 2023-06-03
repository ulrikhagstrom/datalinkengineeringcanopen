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
      if (cs == LSS_CMD_SWITCH_STATE_GLOBAL)
      {
          this->lss_mode = data[1];
          if (this->local_node_lss_callback != NULL)
          {
            this->local_node_lss_callback(this->local_lss_callback_context, this->lss_mode);
          }
          ret = CANOPEN_OK;
      } 
      else if (cs == LSS_CMD_SWITCH_SELECTIVE_VENDOR)
      {
          if (this->vendor_id == ::getU32Val(data, 1))
          {
              this->lss_selective_state = cs; // Start selective state approval
          }
          else
          {
              this->lss_selective_state = 0; // Not this LSS slave
              this->lss_mode = LSS_MODE_WAITING_STATE; // LSS waiting state
          }
      }
      else if (cs == LSS_CMD_SWITCH_SELECTIVE_PRODUCT)
      { 
          // Vendor approved, and still this LSS Slave
          if (this->lss_selective_state == LSS_CMD_SWITCH_SELECTIVE_VENDOR
              && this->product_code == ::getU32Val(data, 1))
          {
              this->lss_selective_state = cs; // continue selective state approval
          }
          else
          {
              this->lss_selective_state = 0; // Not this LSS slave
              this->lss_mode = LSS_MODE_WAITING_STATE; // LSS waiting state
          }
      }
      else if (cs == LSS_CMD_SWITCH_SELECTIVE_REVISION)
      { 
          // Product approved, and still this LSS Slave
          if (this->lss_selective_state == LSS_CMD_SWITCH_SELECTIVE_PRODUCT && this->revision_number == ::getU32Val(data, 1))
          {
              this->lss_selective_state = cs; // continue selective state approval
          }
          else
          {
              this->lss_selective_state = 0; // Not this LSS slave
              this->lss_mode = LSS_MODE_WAITING_STATE;
          }
      }
      else if (cs == LSS_CMD_SWITCH_SELECTIVE_SERIAL)
      {
          // Revision approved, and still this LSS Slave
          if (this->lss_selective_state == LSS_CMD_SWITCH_SELECTIVE_REVISION && this->serial_number == ::getU32Val(data, 1))
          {
              this->lss_selective_state = cs;
              if (this->lss_mode == LSS_MODE_WAITING_STATE)
              {
                  this->lss_mode = LSS_MODE_CONFIGURATION_STATE;
              }
              else
              {
                  this->lss_mode = LSS_MODE_WAITING_STATE;
              }
              u8 canData[8] = { LSS_CMD_SWITCH_SELECTIVE_CONFIRMATION, 0, 0, 0, 0, 0, 0, 0 };
              ret = this->can_interface->canWrite(0x7e4, canData, 8, 0);
              if (this->local_node_lss_callback != NULL)
              {

                  this->local_node_lss_callback(this->local_lss_callback_context, this->lss_mode);
              }
          }
          else
          {
              this->lss_selective_state = 0; // Not this LSS slave
              this->lss_mode = LSS_MODE_WAITING_STATE; // LSS waiting state
          }
      }
      else if (cs == LSS_CMD_CONFIGURE_NODE_ID && this->lss_mode == LSS_MODE_CONFIGURATION_STATE)
      {
          u8 errorCode = 0;
          u8 specError = 0;
          u8 nodeId = data[1];
          if (this->local_node_lss_configure_node_id_callback != NULL)
          {
              this->local_node_lss_configure_node_id_callback(this->local_node_lss_configure_node_id_callback_context, nodeId, &errorCode, &specError);
          }
          
          if (errorCode == 0)
          {
              bool isValidPendingNodeId = (nodeId > 0 && nodeId <= 0x7F) || nodeId == 0xFF;
              if (!isValidPendingNodeId)
                  errorCode = 1; // Node ID out of range
              else
                  this->pendingNodeId = nodeId;
          }

          u8 canData[8] = { LSS_CMD_CONFIGURE_NODE_ID, errorCode, specError, 0, 0, 0, 0, 0 };
          ret = this->can_interface->canWrite(0x7e4, canData, 8, 0);
      }
      else if (cs == LSS_CMD_CONFIGURE_TIMING_PARAMETERS && this->lss_mode == LSS_MODE_CONFIGURATION_STATE)
      {
          u8 errorCode = 0;
          u8 specError = 0;
          u8 tableSelector = data[1];
          u8 tableIndex = data[2];

          if (this->local_node_lss_configure_bit_timing_parameters != NULL)
          {
              this->local_node_lss_configure_bit_timing_parameters(this->local_node_lss_configure_bit_timing_parameters_context, tableSelector, tableIndex, &errorCode, &specError);
          }
          if (errorCode == 0)
          {
              if (tableSelector != 0)
              {
                  // We can not yet ask the this->can_interface for custom table bit rate translation to real bit rate
                  errorCode = LSS_TIMING_ERROR_UNSUPPORTED_BITRATE;
              }
              else
              {
                  switch (tableIndex)
                  {
                  case LSS_TIMING_TABLE0_1000K: // CiA 301 estimated bus length 25m
                      this->pendingBitRate = 1000000;
                      break;
                  case LSS_TIMING_TABLE0_800K: // CiA 301 estimated bus length 50m
                      this->pendingBitRate = 800000;
                      break;
                  case LSS_TIMING_TABLE0_500K: // CiA 301 estimated bus length 100m
                      this->pendingBitRate = 500000;
                      break;
                  case LSS_TIMING_TABLE0_250K: // CiA 301 estimated bus length 250m
                      this->pendingBitRate = 250000;
                      break;
                  case LSS_TIMING_TABLE0_125K: // CiA 301 estimated bus length 500m
                      this->pendingBitRate = 125000;
                      break;
                  case LSS_TIMING_TABLE0_50K: // CiA 301 estimated bus length 1000m
                      this->pendingBitRate = 50000;
                      break;
                  case LSS_TIMING_TABLE0_20K: // CiA 301 estimated bus length 2500m
                      this->pendingBitRate = 20000;
                      break;
                  case LSS_TIMING_TABLE0_10K: // CiA 301 estimated bus length 5000m
                      this->pendingBitRate = 10000;
                      break;
                  default:
                      errorCode = LSS_TIMING_ERROR_UNSUPPORTED_BITRATE;
                  }
              }
              this->pendingBitRateTableSelector = tableSelector;
              this->pendingBitRateTableIndex = tableIndex;
          }
          this->bitRateError = errorCode;
          u8 canData[8] = { LSS_CMD_CONFIGURE_TIMING_PARAMETERS, errorCode, specError, 0, 0, 0, 0, 0 };
          ret = this->can_interface->canWrite(0x7e4, canData, 8, 0);
          if (errorCode == LSS_TIMING_ERROR_UNSUPPORTED_BITRATE)
          {
              ret = CANOPEN_UNSUPPORTED_BITRATE;
          }
      }
      else if (cs == LSS_CMD_ACTIVATE_TIMING_PARAMETERS && this->lss_mode == LSS_MODE_CONFIGURATION_STATE)
      {
          u16 switchDelay = ::buf2val(&data[1], 2);

          if (this->local_node_lss_activate_bit_timing_paramters != NULL)
          {
              this->local_node_lss_activate_bit_timing_paramters(this->local_node_lss_activate_bit_timing_paramters, switchDelay);
          }
          // No feedback, 
          // Flag stop communicating, so all SDO PDO.. start to stop traffic.
          // Wait switchDelay for completing any queued communication and for all SDO PDO.. to stop
          // Copy pending bit rate to active bit rate and tell all / HW  to communicate with new rate
          if (this->bitRateError == LSS_TIMING_ERROR_OK)
          {
              canOpenStatus retSetBitRateStatus = this->can_interface->canSetBitrate(this->bitRate);
              if (retSetBitRateStatus == CANOPEN_OK)
              {
                  this->bitRate = this->pendingBitRate;
                  this->bitRateTableSelector = this->pendingBitRateTableSelector;
                  this->bitRateTableIndex = this->pendingBitRateTableIndex;
              }
          }
          // Flag waiting to start at new speed
          // wait switchDelay before starting SDO PDO... for all devices at new speed.
          // Flag / notify SDO PDO.. to start communication again at new speed.
      }
      else if (cs == LSS_CMD_STORE_CONFIGURATION && this->lss_mode == LSS_MODE_CONFIGURATION_STATE)
      {
          u8 errorCode = 0;
          u8 specError = 0;
          if (this->local_node_lss_store_configuration != NULL)
          {
              // It is the handlers responsibility to permanently store pendingNodeId and pendingBitrateTableSelector/index or set error code
              this->local_node_lss_store_configuration(this->local_node_lss_store_configuration_context, &errorCode, &specError);
          }
          // store 
          u8 canData[8] = { LSS_CMD_STORE_CONFIGURATION, errorCode, specError, 0, 0, 0, 0, 0 };
          this->can_interface->canWrite(0x7e4, canData, 8, 0);	  }
  }
  else if (id == 0x0 && dlc == 2) // NMT
  {
      // if NMT Reset application, get permanent node id and bit rate to pending node id bit rate
      // if NMT Reset communication for active node, then copy pending to active node id
      if (data[0] == 130 && (data[1] == this->activeNodeId || data[1] == 0))
      {
          if (this->pendingNodeId > 0 && this->pendingNodeId <= 0x7F)
          {
              this->activeNodeId = this->pendingNodeId;
              // notify that the active node id is set / or notify that the active node id changed if different.
          }
      }
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