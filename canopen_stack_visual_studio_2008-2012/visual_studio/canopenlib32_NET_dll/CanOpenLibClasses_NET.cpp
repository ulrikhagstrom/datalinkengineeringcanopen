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

#include "stdafx.h"
#include "CanOpenLibClasses_NET.h"

CanInterface_NET::CanInterface_NET()
{ 
  ;
}

CanInterface_NET::~CanInterface_NET()
{ 
  ;
}

ServerSDO_NET::ServerSDO_NET()
{
  typedef canOpenStatus (*SrvReadFuncPtr)(void *context, u16 object_index, u8 sub_index, u8 *buf, u32 *valid, u32 buffer_size, u32 *coerror_code); //Read from srv's application.
  typedef canOpenStatus (*SrvWriteFuncPtr)(void *context, u16 object_index, u8 sub_index, u8 *buf, u32 valid, u32 *coerror_code); //Write to srv's application.
  typedef canOpenStatus (*SrvGetAttrFuncPtr)(void *context, u16 object_index, u8 sub_index, u16 *flags);

  this->cpp_ServerSDO = new ServerSDO();

  srvWriteDelegate =  gcnew ServerWriteDelegate( this, &ServerSDO_NET::serverWrite );
  IntPtr pSrvWriteDelegate = Marshal::GetFunctionPointerForDelegate(srvWriteDelegate);
  void *pf = pSrvWriteDelegate.ToPointer();
  this->cpp_ServerSDO->registerObjectWriteCallback((SrvWriteFuncPtr)pf, 0);

  srvReadDelegate =  gcnew ServerReadDelegate( this, &ServerSDO_NET::serverRead );
  IntPtr pSrvReadDelegate = Marshal::GetFunctionPointerForDelegate(srvReadDelegate);
  pf = pSrvReadDelegate.ToPointer();
  this->cpp_ServerSDO->registerObjectReadCallback((SrvReadFuncPtr)pf, 0);

  srvGetAttrDelegate =  gcnew ServerGetAttrDelegate( this, &ServerSDO_NET::serverGetAttribut );
  IntPtr pSrvGetAttrDelegate = Marshal::GetFunctionPointerForDelegate(srvGetAttrDelegate);
  pf = pSrvGetAttrDelegate.ToPointer();
  this->cpp_ServerSDO->registerObjectGetAttributesCallback((SrvGetAttrFuncPtr)pf, 0);
}

ServerSDO_NET::~ServerSDO_NET()
{
  delete this->cpp_ServerSDO;
  this->cpp_ServerSDO = nullptr;
}

CanOpenStatus ServerSDO_NET::registerObjectReadCallback(SrvReadDelegate^ readDelegate, System::Object^ obj)
{
  this->readObjectDelegate = readDelegate;
  this->readObjectDelgateObject = obj;
  return CanOpenStatus::CANOPEN_OK;
}

CanOpenStatus ServerSDO_NET::registerObjectWriteCallback(SrvWriteDelegate^ writeDelegate, System::Object^ obj)
{
  this->writeObjectDelegate = writeDelegate;
  this->writeObjectDelegateObject = obj;
  return CanOpenStatus::CANOPEN_OK;
}
  
CanOpenStatus ServerSDO_NET::registerObjectGetAttributesCallback(SrvGetAttrDelegate^ getAttrDelegate, System::Object^ obj)
{
  this->attribObjectDelegate = getAttrDelegate;
  this->attribObjectDelegateObject = obj;
  return CanOpenStatus::CANOPEN_OK;
}

CanOpenStatus ServerSDO_NET::canHardwareConnect(int port, int bitrate)
{
  return (CanOpenStatus)this->cpp_ServerSDO->canHardwareConnect(port, bitrate); 
}

CanOpenStatus  ServerSDO_NET::canHardwareDisconnect(void)
{
  return (CanOpenStatus)this->cpp_ServerSDO->canHardwareDisconnect(); 
}


CanOpenStatus ServerSDO_NET::nodeSetId(u8 node_id)
{
  return (CanOpenStatus)this->cpp_ServerSDO->nodeSetId(node_id);
}


canOpenStatus ServerSDO_NET::serverRead(void *p, u16 object_index, u8 sub_index, u8 *buf, u32 *valid, u32 buffer_size, u32 *coerror_code)
{
  canOpenStatus ret;
  array<System::Byte>^ data;
  data = gcnew array<System::Byte>(buffer_size);

  ret = (canOpenStatus)this->readObjectDelegate( this->readObjectDelgateObject, object_index, sub_index, data, *valid, *coerror_code);
  if (ret == CANOPEN_OK)
  {
    for (u32 i = 0; i < *valid; i++)
      buf[i] = data[i];
  }
  return ret;
}

canOpenStatus ServerSDO_NET::serverWrite(void *p, u16 object_index, u8 sub_index, u8 *buf, u32 valid, u32 *coerror_code)
{
  canOpenStatus ret;
  array<System::Byte>^ data;
  data = gcnew array<System::Byte>(valid);

  for (u16 i = 0; i < valid; i++)
    data[i] = buf[i];

  ret = (canOpenStatus)this->writeObjectDelegate( this->writeObjectDelegateObject, object_index, sub_index, data, valid, *coerror_code );
  return ret;

}

canOpenStatus ServerSDO_NET::serverGetAttribut(void *p, u16 object_index, u8 sub_index, u16 *flags)
{
  return (canOpenStatus)this->attribObjectDelegate( this->attribObjectDelegateObject, object_index, sub_index, *flags );
}

