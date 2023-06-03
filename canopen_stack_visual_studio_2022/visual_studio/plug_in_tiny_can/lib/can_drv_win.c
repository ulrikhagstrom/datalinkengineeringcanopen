/**************************************************************************/
/*                      Interface zur Tiny-CAN API                        */
/* ---------------------------------------------------------------------- */
/*  Beschreibung    : Laden einer Treiber DLL                             */
/*                                                                        */
/*  Version         : 1.20                                                */
/*  Datei Name      : can_drv_win.c                                       */
/* ---------------------------------------------------------------------- */
/*  Datum           : 03.02.07                                            */
/*  Author          : Demlehner Klaus, MHS-Elektronik, 94149 Kößlarn      */
/*                    info@mhs-elektronik.de  www.mhs-elektronik.de       */
/* ---------------------------------------------------------------------- */
/*  Compiler        : GNU C Compiler                                      */
/**************************************************************************/
#include "can_drv.h"


#ifndef USE_NO_DLL_DRIVER
int32_t CALLBACK DefCanInitDriver(char *options);
void CALLBACK DefCanDownDriver(void);
int32_t CALLBACK DefCanSetOptions(char *options);
int32_t CALLBACK DefCanDeviceOpen(uint32_t  index, char *parameter);
int32_t CALLBACK DefCanDeviceClose(uint32_t  index);
int32_t CALLBACK DefCanApplaySettings(uint32_t  index);

int32_t CALLBACK DefCanSetMode(uint32_t  index, unsigned char can_op_mode, uint16_t  can_command);
int32_t CALLBACK DefCanSet(uint32_t  index, uint16_t  obj_index, uint16_t  obj_sub_index, void *data, int32_t size);
int32_t CALLBACK DefCanGet(uint32_t  index, uint16_t  obj_index, uint16_t  obj_sub_index, void *data, int32_t size);

int32_t CALLBACK DefCanTransmit(uint32_t  index, struct TCanMsg *msg, int32_t count);
void CALLBACK DefCanTransmitClear(uint32_t  index);
uint32_t  CALLBACK DefCanTransmitGetCount(uint32_t  index);
int32_t CALLBACK DefCanTransmitSet(uint32_t  index, uint16_t  cmd,
  uint32_t  time);
int32_t CALLBACK DefCanReceive(uint32_t  index, struct TCanMsg *msg, int32_t count);
void CALLBACK DefCanReceiveClear(uint32_t  index);
uint32_t  CALLBACK DefCanReceiveGetCount(uint32_t  index);

int32_t CALLBACK DefCanSetSpeed(uint32_t  index, uint16_t  speed);
int32_t CALLBACK DefCanSetSpeedUser(uint32_t  index, uint32_t  value);
char * CALLBACK DefCanDrvInfo(void);
char * CALLBACK DefCanDrvHwInfo(uint32_t  index);
int32_t CALLBACK DefCanSetFilter(uint32_t  index, struct TMsgFilter *msg_filter);
int32_t CALLBACK DefCanGetDeviceStatus(uint32_t  index, struct TDeviceStatus *status);
void CALLBACK DefCanSetPnPEventCallback(void (CALLBACK *event)(uint32_t  index, int32_t status));
void CALLBACK DefCanSetStatusEventCallback(void (CALLBACK *event)
  (uint32_t  index, struct TDeviceStatus *device_status));
void CALLBACK DefCanSetRxEventCallback(void (CALLBACK *event)(uint32_t  index,
  struct TCanMsg *msg, int32_t count));

void CALLBACK DefCanSetEvents(uint16_t  events);
uint32_t  DefCanEventStatus(void);

