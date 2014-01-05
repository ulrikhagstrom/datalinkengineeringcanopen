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
#include "SDOClass.h"

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
SDO :: SDO()
{
  this->can_interface = NULL;
  this->can_hardware_is_initiated = false;
  this->can_message_handler_index = -1;
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
SDO :: ~SDO()
{
  (void)this->canHardwareDisconnect();
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
canOpenStatus  SDO :: writeToCanBus(void)
{
  canOpenStatus ret = CANOPEN_ERROR_HW_NOT_CONNECTED;
  if ( can_interface != NULL )
  {
    ret = can_interface->canWrite(this->cobid_tx, this->can_data_tx, 8, 0); 
    if (ret != CANOPEN_OK)
    {
      DebugLogToFile("writeToCanBus failed\n");
    }
  }
  return ret;
}

//------------------------------------------------------------------------
// Method will toggle the bit that is being used in segmented transfers.
//------------------------------------------------------------------------
void SDO :: toggle(void)
{
  if (this->t)
  {
    this->t = 0;
  }
  else
  {
    this->t = 1;
  }
}

//------------------------------------------------------------------------
//9.2.2.2.2 Initiate SDO Download Protocol 
//9.2.2.2.5 Initiate SDO Upload Protocol
//9.2.2.2.7 Abort SDO Transfer Protocol
//------------------------------------------------------------------------
canOpenStatus SDO ::  setMultiplexor(u16 object_index, u8 sub_index)
{
  ::setU16Val(object_index, this->can_data_tx, 1);
  ::setU8Val(sub_index, this->can_data_tx, 3);
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
//9.2.2.2.2 Initiate SDO Download Protocol 
//9.2.2.2.5 Initiate SDO Upload Protocol
//9.2.2.2.7 Abort SDO Transfer Protocol
//------------------------------------------------------------------------
canOpenStatus SDO ::  getMultiplexor(u16 *object_index, u8 *sub_index)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (object_index != NULL && sub_index != NULL)
  {
    *object_index = ::getU16Val(this->can_data_rx, 1);
    *sub_index = ::getU8Val(this->can_data_rx, 3);
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.2 Initiate SDO Download Protocol
//9.2.2.2.5 Initiate SDO Upload Protocol
//9.2.2.2.7 Abort SDO Transfer Protocol
//------------------------------------------------------------------------
canOpenStatus  SDO ::  setData(u32 val, u8 valid, u8 *n)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (valid > 0 && valid <= 4)
  {
    if (valid == 1)
    {
      ::setU8Val((u8)val, this->can_data_tx, 4);  
    }
    else if (valid == 2)
    {
      ::setU16Val((u16)val, this->can_data_tx, 4);
    }
    else if (valid == 3)
    {
      ::setU24Val(val, this->can_data_tx, 4);
    }
    else
    {
      ::setU32Val(val, this->can_data_tx, 4);
    }
    if (n != NULL)
    {
      *n = (4 - valid);
    }
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.2 Initiate SDO Download Protocol
//9.2.2.2.5 Initiate SDO Upload Protocol
//9.2.2.2.7 Abort SDO Transfer Protocol
//------------------------------------------------------------------------
canOpenStatus  SDO ::  getData(u32 *val, u8 *valid, u8 n)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 lclValid = 4 - n;
  if (lclValid > 0 && lclValid <= 4)
  {
    if (lclValid == 1)
    {
      *val = ::getU8Val(this->can_data_rx, 4);
    }
    else if (lclValid == 2)
    {
      *val = ::getU16Val(this->can_data_rx, 4);
    }
    else if (lclValid == 3)
    {
      *val = ::getU24Val(this->can_data_rx, 4);
    }
    else
    {
      *val = ::getU32Val(this->can_data_rx, 4);
    }
    if (valid != NULL)
    {
      *valid = lclValid;
    }
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.3 Download SDO Segment Protocol
//9.2.2.2.6 Upload SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  SDO :: setSegData(u8 *buf, u8 valid)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (valid <= 7)
  {
    memset(&(this->can_data_tx[1]), 0, 7);
    memcpy(&(this->can_data_tx[1]), buf, valid);
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.3 Download SDO Segment Protocol
//9.2.2.2.6 Upload SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus SDO ::setSegData(u8 valid)
{
  canOpenStatus ret = CANOPEN_ERROR;
  ret = this->setSegData(this->application_buffer + this->application_buffer_offset, valid);
  this->application_buffer_offset += valid;
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.10 Download SDO Block Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  SDO ::setBlkSegData(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u32 remaining_in_buffer = this->getRestAppBuffer();
  u8 bytes_to_be_sent_in_segment = 0;
  u8 c = 0;
  if (remaining_in_buffer > 7)
  {
    bytes_to_be_sent_in_segment = 7;
    c = 0;  // Bug-fix 100618
  }
  else
  {
    bytes_to_be_sent_in_segment = (u8)remaining_in_buffer;
    c = 1; // Bug-fix 100618
    this->valid_bytes_in_last_segment_in_last_block = (u8)remaining_in_buffer;
  }
  ret = setBlkSegData(bytes_to_be_sent_in_segment, c);
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.10 Download SDO Block Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  SDO ::setBlkSegData(u8 bytes_in_segment, u8 c)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if (c <= 1)
  {
    this->can_data_tx[0] = ((c << 7) | (block_transfer_segment_count)); // 110301
    ret = this->setSegData(this->application_buffer + this->application_buffer_offset, 
      bytes_in_segment);
    if (ret == CANOPEN_OK)
    {
      this->application_buffer_offset += bytes_in_segment;
      if (c == 0) // more segments to be downloaded
      {
        // Only increment the counter if we are expected to send more, this
        // is because when the server is acknowledgeing a block (after client
        // has sent the c == 0 etc) it will send the latest properly
        // received messge back to us.
        this->block_transfer_segment_count++;
      }
      else
      {
        this->block_transfer_segment_count++;
      }
    }
  }
  return ret;
}

//------------------------------------------------------------------------
//9.2.2.2.10 Download SDO Block Segment Protocol
//------------------------------------------------------------------------
canOpenStatus   SDO :: getDlBlkSegHeader(u8 *c, u8 *seqNo)
{
  u8 header = this->can_data_rx[0];
  *c =      (header >> 7) & 1;
  *seqNo =  ((header & 0x7f));
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
//9.2.2.2.3 Download SDO Segment Protocol
//9.2.2.2.6 Upload SDO Segment Protocol
//------------------------------------------------------------------------
canOpenStatus  SDO :: getSegData(u8 *buf, u8 *valid, u8 n)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u8 lclValid;
  
  if (n == 0)
    lclValid = 7;
  else
    lclValid = 7 - n;
  if (lclValid <= 7)
  {
    memcpy(buf, &(this->can_data_rx[1]), lclValid);
    *valid = lclValid;
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
// Gets the number of bytes that have been unprocessed in the application's
// buffer.
//------------------------------------------------------------------------
u32  SDO ::  getRestAppBuffer(void)
{
  u32   ret = 0;
  int   intRet = 
    (int)this->application_buffer_length - (int)this->application_buffer_offset;
  if (intRet < 0)
  {
    ret = 0;
  }
  else 
  {
    ret = (u32)intRet;
  }
  return ret;
}

//------------------------------------------------------------------------
// Safe incrementation of the pointer keeping track of the pointer for
// the application buffer.
//------------------------------------------------------------------------
canOpenStatus SDO :: incrementApplicationBuffertOffset(u16 bytes)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if ((this->application_buffer_length - 
    (this->application_buffer_offset + bytes)) > 0)
  {
    this->application_buffer_offset += bytes;
    ret = CANOPEN_OK;
  }
  else
  {
    ret = CANOPEN_APP_BUF_OUT_OF_RANGE;
  }
  return ret;
}

//------------------------------------------------------------------------
// Safe incrementation of the pointer keeping track of the pointer for
// the application buffer.
//------------------------------------------------------------------------
canOpenStatus SDO :: decrementapplication_buffertr(u16 bytes)
{
  canOpenStatus ret = CANOPEN_ERROR;
  this->application_buffer_offset -= bytes;
  ret = CANOPEN_OK;
  return ret;
}


//------------------------------------------------------------------------
// This method copies the data received in frame to the local buffers for the specific SDO.
//------------------------------------------------------------------------
canOpenStatus  SDO :: getFrameData(unsigned long id, 
  unsigned char *data, unsigned int dlc)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if ((this->cobid_rx == (COBID)(id)) && dlc == 8) // All SDO frames have a DLC of 8.
  {
    memcpy(this->can_data_rx, data, 8);
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
// This method is being used for clearing up in the tx-buffer field.
//------------------------------------------------------------------------
canOpenStatus  SDO :: clrTxCanDataField(u8 from, u8 to)
{
  canOpenStatus ret = CANOPEN_ARG_ERROR;
  if (from >= 0 && from <= 7)
  {
    if (to >= 0 && to <= 7)
    {
      if (to > from)
      {
        (void)memset(&(can_data_tx[from]), 0, (to-from+1));
        ret = CANOPEN_OK;
      }
    }
  }
  return ret;
}

//------------------------------------------------------------------------
// This method sets a timer that is being used to identify a maximum time
// for two separate CAN frames. 
//------------------------------------------------------------------------
canOpenStatus SDO :: setLatestEventTimestamp(void)
{
  event_time_stamp = TimeClass::readTimer(); //canReadTimer(this->can_interface->canHandle);
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
// This method sets a timer that is being used to identify a maximum time
// for two separate CAN frames. 
//------------------------------------------------------------------------
unsigned long SDO :: getLatestEventTimestamp(void)
{
  return event_time_stamp;
}

//------------------------------------------------------------------------
// This method set the abort transfer command specifier.
//------------------------------------------------------------------------
canOpenStatus  SDO ::  setCliSrvCmdSpcAbrtTrnsf(void)
{
  canOpenStatus ret = CANOPEN_ERROR;  
  this->can_data_tx[0] = (4 << 5); // ccs == 4.
  ret = CANOPEN_OK;    
  return ret;
}

//------------------------------------------------------------------------
// This method identifies the received command and tells 
// if it is a abort transfer command specifier. 
//------------------------------------------------------------------------
canOpenStatus  SDO ::  getCliSrvCmdSpcAbrtTrnsf(void)
{
  canOpenStatus ret = CANOPEN_ERROR;  
  if ((this->can_data_rx[0] >> 5) == 4) // ccs == 4.
  {
    ret = CANOPEN_OK;
  }
  return ret;
}

//------------------------------------------------------------------------
// This method creates a abort transfer message.
//------------------------------------------------------------------------
canOpenStatus SDO :: setCliSrvAbortTransfer(u16 object_index, u8 sub_index, 
                                            u32 error_code)
{
  canOpenStatus ret = CANOPEN_ERROR;
  ret = setCliSrvCmdSpcAbrtTrnsf();
  if (ret == CANOPEN_OK)
  {
    ret = this->setMultiplexor(object_index, sub_index);
    if (ret == CANOPEN_OK)
    {
      ret = setData(error_code, 4, NULL);
    }
  }
  return ret;
}

//------------------------------------------------------------------------
// This method collects valuble info from the received message.
//------------------------------------------------------------------------
canOpenStatus SDO :: getCliSrvAbortTranfer(u16 *object_index, u8 *sub_index, 
                                           u32 *error_code)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u16 _object_index;
  u8  _sub_index;
  ret = getMultiplexor(&_object_index, &_sub_index);
  if (ret == CANOPEN_OK)
  {
     u32 _error_code; 
     u8 valid;
     ret = getData(&_error_code, &valid, 0);
     if (ret == CANOPEN_OK)
     {
        *object_index  = _object_index;
        *sub_index   = _sub_index;
        *error_code = _error_code; 
     }
  }
  return ret;
}

//------------------------------------------------------------------------
// This method handles the decoding of an abort transfer frame.
//------------------------------------------------------------------------
canOpenStatus SDO :: hndlRemoteAbort(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  u16 object_index;
  u8  sub_index;
  u32 error_code;

  ret = this->getCliSrvAbortTranfer(&object_index, &sub_index, &error_code);
  if (ret == CANOPEN_OK)
  {
    if (this->application_object_index == object_index && 
        this->application_sub_index == sub_index) 
    {
      this->remote_node_error_code = error_code;
    }
  }
  return ret;
}

//------------------------------------------------------------------------
// Handle reception of a invalid or unknown command specifier.
//------------------------------------------------------------------------
canOpenStatus SDO :: hndlInvalidCmdSpec(void)
{
  return this->setCliSrvAbortTransfer(this->application_object_index, 
    this->application_sub_index, COMMAND_SPECIFIER_UNKNOWN);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus SDO :: registerPeriodicTimerCallback( 
  TimeClass::TimeHandlerFuncPtr callback_function, void *context, 
  unsigned long period_time )
{
  canOpenStatus ret = CANOPEN_ERROR;
  TimeClass *tcP = TimeClass::getTimeInterface(); //get the singleton interface.
  if (tcP != NULL)
  {
    ret = tcP->registerPeriodicCallback(callback_function, context, 
      period_time, &periodic_timer_index);
  }
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus SDO :: unregisterPeriodicTimerCallback( void )
{
  canOpenStatus ret = CANOPEN_ERROR;
  TimeClass *timer_interface = TimeClass::getTimeInterface(); //get the singleton interface.
  if (timer_interface != NULL)
  {
    ret = timer_interface->unregisterPeriodicCallback( periodic_timer_index );
  }
  return ret;
}




