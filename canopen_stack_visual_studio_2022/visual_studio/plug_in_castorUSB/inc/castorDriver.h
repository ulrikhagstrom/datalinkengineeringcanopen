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


#ifndef __castorUSBDriver__h_file
#define __castorUSBDriver__h_file

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CASTORDRIVER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CASTORDRIVER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CASTORDRIVER_EXPORTS
#define CASTORDRIVER_API extern "C" __declspec(dllexport)
#else
#define CASTORDRIVER_API extern "C" __declspec(dllimport)
#endif

typedef int castorUSBHandle;

/// NOTE: If you are using this function, your callback function must be compiled using __stdcall calling conventions
///		  or you may/will get a Access Violation exception from the driver
typedef int (*CanRxFunction)(void *context, long id, char *data, unsigned int dlc, unsigned int flags, unsigned long time);

//
// Busspeedconstant for selecting predefined baud rates 
//

// Busspeed
#define canBUSSPEED1000K 		1	
#define canBUSSPEED800K			2
#define canBUSSPEED500K 		3	
#define canBUSSPEED250K 		4	
#define canBUSSPEED125K 		5	
#define canBUSSPEED100K 		6	
#define canBUSSPEED50K			7	
#define canBUSSPEED25K			8

//
// CAN messages flags
//
#define canMSG_STANDARD_IDENTIFIER_FLG	1
#define canMSG_EXTENDED_IDENTIFIER_FLG  2
#define canMSG_RTR_FLG					4
#define canMSG_TX_ACKNOWLEDGE_FLG		8

//
// These are used in function canReadStatus().
//
#define canSTAT_ERROR_PASSIVE   0x00000001	// Bus passive
#define canSTAT_BUS_OFF         0x00000002	// Bus off
#define canSTAT_ERROR_WARNING   0x00000004  // Hardware have notified some errors
#define canSTAT_TX_PENDING      0x00000008  // Message in transmit queue 
#define canSTAT_RX_PENDING      0x00000010  // Message in receive queue
#define canSTAT_TXERR           0x00000020  // At least one Transmit error occourred
#define canSTAT_RXERR           0x00000040  // At least one Receive error occourred
#define canSTAT_SW_OVERRUN      0x00000080  // Software overrun
#define canSTAT_SW_ERROR		0x00000100  // CastorApi/FW errors
#define canSTAT_BADDATA			0x00000200	// Bad data has been noticed on SCI

//
// These are used int function canIoCtl.
//
#define canIOCTL_SET_TXACK		0x01		// This will change the Transmitt Acknowledge
											// Values 0 = turns off, 1 turns on
#define canIOCTL_SET_SILENT		0x02		// This will change the Silent mode
											// Values 0 = NON_SILENT, 1 = SILENT mode

//
// CANlinkStatus messages
//
typedef enum canLinkStatus
{
    castorOK                  = 0,
    castorERR_PARAM           = -1,     // Error in parameter
    castorERR_NOMSG           = -2,     // No messages available
    castorERR_NOTFOUND        = -3,     // Specified hw not found
    castorERR_NOMEM           = -4,     // Out of memory
    castorERR_NOCHANNELS      = -5,     // No channels available
    castorERR_CHANNEL_NOT_OPENED      = -6, // Device/Channel not open
    castorERR_TIMEOUT         = -7,     // Timeout occurred
    castorERR_NOTINITIALIZED  = -8,     // Lib not initialized
    castorERR_NOHANDLES       = -9,     // Can't get handle
    castorERR_INVHANDLE       = -10,    // Handle is invalid
    castorERR_DRIVER          = -11,    // CAN driver type not supported
    castorERR_TXBUFOFL        = -12,    // Transmit buffer overflow
    castorERR_HARDWARE        = -13,    // Some hardware error has occurred
    castorERR_DYNALOAD        = -14,    // Can't find requested DLL
    castorERR_DYNALIB         = -15,    // DLL seems to be wrong version
    castorERR_DYNAINIT        = -16,    // Error when initializing DLL
    castorERR_NOT_SUPPORTED   = -17,	 // Not supported
    castorERR_ALREADY_OPEN    = -18,
    castorERR_THREADS_BLOCKING = -19,
    castorERR_RESERVED_4      = -20,
    castorERR_DRIVERLOAD      = -21,    // Can't find/load driver
    castorERR_DRIVERFAILED    = -22,    
    castorERR_RESERVED_5      = -23,    
    castorERR_NOHARDWARE      = -24,    // The hw was removed or not inserted
    castorERR_LICENSE         = -25,    // The license is not valid.
    castorERR_INTERNAL        = -26,    // Internal error in the driver.
    castorERR_NO_ACCESS       = -27,    // Access denied
    castorERR_NOT_IMPLEMENTED = -28,    // Requested function is not implemented
	castorERR_OTHER			= -29,	// Undefined error
	castorERR_INVBUSSSPEED		= -30,	// Hardware doesn't support busspeed

    castorERR__RESERVED       = -31
} castorStatus;

