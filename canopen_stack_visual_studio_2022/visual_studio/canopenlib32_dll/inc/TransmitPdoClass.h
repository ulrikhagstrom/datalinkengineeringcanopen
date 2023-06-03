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

#ifndef TRANSMIT_PDO_CLASS_H
#define TRANSMIT_PDO_CLASS_H

#include "CANopenLibDefines.h"
#include "CanInterfaceClass.h"
#include "TimerClass.h"
#include "NMTSlaveClass.h"
#include "CanConnectionClass.h"

//typedef void (*NMTOperationalStateFunPtr)( void *context, u8 node_id, u8 state );

//
//  Transmit PDO.
//
class TransmitPDO : public CanConnection
{
  public:
    CANOPENDLL_API TransmitPDO(void);
    CANOPENDLL_API ~TransmitPDO(void);

    CANOPENDLL_API canOpenStatus  setData(COBID cobid, u8* data, u8 len);
    CANOPENDLL_API canOpenStatus  transmitPdo(void);
    CANOPENDLL_API canOpenStatus  periodicTransmission(bool value);

  protected:

  private:
    TimeClass *timer;

    COBID   tPdo_cobid;
    u8      tPdo_data[8];
    u8      tPdo_len;
    bool    tPdo_periodic_transmission;

    static void timerCallbackHandler(void *nmt_master_object);
    
    u8                remote_node_id;
    //
    COBID             cobid_tx;  /* COBID for TX:ing NMT msg. */
    u8                can_data_tx[8]; /* The can frame data for TX. */
    u8                can_data_length; /* msg length */

    //NMTOperationalStateFunPtr operational_state_callback;
    void              *context;

    int               periodic_timer_index;


};

#endif //TRANSMIT_PDO_CLASS_H
