#ifndef CAN_CONNECTION_CLASS_H
#define CAN_CONNECTION_CLASS_H

#include "CANopenLibDefines.h"
#include "CanOpenLibHelper.h"
#include "CanInterfaceClass.h"

class CanConnection
{
public:
  CanConnection();
  CANOPENDLL_API canOpenStatus canHardwareConnect(u8 port, u32 bitrate);
  CANOPENDLL_API canOpenStatus canHardwareDisconnect(void);
  CANOPENDLL_API canOpenStatus canDispatcherPerformance(
    int sleepNoMessageFromCanInterface,
    int sleepProcessedCanInterface);
protected:
  canOpenStatus  canHardwareInit(int port, int bitrate,
    DispatcherCanFuncPtr can_frame_handler_function,
    ProtocolImplementationStateMachineFuncPtr transfer_protocol_handler_function);

  CanInterface* can_interface;
  int                              can_message_handler_index;
  bool                             can_hardware_is_initiated;
private:

};

#endif