//
// Exported Funktions
//
///<summary><c>canInitializeLib</c> This function initializes the driver and must be called before any other function is used.</summary>
///<param name=''></param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canInitializeLib(void);

///<summary><c>canGetNumberOfChannels</c> retruns the number of available channels.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canGetNumberOfChannels( int* channelCount );

///<summary><c>canGetDescription</c> gets the description or name of selected device.</summary>
///<param name='channelNr'> Numer of wanted channel.</param>
///<param name='chDescription'> Char array of enought lenght, where the description is copied.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canGetDescription( int channelNr, char *chDescription  );

//******************************************************************
// canOpenChannel
// Returns -1 if open channel not succeeded
//******************************************************************
///<summary><c>canOpenChannel</c> opens a device for communication.</summary>
///<param name='CANlinkDev'> Type integer, hardware channel/device.</param>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canOpenChannel( int channelNr, castorUSBHandle *handle );

///<summary><c>canClose</c> Closes the handle to castorUSB.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canClose( castorUSBHandle handle );

///<summary><c>canSetBitRate</c> Sets the CAN-bus speed to a pre-defined bit rate.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<param name='BusSpeedConstant'> Type integer, use pre-defined bitrates to get desired speed.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canSetBitRate(castorUSBHandle handle, int BusSpeedConstant );

///<summary><c>canBusOn</c> makes the castorUSB to go on bus.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canBusOn( castorUSBHandle handle );

///<summary><c>canBusOff</c> makes the castorUSB to go off bus.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canBusOff( castorUSBHandle handle );

///<summary><c>canWrite</c> Sends a message to the CAN-bus.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<param name='id'> Type long, CAN identifier, standard/extended.</param>
///<param name='msg'> Type char pointer, message-data array, 8 chars long.</param>
///<param name='dlc'> Type unsigned int, data length code, the number of data bytes in a message.</param>
///<param name='flag'> Type unsigned int, defines message type, see CAN message flags (can_MSGxxx).</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canWrite( castorUSBHandle handle, long id, char* msg, unsigned int dlc, unsigned int flag );

///<summary><c>canRead</c> Reads a message from the CAN-bus.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<param name='id'> Type long pointer, CAN identifier, standard/extended.</param>
///<param name='msg'> Type char pointer, message-data array, 8 chars long.</param>
///<param name='dlc'> Type unsigned int pointer, data length code, the number of data bytes in a message.</param>
///<param name='flag'> Type unsigned int pointer, defines message type, see CAN message flags (can_MSGxxx).</param>
///<param name='time'> Type unsigned long pointer, message timestamp.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canRead( castorUSBHandle handle, long* id, char* msg, unsigned int* dlc, unsigned int* flag, unsigned long* time );

///<summary><c>canReadWait</c> Reads a message from the CAN-bus.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<param name='id'> Type long pointer, CAN identifier, standard/extended.</param>
///<param name='msg'> Type char pointer, message-data array, 8 chars long.</param>
///<param name='dlc'> Type unsigned int pointer, data length code, the number of data bytes in a message.</param>
///<param name='flag'> Type unsigned int pointer, defines message type, see CAN message flags (can_MSGxxx).</param>
///<param name='time'> Type unsigned long pointer, message timestamp.</param>
///<param name='timeout'> Type unsigned long, waiting for new message or timeout.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canReadWait(castorUSBHandle handle, long* id, char* msg, unsigned int* dlc, unsigned int* flag, unsigned long* time, unsigned long timeout);