/***************
**
**  CanMonitor 
**
****************/


CanMonitor_NET::CanMonitor_NET()
{
  typedef void ( * RawCanReceiveFunPtr )( void * context, u32 id, u8 *data, u8 dlc, u32 flags );

  this->cpp_CanMonitor = new CanMonitor();
  can_receive_delegate_CPP =  gcnew CanReceiveDelegate_CPP( this, &CanMonitor_NET::canReceiveCPP );
  IntPtr p_can_receive_delegate_CPP = Marshal::GetFunctionPointerForDelegate(can_receive_delegate_CPP);
  void *p = p_can_receive_delegate_CPP.ToPointer();
  this->cpp_CanMonitor->registerCanReceiveCallback( 0, (RawCanReceiveFunPtr)p );

}

CanMonitor_NET::~CanMonitor_NET()
{
  this->can_receive_delegate = nullptr;
  this->can_receive_delegate_object = nullptr;
  delete this->cpp_CanMonitor;
  this->cpp_CanMonitor = nullptr;
}


CanOpenStatus CanMonitor_NET::canHardwareConnect(int port, int bitrate)
{
  return (CanOpenStatus)this->cpp_CanMonitor->canHardwareConnect(port, bitrate); 
}


CanOpenStatus CanMonitor_NET::canHardwareDisconnect(void)
{
  return (CanOpenStatus)this->cpp_CanMonitor->canHardwareDisconnect();
}

CanOpenStatus CanMonitor_NET::registerCanReceiveCallback( System::Object^ obj, CanReceiveDelegate^ can_receive_delegate )
{
  this->can_receive_delegate = can_receive_delegate;
  this->can_receive_delegate_object = obj;
  return CanOpenStatus::CANOPEN_OK;
}

CanOpenStatus CanMonitor_NET::canWrite( u32 id,  array<Byte>^ data, u8 dlc, u32 flags)
{
  u8 data_cpp[8];

  for (int j = 0; j < dlc; j++)
    data_cpp[j] = data[j]; 

  CanOpenStatus ret = (CanOpenStatus)this->cpp_CanMonitor->canWrite( id, data_cpp, dlc, flags );

  return ret;
}

canOpenStatus CanMonitor_NET::canReceiveCPP(void *context, u32 id, u8 *data, u8 dlc, u32 flags )
{
  canOpenStatus ret;
  array<System::Byte>^ data_cs;
  data_cs = gcnew array<System::Byte>(dlc);

  for (u8 i = 0; i < dlc; i++)
    data_cs[i] = data[i];

  if (can_receive_delegate != nullptr)
  {
    ret = (canOpenStatus)this->can_receive_delegate(this->can_receive_delegate_object, 
      id, data_cs, dlc, flags);
  }
  return ret;
}

/***************
**
**  Emergency Client
**
****************/

EmcyClient_NET::EmcyClient_NET()
{
  this->cpp_EmcyClient = new EmcyClient();
}

EmcyClient_NET::~EmcyClient_NET()
{
  delete this->cpp_EmcyClient;
  this->cpp_EmcyClient = nullptr;
}


CanOpenStatus EmcyClient_NET::canHardwareConnect(int port, int bitrate)
{
  return (CanOpenStatus)this->cpp_EmcyClient->canHardwareConnect(port, bitrate); 
}


CanOpenStatus EmcyClient_NET::canHardwareDisconnect(void)
{
  return (CanOpenStatus)this->cpp_EmcyClient->canHardwareDisconnect();
}
  
CanOpenStatus EmcyClient_NET::nodeSetId(u8 nodeId)
{
  return (CanOpenStatus)cpp_EmcyClient->nodeSetId(nodeId);
}


CanOpenStatus EmcyClient_NET::sendEmcyMessage( u8 nodeId, u16 emcy_error_code, u8 error_register, array<Byte>^ manufSpecificErrorField )
{
  u8 *data_cpp = new u8[5];

  for (int j = 0; j < 5; j++)
    data_cpp[j] = manufSpecificErrorField[j]; 

  CanOpenStatus ret = (CanOpenStatus)this->cpp_EmcyClient->sendEmcyMessage(nodeId, emcy_error_code, error_register, data_cpp);

  delete[] data_cpp;
  return ret;
}


CanOpenStatus EmcyClient_NET::sendEmcyMessage( u16 emcy_error_code, u8 error_register, array<Byte>^ manufSpecificErrorField )
{
  u8 *data_cpp = new u8[5];

  for (int j = 0; j < 5; j++)
    data_cpp[j] = manufSpecificErrorField[j]; 

  CanOpenStatus ret = (CanOpenStatus)this->cpp_EmcyClient->sendEmcyMessage(emcy_error_code, error_register, data_cpp);

  delete[] data_cpp;
  return ret;
}


/***************
**
**  SyncProducer
**
****************/


SyncProducer_NET::SyncProducer_NET()
{
  this->cpp_SyncProducer = new SyncProducer();
}

SyncProducer_NET::~SyncProducer_NET()
{
  delete this->cpp_SyncProducer;
  this->cpp_SyncProducer = nullptr;
}


CanOpenStatus SyncProducer_NET::canHardwareConnect(int port, int bitrate)
{
  return (CanOpenStatus)this->cpp_SyncProducer->canHardwareConnect(port, bitrate); 
}


CanOpenStatus SyncProducer_NET::canHardwareDisconnect(void)
{
  return (CanOpenStatus)this->cpp_SyncProducer->canHardwareDisconnect();
}

