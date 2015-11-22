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

#ifndef RECEIVE_PDO_CLASS_H
#define RECEIVE_PDO_CLASS_H

#include "CANopenLibDefines.h"
#include "CanInterfaceClass.h"
#include "CanConnectionClass.h"

typedef void (*ReceivePdoFunPtr)( void *context, u32 cobid, u8* data, u8 len);

//
//  Receive PDO implementation.
//
class ReceivePDO : public CanConnection
{
  public:
    CANOPENDLL_API ReceivePDO(void);
    CANOPENDLL_API ~ReceivePDO(void);
    CANOPENDLL_API canOpenStatus  canHardwareConnect(u8 port, u32 btr);
    CANOPENDLL_API canOpenStatus  registerReceivePdoMessageCallBack(
      void *context, ReceivePdoFunPtr fp);
    CANOPENDLL_API canOpenStatus  setCobid(COBID cobid);
    CANOPENDLL_API canOpenStatus  requestPDO(u8 dlc);

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

    ReceivePdoFunPtr receive_pdo_callback;
    void              *context;
    u32               cobid;
};

#endif //RECEIVE_PDO_CLASS_H