TCanInitDriver CanInitDriver = DefCanInitDriver;
TCanDownDriver CanDownDriver = DefCanDownDriver;
TCanSetOptions CanSetOptions = DefCanSetOptions;
TCanDeviceOpen CanDeviceOpen = DefCanDeviceOpen;
TCanDeviceClose CanDeviceClose = DefCanDeviceClose;
TCanApplaySettings CanApplaySettings = DefCanApplaySettings;
TCanSetMode CanSetMode = DefCanSetMode;
TCanSet CanSet = DefCanSet;
TCanGet CanGet = DefCanGet;
TCanTransmit CanTransmit = DefCanTransmit;
TCanTransmitClear CanTransmitClear = DefCanTransmitClear;
TCanTransmitGetCount CanTransmitGetCount = DefCanTransmitGetCount;
TCanTransmitSet CanTransmitSet = DefCanTransmitSet;
TCanReceive CanReceive = DefCanReceive;
TCanReceiveClear CanReceiveClear = DefCanReceiveClear;
TCanReceiveGetCount CanReceiveGetCount = DefCanReceiveGetCount;
TCanSetSpeed CanSetSpeed = DefCanSetSpeed;
TCanSetSpeedUser CanSetSpeedUser = DefCanSetSpeedUser;
TCanDrvInfo CanDrvInfo = DefCanDrvInfo;
TCanDrvHwInfo CanDrvHwInfo = DefCanDrvHwInfo;
TCanSetFilter CanSetFilter = DefCanSetFilter;
TCanGetDeviceStatus CanGetDeviceStatus = DefCanGetDeviceStatus;
TCanSetPnPEventCallback CanSetPnPEventCallback = DefCanSetPnPEventCallback;
TCanSetStatusEventCallback CanSetStatusEventCallback = DefCanSetStatusEventCallback;
TCanSetRxEventCallback CanSetRxEventCallback = DefCanSetRxEventCallback;
TCanSetEvents CanSetEvents = DefCanSetEvents;
TCanEventStatus CanEventStatus = DefCanEventStatus;

HINSTANCE DriverDLL = 0;               // Handle to DLL
#endif

