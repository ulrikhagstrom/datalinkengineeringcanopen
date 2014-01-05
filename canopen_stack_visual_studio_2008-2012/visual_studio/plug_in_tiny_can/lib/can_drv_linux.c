/**************************************************************************/
/*                      Interface zur Tiny-CAN API                        */
/* ---------------------------------------------------------------------- */
/*  Beschreibung    : Laden einer Treiber DLL                             */
/*                                                                        */
/*  Version         : 1.20                                                */
/*  Datei Name      : can_drv_linux.c                                     */
/* ---------------------------------------------------------------------- */
/*  Datum           : 03.02.07                                            */
/*  Author          : Demlehner Klaus, MHS-Elektronik, 94149 Kößlarn      */
/*                    info@mhs-elektronik.de  www.mhs-elektronik.de       */
/* ---------------------------------------------------------------------- */
/*  Compiler        : GNU C Compiler                                      */
/**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include "can_drv.h"


#ifndef USE_NO_DLL_DRIVER
int32_t DefCanInitDriver(char *options);
void DefCanDownDriver(void);
int32_t DefCanSetOptions(char *options);
int32_t DefCanDeviceOpen(uint32_t  index, char *parameter);
int32_t DefCanDeviceClose(uint32_t  index);
int32_t DefCanApplaySettings(uint32_t  index);

int32_t DefCanSetMode(uint32_t  index, unsigned char can_op_mode, uint16_t  can_command);
int32_t DefCanSet(uint32_t  index, uint16_t  obj_index, uint16_t  obj_sub_index, void *data, int32_t size);
int32_t DefCanGet(uint32_t  index, uint16_t  obj_index, uint16_t  obj_sub_index, void *data, int32_t size);

int32_t DefCanTransmit(uint32_t  index, struct TCanMsg *msg, int32_t count);
void DefCanTransmitClear(uint32_t  index);
uint32_t  DefCanTransmitGetCount(uint32_t  index);
int32_t DefCanTransmitSet(uint32_t  index, uint16_t  cmd,
  uint32_t  time);
int32_t DefCanReceive(uint32_t  index, struct TCanMsg *msg, int32_t count);
void DefCanReceiveClear(uint32_t  index);
uint32_t  DefCanReceiveGetCount(uint32_t  index);

int32_t DefCanSetSpeed(uint32_t  index, uint16_t  speed);
int32_t DefCanSetSpeedUser(uint32_t  index, uint32_t  value1);
char *DefCanDrvInfo(void);
char *DefCanDrvHwInfo(uint32_t  index);
int32_t DefCanSetFilter(uint32_t  index, struct TMsgFilter *msg_filter);
int32_t DefCanGetDeviceStatus(uint32_t  index, struct TDeviceStatus *status);
void DefCanSetPnPEventCallback(void (*event)(uint32_t  index, int32_t status));
void DefCanSetStatusEventCallback(void (*event)
  (uint32_t  index, struct TDeviceStatus *device_status));
void DefCanSetRxEventCallback(void (*event)(uint32_t  index,
  struct TCanMsg *msg, int32_t count));

void DefCanSetEvents(uint16_t  events);
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

void *DriverHandle = NULL;
#endif

/***************************************************************/
/*  Treiber DLL laden                                          */
/***************************************************************/
int32_t LoadDriver(char *file_name)
{
int32_t error;

error = 0;
#ifndef USE_NO_DLL_DRIVER
UnloadDriver();
DriverHandle = dlopen(file_name, RTLD_LAZY);
if (DriverHandle)
  {
  CanInitDriver = (TCanInitDriver)dlsym(DriverHandle, "CanInitDriver");
  CanDownDriver = (TCanDownDriver)dlsym(DriverHandle, "CanDownDriver");
  CanSetOptions = (TCanSetOptions)dlsym(DriverHandle, "CanSetOptions");
  CanDeviceOpen = (TCanDeviceOpen)dlsym(DriverHandle, "CanDeviceOpen");
  CanDeviceClose = (TCanDeviceClose)dlsym(DriverHandle, "CanDeviceClose");
  CanApplaySettings = (TCanApplaySettings)dlsym(DriverHandle, "CanApplaySettings");
  CanSetMode = (TCanSetMode)dlsym(DriverHandle, "CanSetMode");
  CanSet = (TCanSet)dlsym(DriverHandle, "CanSet");
  CanGet = (TCanGet)dlsym(DriverHandle, "CanGet");
  CanTransmit = (TCanTransmit)dlsym(DriverHandle, "CanTransmit");
  CanTransmitClear = (TCanTransmitClear)dlsym(DriverHandle, "CanTransmitClear");
  CanTransmitGetCount = (TCanTransmitGetCount)dlsym(DriverHandle, "CanTransmitGetCount");
  CanTransmitSet = (TCanTransmitSet)dlsym(DriverHandle, "CanTransmitSet");
  CanReceive = (TCanReceive)dlsym(DriverHandle, "CanReceive");
  CanReceiveClear = (TCanReceiveClear)dlsym(DriverHandle, "CanReceiveClear");
  CanReceiveGetCount = (TCanReceiveGetCount)dlsym(DriverHandle, "CanReceiveGetCount");
  CanSetSpeed = (TCanSetSpeed)dlsym(DriverHandle, "CanSetSpeed");
  CanSetSpeedUser = (TCanSetSpeedUser)dlsym(DriverHandle, "CanSetSpeedUser");
  CanDrvInfo = (TCanDrvInfo)dlsym(DriverHandle, "CanDrvInfo");
  CanDrvHwInfo = (TCanDrvHwInfo)dlsym(DriverHandle, "CanDrvHwInfo");
  CanSetFilter = (TCanSetFilter)dlsym(DriverHandle, "CanSetFilter");
  CanGetDeviceStatus = (TCanGetDeviceStatus)dlsym(DriverHandle, "CanGetDeviceStatus");
  CanSetPnPEventCallback = (TCanSetPnPEventCallback)dlsym(DriverHandle, "CanSetPnPEventCallback");
  CanSetStatusEventCallback = (TCanSetStatusEventCallback)dlsym(DriverHandle, "CanSetStatusEventCallback");
  CanSetRxEventCallback = (TCanSetRxEventCallback)dlsym(DriverHandle, "CanSetRxEventCallback");
  CanSetEvents = (TCanSetEvents)dlsym(DriverHandle, "CanSetEvents");
  CanEventStatus = (TCanEventStatus)dlsym(DriverHandle, "CanEventStatus");

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
if ((DriverHandle) && (CanDownDriver))
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

if (DriverHandle)
  {
  dlclose(DriverHandle);
  DriverHandle = NULL;
  }
#endif
}


#ifndef USE_NO_DLL_DRIVER
/***************************************************************/
/*  Default Call                                               */
/***************************************************************/
void DefDriverProcedure(int32_t func)
{

}


/***************************************************************/
/*  Default Funktionen                                         */
/***************************************************************/
int32_t DefCanInitDriver(char *options)
{
DefDriverProcedure(1);
return(0);
}


void DefCanDownDriver(void)
{
// DefDriverProcedure(1);
}


int32_t DefCanSetOptions(char *options)
{
DefDriverProcedure(1);
return(0);
}


int32_t DefCanDeviceOpen(uint32_t  index, char *parameter)
{
DefDriverProcedure(1);
return(0);
}


int32_t DefCanDeviceClose(uint32_t  index)
{
return(0);
}


int32_t DefCanApplaySettings(uint32_t  index)
{
DefDriverProcedure(1);
return(0);
}


int32_t DefCanSetMode(uint32_t  index, unsigned char can_op_mode, uint16_t  can_command)
{
DefDriverProcedure(1);
return(0);
}


int32_t DefCanSet(uint32_t  index, uint16_t  obj_index, uint16_t  obj_sub_index, void *data, int32_t size)
{
DefDriverProcedure(1);
return(0);
}


int32_t DefCanGet(uint32_t  index, uint16_t  obj_index, uint16_t  obj_sub_index, void *data, int32_t size)
{
DefDriverProcedure(1);
return(0);
}


int32_t DefCanTransmit(uint32_t  index, struct TCanMsg *msg, int32_t count)
{
DefDriverProcedure(1);
return(0);
}


void DefCanTransmitClear(uint32_t  index)
{
DefDriverProcedure(1);
}


uint32_t  DefCanTransmitGetCount(uint32_t  index)
{
DefDriverProcedure(1);
return(0);
}


int32_t DefCanTransmitSet(uint32_t  index, uint16_t  cmd,
  uint32_t  time)
{
DefDriverProcedure(1);
return(0);
}


int32_t DefCanReceive(uint32_t  index, struct TCanMsg *msg, int32_t count)
{
DefDriverProcedure(1);
return(0);
}


void DefCanReceiveClear(uint32_t  index)
{
DefDriverProcedure(1);
}


uint32_t  DefCanReceiveGetCount(uint32_t  index)
{
DefDriverProcedure(1);
return(0);
}


int32_t DefCanSetSpeed(uint32_t  index, uint16_t  speed)
{
DefDriverProcedure(1);
return(0);
}


int32_t DefCanSetSpeedUser(uint32_t  index, uint32_t  value)
{
DefDriverProcedure(1);
return(0);
}


char *DefCanDrvInfo(void)
{
DefDriverProcedure(1);
return(NULL);
}


char *DefCanDrvHwInfo(uint32_t  index)
{
DefDriverProcedure(1);
return(NULL);
}


int32_t DefCanSetFilter(uint32_t  index, struct TMsgFilter *msg_filter)
{
DefDriverProcedure(1);
return(0);
}


int32_t DefCanGetDeviceStatus(uint32_t  index, struct TDeviceStatus *status)
{
status->CanStatus = CAN_STATUS_UNBEKANNT;
status->FifoStatus = FIFO_STATUS_UNBEKANNT;
status->DrvStatus = DRV_NOT_LOAD;
return(0);
}


void DefCanSetPnPEventCallback(void (*event)(uint32_t  index, int32_t status))
{
DefDriverProcedure(1);
}


void DefCanSetStatusEventCallback(void (*event)
  (uint32_t  index, struct TDeviceStatus *device_status))
{
DefDriverProcedure(1);
}


void DefCanSetRxEventCallback(void (*event)(uint32_t  index,
  struct TCanMsg *msg, int32_t count))
{
DefDriverProcedure(1);
}


void DefCanSetEvents(uint16_t  events)
{
DefDriverProcedure(1);
}


uint32_t  DefCanEventStatus(void)
{
DefDriverProcedure(1);
return(1);
}


#endif
