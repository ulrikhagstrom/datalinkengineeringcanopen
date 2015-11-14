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

// canlib32.cpp : Defines the entry point for the DLL application.
//
                                                                                                      
#include "windows.h"
#include "stdio.h"
#include "canopenlib_hw.h"
#include "canlib32_dyn_load.h"
#include "CANopenLibHelper.h"


#include <time.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <conio.h>
#include <shlwapi.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif


#ifdef _MANAGED
#pragma managed(pop)
#endif



HINSTANCE  hCanLib;

DllGetVersionFP dllGetVersionFP = NULL;
CanInitializeLibraryFP canInitializeLibraryFP = NULL;
CanOpenChannelFP  canOpenChannelFP = NULL;
CanBusOnFP canBusOnFP = NULL;
CanSetBusParamsFP canSetBusParamsFP = NULL;
CanReadFP canReadFP = NULL;
CanWriteFP canWriteFP = NULL;
CanBusOffFP canBusOffFP = NULL;
CanCloseFP canCloseFP = NULL;
CanGetBusParamsFP canGetBusParamsFP = NULL;
CanSetBusOutputControlFP canSetBusOutputControlFP = NULL;
CanGetBusOutputControlFP canGetBusOutputControlFP = NULL;
CanAcceptFP canAcceptFP = NULL;
CanReadStatusFP canReadStatusFP = NULL;
CanReadErrorCountersFP canReadErrorCountersFP = NULL;
CanWriteSyncFP canWriteSyncFP = NULL;
CanReadWaitFP canReadWaitFP = NULL;
CanReadSpecificFP canReadSpecificFP = NULL;
CanReadSyncFP canReadSyncFP = NULL;
CanReadSyncSpecificFP canReadSyncSpecificFP = NULL;
CanReadSpecificSkipFP canReadSpecificSkipFP = NULL;
CanSetNotifyFP canSetNotifyFP = NULL;
CanTranslateBaudFP canTranslateBaudFP = NULL;
CanGetErrorTextFP canGetErrorTextFP = NULL;
CanGetVersionFP canGetVersionFP = NULL;
CanIoCtlFP canIoCtlFP = NULL;
CanReadTimerFP canReadTimerFP = NULL;

CanGetNumberOfChannelsFP canGetNumberOfChannelsFP = NULL;
CanGetChannelDataFP canGetChannelDataFP = NULL;
CanWaitForEventFP canWaitForEventFP = NULL;
CanSetBusParamsC200FP canSetBusParamsC200FP = NULL;
CanSetDriverModeFP canSetDriverModeFP = NULL;
CanGetDriverModeFP canGetDriverModeFP = NULL;
CanGetVersionExFP canGetVersionExFP = NULL;
CanParamGetCountFP canParamGetCountFP = NULL;
CanParamCommitChangesFP canParamCommitChangesFP = NULL;
CanParamDeleteEntryFP canParamDeleteEntryFP = NULL;
CanParamCreateNewEntryFP canParamCreateNewEntryFP = NULL;
CanParamSwapEntriesFP canParamSwapEntriesFP = NULL;
CanParamGetNameFP canParamGetNameFP = NULL;
CanParamGetChannelNumberFP canParamGetChannelNumberFP = NULL;
CanParamGetBusParamsFP canParamGetBusParamsFP = NULL;

CanParamSetNameFP canParamSetNameFP = NULL;
CanParamSetChannelNumberFP canParamSetChannelNumberFP = NULL;
CanParamSetBusParamsFP canParamSetBusParamsFP = NULL;
CanParamFindByNameFP canParamFindByNameFP = NULL;
CanObjBufFreeAllFP canObjBufFreeAllFP = NULL;
CanObjBufAllocateFP canObjBufAllocateFP = NULL;
CanObjBufFreeFP canObjBufFreeFP = NULL;
CanObjBufWriteFP canObjBufWriteFP = NULL;
CanObjBufSetFilterFP canObjBufSetFilterFP = NULL;
CanObjBufSetFlagsFP canObjBufSetFlagsFP = NULL;
CanObjBufSetPeriodFP canObjBufSetPeriodFP = NULL;
CanObjBufEnableFP canObjBufEnableFP = NULL;
CanObjBufDisableFP canObjBufDisableFP = NULL;
CanObjBufSendBurstFP canObjBufSendBurstFP = NULL;
CanResetBusFP canResetBusFP = NULL;
CanWriteWaitFP canWriteWaitFP = NULL;
CanUnloadLibraryFP canUnloadLibraryFP = NULL;
CanSetAcceptanceFilterFP canSetAcceptanceFilterFP = NULL; 
CanFlushReceiveQueueFP canFlushReceiveQueueFP = NULL;
CanFlushTransmitQueueFP canFlushTransmitQueueFP = NULL;

