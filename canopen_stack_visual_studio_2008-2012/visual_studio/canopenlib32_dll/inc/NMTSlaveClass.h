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

#ifndef NMT_SLAVE_CLASS_H
#define NMT_SLAVE_CLASS_H

#include "CANopenLibDefines.h"
#include "CanInterfaceClass.h"
#include "TimerClass.h"
#include "CanConnectionClass.h"


typedef void (*NMTLocalNodeOperationalStateFunPtr)( void *context, u8 state );

//
//  Network management master.
//
class NMTSlave : public CanConnection
{
public:
  CANOPENDLL_API NMTSlave(void);
  CANOPENDLL_API ~NMTSlave(void);
  CANOPENDLL_API canOpenStatus  canHardwareConnect(u8 port, u32 btr);
  CANOPENDLL_API canOpenStatus  nodeSetId( u8 node_id );
  CANOPENDLL_API canOpenStatus  nodeSetState( u8 node_state );
  CANOPENDLL_API canOpenStatus writeMessage(long id, void* msg, unsigned int dlc, unsigned int flags);
  CANOPENDLL_API canOpenStatus  registerLocalNodeStateChangeCallback( 
    NMTLocalNodeOperationalStateFunPtr state_change_callback_function, 
    void *context );
  CANOPENDLL_API u8 getNodeState(void);

  static canOpenStatus canFrameConsumerW(void *nmt_slave_context, 
    unsigned long id, unsigned char *data, unsigned int dlc, unsigned int flags);
  static canOpenStatus transferHelperW(void *nmt_slave_context);
  
  canOpenStatus  canFrameConsumer(unsigned long id, 
    unsigned char *data, unsigned int dlc, unsigned int flags);
  canOpenStatus  transferHelper(void);

protected:

private:
  u8                                    node_state;
  u8                                    node_id;
  u8                                    toggle;

  NMTLocalNodeOperationalStateFunPtr    local_node_operational_state_change_callback;
  void                                  *local_node_operational_state_change_callback_context;

};


#endif //NMT_SLAVE_CLASS_H
