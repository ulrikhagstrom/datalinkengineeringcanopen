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

#ifndef CLIENT_SDO_CLASS_H
#define CLIENT_SDO_CLASS_H

#include <stdio.h>
#include "SDOClass.h"
#include "CANopenLibHelper.h"
#include "DCFFileClass.h"

#define WRITE_OBJ_TIMEOUT     200000
#define READ_OBJ_TIMEOUT      400000
#define TAIL_TO_TAIL_TIMEOUT  2000

typedef enum { 
  UNACTIVE                                = 0,
  WAIT_INITIATE_UPLOAD_RESPONSE           = 1,             
  WAIT_UPLOAD_SEGMENT_RESPONSE            = 2,
  WAIT_INITIATE_DOWNLOAD_RESPONSE         = 3,
  WAIT_INITIATE_DOWNLOAD_BLOCK_RESPONSE   = 4,
  WAIT_DOWNLOAD_SEGMENT_RESPONSE          = 5,
  SENDING_DOWNLOAD_BLOCK                  = 7,
  WAIT_DOWNLOAD_BLOCK_RESPONSE            = 8,
  WAIT_DOWNLOAD_BLOCK_END                 = 9
} ClientWaitStates;

typedef enum { 
  WRITE_SESSION                           = 0,
  READ_SESSION                            = 1
} Direction;

//
//
//
class ClientSDO : public SDO
{
  typedef void (*CliReadFuncPtr)(void *context, canOpenStatus status, u8 node_id, u16 object_index, 
    u8 sub_index, u8 *buffer, u32 valid, u32 co_error_code);
  typedef void (*CliWriteFuncPtr)(void *context, canOpenStatus status, u8 node_id, u16 object_index, 
    u8 sub_index, u32 co_error_code); 

  public:
    CANOPENDLL_API ClientSDO();
    CANOPENDLL_API ~ClientSDO();

    CANOPENDLL_API canOpenStatus  objectRead(
      u16 object_index, 
      u8 sub_index, 
      u8  *buf, 
      u32 buffer_size, 
      u32 *valid, 
      CanOpenErrorCode *error_code
      );    
    CANOPENDLL_API canOpenStatus  objectRead(u16 object_index, 
      u8 sub_index, u8  *value, CanOpenErrorCode *error_code);
    CANOPENDLL_API canOpenStatus  objectRead(u16 object_index, 
      u8 sub_index, u16 *value, CanOpenErrorCode *error_code);
    CANOPENDLL_API canOpenStatus  objectRead(u16 object_index, 
      u8 sub_index, u32 *value, CanOpenErrorCode *error_code);
#ifdef HAS_LONG_LONG
    CANOPENDLL_API canOpenStatus  objectRead(u16 object_index, 
      u8 sub_index, u64 *val, CanOpenErrorCode *error_code);
#endif
    CANOPENDLL_API canOpenStatus  objectWrite(u16 object_index, 
      u8 sub_index, u8 *buffer, u32 valid, CanOpenErrorCode *error_code);
    CANOPENDLL_API canOpenStatus  objectWriteBlock(u16 object_index, 
      u8 sub_index, u16 crc_checksum, u8 *buffer, u32 valid, CanOpenErrorCode *error_code);
    CANOPENDLL_API canOpenStatus  objectWrite(u16 object_index, 
      u8 sub_index, u8  value, CanOpenErrorCode *error_code);
    CANOPENDLL_API canOpenStatus  objectWrite(u16 object_index, 
      u8 sub_index, u16 value, CanOpenErrorCode *error_code);
    CANOPENDLL_API canOpenStatus  objectWrite(u16 object_index, 
      u8 sub_index, u32 value, CanOpenErrorCode *error_code);
#ifdef HAS_LONG_LONG
    CANOPENDLL_API canOpenStatus  objectWrite(u16 object_index, u8 sub_index, u64 val, CanOpenErrorCode *error_code);
#endif
    CANOPENDLL_API canOpenStatus  registerObjectReadResultCallback(
      CliReadFuncPtr function_pointer, void *context); // Plain C or static class member
    CANOPENDLL_API canOpenStatus  registerObjectWriteResultCallback(
      CliWriteFuncPtr function_pointer, void *context); // --
    CANOPENDLL_API canOpenStatus  unregisterObjectReadWriteResultCallbacks(void);

    CANOPENDLL_API bool isObjectWriteResultCallbackEnabled(void);
    CANOPENDLL_API bool isObjectReadResultCallbackEnabled(void);

    CANOPENDLL_API canOpenStatus  connect(u8 node_id);
    CANOPENDLL_API canOpenStatus  connect(COBID cobid_tx, COBID cobid_rx);
    CANOPENDLL_API canOpenStatus  canHardwareConnect(u8 port, u32 bitrate);
    CANOPENDLL_API void setWriteObjectTimeout(unsigned int timeout);
    CANOPENDLL_API void setReadObjectTimeout(unsigned int timeout);
    CANOPENDLL_API void setNodeResponseTimeout(unsigned int timeout);

