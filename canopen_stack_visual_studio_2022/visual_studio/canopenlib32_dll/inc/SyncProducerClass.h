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

#ifndef SYNC_PRODUCER_CLASS_H
#define SYNC_PRODUCER_CLASS_H

#include "CANopenLibDefines.h"
#include "CanInterfaceClass.h"
#include "TimerClass.h"
#include "CanConnectionClass.h"
#include "PeriodicTransmitterClass.h"


//
//  Network management master.
//
class SyncProducer : public PeriodicTransmitter
{
public:
  CANOPENDLL_API SyncProducer(void);
  CANOPENDLL_API ~SyncProducer(void);
  CANOPENDLL_API canOpenStatus setSyncCOBID(COBID id);
  CANOPENDLL_API canOpenStatus startPeriodicTransmission(bool enabled);
};

#endif //SYNC_PRODUCER_CLASS_H
