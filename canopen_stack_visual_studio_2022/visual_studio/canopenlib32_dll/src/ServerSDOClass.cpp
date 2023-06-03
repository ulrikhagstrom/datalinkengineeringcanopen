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

#include "ServerSDOClass.h"
#include "CanInterfaceClass.h"

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
ServerSDO :: ServerSDO()
{
  if (this->registerPeriodicTimerCallback() == CANOPEN_OK)
  {
    this->state = SRV_WAIT_ANY_CMD;
  }
  else
  {
    this->state = SRV_INTERNAL_ERROR;
  }
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
ServerSDO :: ~ServerSDO()
{ 
  (void)SDO::unregisterPeriodicTimerCallback();
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus ServerSDO :: registerPeriodicTimerCallback(void)
{
  return SDO::registerPeriodicTimerCallback( 
    (TimeClass::TimeHandlerFuncPtr)periodicTimerCallbackWrapper, 
    this, CALLBACK_PERIOD_TIME);
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: nodeSetId(u8 _node_id)
{
  canOpenStatus ret = CANOPEN_ERROR;  
  if (_node_id < 128)
  {
    this->node_id = _node_id;
    this->cobid_rx = 0x600 + node_id;
    this->cobid_tx = 0x580 + node_id;
    ret = CANOPEN_OK;
  }
  else
  {
    this->node_id = INVALID_NODE_ID;
  }
  return ret;
}

//------------------------------------------------------------------------
// Collect client command specifier.
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: getCliCmd(u8 *cmd)
{
  *cmd = (this->can_data_rx[0] >> 5) & 7;
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
//9.2.2.2.2 Initiate SDO Download Protocol
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: getCliCmdSpcInitDnReq(u8 *n, u8 *e, u8 *s)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 flags = this->can_data_rx[0]; 
  u8 ccs = (flags >> 5) & 7;
  if (ccs == 1)
  {
    *n = (flags >> 2) & 3;
    *e = (flags >> 1) & 1;
    *s = (flags >> 0) & 1;
    ret = CANOPEN_OK;
  }
  else
  {
    ret = CANOPEN_UNEXPECTED_CCS;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.3 Download SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: getCliCmdSpcDnSegReq(u8 *t, u8 *n, u8 *c)  
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 flags = this->can_data_rx[0]; 
  u8 ccs = (flags >> 5) & 7;
  if (ccs == 0)
  {
    *t = (flags >> 4) & 1;
    *n = (flags >> 1) & 7;
    *c = (flags >> 0) & 1;
    ret = CANOPEN_OK;
  }
  else
  {
    ret = CANOPEN_UNEXPECTED_CCS;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.5 Initiate SDO Upload Protocol 
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: getCliCmdSpcInitUpReq(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 flags = this->can_data_rx[0]; 
  u8 ccs = (flags >> 5) & 7;
  if (ccs == 2)
  {
    ret = CANOPEN_OK;
  }
  else
  {
    ret = CANOPEN_UNEXPECTED_CCS;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.6 Upload SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: getCliCmdSpcUpSegReq(u8 *t)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 flags = this->can_data_rx[0]; 
  u8 ccs = (flags >> 5) & 7;
  if (ccs == 3)
  {
    *t = (flags >> 4) & 1;
    ret = CANOPEN_OK;
  }
  else
  {
    ret = CANOPEN_UNEXPECTED_CCS;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.7 Abort SDO Transfer Protocol
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: getCliCmdSpcAbrtTrnsf(void)
{
  canOpenStatus ret = CANOPEN_ERROR;

  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.9 Initiate SDO Block Download Protocol
//------------------------------------------------------------------------
canOpenStatus   ServerSDO ::  getCliCmdSpcBlkDn(u8 *cc, u8 *s, u8 *cs)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 flags = this->can_data_rx[0]; 
  u8 ccs = (flags >> 5) & 7;
  if (ccs == 6)
  {
    *cc = (flags >> 2) & 1;
    *s = (flags >> 1) & 7;
    *cs = (flags >> 0) & 1;
    ret = CANOPEN_OK;
  }
  else
  {
    ret = CANOPEN_UNEXPECTED_CCS;
  }
  return ret;

}

//------------------------------------------------------------------------
// 9.2.2.2.11 End SDO Block Download Protocol
//------------------------------------------------------------------------
canOpenStatus  ServerSDO ::  getCliCmdSpcEndBlkDnReq(u8 *n, u16 *crc)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 flags = this->can_data_rx[0];
  u8 ccs = (flags >> 5) & 7;
  u8 cs = flags & 1;
  if (ccs == 6 && cs == 1)
  {
    *n = ((flags >> 2) & 7);
    *crc = ::getU16Val(this->can_data_rx, 1);
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.2 Initiate SDO Download Protocol
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: setSrvCmdSpcInitDnResp(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  this->can_data_tx[0] = (3 << 5); // scs == 3
  ret = CANOPEN_OK;
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.3 Download SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: setSrvCmdSpcDnSegResp(u8 t)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (t <= 1)
  {
    u8 flags               = 0;
    flags                  = (1 << 5);  // scs == 1
    flags                 |= (t << 4);
    this->can_data_tx[0]   = flags;
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.9 Initiate SDO Block Download Protocol
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: setSrvCmdSpcInitBlockDnResp(u8 sc)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (sc <= 1)
  {
    u8 flags               = 0;
    flags                  = (5 << 5);  // scs == 1
    flags                 |= (sc << 2);
    flags                 |= (0 << 0);  // initiate download response (server subcommand)  
    this->can_data_tx[0]   = flags;
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.5 Initiate SDO Upload Protocol
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: setSrvCmdSpcInitUpResp(u8 valid, u8 e, u8 s)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 n = (4 -  valid);
  if ((n <= 3) && (e <= 1) && (s <= 1))
  {
    u8 flags   = (2 << 5); // scs == 2.
    flags     |= (n << 2);
    flags     |= (e << 1);
    flags     |= (s << 0);
    this->can_data_tx[0] = flags;
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.6 Upload SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: setSrvCmdSpcUpSegResp(u8 t, u8 n, u8 c)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if ((t <= 1) && (n <= 7) && (c <= 1))
  {
    u8 flags   = (0 << 5); // scs == 0.
    flags     |= (t << 4);
    flags     |= (n << 1);
    flags     |= (c << 0);
    this->can_data_tx[0] = flags;
    ret = CANOPEN_OK;
  }
  else
  {
    ret = CANOPEN_ARG_ERROR;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.10 Upload SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: setSrvCmdSpcBlkDnResp(u8 ss, u8 ackseq, u8 blksize)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (ss <= 3)
  {
    u8 flags   =  (5 << 5); // scs == 5, block download.
    flags     |= (ss << 0); // ss == 2, server subcommand.
    this->can_data_tx[0] = flags;
    ::setU8Val(ackseq, this->can_data_tx, 1);
    ::setU8Val(blksize, this->can_data_tx, 2);
    ret = this->clrTxCanDataField(3, 7); // Reserved space == 0.
  }
  else
  {
    ret = CANOPEN_ARG_ERROR;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.11 End SDO Block Download Protocol
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: setSrvCmdSpcEndBlkDnResp(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 flags = 0;
  flags  = (5 << 5); // scs == 5.
  flags |= (1 << 0); // ss == 1;
  ::setU8Val(flags, this->can_data_tx, 0);
  ret = clrTxCanDataField(1, 7); // reserved space == 0.
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus ServerSDO :: periodicTimerCallbackWrapper(void *context)
{
  ServerSDO *srvSDO = (ServerSDO*) context;
  return srvSDO->periodicTimerCallback();
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus ServerSDO ::periodicTimerCallback(void) 
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (this->state != SRV_WAIT_ANY_CMD)
  {
    if ((TimeClass::readTimer() - 
         this->getLatestEventTimestamp()) > ONGOING_TRANSFER_INACTIVITY_TIMEOUT)
    {
      ret = this->setCliSrvAbortTransfer(this->application_object_index, 
        this->application_sub_index, SDO_PROTOCOL_TIMED_OUT);
      if (ret == CANOPEN_OK)
      {
        ret = this->writeToCanBus();
      }
      this->state = SRV_WAIT_ANY_CMD;
    }
  }
  return ret;
}

//------------------------------------------------------------------------
// Wrapper for being able to setup callbacks to non-static funcs.
//------------------------------------------------------------------------
canOpenStatus ServerSDO :: canFrameConsumerW(void *context, unsigned long id, 
                                             unsigned char *data, 
                                             unsigned int dlc, unsigned int flags)
{
  canOpenStatus ret = CANOPEN_ERROR;
  ServerSDO *srvSDO = (ServerSDO*) context;
  ret = srvSDO->setLatestEventTimestamp();
  if (ret == CANOPEN_OK)
  {
    ret = srvSDO->canFrameConsumer(id, data, dlc, flags);
  }
  return ret;
}

//------------------------------------------------------------------------
// Wrapper for being able to setup callbacks to non-static funcs.
//------------------------------------------------------------------------
canOpenStatus ServerSDO :: serverSDOStateMachineW(void *client_sdo_object)
{
  //qqq, TBD.
  return CANOPEN_ERROR;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: serverSDOStateMachine(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  ;
  return ret;
}

//------------------------------------------------------------------------
// Callback handler for Server
//------------------------------------------------------------------------
canOpenStatus  ServerSDO :: canFrameConsumer(unsigned long id, 
  unsigned char *data, unsigned int dlc, unsigned int flags)
{
  canOpenStatus ret = CANOPEN_ERROR;

  // Copy the contents of the received CAN message to the buffers held by the object.
  ret = this->getFrameData(id, data, dlc);

  if (ret == CANOPEN_OK)
  {
    switch (this->state)
    {
      case SRV_WAIT_ANY_CMD:
        ret = hndlCliRequest();
        break;
      case SRV_WAIT_DOWNLOAD_SEGMENT_REQUEST:
        ret = hndlDlSegReq();
        break;
      case SRV_WAIT_UPLOAD_SEGMENT_REQUEST:
        ret = hndlUpSegReq();
        break;
      case SRV_WAIT_DOWNLOAD_BLOCK:
        ret = hndlDlBlkSegReq();  //9.2.2.2.10 Download SDO Block Segment Protocol
        break; 
      case SRV_WAIT_END_BLOCK_DOWNLOAD:
        ret = hndlEndBlkDnReq();  //9.2.2.2.11 End SDO Block Download Protocol
        break; 
      default:
        ret = CANOPEN_INTERNAL_STATE_ERROR;
        break;
    }
    if (ret == CANOPEN_CMD_SPEC_UNKNOWN_OR_INVALLD)
    {
      ret = SDO :: hndlInvalidCmdSpec(); // A invalid or unknown command specifier, surender.
      this->state = SRV_WAIT_ANY_CMD;
    }
  }
  return ret;
}

//------------------------------------------------------------------------
//  This method initiates the SDO on the network.
//------------------------------------------------------------------------
canOpenStatus   ServerSDO :: canHardwareConnect(u8 port, u32 bitrate) 
{
  return SDO :: canHardwareInit(
    port, 
    bitrate, 
    (DispatcherCanFuncPtr)canFrameConsumerW, 
    (ProtocolImplementationStateMachineFuncPtr)serverSDOStateMachineW);
}


//------------------------------------------------------------------------
// This method registers the callback to be used for read object callbacks.
//------------------------------------------------------------------------
canOpenStatus   ServerSDO :: registerObjectReadCallback(SrvReadFuncPtr fp, 
                                                        void *context)
{
  this->object_read_callback = fp;
  this->object_read_callback_context = context;
  return CANOPEN_OK;
}
  
//------------------------------------------------------------------------
// This method registers the callback to be used for write object callbacks.
//------------------------------------------------------------------------
canOpenStatus   ServerSDO :: registerObjectWriteCallback( SrvWriteFuncPtr callback, 
                                                          void *context)
{
  this->object_write_callback = callback;
  this->object_write_callback_context = context;
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus   ServerSDO :: hndlCliRequest(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 cliCmd = 0;
  ret = getCliCmd(&cliCmd);
  if (ret == CANOPEN_OK)
  {
    switch (cliCmd)
    {
      case CLI_CMD_INIT_DL_REQ:
        ret = hndlInitDlReq();
        break;
      case CLI_CMD_DL_SEG_REQ:
        ret = hndlDlSegReq();
        break;
      case CLI_CMD_INIT_UL_REQ:
        ret = hndlInitUpReq();
        break;
      case CLI_CMD_UL_SEG_REQ:
        ret = hndlUpSegReq();
        break;
      case CLI_CMD_BLOCK_DL_REQ:
        ret = hndlBlkDl();  // CSS is set to 6 for "initiate download request" and "end block download request".
        break;
      default:
        ret = CANOPEN_CMD_SPEC_UNKNOWN_OR_INVALLD;
        break;
    }
  }
  return ret;
}


//------------------------------------------------------------------------
//9.2.2.2.2 Initiate SDO Download Protocol
//------------------------------------------------------------------------
canOpenStatus   ServerSDO :: hndlInitDlReq(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 n = 0;
  u8 e = 0;
  u8 s = 0;
  ret = getCliCmdSpcInitDnReq(&n, &e, &s);
  if (ret == CANOPEN_OK)
  {
    u16 object_index;
    u8  sub_index;
    ret = getMultiplexor(&object_index, &sub_index);
    if (ret == CANOPEN_OK)
    {
      u16 flags;
      this->application_object_index = object_index;
      this->application_sub_index = sub_index;
      ret = object_attributes_callback(object_attributes_callback_context, 
        object_index, sub_index, &flags);
      if (ret == CANOPEN_OK)
      {
        if (flags & OBJ_WRITEABLE)
        {
          u32 data;
          u8 valid;
          ret = getData(&data, &valid, n);
          if (ret == CANOPEN_OK)
          {
            if (e == 1)
            {
              //It's expedited transfer, just pass the data to the application.
              u32 canopen_error_code = 0;
              u8 buffer_data[4];  //Max 4 bytes.
              ::val2buf(data, buffer_data, valid);
              ret = this->object_write_callback(object_write_callback_context, 
                object_index, sub_index, buffer_data, valid, &canopen_error_code);
              if (ret == CANOPEN_OK)
              {
                // Send the "Initiate Download Response" frame.
                ret = setSrvCmdSpcInitDnResp();
                if (ret == CANOPEN_OK)
                {
                  ret = setMultiplexor(this->application_object_index, 
                    this->application_sub_index);
                  if (ret == CANOPEN_OK)
                  {
                    ret = setData(0, 4, NULL);  //Clear the reserved data field.
                    if (ret == CANOPEN_OK)
                    {
                      ret = this->writeToCanBus();
                      if (ret == CANOPEN_OK)
                      {
                        this->state = SRV_WAIT_ANY_CMD; // The transfer is finished.
                      }
                    }
                  }
                }
              }
              else
              {
                u32 canopen_error_code_to_client = 0;
                if (ret == CANOPEN_ERR_DEFINED_BY_APPL && canopen_error_code != 0)
                {
                  canopen_error_code_to_client = canopen_error_code;
                }
                else
                {
                  canopen_error_code_to_client = DATA_CAN_NOT_BE_STORED; // Error defined by CANopen spec.
                }
                ret = this->setCliSrvAbortTransfer(this->application_object_index, 
                  this->application_sub_index, canopen_error_code_to_client);
                if (ret == CANOPEN_OK)
                {
                  ret = this->writeToCanBus();
                }
              }
            }
            else  
            {
              // It's not expedited.
              if (s == 1)
              {
                if(data < SDO_SERVER_BUFFER)
                {
                  ret = setSrvCmdSpcInitDnResp();
                  if (ret == CANOPEN_OK)
                  {
                    ret = setMultiplexor(this->application_object_index, 
                      this->application_sub_index);
                    if (ret == CANOPEN_OK)
                    {
                      u8 n;
                      ret = setData(0, 4, &n); //reserved, according to spec - always 0.
                      if (ret == CANOPEN_OK)
                      {
                        state = SRV_WAIT_DOWNLOAD_SEGMENT_REQUEST;
                        this->data_buffer_counter = 0;
                        this->t = 0;
                        ret = this->writeToCanBus();
                      }
                    }
                  }                  
                }
                else
                {
                  ret = this->setCliSrvAbortTransfer(this->application_object_index, 
                    this->application_sub_index, OUT_OF_MEMORY);
                  if (ret == CANOPEN_OK)
                  {
                    ret = this->writeToCanBus();
                  }
                  state = SRV_WAIT_ANY_CMD;
                }
              }
            }
          }
        }
        //else
        //{
        //  this->state = state = SRV_WAIT_ANY_CMD;
        //  this->setCliSrvAbortTransfer(this->application_object_index, 
        //          this->application_sub_index, ATTEMPT_TO_WRITE_A_READ_ONLY_OBJ);
        //  this->writeToCanBus();
        //}
        else
        {
          // Write object properties where missing.
          u32 err = 0;
          if (flags & OBJ_READABLE)
          {
            err = ATTEMPT_TO_WRITE_A_READ_ONLY_OBJ;
          }
          else
          {
            err = UNSUPPORTED_ACCESS_TO_AN_OBJECT;
          }
          ret = this->setCliSrvAbortTransfer(this->application_object_index, 
            this->application_sub_index, 
            err);
          if (ret == CANOPEN_OK)
          {
            ret = this->writeToCanBus();
            if (ret == CANOPEN_OK)
            {
              this->state = SRV_WAIT_ANY_CMD;
            }
          }
        }
      }
    }
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.3 Download SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus   ServerSDO :: hndlDlSegReq(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 t = 0;
  u8 n = 0;
  u8 c = 0;

  ret = getCliCmdSpcDnSegReq(&t, &n, &c);
  if (ret == CANOPEN_OK)
  {
    if (this->t == t)
    {
      u8 valid = 0;
      u8 segmData[7];
      ret = getSegData(segmData, &valid, n);
      if (ret == CANOPEN_OK)
      {
        u32 canopen_error_code ;
        if ((this->data_buffer_counter + valid) < SDO_SERVER_BUFFER)
        {
          memcpy(&this->data_buffer[this->data_buffer_counter], segmData, valid);
          this->data_buffer_counter += valid;
          if (c == 1) // no more segments to be downloaded
          {
            ret = this->object_write_callback(object_write_callback_context, 
              this->application_object_index, 
              this->application_sub_index, 
              data_buffer, 
              this->data_buffer_counter, 
              &canopen_error_code );
          }
          if (ret == CANOPEN_OK)
          {
            ret = setSrvCmdSpcDnSegResp(this->t);
            if (ret == CANOPEN_OK)
            {
              ret = this->clrTxCanDataField(1, 7); // Clear the reserved data-bytes of the frame.
              if (ret == CANOPEN_OK)
              {
                if (c == 0) // more segments to be downloaded
                {
                  this->toggle();
                }
                else
                {
                  this->state = SRV_WAIT_ANY_CMD;
                }
                ret = this->writeToCanBus();
              }
            }
          }
        }
        else 
        {
          // Server SDO ran out of buffer.
          ret = this->setCliSrvAbortTransfer(this->application_object_index, 
            this->application_sub_index, CANOPEN_OUT_OF_MEM); 
          if (ret == CANOPEN_OK)
          {
            ret = this->writeToCanBus();
            this->state = SRV_WAIT_ANY_CMD;
          }
        }
      }
    }
    else
    {
      // Toggle miss-match.
      ret = this->setCliSrvAbortTransfer(this->application_object_index, 
        this->application_sub_index, TOGGLE_BIT_NOT_ALTERED);
      if (ret == CANOPEN_OK)
      {
        ret = this->writeToCanBus();
        this->state = SRV_WAIT_ANY_CMD;
      }
    }
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.10 Download SDO Block Segment Protocol
//------------------------------------------------------------------------
canOpenStatus   ServerSDO :: hndlDlBlkSegReq(void)
{
  static int generatefault = 1;
  canOpenStatus ret = CANOPEN_ERROR;
  u8 c                = 0;
  u8 sequence_number  = 0;
  ret = this->getDlBlkSegHeader(&c, &sequence_number);
  if (ret == CANOPEN_OK)
  {
    // qqq This is TESTCODE for generating errors.
    /*
    if (sequence_number == 3 & generatefault == 1)
    {
      sequence_number = 4;
      generatefault = 0;
    }
    */

    // qqq remove this END!!
    if (block_transfer_segment_count == sequence_number)
    {
      u8 segmData[7];
      u8 valid;
      ret = getSegData(segmData, &valid, 0);
      if (ret == CANOPEN_OK)
      {
        if ((this->data_buffer_counter + valid) < SDO_SERVER_BUFFER)
        {
          memcpy(&this->data_buffer[this->data_buffer_counter], segmData, valid);
          this->data_buffer_counter += valid;
          if ((c == 1) || (sequence_number >= MAX_SRV_BLOCK_SEGM )) //c = 1 gives: more segments to be downloaded.
          {
            ret = setSrvCmdSpcBlkDnResp(2, sequence_number, MAX_SRV_BLOCK_SEGM);
            if (ret == CANOPEN_OK)
            {
              if (c == 1)
              {
                this->state = SRV_WAIT_END_BLOCK_DOWNLOAD;
              }
              else
              {
                block_transfer_segment_count = 1;
              }
              if (ret == CANOPEN_OK)
              {
                ret = this->writeToCanBus();
              }
            }
          }
          else 
          {
            block_transfer_segment_count++;
          }
        }
      }
    }
    else
    {
      // Segment counter missmatch.
      if (block_transfer_segment_count > 0)
      {
        ret = setSrvCmdSpcBlkDnResp(2, 
          (block_transfer_segment_count - 1), 
          MAX_SRV_BLOCK_SEGM); // qqq: Should trigger re-transmission in the client.
        if (ret == CANOPEN_OK)
        {
          ret = this->writeToCanBus();
        }
      }
      else
      {
        ret = CANOPEN_ERROR; //qqq: Add abort transfer here, should never happen.
      }
    }
  }
  return ret;
}

//------------------------------------------------------------------------
// 9.2.2.2.11 End SDO Block Download Protocol
//------------------------------------------------------------------------
canOpenStatus   ServerSDO :: hndlEndBlkDnReq(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 n = 0;
  u16 crc = 0;
  u32 canopen_error_code = 0;

  ret = getCliCmdSpcEndBlkDnReq(&n, &crc);
  if (ret == CANOPEN_OK)
  {
    // qqq, decrement the number of received bytes in the last received frame due 
    // to that n is being told only in the end block transfer request.
    this->data_buffer_counter -= n;
    ret = this->object_write_callback(object_write_callback_context, 
      this->application_object_index, 
      this->application_sub_index, 
      data_buffer, 
      this->data_buffer_counter, 
      &canopen_error_code);
    if (state == SRV_WAIT_END_BLOCK_DOWNLOAD)
    {
      if (ret == CANOPEN_OK)
      {
        ret = this->setSrvCmdSpcEndBlkDnResp();
        if (ret == CANOPEN_OK)
        {
          this->state = SRV_WAIT_ANY_CMD;
          ret = this->writeToCanBus();
        }
      }
      else
      {
        u32 canopen_error_code_to_client = 0;
        if (ret == CANOPEN_ERR_DEFINED_BY_APPL && canopen_error_code != 0)
        {
          canopen_error_code_to_client = canopen_error_code;
        }
        else
        {
          canopen_error_code_to_client = DATA_CAN_NOT_BE_STORED; // Error defined by CANopen spec.
        }
        ret = this->setCliSrvAbortTransfer(this->application_object_index, 
          this->application_sub_index, 
          canopen_error_code_to_client);
        if (ret == CANOPEN_OK)
        {
          this->state = SRV_WAIT_ANY_CMD;
          ret = this->writeToCanBus();
        }
      }
    }
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.9 Initiate SDO Block Download Protocol
//------------------------------------------------------------------------
canOpenStatus   ServerSDO :: hndlBlkDl(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 cc = 0;
  u8 s = 0;
  u8 cs = 0;

  ret = getCliCmdSpcBlkDn(&cc, &s, &cs);
  if (ret == CANOPEN_OK)
  {
    if (cs == 0)  // Initiate block download request.
    {
      u16 object_index;
      u8  sub_index;
      ret = getMultiplexor(&object_index, &sub_index);
      if (ret == CANOPEN_OK)
      {
        u32 objSize;
        this->application_object_index = object_index;
        this->application_sub_index = sub_index;
        ret = getData(&objSize, NULL, 0);
        if (ret == CANOPEN_OK)
        {
          if(objSize < SDO_SERVER_BUFFER)
          {
            ret = setSrvCmdSpcInitBlockDnResp(0); // No CRC-check support.
            if (ret == CANOPEN_OK)
            {
              ret = setMultiplexor(this->application_object_index, 
                this->application_sub_index);
              if (ret == CANOPEN_OK)
              {
                ret = setData(MAX_SRV_BLOCK_SEGM, 1, NULL); // 127 block.
                if (ret == CANOPEN_OK)
                {
                  ret = clrTxCanDataField(5, 7);
                  if (ret == CANOPEN_OK)
                  {
                    state = SRV_WAIT_DOWNLOAD_BLOCK;
                    this->data_buffer_counter = 0;
                    this->block_transfer_segment_count = 1;
                    ret = this->writeToCanBus();
                  }
                }
              }
            }
          }
        }
      }  
    }
    else // End block download request
    {
      
    }
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.5 Initiate SDO Upload Protocol
//------------------------------------------------------------------------
canOpenStatus   ServerSDO :: hndlInitUpReq(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 n = 0;
  u8 e = 0;
  u8 s = 0;

  ret = getCliCmdSpcInitUpReq();
  if (ret == CANOPEN_OK)
  {
    u16 object_index;
    u8 sub_index;
    ret = getMultiplexor(&object_index, &sub_index);
    if (ret == CANOPEN_OK)
    {
      u16 flags = 0;
      this->application_object_index = object_index;
      this->application_sub_index = sub_index;
      ret = object_attributes_callback(object_attributes_callback_context, 
        object_index, sub_index, &flags);
      if (ret == CANOPEN_OK)
      {
        if (flags & OBJ_READABLE)
        {
          u32 coErrorCode = 0;
          u32 application_data_size;
          ret = this->object_read_callback(object_read_callback_context, 
            object_index, sub_index, data_buffer, &application_data_size, 
            SDO_SERVER_BUFFER, &coErrorCode);
          if (ret == CANOPEN_OK)
          {
            u32 dataSrvUploadResp; // Data-field of the initiate upload response frame.
            u8 e;
            u8 s;
            u8 valid = 0;
            if (application_data_size <= 4)
            {
              // Expedited response. Convert the data in the buffer to a u32 to be used later.
              e = 1;
              s = 1;
              valid = (u8)application_data_size;
              dataSrvUploadResp = ::buf2val(data_buffer, (u8)application_data_size);
            }
            else
            {
              // Segmented response. 
              e = 0;
              s = 1;
              valid = 4;
              this->application_buffer_length   = application_data_size;
              this->application_buffer_offset   = 0;
              dataSrvUploadResp                 = application_data_size;
            }
            //canOpenStatus  setSrvCmdSpcInitUpResp(u8 valid, u8 e, u8 s); //9.2.2.2.5 Initiate SDO Upload Protocol
            ret = setSrvCmdSpcInitUpResp(valid, e, s);
            if (ret == CANOPEN_OK)
            {
              ret = setMultiplexor(object_index, sub_index);
              if (ret == CANOPEN_OK)
              {
                ret = setData(dataSrvUploadResp, valid, NULL);
                if (ret == CANOPEN_OK)
                {
                  if (e == 0)
                  {
                    this->state = SRV_WAIT_UPLOAD_SEGMENT_REQUEST;
                    this->data_buffer_counter = 0;
                    this->t = 0;
                  }
                  else
                  {
                    this->state = SRV_WAIT_ANY_CMD;
                  }
                  ret = this->writeToCanBus();
                }
              }
            }
          }
          else if (ret == CANOPEN_ERR_DEFINED_BY_APPL)
          {
            ret = this->setCliSrvAbortTransfer(this->application_object_index, 
              this->application_sub_index, 
              coErrorCode);
            if (ret == CANOPEN_OK)
            {
              ret = this->writeToCanBus();
              if (ret == CANOPEN_OK)
              {
                this->state = SRV_WAIT_ANY_CMD;
              }
            }
          }
        }
        else
        {
          // Read object properties where missing.
          u32 err = 0;
          if (flags & OBJ_WRITEABLE)
          {
            err = ATTEMPT_TO_READ_A_WRITE_ONLY_OBJ;
          }
          else
          {
            err = UNSUPPORTED_ACCESS_TO_AN_OBJECT;
          }
          ret = this->setCliSrvAbortTransfer(this->application_object_index, 
            this->application_sub_index, 
            err);
          if (ret == CANOPEN_OK)
          {
            ret = this->writeToCanBus();
            if (ret == CANOPEN_OK)
            {
              this->state = SRV_WAIT_ANY_CMD;
            }
          }
        }
      }

    }
  }
  return ret;
}


//------------------------------------------------------------------------
//9.2.2.2.6 Upload SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus   ServerSDO :: hndlUpSegReq(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 t;
  ret = getCliCmdSpcUpSegReq(&t);
  if (ret == CANOPEN_OK)
  {
    if (this->t == t)
    {
      u32 data_left_in_buffer = (this->application_buffer_length - data_buffer_counter);
      u8  bytes_to_send_in_segment = 0;
      u8  c = 0;
      if (data_left_in_buffer > 7)
      {
        bytes_to_send_in_segment = 7;
        c = 0; // more segments to be uploaded
      }
      else 
      {
       // There are 1 -> 7 bytes left to copy.
        bytes_to_send_in_segment = (u8)data_left_in_buffer;
        c = 1; // no more segments to be uploaded
        this->state = SRV_WAIT_ANY_CMD;
      }
      ret = setSrvCmdSpcUpSegResp(this->t, (7 - bytes_to_send_in_segment), c);
      if (ret == CANOPEN_OK)
      {
        ret = setSegData(&data_buffer[data_buffer_counter], bytes_to_send_in_segment);
        if (ret == CANOPEN_OK)
        {
          data_buffer_counter += bytes_to_send_in_segment;
          if (ret == CANOPEN_OK)
          {
            ret = this->writeToCanBus();
          }
        }
      }
      this->toggle();
    }    
    else
    {
      ; // toggle missmatch ?? qqq
    }
  }
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus ServerSDO :: registerObjectGetAttributesCallback(
  SrvGetAttrFuncPtr callback_function, void *context)
{
  this->object_attributes_callback = callback_function;
  this->object_attributes_callback_context = context;
  return CANOPEN_OK;
}
