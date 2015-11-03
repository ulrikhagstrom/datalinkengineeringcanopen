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

#ifndef CLIENT_SDO_CLASS_NET_H
#define CLIENT_SDO_CLASS_NET_H

#pragma once

#include <windows.h>


using namespace System;

/*namespace CANOPEN_DLL_EXPORT_NET {

    public ref class Class1
    {
        // TODO: Add your methods for this class here.
    };
}*/
#include <STDIO.H>									// needed for NULL
using namespace System::Runtime::InteropServices;

#include "ClientSDOClass.h"
#include "ServerSDOClass.h"
#include "NMTMasterClass.h"
#include "NMTSlaveClass.h"
#include "CanMonitorClass.h"
#include "CanInterfaceClass.h"
#include "ReceivePDOClass.h"
#include "TransmitPdoClass.h"
#include "EmcyClientClass.h"
#include "EmcyServerClass.h"
#include "SyncServerClass.h"
#include "SyncProducerClass.h"
#include "LSSMasterClass.h"
#include "LSSSlaveClass.h"

public enum class CanOpenStatus {     
    CANOPEN_OK                             =  0,
    CANOPEN_ARG_ERROR                      = -1,
    CANOPEN_UNEXPECTED_SCS                 = -2,
    CANOPEN_UNEXPECTED_CCS                 = -3,
    CANOPEN_TIMEOUT                        = -4,
    CANOPEN_MSG_NOT_PROCESSED              = -5,
    CANOPEN_TOGGLE                         = -6,
    CANOPEN_BUFFER_TOO_SMALL               = -7,
    CANOPEN_OBJECT_MISSMATCH               = -8,
    CANOPEN_APP_BUF_OUT_OF_RANGE           = -9,
    CANOPEN_REMOTE_NODE_ABORT              = -10,
    CANOPEN_OUT_OF_MEM                     = -11,
    CANOPEN_ERR_DEFINED_BY_APPL            = -12,
    CANOPEN_CMD_SPEC_UNKNOWN_OR_INVALLD    = -13,
    CANOPEN_INTERNAL_STATE_ERROR           = -14,
    CANOPEN_NODE_ID_OUT_OF_RANGE           = -15,
    CANOPEN_CAN_LAYER_FAILED               = -100,
    CANOPEN_ERROR_CAN_LAYER                = -101,
    CANOPEN_ERROR_CAN_LAYER_OVRFLOW        = -102,
    CANOPEN_ERROR_HW_CONNECTED             = -103,
    CANOPEN_ERROR_HW_NOT_CONNECTED         = -104,
    CANOPEN_ERROR_HW_UNDEFINED             = -105,
    CANOPEN_ERROR_DRIVER                   = -106,
    CANOPEN_OUT_OF_CAN_INTERFACES          = -107,
    CANOPEN_UNSUPPORTED_BITRATE            = -108,
    CANOPEN_ERROR_NO_MESSAGE               = -109,
    CANOPEN_ERROR_NOT_IMPLEMENTED          = -110,
    CANOPEN_ERROR_FILE_NOT_FOUND           = -111,
    CANOPEN_ERROR_NOT_EDS_DCF_CONFIGURED   = -112,
    CANOPEN_ASYNC_TRANSFER                 = -113,
    CANOPEN_ERROR_ASYNC_TRANSFER_ENABLED   = -114,
    CANOPEN_ERROR                          = -255 
};



public ref class OBJECT_ATTRIBUTES_NET 
{
public:
    static const u16 OBJECT_READABLE		= 0x1;
    static const u16 OBJECT_WRITEABLE		= 0x2;
    static const u16 OBJECT_MAPPABLE		= 0x4;
};

