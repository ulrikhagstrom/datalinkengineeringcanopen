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

#ifndef CANLIB32_DYN_LOAD_H
#define CANLIB32_DYN_LOAD_H

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CANLIB32_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CANLIB32_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CANLIB32_EXPORTS
#define CANLIB32_API  /*extern "C"*/ __declspec(dllexport) 
#else
#define CANLIB32_API __declspec(dllimport)
#endif

#include <shlwapi.h>


#define CAN_EXT_FLAG 0x1
#define CAN_RTR_FLAG 0x2
typedef int canHandle;

#define canINVALID_HANDLE      (-1)

// Flags for canOpenChannel
// 0x01, 0x02, 0x04 are obsolete and reserved.
// The canWANT_xxx names are also obsolete, use canOPEN_xxx instead for new developments.
#define canWANT_EXCLUSIVE           0x0008
#define canWANT_EXTENDED            0x0010
#define canWANT_VIRTUAL             0x0020
#define canOPEN_EXCLUSIVE           canWANT_EXCLUSIVE
#define canOPEN_REQUIRE_EXTENDED    canWANT_EXTENDED
#define canOPEN_ACCEPT_VIRTUAL      canWANT_VIRTUAL
#define canOPEN_OVERRIDE_EXCLUSIVE  0x0040
#define canOPEN_REQUIRE_INIT_ACCESS 0x0080
#define canOPEN_NO_INIT_ACCESS      0x0100
#define canOPEN_ACCEPT_LARGE_DLC    0x0200  // DLC can be greater than 8

//
// CAN driver types - not all are supported on all cards.
//
#define canDRIVER_NORMAL           4
#define canDRIVER_SILENT           1
#define canDRIVER_SELFRECEPTION    8
#define canDRIVER_OFF              0
// 2,3,5,6,7 are reserved values for compatibility reasons.

// For canIOCTL_xxx_USER_IOPORT
typedef struct {
  unsigned int portNo;
  unsigned int portValue;
} canUserIoPortData;
// SSSOLS

#define canBITRATE_1M        (-1)
#define canBITRATE_500K      (-2)
#define canBITRATE_250K      (-3)
#define canBITRATE_125K      (-4)
#define canBITRATE_100K      (-5)
#define canBITRATE_62K       (-6)
#define canBITRATE_50K       (-7)
#define canBITRATE_83K       (-8)
#define canBITRATE_10K       (-9)

#define canFILTER_ACCEPT        1
#define canFILTER_REJECT        2
#define canFILTER_SET_CODE_STD  3
#define canFILTER_SET_MASK_STD  4
#define canFILTER_SET_CODE_EXT  5
#define canFILTER_SET_MASK_EXT  6

#define canCHANNELDATA_CHANNEL_CAP              1
#define canCHANNELDATA_TRANS_CAP                2
#define canCHANNELDATA_CHANNEL_FLAGS            3   // available, etc
#define canCHANNELDATA_CARD_TYPE                4   // canHWTYPE_xxx
#define canCHANNELDATA_CARD_NUMBER              5   // Number in machine, 0,1,...
#define canCHANNELDATA_CHAN_NO_ON_CARD          6
#define canCHANNELDATA_CARD_SERIAL_NO           7
#define canCHANNELDATA_TRANS_SERIAL_NO          8
#define canCHANNELDATA_CARD_FIRMWARE_REV        9
#define canCHANNELDATA_CARD_HARDWARE_REV        10
#define canCHANNELDATA_CARD_UPC_NO              11
#define canCHANNELDATA_TRANS_UPC_NO             12
#define canCHANNELDATA_CHANNEL_NAME             13
#define canCHANNELDATA_DLL_FILE_VERSION         14
#define canCHANNELDATA_DLL_PRODUCT_VERSION      15
#define canCHANNELDATA_DLL_FILETYPE             16
#define canCHANNELDATA_TRANS_TYPE               17
#define canCHANNELDATA_DEVICE_PHYSICAL_POSITION 18
#define canCHANNELDATA_UI_NUMBER                19
#define canCHANNELDATA_TIMESYNC_ENABLED         20
#define canCHANNELDATA_DRIVER_FILE_VERSION      21
#define canCHANNELDATA_DRIVER_PRODUCT_VERSION   22
#define canCHANNELDATA_MFGNAME_UNICODE          23
#define canCHANNELDATA_MFGNAME_ASCII            24
#define canCHANNELDATA_DEVDESCR_UNICODE         25
#define canCHANNELDATA_DEVDESCR_ASCII           26

