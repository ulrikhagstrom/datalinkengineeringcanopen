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

#ifndef NMT_MASTER_CLASS_H
#define NMT_MASTER_CLASS_H

#include "CANopenLibDefines.h"
#include "CanInterfaceClass.h"
#include "TimerClass.h"
#include "NMTSlaveClass.h"

typedef void (*NMTOperationalStateFunPtr)(void* context, u8 node_id, u8 state);

//
// 9.2.6.2.1 Module Control Protocols, p.9.44 - p.9.46.
//
typedef enum {
  START_CMD = 1,  /* Start node. */
  STOP_CMD = 2,  /* Stop node. */
  PRE_OPERATIONAL_CMD = 128,  /* Set node in preoperational. */
  RESET_CMD = 129,  /* Reset node. */
  RESET_COMMUNCATION_CMD = 130,  /* Reset communcication. */
} SetNodeStateCmd;

//
// 9.2.6.2.2 Error Control Protocols, p.9.48.
//
typedef enum {
  BOOTUP_STATE = 0,   /* Start node. */
  STOPPED_STATE = 4,   /* Stop node. */
  OPERATIONAL_STATE = 5,   /* Set node in preoperational. */
  RESET_STATE = 127, /* Pre-Opretional */
  UNDEFINED = -1
} NodeState;

// Structure for keeping track on the 
// on the received heartbeats.
typedef struct {
  NodeState       node_state;
  unsigned long   time_stamp;
  bool            node_guard_poll;
  bool            error_control;
} NetworkNodeData;


//
//  Network management master.
//
class NMTMaster : public NMTSlave
{
public:
  CANOPENDLL_API NMTMaster(void);
  CANOPENDLL_API ~NMTMaster(void);
  CANOPENDLL_API canOpenStatus  canHardwareConnect(u8 port, u32 btr);

  CANOPENDLL_API canOpenStatus  connect(u8 node_id);
  CANOPENDLL_API canOpenStatus  nodeStart(void);
  CANOPENDLL_API canOpenStatus  nodeStart(u8 node_id);
  CANOPENDLL_API canOpenStatus  nodeStop(void);
  CANOPENDLL_API canOpenStatus  nodeStop(u8 node_id);
  CANOPENDLL_API canOpenStatus  nodePreoperational(void);
  CANOPENDLL_API canOpenStatus  nodePreoperational(u8 node_id);
  CANOPENDLL_API canOpenStatus  nodeReset(void);
  CANOPENDLL_API canOpenStatus  nodeReset(u8 node_id);
  CANOPENDLL_API canOpenStatus  nodeResetCommunication(void);
  CANOPENDLL_API canOpenStatus  nodeResetCommunication(u8 node_id);
  CANOPENDLL_API canOpenStatus  nodeReadOperationalState(
    u8 node_id,
    unsigned long maxMsTimeout,
    NodeState* node_stateP);
  CANOPENDLL_API canOpenStatus  nodeGuardPollStart(u8 node_id,
    unsigned long node_life_time_ms);
  CANOPENDLL_API canOpenStatus  nodeGuardPollStop(u8 node_id);
  CANOPENDLL_API canOpenStatus  heartbeatMonitorStart(u8 node_id,
    unsigned long heartbeat_consumer_time_ms);
  CANOPENDLL_API canOpenStatus  heartbeatMonitorStop(u8 node_id);
  CANOPENDLL_API canOpenStatus  registerRemoteNodeStateCallback(
    NMTOperationalStateFunPtr fp, void* context);

  canOpenStatus                 sendNodeGuardMessages(void);
  void                          checkHeartbeatEvent(void);

protected:

private:
  TimeClass* timer;
  static canOpenStatus canFrameConsumerW(
    void* nmt_master_object,
    unsigned long id,
    unsigned char* data,
    unsigned int dlc,
    unsigned int flags);

  static canOpenStatus transferHelperW(void* nmt_master_object);

  static void timerCallbackHandler(void* nmt_master_object);

  canOpenStatus  canFrameConsumer(
    unsigned long id,
    unsigned char* data,
    unsigned int dlc,
    unsigned int flags);
  canOpenStatus  transferHelper(void);
  canOpenStatus  nodeSetState(SetNodeStateCmd node_state);
  //
  //
  //
  NetworkNodeData   nodeList[127];
  u8                remote_node_id;
  //
  COBID             cobid_tx;  /* COBID for TX:ing NMT msg. */
  u8                can_data_tx[8]; /* The can frame data for TX. */
  u8                can_data_length; /* msg length */

  NMTOperationalStateFunPtr operational_state_callback;
  void* context;

  int               periodic_timer_index;


};

#endif //NMT_MASTER_CLASS_H
