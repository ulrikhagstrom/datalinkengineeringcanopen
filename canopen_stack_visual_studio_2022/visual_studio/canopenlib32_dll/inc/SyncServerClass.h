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

#ifndef SYNC_SERVER_CLASS_H
#define SYNC_SERVER_CLASS_H

#include "CANopenLibDefines.h"
#include "CanInterfaceClass.h"
#include "CanConnectionClass.h"

typedef void (*SyncMessageFunPtr)(void* context);

//
//  Emergency Server implementation.
//
class SyncServer : public CanConnection
{
public:
  CANOPENDLL_API SyncServer(void);
  CANOPENDLL_API ~SyncServer(void);
  CANOPENDLL_API canOpenStatus  canHardwareConnect(u8 port, u32 btr);
  CANOPENDLL_API canOpenStatus  registerSyncMessageCallBack(
    void* context, SyncMessageFunPtr fp);
  CANOPENDLL_API canOpenStatus  reconfigureSyncCOBID(u32 COBID);

protected:
private:
  static canOpenStatus canFrameConsumerW(
    void* nmt_master_object,
    unsigned long id,
    unsigned char* data,
    unsigned int dlc,
    unsigned int flags);

  canOpenStatus  canHardwareInit(
    int port,
    int bitrate,
    DispatcherCanFuncPtr can_consumer_callback,
    ProtocolImplementationStateMachineFuncPtr protocol_helper_function);

  canOpenStatus  canFrameConsumer(
    unsigned long id,
    unsigned char* data,
    unsigned int dlc,
    unsigned int flags);

  SyncMessageFunPtr sync_messagae_callback;
  void* context;
  u32               COBID_Sync;
};

#endif //SYNC_SERVER_CLASS_H