CanOpenStatus SyncProducer_NET::setSyncCOBID(u32 id)
{
  if (cpp_SyncProducer == nullptr)
    return (CanOpenStatus::CANOPEN_INTERNAL_STATE_ERROR);

  CanOpenStatus ret = (CanOpenStatus)this->cpp_SyncProducer->setSyncCOBID(id);

  return ret;

}
  
CanOpenStatus SyncProducer_NET::setTransmissionPeriodTime(unsigned long sync_period_time)
{
  if (cpp_SyncProducer == nullptr)
    return (CanOpenStatus::CANOPEN_INTERNAL_STATE_ERROR);

  CanOpenStatus ret = (CanOpenStatus)this->cpp_SyncProducer->setTransmissionPeriodTime(sync_period_time);

  return ret;

}

CanOpenStatus SyncProducer_NET::startPeriodicTransmission(bool value)
{
  return (CanOpenStatus)cpp_SyncProducer->startPeriodicTransmission(value);
}


/***************
**
**  TransmitPDO
**
****************/


TransmitPDO_NET::TransmitPDO_NET()
{
  this->cpp_TransmitPDO = new TransmitPDO();
}

TransmitPDO_NET::~TransmitPDO_NET()
{
  delete this->cpp_TransmitPDO;
  this->cpp_TransmitPDO = nullptr;
}


CanOpenStatus TransmitPDO_NET::canHardwareConnect(int port, int bitrate)
{
  return (CanOpenStatus)this->cpp_TransmitPDO->canHardwareConnect(port, bitrate); 
}


CanOpenStatus TransmitPDO_NET::canHardwareDisconnect(void)
{
  return (CanOpenStatus)this->cpp_TransmitPDO->canHardwareDisconnect();
}

CanOpenStatus TransmitPDO_NET::setup(u32 id,  array<Byte>^ data, u8 dlc)
{
  if (cpp_TransmitPDO == nullptr)
    return (CanOpenStatus::CANOPEN_INTERNAL_STATE_ERROR);

  u8 *data_cpp = new u8[8];

  if (dlc < 0 || dlc > 8)
    return CanOpenStatus::CANOPEN_ARG_ERROR;

  for (int j = 0; j < dlc; j++)
    data_cpp[j] = data[j]; 

  CanOpenStatus ret = (CanOpenStatus)this->cpp_TransmitPDO->setData( id, data_cpp, dlc);

  delete[] data_cpp;
  return ret;

}
  
CanOpenStatus TransmitPDO_NET::transmit(void)
{
  if (cpp_TransmitPDO != nullptr)
    return (CanOpenStatus)cpp_TransmitPDO->transmitPdo();
  else
    return (CanOpenStatus::CANOPEN_INTERNAL_STATE_ERROR);
}


CanOpenStatus TransmitPDO_NET::periodicTransmission(bool value)
{
  return (CanOpenStatus)cpp_TransmitPDO->periodicTransmission(value);
}


/***************
**
**  Receive PDO
**
****************/


ReceivePDO_NET::ReceivePDO_NET()
{
  typedef void ( * RawPdoReceiveFunPtr )( void * context, u32 id, u8 *data, u8 dlc);

  this->cpp_ReceivePDO = new ReceivePDO();
  receive_pdo_delegate_CPP =  gcnew ReceivePdoDelegate_CPP( this, &ReceivePDO_NET::receivePdoCPP );
  IntPtr p_pdo_receive_delegate_CPP = Marshal::GetFunctionPointerForDelegate(receive_pdo_delegate_CPP);
  void *p = p_pdo_receive_delegate_CPP.ToPointer();
  this->cpp_ReceivePDO->registerReceivePdoMessageCallBack( 0, (RawPdoReceiveFunPtr)p );

}

ReceivePDO_NET::~ReceivePDO_NET()
{
  delete this->cpp_ReceivePDO;
  this->cpp_ReceivePDO = nullptr;
}


CanOpenStatus ReceivePDO_NET::canHardwareConnect(int port, int bitrate)
{
  return (CanOpenStatus)this->cpp_ReceivePDO->canHardwareConnect(port, bitrate); 
}


CanOpenStatus ReceivePDO_NET::canHardwareDisconnect(void)
{
  return (CanOpenStatus)this->cpp_ReceivePDO->canHardwareDisconnect();
}

CanOpenStatus ReceivePDO_NET::registerReceivePdoMessageCallBack( System::Object^ obj, ReceivePdoDelegate^ receive_pdo_delegate )
{
  this->receive_pdo_delegate = receive_pdo_delegate;
  this->receive_pdo_delegate_object = obj;
  return CanOpenStatus::CANOPEN_OK;
}

canOpenStatus ReceivePDO_NET::receivePdoCPP(void *context, u32 id, u8 *data, u8 dlc)
{
  canOpenStatus ret;
  array<System::Byte>^ data_cs;
  data_cs = gcnew array<System::Byte>(dlc);

  for (u8 i = 0; i < dlc; i++)
    data_cs[i] = data[i];

  ret = (canOpenStatus)this->receive_pdo_delegate(this->receive_pdo_delegate_object, 
    id, data_cs, dlc);
  return ret;
}

CanOpenStatus ReceivePDO_NET::setCobid(u32 cobid)
{
  return (CanOpenStatus)this->cpp_ReceivePDO->setCobid( cobid );
}


/***************
**
**  Sync server
**
****************/