///<summary><c>canReadQueueCounters</c> reads how many are in queue still.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<param name='txQueueCount'> Type unsigned long pointer, transmitter queue.</param>
///<param name='rxQueueCount'> Type unsigned long pointer, receiver queue.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canReadQueueCounters( castorUSBHandle handle, unsigned long* txQueueCount, unsigned long* rxQueueCount );

///<summary><c>canFlushReceivedQueue</c> flushing/purging the received queue.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canFlushReceiveQueue(castorUSBHandle handle);

///<summary><c>canFlushTransmitQueue</c> flushing/purging the transmit queue.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canFlushTransmitQueue(castorUSBHandle handle);

///<summary><c>canGetSerialNumber</c> Gets serial number of device.</summary>
///<param name='DevNumber'> Type integer, gets serialnumber of desired device.</param>
///<param name='msg'> Type char pointer, pointer to a enough long character array.</param>
///<param name='length'> Type integer, length of msg.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canGetSerialNumber( castorUSBHandle handle, char* msg, int length  );

///<summary><c>canGetVersion</c> Gets the version of the API.</summary>
///<param name=''></param>
///<returns>unsigned short</returns>
///<remarks>Not implemented</remarks>
CASTORDRIVER_API castorStatus __stdcall canGetFwVersion( castorUSBHandle handle, unsigned long *ulVersion );

///<summary><c>canReadStatus</c> Is used to get the status of the CAN-interface. See status flags (can_STATxxx).</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<param name='flags'> Type unsigned long, a pointer to the status flags of the CAN-interface.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canReadStatus( castorUSBHandle handle, unsigned long* flags );

///<summary><c>canIoCtl</c> This API call performs several different functions; these are described below. The functions are handle-specific unless otherwise noted; this means that they affect only the handle you pass to canIoCtl(), whereas other open handles will remain unaffected. .</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<param name='func'> Type int, A canIOCTL_xxx function code.</param>
///<param name='buf'> Type void pointer, Pointer to a buffer containing function-dependent data; or a NULL pointer for certain function codes. The buffer can be used for both input and output depending on the function code.</param>
///<param name='buflen'> Type unsigned int, The length of the buffer.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canIoCtl(castorUSBHandle handle, unsigned int func, void* buf, unsigned int buflen);

///<summary><c>canReadHWErrorCounters</c> reads how many are in queue still.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<param name='txErrors'> Type unsigned long pointer, transmitted errors counter.</param>
///<param name='rxErrors'> Type unsigned long pointer, received errors counter.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canReadHWErrorCounters( castorUSBHandle handle, unsigned int* txErrors, unsigned int* rxErrors );

CASTORDRIVER_API castorStatus __stdcall canSetAcceptanceFilters(castorUSBHandle handle, unsigned long code, unsigned long mask, int is_extended);
CASTORDRIVER_API unsigned long __stdcall canGetVersion(void);

///<summary><c>canReadTime</c> Gets the time stamp from castor.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<returns>unsigned long</returns>
CASTORDRIVER_API unsigned long __stdcall canReadTime(castorUSBHandle handle);

/* If called with non-NULL function pointer, the supplied function will be
* called for each received CAN message. If a zero value is returned, the
* message will not be placed in the receive queue.
* This function can be used as a filter to decide what messages to let
* canReadMessage() receive.
*/
///<summary><c>canSetRxCallback</c> Set a callback function for received messages.
/// NOTE: If you are using this function, your callback function must be compiled using __stdcall calling conventions
///		  or you may/will get a Access Violation exception from the driver.</summary>
///<param name='handle'> Type castorUSBHandle, a handle to castorUSB CAN-interface.</param>
///<param name='context'>Desired context for functions that are called.</param>
///<param name='fun'>Pointer to function to be called.</param>
///<returns>castorStatus</returns>
CASTORDRIVER_API castorStatus __stdcall canSetRxCallback(castorUSBHandle handle, void *context, CanRxFunction fun);

#endif