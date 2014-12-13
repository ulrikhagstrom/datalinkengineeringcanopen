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

ServerSDO_NET::CanOpenStatus ServerSDO_NET::registerObjectReadCallback(SrvReadDelegate^ readDelegate, System::Object^ obj)
{
  this->readObjectDelegate = readDelegate;
  this->readObjectDelgateObject = obj;
  return ServerSDO_NET::CanOpenStatus::CANOPEN_OK;
}

ServerSDO_NET::CanOpenStatus ServerSDO_NET::registerObjectWriteCallback(SrvWriteDelegate^ writeDelegate, System::Object^ obj)
{
  this->writeObjectDelegate = writeDelegate;
  this->writeObjectDelegateObject = obj;
  return ServerSDO_NET::CanOpenStatus::CANOPEN_OK;
}
  
ServerSDO_NET::CanOpenStatus ServerSDO_NET::registerObjectGetAttributesCallback(SrvGetAttrDelegate^ getAttrDelegate, System::Object^ obj)
{
  this->attribObjectDelegate = getAttrDelegate;
  this->attribObjectDelegateObject = obj;
  return ServerSDO_NET::CanOpenStatus::CANOPEN_OK;
}

ServerSDO_NET::CanOpenStatus ServerSDO_NET::canHardwareConnect(int port, int bitrate)
{
  return (ServerSDO_NET::CanOpenStatus)this->cpp_ServerSDO->canHardwareConnect(port, bitrate); 
}

ServerSDO_NET::CanOpenStatus  ServerSDO_NET::canHardwareDisconnect(void)
{
  return (ServerSDO_NET::CanOpenStatus)this->cpp_ServerSDO->canHardwareDisconnect(); 
}


ServerSDO_NET::CanOpenStatus ServerSDO_NET::nodeSetId(u8 node_id)
{
  return (ServerSDO_NET::CanOpenStatus)this->cpp_ServerSDO->nodeSetId(node_id);
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
  delete this->cpp_CanMonitor;
  this->cpp_CanMonitor = nullptr;
}


CanMonitor_NET::CanOpenStatus CanMonitor_NET::canHardwareConnect(int port, int bitrate)
{
  return (CanMonitor_NET::CanOpenStatus)this->cpp_CanMonitor->canHardwareConnect(port, bitrate); 
}


CanMonitor_NET::CanOpenStatus CanMonitor_NET::canHardwareDisconnect(void)
{
  return (CanMonitor_NET::CanOpenStatus)this->cpp_CanMonitor->canHardwareDisconnect();
}

CanMonitor_NET::CanOpenStatus CanMonitor_NET::registerCanReceiveCallback( System::Object^ obj, CanReceiveDelegate^ can_receive_delegate )
{
  this->can_receive_delegate = can_receive_delegate;
  this->can_receive_delegate_object = obj;
  return CanMonitor_NET::CanOpenStatus::CANOPEN_OK;
}

CanMonitor_NET::CanOpenStatus CanMonitor_NET::canWrite( u32 id,  array<Byte>^ data, u8 dlc, u32 flags)
{
  u8 *data_cpp = new u8[dlc];

  for (int j = 0; j < dlc; j++)
    data_cpp[j] = data[j]; 

  CanMonitor_NET::CanOpenStatus ret = (CanMonitor_NET::CanOpenStatus)this->cpp_CanMonitor->canWrite( id, data_cpp, dlc, flags );

  delete[] data_cpp;
  return ret;
}

