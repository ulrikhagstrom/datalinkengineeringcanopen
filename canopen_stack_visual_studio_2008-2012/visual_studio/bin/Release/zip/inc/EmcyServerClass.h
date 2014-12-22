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

#ifndef EMCY_SERVER_CLASS_H
#define EMCY_SERVER_CLASS_H

#include "CANopenLibDefines.h"
#include "CanInterfaceClass.h"
#include "TimerClass.h"
#include "NMTSlaveClass.h"
#include "CanConnectionClass.h"

typedef void (*EmcyMessageFunPtr)( void *context, u8 nodeId, u16 emcyErrorCode, u8 errorRegister, u8 *manufacturerSpecificErrorField);

//
//  Emergency Server implementation.
//
class EmcyServer : public CanConnection
{
  public:
    CANOPENDLL_API EmcyServer(void);
    CANOPENDLL_API ~EmcyServer(void);
    CANOPENDLL_API canOpenStatus  canHardwareConnect(u8 port, u32 btr);
    CANOPENDLL_API canOpenStatus  registerEmergencyMessageCallBack(
      void *context, EmcyMessageFunPtr fp);

  protected:

  private:
    static canOpenStatus canFrameConsumerW(
      void *nmt_master_object, 
      unsigned long id, 
      unsigned char *data, 
      unsigned int dlc, 
      unsigned int flags);
    
    canOpenStatus  canFrameConsumer(
      unsigned long id, 
      unsigned char *data, 
      unsigned int dlc, 
      unsigned int flags);

    EmcyMessageFunPtr emergency_messagae_callback;
    void              *context;
};

#endif //EMCY_SERVER_CLASS_H
