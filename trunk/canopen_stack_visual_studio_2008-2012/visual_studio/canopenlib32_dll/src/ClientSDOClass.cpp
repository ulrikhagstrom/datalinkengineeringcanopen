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

#include "ClientSDOClass.h"

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
ClientSDO :: ClientSDO()
{
  DebugLogToFile("ClientSDO() entered\n");
  this->state = UNACTIVE;
  this->object_read_callback = NULL;
  this->object_write_callback = NULL;
  this->write_object_timeout = WRITE_OBJ_TIMEOUT;
  this->read_object_timeout = READ_OBJ_TIMEOUT;
  this->node_response_timeout = TAIL_TO_TAIL_TIMEOUT;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
ClientSDO :: ~ClientSDO()
{
    DebugLogToFile("~ClientSDO() entered\n");
  canHardwareDisconnect();
}


//------------------------------------------------------------------------
//  This method setup this SDO client to connect to the 
//  default server SDO (predefined connection set)
//  (see CiA DS301, 9.4.3 Pre-Defined Connection Set)
//  for more details.
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: connect(u8 node_id)
{
  DebugLogToFile("connect(u8 node_id) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;  
  if (node_id < 128)
  {
    this->node_id = node_id;
    this->cobid_tx = 0x600 + node_id;
    this->cobid_rx = 0x580 + node_id;
    ret = CANOPEN_OK;
  }
  else
  {
    this->node_id = INVALID_NODE_ID;
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//  With this method the user can connect to any SDO with any
//  COBID (for more advanced users).
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: connect(COBID cobid_tx, COBID cobid_rx)
{
  DebugLogToFile("connect(COBID cobid_tx, COBID cobid_rx) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;  
  this->cobid_tx = 0x600 + node_id;
  this->cobid_rx = 0x580 + node_id;
  ret = CANOPEN_OK;
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//Read CANopen DS301 section
//9.2.2.2.2 Initiate SDO Download Protocol
//------------------------------------------------------------------------

canOpenStatus  ClientSDO ::  setCliCmdSpcInitDnReq(u8 valid, u8 e, u8 s)
{
  DebugLogToFile("setCliCmdSpcInitDnReq(u8 valid, u8 e, u8 s) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  u8 n = 4 - valid;
  if ((n <= 3) && (e <= 1) && (s <= 1))
  {
    u8 temp   =   (1 << 5); // ccs == 1.
    temp     |=   (n << 2); // n
    temp     |=   (e << 1); // e
    temp     |=   (s << 0); // s
    this->can_data_tx[0] = temp;
    ret = CANOPEN_OK;
  }
  else
  {
    DebugLogToFile("setCliCmdSpcInitDnReq failed.\n");
    ret = CANOPEN_ARG_ERROR; // The arg where out of range.
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//Read CANopen DS301 section
//9.2.2.2.3 Download SDO Segment Protocol
//------------------------------------------------------------------------

canOpenStatus  ClientSDO ::  setCliCmdSpcDnSegReq(u8 valid, u8 c)
{
  DebugLogToFile("setCliCmdSpcDnSegReq(u8 valid, u8 c) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  u8 n = 7 - valid;
  u8 _t = this->t;
  if ((n <= 7) && (t <= 1) && (c <= 1))
  {
    u8 temp   =   (0  << 5); // ccs == 0
    temp     |=   (_t << 4); // t
    temp     |=   (n  << 1); // n
    temp     |=   (c  << 0); // c
    this->can_data_tx[0] = temp;
    ret = CANOPEN_OK;
  } 
  else
  {
    DebugLogToFile("setCliCmdSpcDnSegReq failed.\n");
    ret = CANOPEN_ARG_ERROR;
  }
  DebugLogToFile("setCliCmdSpcDnSegReq(u8 valid, u8 c) exit\n");
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//Read CANopen DS301 section
//9.2.2.2.5 Initiate SDO Upload Protocol 
//------------------------------------------------------------------------

canOpenStatus  ClientSDO ::  setCliCmdSpcInitUpReq(void)  
{
  DebugLogToFile("setCliCmdSpcInitUpReq(void) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;  
  this->can_data_tx[0] = (2 << 5); // ccs == 2.
  ret = CANOPEN_OK;
  DebugLogToFile("setCliCmdSpcInitUpReq(void) exit\n");
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//Read CANopen DS301 section
//9.2.2.2.6 Upload SDO Segment Protocol
//------------------------------------------------------------------------

canOpenStatus  ClientSDO ::  setCliCmdSpcUpSegReq(void) 
{
  DebugLogToFile("setCliCmdSpcUpSegReq(void) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;  
  u8 temp         =   (3 << 5); // ccs == 0
  temp           |=   (this->t << 4); // t
  can_data_tx[0]  =   temp; 
  ret = CANOPEN_OK;
  DebugLogToFile("setCliCmdSpcUpSegReq(void) exit\n");
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//9.2.2.2.2 Initiate SDO Download Protocol
//------------------------------------------------------------------------
canOpenStatus  ClientSDO ::  getSrvCmdSpcInitDnResp(void)
{
  DebugLogToFile("getSrvCmdSpcInitDnResp(void) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  u8   scs = this->getSrvCmdSpc();
  if (scs == 3)
  {
    ret = CANOPEN_OK;
  }
  else
  {
    DebugLogToFile("getSrvCmdSpcInitDnResp failed.\n");
    ret = CANOPEN_UNEXPECTED_SCS;
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//9.2.2.2.3 Download SDO Segment Protocol
//------------------------------------------------------------------------

canOpenStatus  ClientSDO ::  getSrvCmdSpcDnSegResp(u8 *t) 
{
  DebugLogToFile("getSrvCmdSpcDnSegResp(u8 *t) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;  
  u8   scs = this->getSrvCmdSpc();
  if (scs == 1)
  {
    *t = ((this->can_data_rx[0] >> 4) & 1);
    ret = CANOPEN_OK;
  }
  else
  {
    DebugLogToFile("getSrvCmdSpcDnSegResp failed.\n");
    ret = CANOPEN_UNEXPECTED_SCS;
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
// 9.2.2.2.9 Initiate SDO Block Download Protocol
//------------------------------------------------------------------------
canOpenStatus  ClientSDO ::  getSrvCmdSpcInitBlockDnResp(u8 *cs) 
{
  DebugLogToFile("getSrvCmdSpcInitBlockDnResp(u8 *cs) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;  
  u8   scs = this->getSrvCmdSpc();
  if (scs == 5)
  {
    if ((this->can_data_rx[0] & 2) == 0) // server subcommand == initiate download response ?
    {
      *cs = ((this->can_data_rx[0] >> 2) & 1);
      ret = CANOPEN_OK;
    }
    else 
    {
      DebugLogToFile("getSrvCmdSpcInitBlockDnResp failed.\n");
      ret = CANOPEN_UNEXPECTED_SCS;
    }
  }
  else
  {
    DebugLogToFile("getSrvCmdSpcInitBlockDnResp failed.\n");
    ret = CANOPEN_UNEXPECTED_SCS;
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//Read CANopen DS301 section
//9.2.2.2.5 Initiate SDO Upload Protocol
//------------------------------------------------------------------------

canOpenStatus  ClientSDO ::  getSrvCmdSpcInitUpResp(u8 *n, u8 *e, u8 *s) 
{
  DebugLogToFile("getSrvCmdSpcInitUpResp(u8 *n, u8 *e, u8 *s) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  u8   scs = this->getSrvCmdSpc();
  if (scs == 2)
  {
    *n = ((this->can_data_rx[0] >> 2) & 3);
    *e = ((this->can_data_rx[0] >> 1) & 1);
    *s = ((this->can_data_rx[0] >> 0) & 1);
    ret = CANOPEN_OK;
  }
  else
  {
    DebugLogToFile("getSrvCmdSpcInitUpResp failed.\n");
    ret = CANOPEN_UNEXPECTED_SCS;
  }
  DebugLogToFile("getSrvCmdSpcInitUpResp(u8 *n, u8 *e, u8 *s) exit\n");
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//9.2.2.2.6 Upload SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  ClientSDO ::  getSrvCmdSpcUpSegResp(u8 *t, u8 *n, u8 *c) 
{
  DebugLogToFile("getSrvCmdSpcUpSegResp(u8 *t, u8 *n, u8 *c) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;  
  u8   scs = this->getSrvCmdSpc();
  if (scs == 0)
  {
    *t = ((this->can_data_rx[0] >> 4) & 1);
    *n = ((this->can_data_rx[0] >> 1) & 7);
    *c = ((this->can_data_rx[0] >> 0) & 1);
    ret = CANOPEN_OK;
  }
  else
  {
    DebugLogToFile("getSrvCmdSpcUpSegResp failed.\n");
    ret = CANOPEN_UNEXPECTED_SCS;
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//9.2.2.2.10 Download SDO Block Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  ClientSDO ::  getSrvCmdSpcBlkDnResp(u8 *ss, u8 *ack_sequence, 
                                                   u8 *block_size)
{
  DebugLogToFile("getSrvCmdSpcBlkDnResp(u8 *ss, u8 *ack_sequence, u8 *block_size) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  u8 flags = this->can_data_rx[0];
  u8 scs = 0;
  scs = (flags >> 5) & 7; 
  *ss  = flags & 3;
  if (scs == 5) // expected scs == 5, block download.
  {    
    *ack_sequence = ::getU8Val(this->can_data_rx, 1); // Get from CAN.
    *block_size = ::getU8Val(this->can_data_rx, 2);
    ret = CANOPEN_OK;
  }
  else
  {
    DebugLogToFile("getSrvCmdSpcBlkDnResp failed.\n");
    ret = CANOPEN_ERROR; // Most likely a abort transfer frame.
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
// 9.2.2.2.11 End SDO Block Download Protocol
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: getSrvCmdSpcEndBlkDnResp(void)
{
  DebugLogToFile("getSrvCmdSpcEndBlkDnResp(void) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  u8 flags = this->can_data_rx[0];
  u8 scs = (flags >> 5) & 7;
  u8 ss  = flags & 3;
  if (scs == 5 && ss == 1)
  {
    ret = CANOPEN_OK;
  }
  else
  {
    DebugLogToFile("getSrvCmdSpcEndBlkDnResp failed.\n");
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//9.2.2.2.9 Initiate SDO Block Download Protocol
//------------------------------------------------------------------------
canOpenStatus  ClientSDO ::  setCliCmdSpcInitBlockDnReq(u8 cc, u8 s)
{
  DebugLogToFile("setCliCmdSpcInitBlockDnReq(u8 cc, u8 s) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  if ((cc <= 1) && (s <= 1))
  {
    u8 temp   =   (6 << 5); // ccs == 6.  // Block download.
    temp     |=   (cc << 2); // cc - client crc support.
    temp     |=   (s << 1); // s - size.
    temp     |=   (0 << 0); // initiate download request (client subcommand)
    this->can_data_tx[0] = temp;
    ret = CANOPEN_OK;
  }
  else
  {
    DebugLogToFile("getSrvCmdSpcEndBlkDnResp failed.\n");
    ret = CANOPEN_ARG_ERROR; // The arg where out of range.
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//  Collects the server command specifier from a received message.
//------------------------------------------------------------------------
u8 ClientSDO ::  getSrvCmdSpc(void)
{
  return ((this->can_data_rx[0] >> 5) & 7);  // scs.
}

//------------------------------------------------------------------------
// 9.2.2.2.11 End SDO Block Download Protocol
//------------------------------------------------------------------------
canOpenStatus ClientSDO :: setCliCmdSpcEndBlkDnReq(u8 n, u16 crc)
{
  DebugLogToFile("setCliCmdSpcEndBlkDnReq(u8 n, u16 crc) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  if (n >= 0 && n <= 7)
  {
    u8 temp = 0;
    temp  = (6 << 5); // ccs = 6.
    temp |= (n << 2); // n (0...7)
    temp |= (1 << 0); // cs = 1.
    this->can_data_tx[0] = temp;
    ::setU16Val(crc, can_data_tx, 1);
    ret = this->clrTxCanDataField(3, 7);
  }
  else
  {
    DebugLogToFile("setCliCmdSpcEndBlkDnReq failed.\n");
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
// This method is being used for reading U8:s from the object 
// dictionary of a remote node.
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: objectRead(u16 object_index, u8 sub_index, 
  u8  *val, CanOpenErrorCode *error_code)
{
  DebugLogToFile("objectRead(u16 object_index, u8 sub_index, u8  *val, CanOpenErrorCode *error_code) entered\n");
  CanOpenErrorCode coErrorCode = 0;
  u32 valid = 0;
  u8  buf[1];
  canOpenStatus ret = CANOPEN_ERROR;

  if (this->object_read_callback != NULL)
  {
    DebugLogToFile("objectRead-u8 read - async enabled.\n");
    ret = CANOPEN_ERROR_ASYNC_TRANSFER_ENABLED;
  }
  else
  {
    ret = this->objectRead(object_index, sub_index, 
      buf, 1, &valid, error_code);
    if (ret == CANOPEN_OK)
    {
      if (valid == 1)
      {
        *val = ::getU8Val(buf, 0);
      }
      else
      {
        DebugLogToFile("objectRead-u8 failed #1.\n");
        ret = CANOPEN_ERROR;
      }
    }
    else
    {
        DebugLogToFile("objectRead-u8 failed #2.\n");
    }
  }
  return DebugExitErrorValueLogToFile(ret);
}


//------------------------------------------------------------------------
// This method is being used for reading U16:s from the object dictionary 
// of a remote node.
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: objectRead(u16 object_index, u8 sub_index, 
  u16  *val, CanOpenErrorCode *error_code)
{
  DebugLogToFile("objectRead(u16 object_index, u8 sub_index, u16 *val, CanOpenErrorCode *error_code) entered\n");
  CanOpenErrorCode coErrorCode = 0;
  u32 valid = 0;
  u8  buf[2];
  canOpenStatus ret = CANOPEN_ERROR;

  if (this->object_read_callback != NULL)
  {
    DebugLogToFile("objectRead-u16 read - async enabled.\n");
    ret = CANOPEN_ERROR_ASYNC_TRANSFER_ENABLED;
  }
  else
  {
    ret = this->objectRead(object_index, sub_index, buf, 2, &valid, error_code);
    if (ret == CANOPEN_OK)
    {
      if (valid == 2)
      {
        *val = ::getU16Val(buf, 0);
      }
      else
      {
        DebugLogToFile("objectRead-u16 failed #1.\n");
        ret = CANOPEN_ERROR;
      }
    }
    else
    {
      DebugLogToFile("objectRead-u16 failed #2.\n");
    }
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
// This method is being used for reading U32:s from the object dictionary 
// of a remote node.
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: objectRead(u16 object_index, u8 sub_index, 
                                       u32  *val, CanOpenErrorCode *error_code)
{
  DebugLogToFile("objectRead(u16 object_index, u8 sub_index, u32  *val, CanOpenErrorCode *error_code) entered\n");
  CanOpenErrorCode coErrorCode = 0;
  u32 valid = 0;
  u8  buf[4];
  canOpenStatus ret = CANOPEN_ERROR;

  if (this->object_read_callback != NULL)
  {
    DebugLogToFile("objectRead-u32 read - async enabled.\n");
    ret = CANOPEN_ERROR_ASYNC_TRANSFER_ENABLED;
  }
  else
  {
    ret = this->objectRead(object_index, sub_index, buf, 4, &valid, error_code);
    if (ret == CANOPEN_OK)
    {
      DebugLogToFile("ret == CANOPEN_OK\n");
      char dbgStr[20];
      sprintf(dbgStr, "1: ret = %d\n", ret);
      DebugLogToFile(dbgStr);

      sprintf(dbgStr, "valid = %d\n", ret);
      DebugLogToFile(dbgStr);

      if (valid <= 4)
      {
        if (valid == 4)
          *val = ::getU32Val(buf, 0);
        if (valid == 3)
          *val = ::getU24Val(buf, 0);
        if (valid == 2)
          *val = ::getU16Val(buf, 0);
        if (valid == 1)
          *val = ::getU8Val(buf, 0);

        char dbgStr[20];
        sprintf(dbgStr, "2: ret = %d\n", ret);
        DebugLogToFile(dbgStr);
      }
      else
      {
        DebugLogToFile("valid seems to be corrupt.\n");
        ret = CANOPEN_ERROR;
      }
      sprintf(dbgStr, "3: ret = %d\n", ret);
      DebugLogToFile(dbgStr);
    }
    else
    {
      DebugLogToFile("objectRead-u32 failed #2.\n");
    }
    char dbgStr[20];
    sprintf(dbgStr, "4: ret = %d\n", ret);
    DebugLogToFile(dbgStr);
  }
  DebugLogToFile("objectRead(u16 object_index, u8 sub_index, u32  *val, CanOpenErrorCode *error_code) exit\n");
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
// This method is being used for reading U64:s from the object dictionary of a remote node.
//------------------------------------------------------------------------
#ifdef HAS_LONG_LONG
canOpenStatus  ClientSDO :: objectRead(u16 object_index, u8 sub_index, u64  *val, CanOpenErrorCode *error_code)
{
  CanOpenErrorCode coErrorCode = 0;
  u32 valid = 0;
  u8  buf[8];
  canOpenStatus ret = this->objectRead(object_index, sub_index, buf, 8, &valid, error_code);
  if (ret == CANOPEN_OK)
  {
    if (valid == 8)
    {
      *val = ::getU64Val(buf, 0);
    }
    else
    {
      ret = CANOPEN_ERROR;
    }
  }
  return DebugExitErrorValueLogToFile(ret);
}
#endif

//------------------------------------------------------------------------
// This method is being used for reading any object type from the object 
// dictionary of a remote node.
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: objectRead(u16 object_index, u8 sub_index, 
  u8 *buf, u32 buffer_size, u32 *valid, CanOpenErrorCode *error_code)
{
  WaitForSingleObject( this->rx_tx_mutex, INFINITE);
  DebugLogToFile("objectRead(u16 object_index, u8 sub_index, u8 *buf, u32 buffer_size, u32 *valid, CanOpenErrorCode *error_code) entered\n");
  this->application_object_index       = object_index;
  this->application_sub_index          = sub_index;
  this->application_buffer_length      = buffer_size;
  this->application_buffer_offset      = 0; // Initiate the buffer pointer.
  this->application_buffer             = buf;
  this->application_valid_bytes        = valid;
  this->application_canopen_error_code = error_code;
  this->remote_node_error_code         = 0;
  *error_code                          = 0;
  *valid                               = 0;

  canOpenStatus ret = CANOPEN_ERROR;  

  ret = this->setCliCmdSpcInitUpReq();
  if (ret == CANOPEN_OK)
  {
    ret = this->setMultiplexor(object_index, sub_index);
    if (ret == CANOPEN_OK)
    {
      ret = this->setData(0, 4, NULL);  // Clear the reserved bytes in the "Initiate upload request"-frame.
    }
    else
    {
      DebugLogToFile("objectRead *buf failed #1.\n");
    }
    if (ret == CANOPEN_OK)
    {
      this->state = WAIT_INITIATE_UPLOAD_RESPONSE;
    }
    else
    {
      DebugLogToFile("objectRead *buf failed #2.\n");
    }
  }
  else
  {
    DebugLogToFile("objectRead *buf failed #3.\n");
  }
  if (ret == CANOPEN_OK)
  {
    this->is_transfer_stopped_or_finished = FALSE;
    ret = this->synchronize(this->read_object_timeout, READ_SESSION);
  }
  else
  {
    DebugLogToFile("objectRead *buf failed #4.\n");
  }
  DebugLogToFile("objectRead(u16 object_index, u8 sub_index, u8 *buf, u32 buffer_size, u32 *valid, CanOpenErrorCode *error_code) exit\n");
  ReleaseMutex(this->rx_tx_mutex);
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
// 9.2.2.2.5 Initiate SDO Upload Protocol
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: hndlInitUpResp(void)
{
  DebugLogToFile("hndlInitUpResp(void) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  u8  n;
  u8  e;
  u8  s;
  ret = this->getSrvCmdSpcInitUpResp(&n, &e, &s);
  if (ret == CANOPEN_OK)
  {
    u16 object_index;
    u8  sub_index;
    ret = this->getMultiplexor(&object_index, &sub_index);
    if (ret == CANOPEN_OK)
    {
      if (this->application_object_index == object_index && this->application_sub_index == sub_index)
      {
        u32 val;
        u8  valid;
        ret = this->getData(&val, &valid, n);
        if (ret == CANOPEN_OK)
        {
          if (e == 1) // Expedited.
          {
            if (this->application_buffer_length >= valid) // chk appl buf is OK.
            {
              ::val2buf(val, this->application_buffer, valid); // write to applications buffer.
              *(this->application_valid_bytes) = valid; // tell the application number of bytes written to buffer.
              *(this->application_canopen_error_code) = 0;
              this->transfer_result = CANOPEN_OK;
            }
            else
            {
              DebugLogToFile("hndlInitUpResp failed #1\n");
              this->transfer_result = CANOPEN_BUFFER_TOO_SMALL;
            }
            this->is_transfer_stopped_or_finished = TRUE;
            if (this->object_read_callback != NULL)
            {
              this->object_read_callback(this->object_read_callback_context,
                this->transfer_result,
                this->node_id,
                this->application_object_index, 
                this->application_sub_index, 
                this->application_buffer, 
                *(this->application_valid_bytes),
                *(this->application_canopen_error_code));
            }
          }
          else // e == 0.
          {
            if (s == 1)
            {
              if (val <= this->application_buffer_length)
              {
                this->t = 0;
                ret = this->setCliCmdSpcUpSegReq();
                if (ret == CANOPEN_OK)
                {
                  ret = this->clrTxCanDataField(1, 7); //Clear the "reserved" space in the CAN frame.
                  if (ret == CANOPEN_OK)
                  {
                    this->state = WAIT_UPLOAD_SEGMENT_RESPONSE;
                    ret = this->writeToCanBus();
                  }
                  else 
                  {
                    DebugLogToFile("hndlInitUpResp failed #2\n");
                  }
                }
                else 
                {
                  DebugLogToFile("hndlInitUpResp failed #3\n");
                }
              }
              else
              {
                // Rx buffer size too small.
                DebugLogToFile("hndlInitUpResp failed #4\n");
                ret = this->setCliSrvAbortTransfer(this->application_object_index, 
                  this->application_sub_index, OUT_OF_MEMORY);
                if (ret == CANOPEN_OK)
                {
                  ret = this->writeToCanBus();
                  this->is_transfer_stopped_or_finished = TRUE;
                  this->transfer_result = CANOPEN_BUFFER_TOO_SMALL;
                }
                else 
                {
                  DebugLogToFile("hndlInitUpResp failed #5\n");
                }
              }
            }
            else 
            {
              DebugLogToFile("hndlInitUpResp failed #6\n");
            }
          }
        }
        else 
        {
          DebugLogToFile("hndlInitUpResp failed #7\n");
        }
      }
      else 
      {
        DebugLogToFile("hndlInitUpResp failed #8\n");
      }
    }
    else 
    {
      DebugLogToFile("hndlInitUpResp failed #9\n");
    }
  }
  else 
  {
    DebugLogToFile("hndlInitUpResp failed #10\n");
  }
  DebugLogToFile("hndlInitUpResp(void) exit\n");
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
// 9.2.2.2.6 Upload SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  ClientSDO ::hndlSegmUpResp(void)
{
  DebugLogToFile("hndlSegmUpResp(void) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;  
  u8 t;
  u8 n;
  u8 c;
  ret = this->getSrvCmdSpcUpSegResp(&t, &n, &c);
  if (ret == CANOPEN_OK)
  {
    if (this->t == t)
    {
      u8 segment_data[8];
      u8 valid;
      ret = this->getSegData(segment_data, &valid, n);
      if (ret == CANOPEN_OK)
      {
        u32 calc_valid = application_buffer_offset + valid; // calc valid data so far, 
        if (calc_valid <= this->application_buffer_length)
        {
          memcpy(this->application_buffer + this->application_buffer_offset, segment_data, valid); 
          this->application_buffer_offset = calc_valid;
          this->transfer_result = CANOPEN_OK;
          if (c == 1) // No more segments.
          {
            *(this->application_valid_bytes) = calc_valid;   
            if (this->object_read_callback != NULL)
            {
              this->object_read_callback(this->object_read_callback_context,
                this->transfer_result,
                this->node_id,
                this->application_object_index, 
                this->application_sub_index, 
                this->application_buffer, 
                *(this->application_valid_bytes),
                *(this->application_canopen_error_code));
            }
            this->state = UNACTIVE;
            this->is_transfer_stopped_or_finished = TRUE;  // Will release endless loop in API call objectRead()
          }
          else
          {
            this->toggle();
            ret = this->setCliCmdSpcUpSegReq();
            if (ret == CANOPEN_OK)
            {
              ret = this->clrTxCanDataField(1, 7); // Clear the "reserved" data field.
              if (ret == CANOPEN_OK)
              {
                ret = this->writeToCanBus();
              }
              else
              {
                DebugLogToFile("hndlSegmUpResp failed #1\n");
              }
            }
            else
            {
              DebugLogToFile("hndlSegmUpResp failed #2\n");
            }
          }
        }
        else
        {
          DebugLogToFile("hndlSegmUpResp failed #3\n");
          ret = CANOPEN_BUFFER_TOO_SMALL;
        }
      }
    }
    else
    {
      DebugLogToFile("hndlSegmUpResp failed #4\n");
      ret = CANOPEN_TOGGLE;
    }
  }
  else 
  {
    DebugLogToFile("hndlSegmUpResp failed #5\n");
  }
  if (ret != CANOPEN_OK)
  {
    this->transfer_result = ret;
    this->is_transfer_stopped_or_finished = TRUE;
    if (this->object_read_callback != NULL)
    {
      this->object_read_callback(this->object_read_callback_context,
        this->transfer_result,
        this->node_id,
        this->application_object_index, 
        this->application_sub_index, 
        this->application_buffer, 
        *(this->application_valid_bytes),
        *(this->application_canopen_error_code));
    }
  }
  return DebugExitErrorValueLogToFile(ret);  
}

//------------------------------------------------------------------------
// 9.2.2.2.2 Initiate SDO Download Protocol
//------------------------------------------------------------------------
canOpenStatus  ClientSDO ::hndlInitDlResp(void)
{
  DebugLogToFile("hndlInitDlResp(void) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;  

  ret = this->getSrvCmdSpcInitDnResp();
  if (ret == CANOPEN_OK)
  {
    u16 object_index;
    u8 sub_index;
    ret = this->getMultiplexor(&object_index, &sub_index);
    if (ret == CANOPEN_OK)
    {
      if (this->application_object_index == object_index && this->application_sub_index == sub_index)
      {
        if (this->application_buffer_length > 4) // Check if this was expedited or not.
        {
          u32 remaining_in_buffer = this->getRestAppBuffer();
          // This is segmented, build a download segment request message.
          u8 bytes_in_current_segment = 0;
          if (remaining_in_buffer > 7)
          {
            bytes_in_current_segment = 7;
          }
          else
          {
            bytes_in_current_segment = (u8)remaining_in_buffer; // 7 max.
          }
          this->t = 0;
          ret = this->setSegData(bytes_in_current_segment);
          if (ret == CANOPEN_OK)
          {
            if (this->getRestAppBuffer() <= 0) // Are there more to download? (should c be set or not?)
            {
              // transfer is finished, still we can not shut down the transfers
              // here becuse we'll have to wait for the acknowledge on this frame.
              ret = this->setCliCmdSpcDnSegReq(bytes_in_current_segment, 1); // no more segments to be uploaded
              this->state = WAIT_DOWNLOAD_SEGMENT_RESPONSE;
            }
            else
            {
              // transfer will contine..
              ret = this->setCliCmdSpcDnSegReq(bytes_in_current_segment, 0); // more segments to be uploaded
              this->state = WAIT_DOWNLOAD_SEGMENT_RESPONSE;
            }
            if (ret == CANOPEN_OK)
            {
              ret = this->writeToCanBus();
            }
            else
            {
              DebugLogToFile("hndlInitDlResp failed #1\n");
            }
          }
          else
          {
            DebugLogToFile("hndlInitDlResp failed #2\n");
          }
        }
        else 
        {
          // A proper expedited write success acknowledge has been 
          // received from the server. Shut down this transfer.
          this->state = UNACTIVE;
          this->is_transfer_stopped_or_finished = TRUE; 
          this->transfer_result = CANOPEN_OK;
          *(this->application_canopen_error_code) = 0;
          if (this->object_write_callback != NULL)
          {
            this->object_write_callback(this->object_write_callback_context,
              this->transfer_result,
              this->node_id,
              this->application_object_index, 
              this->application_sub_index, 
              *(this->application_canopen_error_code));
          }
        }
      }
      else 
      {
        DebugLogToFile("hndlInitDlResp failed #3\n");
        ret = CANOPEN_OBJECT_MISSMATCH;
      }
    }
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
// 9.2.2.2.3 Download SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  ClientSDO ::hndlSegDlResp(void)
{
  DebugLogToFile("hndlSegDlResp(void) entered\n");  
  canOpenStatus ret = CANOPEN_ERROR;  
  u8 t;

  ret = this->getSrvCmdSpcDnSegResp(&t);
  if (ret == CANOPEN_OK)
  {
    if (this->t == t)
    {
      u32 remaining_in_buffer = this->getRestAppBuffer();
      if (remaining_in_buffer > 0)
      {
        u8 bytes_in_current_segment = 0;
        u8 c;
        if (remaining_in_buffer > 7)
        {
          bytes_in_current_segment = 7;  // Max 7 bytes in one segment.
          c = 0; // more segments to be downloaded
        }
        else 
        {
          bytes_in_current_segment = (u8)remaining_in_buffer;
          c = 1;  // no more segments to be downloaded
        }
        this->toggle();
        ret = this->setSegData( bytes_in_current_segment );
        if (ret == CANOPEN_OK)
        {
          ret = this->setCliCmdSpcDnSegReq(bytes_in_current_segment, c);
          if (ret == CANOPEN_OK)
          {
            ret = this->writeToCanBus();
          }
          else 
          {
            DebugLogToFile("hndlSegDlResp failed #1\n");
          }
        }
      }
      else
      {
        this->transfer_result = CANOPEN_OK;
        this->state = UNACTIVE;
        this->is_transfer_stopped_or_finished = TRUE;
        if (this->object_write_callback != NULL)
        {
          this->object_write_callback(this->object_write_callback_context,
            this->transfer_result,
            this->node_id,
            this->application_object_index, 
            this->application_sub_index, 
            *(this->application_canopen_error_code));
        }
      }
    }
    else
    {
      DebugLogToFile("hndlSegDlResp failed #2\n");
      ret = CANOPEN_TOGGLE;
    }
  }
  else 
  {
    DebugLogToFile("hndlSegDlResp failed #3\n");
  }
  return DebugExitErrorValueLogToFile(ret);  
}


//------------------------------------------------------------------------
// 9.2.2.2.9 Initiate SDO Block Download Protocol
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: hndlInitBlkDlResp(void)
{
  DebugLogToFile("hndlInitBlkDlResp(void) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  u8 cs = 0;
  ret = this->getSrvCmdSpcInitBlockDnResp(&cs);
  if (ret == CANOPEN_OK)
  {
    if (((this->crc ? 0 : 1) ^ (cs ? 0 : 1)) == 0)
    {
      u16 object_index;
      u8  sub_index;
      ret = this->getMultiplexor(&object_index, &sub_index);
      if (ret == CANOPEN_OK)
      {
        if (this->application_object_index == object_index && this->application_sub_index == sub_index)
        {
          u32 data = 0;
          ret = this->getData(&data, NULL, 0);
          if (ret == CANOPEN_OK)
          {
            this->block_transfer_segment_count = 1;
            this->block_size = (u8)(data & 0xff);  // qqq, what about if data == 0?
            this->state = SENDING_DOWNLOAD_BLOCK;
            ret = this->setBlkSegData();
            if (ret == CANOPEN_OK)
            {
              if (this->getRestAppBuffer() > 0)
              {
                this->state = SENDING_DOWNLOAD_BLOCK;
              }
              else
              {
                // Block transfer on a very small object.
                this->state = WAIT_DOWNLOAD_BLOCK_RESPONSE;
              }
              ret = this->writeToCanBus();
            }
          }
          else
          {
            DebugLogToFile("hndlInitBlkDlResp failed #1\n");
          }
        }
        else
        {
          DebugLogToFile("hndlInitBlkDlResp failed #2\n");
        }
      }
      else
      {
        DebugLogToFile("hndlInitBlkDlResp failed #3\n");
      }
    }
  }
  else 
  {
    DebugLogToFile("hndlInitBlkDlResp failed #4\n");
  }
  return DebugExitErrorValueLogToFile(ret);
}


//------------------------------------------------------------------------
// 9.2.2.2.10 Download SDO Block Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: hndlDlBlkResp(void)
{
  DebugLogToFile("hndlDlBlkResp(void) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  u8 ss             = 0;
  u8 ack_sequence   = 0;
  u8 block_size     = 0;
  ret = getSrvCmdSpcBlkDnResp(&ss, &ack_sequence, &block_size);
  if (ret == CANOPEN_OK && ss == 2)
  {
    u32 left_in_application_buffer = this->getRestAppBuffer();
    this->block_size = block_size;
    if (block_transfer_segment_count && (ack_sequence < (block_transfer_segment_count - 1)))
    {
      u16 blocks_to_retransmit  = 0;
      u32 bytes_to_retransmit = 0;
      blocks_to_retransmit = ((block_transfer_segment_count - 1) - ack_sequence);
      if (left_in_application_buffer != 0)
      {
        bytes_to_retransmit = blocks_to_retransmit * 7;
      }
      else 
      {
        if (this->application_buffer_length % 7)
        {
          bytes_to_retransmit = (blocks_to_retransmit - 1) * 7 + (this->application_buffer_length % 7);
        }
        else
        {
          bytes_to_retransmit = blocks_to_retransmit * 7;
        }
      }
      ret = this->decrementapplication_buffertr(bytes_to_retransmit);
      if (ret == CANOPEN_OK)
      {
        if (this->block_transfer_segment_count >= blocks_to_retransmit)
        {
          this->block_transfer_segment_count -= blocks_to_retransmit;
          this->state = SENDING_DOWNLOAD_BLOCK;
        }
      }
      else
      {
        DebugLogToFile("hndlDlBlkResp failed #1\n");
      }
    }
    else if ((ack_sequence == (block_transfer_segment_count - 1)) && ss == 2)
    {
      if (left_in_application_buffer == 0)
      {
        // Calculate the number of bytes in the last block frame that was not valid 
        // == n here : 9.2.2.2.11 End SDO Block Download Protocol.
        // Bytes [8-n, 7] do not contain segment data.
        
        // 7 valid bytes (no invalid) : n = 0 [8,7], n = 1 [7,7] ?
        // 6 valid bytes (1 invalid) : n = 2 [6,7].
        // 5 valid bytes (2 invalid) : n = 3 [5,7].
        // 4 valid bytes (3 invalid) : n = 4 [4,7].
        // 3 valid bytes (4 invalid) : n = 5 [3,7].
        // 2 valid bytes (5 invalid) : n = 6 [2,7]
        // 1 valid bytes (6 invalid) : n = 7.[2,7]

        u8 n = 7 - this->valid_bytes_in_last_segment_in_last_block;
        ret = setCliCmdSpcEndBlkDnReq(n, this->crc);
        if (ret == CANOPEN_OK)
        {
          this->state = WAIT_DOWNLOAD_BLOCK_END;
          ret = this->writeToCanBus();
        }
        else
        {
          DebugLogToFile("hndlDlBlkResp failed #2\n");
        }
      }
      else
      {
        // Everything is fine, just continue.
        this->block_transfer_segment_count = 1;
        this->state = SENDING_DOWNLOAD_BLOCK;
      }
    }
    else 
    {
      DebugLogToFile("hndlDlBlkResp failed #3\n");
      ret = CANOPEN_INTERNAL_STATE_ERROR; // Should never happen.
    }
  }
  else
  {
    DebugLogToFile("hndlDlBlkResp failed #4\n");
  }
  return DebugExitErrorValueLogToFile(ret);
}



//------------------------------------------------------------------------
// 9.2.2.2.11 End SDO Block Download Protocol
//------------------------------------------------------------------------
canOpenStatus ClientSDO :: hndlEndBlkDnResp(void)
{
  DebugLogToFile("hndlEndBlkDnResp(void) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  ret = this->getSrvCmdSpcEndBlkDnResp();
  if (ret == CANOPEN_OK)
  {
    this->transfer_result = CANOPEN_OK;
    this->is_transfer_stopped_or_finished = true; //Releases the sync write.
    if (this->object_write_callback != NULL)
    {
      this->object_write_callback(this->object_write_callback_context,
        this->transfer_result,
        this->node_id,
        this->application_object_index, 
        this->application_sub_index, 
        *(this->application_canopen_error_code));
    }
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
// Wrapper for being able to setup callbacks to non-static funcs.
//------------------------------------------------------------------------
canOpenStatus ClientSDO :: canFrameConsumerW(void *ClientSDOcontext, 
  unsigned long id, unsigned char *data, unsigned int dlc, unsigned int flags)
{
  ClientSDO *cliSDO = (ClientSDO*) ClientSDOcontext;
  return cliSDO->canFrameConsumer(id, data, dlc, flags);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: canFrameConsumer(unsigned long id, 
  unsigned char *data, unsigned int dlc, unsigned int flags)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (id == this->cobid_rx)
  {
    ret = this->setLatestEventTimestamp();
    if (ret == CANOPEN_OK)
    {
      ret = this->getFrameData(id, data, dlc);
      if (ret == CANOPEN_OK)
      {
        switch (this->state)
        {
          case WAIT_INITIATE_UPLOAD_RESPONSE:
            DebugLogToFile("calling hndlInitUpResp()\n");
            ret = hndlInitUpResp(); // 9.2.2.2.5 Initiate SDO Upload Protocol
            break;
          case WAIT_UPLOAD_SEGMENT_RESPONSE:
            DebugLogToFile("calling hndlSegmUpResp()\n");
            ret = hndlSegmUpResp(); // 9.2.2.2.6 Upload SDO Segment Protocol
            break;
          case WAIT_INITIATE_DOWNLOAD_RESPONSE:
            DebugLogToFile("calling hndlInitDlResp()\n");
            ret = hndlInitDlResp(); // 9.2.2.2.2 Initiate SDO Download Protocol
            break;
          case WAIT_DOWNLOAD_SEGMENT_RESPONSE:
            DebugLogToFile("calling hndlSegDlResp()\n");
            ret = hndlSegDlResp(); // 9.2.2.2.3 Download SDO Segment Protocol
            break;
          case WAIT_INITIATE_DOWNLOAD_BLOCK_RESPONSE:
            DebugLogToFile("calling hndlInitBlkDlResp()\n");
            ret = hndlInitBlkDlResp(); // 9.2.2.2.9 Initiate SDO Block Download Protocol
            break;
          case SENDING_DOWNLOAD_BLOCK:  // Fall-thru is OK since remote node maybe sends us retransmit request.
          case WAIT_DOWNLOAD_BLOCK_RESPONSE:
            DebugLogToFile("calling hndlDlBlkResp()\n");
            ret = hndlDlBlkResp(); // 9.2.2.2.10 Download SDO Block Segment Protocol
            break;
          case WAIT_DOWNLOAD_BLOCK_END:  //9.2.2.2.11 End SDO Block Download Protocol
            DebugLogToFile("calling hndlEndBlkDnResp()\n");
            ret = hndlEndBlkDnResp(); 
            break;
          case UNACTIVE:
            ret = CANOPEN_MSG_NOT_PROCESSED;
            break;
          default:
            DebugLogToFile("canFrameConsumer failed #1\n\n");
            ret = CANOPEN_INTERNAL_STATE_ERROR;
            break;
        }
      }
      else
      {
        DebugLogToFile("canFrameConsumer failed #2\n");
      }
      if (ret != CANOPEN_OK)
      {
        // Abort frame, maybe?
        DebugLogToFile("canFrameConsumer failed #3\n");
        ret = getCliSrvCmdSpcAbrtTrnsf();
        if (ret == CANOPEN_OK)
        {
          ret = hndlRemoteAbort();  //9.2.2.2.7 Abort SDO Transfer Protocol
          if (ret == CANOPEN_OK)
          {
            this->transfer_result = CANOPEN_REMOTE_NODE_ABORT; // The CANopen error-code has been witten to remote_node_error_code. 
            if (this->transfer_direction == WRITE_SESSION && object_write_callback != NULL)
            {
              this->object_write_callback(this->object_write_callback_context,
                this->transfer_result,
                this->node_id,
                this->application_object_index, 
                this->application_sub_index, 
                this->remote_node_error_code);
                this->is_transfer_stopped_or_finished = TRUE;
                this->state = UNACTIVE;
            }
            else if (this->transfer_direction == READ_SESSION && object_read_callback != NULL)
            {
              this->object_read_callback(this->object_read_callback_context,
                this->transfer_result,
                this->node_id,
                this->application_object_index, 
                this->application_sub_index, 
                NULL, 
                0,
                this->remote_node_error_code);
                this->is_transfer_stopped_or_finished = TRUE;
                this->state = UNACTIVE;
            }
          }
          else
          {
            DebugLogToFile("canFrameConsumer failed #3\n");
          }
        }
      }
    }
    else
    {
      DebugLogToFile("canFrameConsumer failed #4\n");
    }
  }
  else
  {
    ret = CANOPEN_MSG_NOT_PROCESSED;
  }
  return ret;
}

//------------------------------------------------------------------------
// Wrapper for being able to setup callbacks to non-static funcs.
//------------------------------------------------------------------------
canOpenStatus ClientSDO :: clientSDOStateMachineW(void *ClientSDOcontext)
{
  canOpenStatus ret = CANOPEN_ERROR;
  ClientSDO *cliSDO = (ClientSDO*) ClientSDOcontext;
  ret = cliSDO->clientSDOStateMachine();
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: clientSDOStateMachine(void)
{
  bool transfer_timeout = FALSE;
  canOpenStatus ret = CANOPEN_ERROR;
  if (this->state == SENDING_DOWNLOAD_BLOCK)
  {
    if (this->block_size <= this->block_transfer_segment_count)
    {
      this->state = WAIT_DOWNLOAD_BLOCK_RESPONSE;
    }
    ret = this->setBlkSegData();

    if (this->getRestAppBuffer() == 0)
    {
      this->state = WAIT_DOWNLOAD_BLOCK_RESPONSE;
    }
    if (ret == CANOPEN_OK)
    {
      ret = this->writeToCanBus();
      if (ret == CANOPEN_OK)
      {
        this->setLatestEventTimestamp();
      }
    }
  }
  else 
  {
    ret = CANOPEN_OK;
  }

  // Need to implement timeout control since synchronization not done
  // if callbacks are configured.
  if (this->isTransferTimeout() == TRUE && 
      this->is_transfer_stopped_or_finished == FALSE &&
      this->state != UNACTIVE &&
      (this->object_write_callback != NULL || this->object_read_callback != NULL))
  {
    this->transfer_result = CANOPEN_TIMEOUT;
    if (this->transfer_direction == WRITE_SESSION &&
        this->object_write_callback != NULL)
    {
      this->object_write_callback( this->object_write_callback_context,
        this->transfer_result,
        this->node_id,
        this->application_object_index,
        this->application_sub_index,
        0);
    }
    else if (this->transfer_direction == READ_SESSION &&
             this->object_read_callback != NULL)
    {
      this->object_read_callback( this->object_read_callback_context,
        this->transfer_result,
        this->node_id,
        this->application_object_index,
        this->application_sub_index,
        NULL,
        0,
        0);
    }
    this->state = UNACTIVE;
    this->is_transfer_stopped_or_finished = TRUE;
  }
  return ret;
}

//------------------------------------------------------------------------
//  U8 write to remote node.
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: objectWrite(u16 object_index, u8 sub_index, 
  u8 val, CanOpenErrorCode *error_code)
{
  DebugLogToFile("objectWrite(u16 object_index, u8 sub_index, u8 val, CanOpenErrorCode *error_code) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  if (this->object_write_callback != NULL)
  {
    ret = CANOPEN_ERROR_ASYNC_TRANSFER_ENABLED;
  }
  else
  {
    ret = this->objectWrite(object_index, sub_index, &val, 1, error_code);
  }
  if (ret != CANOPEN_OK)
  {
    DebugLogToFile("objectWrite (u8) failed\n");
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
// U16 write to remote node.
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: objectWrite(u16 object_index, u8 sub_index, 
  u16 val, CanOpenErrorCode *error_code)
{
  DebugLogToFile("objectWrite(u16 object_index, u8 sub_index, u16 val, CanOpenErrorCode *error_code) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  u8 u16Buf[2];
  ::setU16Val(val, u16Buf, 0);

  if (this->object_write_callback != NULL)
  {
    ret = CANOPEN_ERROR_ASYNC_TRANSFER_ENABLED;
  }
  else
  {
    ret = this->objectWrite(object_index, sub_index, u16Buf, 2, error_code);
  }
  if (ret != CANOPEN_OK)
  {
    DebugLogToFile("objectWrite (u16) failed\n");
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
// U32 write to remote node.
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: objectWrite(u16 object_index, u8 sub_index, 
  u32 val, CanOpenErrorCode *error_code)
{
  DebugLogToFile("objectWrite(u16 object_index, u8 sub_index, u32 val, CanOpenErrorCode *error_code) entered\n");
  canOpenStatus ret = CANOPEN_ERROR;
  u8 u32Buf[4];
  ::setU32Val(val, u32Buf, 0);

  if (this->object_write_callback != NULL)
  {
    ret = CANOPEN_ERROR_ASYNC_TRANSFER_ENABLED;
  }
  else
  {
    ret = this->objectWrite(object_index, sub_index, u32Buf, 4, error_code);
  }
  if (ret != CANOPEN_OK)
  {
    DebugLogToFile("objectWrite (u32) failed\n");
  }
  return DebugExitErrorValueLogToFile(ret);
}

// 
//  U64 write to remote node.
//
#ifdef HAS_LONG_LONG
canOpenStatus  ClientSDO :: objectWrite(u16 object_index, u8 sub_index, u64 val, CanOpenErrorCode *error_code)
{
  u8 u64Buf[8];
  ::setU64Val(val, u64Buf, 0);
  return this->objectWrite(object_index, sub_index, u64Buf, 8, error_code);
}
#endif

//------------------------------------------------------------------------
//  This metod uses block transfer protocol.
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: objectWriteBlock(u16 object_index, u8 sub_index, 
  u16 crc, u8 *buf, u32 valid, CanOpenErrorCode *error_code)
{
  DebugLogToFile("objectWriteBlock(u16 object_index, u8 sub_index, u16 crc, u8 *buf, u32 valid, CanOpenErrorCode *error_code) entered\n");
  canOpenStatus ret                     = CANOPEN_ERROR;
  this->application_object_index        = object_index;
  this->application_sub_index           = sub_index;
  this->application_buffer              = buf;
  this->application_buffer_length       = valid;
  this->application_buffer_offset       = 0;
  this->application_canopen_error_code  = error_code;
  this->crc                             = crc;
  this->remote_node_error_code          = 0;
  *error_code                           = 0;

  ret = setCliCmdSpcInitBlockDnReq(crc ? 1 : 0, 1);
  if (ret == CANOPEN_OK)
  {
    ret = this->setMultiplexor(object_index, sub_index);
    if (ret == CANOPEN_OK)
    {
      u32 val = ::buf2val(buf, (u8)valid);
      ret = this->setData((u32)valid, 4, NULL); // The "data" is the size of the object in this case.
      if (ret == CANOPEN_OK)
      {
        this->state = WAIT_INITIATE_DOWNLOAD_BLOCK_RESPONSE;
        this->is_transfer_stopped_or_finished = FALSE;
        ret = this->synchronize(this->write_object_timeout, WRITE_SESSION);
      }
    }
  }
  if (ret != CANOPEN_OK)
  {
    DebugLogToFile("objectWriteBlock failed\n");
  }
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
// Any opbject type write to remote node.
//------------------------------------------------------------------------
canOpenStatus  ClientSDO :: objectWrite(u16 object_index, u8 sub_index, 
  u8 *buf, u32 valid, CanOpenErrorCode *error_code)
{
  WaitForSingleObject(rx_tx_mutex, INFINITE);
  DebugLogToFile("objectWrite(u16 object_index, u8 sub_index, u8 *buf, u32 valid, CanOpenErrorCode *error_code) entered\n");
  canOpenStatus ret = CANOPEN_ERROR; 

  this->application_object_index        = object_index;
  this->application_sub_index           = sub_index;
  this->application_buffer              = buf;
  this->application_buffer_length       = valid;
  this->application_buffer_offset       = 0;
  this->application_canopen_error_code  = error_code;
  this->remote_node_error_code          = 0;

  if (valid <= 4)
  {
    // ########################
    // ## EXPEDITED TRANSFER ##
    // ########################
    ret = this->setCliCmdSpcInitDnReq((u8)valid, 1, 1); // 100620: Indicate that the size is given.
    if (ret == CANOPEN_OK)
    {
      ret = this->setMultiplexor(object_index, sub_index);
      if (ret == CANOPEN_OK)
      {
        u8 n;
        u32 val = ::buf2val(buf, (u8)valid);
        ret = this->setData(val, (u8)valid, &n);
      }
    }
  }
  else
  {
    // ########################
    // ## SEGMENTED TRANSFER ##
    // ########################
    ret = this->setCliCmdSpcInitDnReq(4, 0, 1); // valid=4, e=0, s=1;
    if (ret == CANOPEN_OK)
    {
      ret = this->setMultiplexor(object_index, sub_index);
      if (ret == CANOPEN_OK)
      {
        ret = this->setData((u32)valid, 4, NULL); // The "data" is the size of the object in this case.
      }
    }
  }
  if (ret == CANOPEN_OK)
  {
    this->state = WAIT_INITIATE_DOWNLOAD_RESPONSE;
    this->is_transfer_stopped_or_finished = FALSE;
    ret = this->synchronize(this->write_object_timeout, WRITE_SESSION);
  }
  ReleaseMutex(this->rx_tx_mutex);
  return DebugExitErrorValueLogToFile(ret);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus   ClientSDO :: synchronize(unsigned long timeout, Direction direction)
{
  DebugLogToFile("synchronize(unsigned long timeout, Direction direction) entered\n");
  unsigned long startTransfertime_stamp = TimeClass ::readTimer();
  unsigned long now = startTransfertime_stamp ;
  bool transfer_timeout = FALSE;
  bool remote_aborted = FALSE;
  this->operation_timeout = timeout;
  this->transfer_direction = direction;

  //this->is_transfer_stopped_or_finished = FALSE;
  this->setLatestEventTimestamp(); // Init.

  if (this->is_transfer_stopped_or_finished == FALSE)
  {
    this->transfer_result = this->writeToCanBus();
    if (this->transfer_result != CANOPEN_OK)
    {
      DebugLogToFile("Method writeToCanBus did not return CANOPEN_OK\n");
      return this->transfer_result;
    }
  }
  
  while ( ((direction == WRITE_SESSION && object_write_callback == NULL) || // Do not sync if callback configured.
           (direction == READ_SESSION && object_read_callback == NULL)) &&  // Do not sync if callback configured.
          this->is_transfer_stopped_or_finished == FALSE && 
          transfer_timeout == FALSE && 
          remote_aborted == FALSE) 
  {

    if (this->isTransferTimeout())
    {
      DebugLogToFile("synchronize timeout\n");
      this->transfer_result = CANOPEN_TIMEOUT; 
      transfer_timeout = TRUE;
    }

    if (remote_node_error_code != 0)
    {
      DebugLogToFile("synchronize failed #1\n");
      remote_aborted = true;
      this->transfer_result = CANOPEN_REMOTE_NODE_ABORT;
    }
    *(this->application_canopen_error_code) = remote_node_error_code;
    Sleep(1);
  }

  if ((direction == WRITE_SESSION && object_write_callback != NULL) || // Do not sync if callback configured.
      (direction == READ_SESSION && object_read_callback != NULL))
  {
    DebugLogToFile("synchronize CANOPEN_ASYNC_TRANSFER\n");
    this->transfer_result = CANOPEN_ASYNC_TRANSFER;
  }

  DebugLogToFile("synchronize(unsigned long timeout, Direction direction) exit\n");
  return DebugExitErrorValueLogToFile(this->transfer_result);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

bool ClientSDO :: isTransferTimeout(void)
{
  bool transfer_timeout = FALSE;
  unsigned long now = TimeClass::readTimer();

  unsigned long latestTimeStamp = getLatestEventTimestamp();

  if (now > (latestTimeStamp + this->operation_timeout))
  {
    this->transfer_result = CANOPEN_TIMEOUT; 
    transfer_timeout = TRUE;
  }
  if (now > (latestTimeStamp + this->node_response_timeout))
  {
    if (this->state != WAIT_DOWNLOAD_BLOCK_END)
    {
      this->transfer_result = CANOPEN_TIMEOUT; 
      transfer_timeout = TRUE;
    }
  }
  return transfer_timeout;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus ClientSDO :: canHardwareConnect(u8 port, u32 bitrate)
{
  /*
  MessageBox(NULL, "This application is using a unregistred (demo) version of CANopenLib.dll \
           \n\nContact info@canopen.nu for more information.", 
           "Demo version of CANopenLib.DLL in use.", MB_OK);
  */
  return SDO :: canHardwareInit(
    port, 
    bitrate, 
    (DispatcherCanFuncPtr)canFrameConsumerW, 
    (ProtocolImplementationStateMachineFuncPtr) 
    clientSDOStateMachineW);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus ClientSDO ::  registerObjectReadResultCallback( CliReadFuncPtr function_pointer, void *context)
{
  DebugLogToFile(" registerObjectReadResultCallback entered\n");
  this->object_read_callback = function_pointer;
  this->object_read_callback_context = context;
  return CANOPEN_OK;
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus ClientSDO :: registerObjectWriteResultCallback(CliWriteFuncPtr function_pointer, void *context)
{
  DebugLogToFile("registerObjectReadResultCallback( CliReadFuncPtr function_pointer, void *context) entered\n");
  this->object_write_callback = function_pointer;
  this->object_write_callback_context = context;
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus ClientSDO :: unregisterObjectReadWriteResultCallbacks(void)
{
  DebugLogToFile("unregisterObjectReadWriteResultCallbacks(void) entered\n");
  this->object_write_callback = NULL;
  this->object_write_callback_context = NULL;
  this->object_read_callback = NULL;
  this->object_read_callback_context = NULL;
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

bool ClientSDO :: isObjectWriteResultCallbackEnabled(void)
{
  DebugLogToFile("isObjectWriteResultCallbackEnabled(void) entered\n");
  bool ret = false;
  if (this->object_write_callback != NULL)
  {
    ret = true;
  }
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

bool ClientSDO :: isObjectReadResultCallbackEnabled(void)
{
  DebugLogToFile("isObjectReadResultCallbackEnabled(void) entered\n");
  bool ret = false;
  if (this->object_read_callback != NULL)
  {
    ret = true;
  }
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

void ClientSDO :: setWriteObjectTimeout(unsigned int timeout)
{
   this->write_object_timeout = timeout;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

void ClientSDO :: setReadObjectTimeout(unsigned int timeout)
{
   this->read_object_timeout = timeout;
}

 //------------------------------------------------------------------------
//
//------------------------------------------------------------------------

void ClientSDO :: setNodeResponseTimeout(unsigned int timeout)
{
  this->node_response_timeout = timeout;
}

