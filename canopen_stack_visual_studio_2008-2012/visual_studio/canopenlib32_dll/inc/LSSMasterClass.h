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

#ifndef LSS_MASTER_CLASS_H
#define LSS_MASTER_CLASS_H

#include "CANopenLibDefines.h"
#include "CanInterfaceClass.h"
#include "TimerClass.h"
#include "CanConnectionClass.h"


const int COBID_TX_LSS_MASTER = 0x7e5;

//
//  LSS Master
//
class LSSMaster : public CanConnection
{
  public:
    CANOPENDLL_API LSSMaster(void);
    CANOPENDLL_API ~LSSMaster(void);
    CANOPENDLL_API canOpenStatus  canHardwareConnect(u8 port, u32 btr);
    CANOPENDLL_API canOpenStatus  switchModeGlobal(u8 mode);
    CANOPENDLL_API canOpenStatus  switchModeSelectiveVendorId(u32 vendorId, u8 *mode);
    CANOPENDLL_API canOpenStatus  switchModeSelectiveProductCode(u32 productCode, u8 *mode);
    CANOPENDLL_API canOpenStatus  switchModeSelectiveRevisionNumber(u32 revisionNumber, u8 *mode);
    CANOPENDLL_API canOpenStatus  switchModeSelectiveSerialNumber(u32 serialNumber, u8 *mode);
    CANOPENDLL_API canOpenStatus  configureNodeId(u8 nodeId, u8 *errorCode, u8 *specificErrorCode);
    CANOPENDLL_API canOpenStatus  configureBitTimingParamteres(u8 tableSelector, u8 tableIndex, u8 *errorCode, u8 *specificErrorCode);
    CANOPENDLL_API canOpenStatus  activateBitTimingParameters(u16 switchDelay, u8 *errorCode, u8 *specificErrorCode);
    CANOPENDLL_API canOpenStatus  storeConfiguration(u8 *errorCode, u8 *specificErrorCode);

  protected:

  private:
    CANOPENDLL_API canOpenStatus switchModeSelectiveGeneric(u8 cs, u32 param);
    CANOPENDLL_API canOpenStatus configureGenericSyncResponse(u8 cs, u8 param1, u8 param2, u8 *errorCode, u8 *specificError);
    CANOPENDLL_API canOpenStatus switchModeSelectiveSyncResponse(u8 cs, u32 parameter, u8 *mode);
    TimeClass *timer;
    static canOpenStatus canFrameConsumerW(
      void *lss_master_object, 
      unsigned long id, 
      unsigned char *data, 
      unsigned int dlc, 
      unsigned int flags);

    static canOpenStatus transferHelperW(void *lss_master_object);

    static void timerCallbackHandler(void *lss_master_object);
    
    canOpenStatus  canFrameConsumer(
      unsigned long id, 
      unsigned char *data, 
      unsigned int dlc, 
      unsigned int flags);
    canOpenStatus  transferHelper(void);

    u8                can_data_rx[8]; /* The can frame data for RX. */

    int               periodic_timer_index;

    bool              wait_lss_mode;
    bool              wait_configure;
    u8                response_mode;
    u8                configure_error_code;
    u8                configure_specific_error;
};

#endif //LSS_MASTER_CLASS_H