public ref class CanMessageTypes 
{
public:
    static const u32 CAN_MSG_RTR_FLAG		= 0x0001;
    static const u32 CAN_MSG_EXT_FLAG		= 0x0002;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


public delegate CanOpenStatus SrvReadDelegate( System::Object^ obj, u16 object_index, u8 sub_index, array<System::Byte>^ data,
                                                       [OutAttribute] u32 %valid,
                                                       [OutAttribute] u32 %canopenErrorCode); //Read from srv's application.
public delegate CanOpenStatus SrvWriteDelegate( System::Object^ obj, 
                                                        u16 object_index, 
                                                        u8 sub_index, 
                                                        array<System::Byte>^ data,
                                                        u32 valid,
                                                        [OutAttribute] u32 %canopenErrorCode); //Write to srv's application.
public delegate CanOpenStatus SrvGetAttrDelegate( System::Object^ obj, 
                                                          u16 object_index, 
                                                          u8 sub_index, 
                                                          [OutAttribute] u16 %flags );


public delegate void CliReadResultDelegate( System::Object^ obj, CanOpenStatus status, u8 node_id, u16 object_index, u8 sub_index, array<System::Byte>^ data,
                                                       u32 valid,
                                                       u32 canopenErrorCode); 

public delegate void CliWriteResultDelegate( System::Object^ obj, 
                                                        CanOpenStatus status,
                                                        u8 node_id,
                                                        u16 object_index, 
                                                        u8 sub_index, 
                                                        u32 canopenErrorCode); 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


public ref class ServerSDO_NET
{

private:

    ServerSDO *cpp_ServerSDO;
    [UnmanagedFunctionPointer(CallingConvention::Cdecl)]
    delegate canOpenStatus ServerWriteDelegate(void *context, u16 object_index, u8 sub_index, u8 *buf, u32 valid, u32 *coErrorCode);
    ServerWriteDelegate ^srvWriteDelegate;
    [UnmanagedFunctionPointer(CallingConvention::Cdecl)]
    delegate canOpenStatus ServerReadDelegate(void *context, u16 object_index, u8 sub_index, u8 *buf, u32 *valid, u32 buffer_size, u32 *coerror_code);
    ServerReadDelegate ^srvReadDelegate;
    [UnmanagedFunctionPointer(CallingConvention::Cdecl)]
    delegate canOpenStatus ServerGetAttrDelegate(void *p, u16 object_index, u8 sub_index, u16 *flags);
    ServerGetAttrDelegate ^srvGetAttrDelegate;

    SrvReadDelegate^ readObjectDelegate;
    System::Object^ readObjectDelgateObject;

    SrvWriteDelegate^ writeObjectDelegate;
    System::Object^ writeObjectDelegateObject;

    SrvGetAttrDelegate^ attribObjectDelegate;
    System::Object^ attribObjectDelegateObject;

public:
    ServerSDO_NET();
    ~ServerSDO_NET();
    CanOpenStatus  canHardwareConnect(int port, int bitrate);
    CanOpenStatus  canHardwareDisconnect(void);

    CanOpenStatus  nodeSetId(u8 node_id);

    CanOpenStatus registerObjectReadCallback(SrvReadDelegate^ readDelegate, System::Object^ obj); 
    CanOpenStatus registerObjectWriteCallback(SrvWriteDelegate^ writeDelegate, System::Object^ obj); 
    CanOpenStatus registerObjectGetAttributesCallback(SrvGetAttrDelegate^ getAttrDelegate, System::Object^ obj); 

private:
    canOpenStatus serverRead(void *p, u16 object_index, u8 sub_index, u8 *buf, u32 *valid, u32 buffer_size, u32 *coerror_code);
    canOpenStatus serverWrite(void *p, u16 object_index, u8 sub_index, u8 *buf, u32 valid, u32 *coerror_code);
    canOpenStatus serverGetAttribut(void *p, u16 object_index, u8 sub_index, u16 *flags);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


public delegate CanOpenStatus CanReceiveDelegate( System::Object^ obj, 
                                                        u32 id, 
                                                        array<System::Byte>^ data,
                                                        u8 dlc,
                                                        u32 flags); //Write to srv's application.

public delegate CanOpenStatus ReceivePdoDelegate( System::Object^ obj, 
                                                        u32 id, 
                                                        array<System::Byte>^ data,
                                                        u8 len); 

public delegate CanOpenStatus EmcyServerDelegate( System::Object^ obj, 
                                                        u8 nodeId,
                                                        u16 emcyErrorCode,
                                                        u8 errorRegister,
                                                        array<System::Byte>^ manufacturerSpecificErrorField); 

public delegate CanOpenStatus SyncServerDelegate( System::Object^ obj);

public ref class CanInterface_NET
{
public:
    CanInterface_NET();
    ~CanInterface_NET();
    //CanOpenStatus unlockCanopenLibrary( array<Byte>^ license_file, array<Byte>^ unlock_code );
};


public ref class CanMonitor_NET
{

private:
    CanMonitor *cpp_CanMonitor;
    [UnmanagedFunctionPointer(CallingConvention::Cdecl)]
    delegate canOpenStatus CanReceiveDelegate_CPP(void *context, u32 id, u8 *data, u8 dlc, u32 flags);
    CanReceiveDelegate_CPP ^can_receive_delegate_CPP;

    CanReceiveDelegate^ can_receive_delegate;  //C#
    System::Object^ can_receive_delegate_object; //C#

public:
    CanMonitor_NET();
    ~CanMonitor_NET();

    CanOpenStatus    canHardwareConnect(int port, int bitrate);
    CanOpenStatus    canHardwareDisconnect(void);

    CanOpenStatus    registerCanReceiveCallback( System::Object^ obj, CanReceiveDelegate^ can_receive_delegate );
    CanOpenStatus    canWrite( u32 id,  array<Byte>^ data, u8 dlc, u32 flags);

private:
    canOpenStatus canReceiveCPP(void * context, u32 id, u8 *data, u8 dlc, u32 flags );

};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public ref class ReceivePDO_NET
{
private:
    ReceivePDO *cpp_ReceivePDO;
    [UnmanagedFunctionPointer(CallingConvention::Cdecl)]
    delegate canOpenStatus ReceivePdoDelegate_CPP(void *context, u32 cobid, u8* data, u8 len);
    ReceivePdoDelegate_CPP ^receive_pdo_delegate_CPP;

    ReceivePdoDelegate^ receive_pdo_delegate;  //C#
    System::Object^ receive_pdo_delegate_object; //C#

public:
    ReceivePDO_NET();
    ~ReceivePDO_NET();

    CanOpenStatus    canHardwareConnect(int port, int bitrate);
    CanOpenStatus    canHardwareDisconnect(void);

    CanOpenStatus    registerReceivePdoMessageCallBack( System::Object^ obj, ReceivePdoDelegate^ can_receive_delegate );
    CanOpenStatus    setCobid(u32 cobid);

private:
    canOpenStatus    receivePdoCPP(void * context, u32 id, u8 *data, u8 dlc);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public ref class SyncServer_NET
{
private:
    SyncServer *cpp_SyncServer;
    [UnmanagedFunctionPointer(CallingConvention::Cdecl)]
    delegate canOpenStatus SyncServerDelegate_CPP(void *context);
    SyncServerDelegate_CPP ^sync_server_delegate_CPP;

    SyncServerDelegate^ sync_server_delegate;  //C#
    System::Object^ sync_server_delegate_object; //C#

public:
    SyncServer_NET();
    ~SyncServer_NET();

    CanOpenStatus    canHardwareConnect(int port, int bitrate);
    CanOpenStatus    canHardwareDisconnect(void);

    CanOpenStatus    registerSyncMessageCallBack( System::Object^ obj, SyncServerDelegate^ can_receive_delegate );
    CanOpenStatus    setCobid(u32 cobid);

private:
    canOpenStatus    syncServerCallbackCPP(void * context);
};



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public ref class EmcyServer_NET
{
private:
    EmcyServer *cpp_EmcyServer;
    [UnmanagedFunctionPointer(CallingConvention::Cdecl)]
    delegate canOpenStatus EmcyServerDelegate_CPP(void *context, u8 nodeId, u16 emcyErrorCode, u8 errorRegister, u8 *manufacturerSpecificErrorField);
    EmcyServerDelegate_CPP ^emcy_server_delegate_CPP;

    EmcyServerDelegate^ emcy_server_delegate;  //C#
    System::Object^ emcy_server_delegate_object; //C#

public:
    EmcyServer_NET();
    ~EmcyServer_NET();

    CanOpenStatus    canHardwareConnect(int port, int bitrate);
    CanOpenStatus    canHardwareDisconnect(void);

    CanOpenStatus    registerEmcyServerMessageCallBack( System::Object^ obj, EmcyServerDelegate^ can_receive_delegate );

private:
    canOpenStatus emcyServerCallbackCPP(void *context, u8 nodeId, u16 emcyErrorCode, u8 errorRegister, u8 *manufacturerSpecificErrorField);
};



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public ref class TransmitPDO_NET
{
private:
    TransmitPDO *cpp_TransmitPDO;

public:
    TransmitPDO_NET();
    ~TransmitPDO_NET();

    CanOpenStatus    canHardwareConnect(int port, int bitrate);
    CanOpenStatus    canHardwareDisconnect(void);

    CanOpenStatus    setup(u32 id, array<Byte>^ data, u8 dlc);
    CanOpenStatus    transmit(void);
    CanOpenStatus    periodicTransmission(bool value);
};


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public ref class SyncProducer_NET
{
private:
    SyncProducer *cpp_SyncProducer;

public:
    SyncProducer_NET();
    ~SyncProducer_NET();

    CanOpenStatus    canHardwareConnect(int port, int bitrate);
    CanOpenStatus    canHardwareDisconnect(void);

	CanOpenStatus    setSyncCOBID(COBID id);
	CanOpenStatus    setTransmissionPeriodTime(unsigned long sync_period_time);
	CanOpenStatus    startPeriodicTransmission(bool enabled);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public ref class EmcyClient_NET
{
private:
    EmcyClient *cpp_EmcyClient;

public:
    EmcyClient_NET();
    ~EmcyClient_NET();

    CanOpenStatus    canHardwareConnect(int port, int bitrate);
    CanOpenStatus    canHardwareDisconnect(void);

    CanOpenStatus    nodeSetId( u8 node_id );
    CanOpenStatus    sendEmcyMessage( u8 nodeId, u16 emcy_error_code, u8 error_register, array<Byte>^ manufSpecificErrorField );
    CanOpenStatus    sendEmcyMessage( u16 emcy_error_code, u8 error_register, array<Byte>^ manufSpecificErrorField );
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


public ref class ClientSDO_NET
{

public:
    ClientSDO_NET();
    ~ClientSDO_NET();


    CanOpenStatus  connect(u8 node_id);

    CanOpenStatus  connect(COBID cobid_tx, COBID cobid_rx);

    CanOpenStatus  canHardwareConnect(int port, int bitrate);
    CanOpenStatus  canHardwareDisconnect(void);

    CanOpenStatus registerObjectReadResultCallback(CliReadResultDelegate^ readDelegate, System::Object^ obj); 
    CanOpenStatus registerObjectWriteResultCallback(CliWriteResultDelegate^ writeDelegate, System::Object^ obj); 
    CanOpenStatus unregisterObjectReadWriteResultCallbacks(void);

    void          setWriteObjectTimeout(u32 timeout);
    void          setReadObjectTimeout(u32 timeout);
    void          setNodeResponseTimeout(u32 timeout);
  
    bool isObjectWriteResultCallbackEnabled(void);
    bool isObjectReadResultCallbackEnabled(void);

    CanOpenStatus  objectRead(u16 object_index, 
                                u8 sub_index, 
                                array<Byte>^ val,
                                [System::Runtime::InteropServices::Out] u32 %valid,
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode);

    CanOpenStatus  objectRead(u16 object_index, u8 sub_index, 
                                [System::Runtime::InteropServices::Out] u8 %val, 
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode);

    CanOpenStatus  objectRead(u16 object_index, u8 sub_index, 
                                [System::Runtime::InteropServices::Out] u16 %val, 
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode);

    CanOpenStatus  objectRead(u16 object_index, u8 sub_index, 
                                [System::Runtime::InteropServices::Out] u32 %val, 
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode);
#ifdef HAS_LONG_LONG
    CanOpenStatus  objectRead(u16 object_index, u8 sub_index, 
                                [System::Runtime::InteropServices::Out] u64 %val, 
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode);
#endif

    CanOpenStatus  objectWrite(u16 object_index, 
                                u8 sub_index, 
                                array<Byte>^ buf, //u8 *buf, 
                                u32 valid, 
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode);

    CanOpenStatus  objectWriteBlock(u16 object_index, 
                                u8 sub_index, 
                                u16 crc, 
                                array<Byte>^ buf, 
                                u32 valid, 
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode);

    CanOpenStatus  objectWrite(u16 object_index, 
                                u8 sub_index, 
                                u8  val, 
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode);

    CanOpenStatus  objectWrite(u16 object_index, 
                                u8 sub_index, 
                                u16 val, 
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode);

    CanOpenStatus  objectWrite(u16 object_index, 
                                u8 sub_index, 
                                u32 val, 
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode);
#ifdef HAS_LONG_LONG
    CanOpenStatus  objectWrite(u16 object_index, 
                                u8 sub_index, 
                                u64 val, 
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode);
#endif
private:
    HANDLE rx_tx_mutex;
    ClientSDO *cpp_ClientSDO;
    u8 *async_data_buffer;
    array<Byte>^ applications_buffer; 
        
  //typedef void (*CliWriteFuncPtr)(void *context, canOpenStatus status, u16 object_index, 
  //  u8 sub_index, u32 co_error_code); 

    [UnmanagedFunctionPointer(CallingConvention::Cdecl)]
    delegate void ClientWriteResultDelegate(void *context, canOpenStatus status, u8 node_id, u16 object_index, u8 sub_index, u32 co_error_code);
    ClientWriteResultDelegate ^cliWriteResultDelegate;
    
    CliWriteResultDelegate^ writeObjectResultDelegate;
    System::Object^ writeObjectResultDelegateObject;

    //typedef void (*CliReadFuncPtr)(void *context, canOpenStatus status, u16 object_index, 
    //  u8 sub_index, u8 *buffer, u32 valid, u32 co_error_code);

    [UnmanagedFunctionPointer(CallingConvention::Cdecl)]
    delegate void ClientReadResultDelegate(void *context, canOpenStatus status, u8 node_id, u16 object_index, u8 sub_index, u8 *buffer, u32 valid, u32 co_error_code);
    ClientReadResultDelegate ^cliReadResultDelegate;
  
    CliReadResultDelegate^ readObjectResultDelegate;
    System::Object^ readObjectResultDelgateObject;

    void clientReadResultWrapperCallback(void *context, canOpenStatus status, u8 node_id, u16 object_index, u8 sub_index, u8 *buffer, u32 valid, u32 co_error_code);
    void clientWriteResultWrapperCallback(void *context, canOpenStatus status, u8 node_id, u16 object_index, u8 sub_index, u32 co_error_code);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public delegate CanOpenStatus NMTOperationalStateDelegate(System::Object^ obj, u8 node_id, u8 state);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public ref class NMT_Master_NET
{
public:

    NMT_Master_NET();
    ~NMT_Master_NET();

    CanOpenStatus  canHardwareConnect(int port, int btr);
    CanOpenStatus  canHardwareDisconnect(void);

    CanOpenStatus  nodeStart(u8 node_id);
    CanOpenStatus  nodeStop(u8 node_id);
    CanOpenStatus  nodePreoperational(u8 node_id);
    CanOpenStatus  nodeReset(u8 node_id);
    CanOpenStatus  nodeResetCommunication(u8 node_id);

    CanOpenStatus  nodeGuardPollStart(u8 node_id, u32 node_life_time_ms);
    CanOpenStatus  nodeGuardPollStop(u8 node_id);

    CanOpenStatus  heartbeatMonitorStart(u8 node_id, u32 heartbeat_consumer_time_ms);
    CanOpenStatus  heartbeatMonitorStop(u8 node_id);

    CanOpenStatus  registerNodeStateCallback(NMTOperationalStateDelegate^ opStateDelegate, System::Object^ obj);

    CanOpenStatus  nodeReadOperationalState(
          u8 node_id, 
          u32 maxMsTimeout, 
      [System::Runtime::InteropServices::Out] u8 %node_state);

    CanOpenStatus NMTOperationalStateWrapperCPP(void *obj, u8 node_id, u8 state);

private:
    NMTMaster *cpp_NMTMaster;
    NMTOperationalStateDelegate^ nmtOperationalStateDelegate_CS;
    System::Object^ nmtOperationalStateDelegateObject;

    [UnmanagedFunctionPointer(CallingConvention::Cdecl)]
    delegate CanOpenStatus NMTOperationalStateDelegate_CPP(void *obj, u8 node_id, u8 state);

    NMTOperationalStateDelegate_CPP ^nmtOperationalStateDelegateCPP;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public delegate CanOpenStatus NMTLocalNodeOperationalStateDelegate(System::Object^ obj, u8 state);

public ref class NMT_Slave_NET
{
public:

    NMT_Slave_NET();
    ~NMT_Slave_NET();

    CanOpenStatus  canHardwareConnect(int port, int btr);
    CanOpenStatus  canHardwareDisconnect(void);

    CanOpenStatus  nodeSetId(u8 node_id);
    CanOpenStatus  nodeSetState(u8 node_state);
    CanOpenStatus  writeMessage(u32 id, array<Byte>^, u8 dlc, u32 flags);
    CanOpenStatus  registerLocalNodeStateChangeCallback(NMTLocalNodeOperationalStateDelegate^ localOperationalStateDelegate, System::Object^ obj);

private:
    NMTSlave *cpp_NMTSlave;

    NMTLocalNodeOperationalStateDelegate^ nmtLocalNodeOperationalStateDelegate_CS;
    System::Object^ nmtLocalNodeOperationalStateDelegateObject;

    [UnmanagedFunctionPointer(CallingConvention::Cdecl)]
    delegate CanOpenStatus NMTLocalNodeOperationalStateDelegate_CPP(void *obj, u8 state);
    NMTLocalNodeOperationalStateDelegate_CPP ^nmtLocalNodeOperationalStateDelegatCPP ;

    CanOpenStatus NMTLocalNodeOperationalStateWrapperCPP(void *obj, u8 state);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public ref class LSS_Master_NET
{
public:

    LSS_Master_NET();
    ~LSS_Master_NET();

    CanOpenStatus  canHardwareConnect(int port, int btr);
    CanOpenStatus  canHardwareDisconnect(void);
	CanOpenStatus  switchModeGlobal(u8 mode);
	CanOpenStatus  switchModeSelectiveVendorId(u32 vendorId);
	CanOpenStatus  switchModeSelectiveProductCode(u32 productCode);
	CanOpenStatus  switchModeSelectiveRevisionNumber(u32 revisionNumber);
	CanOpenStatus  switchModeSelectiveSerialNumber(u32 serialNumber);
	CanOpenStatus  configureNodeId(u8 nodeId, [System::Runtime::InteropServices::Out] u8 %errorCode, [System::Runtime::InteropServices::Out] u8 %specificErrorCode);
	CanOpenStatus  configureBitTimingParamteres(u8 tableSelector, u8 tableIndex, [System::Runtime::InteropServices::Out] u8 %errorCode, [System::Runtime::InteropServices::Out] u8 %specificErrorCode);
	CanOpenStatus  activateBitTimingParameters(u16 switchDelay);
	CanOpenStatus  storeConfiguration([System::Runtime::InteropServices::Out] u8 %errorCode, [System::Runtime::InteropServices::Out] u8 %specificErrorCode);

private:
    LSSMaster *cpp_LSSMaster;

};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public ref class LSS_Slave_NET
{
public:

    LSS_Slave_NET();
    ~LSS_Slave_NET();

    CanOpenStatus  canHardwareConnect(int port, int btr);
    CanOpenStatus  canHardwareDisconnect(void);
	void  setDeviceParameters(u32 vendorId, u32 productCode, u32 revisionNumber, u32 serialNumber);

private:
    LSSSlave *cpp_LSSSlave;

};


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


public ref class CANopenLibraryConfiguration
{

public:
  enum class CanAdapter {  // qqq. CANT MAKE THIS CLASS VISIBLE IN MANAGED ENVIRONMENT!
    canAdapter_DEFAULT = 0,
    canAdapter_EMS = 1,
    canAdapter_IXXAT = 2,
    canAdapter_KVASER = 3,
    canAdapter_CAN232 = 4,
    canAdapter_CANUSB = 5,
    canAdapter_CASTOR = 6,
    canAdapter_PCAN = 7,
    canAdapter_TINYCAN = 8,
    canAdapter_ZANTHIC = 9
} ;

  static void SetAdapter(CanAdapter userAdapter);
};


#endif