SyncServer_NET::SyncServer_NET()
{
  typedef void ( * RawSyncMessageFunPtr )(void *context);

  this->cpp_SyncServer = new SyncServer();
  sync_server_delegate_CPP =  gcnew SyncServerDelegate_CPP( this, &SyncServer_NET::syncServerCallbackCPP );
  IntPtr p_pdo_receive_delegate_CPP = Marshal::GetFunctionPointerForDelegate(sync_server_delegate_CPP);
  void *p = p_pdo_receive_delegate_CPP.ToPointer();
  this->cpp_SyncServer->registerSyncMessageCallBack( 0, (RawSyncMessageFunPtr)p );

}

SyncServer_NET::~SyncServer_NET()
{
  delete this->cpp_SyncServer;
  this->cpp_SyncServer = nullptr;
}

CanOpenStatus SyncServer_NET::canHardwareConnect(int port, int bitrate)
{
  return (CanOpenStatus)this->cpp_SyncServer->canHardwareConnect(port, bitrate); 
}

CanOpenStatus SyncServer_NET::canHardwareDisconnect(void)
{
  return (CanOpenStatus)this->cpp_SyncServer->canHardwareDisconnect();
}

CanOpenStatus SyncServer_NET::registerSyncMessageCallBack( System::Object^ obj, SyncServerDelegate^ sync_server_delegate )
{
  this->sync_server_delegate = sync_server_delegate;
  this->sync_server_delegate_object = obj;
  return CanOpenStatus::CANOPEN_OK;
}

canOpenStatus SyncServer_NET::syncServerCallbackCPP(void *context)
{
  canOpenStatus ret;
  ret = (canOpenStatus)this->sync_server_delegate(this->sync_server_delegate_object);
  return ret;
}

CanOpenStatus SyncServer_NET::setCobid(u32 cobid)
{
  return (CanOpenStatus)this->cpp_SyncServer->reconfigureSyncCOBID( cobid );
}



/***************
**
**  Emergency Server
**
****************/


EmcyServer_NET::EmcyServer_NET()
{
  typedef void ( * EmcyServerFunPtr )( void *context, u8 nodeId, u16 emcyErrorCode, u8 errorRegister, u8 *manufacturerSpecificErrorField);

  this->cpp_EmcyServer = new EmcyServer();
  emcy_server_delegate_CPP =  gcnew EmcyServerDelegate_CPP( this, &EmcyServer_NET::emcyServerCallbackCPP );
  IntPtr p_emcy_server_delegate_CPP = Marshal::GetFunctionPointerForDelegate(emcy_server_delegate_CPP);
  void *p = p_emcy_server_delegate_CPP.ToPointer();
  this->cpp_EmcyServer->registerEmergencyMessageCallBack( 0, (EmcyServerFunPtr)p );

}

EmcyServer_NET::~EmcyServer_NET()
{
  delete this->cpp_EmcyServer;
  this->cpp_EmcyServer = nullptr;
}


CanOpenStatus EmcyServer_NET::canHardwareConnect(int port, int bitrate)
{
  return (CanOpenStatus)this->cpp_EmcyServer->canHardwareConnect(port, bitrate); 
}


CanOpenStatus EmcyServer_NET::canHardwareDisconnect(void)
{
  return (CanOpenStatus)this->cpp_EmcyServer->canHardwareDisconnect();
}

CanOpenStatus EmcyServer_NET::registerEmcyServerMessageCallBack( System::Object^ obj, EmcyServerDelegate^ emcy_server_delegate )
{
  this->emcy_server_delegate = emcy_server_delegate;
  this->emcy_server_delegate_object = obj;
  return CanOpenStatus::CANOPEN_OK;
}

canOpenStatus EmcyServer_NET::emcyServerCallbackCPP(void *context, u8 nodeId, u16 emcyErrorCode, u8 errorRegister, u8 *manufacturerSpecificErrorField)
{
  canOpenStatus ret;
  array<System::Byte>^ data_cs;
  data_cs = gcnew array<System::Byte>(5);

  for (u8 i = 0; i < 5; i++)
    data_cs[i] = manufacturerSpecificErrorField[i];

  ret = (canOpenStatus)this->emcy_server_delegate(this->emcy_server_delegate_object, 
    nodeId, emcyErrorCode, errorRegister, data_cs);
  return ret;
}

/***************
**
**  ClientSDO
**
****************/

ClientSDO_NET::ClientSDO_NET()
{
  this->rx_tx_mutex = CreateMutex( NULL, FALSE, NULL);
  this->readObjectResultDelegate = nullptr;
  this->readObjectResultDelgateObject = nullptr;
  this->writeObjectResultDelegate = nullptr;
  this->writeObjectResultDelegateObject = nullptr;

  this->cpp_ClientSDO = new ClientSDO();
}

ClientSDO_NET::~ClientSDO_NET()
{
  WaitForSingleObject( this->rx_tx_mutex, INFINITE);
  delete this->cpp_ClientSDO;
  this->cpp_ClientSDO = NULL;
  ReleaseMutex(this->rx_tx_mutex);
  CloseHandle(this->rx_tx_mutex);
}

CanOpenStatus ClientSDO_NET::registerObjectReadResultCallback(CliReadResultDelegate^ readResultDelegate, System::Object^ obj)
{
  typedef void (*CliReadResultFuncPtr)(void *context, canOpenStatus status, u8 nodeid, u16 object_index, u8 sub_index, u8 *buffer, u32 valid, u32 co_error_code);
  this->readObjectResultDelegate = readResultDelegate;
  this->readObjectResultDelgateObject = obj;

  cliReadResultDelegate =  gcnew ClientReadResultDelegate( this, &ClientSDO_NET::clientReadResultWrapperCallback );
  IntPtr pCliReadResultDelegate = Marshal::GetFunctionPointerForDelegate(cliReadResultDelegate);
  void *pf = pCliReadResultDelegate.ToPointer();
  this->cpp_ClientSDO->registerObjectReadResultCallback((CliReadResultFuncPtr)pf, 0); 

  return CanOpenStatus::CANOPEN_OK;
}

