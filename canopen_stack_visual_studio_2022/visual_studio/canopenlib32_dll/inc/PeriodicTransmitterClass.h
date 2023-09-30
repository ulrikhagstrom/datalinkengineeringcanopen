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

#ifndef PERIODIC_TRANSMITTER_CLASS_H
#define PERIODIC_TRANSMITTER_CLASS_H

#include "CANopenLibDefines.h"
#include "CanInterfaceClass.h"
#include "TimerClass.h"
#include "NMTSlaveClass.h"
#include "CanConnectionClass.h"

//typedef void (*NMTOperationalStateFunPtr)( void *context, u8 node_id, u8 state );

//
//  Transmit PDO.
//
class PeriodicTransmitter : public CanConnection
{
public:
  CANOPENDLL_API canOpenStatus  setTransmissionPeriodTime(unsigned long sync_period_time);

protected:
  PeriodicTransmitter(void);
  ~PeriodicTransmitter(void);
  canOpenStatus  setData(COBID cobid, u8* data, u8 len);
  canOpenStatus  transmitMsg(void);
  canOpenStatus  periodicTransmission(bool value);

private:



  TimeClass* timer;

  COBID         tMsg_cobid;
  u8            tMsg_data[8];
  u8            tMsg_len;
  bool          tMsg_periodic_transmission;
  unsigned long tMsg_period_time;
  unsigned long tMsg_time_stamp_last_tx;

  static void timerCallbackHandler(void* nmt_master_object);

  u8                remote_node_id;
  //
  COBID             cobid_tx;  /* COBID for TX:ing NMT msg. */
  u8                can_data_tx[8]; /* The can frame data for TX. */
  u8                can_data_length; /* msg length */

  void* context;

  int               periodic_timer_index;
};

#endif //PERIODIC_TRANSMITTER_CLASS_H
