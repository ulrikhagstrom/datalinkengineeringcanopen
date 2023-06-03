#ifndef __CAN_DRV_H__
#define __CAN_DRV_H__

#include "can_types.h"

#ifdef __WIN32__
// ****** Windows
#include <windows.h>
#define CALLBACK_TYPE CALLBACK
#else
#define CALLBACK_TYPE
#endif

#ifdef __cplusplus
  extern "C" {
#endif

/***************************************************************/
/* Define Makros                                               */
/***************************************************************/
// CAN Übertragungsgeschwindigkeit
#define CAN_10K_BIT   10    // 10 kBit/s
#define CAN_20K_BIT   20    // 20 kBit/s
#define CAN_50K_BIT   50    // 50 kBit/s
#define CAN_100K_BIT  100   // 100 kBit/s
#define CAN_125K_BIT  125   // 125 kBit/s
#define CAN_250K_BIT  250   // 250 kBit/s
#define CAN_500K_BIT  500   // 500 kBit/s
#define CAN_800K_BIT  800   // 800 kBit/s
#define CAN_1M_BIT    1000  // 1 MBit/s

// CAN Bus Mode
#define OP_CAN_NO_CHANGE         0  // Aktuellen Zustand nicht ändern
#define OP_CAN_START             1  // Startet den CAN-Bus
#define OP_CAN_STOP              2  // Stopt den CAN-Bus
#define OP_CAN_RESET             3  // Reset CAN Controller (BusOff löschen)
#define OP_CAN_START_LOM         4  // Startet den CAN-Bus im Silent Mode (Listen Only Mode)
#define OP_CAN_START_NO_RETRANS  5  // Startet den CAN-Bus im Automatic Retransmission disable Mode


#define CAN_CMD_NONE                0x0000
#define CAN_CMD_RXD_OVERRUN_CLEAR   0x0001
#define CAN_CMD_RXD_FIFOS_CLEAR     0x0002
#define CAN_CMD_TXD_OVERRUN_CLEAR   0x0004
#define CAN_CMD_TXD_FIFOS_CLEAR     0x0008
#define CAN_CMD_HW_FILTER_CLEAR     0x0010
#define CAN_CMD_SW_FILTER_CLEAR     0x0020
#define CAN_CMD_TXD_PUFFERS_CLEAR   0x0040

#define CAN_CMD_ALL_CLEAR           0x0FFF


// DrvStatus
#define DRV_NOT_LOAD              0  // Die Treiber DLL wurde noch nicht geladen
#define DRV_STATUS_NOT_INIT       1  // Treiber noch nicht Initialisiert
#define DRV_STATUS_INIT           2  // Treiber erfolgrich Initialisiert
#define DRV_STATUS_PORT_NOT_OPEN  3  // Die Schnittstelle wurde geöffnet
#define DRV_STATUS_PORT_OPEN      4  // Die Schnittstelle wurde nicht geöffnet
#define DRV_STATUS_DEVICE_FOUND   5  // Verbindung zur Hardware wurde Hergestellt
#define DRV_STATUS_CAN_OPEN       6  // Device wurde geöffnet und erfolgreich Initialisiert
#define DRV_STATUS_CAN_RUN_TX     7  // CAN Bus RUN nur Transmitter (wird nicht verwendet !)
#define DRV_STATUS_CAN_RUN        8  // CAN Bus RUN

// CanStatus
#define CAN_STATUS_OK          0     // CAN-Controller: Ok
#define CAN_STATUS_ERROR       1     // CAN-Controller: CAN Error
#define CAN_STATUS_WARNING     2     // CAN-Controller: Error warning
#define CAN_STATUS_PASSIV      3     // CAN-Controller: Error passiv
#define CAN_STATUS_BUS_OFF     4     // CAN-Controller: Bus Off
#define CAN_STATUS_UNBEKANNT   5     // CAN-Controller: Status Unbekannt

// Fifo Status
#define FIFO_OK                 0 // Fifo-Status: Ok
#define FIFO_HW_OVERRUN         1 // Fifo-Status: Hardware Fifo Überlauf
#define FIFO_SW_OVERRUN         2 // Fifo-Status: Software Fifo Überlauf
#define FIFO_HW_SW_OVERRUN      3 // Fifo-Status: Hardware & Software Fifo Überlauf
#define FIFO_STATUS_UNBEKANNT   4 // Fifo-Status: Unbekannt

// Makros für SetEvent
#define EVENT_ENABLE_PNP_CHANGE          0x0001
#define EVENT_ENABLE_STATUS_CHANGE       0x0002
#define EVENT_ENABLE_RX_FILTER_MESSAGES  0x0004
#define EVENT_ENABLE_RX_MESSAGES         0x0008
#define EVENT_ENABLE_ALL                 0x00FF

#define EVENT_DISABLE_PNP_CHANGE         0x0100
#define EVENT_DISABLE_STATUS_CHANGE      0x0200
#define EVENT_DISABLE_RX_FILTER_MESSAGES 0x0400
#define EVENT_DISABLE_RX_MESSAGES        0x0800
#define EVENT_DISABLE_ALL                0xFF00


/***************************************************************/
/*  Typen                                                      */
/***************************************************************/

/******************************************/
/*             Device Status              */
/******************************************/
struct TDeviceStatus
  {
  int32_t DrvStatus;
  unsigned char CanStatus;
  unsigned char FifoStatus;
  };


/***************************************************************/
/*  Funktionstypen                                             */
/***************************************************************/
typedef int32_t (CALLBACK_TYPE *TCanInitDriver)(char *options);
typedef void (CALLBACK_TYPE *TCanDownDriver)(void);
typedef int32_t (CALLBACK_TYPE *TCanSetOptions)(char *options);
typedef int32_t (CALLBACK_TYPE *TCanDeviceOpen)(uint32_t index, char *parameter);
typedef int32_t (CALLBACK_TYPE *TCanDeviceClose)(uint32_t index);
typedef int32_t (CALLBACK_TYPE *TCanApplaySettings)(uint32_t index);

typedef int32_t (CALLBACK_TYPE *TCanSetMode)(uint32_t index, unsigned char can_op_mode, uint16_t  can_command);
typedef int32_t (CALLBACK_TYPE *TCanSet)(uint32_t index, uint16_t obj_index, uint16_t obj_sub_index, void *data, int32_t size);
typedef int32_t (CALLBACK_TYPE *TCanGet)(uint32_t index, uint16_t obj_index, uint16_t obj_sub_index, void *data, int32_t size);

typedef int32_t (CALLBACK_TYPE *TCanTransmit)(uint32_t index, struct TCanMsg *msg, int32_t count);
typedef void (CALLBACK_TYPE *TCanTransmitClear)(uint32_t index);
typedef uint32_t  (CALLBACK_TYPE *TCanTransmitGetCount)(uint32_t index);
typedef int32_t (CALLBACK_TYPE *TCanTransmitSet)(uint32_t index, uint16_t cmd,
  uint32_t time);
typedef int32_t (CALLBACK_TYPE *TCanReceive)(uint32_t index, struct TCanMsg *msg, int32_t count);
typedef void (CALLBACK_TYPE *TCanReceiveClear)(uint32_t index);
typedef uint32_t  (CALLBACK_TYPE *TCanReceiveGetCount)(uint32_t index);

typedef int32_t (CALLBACK_TYPE *TCanSetSpeed)(uint32_t index, uint16_t speed);
typedef int32_t (CALLBACK_TYPE *TCanSetSpeedUser)(uint32_t index, uint32_t value);
typedef char * (CALLBACK_TYPE *TCanDrvInfo)(void);
typedef char * (CALLBACK_TYPE *TCanDrvHwInfo)(uint32_t index);
typedef int32_t (CALLBACK_TYPE *TCanSetFilter)(uint32_t index, struct TMsgFilter *msg_filter);

typedef int32_t (CALLBACK_TYPE *TCanGetDeviceStatus)(uint32_t index, struct TDeviceStatus *status);

typedef void (CALLBACK_TYPE *TCanSetPnPEventCallback)(void (CALLBACK_TYPE *event)(uint32_t index, int32_t status));
typedef void (CALLBACK_TYPE *TCanSetStatusEventCallback)(void (CALLBACK_TYPE *event)
   (uint32_t index, struct TDeviceStatus *device_status));
typedef void (CALLBACK_TYPE *TCanSetRxEventCallback)(void (CALLBACK_TYPE *event)
   (uint32_t index, struct TCanMsg *msg, int32_t count));

typedef void (CALLBACK_TYPE *TCanSetEvents)(uint16_t events);
typedef uint32_t (*TCanEventStatus)(void);

/***************************************************************/
/*  Tiny-CAN API Funktionen                                    */
/***************************************************************/
extern TCanInitDriver CanInitDriver;
extern TCanDownDriver CanDownDriver;
extern TCanSetOptions CanSetOptions;
extern TCanDeviceOpen CanDeviceOpen;
extern TCanDeviceClose CanDeviceClose;

extern TCanApplaySettings CanApplaySettings;
extern TCanSetMode CanSetMode;
extern TCanSet CanSet;
extern TCanGet CanGet;

extern TCanTransmit CanTransmit;
extern TCanTransmitClear CanTransmitClear;
extern TCanTransmitGetCount CanTransmitGetCount;
extern TCanTransmitSet CanTransmitSet;
extern TCanReceive CanReceive;
extern TCanReceiveClear CanReceiveClear;
extern TCanReceiveGetCount CanReceiveGetCount;

extern TCanSetSpeed CanSetSpeed;
extern TCanSetSpeedUser CanSetSpeedUser;
// extern TCanSetAccMask CanSetAccMask;

extern TCanDrvInfo CanDrvInfo;
extern TCanDrvHwInfo CanDrvHwInfo;
extern TCanSetFilter CanSetFilter;

extern TCanGetDeviceStatus CanGetDeviceStatus;

extern TCanSetPnPEventCallback CanSetPnPEventCallback;
extern TCanSetStatusEventCallback CanSetStatusEventCallback;
extern TCanSetRxEventCallback CanSetRxEventCallback;

extern TCanSetEvents CanSetEvents;
extern TCanEventStatus CanEventStatus;

/***************************************************************/
/*  Funktionen Treiber laden/entladen                          */
/***************************************************************/
int32_t LoadDriver(char *file_name);
void UnloadDriver(void);


#ifdef __cplusplus
  }
#endif

#endif
