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
    *             Copyright (C) 2009-2013 Ulrik Hagstr�m.             *
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

public ref class CANOPEN_LIB_ERROR
{
public:
    enum class CanOpenStatus {     
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
};


public ref class SDO_NET : CANOPEN_LIB_ERROR
{
public:
/*
  enum class CanOpenStatus {     
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
        CANOPEN_ERROR                          = -255 
    };
*/
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


public delegate SDO_NET::CanOpenStatus SrvReadDelegate( System::Object^ obj, u16 object_index, u8 sub_index, array<System::Byte>^ data,
                                                       [OutAttribute] u32 %valid,
                                                       [OutAttribute] u32 %canopenErrorCode); //Read from srv's application.
public delegate SDO_NET::CanOpenStatus SrvWriteDelegate( System::Object^ obj, 
                                                        u16 object_index, 
                                                        u8 sub_index, 
                                                        array<System::Byte>^ data,
                                                        u32 valid,
                                                        [OutAttribute] u32 %canopenErrorCode); //Write to srv's application.
public delegate SDO_NET::CanOpenStatus SrvGetAttrDelegate( System::Object^ obj, 
                                                          u16 object_index, 
                                                          u8 sub_index, 
                                                          [OutAttribute] u16 %flags );


public delegate void CliReadResultDelegate( System::Object^ obj, SDO_NET::CanOpenStatus status, u8 node_id, u16 object_index, u8 sub_index, array<System::Byte>^ data,
                                                       u32 valid,
                                                       u32 canopenErrorCode); 

public delegate void CliWriteResultDelegate( System::Object^ obj, 
                                                        SDO_NET::CanOpenStatus status,
                                                        u8 node_id,
                                                        u16 object_index, 
                                                        u8 sub_index, 
                                                        u32 canopenErrorCode); 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


public ref class ServerSDO_NET : SDO_NET
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


public delegate CANOPEN_LIB_ERROR::CanOpenStatus CanReceiveDelegate( System::Object^ obj, 
                                                        u32 id, 
                                                        array<System::Byte>^ data,
                                                        u8 dlc,
                                                        u32 flags); //Write to srv's application.

public delegate CANOPEN_LIB_ERROR::CanOpenStatus ReceivePdoDelegate( System::Object^ obj, 
                                                        u32 id, 
                                                        array<System::Byte>^ data,
                                                        u8 len); 

public delegate CANOPEN_LIB_ERROR::CanOpenStatus EmcyServerDelegate( System::Object^ obj, 
                                                        u8 nodeId,
                                                        u16 emcyErrorCode,
                                                        u8 errorRegister,
                                                        array<System::Byte>^ manufacturerSpecificErrorField); 

public delegate CANOPEN_LIB_ERROR::CanOpenStatus SyncServerDelegate( System::Object^ obj);

public ref class CanInterface_NET : CANOPEN_LIB_ERROR
{
public:
    CanInterface_NET();
  ~CanInterface_NET();
  //CanOpenStatus unlockCanopenLibrary( array<Byte>^ license_file, array<Byte>^ unlock_code );
};


public ref class CanMonitor_NET : CANOPEN_LIB_ERROR
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

public ref class ReceivePDO_NET : CANOPEN_LIB_ERROR
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

public ref class SyncServer_NET : CANOPEN_LIB_ERROR
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

public ref class EmcyServer_NET : CANOPEN_LIB_ERROR
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

public ref class TransmitPDO_NET : CANOPEN_LIB_ERROR
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

public ref class EmcyClient_NET : CANOPEN_LIB_ERROR
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


public ref class ClientSDO_NET : SDO_NET
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
    ClientSDO *cpp_ClientSDO;
  u8 *temp_data_buffer;
  array<Byte>^ applicationsBuffer; 
        
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

public delegate CANOPEN_LIB_ERROR::CanOpenStatus NMTOperationalStateDelegate(System::Object^ obj, u8 node_id, u8 state);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public ref class NMT_Master_NET : CANOPEN_LIB_ERROR
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

public delegate CANOPEN_LIB_ERROR::CanOpenStatus NMTLocalNodeOperationalStateDelegate(System::Object^ obj, u8 state);

public ref class NMT_Slave_NET : CANOPEN_LIB_ERROR
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

#endif