canOpenStatus CanMonitor_NET::canReceiveCPP(void *context, u32 id, u8 *data, u8 dlc, u32 flags )
{
  canOpenStatus ret;
  array<System::Byte>^ data_cs;
  data_cs = gcnew array<System::Byte>(dlc);

  for (u8 i = 0; i < dlc; i++)
    data_cs[i] = data[i];

  ret = (canOpenStatus)this->can_receive_delegate(this->can_receive_delegate_object, 
    id, data_cs, dlc, flags);
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


EmcyClient_NET::CanOpenStatus EmcyClient_NET::canHardwareConnect(int port, int bitrate)
{
  return (EmcyClient_NET::CanOpenStatus)this->cpp_EmcyClient->canHardwareConnect(port, bitrate); 
}


EmcyClient_NET::CanOpenStatus EmcyClient_NET::canHardwareDisconnect(void)
{
  return (EmcyClient_NET::CanOpenStatus)this->cpp_EmcyClient->canHardwareDisconnect();
}
  
EmcyClient_NET::CanOpenStatus EmcyClient_NET::nodeSetId(u8 nodeId)
{
  return (EmcyClient_NET::CanOpenStatus)cpp_EmcyClient->nodeSetId(nodeId);
}


EmcyClient_NET::CanOpenStatus EmcyClient_NET::sendEmcyMessage( u8 nodeId, u16 emcy_error_code, u8 error_register, array<Byte>^ manufSpecificErrorField )
{
  u8 *data_cpp = new u8[5];

  for (int j = 0; j < 5; j++)
    data_cpp[j] = manufSpecificErrorField[j]; 

  EmcyClient_NET::CanOpenStatus ret = (EmcyClient_NET::CanOpenStatus)this->cpp_EmcyClient->sendEmcyMessage(nodeId, emcy_error_code, error_register, data_cpp);

  delete[] data_cpp;
  return ret;
}


EmcyClient_NET::CanOpenStatus EmcyClient_NET::sendEmcyMessage( u16 emcy_error_code, u8 error_register, array<Byte>^ manufSpecificErrorField )
{
  u8 *data_cpp = new u8[5];

  for (int j = 0; j < 5; j++)
    data_cpp[j] = manufSpecificErrorField[j]; 

  EmcyClient_NET::CanOpenStatus ret = (EmcyClient_NET::CanOpenStatus)this->cpp_EmcyClient->sendEmcyMessage(emcy_error_code, error_register, data_cpp);

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


SyncProducer_NET::CanOpenStatus SyncProducer_NET::canHardwareConnect(int port, int bitrate)
{
  return (SyncProducer_NET::CanOpenStatus)this->cpp_SyncProducer->canHardwareConnect(port, bitrate); 
}


SyncProducer_NET::CanOpenStatus SyncProducer_NET::canHardwareDisconnect(void)
{
  return (SyncProducer_NET::CanOpenStatus)this->cpp_SyncProducer->canHardwareDisconnect();
}

SyncProducer_NET::CanOpenStatus SyncProducer_NET::setSyncCOBID(u32 id)
{
  if (cpp_SyncProducer == nullptr)
    return (SyncProducer_NET::CanOpenStatus::CANOPEN_INTERNAL_STATE_ERROR);

  SyncProducer_NET::CanOpenStatus ret = (SyncProducer_NET::CanOpenStatus)this->cpp_SyncProducer->setSyncCOBID(id);

  return ret;

}
  
SyncProducer_NET::CanOpenStatus SyncProducer_NET::setTransmissionPeriodTime(unsigned long sync_period_time)
{
  if (cpp_SyncProducer == nullptr)
    return (SyncProducer_NET::CanOpenStatus::CANOPEN_INTERNAL_STATE_ERROR);

  SyncProducer_NET::CanOpenStatus ret = (SyncProducer_NET::CanOpenStatus)this->cpp_SyncProducer->setTransmissionPeriodTime(sync_period_time);

  return ret;

}

SyncProducer_NET::CanOpenStatus SyncProducer_NET::startPeriodicTransmission(bool value)
{
  return (SyncProducer_NET::CanOpenStatus)cpp_SyncProducer->startPeriodicTransmission(value);
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


TransmitPDO_NET::CanOpenStatus TransmitPDO_NET::canHardwareConnect(int port, int bitrate)
{
  return (CanMonitor_NET::CanOpenStatus)this->cpp_TransmitPDO->canHardwareConnect(port, bitrate); 
}


TransmitPDO_NET::CanOpenStatus TransmitPDO_NET::canHardwareDisconnect(void)
{
  return (CanMonitor_NET::CanOpenStatus)this->cpp_TransmitPDO->canHardwareDisconnect();
}

TransmitPDO_NET::CanOpenStatus TransmitPDO_NET::setup(u32 id,  array<Byte>^ data, u8 dlc)
{
  if (cpp_TransmitPDO == nullptr)
    return (TransmitPDO_NET::CanOpenStatus::CANOPEN_INTERNAL_STATE_ERROR);

  u8 *data_cpp = new u8[8];

  if (dlc < 0 || dlc > 8)
    return CanMonitor_NET::CanOpenStatus::CANOPEN_ARG_ERROR;

  for (int j = 0; j < dlc; j++)
    data_cpp[j] = data[j]; 

  TransmitPDO_NET::CanOpenStatus ret = (TransmitPDO_NET::CanOpenStatus)this->cpp_TransmitPDO->setData( id, data_cpp, dlc);

  delete[] data_cpp;
  return ret;

}
  
TransmitPDO_NET::CanOpenStatus TransmitPDO_NET::transmit(void)
{
  if (cpp_TransmitPDO != nullptr)
    return (TransmitPDO_NET::CanOpenStatus)cpp_TransmitPDO->transmitPdo();
  else
    return (TransmitPDO_NET::CanOpenStatus::CANOPEN_INTERNAL_STATE_ERROR);
}


TransmitPDO_NET::CanOpenStatus TransmitPDO_NET::periodicTransmission(bool value)
{
  return (TransmitPDO_NET::CanOpenStatus)cpp_TransmitPDO->periodicTransmission(value);
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


ReceivePDO_NET::CanOpenStatus ReceivePDO_NET::canHardwareConnect(int port, int bitrate)
{
  return (ReceivePDO_NET::CanOpenStatus)this->cpp_ReceivePDO->canHardwareConnect(port, bitrate); 
}


ReceivePDO_NET::CanOpenStatus ReceivePDO_NET::canHardwareDisconnect(void)
{
  return (ReceivePDO_NET::CanOpenStatus)this->cpp_ReceivePDO->canHardwareDisconnect();
}

ReceivePDO_NET::CanOpenStatus ReceivePDO_NET::registerReceivePdoMessageCallBack( System::Object^ obj, ReceivePdoDelegate^ receive_pdo_delegate )
{
  this->receive_pdo_delegate = receive_pdo_delegate;
  this->receive_pdo_delegate_object = obj;
  return ReceivePDO_NET::CanOpenStatus::CANOPEN_OK;
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

ReceivePDO_NET::CanOpenStatus ReceivePDO_NET::setCobid(u32 cobid)
{
  return (ReceivePDO_NET::CanOpenStatus)this->cpp_ReceivePDO->setCobid( cobid );
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

SyncServer_NET::CanOpenStatus SyncServer_NET::canHardwareConnect(int port, int bitrate)
{
  return (ReceivePDO_NET::CanOpenStatus)this->cpp_SyncServer->canHardwareConnect(port, bitrate); 
}

SyncServer_NET::CanOpenStatus SyncServer_NET::canHardwareDisconnect(void)
{
  return (ReceivePDO_NET::CanOpenStatus)this->cpp_SyncServer->canHardwareDisconnect();
}

SyncServer_NET::CanOpenStatus SyncServer_NET::registerSyncMessageCallBack( System::Object^ obj, SyncServerDelegate^ sync_server_delegate )
{
  this->sync_server_delegate = sync_server_delegate;
  this->sync_server_delegate_object = obj;
  return ReceivePDO_NET::CanOpenStatus::CANOPEN_OK;
}

canOpenStatus SyncServer_NET::syncServerCallbackCPP(void *context)
{
  canOpenStatus ret;
  ret = (canOpenStatus)this->sync_server_delegate(this->sync_server_delegate_object);
  return ret;
}

SyncServer_NET::CanOpenStatus SyncServer_NET::setCobid(u32 cobid)
{
  return (SyncServer_NET::CanOpenStatus)this->cpp_SyncServer->reconfigureSyncCOBID( cobid );
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


EmcyServer_NET::CanOpenStatus EmcyServer_NET::canHardwareConnect(int port, int bitrate)
{
  return (EmcyServer_NET::CanOpenStatus)this->cpp_EmcyServer->canHardwareConnect(port, bitrate); 
}


EmcyServer_NET::CanOpenStatus EmcyServer_NET::canHardwareDisconnect(void)
{
  return (EmcyServer_NET::CanOpenStatus)this->cpp_EmcyServer->canHardwareDisconnect();
}

EmcyServer_NET::CanOpenStatus EmcyServer_NET::registerEmcyServerMessageCallBack( System::Object^ obj, EmcyServerDelegate^ emcy_server_delegate )
{
  this->emcy_server_delegate = emcy_server_delegate;
  this->emcy_server_delegate_object = obj;
  return EmcyServer_NET::CanOpenStatus::CANOPEN_OK;
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
  this->temp_data_buffer = NULL;
  this->readObjectResultDelegate = nullptr;
  this->readObjectResultDelgateObject = nullptr;
  this->writeObjectResultDelegate = nullptr;
  this->writeObjectResultDelegateObject = nullptr;

  this->cpp_ClientSDO = new ClientSDO();
}

ClientSDO_NET::~ClientSDO_NET()
{
  delete this->cpp_ClientSDO;
  this->cpp_ClientSDO = NULL;
}

ClientSDO_NET::CanOpenStatus ClientSDO_NET::registerObjectReadResultCallback(CliReadResultDelegate^ readResultDelegate, System::Object^ obj)
{
  typedef void (*CliReadResultFuncPtr)(void *context, canOpenStatus status, u8 nodeid, u16 object_index, u8 sub_index, u8 *buffer, u32 valid, u32 co_error_code);
  this->readObjectResultDelegate = readResultDelegate;
  this->readObjectResultDelgateObject = obj;

  cliReadResultDelegate =  gcnew ClientReadResultDelegate( this, &ClientSDO_NET::clientReadResultWrapperCallback );
  IntPtr pCliReadResultDelegate = Marshal::GetFunctionPointerForDelegate(cliReadResultDelegate);
  void *pf = pCliReadResultDelegate.ToPointer();
  this->cpp_ClientSDO->registerObjectReadResultCallback((CliReadResultFuncPtr)pf, 0); 

  return ClientSDO_NET::CanOpenStatus::CANOPEN_OK;
}

ClientSDO_NET::CanOpenStatus ClientSDO_NET::registerObjectWriteResultCallback(CliWriteResultDelegate^ writeDelegate, System::Object^ obj)
{
  typedef void (*CliWriteResultFuncPtr)(void *context, canOpenStatus status, u8 node_id, u16 object_index, u8 sub_index, u32 co_error_code);
  this->writeObjectResultDelegate = writeDelegate;
  this->writeObjectResultDelegateObject = obj;

  cliWriteResultDelegate =  gcnew ClientWriteResultDelegate( this, &ClientSDO_NET::clientWriteResultWrapperCallback );
  IntPtr pCliWriteResultDelegate = Marshal::GetFunctionPointerForDelegate(cliWriteResultDelegate);
  void *pf = pCliWriteResultDelegate.ToPointer();
  this->cpp_ClientSDO->registerObjectWriteResultCallback((CliWriteResultFuncPtr)pf, 0); 

  return ClientSDO_NET::CanOpenStatus::CANOPEN_OK;
}

ClientSDO_NET::CanOpenStatus ClientSDO_NET::unregisterObjectReadWriteResultCallbacks(void)
{
  this->writeObjectResultDelegate = nullptr;
  this->writeObjectResultDelegateObject = nullptr;
  return (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->unregisterObjectReadWriteResultCallbacks();
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


ClientSDO_NET::CanOpenStatus ClientSDO_NET::objectRead(u16 object_index, u8 sub_index, 
                [System::Runtime::InteropServices::Out] u8 %val, 
                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenErrorCode temp_coErrorCode;
  ClientSDO_NET::CanOpenStatus ret;
  u8 temp_val;
  
  ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->objectRead(object_index, sub_index, &temp_val, &temp_coErrorCode);
  val = temp_val;
  coErrorCode = temp_coErrorCode;
  return ret;
}

ClientSDO_NET::CanOpenStatus ClientSDO_NET::objectRead(u16 object_index, 
                             u8 sub_index, 
                             array<Byte>^ data_buffer, 
                             [System::Runtime::InteropServices::Out] u32 %valid,
                             [System::Runtime::InteropServices::Out] u32 %coErrorCode)
{
  ClientSDO_NET::CanOpenStatus ret;
  u32 temp_coErrorCode;
  u32 temp_valid;

  if (this->temp_data_buffer == NULL)
  {
    this->temp_data_buffer = new u8[data_buffer->Length];
    this->applicationsBuffer = data_buffer;

    ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->objectRead( object_index, 
                                       sub_index, 
                                       temp_data_buffer, 
                                       data_buffer->Length, 
                                       &temp_valid, 
                                       &temp_coErrorCode);

    if (ret == ClientSDO_NET::CanOpenStatus::CANOPEN_OK)
    {
      for (u32 i = 0 ; i < temp_valid; i++)
      {
        applicationsBuffer[i] = temp_data_buffer[i];
      }

      valid = temp_valid;
      coErrorCode = temp_coErrorCode;
    }
    if ( ret != ClientSDO_NET::CanOpenStatus::CANOPEN_ASYNC_TRANSFER)
    {
      delete[] this->temp_data_buffer;
      this->temp_data_buffer = NULL;
    }
  }
  else
  {
    ret = ClientSDO_NET::CanOpenStatus::CANOPEN_ERROR; // Busy.
  }

  return ret;
}


ClientSDO_NET::CanOpenStatus ClientSDO_NET::objectRead(u16 object_index, u8 sub_index, 
                [System::Runtime::InteropServices::Out] u16 %val, 
                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenErrorCode temp_coErrorCode;
  ClientSDO_NET::CanOpenStatus ret;
  u16 temp_val;
  
  ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->objectRead(object_index, sub_index, &temp_val, &temp_coErrorCode);
  val = temp_val;
  coErrorCode = temp_coErrorCode;
  return ret;
}

ClientSDO_NET::CanOpenStatus ClientSDO_NET::objectRead(u16 object_index, u8 sub_index, 
                [System::Runtime::InteropServices::Out] u32 %val, 
                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenErrorCode temp_coErrorCode;
  ClientSDO_NET::CanOpenStatus ret;
  u32 temp_val;
  
  ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->objectRead(object_index, sub_index, &temp_val, &temp_coErrorCode);
  val = temp_val;
  coErrorCode = temp_coErrorCode;
  return ret;
}
#ifdef HAS_LONG_LONG

ClientSDO_NET::CanOpenStatus ClientSDO_NET::objectRead(u16 object_index, u8 sub_index, 
                [System::Runtime::InteropServices::Out] u64 %val, 
                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  CanOpenErrorCode temp_coErrorCode;
  ClientSDO_NET::CanOpenStatus ret;
  u64 temp_val;
  
  ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->objectRead(object_index, sub_index, &temp_val, &temp_coErrorCode);
  val = temp_val;
  coErrorCode = temp_coErrorCode;
  return ret;
}

#endif
ClientSDO_NET::CanOpenStatus  ClientSDO_NET::objectWrite(u16 object_index, 
                            u8 sub_index, 
                            array<Byte>^ data_buffer, //u8 *buf, 
                            u32 valid, 
                            [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  ClientSDO_NET::CanOpenStatus ret;
  CanOpenErrorCode temp_coErrorCode;
  
  if (this->temp_data_buffer == NULL)
  {
    this->temp_data_buffer = new u8[data_buffer->Length];

    for (u32 j = 0; j < valid; j++)
    {
      this->temp_data_buffer[j] = data_buffer[j];
    }

    ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->objectWrite(object_index,
                                      sub_index,
                                      temp_data_buffer,
                                      valid,
                                      &temp_coErrorCode);
    
    if ( ret != ClientSDO_NET::CanOpenStatus::CANOPEN_ASYNC_TRANSFER)
    {
      delete[] this->temp_data_buffer;
      this->temp_data_buffer = NULL;
    }
    coErrorCode = temp_coErrorCode;
  }
  else
  {
    ret = ClientSDO_NET::CanOpenStatus::CANOPEN_ERROR; // Busy.
  }  
  return ret;
}

ClientSDO_NET::CanOpenStatus  ClientSDO_NET::objectWriteBlock(u16 object_index, 
                                u8 sub_index, 
                                u16 crc, 
                                array<Byte>^ data_buffer, 
                                u32 valid, 
                                [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  ClientSDO_NET::CanOpenStatus ret;
  CanOpenErrorCode temp_coErrorCode;

  if (this->temp_data_buffer == NULL)
  {
    temp_data_buffer = new u8[data_buffer->Length];

    for (u32 j = 0; j < valid; j++)
    {
      temp_data_buffer[j] = data_buffer[j];
    }

    ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->objectWriteBlock(object_index,
                                      sub_index,
                                      crc,
                                      temp_data_buffer,
                                      valid,
                                      &temp_coErrorCode);
    
    coErrorCode = temp_coErrorCode; 

    if (ret != ClientSDO_NET::CanOpenStatus::CANOPEN_ASYNC_TRANSFER)
    {
      delete[] this->temp_data_buffer;
      this->temp_data_buffer = NULL;
    }
  }
  else
  {
    ret = ClientSDO_NET::CanOpenStatus::CANOPEN_ERROR; // Busy
  }
  return ret;
}

ClientSDO_NET::CanOpenStatus  ClientSDO_NET::objectWrite(u16 object_index, 
                             u8 sub_index, 
                             u8  val, 
                             [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  ClientSDO_NET::CanOpenStatus ret;
  CanOpenErrorCode temp_coErrorCode;

  ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->objectWrite(object_index,
                                    sub_index,
                                    val,
                                    &temp_coErrorCode);
  coErrorCode = temp_coErrorCode;
  return ret;
}

ClientSDO_NET::CanOpenStatus  ClientSDO_NET::objectWrite(u16 object_index, 
                             u8 sub_index, 
                             u16 val, 
                             [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  ClientSDO_NET::CanOpenStatus ret;
  CanOpenErrorCode temp_coErrorCode;

  ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->objectWrite(object_index,
                                    sub_index,
                                    val,
                                    &temp_coErrorCode);
  coErrorCode = temp_coErrorCode;
  return ret;
}

ClientSDO_NET::CanOpenStatus  ClientSDO_NET::objectWrite(u16 object_index, 
                             u8 sub_index, 
                             u32 val, 
                             [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  ClientSDO_NET::CanOpenStatus ret;
  CanOpenErrorCode temp_coErrorCode;

  ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->objectWrite(object_index,
                                    sub_index,
                                    val,
                                    &temp_coErrorCode);
  coErrorCode = temp_coErrorCode;
  return ret;
}

#ifdef HAS_LONG_LONG
ClientSDO_NET::CanOpenStatus  ClientSDO_NET::objectWrite(u16 object_index, 
                             u8 sub_index, 
                             u64 val, 
                             [System::Runtime::InteropServices::Out] CanOpenErrorCode %coErrorCode)
{
  ClientSDO_NET::CanOpenStatus ret;
  CanOpenErrorCode temp_coErrorCode;

  ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->objectWrite(object_index,
                                    sub_index,
                                    val,
                                    &temp_coErrorCode);
  coErrorCode = temp_coErrorCode;
  return ret;
}
#endif


ClientSDO_NET::CanOpenStatus ClientSDO_NET::connect(u8 node_id)
{
  ClientSDO_NET::CanOpenStatus ret;

  ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->connect(node_id);
  return ret;
}

ClientSDO_NET::CanOpenStatus ClientSDO_NET::connect(COBID cobid_tx, COBID cobid_rx)
{
  ClientSDO_NET::CanOpenStatus ret;

  ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->connect(cobid_tx, cobid_rx);
  return ret;
}

ClientSDO_NET::CanOpenStatus  ClientSDO_NET::canHardwareConnect(int port, int bitrate)
{
  ClientSDO_NET::CanOpenStatus ret;

  ret = (ClientSDO_NET::CanOpenStatus)this->cpp_ClientSDO->canHardwareConnect(port,bitrate);
  return ret;
}

ClientSDO_NET::CanOpenStatus  ClientSDO_NET::canHardwareDisconnect(void)
{
  return (ServerSDO_NET::CanOpenStatus)this->cpp_ClientSDO->canHardwareDisconnect(); 
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
    this->applicationsBuffer[i] = buffer[i];
  }

  if (this->readObjectResultDelegate != nullptr)
  {
    this->readObjectResultDelegate(this->readObjectResultDelgateObject,
                                   (CanOpenStatus)status,
                                   node_id, object_index,
                                   sub_index,
                                   this->applicationsBuffer,
                                   valid,
                                   co_error_code );
  }

  if (this->temp_data_buffer != NULL)
  {
    delete[] this->temp_data_buffer;
    this->temp_data_buffer = NULL;
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
  if (this->temp_data_buffer != NULL)
  {
    delete[] this->temp_data_buffer;
    this->temp_data_buffer = NULL;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// **************************
// Network Mangement Master
// **************************

NMT_Master_NET::CanOpenStatus NMT_Master_NET::NMTOperationalStateWrapperCPP(void *obj,
                                                                            u8 node_id,
                                                                            u8 state)
{
  // Convert and call the C# callback
  if ( this->nmtOperationalStateDelegate_CS != nullptr )
  {
    return (NMT_Master_NET::CanOpenStatus)this->nmtOperationalStateDelegate_CS(this->nmtOperationalStateDelegateObject, node_id, state);
  }
  else
  { 
    return NMT_Master_NET::CanOpenStatus::CANOPEN_OK;
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

NMT_Master_NET::CanOpenStatus  NMT_Master_NET::canHardwareConnect(int port, int btr)
{
  NMT_Master_NET::CanOpenStatus ret;

  ret = (NMT_Master_NET::CanOpenStatus)this->cpp_NMTMaster->canHardwareConnect(port, btr);

  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::CanOpenStatus  NMT_Master_NET::canHardwareDisconnect(void)
{
  NMT_Master_NET::CanOpenStatus ret;

  ret = (NMT_Master_NET::CanOpenStatus)this->cpp_NMTMaster->canHardwareDisconnect();

  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::CanOpenStatus  NMT_Master_NET::nodeStart(u8 node_id)
{
  NMT_Master_NET::CanOpenStatus ret;

  ret = (NMT_Master_NET::CanOpenStatus)this->cpp_NMTMaster->nodeStart(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::CanOpenStatus  NMT_Master_NET::nodeStop(u8 node_id)
{
  NMT_Master_NET::CanOpenStatus ret;

  ret = (NMT_Master_NET::CanOpenStatus)this->cpp_NMTMaster->nodeStop(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::CanOpenStatus  NMT_Master_NET::nodePreoperational(u8 node_id)
{
  NMT_Master_NET::CanOpenStatus ret;

  ret = (NMT_Master_NET::CanOpenStatus)this->cpp_NMTMaster->nodePreoperational(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::CanOpenStatus  NMT_Master_NET::nodeReset(u8 node_id)
{
  NMT_Master_NET::CanOpenStatus ret;

  ret = (NMT_Master_NET::CanOpenStatus)this->cpp_NMTMaster->nodeReset(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::CanOpenStatus  NMT_Master_NET::nodeResetCommunication(u8 node_id)
{
  NMT_Master_NET::CanOpenStatus ret;

  ret = (NMT_Master_NET::CanOpenStatus)this->cpp_NMTMaster->nodeResetCommunication(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::CanOpenStatus  NMT_Master_NET::nodeGuardPollStart(u8 node_id,
                                                                  u32 node_life_time_ms)
{
  NMT_Master_NET::CanOpenStatus ret;

  ret = (NMT_Master_NET::CanOpenStatus)this->cpp_NMTMaster->nodeGuardPollStart(node_id,
    node_life_time_ms);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::CanOpenStatus  NMT_Master_NET::nodeGuardPollStop(u8 node_id)
{
  NMT_Master_NET::CanOpenStatus ret;

  ret = (NMT_Master_NET::CanOpenStatus)this->cpp_NMTMaster->nodeGuardPollStop(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::CanOpenStatus  NMT_Master_NET::heartbeatMonitorStart(u8 node_id, u32 heartbeat_consumer_time_ms)
{
  NMT_Master_NET::CanOpenStatus ret;

  ret = (NMT_Master_NET::CanOpenStatus)this->cpp_NMTMaster->heartbeatMonitorStart(node_id, heartbeat_consumer_time_ms);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::CanOpenStatus  NMT_Master_NET::heartbeatMonitorStop(u8 node_id)
{
  NMT_Master_NET::CanOpenStatus ret;

  ret = (NMT_Master_NET::CanOpenStatus)this->cpp_NMTMaster->heartbeatMonitorStop(node_id);

  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::CanOpenStatus NMT_Master_NET::registerNodeStateCallback(NMTOperationalStateDelegate^ opStateDelegate,
                                                                        System::Object^ obj)
{
  NMT_Master_NET::CanOpenStatus ret;
  this->nmtOperationalStateDelegateObject = obj;
  this->nmtOperationalStateDelegate_CS = opStateDelegate;
  ret = NMT_Master_NET::CanOpenStatus::CANOPEN_OK;
  return ret;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Master_NET::CanOpenStatus  NMT_Master_NET::nodeReadOperationalState(u8 node_id, 
                                                                        u32 maxMsTimeout,
                                                                        [System::Runtime::InteropServices::Out] u8 %node_state)
{
  NMT_Master_NET::CanOpenStatus ret;
  NodeState tmp_NodeState;

  ret =(NMT_Master_NET::CanOpenStatus)this->cpp_NMTMaster->nodeReadOperationalState(node_id,
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

NMT_Slave_NET :: CanOpenStatus  NMT_Slave_NET :: canHardwareConnect(int port, int btr)
{
  NMT_Slave_NET::CanOpenStatus ret;
  ret = (NMT_Slave_NET::CanOpenStatus)this->cpp_NMTSlave->canHardwareConnect( port, btr );
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Slave_NET :: CanOpenStatus  NMT_Slave_NET :: canHardwareDisconnect(void)
{
  NMT_Slave_NET::CanOpenStatus ret;
  ret = (NMT_Slave_NET::CanOpenStatus)this->cpp_NMTSlave->canHardwareDisconnect();
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Slave_NET :: CanOpenStatus  NMT_Slave_NET :: nodeSetId(u8 node_id)
{
  NMT_Slave_NET::CanOpenStatus ret;
  ret = (NMT_Slave_NET::CanOpenStatus)this->cpp_NMTSlave->nodeSetId(node_id);
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
NMT_Slave_NET :: CanOpenStatus  NMT_Slave_NET :: nodeSetState(u8 node_state)
{
  NMT_Slave_NET::CanOpenStatus ret;
  ret = (NMT_Slave_NET::CanOpenStatus)this->cpp_NMTSlave->nodeSetState(node_state);
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Slave_NET :: CanOpenStatus  NMT_Slave_NET :: writeMessage(u32 id, array<Byte>^ msg, u8 dlc, u32 flags)
{
  u8 *msg_cpp = new u8[dlc];

  for (int j = 0; j < dlc; j++)
  {
    msg_cpp[j] = msg[j];
  }

  NMT_Slave_NET::CanOpenStatus ret  = (NMT_Slave_NET::CanOpenStatus)this->cpp_NMTSlave->writeMessage(id,
    msg_cpp,
    dlc,
    flags);

  delete[] msg_cpp;
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Slave_NET :: CanOpenStatus  NMT_Slave_NET :: registerLocalNodeStateChangeCallback(
  NMTLocalNodeOperationalStateDelegate^ localOperationalStateDelegate,
  System::Object^ obj)
{
  NMT_Slave_NET::CanOpenStatus ret = NMT_Slave_NET::CanOpenStatus::CANOPEN_OK;
  this->nmtLocalNodeOperationalStateDelegateObject = obj;
  this->nmtLocalNodeOperationalStateDelegate_CS = localOperationalStateDelegate;
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

NMT_Slave_NET :: CanOpenStatus NMT_Slave_NET :: NMTLocalNodeOperationalStateWrapperCPP(void *obj, u8 state)
{
  // Convert and call the C# callback
  if (nmtLocalNodeOperationalStateDelegate_CS != nullptr)
    return (NMT_Slave_NET::CanOpenStatus)this->nmtLocalNodeOperationalStateDelegate_CS(
    this->nmtLocalNodeOperationalStateDelegateObject , state);
  else 
    return NMT_Slave_NET::CanOpenStatus::CANOPEN_OK;
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

LSS_Master_NET :: CanOpenStatus  LSS_Master_NET :: canHardwareConnect(int port, int btr)
{
  NMT_Slave_NET::CanOpenStatus ret;
  ret = (NMT_Slave_NET::CanOpenStatus)this->cpp_LSSMaster->canHardwareConnect( port, btr );
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Master_NET :: CanOpenStatus  LSS_Master_NET :: canHardwareDisconnect(void)
{
  LSS_Master_NET::CanOpenStatus ret;
  ret = (LSS_Master_NET::CanOpenStatus)this->cpp_LSSMaster->canHardwareDisconnect();
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Master_NET :: CanOpenStatus  LSS_Master_NET :: switchModeGlobal(u8 mode)
{
  LSS_Master_NET::CanOpenStatus ret;
  ret = (LSS_Master_NET::CanOpenStatus)this->cpp_LSSMaster->switchModeGlobal(mode);
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Master_NET :: CanOpenStatus  LSS_Master_NET :: switchModeSelectiveVendorId(u32 vendorId)
{
  LSS_Master_NET::CanOpenStatus ret;
  ret = (LSS_Master_NET::CanOpenStatus)this->cpp_LSSMaster->switchModeSelectiveVendorId(vendorId);
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Master_NET :: CanOpenStatus  LSS_Master_NET :: switchModeSelectiveProductCode(u32 productCode)
{
  LSS_Master_NET::CanOpenStatus ret;
  ret = (LSS_Master_NET::CanOpenStatus)this->cpp_LSSMaster->switchModeSelectiveProductCode(productCode);
  return ret;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Master_NET :: CanOpenStatus  LSS_Master_NET :: switchModeSelectiveRevisionNumber(u32 revisionNumber)
{
  LSS_Master_NET::CanOpenStatus ret;
  ret = (LSS_Master_NET::CanOpenStatus)this->cpp_LSSMaster->switchModeSelectiveRevisionNumber(revisionNumber);
  return ret;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Master_NET :: CanOpenStatus  LSS_Master_NET :: switchModeSelectiveSerialNumber(u32 serialNumber)
{
  LSS_Master_NET::CanOpenStatus ret;
  ret = (LSS_Master_NET::CanOpenStatus)this->cpp_LSSMaster->switchModeSelectiveSerialNumber(serialNumber);
  return ret;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Master_NET :: CanOpenStatus  LSS_Master_NET :: configureNodeId(u8 nodeId,
                                                                   [System::Runtime::InteropServices::Out] u8 %errorCode,
                                                                   [System::Runtime::InteropServices::Out] u8 %specificErrorCode)
{
  LSS_Master_NET::CanOpenStatus ret;
  u8 cpp_error_code;
  u8 cpp_specific_error_code;
  ret = (LSS_Master_NET::CanOpenStatus)this->cpp_LSSMaster->configureNodeId(nodeId,
    &cpp_error_code,
    &cpp_specific_error_code);
  errorCode = cpp_error_code;
  specificErrorCode = cpp_specific_error_code;
  return ret;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Master_NET :: CanOpenStatus  LSS_Master_NET :: configureBitTimingParamteres(u8 tableSelector,
                                                                                u8 tableIndex,
                                                                                [System::Runtime::InteropServices::Out] u8 %errorCode,
                                                                                [System::Runtime::InteropServices::Out] u8 %specificErrorCode)
{
  LSS_Master_NET::CanOpenStatus ret;
  u8 cpp_error_code;
  u8 cpp_specific_error_code;
  ret = (LSS_Master_NET::CanOpenStatus)this->cpp_LSSMaster->configureBitTimingParamteres(tableSelector,
    tableIndex,
    &cpp_error_code,
    &cpp_specific_error_code);
  errorCode = cpp_error_code;
  specificErrorCode = cpp_specific_error_code;
  return ret;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Master_NET :: CanOpenStatus  LSS_Master_NET :: activateBitTimingParameters(u16 switchDelay)
{
  LSS_Master_NET::CanOpenStatus ret;
  ret = (LSS_Master_NET::CanOpenStatus)this->cpp_LSSMaster->activateBitTimingParameters(switchDelay);
  return ret;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Master_NET :: CanOpenStatus  LSS_Master_NET :: storeConfiguration([System::Runtime::InteropServices::Out] u8 %errorCode,
                                                                      [System::Runtime::InteropServices::Out] u8 %specificErrorCode)
{
  LSS_Master_NET::CanOpenStatus ret;
  u8 cpp_error_code;
  u8 cpp_specific_error_code;
  ret = (LSS_Master_NET::CanOpenStatus)this->cpp_LSSMaster->storeConfiguration(&cpp_error_code, &cpp_specific_error_code);
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

LSS_Slave_NET :: CanOpenStatus  LSS_Slave_NET :: canHardwareConnect(int port, int btr)
{
  LSS_Slave_NET::CanOpenStatus ret;
  ret = (LSS_Slave_NET::CanOpenStatus)this->cpp_LSSSlave->canHardwareConnect( port, btr );
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LSS_Slave_NET :: CanOpenStatus  LSS_Slave_NET :: canHardwareDisconnect(void)
{
  LSS_Slave_NET::CanOpenStatus ret;
  ret = (LSS_Slave_NET::CanOpenStatus)this->cpp_LSSSlave->canHardwareDisconnect();
  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void  LSS_Slave_NET :: setDeviceParameters(u32 vendorId, u32 productCode, u32 revisionNumber, u32 serialNumber)
{
  return this->cpp_LSSSlave->setDeviceParameters(vendorId, productCode, revisionNumber, serialNumber);
}