#define canIOCTL_PREFER_EXT                     1
#define canIOCTL_PREFER_STD                     2
// 3,4 reserved.
#define canIOCTL_CLEAR_ERROR_COUNTERS           5
#define canIOCTL_SET_TIMER_SCALE                6
#define canIOCTL_SET_TXACK                      7
#define canIOCTL_GET_RX_BUFFER_LEVEL            8
#define canIOCTL_GET_TX_BUFFER_LEVEL            9
#define canIOCTL_FLUSH_RX_BUFFER                10
#define canIOCTL_FLUSH_TX_BUFFER                11
#define canIOCTL_GET_TIMER_SCALE                12
#define canIOCTL_SET_TXRQ                       13
#define canIOCTL_GET_EVENTHANDLE                14
#define canIOCTL_SET_BYPASS_MODE                15
#define canIOCTL_SET_WAKEUP                     16
#define canIOCTL_GET_DRIVERHANDLE               17
#define canIOCTL_MAP_RXQUEUE                    18
#define canIOCTL_GET_WAKEUP                     19
#define canIOCTL_SET_REPORT_ACCESS_ERRORS       20
#define canIOCTL_GET_REPORT_ACCESS_ERRORS       21
#define canIOCTL_CONNECT_TO_VIRTUAL_BUS         22
#define canIOCTL_DISCONNECT_FROM_VIRTUAL_BUS    23
#define canIOCTL_SET_USER_IOPORT                24
#define canIOCTL_GET_USER_IOPORT                25
#define canIOCTL_SET_BUFFER_WRAPAROUND_MODE     26
#define canIOCTL_SET_RX_QUEUE_SIZE              27
#define canIOCTL_SET_USB_THROTTLE               28
#define canIOCTL_GET_USB_THROTTLE               29


#define BAUD_1M        (-1)
#define BAUD_500K      (-2)
#define BAUD_250K      (-3)
#define BAUD_125K      (-4)
#define BAUD_100K      (-5)
#define BAUD_62K       (-6)
#define BAUD_50K       (-7)
#define BAUD_83K       (-8)

#define PUMACAN_BAUD_125K_75  0x1401 // tseg1 = 5, tseg2 = 2, brp=124/1=124, sp=6/8=75%
#define PUMACAN_BAUD_250K_75  0x1402 // tseg1 = 5, tseg2 = 2, brp=124/2=62, sp=6/8=75%
#define PUMACAN_BAUD_500K_75  0x1404 // tseg1 = 5, tseg2 = 2, brp=124/4=31, sp=6/8=75%
#define PUMACAN_BAUD_1000K_75 0x0104 // tseg1 = 2, tseg2 = 1, brp=124/4=31, sp=3/4=75%

#define canFILTER_NULL_MASK     0L
#define canINVALID_HANDLE      (-1)


// Retained for compatibility.
typedef void* (*canMemoryAllocator)  (size_t size);
typedef void  (*canMemoryDeallocator)(void* p);


typedef struct tagCanHWDescr 
{
    int                    circuitType;
    int                    cardType;
    int                    channel;
} canHWDescr;

typedef struct tagCanSWDescr 
{
    unsigned int           rxBufSize;
    unsigned int           txBufSize;
    canMemoryAllocator     alloc;
    canMemoryDeallocator   deAlloc;
} canSWDescr;