/***************************************************************/
/*  Treiber DLL laden                                          */
/***************************************************************/
int32_t LoadDriver(char *file_name)
{
int error;

error = 0;
#ifndef USE_NO_DLL_DRIVER
UnloadDriver();
DriverDLL = LoadLibrary(file_name);
if (DriverDLL)
  {
  CanInitDriver = (TCanInitDriver)GetProcAddress(DriverDLL, (LPCSTR)"CanInitDriver");
  CanDownDriver = (TCanDownDriver)GetProcAddress(DriverDLL, (LPCSTR)"CanDownDriver");
  CanSetOptions = (TCanSetOptions)GetProcAddress(DriverDLL, (LPCSTR)"CanSetOptions");
  CanDeviceOpen = (TCanDeviceOpen)GetProcAddress(DriverDLL, (LPCSTR)"CanDeviceOpen");
  CanDeviceClose = (TCanDeviceClose)GetProcAddress(DriverDLL, (LPCSTR)"CanDeviceClose");
  CanApplaySettings = (TCanApplaySettings)GetProcAddress(DriverDLL, (LPCSTR)"CanApplaySettings");
  CanSetMode = (TCanSetMode)GetProcAddress(DriverDLL, (LPCSTR)"CanSetMode");
  CanSet = (TCanSet)GetProcAddress(DriverDLL, (LPCSTR)"CanSet");
  CanGet = (TCanGet)GetProcAddress(DriverDLL, (LPCSTR)"CanGet");
  CanTransmit = (TCanTransmit)GetProcAddress(DriverDLL, (LPCSTR)"CanTransmit");
  CanTransmitClear = (TCanTransmitClear)GetProcAddress(DriverDLL, (LPCSTR)"CanTransmitClear");
  CanTransmitGetCount = (TCanTransmitGetCount)GetProcAddress(DriverDLL, (LPCSTR)"CanTransmitGetCount");
  CanTransmitSet = (TCanTransmitSet)GetProcAddress(DriverDLL, (LPCSTR)"CanTransmitSet");
  CanReceive = (TCanReceive)GetProcAddress(DriverDLL, (LPCSTR)"CanReceive");
  CanReceiveClear = (TCanReceiveClear)GetProcAddress(DriverDLL, (LPCSTR)"CanReceiveClear");
  CanReceiveGetCount = (TCanReceiveGetCount)GetProcAddress(DriverDLL, (LPCSTR)"CanReceiveGetCount");
  CanSetSpeed = (TCanSetSpeed)GetProcAddress(DriverDLL, (LPCSTR)"CanSetSpeed");
  CanSetSpeedUser = (TCanSetSpeedUser)GetProcAddress(DriverDLL, (LPCSTR)"CanSetSpeedUser");
  CanDrvInfo = (TCanDrvInfo)GetProcAddress(DriverDLL, (LPCSTR)"CanDrvInfo");
  CanDrvHwInfo = (TCanDrvHwInfo)GetProcAddress(DriverDLL, (LPCSTR)"CanDrvHwInfo");
  CanSetFilter = (TCanSetFilter)GetProcAddress(DriverDLL, (LPCSTR)"CanSetFilter");
  CanGetDeviceStatus = (TCanGetDeviceStatus)GetProcAddress(DriverDLL, (LPCSTR)"CanGetDeviceStatus");
  CanSetPnPEventCallback = (TCanSetPnPEventCallback)GetProcAddress(DriverDLL, (LPCSTR)"CanSetPnPEventCallback");
  CanSetStatusEventCallback = (TCanSetStatusEventCallback)GetProcAddress(DriverDLL, (LPCSTR)"CanSetStatusEventCallback");
  CanSetRxEventCallback = (TCanSetRxEventCallback)GetProcAddress(DriverDLL, (LPCSTR)"CanSetRxEventCallback");
  CanSetEvents = (TCanSetEvents)GetProcAddress(DriverDLL, (LPCSTR)"CanSetEvents");
  CanEventStatus = (TCanEventStatus)GetProcAddress(DriverDLL, (LPCSTR)"CanEventStatus");

  if (!CanInitDriver)
    error = -1;
  else if (!CanDownDriver)
    error = -2;
  else if (!CanSetOptions)
    error = -3;
  else if (!CanDeviceOpen)
    error = -4;
  else if (!CanDeviceClose)
    error = -5;
  else if (!CanApplaySettings)
    error = -6;
  else if (!CanSetMode)
    error = -7;
  else if (!CanSet)
    error = -8;
  else if (!CanGet)
    error = -9;
  else if (!CanTransmit)
    error = -10;
  else if (!CanTransmitClear)
    error = -11;
  else if (!CanTransmitGetCount)
    error = -12;
  else if (!CanTransmitSet)
    error = -13;
  else if (!CanReceive)
    error = -14;
  else if (!CanReceiveClear)
    error = -15;
  else if (!CanReceiveGetCount)
    error = -16;
  else if (!CanSetSpeed)
    error = -17;
  else if (!CanSetSpeedUser)
    error = -18;
  else if (!CanDrvInfo)
    error = -19;
  else if (!CanDrvHwInfo)
    error = -20;
  else if (!CanSetFilter)
    error = -21;
  else if (!CanGetDeviceStatus)
    error = -22;
  else if (!CanSetPnPEventCallback)
    error = -23;
  else if (!CanSetStatusEventCallback)
    error = -24;
  else if (!CanSetRxEventCallback)
    error = -25;
  else if (!CanSetEvents)
    error = -26;
  else if (!CanEventStatus)
    error = -27;
  }
else
  error = -100;
if (error)
  UnloadDriver();
#endif
return(error);
}


/***************************************************************/
/*  Treiber DLL entladen                                       */
/***************************************************************/
void UnloadDriver(void)
{
#ifndef USE_NO_DLL_DRIVER
if ((DriverDLL) && (CanDownDriver))
  CanDownDriver();
CanInitDriver = DefCanInitDriver;
CanDownDriver = DefCanDownDriver;
CanSetOptions = DefCanSetOptions;
CanDeviceOpen = DefCanDeviceOpen;
CanDeviceClose = DefCanDeviceClose;
CanApplaySettings = DefCanApplaySettings;
CanSetMode = DefCanSetMode;
CanSet = DefCanSet;
CanGet = DefCanGet;
CanTransmit = DefCanTransmit;
CanTransmitClear = DefCanTransmitClear;
CanTransmitGetCount = DefCanTransmitGetCount;
CanTransmitSet = DefCanTransmitSet;
CanReceive = DefCanReceive;
CanReceiveClear = DefCanReceiveClear;
CanReceiveGetCount = DefCanReceiveGetCount;
CanSetSpeed = DefCanSetSpeed;
CanSetSpeedUser = DefCanSetSpeedUser;
CanDrvInfo = DefCanDrvInfo;
CanDrvHwInfo = DefCanDrvHwInfo;
CanSetFilter = DefCanSetFilter;
CanGetDeviceStatus = DefCanGetDeviceStatus;
CanSetPnPEventCallback = DefCanSetPnPEventCallback;
CanSetStatusEventCallback = DefCanSetStatusEventCallback;
CanSetRxEventCallback = DefCanSetRxEventCallback;
CanSetEvents = DefCanSetEvents;
CanEventStatus = DefCanEventStatus;
if (DriverDLL)
  {
  FreeLibrary(DriverDLL);
  DriverDLL = NULL;
  }
#endif
}