KvGetApplicationMappingFP kvGetApplicationMappingFP = NULL;
KvBeep kvBeepFP = NULL;
KvSelfTest kvSelfTestFP = NULL;
KvFlashLeds kvFlashLedsFP = NULL;
CanRequestChipStatus canRequestChipStatusFP = NULL;
CanRequestBusStatistics canRequestBusStatisticsFP = NULL;
CanGetBusStatistics canGetBusStatisticsFP = NULL;
CanSetBitrate canSetBitrateFP = NULL;
KvAnnounceIdentity kvAnnounceIdentityFP = NULL;
CanGetHandleData canGetHandleDataFP = NULL;
KvTimeDomainCreate kvTimeDomainCreateFP = NULL;
KvTimeDomainDelete kvTimeDomainDeleteFP = NULL;
KvTimeDomainResetTime kvTimeDomainResetTimeFP = NULL;
KvTimeDomainGetData kvTimeDomainGetDataFP = NULL;
KvTimeDomainAddHandle kvTimeDomainAddHandleFP = NULL;
KvTimeDomainRemoveHandle kvTimeDomainRemoveHandleFP = NULL;
KvSetNotifyCallback kvSetNotifyCallbackFP = NULL; 


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
u32 buf2val(u8 *buf, u8 len) {
  u32 val = 0;
  buf += len; /* Points to MSB+1 (we use predecrement or CodeGuard will complain on buffer underrun) */
  while(len--)
    val = (val << 8)+*--buf;
  return val;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	  case DLL_PROCESS_ATTACH:
      break;
	  case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH:
      if (hCanLib != NULL)
	  {
        FreeLibrary(hCanLib);
        canInitializeLibraryFP = null;
        canReadFP = NULL;
      }
      break;
	}
  return TRUE;
} 


CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortLibraryInit(void)
{
  /*
	*	Load proper CANLIB.DLL
	*/
	hCanLib = LoadLibrary(TEXT("canlib32.dll"));
  if (hCanLib == NULL)
    return CANOPEN_ERROR_DRIVER;
	/*
	*	Connect to the functions in the DLL.
	*/
  dllGetVersionFP = (DllGetVersionFP) GetProcAddress(hCanLib, "DllGetVersion");
	canInitializeLibraryFP = (CanInitializeLibraryFP)GetProcAddress(hCanLib, "canInitializeLibrary");
	canOpenChannelFP = (CanOpenChannelFP)GetProcAddress(hCanLib, "canOpenChannel");
	canBusOnFP = (CanBusOnFP)GetProcAddress(hCanLib, "canBusOn");
	canSetBusParamsFP = (CanSetBusParamsFP)GetProcAddress(hCanLib, "canSetBusParams");
	canReadFP = (CanReadFP)GetProcAddress(hCanLib, "canRead");
	canWriteFP = (CanWriteFP)GetProcAddress(hCanLib, "canWrite");
	canBusOffFP = (CanBusOffFP)GetProcAddress(hCanLib, "canBusOff");
	canCloseFP = (CanCloseFP)GetProcAddress(hCanLib, "canClose");
	canGetBusParamsFP = (CanGetBusParamsFP)GetProcAddress(hCanLib, "canGetBusParams");
	canSetBusOutputControlFP = (CanSetBusOutputControlFP)GetProcAddress(hCanLib, "canSetBusOutputControl");
	canGetBusOutputControlFP = (CanGetBusOutputControlFP)GetProcAddress(hCanLib, "canGetBusOutputControl");
	canAcceptFP = (CanAcceptFP)GetProcAddress(hCanLib, "canAccept");
	canReadStatusFP = (CanReadStatusFP)GetProcAddress(hCanLib, "canReadStatus");
	canReadErrorCountersFP = (CanReadErrorCountersFP)GetProcAddress(hCanLib, "canReadErrorCounters");
	canWriteSyncFP = (CanWriteSyncFP)GetProcAddress(hCanLib, "canWriteSync");
	canReadWaitFP = (CanReadWaitFP)GetProcAddress(hCanLib, "canReadWait");
	canReadSpecificFP = (CanReadSpecificFP)GetProcAddress(hCanLib, "canReadSpecific");
	canReadSyncFP = (CanReadSyncFP)GetProcAddress(hCanLib, "canReadSync");
	canReadSyncSpecificFP = (CanReadSyncSpecificFP)GetProcAddress(hCanLib, "canReadSyncSpecific");
	canReadSpecificSkipFP = (CanReadSpecificSkipFP)GetProcAddress(hCanLib, "canReadSpecificSkip");
	canSetNotifyFP = (CanSetNotifyFP)GetProcAddress(hCanLib, "canSetNotify");
	canTranslateBaudFP = (CanTranslateBaudFP)GetProcAddress(hCanLib, "canTranslateBaud");
	canGetErrorTextFP = (CanGetErrorTextFP)GetProcAddress(hCanLib, "canGetErrorText");
	canGetVersionFP = (CanGetVersionFP)GetProcAddress(hCanLib, "canGetVersion");
	canIoCtlFP = (CanIoCtlFP)GetProcAddress(hCanLib, "canIoCtl");
	canReadTimerFP = (CanReadTimerFP)GetProcAddress(hCanLib, "canReadTimer");

	canGetNumberOfChannelsFP = (CanGetNumberOfChannelsFP)GetProcAddress(hCanLib, "canGetNumberOfChannels");
	canGetChannelDataFP = (CanGetChannelDataFP)GetProcAddress(hCanLib, "canGetChannelData");
	canWaitForEventFP = (CanWaitForEventFP)GetProcAddress(hCanLib, "canWaitForEvent");
	canSetBusParamsC200FP = (CanSetBusParamsC200FP)GetProcAddress(hCanLib, "canSetBusParamsC200");
	canSetDriverModeFP = (CanSetDriverModeFP)GetProcAddress(hCanLib, "canSetDriverMode");
	canGetDriverModeFP = (CanGetDriverModeFP)GetProcAddress(hCanLib, "canGetDriverMode");
	canGetVersionExFP = (CanGetVersionExFP)GetProcAddress(hCanLib, "canGetVersionEx");
	canParamGetCountFP = (CanParamGetCountFP)GetProcAddress(hCanLib, "canParamGetCount");
	canParamCommitChangesFP = (CanParamCommitChangesFP)GetProcAddress(hCanLib, "canParamCommitChanges");
	canParamDeleteEntryFP = (CanParamDeleteEntryFP)GetProcAddress(hCanLib, "canParamDeleteEntry");
	canParamCreateNewEntryFP = (CanParamCreateNewEntryFP)GetProcAddress(hCanLib, "canParamCreateNewEntry");
	canParamSwapEntriesFP = (CanParamSwapEntriesFP)GetProcAddress(hCanLib, "canParamSwapEntries");
	canParamGetNameFP = (CanParamGetNameFP)GetProcAddress(hCanLib, "canParamGetName");
	canParamGetChannelNumberFP = (CanParamGetChannelNumberFP)GetProcAddress(hCanLib, "canParamGetChannelNumber");
	canParamGetBusParamsFP = (CanParamGetBusParamsFP)GetProcAddress(hCanLib, "canParamGetBusParams");


	canParamSetNameFP = (CanParamSetNameFP)GetProcAddress(hCanLib, "canParamSetName");
	canParamSetChannelNumberFP = (CanParamSetChannelNumberFP)GetProcAddress(hCanLib, "canParamSetChannelNumber");
	canParamSetBusParamsFP = (CanParamSetBusParamsFP)GetProcAddress(hCanLib, "canParamSetBusParams");
	canParamFindByNameFP = (CanParamFindByNameFP)GetProcAddress(hCanLib, "canParamFindByName");
	canObjBufFreeAllFP = (CanObjBufFreeAllFP)GetProcAddress(hCanLib, "canObjBufFreeAll");
	canObjBufAllocateFP = (CanObjBufAllocateFP)GetProcAddress(hCanLib, "canObjBufAllocate");
	canObjBufFreeFP = (CanObjBufFreeFP)GetProcAddress(hCanLib, "canObjBufFree");
	canObjBufWriteFP = (CanObjBufWriteFP)GetProcAddress(hCanLib, "canObjBufWrite");
	canObjBufWriteFP = (CanObjBufWriteFP)GetProcAddress(hCanLib, "canObjBufWrite");
	canObjBufSetFlagsFP = (CanObjBufSetFlagsFP)GetProcAddress(hCanLib, "canObjBufSetFlags");
	canObjBufSetPeriodFP = (CanObjBufSetPeriodFP)GetProcAddress(hCanLib, "canObjBufSetPeriod");
	canObjBufEnableFP = (CanObjBufEnableFP)GetProcAddress(hCanLib, "canObjBufEnable");
	canObjBufDisableFP = (CanObjBufDisableFP)GetProcAddress(hCanLib, "canObjBufDisable");
	canObjBufSendBurstFP = (CanObjBufSendBurstFP)GetProcAddress(hCanLib, "canObjBufSendBurst");
	canResetBusFP = (CanResetBusFP)GetProcAddress(hCanLib, "canResetBus");
	canWriteWaitFP = (CanWriteWaitFP)GetProcAddress(hCanLib, "canWriteWait");
	canUnloadLibraryFP = (CanUnloadLibraryFP)GetProcAddress(hCanLib, "canUnloadLibrary");
	canSetAcceptanceFilterFP = (CanSetAcceptanceFilterFP)GetProcAddress(hCanLib, "canSetAcceptanceFilter"); 
	canFlushReceiveQueueFP = (CanFlushReceiveQueueFP)GetProcAddress(hCanLib, "canFlushReceiveQueue");
	canFlushTransmitQueueFP = (CanFlushTransmitQueueFP)GetProcAddress(hCanLib, "canFlushTransmitQueue");

	kvGetApplicationMappingFP = (KvGetApplicationMappingFP)GetProcAddress(hCanLib, "kvGetApplicationMapping");
	kvBeepFP = (KvBeep)GetProcAddress(hCanLib, "kvBeep");
	kvSelfTestFP = (KvSelfTest)GetProcAddress(hCanLib, "kvSelfTest");
	kvFlashLedsFP = (KvFlashLeds)GetProcAddress(hCanLib, "kvFlashLeds");
	canRequestChipStatusFP = (CanRequestChipStatus)GetProcAddress(hCanLib, "canRequestChipStatus");
	canRequestBusStatisticsFP = (CanRequestBusStatistics)GetProcAddress(hCanLib, "canRequestBusStatistics");
	canGetBusStatisticsFP = (CanGetBusStatistics)GetProcAddress(hCanLib, "canGetBusStatistics");
	canSetBitrateFP = (CanSetBitrate)GetProcAddress(hCanLib, "canSetBitrate");
	kvAnnounceIdentityFP = (KvAnnounceIdentity)GetProcAddress(hCanLib, "kvAnnounceIdentity");
	canGetHandleDataFP = (CanGetHandleData)GetProcAddress(hCanLib, "canGetHandleData");
	kvTimeDomainCreateFP = (KvTimeDomainCreate)GetProcAddress(hCanLib, "kvTimeDomainCreate");
	kvTimeDomainDeleteFP = (KvTimeDomainDelete)GetProcAddress(hCanLib, "kvTimeDomainDelete");
	kvTimeDomainResetTimeFP = (KvTimeDomainResetTime)GetProcAddress(hCanLib, "kvTimeDomainResetTime");
	kvTimeDomainGetDataFP = (KvTimeDomainGetData)GetProcAddress(hCanLib, "kvTimeDomainGetData");
	kvTimeDomainAddHandleFP = (KvTimeDomainAddHandle)GetProcAddress(hCanLib, "kvTimeDomainAddHandle");
	kvTimeDomainRemoveHandleFP = (KvTimeDomainRemoveHandle)GetProcAddress(hCanLib, "kvTimeDomainRemoveHandle");
  kvSetNotifyCallbackFP = (KvSetNotifyCallback)GetProcAddress(hCanLib, "kvSetNotifyCallback");

 	/*
	*	Do the initialzation!
	*/
	canInitializeLibraryFP();
  return CANOPEN_OK;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortOpen( int port, canPortHandle *handle )
{
  if (canOpenChannelFP)
  {
    *handle =  canOpenChannelFP( port, canOPEN_ACCEPT_VIRTUAL);
    if (*handle >= 0)
      return CANOPEN_OK;
    else
      return CANOPEN_ERROR_HW_UNDEFINED;
  }
  return CANOPEN_ERROR_DRIVER;

}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortClose( canPortHandle handle )
{
  if (canCloseFP)
  {
    canStatus kv_status = canCloseFP( (int)handle );
    if (kv_status == canOK)
      return CANOPEN_OK;
    else
      return CANOPEN_ERROR_HW_UNDEFINED;
  }
  return CANOPEN_ERROR_DRIVER;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortBitrateSet( canPortHandle handle, int bitrate )
{
  if (canSetBitrateFP)
  {
    canStatus kv_status = canSetBitrateFP( (int)handle, bitrate );
    if (kv_status == canOK)
      return CANOPEN_OK;
    else
      return CANOPEN_ERROR_HW_UNDEFINED;
  }
  return CANOPEN_ERROR_DRIVER;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortEcho( canPortHandle handle, bool enabled )
{
  if (canIoCtlFP)
  {
    DWORD flag = enabled ? 1 : 0;
    canStatus kv_status = canIoCtlFP( (int)handle, canIOCTL_SET_TXACK, &flag, sizeof(DWORD));
    if ( kv_status == canOK )
      return CANOPEN_OK;
    else
      return CANOPEN_ERROR_HW_UNDEFINED;
  }
  return CANOPEN_ERROR_DRIVER;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOn( canPortHandle handle )
{
  if (canBusOnFP)
  {
    canStatus kv_status = canBusOnFP( (int)handle );
    if ( kv_status == canOK )
      return CANOPEN_OK;
    else
      return CANOPEN_ERROR;
  }
  return CANOPEN_ERROR_DRIVER;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOff( canPortHandle handle )
{
  if (canBusOffFP)
  {
    canStatus kv_status = canBusOffFP( (int)handle );
    if ( kv_status == canOK )
      return CANOPEN_OK;
    else
      return CANOPEN_ERROR;
  }
  return CANOPEN_ERROR_DRIVER;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortWrite(canPortHandle handle,
                                        long id,
                                        void *msg,
                                        unsigned int dlc,
                                        unsigned int flags)
{
  if (canWriteFP)
  {
    unsigned int kv_flags = 0;
    if ( flags & CAN_MSG_EXT )
      kv_flags |= canMSG_EXT;
    if ( flags & CAN_MSG_RTR )
      kv_flags |= canMSG_RTR;

    canStatus kv_status = canWriteFP( (int)handle,
      id,
      msg,
      dlc,
      kv_flags);
    if ( kv_status == canOK )
      return CANOPEN_OK;
    else
      return CANOPEN_ERROR;
  }
  return CANOPEN_ERROR_DRIVER;
}

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortRead(canPortHandle handle,
                                        long *id,
                                        void *msg,
                                        unsigned int *dlc,
                                        unsigned int *flags)
{
  if (canReadFP)
  {
    unsigned int kv_flags = 0;
    DWORD kv_time = 0;

    canStatus kv_status = canReadFP( (int)handle,
      id,
      msg,
      dlc,
      &kv_flags,
      &kv_time);

    if ( kv_status == canOK )
    {
      if ( kv_flags & canMSG_ERROR_FRAME )
      {
        return CANOPEN_ERROR_NO_MESSAGE;
      }
      *flags = 0;
      if ( kv_flags & canMSG_EXT )
        *flags |= CAN_MSG_EXT;
      if ( kv_flags & canMSG_RTR )
        *flags |= CAN_MSG_RTR;
      return CANOPEN_OK;
    } 
    else if ( kv_status == canERR_NOMSG ) 
    {
      return CANOPEN_ERROR_NO_MESSAGE;
    } 
    else
    {
      return CANOPEN_ERROR;
    }
  }
  return CANOPEN_ERROR_DRIVER;
}

CANOPENLIB_HW_API   canOpenStatus  __stdcall canPortGetSerialNumber(canPortHandle handle, char *buffer, int bufferLen)
{
  memset(buffer, 0, bufferLen);
  canStatus kv_status = canGetChannelDataFP((int)handle, canCHANNELDATA_CARD_SERIAL_NO, buffer, bufferLen);
  u32 serial_number = buf2val((u8*)buffer, 2);
  sprintf_s(buffer, bufferLen, "%d", serial_number);
  if (kv_status == canOK)
    return CANOPEN_OK;
  else
    return CANOPEN_ERROR;
}