typedef enum {
    canOK                  = 0,
    canERR_PARAM           = -1,     // Error in parameter
    canERR_NOMSG           = -2,     // No messages available
    canERR_NOTFOUND        = -3,     // Specified hw not found
    canERR_NOMEM           = -4,     // Out of memory
    canERR_NOCHANNELS      = -5,     // No channels avaliable
    canERR_RESERVED_3      = -6,
    canERR_TIMEOUT         = -7,     // Timeout occurred
    canERR_NOTINITIALIZED  = -8,     // Lib not initialized
    canERR_NOHANDLES       = -9,     // Can't get handle
    canERR_INVHANDLE       = -10,    // Handle is invalid
    canERR_INIFILE         = -11,    // Error in the ini-file (16-bit only)
    canERR_DRIVER          = -12,    // CAN driver type not supported
    canERR_TXBUFOFL        = -13,    // Transmit buffer overflow
    canERR_RESERVED_1      = -14,
    canERR_HARDWARE        = -15,    // Some hardware error has occurred
    canERR_DYNALOAD        = -16,    // Can't find requested DLL
    canERR_DYNALIB         = -17,    // DLL seems to be wrong version
    canERR_DYNAINIT        = -18,    // Error when initializing DLL
    canERR_RESERVED_4      = -19,
    canERR_RESERVED_5      = -20,
    canERR_RESERVED_6      = -21,
    canERR_RESERVED_2      = -22,
    canERR_DRIVERLOAD      = -23,    // Can't find/load driver
    canERR_DRIVERFAILED    = -24,    // DeviceIOControl failed; use Win32 GetLastError()
    canERR_NOCONFIGMGR     = -25,    // Can't find req'd config s/w (e.g. CS/SS)
    canERR_NOCARD          = -26,    // The card was removed or not inserted
    canERR_RESERVED_7      = -27,
    canERR_REGISTRY        = -28,    // Error in the Registry
    canERR_LICENSE         = -29,    // The license is not valid.
    canERR_INTERNAL        = -30,    // Internal error in the driver.
    canERR_NO_ACCESS       = -31,    // Access denied
    canERR_NOT_IMPLEMENTED = -32,    // Requested function is not implemented

    // The last entry - a dummy so we know where NOT to place a comma.
    canERR__RESERVED       = -33
} canStatus;



typedef canStatus kvStatus;
typedef void (*kvCallback_t)(int handle, void* context, unsigned int notifyEvent);

#define canNOTIFY_NONE          0           // Turn notifications off.
#define canNOTIFY_RX            0x0001      // Notify on receive
#define canNOTIFY_TX            0x0002      // Notify on transmit
#define canNOTIFY_ERROR         0x0004      // Notify on error
#define canNOTIFY_STATUS        0x0008      // Notify on (some) status changes


#define PUMA_CAN_FLAGS_RTR         0x01
#define PUMA_CAN_FLAGS_ERROR_FRAME 0x02
#define PUMA_CAN_FLAGS_OVERRUN     0x04
#define PUMA_CAN_FLAGS_ECHO        0x08
#define PUMA_CAN_FLAGS_BLOCK       0x10 // Only when transmitting
#define PUMA_CAN_OVERLOADED        0x20
#define PUMA_CAN_RX_FLAG_MASK ( PUMA_CAN_FLAGS_RTR         | \
                                PUMA_CAN_FLAGS_ERROR_FRAME | \
							                  PUMA_CAN_FLAGS_OVERRUN     | \
								                PUMA_CAN_FLAGS_ECHO        | \
								                PUMA_CAN_OVERLOADED )  

#define PUMA_CAN_ID_INVALID 0xffffffffL
#define PUMA_CAN_ID_MASK 0x1fffffffL // Masks the id bits only
#define PUMA_CAN_ID_EXTENDED 0x80000000L // Bit set in extended CAN id's