#ifndef USE_NO_DLL_DRIVER
/***************************************************************/
/*  Default Call                                               */
/***************************************************************/
void DefDriverProcedure(int func)
{

}


/***************************************************************/
/*  Default Funktionen                                         */
/***************************************************************/
int32_t CALLBACK DefCanInitDriver(char *options)
{
DefDriverProcedure(1);
return(0);
}


void CALLBACK DefCanDownDriver(void)
{
// DefDriverProcedure(1);
}


int32_t CALLBACK DefCanSetOptions(char *options)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanDeviceOpen(uint32_t  index, char *parameter)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanDeviceClose(uint32_t  index)
{
return(0);
}


int32_t CALLBACK DefCanApplaySettings(uint32_t  index)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanSetMode(uint32_t index, unsigned char can_op_mode, uint16_t can_command)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanSet(uint32_t index, uint16_t obj_index, uint16_t obj_sub_index, void *data, int32_t size)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanGet(uint32_t index, uint16_t obj_index, uint16_t obj_sub_index, void *data, int32_t size)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanTransmit(uint32_t index, struct TCanMsg *msg, int32_t count)
{
DefDriverProcedure(1);
return(0);
}


void CALLBACK DefCanTransmitClear(uint32_t index)
{
DefDriverProcedure(1);
}


uint32_t  CALLBACK DefCanTransmitGetCount(uint32_t index)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanTransmitSet(uint32_t index, uint16_t cmd,
  uint32_t time)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanReceive(uint32_t index, struct TCanMsg *msg, int32_t count)
{
DefDriverProcedure(1);
return(0);
}


void CALLBACK DefCanReceiveClear(uint32_t index)
{
DefDriverProcedure(1);
}


uint32_t  CALLBACK DefCanReceiveGetCount(uint32_t index)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanSetSpeed(uint32_t index, uint16_t speed)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanSetSpeedUser(uint32_t index, uint32_t value)
{
DefDriverProcedure(1);
return(0);
}


char * CALLBACK DefCanDrvInfo(void)
{
DefDriverProcedure(1);
return(NULL);
}


char * CALLBACK DefCanDrvHwInfo(uint32_t index)
{
DefDriverProcedure(1);
return(NULL);
}


int32_t CALLBACK DefCanSetFilter(uint32_t index, struct TMsgFilter *msg_filter)
{
DefDriverProcedure(1);
return(0);
}


int32_t CALLBACK DefCanGetDeviceStatus(uint32_t index, struct TDeviceStatus *status)
{
status->CanStatus = CAN_STATUS_UNBEKANNT;
status->FifoStatus = FIFO_STATUS_UNBEKANNT;
status->DrvStatus = DRV_NOT_LOAD;
return(0);
}


void CALLBACK DefCanSetPnPEventCallback(void (CALLBACK *event)(uint32_t index, int32_t status))
{
DefDriverProcedure(1);
}


void CALLBACK DefCanSetStatusEventCallback(void (CALLBACK *event)
  (uint32_t index, struct TDeviceStatus *device_status))
{
DefDriverProcedure(1);
}


void CALLBACK DefCanSetRxEventCallback(void (CALLBACK *event)(uint32_t index,
  struct TCanMsg *msg, int32_t count))
{
DefDriverProcedure(1);
}


void CALLBACK DefCanSetEvents(uint16_t  events)
{
DefDriverProcedure(1);
}


uint32_t  DefCanEventStatus(void)
{
DefDriverProcedure(1);
return(1);
}

#endif