    CANOPENDLL_API canOpenStatus sendConfigurationData(char *dcfFile, u16 *failing_object_index, u8 *failing_sub_index, CanOpenErrorCode *errCode);

  protected:
  private:
    CliReadFuncPtr      object_read_callback;      //Ptr to callback func.
    void                *object_read_callback_context;  //Ptr to addtional object.

    CliWriteFuncPtr     object_write_callback;
    void                *object_write_callback_context;

    unsigned long       operation_timeout;
    Direction           transfer_direction;

    static canOpenStatus  canFrameConsumerW(
      void *client_sdo_object, 
      unsigned long id, 
      unsigned char *data, 
      unsigned int dlc, 
      unsigned int flags);

    canOpenStatus  canFrameConsumer(
      unsigned long id, 
      unsigned char *data, 
      unsigned int dlc, 
      unsigned int flags);
    
    static canOpenStatus clientSDOStateMachineW(void *client_sdo_object);
    canOpenStatus  clientSDOStateMachine(void);
    canOpenStatus  synchronize(unsigned long timeout, Direction direction);
    bool isTransferTimeout(void);

    //
    //  Private functions.
    //
    canOpenStatus  setCliCmdSpcInitDnReq(u8 valid, u8 e, u8 s); //9.2.2.2.2 Initiate SDO Download Protocol
    canOpenStatus  setCliCmdSpcDnSegReq(u8 valid, u8 c);  //9.2.2.2.3 Download SDO Segment Protocol
    canOpenStatus  setCliCmdSpcInitUpReq(void);  //9.2.2.2.5 Initiate SDO Upload Protocol 
    canOpenStatus  setCliCmdSpcUpSegReq(void); //9.2.2.2.6 Upload SDO Segment Protocol
    canOpenStatus  setCliCmdSpcInitBlockDnReq(u8 cc, u8 s); //9.2.2.2.9 Initiate SDO Block Download Protocol
    canOpenStatus  setCliCmdSpcEndBlkDnReq(u8 n, u16 crc); // 9.2.2.2.11 End SDO Block Download Protocol

    canOpenStatus  getSrvCmdSpcInitDnResp(void); //9.2.2.2.2 Initiate SDO Download Protocol
    canOpenStatus  getSrvCmdSpcDnSegResp(u8 *t); //9.2.2.2.3 Download SDO Segment Protocol
    canOpenStatus  getSrvCmdSpcInitUpResp(u8 *n, u8 *e, u8 *s); //9.2.2.2.5 Initiate SDO Upload Protocol
    canOpenStatus  getSrvCmdSpcUpSegResp(u8 *t, u8 *n, u8 *c); //9.2.2.2.6 Upload SDO Segment Protocol
    canOpenStatus  getSrvCmdSpcInitBlockDnResp(u8 *cs); //9.2.2.2.9 Initiate SDO Block Download Protocol
    canOpenStatus  getSrvCmdSpcBlkDnResp(u8 *ss, u8 *ackseq, u8 *blksize); //9.2.2.2.10 Download SDO Block Segment Protocol
    canOpenStatus  getSrvCmdSpcEndBlkDnResp(void); // 9.2.2.2.11 End SDO Block Download Protocol

    u8             getSrvCmdSpc(void);

    canOpenStatus  hndlInitUpResp(void); // 9.2.2.2.5 Initiate SDO Upload Protocol
    canOpenStatus  hndlSegmUpResp(void); // 9.2.2.2.6 Upload SDO Segment Protocol
    canOpenStatus  hndlInitDlResp(void); // 9.2.2.2.2 Initiate SDO Download Protocol
    canOpenStatus  hndlSegDlResp(void); // 9.2.2.2.3 Download SDO Segment Protocol
    canOpenStatus  hndlInitBlkDlResp(void); // 9.2.2.2.9 Initiate SDO Block Download Protocol    
    canOpenStatus  hndlDlBlkResp(void); // 9.2.2.2.10 Download SDO Block Segment Protocol
    canOpenStatus  hndlEndBlkDnResp(void); // 9.2.2.2.11 End SDO Block Download Protocol


    CanOpenErrorCode    canopen_error_code; // back-ground thead CANopen error code.
     // back-ground thread API response.
    ClientWaitStates    state;
    bool                is_transfer_stopped_or_finished;
    unsigned int        write_object_timeout;
    unsigned int        read_object_timeout;
    unsigned int        node_response_timeout;

    // canFrameConsumer vars
    unsigned long   *rxId;
    unsigned char   *rxData;
    unsigned int    *rxDlc;
    unsigned int    *rxFlags;

    //SDO block transfer.
    u16     crc;
    u8      block_size; // Block size given by SDO server.

    HANDLE            sync_mutex;
};

#endif