//
// Message information flags, < 0x100
// All flags and/or combinations of them are meaningful for received messages
// RTR,STD,EXT,WAKEUP,ERROR_FRAME are meaningful also for transmitted messages
//
#define canMSG_MASK             0x00ff      // Used to mask the non-info bits
#define canMSG_RTR              0x0001      // Message is a remote request
#define canMSG_STD              0x0002      // Message has a standard ID
#define canMSG_EXT              0x0004      // Message has an extended ID
#define canMSG_WAKEUP           0x0008      // Message to be sent / was received in wakeup mode
#define canMSG_NERR             0x0010      // NERR was active during the message
#define canMSG_ERROR_FRAME      0x0020      // Message is an error frame
#define canMSG_TXACK            0x0040      // Message is a TX ACK (msg is really sent)
#define canMSG_TXRQ             0x0080      // Message is a TX REQUEST (msg is transfered to the chip)

//
// Message error flags, >= 0x0100
//
#define canMSGERR_MASK          0xff00      // Used to mask the non-error bits
// 0x0100 reserved
#define canMSGERR_HW_OVERRUN    0x0200      // HW buffer overrun
#define canMSGERR_SW_OVERRUN    0x0400      // SW buffer overrun
#define canMSGERR_STUFF         0x0800      // Stuff error
#define canMSGERR_FORM          0x1000      // Form error
#define canMSGERR_CRC           0x2000      // CRC error
#define canMSGERR_BIT0          0x4000      // Sent dom, read rec
#define canMSGERR_BIT1          0x8000      // Sent rec, read dom

//
// Convenience values for the message error flags.
//
#define canMSGERR_OVERRUN       0x0600      // Any overrun condition.
#define canMSGERR_BIT           0xC000      // Any bit error.
#define canMSGERR_BUSERR        0xF800      // Any RX error

typedef struct canBusStatistics_s {
  unsigned long  stdData;
  unsigned long  stdRemote;
  unsigned long  extData;
  unsigned long  extRemote;
  unsigned long  errFrame;      // Error frames
  unsigned long  busLoad;       // 0 .. 10000 meaning 0.00-100.00%
  unsigned long  overruns;
} canBusStatistics;

typedef void *kvTimeDomain;

typedef struct kvTimeDomainData_s {
  int qqq;
} kvTimeDomainData;

