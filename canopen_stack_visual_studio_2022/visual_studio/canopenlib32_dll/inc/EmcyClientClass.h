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

#ifndef EMCY_CLIENT_CLASS_H
#define EMCY_CLIENT_CLASS_H

#include "CANopenLibDefines.h"
#include "CanInterfaceClass.h"
#include "TimerClass.h"
#include "CanConnectionClass.h"


typedef void (*NMTLocalNodeOperationalStateFunPtr)(void* context, u8 state);

//
//  Network management master.
//
class EmcyClient : public CanConnection
{
public:
  CANOPENDLL_API EmcyClient(void);
  CANOPENDLL_API ~EmcyClient(void);

  CANOPENDLL_API canOpenStatus  nodeSetId(u8 node_id);
  CANOPENDLL_API canOpenStatus  sendEmcyMessage(u8 nodeId, u16 emcy_error_code, u8 error_register, u8* manufSpecificErrorField);
  CANOPENDLL_API canOpenStatus  sendEmcyMessage(u16 emcy_error_code, u8 error_register, u8* manufSpecificErrorField);

  static canOpenStatus canFrameConsumerW(void* nmt_slave_context,
    unsigned long id, unsigned char* data, unsigned int dlc, unsigned int flags);
  static canOpenStatus transferHelperW(void* nmt_slave_context);

  canOpenStatus  canFrameConsumer(unsigned long id,
    unsigned char* data, unsigned int dlc, unsigned int flags);
  canOpenStatus  transferHelper(void);

protected:

private:
  u8                                    node_id;
};


#endif //EMCY_CLIENT_CLASS_H