CanOpenStatus ClientSDO_NET::registerObjectWriteResultCallback(CliWriteResultDelegate^ writeDelegate, System::Object^ obj)
{
  typedef void (*CliWriteResultFuncPtr)(void *context, canOpenStatus status, u8 node_id, u16 object_index, u8 sub_index, u32 co_error_code);
  this->writeObjectResultDelegate = writeDelegate;
  this->writeObjectResultDelegateObject = obj;

  cliWriteResultDelegate =  gcnew ClientWriteResultDelegate( this, &ClientSDO_NET::clientWriteResultWrapperCallback );
  IntPtr pCliWriteResultDelegate = Marshal::GetFunctionPointerForDelegate(cliWriteResultDelegate);
  void *pf = pCliWriteResultDelegate.ToPointer();
  this->cpp_ClientSDO->registerObjectWriteResultCallback((CliWriteResultFuncPtr)pf, 0); 

  return CanOpenStatus::CANOPEN_OK;
}

CanOpenStatus ClientSDO_NET::unregisterObjectReadWriteResultCallbacks(void)
{
  this->writeObjectResultDelegate = nullptr;
  this->writeObjectResultDelegateObject = nullptr;
  return (CanOpenStatus)this->cpp_ClientSDO->unregisterObjectReadWriteResultCallbacks();
}

void ClientSDO_NET::setWriteObjectTimeout(u32 timeout)
{
  this->cpp_ClientSDO->setWriteObjectTimeout(timeout);
}
  
void ClientSDO_NET::setReadObjectTimeout(u32 timeout)
{
   this->cpp_ClientSDO->setReadObjectTimeout(timeout);
}

void ClientSDO_NET::setNodeResponseTimeout(u32 timeout)
{
   this->cpp_ClientSDO->setNodeResponseTimeout(timeout);
}

bool ClientSDO_NET::isObjectWriteResultCallbackEnabled(void)
{
  return this->cpp_ClientSDO->isObjectWriteResultCallbackEnabled();
}

bool ClientSDO_NET::isObjectReadResultCallbackEnabled(void)
{
  return this->cpp_ClientSDO->isObjectReadResultCallbackEnabled();
}


CanOpenStatus ClientSDO_NET::objectRead(u16 object_index, u8 sub_index, 
                [System::Runtime::InteropServices::Out] u8 %val, 
                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenErrorCode temp_coErrorCode;
  CanOpenStatus ret;
  u8 temp_val;
  
  ret = (CanOpenStatus)this->cpp_ClientSDO->objectRead(object_index, sub_index, &temp_val, &temp_coErrorCode);
  val = temp_val;
  coErrorCode = temp_coErrorCode;
  return ret;
}

CanOpenStatus ClientSDO_NET::objectRead(u16 object_index, 
                             u8 sub_index, 
                             array<Byte>^ data_buffer, 
                             [System::Runtime::InteropServices::Out] u32 %valid,
                             [System::Runtime::InteropServices::Out] u32 %coErrorCode)
{
  CanOpenStatus ret;
  u32 temp_coErrorCode;
  u32 temp_valid;
  WaitForSingleObject( this->rx_tx_mutex, INFINITE);

  pin_ptr<u8> p = &data_buffer[0];   // pin pointer to first element in arr
  u8* np = p;   // pointer to the first element in arr

  ret = (CanOpenStatus)this->cpp_ClientSDO->objectRead( object_index, 
                                    sub_index, 
                                    np, 
                                    data_buffer->Length, 
                                    &temp_valid, 
                                    &temp_coErrorCode);

  coErrorCode = temp_coErrorCode;
  if (ret == CanOpenStatus::CANOPEN_OK)
  {
	valid = temp_valid;
  }

  ReleaseMutex(this->rx_tx_mutex);
  return ret;
}


CanOpenStatus ClientSDO_NET::objectRead(u16 object_index, u8 sub_index, 
                [System::Runtime::InteropServices::Out] u16 %val, 
                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenErrorCode temp_coErrorCode;
  CanOpenStatus ret;
  u16 temp_val;
  
  ret = (CanOpenStatus)this->cpp_ClientSDO->objectRead(object_index, sub_index, &temp_val, &temp_coErrorCode);
  val = temp_val;
  coErrorCode = temp_coErrorCode;
  return ret;
}

CanOpenStatus ClientSDO_NET::objectRead(u16 object_index, u8 sub_index, 
                [System::Runtime::InteropServices::Out] u32 %val, 
                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenErrorCode temp_coErrorCode;
  CanOpenStatus ret;
  u32 temp_val;
  
  ret = (CanOpenStatus)this->cpp_ClientSDO->objectRead(object_index, sub_index, &temp_val, &temp_coErrorCode);
  val = temp_val;
  coErrorCode = temp_coErrorCode;
  return ret;
}
#ifdef HAS_LONG_LONG

CanOpenStatus ClientSDO_NET::objectRead(u16 object_index, u8 sub_index, 
                [System::Runtime::InteropServices::Out] u64 %val, 
                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenErrorCode temp_coErrorCode;
  CanOpenStatus ret;
  u64 temp_val;
  
  ret = (CanOpenStatus)this->cpp_ClientSDO->objectRead(object_index, sub_index, &temp_val, &temp_coErrorCode);
  val = temp_val;
  coErrorCode = temp_coErrorCode;
  return ret;
}

