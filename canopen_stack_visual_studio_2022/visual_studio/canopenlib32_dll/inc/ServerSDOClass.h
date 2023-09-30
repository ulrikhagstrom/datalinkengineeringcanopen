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

#ifndef SERVER_SDO_CLASS_H
#define SERVER_SDO_CLASS_H


#include "SDOClass.h"
#include "CANopenLibHelper.h"

class  ServerSDO : public SDO
{
  // Server SDO callback for implementing OD reads.
  typedef canOpenStatus(*SrvReadFuncPtr)(void* context, u16 object_index,
    u8 sub_index, u8* buffer, u32* valid, u32 buffer_size, u32* co_error_code);
  typedef canOpenStatus(*SrvWriteFuncPtr)(void* context, u16 object_index,
    u8 sub_index, u8* buffer, u32 valid, u32* co_error_code); //Write to srv's application.
  typedef canOpenStatus(*SrvGetAttrFuncPtr)(void* context, u16 object_index,
    u8 sub_index, u16* flags);

public:
  CANOPENDLL_API ServerSDO();
  CANOPENDLL_API ~ServerSDO();
  CANOPENDLL_API canOpenStatus  canHardwareConnect(u8 port, u32 bitrate);
  CANOPENDLL_API canOpenStatus  nodeSetId(u8 node_id);
  CANOPENDLL_API canOpenStatus  registerObjectReadCallback(
    SrvReadFuncPtr function_pointer, void* context); // Plain C or static class member
  CANOPENDLL_API canOpenStatus  registerObjectWriteCallback(
    SrvWriteFuncPtr function_pointer, void* context); // --
  CANOPENDLL_API canOpenStatus  registerObjectGetAttributesCallback(
    SrvGetAttrFuncPtr function_pointer, void* context);
protected:
private:
  ServerSdoState      state;
  SrvReadFuncPtr      object_read_callback;      //Ptr to callback func.
  void* object_read_callback_context;  //Ptr to addtional object.

  SrvWriteFuncPtr     object_write_callback;
  void* object_write_callback_context;

  SrvGetAttrFuncPtr   object_attributes_callback;
  void* object_attributes_callback_context;

  u8                  data_buffer[SDO_SERVER_BUFFER];
  u32                 data_buffer_counter;

  //
  //  Private functions.
  //
  static canOpenStatus  canFrameConsumerW(void* context, unsigned long id, unsigned char* data, unsigned int dlc, unsigned int flags);
  canOpenStatus  canFrameConsumer(unsigned long id, unsigned char* data, unsigned int dlc, unsigned int flags);

  static canOpenStatus  serverSDOStateMachineW(void* context);
  canOpenStatus  serverSDOStateMachine(void);

  static canOpenStatus  periodicTimerCallbackWrapper(void* context);
  canOpenStatus  periodicTimerCallback(void);

  canOpenStatus  registerPeriodicTimerCallback(void);

  canOpenStatus  getCliCmd(u8* cmd); // Collect client command specifier.
  canOpenStatus  getCliCmdSpcInitDnReq(u8* n, u8* e, u8* s); //9.2.2.2.2 Initiate SDO Download Protocol
  canOpenStatus  getCliCmdSpcDnSegReq(u8* t, u8* n, u8* c);  //9.2.2.2.3 Download SDO Segment Protocol
  canOpenStatus  getCliCmdSpcInitUpReq(void);  //9.2.2.2.5 Initiate SDO Upload Protocol 
  canOpenStatus  getCliCmdSpcUpSegReq(u8* t); //9.2.2.2.6 Upload SDO Segment Protocol
  canOpenStatus  getCliCmdSpcAbrtTrnsf(void); //9.2.2.2.7 Abort SDO Transfer Protocol
  canOpenStatus  getCliCmdSpcBlkDn(u8* cc, u8* s, u8* cs); //9.2.2.2.9 Initiate SDO Block Download Protocol
  canOpenStatus  getCliCmdSpcEndBlkDnReq(u8* n, u16* crc);  // 9.2.2.2.11 End SDO Block Download Protocol

  canOpenStatus  setSrvCmdSpcInitDnResp(void); //9.2.2.2.2 Initiate SDO Download Protocol
  canOpenStatus  setSrvCmdSpcDnSegResp(u8 t); //9.2.2.2.3 Download SDO Segment Protocol
  canOpenStatus  setSrvCmdSpcInitUpResp(u8 valid, u8 e, u8 s); //9.2.2.2.5 Initiate SDO Upload Protocol
  canOpenStatus  setSrvCmdSpcUpSegResp(u8 t, u8 n, u8 c); //9.2.2.2.6 Upload SDO Segment Protocol
  canOpenStatus  setSrvCmdSpcInitBlockDnResp(u8 sc); //9.2.2.2.9 Initiate SDO Block Download Protocol
  canOpenStatus  setSrvCmdSpcBlkDnResp(u8 ss, u8 ackseq, u8 blksize); //9.2.2.2.10 Download SDO Block Segment Protocol
  canOpenStatus  setSrvCmdSpcEndBlkDnResp(void); //9.2.2.2.11 End SDO Block Download Protocol

  canOpenStatus  hndlCliRequest(void);
  canOpenStatus  hndlDlSegReq(void);
  canOpenStatus  hndlDlBlkSegReq(void); // 9.2.2.2.10 Download SDO Block Segment Protocol
  canOpenStatus  hndlEndBlkDnReq(void); // 9.2.2.2.11 End SDO Block Download Protocol
  canOpenStatus  hndlUpSegReq(void);
  canOpenStatus  hndlInitDlReq(void);
  canOpenStatus  hndlInitUpReq(void);
  canOpenStatus  hndlBlkDl(void);  //9.2.2.2.9 Initiate SDO Block Download Protocol
};
#endif

