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

#ifndef LSS_SLAVE_CLASS_H
#define LSS_SLAVE_CLASS_H

#include "CANopenLibDefines.h"
#include "CanInterfaceClass.h"
#include "TimerClass.h"
#include "CanConnectionClass.h"
#include "LSSDefines.h"


typedef void (*LSSReqestFunPtr)(void *context, u8 state );
typedef void (*LSSConfigureNodeId) (void *context, u8 nodeId, u8* errorCode, u8* specError);
typedef void (*LSSConfigureBitTimingParamters) (void *context, u8 tableDelector, u8 tableIndex, u8* errorCode, u8* specError);
typedef void (*LSSActivateBitTimingParameters) (void *context, u16 switchDelay);
typedef void (*LSSStoreConfiguration) (void *context, u8* errorCode, u8* specError);

#define   LSS_CMD_SWITCH_STATE_GLOBAL 0x04
#define   LSS_CMD_SWITCH_SELECTIVE_VENDOR 64
#define   LSS_CMD_SWITCH_SELECTIVE_PRODUCT 65
#define   LSS_CMD_SWITCH_SELECTIVE_REVISION 66
#define   LSS_CMD_SWITCH_SELECTIVE_SERIAL 67
#define   LSS_CMD_SWITCH_SELECTIVE_CONFIRMATION 68

#define   LSS_CMD_CONFIGURE_NODE_ID 17
#define   LSS_CMD_CONFIGURE_TIMING_PARAMETERS 19
#define   LSS_CMD_ACTIVATE_TIMING_PARAMETERS 21
#define   LSS_CMD_STORE_CONFIGURATION 23

#define   LSS_MODE_WAITING_STATE 0
#define   LSS_MODE_CONFIGURATION_STATE 1


//
//  LSS Slave.
//
class LSSSlave : public CanConnection
{
public:
  CANOPENDLL_API LSSSlave(void);
  CANOPENDLL_API ~LSSSlave(void);
  CANOPENDLL_API canOpenStatus  canHardwareConnect(u8 port, u32 btr);
  CANOPENDLL_API canOpenStatus  registerLocalNodeStateChangeCallback( 
    LSSReqestFunPtr lss_callback_function, 
    void *context );

  CANOPENDLL_API canOpenStatus registerLocalNodeNewNodeIdRequest(
    LSSConfigureNodeId lss_configure_node_id,
    void *context);


  CANOPENDLL_API void setDeviceParameters(u32 vendorId, u32 productCode, u32 revisionNumber, u32 serialNumber);

  static canOpenStatus canFrameConsumerW(void *nmt_slave_context, 
    unsigned long id, unsigned char *data, unsigned int dlc, unsigned int flags);
  static canOpenStatus transferHelperW(void *nmt_slave_context);
  
  canOpenStatus  canFrameConsumer(unsigned long id, 
    unsigned char *data, unsigned int dlc, unsigned int flags);
  canOpenStatus  transferHelper(void);

protected:

private:
  LSSReqestFunPtr                       local_node_lss_callback;
  void                                  *local_lss_callback_context;

  LSSConfigureNodeId                    local_node_lss_configure_node_id_callback;
  void                                  *local_node_lss_configure_node_id_callback_context;

  LSSConfigureBitTimingParamters        local_node_lss_configure_bit_timing_parameters;
  void                                  *local_node_lss_configure_bit_timing_parameters_context;

  LSSActivateBitTimingParameters        local_node_lss_activate_bit_timing_paramters;
  void                                  *local_node_lss_activate_bit_timing_paramters_context;

  LSSStoreConfiguration                 local_node_lss_store_configuration;
  void                                  *local_node_lss_store_configuration_context;

  u8                                    lss_mode;
  u8                                    lss_selective_state;
  
  u8                                    bitRateTableSelector;
  u8                                    bitRateTableIndex;
  u8                                    bitRateError;
  u32                                   bitRate;
  u8                                    activeNodeId;
  
  u8                                    pendingBitRateTableSelector;
  u8                                    pendingBitRateTableIndex;
  u32                                   pendingBitRate;
  u8                                    pendingNodeId;

  u32                                   vendor_id;
  u32                                   product_code;
  u32                                   revision_number;
  u32                                   serial_number;
};


#endif //LSS_SLAVE_CLASS_H