#endif
CanOpenStatus  ClientSDO_NET::objectWrite(u16 object_index, 
                            u8 sub_index, 
                            array<Byte>^ data_buffer, //u8 *buf, 
                            u32 valid, 
                            [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenStatus ret;
  CanOpenErrorCode temp_coErrorCode;
  
  WaitForSingleObject( this->rx_tx_mutex, INFINITE);

  pin_ptr<u8> p = &data_buffer[0];   // pin pointer to first element in arr
  u8* np = p;   // pointer to the first element in arr

  ret = (CanOpenStatus)this->cpp_ClientSDO->objectWrite(object_index,
                                    sub_index,
                                    np,
                                    valid,
                                    &temp_coErrorCode);
    
  coErrorCode = temp_coErrorCode;

  ReleaseMutex(this->rx_tx_mutex);

  return ret;
}

CanOpenStatus  ClientSDO_NET::objectWriteBlock(u16 object_index, 
                                u8 sub_index, 
                                u16 crc, 
                                array<Byte>^ data_buffer, 
                                u32 valid, 
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenStatus ret;
  CanOpenErrorCode temp_coErrorCode;

  WaitForSingleObject(this->rx_tx_mutex, INFINITE);

  pin_ptr<u8> p = &data_buffer[0];   // pin pointer to first element in arr
  u8* np = p;   // pointer to the first element in arr

    ret = (CanOpenStatus)this->cpp_ClientSDO->objectWriteBlock(object_index,
                                      sub_index,
                                      crc,
                                      np,
                                      valid,
                                      &temp_coErrorCode);
    
    coErrorCode = temp_coErrorCode; 


  ReleaseMutex(this->rx_tx_mutex);
  return ret;
}

CanOpenStatus  ClientSDO_NET::objectWrite(u16 object_index, 
                             u8 sub_index, 
                             u8  val, 
                             [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenStatus ret;
  CanOpenErrorCode temp_coErrorCode;

  ret = (CanOpenStatus)this->cpp_ClientSDO->objectWrite(object_index,
                                    sub_index,
                                    val,
                                    &temp_coErrorCode);
  coErrorCode = temp_coErrorCode;
  return ret;
}

CanOpenStatus  ClientSDO_NET::objectWrite(u16 object_index, 
                             u8 sub_index, 
                             u16 val, 
                             [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenStatus ret;
  CanOpenErrorCode temp_coErrorCode;

  ret = (CanOpenStatus)this->cpp_ClientSDO->objectWrite(object_index,
                                    sub_index,
                                    val,
                                    &temp_coErrorCode);
  coErrorCode = temp_coErrorCode;
  return ret;
}

CanOpenStatus  ClientSDO_NET::objectWrite(u16 object_index, 
                             u8 sub_index, 
                             u32 val, 
                             [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenStatus ret;
  CanOpenErrorCode temp_coErrorCode;

  ret = (CanOpenStatus)this->cpp_ClientSDO->objectWrite(object_index,
                                    sub_index,
                                    val,
                                    &temp_coErrorCode);
  coErrorCode = temp_coErrorCode;
  return ret;
}

#ifdef HAS_LONG_LONG
CanOpenStatus  ClientSDO_NET::objectWrite(u16 object_index, 
                             u8 sub_index, 
                             u64 val, 
                             [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenStatus ret;
  CanOpenErrorCode temp_coErrorCode;

  ret = (CanOpenStatus)this->cpp_ClientSDO->objectWrite(object_index,
                                    sub_index,
                                    val,
                                    &temp_coErrorCode);
  coErrorCode = temp_coErrorCode;
  return ret;
}
#endif


CanOpenStatus ClientSDO_NET::connect(u8 node_id)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_ClientSDO->connect(node_id);
  return ret;
}

CanOpenStatus ClientSDO_NET::connect(COBID cobid_tx, COBID cobid_rx)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_ClientSDO->connect(cobid_tx, cobid_rx);
  return ret;
}

CanOpenStatus  ClientSDO_NET::canHardwareConnect(int port, int bitrate)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_ClientSDO->canHardwareConnect(port,bitrate);
  return ret;
}

CanOpenStatus  ClientSDO_NET::canHardwareDisconnect(void)
{
  return (CanOpenStatus)this->cpp_ClientSDO->canHardwareDisconnect(); 
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ClientSDO_NET :: clientReadResultWrapperCallback(void *context,
                                                      canOpenStatus status,
                                                      u8 node_id,
                                                      u16 object_index,
                                                      u8 sub_index,
                                                      u8 *buffer,
                                                      u32 valid,
                                                      u32 co_error_code)
{
  for (u32 i = 0; i < valid; i++)
  {
    this->applications_buffer[i] = buffer[i];
  }

  if (this->readObjectResultDelegate != nullptr)
  {
    this->readObjectResultDelegate(this->readObjectResultDelgateObject,
                                   (CanOpenStatus)status,
                                   node_id, object_index,
                                   sub_index,
                                   this->applications_buffer,
                                   valid,
                                   co_error_code );
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ClientSDO_NET :: clientWriteResultWrapperCallback(void *context,
                                                       canOpenStatus status,
                                                       u8 node_id,
                                                       u16 object_index,
                                                       u8 sub_index,
                                                       u32 co_error_code)
{
  if (this->writeObjectResultDelegate != nullptr)
  {
    this->writeObjectResultDelegate( this->writeObjectResultDelegateObject, (CanOpenStatus)status, node_id, object_index, sub_index, co_error_code);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// **************************
// Network Mangement Master
// **************************

CanOpenStatus NMT_Master_NET::NMTOperationalStateWrapperCPP(void *obj,
                                                                            u8 node_id,
                                                                            u8 state)
{
  // Convert and call the C# callback
  if ( this->nmtOperationalStateDelegate_CS != nullptr )
  {
    return (CanOpenStatus)this->nmtOperationalStateDelegate_CS(this->nmtOperationalStateDelegateObject, node_id, state);
  }
  else
  { 
    return CanOpenStatus::CANOPEN_OK;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::NMT_Master_NET()
{
  this->cpp_NMTMaster = new NMTMaster();
  this->nmtOperationalStateDelegate_CS = nullptr;
  this->nmtOperationalStateDelegateObject = nullptr;

  // Wrap the C++ callback
  nmtOperationalStateDelegateCPP =  gcnew NMTOperationalStateDelegate_CPP(this,
    &NMT_Master_NET::NMTOperationalStateWrapperCPP );
  IntPtr pNMTOperationalStateWrapper = Marshal::GetFunctionPointerForDelegate(nmtOperationalStateDelegateCPP);
  void *pf = pNMTOperationalStateWrapper.ToPointer();
  this->cpp_NMTMaster->registerRemoteNodeStateCallback((NMTOperationalStateFunPtr)pf, 0);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::~NMT_Master_NET()
{
  delete this->cpp_NMTMaster;
  this->cpp_NMTMaster = nullptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Master_NET::canHardwareConnect(int port, int btr)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_NMTMaster->canHardwareConnect(port, btr);

  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Master_NET::canHardwareDisconnect(void)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_NMTMaster->canHardwareDisconnect();

  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Master_NET::nodeStart(u8 node_id)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_NMTMaster->nodeStart(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Master_NET::nodeStop(u8 node_id)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_NMTMaster->nodeStop(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Master_NET::nodePreoperational(u8 node_id)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_NMTMaster->nodePreoperational(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Master_NET::nodeReset(u8 node_id)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_NMTMaster->nodeReset(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Master_NET::nodeResetCommunication(u8 node_id)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_NMTMaster->nodeResetCommunication(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Master_NET::nodeGuardPollStart(u8 node_id,
                                                                  u32 node_life_time_ms)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_NMTMaster->nodeGuardPollStart(node_id,
    node_life_time_ms);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Master_NET::nodeGuardPollStop(u8 node_id)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_NMTMaster->nodeGuardPollStop(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Master_NET::heartbeatMonitorStart(u8 node_id, u32 heartbeat_consumer_time_ms)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_NMTMaster->heartbeatMonitorStart(node_id, heartbeat_consumer_time_ms);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Master_NET::heartbeatMonitorStop(u8 node_id)
{
  CanOpenStatus ret;

  ret = (CanOpenStatus)this->cpp_NMTMaster->heartbeatMonitorStop(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus NMT_Master_NET::registerNodeStateCallback(NMTOperationalStateDelegate^ opStateDelegate,
                                                                        System::Object^ obj)
{
  CanOpenStatus ret;
  this->nmtOperationalStateDelegateObject = obj;
  this->nmtOperationalStateDelegate_CS = opStateDelegate;
  ret = CanOpenStatus::CANOPEN_OK;
  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Master_NET::nodeReadOperationalState(u8 node_id, 
                                                                        u32 maxMsTimeout,
                                                                        [System::Runtime::InteropServices::Out] u8 %node_state)
{
  CanOpenStatus ret;
  NodeState tmp_NodeState;

  ret =(CanOpenStatus)this->cpp_NMTMaster->nodeReadOperationalState(node_id,
    maxMsTimeout,
    &tmp_NodeState);

  node_state = (NodeState)tmp_NodeState;
  return ret;

}

// **************************
// Network Mangement Slave
// **************************

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Slave_NET::NMT_Slave_NET()
{
  this->cpp_NMTSlave = new NMTSlave();

  this->nmtLocalNodeOperationalStateDelegate_CS = nullptr;

  nmtLocalNodeOperationalStateDelegatCPP =  gcnew NMTLocalNodeOperationalStateDelegate_CPP(this,
    &NMT_Slave_NET::NMTLocalNodeOperationalStateWrapperCPP );
  IntPtr pNMTLocalNodeOperationalStateWrapper = Marshal::GetFunctionPointerForDelegate(nmtLocalNodeOperationalStateDelegatCPP);
  void *pf = pNMTLocalNodeOperationalStateWrapper.ToPointer();
  this->cpp_NMTSlave->registerLocalNodeStateChangeCallback((NMTLocalNodeOperationalStateFunPtr)pf, 0);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Slave_NET::~NMT_Slave_NET()
{
  delete this->cpp_NMTSlave;
  this->cpp_NMTSlave = NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Slave_NET :: canHardwareConnect(int port, int btr)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_NMTSlave->canHardwareConnect( port, btr );
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Slave_NET :: canHardwareDisconnect(void)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_NMTSlave->canHardwareDisconnect();
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Slave_NET :: nodeSetId(u8 node_id)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_NMTSlave->nodeSetId(node_id);
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
CanOpenStatus  NMT_Slave_NET :: nodeSetState(u8 node_state)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_NMTSlave->nodeSetState(node_state);
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Slave_NET :: writeMessage(u32 id, array<Byte>^ msg, u8 dlc, u32 flags)
{
  u8 *msg_cpp = new u8[dlc];

  for (int j = 0; j < dlc; j++)
  {
    msg_cpp[j] = msg[j];
  }

  CanOpenStatus ret  = (CanOpenStatus)this->cpp_NMTSlave->writeMessage(id,
    msg_cpp,
    dlc,
    flags);

  delete[] msg_cpp;
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  NMT_Slave_NET :: registerLocalNodeStateChangeCallback(
  NMTLocalNodeOperationalStateDelegate^ localOperationalStateDelegate,
  System::Object^ obj)
{
  CanOpenStatus ret = CanOpenStatus::CANOPEN_OK;
  this->nmtLocalNodeOperationalStateDelegateObject = obj;
  this->nmtLocalNodeOperationalStateDelegate_CS = localOperationalStateDelegate;
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus NMT_Slave_NET :: NMTLocalNodeOperationalStateWrapperCPP(void *obj, u8 state)
{
  // Convert and call the C# callback
  if (nmtLocalNodeOperationalStateDelegate_CS != nullptr)
    return (CanOpenStatus)this->nmtLocalNodeOperationalStateDelegate_CS(
    this->nmtLocalNodeOperationalStateDelegateObject , state);
  else 
    return CanOpenStatus::CANOPEN_OK;
}

// **************************
// LSS Master
// **************************

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Master_NET::LSS_Master_NET()
{
  this->cpp_LSSMaster = new LSSMaster();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Master_NET::~LSS_Master_NET()
{
  delete this->cpp_LSSMaster;
  this->cpp_LSSMaster = NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Master_NET :: canHardwareConnect(int port, int btr)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_LSSMaster->canHardwareConnect( port, btr );
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Master_NET :: canHardwareDisconnect(void)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_LSSMaster->canHardwareDisconnect();
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Master_NET :: switchModeGlobal(u8 mode)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_LSSMaster->switchModeGlobal(mode);
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Master_NET :: switchModeSelectiveVendorId(u32 vendorId)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_LSSMaster->switchModeSelectiveVendorId(vendorId);
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Master_NET :: switchModeSelectiveProductCode(u32 productCode)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_LSSMaster->switchModeSelectiveProductCode(productCode);
  return ret;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Master_NET :: switchModeSelectiveRevisionNumber(u32 revisionNumber)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_LSSMaster->switchModeSelectiveRevisionNumber(revisionNumber);
  return ret;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Master_NET :: switchModeSelectiveSerialNumber(u32 serialNumber)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_LSSMaster->switchModeSelectiveSerialNumber(serialNumber);
  return ret;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Master_NET :: configureNodeId(u8 nodeId,
                                                                   [System::Runtime::InteropServices::Out] u8 %errorCode,
                                                                   [System::Runtime::InteropServices::Out] u8 %specificErrorCode)
{
  CanOpenStatus ret;
  u8 cpp_error_code;
  u8 cpp_specific_error_code;
  ret = (CanOpenStatus)this->cpp_LSSMaster->configureNodeId(nodeId,
    &cpp_error_code,
    &cpp_specific_error_code);
  errorCode = cpp_error_code;
  specificErrorCode = cpp_specific_error_code;
  return ret;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Master_NET :: configureBitTimingParamteres(u8 tableSelector,
                                                                                u8 tableIndex,
                                                                                [System::Runtime::InteropServices::Out] u8 %errorCode,
                                                                                [System::Runtime::InteropServices::Out] u8 %specificErrorCode)
{
  CanOpenStatus ret;
  u8 cpp_error_code;
  u8 cpp_specific_error_code;
  ret = (CanOpenStatus)this->cpp_LSSMaster->configureBitTimingParamteres(tableSelector,
    tableIndex,
    &cpp_error_code,
    &cpp_specific_error_code);
  errorCode = cpp_error_code;
  specificErrorCode = cpp_specific_error_code;
  return ret;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Master_NET :: activateBitTimingParameters(u16 switchDelay)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_LSSMaster->activateBitTimingParameters(switchDelay);
  return ret;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Master_NET :: storeConfiguration([System::Runtime::InteropServices::Out] u8 %errorCode,
                                                                      [System::Runtime::InteropServices::Out] u8 %specificErrorCode)
{
  CanOpenStatus ret;
  u8 cpp_error_code;
  u8 cpp_specific_error_code;
  ret = (CanOpenStatus)this->cpp_LSSMaster->storeConfiguration(&cpp_error_code, &cpp_specific_error_code);
  errorCode = cpp_error_code;
  specificErrorCode = cpp_specific_error_code;
  return ret;
}

// **************************
// LSS Slave
// **************************

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Slave_NET::LSS_Slave_NET()
{
  this->cpp_LSSSlave = new LSSSlave();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Slave_NET::~LSS_Slave_NET()
{
  delete this->cpp_LSSSlave;
  this->cpp_LSSSlave = NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Slave_NET :: canHardwareConnect(int port, int btr)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_LSSSlave->canHardwareConnect( port, btr );
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CanOpenStatus  LSS_Slave_NET :: canHardwareDisconnect(void)
{
  CanOpenStatus ret;
  ret = (CanOpenStatus)this->cpp_LSSSlave->canHardwareDisconnect();
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void  LSS_Slave_NET :: setDeviceParameters(u32 vendorId, u32 productCode, u32 revisionNumber, u32 serialNumber)
{
  return this->cpp_LSSSlave->setDeviceParameters(vendorId, productCode, revisionNumber, serialNumber);
}

// **************************
// Library configuration
// **************************

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void CANopenLibraryConfiguration :: SetAdapter(CanAdapter adapter)
{
  return ::setAdapter((canAdapter)adapter);
}