#ifdef __cplusplus
extern "C" {
#endif

CANLIB32_API HRESULT CALLBACK DllGetVersion(DLLVERSIONINFO *pdvi);
CANLIB32_API void __stdcall canInitializeLibrary(void) ;
CANLIB32_API void __stdcall canLocateHardware(void);

CANLIB32_API canStatus __stdcall canClose(const int handle);
CANLIB32_API canStatus __stdcall canBusOn(const int handle); 
CANLIB32_API canStatus __stdcall canBusOff(const int handle);


CANLIB32_API canStatus __stdcall canSetBusParams(const int handle,
                           long freq,
                           unsigned int tseg1,
                           unsigned int tseg2,
                           unsigned int sjw,
                           unsigned int noSamp,
                           unsigned int syncmode);

CANLIB32_API canStatus __stdcall canGetBusParams(const int handle,
                              long * freq,
                              unsigned int * tseg1,
                              unsigned int * tseg2,
                              unsigned int * sjw,
                              unsigned int * noSamp,
                              unsigned int * syncmode);



CANLIB32_API canStatus __stdcall canSetBusOutputControl(const int handle,
                                     const unsigned int drivertype);

CANLIB32_API canStatus __stdcall canGetBusOutputControl(const int handle,
                                     unsigned int * drivertype);
CANLIB32_API canStatus __stdcall canAccept(const int handle,
								const long envelope,
								const unsigned int flag);
CANLIB32_API canStatus __stdcall canReadStatus(const int handle,
                            unsigned long * const flags);
CANLIB32_API canStatus __stdcall canReadErrorCounters(int handle,
                               unsigned int * txErr,
                               unsigned int * rxErr,
                               unsigned int * ovErr);

CANLIB32_API canStatus __stdcall canWrite(int handle, long id, void * msg,
                       unsigned int dlc, unsigned int flag);
CANLIB32_API canStatus __stdcall canWriteSync(int handle, unsigned long timeout);

CANLIB32_API canStatus __stdcall canRead(int handle,
                      long * id,
                      void * msg,
                      unsigned int * dlc,
                      unsigned int * flag,
                      unsigned long * time);

CANLIB32_API canStatus __stdcall canReadWait(int handle,
                          long * id,
                          void * msg,
                          unsigned int * dlc,
                          unsigned int * flag,
                          unsigned long * time,
                          unsigned long timeout);

CANLIB32_API canStatus __stdcall canReadSpecific(int handle, long id, void * msg,
                              unsigned int * dlc, unsigned int * flag,
                              unsigned long * time);

CANLIB32_API canStatus __stdcall canReadSync(int handle, unsigned long timeout);

CANLIB32_API canStatus __stdcall canReadSyncSpecific(int handle, long id, unsigned long timeout);

CANLIB32_API canStatus __stdcall canReadSpecificSkip(int hnd,
                                  long id,
                                  void * msg,
                                  unsigned int * dlc,
                                  unsigned int * flag,
                                  unsigned long * time);

CANLIB32_API canStatus __stdcall canSetNotify(int handle, HWND aHWnd, unsigned int aNotifyFlags);

CANLIB32_API canStatus __stdcall canTranslateBaud(long * const freq,
                               unsigned int * const tseg1,
                               unsigned int * const tseg2,
                               unsigned int * const sjw,
                               unsigned int * const nosamp,
                               unsigned int * const syncMode);

CANLIB32_API canStatus __stdcall canGetErrorText(int err, char * buf, unsigned int bufsiz);

CANLIB32_API unsigned short __stdcall canGetVersion(void);

CANLIB32_API canStatus __stdcall canIoCtl(int handle, unsigned int func,
                       void * buf, unsigned int buflen);

CANLIB32_API unsigned long __stdcall canReadTimer(int hnd);

CANLIB32_API canHandle __stdcall canOpenChannel(unsigned int channel, unsigned int flags);

CANLIB32_API canHandle __stdcall canOpen ( canHWDescr *hwdescr, canSWDescr* swdescr, unsigned short flags);


CANLIB32_API canStatus __stdcall canGetNumberOfChannels(int * channelCount);

CANLIB32_API canStatus __stdcall canGetChannelData(int channel, int item, void *buffer, size_t bufsize);

CANLIB32_API canStatus __stdcall canWaitForEvent(int hnd, DWORD timeout);

CANLIB32_API canStatus __stdcall canSetBusParamsC200(int hnd, BYTE btr0, BYTE btr1);

CANLIB32_API canStatus __stdcall canSetDriverMode(int hnd, int lineMode, int resNet);

CANLIB32_API canStatus __stdcall canGetDriverMode(int hnd, int *lineMode, int *resNet);

CANLIB32_API unsigned int __stdcall canGetVersionEx(unsigned int itemCode);

CANLIB32_API canStatus __stdcall canParamGetCount (void);

CANLIB32_API canStatus __stdcall canParamCommitChanges (void);

CANLIB32_API canStatus __stdcall canParamDeleteEntry (int index);

CANLIB32_API canStatus __stdcall canParamCreateNewEntry (void);

CANLIB32_API canStatus __stdcall canParamSwapEntries (int index1, int index2);

CANLIB32_API canStatus __stdcall canParamGetName (int index, char *buffer, int maxlen);

CANLIB32_API canStatus __stdcall canParamGetChannelNumber (int index);

CANLIB32_API canStatus __stdcall canParamGetBusParams (int index,
                                          long* bitrate,
                                          unsigned int *tseg1,
                                          unsigned int *tseg2,
                                          unsigned int *sjw,
                                          unsigned int *noSamp);

CANLIB32_API canStatus __stdcall canParamSetName (int index, const char *buffer);
CANLIB32_API canStatus __stdcall canParamSetChannelNumber (int index, int channel);

CANLIB32_API canStatus __stdcall canParamSetBusParams (int index,
                                          long bitrate,
                                          unsigned int tseg1,
                                          unsigned int tseg2,
                                          unsigned int sjw,
                                          unsigned int noSamp);
CANLIB32_API canStatus __stdcall canParamFindByName (const char *name);
CANLIB32_API canStatus __stdcall canObjBufFreeAll(int handle);
CANLIB32_API canStatus __stdcall canObjBufAllocate(int handle, int type);
CANLIB32_API canStatus __stdcall canObjBufFree(int handle, int idx);
CANLIB32_API canStatus __stdcall canObjBufWrite(int handle, int idx, int id, void* msg,
                                   unsigned int dlc, unsigned int flags);
CANLIB32_API canStatus __stdcall canObjBufSetFilter(int handle, int idx,
                                       unsigned int code, unsigned int mask);
CANLIB32_API canStatus __stdcall canObjBufSetFlags(int handle, int idx, unsigned int flags);
CANLIB32_API canStatus __stdcall canObjBufSetPeriod(int hnd, int idx, unsigned int period);
CANLIB32_API canStatus __stdcall canObjBufEnable(int handle, int idx);
CANLIB32_API canStatus __stdcall canObjBufDisable(int handle, int idx);
CANLIB32_API canStatus __stdcall canObjBufSendBurst(int hnd, int idx, unsigned int burstlen);
CANLIB32_API canStatus __stdcall canResetBus(int handle);
CANLIB32_API canStatus __stdcall canWriteWait(int handle, long id, void * msg,
                                 unsigned int dlc, unsigned int flag,
                                 unsigned long timeout);
CANLIB32_API canStatus __stdcall canUnloadLibrary(void);
CANLIB32_API canStatus __stdcall canSetAcceptanceFilter(int hnd, unsigned int code,
                                           unsigned int mask, int is_extended);
CANLIB32_API canStatus __stdcall canFlushReceiveQueue(int hnd);
CANLIB32_API canStatus __stdcall canFlushTransmitQueue(int hnd);


CANLIB32_API canStatus __stdcall kvGetApplicationMapping(int busType,
                                  char *appName,
                                  int appChannel,
                                  int *resultingChannel);
CANLIB32_API canStatus __stdcall kvBeep(int hnd, int freq, unsigned int duration);
CANLIB32_API canStatus __stdcall kvSelfTest(int hnd, unsigned long *presults);
CANLIB32_API canStatus __stdcall kvFlashLeds(int hnd, int action, int timeout);
CANLIB32_API canStatus __stdcall canRequestChipStatus(int hnd);
CANLIB32_API canStatus __stdcall canRequestBusStatistics(int hnd);
CANLIB32_API canStatus __stdcall canGetBusStatistics(int hnd, canBusStatistics *stat, size_t bufsiz);
CANLIB32_API canStatus __stdcall canSetBitrate(int hnd, int bitrate);
CANLIB32_API canStatus __stdcall kvAnnounceIdentity(int hnd, void *buf, size_t bufsiz);
CANLIB32_API canStatus __stdcall canGetHandleData(int hnd, int item, void *buffer, size_t bufsize);
CANLIB32_API int __stdcall kvTimeDomainCreate(kvTimeDomain *domain);
CANLIB32_API int __stdcall kvTimeDomainDelete(kvTimeDomain domain);
CANLIB32_API int __stdcall kvTimeDomainResetTime(kvTimeDomain domain);
CANLIB32_API int __stdcall kvTimeDomainGetData(kvTimeDomain domain, kvTimeDomainData *data, size_t bufsiz);
CANLIB32_API int __stdcall kvTimeDomainAddHandle(kvTimeDomain domain, int handle);
CANLIB32_API int __stdcall kvTimeDomainRemoveHandle(kvTimeDomain domain, int handle);


CANLIB32_API BOOL __stdcall canProbeVersion( int hnd, int major, int minor, int oem_id,  unsigned int flags);


/* type def int == canStatus in kvaser API, which is an int! */
typedef int (__stdcall *DllGetVersionFP)(DLLVERSIONINFO2 *); 
typedef int (__stdcall *CanInitializeLibraryFP)(void);
typedef canStatus (__stdcall *CanBusOnFP)(const int);
typedef canStatus (__stdcall *CanBusOffFP)(const int);
typedef canStatus (__stdcall *CanSetBusParamsFP)(int, long, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
typedef canStatus (__stdcall *CanReadFP)(int, long *, void *, unsigned int *, unsigned int *, unsigned long *);
typedef canStatus (__stdcall *CanWriteFP)(int, long, void *, unsigned int, unsigned int);
typedef canStatus (__stdcall *CanCloseFP)(const int);
typedef canStatus (__stdcall *CanGetBusParamsFP)(const int, long *, unsigned int *, unsigned int *, unsigned int *, unsigned int *, unsigned int *);
typedef canStatus (__stdcall *CanSetBusOutputControlFP)(const int, const unsigned int);
typedef canStatus (__stdcall *CanGetBusOutputControlFP)(const int, unsigned int *);
typedef canStatus (__stdcall *CanAcceptFP)(const int, const long, const unsigned int);
typedef canStatus (__stdcall *CanReadStatusFP)(const int, unsigned long * const);
typedef canStatus (__stdcall *CanReadErrorCountersFP)(int, unsigned int *, unsigned int *, unsigned int *);
typedef canStatus (__stdcall *CanWriteSyncFP)(int, unsigned long);
typedef canStatus (__stdcall *CanReadWaitFP)(int, long *, void *, unsigned int *, unsigned int *, unsigned long *, unsigned long);
typedef canStatus (__stdcall *CanReadSpecificFP)(int, long, void *, unsigned int *, unsigned int *, unsigned long *);
typedef canStatus (__stdcall *CanReadSyncFP)(int, unsigned long);
typedef canStatus (__stdcall *CanReadSyncSpecificFP)(int, long, unsigned long);
typedef canStatus (__stdcall *CanReadSpecificSkipFP)(int, long, void *, unsigned int *, unsigned int *, unsigned long *);
typedef canStatus (__stdcall *CanSetNotifyFP)(int, HWND, unsigned int);
typedef canStatus (__stdcall *CanTranslateBaudFP)(long * const,unsigned int * const, unsigned int * const, unsigned int * const, unsigned int * const, unsigned int * const);
typedef canStatus (__stdcall *CanGetErrorTextFP)(int, char *, unsigned int);
typedef canStatus (__stdcall *CanGetVersionFP)(void);
typedef canStatus (__stdcall *CanIoCtlFP)(int, unsigned int, void *, unsigned int);
typedef unsigned long (__stdcall *CanReadTimerFP)(int);
typedef int (__stdcall *CanOpenChannelFP)(int, int);
typedef canStatus (__stdcall *CanGetNumberOfChannelsFP)(int *);
typedef canStatus (__stdcall *CanGetChannelDataFP)(int, int, void *, size_t);
typedef canStatus (__stdcall *CanWaitForEventFP)(int, DWORD);
typedef canStatus (__stdcall *CanSetBusParamsC200FP)(int, BYTE, BYTE);
typedef canStatus (__stdcall *CanSetDriverModeFP)(int, int, int);
typedef canStatus (__stdcall *CanGetDriverModeFP)(int, int *, int *);
typedef canStatus (__stdcall *CanGetVersionExFP)(unsigned int);
typedef canStatus (__stdcall *CanParamGetCountFP)(void);
typedef canStatus (__stdcall *CanParamCommitChangesFP)(void);
typedef canStatus (__stdcall *CanParamDeleteEntryFP)(int);
typedef canStatus (__stdcall *CanParamCreateNewEntryFP)(void);
typedef canStatus (__stdcall *CanParamSwapEntriesFP)(int, int);
typedef canStatus (__stdcall *CanParamGetNameFP)(int, char *, int);
typedef canStatus (__stdcall *CanParamGetChannelNumberFP)(int);
typedef canStatus (__stdcall *CanParamGetBusParamsFP)(int, long*, unsigned int *, unsigned int *, unsigned int *, unsigned int *);
typedef canStatus (__stdcall *CanParamSetNameFP)(int, const char *);
typedef canStatus (__stdcall *CanParamSetChannelNumberFP)(int, int);
typedef canStatus (__stdcall *CanParamSetBusParamsFP)(int, long, unsigned int, unsigned int, unsigned int, unsigned int);
typedef canStatus (__stdcall *CanParamFindByNameFP)(const char *);
typedef canStatus (__stdcall *CanObjBufFreeAllFP)(int);
typedef canStatus (__stdcall *CanObjBufAllocateFP)(int, int);
typedef canStatus (__stdcall *CanObjBufFreeFP)(int, int);
typedef canStatus (__stdcall *CanObjBufWriteFP)(int, int, int, void*, unsigned int, unsigned int);
typedef canStatus (__stdcall *CanObjBufSetFilterFP)(int, int, unsigned int, unsigned int);
typedef canStatus (__stdcall *CanObjBufSetFlagsFP)(int, int, unsigned int);
typedef canStatus (__stdcall *CanObjBufSetPeriodFP)(int, int, unsigned int);
typedef canStatus (__stdcall *CanObjBufEnableFP)(int, int);
typedef canStatus (__stdcall *CanObjBufDisableFP)(int, int);
typedef canStatus (__stdcall *CanObjBufSendBurstFP)(int, int, unsigned int);
typedef canStatus (__stdcall *CanResetBusFP)(int);
typedef canStatus (__stdcall *CanWriteWaitFP)(int, long, void *, unsigned int, unsigned int, unsigned long);
typedef canStatus (__stdcall *CanUnloadLibraryFP)(void);
typedef canStatus (__stdcall *CanSetAcceptanceFilterFP)(int, unsigned int, unsigned int, int);
typedef canStatus (__stdcall *CanFlushReceiveQueueFP)(int);
typedef canStatus (__stdcall *CanFlushTransmitQueueFP)(int);
typedef canStatus (__stdcall *KvGetApplicationMappingFP)(int, char *, int, int *);
typedef canStatus (__stdcall *KvBeep)(int, int, unsigned int);
typedef canStatus (__stdcall *KvSelfTest)(int, unsigned long *);
typedef canStatus (__stdcall *KvFlashLeds)(int, int, int);
typedef canStatus (__stdcall *CanRequestChipStatus)(int);
typedef canStatus (__stdcall *CanRequestBusStatistics)(int);
typedef canStatus (__stdcall *CanGetBusStatistics)(int, canBusStatistics *, size_t);
typedef canStatus (__stdcall *CanSetBitrate)(int, int);
typedef canStatus (__stdcall *KvAnnounceIdentity)(int, void *, size_t);
typedef canStatus (__stdcall *CanGetHandleData)(int, int, void *, size_t);
typedef canStatus (__stdcall *KvTimeDomainCreate)(kvTimeDomain *);
typedef canStatus (__stdcall *KvTimeDomainDelete)(kvTimeDomain);
typedef canStatus (__stdcall *KvTimeDomainResetTime)(kvTimeDomain);
typedef canStatus (__stdcall *KvTimeDomainGetData)(kvTimeDomain, kvTimeDomainData *, size_t);
typedef canStatus (__stdcall *KvTimeDomainAddHandle)(kvTimeDomain, int);
typedef canStatus (__stdcall *KvTimeDomainRemoveHandle)(kvTimeDomain, int);
typedef canStatus (__stdcall *KvSetNotifyCallback)(int, kvCallback_t, void*, unsigned int);


#ifdef __cplusplus
}
#endif

#endif