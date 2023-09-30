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

#ifndef CAN_MONITOR_CLASS_H
#define CAN_MONITOR_CLASS_H


#include "CANopenLibHelper.h"
#include "CanInterfaceClass.h"
#include "CanConnectionClass.h"


typedef void (*RawCanReceiveFunPtr)(void* context, u32 id, u8* data, u8 dlc, u32 flags);

class CanMonitor : public CanConnection
{
public:
  CANOPENDLL_API CanMonitor();
  CANOPENDLL_API ~CanMonitor();

  CANOPENDLL_API canOpenStatus  canHardwareConnect(u8 port, u32 bitrate);
  CANOPENDLL_API canOpenStatus  registerCanReceiveCallback(void* context, RawCanReceiveFunPtr raw_can_receive_function);
  CANOPENDLL_API canOpenStatus  canWrite(u32 id, u8* data, u8 dlc, u32 flags);

protected:
private:

  static canOpenStatus canFrameConsumerW(void* can_monitor_object, unsigned long id, unsigned char* data, unsigned int  dlc, unsigned int flags);

  canOpenStatus  canFrameConsumer(
    u32 id,
    u8* data,
    u8 dlc,
    u32 flags);


  RawCanReceiveFunPtr     application_can_receive_callback;
  void* application_context;

  HANDLE            rx_tx_mutex;
};

#endif
