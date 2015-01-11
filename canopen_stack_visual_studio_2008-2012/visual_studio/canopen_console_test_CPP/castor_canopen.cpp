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

#include <stdio.h>
#include <conio.h>

#include "canopenlib.h"
#include "CANopenLibHelper.h"

#define FULL_TEST  1

canOpenStatus  objectRead(void *context, u16 object_index, u8 sub_index, u8 *buf, u32 *valid, u32 buffer_size, u32 *coErrorCode)
{
  printf("\n+-----------------------------------------------+");
  printf("\n| objectRead callback is called (SERVER)|");
  printf("\n+-----------------------------------------------+\n\n");

  if (object_index == 0x5678)
  {

    for (int i = 0; i < buffer_size; i++)
    {
      buf[i] = i;
	    *valid = i;
    }
    *valid = 70000;
  }
  else if (object_index == 10)
  {
    *valid = 4;
    buf[0] = 0x10;
    buf[1] = 0x20;
    buf[2] = 0x30;
    buf[3] = 0x40;
    *coErrorCode = 0;
    
  }
  return CANOPEN_OK;
}

canOpenStatus objectWrite(void *context, u16 object_index, u8 sub_index, u8 *buf, u32 valid, u32 *coErrorCode)
{
  u16 i;

  printf("\n+------------------------------------------------+");
  printf("\n|  objectWrite callback is called (SERVER) |");
  printf("\n+------------------------------------------------+\n\n");

  if (object_index == 0x667)
  {
    *coErrorCode = 0x2332;
    return CANOPEN_ERR_DEFINED_BY_APPL;
  }

  printf("\n\nOBJECT INDEX: 0x%04x, SUBINDEX: 0x%02x:\n\n", object_index, sub_index);
  
  for (i=0; i < valid; i++)
  {
    if (object_index == 0x666)
    {
      printf("[0x%02x],", buf[i]);
    }
    else
    {
      printf("[0x%02x], [%c]", buf[i], buf[i]);
    }
  }
  return CANOPEN_OK;
}

canOpenStatus getAttributesObject(void *context, u16 object_index, u8 sub_index, u16 *flags)
{
  printf("\n+--------------------------------------------------+");
  printf("\n|  getAttributesObject callback is called (SERVER) |");
  printf("\n+--------------------------------------------------+\n\n");
  *flags = (OBJ_WRITEABLE | OBJ_READABLE);
  return CANOPEN_OK;
}

void nodeOperationalState( void *context, u8 node_id, u8 state )
{
  printf("\nNode %d is in state %x ", node_id, state);


}

void localNodeOperationalState( void *context, u8 state )
{
  printf("\nLocal state is : %x ", state);
}

u8 latest_received_can_frame[8];

void canSpy( void *context, u32 id, u8 *data, u8 dlc, u32 flags )
{
  printf("\n Id: 0x%3x, dlc: %d, flags: %d data: ", id, dlc, flags);
  if ( flags & CAN_MSG_RTR )
  {
    printf(" No data, is RTR frame");
  }
  else
  {
    for (int i = 0; i < dlc; i++)
    {
      printf(" %x ", ((u8*)data)[i]);
    }
  }
  memcpy( latest_received_can_frame, data, dlc);
  printf("\n");
}

void emcy_callback( void *context, u8 nodeId, u16 emcyErrorCode, u8 errorRegister, u8 *manufacturerSpecificErrorField)
{
  printf("\nGot emergency messages!");
  printf("nodeId: 0x%x", nodeId);
  printf("emcyErrorCode: 0x%x", emcyErrorCode);
  printf("nodeId: 0x%x", errorRegister);
  printf("nodeId: %d", nodeId);

}
//( void *context, u32 cobid, u8* data, u8 len);
void rpdo_callback(void *context, u32 cobid, u8* data, u8 len)
{
  printf("RPDO received!");
}


int main (void) {
 //canStatus res;
  bool isOK = false;
  ClientSDO *clientSDO;
  ServerSDO serverSDO;
  NMTMaster *nmtMaster;
  NMTSlave *nmtSlave;
  CanMonitor *can_monitor;
  CanMonitor *can_monitor2;
  EmcyServer *emcy_server;
  EmcyClient *emcy_client;

  u8 txbufferBuffer[128000];  // Buffer to be used for recieiving 
  u8 receiveBuffer[128000];  // Buffer to be used for recieiving 
  u32 receiveBufferValid = 0;
  CanOpenErrorCode coErr;
  canOpenStatus ret;
  CanOpenErrorCode canOpenErr;
  const char stringOne[] = "123456789abcd";
  const char stringTwo[] = "blck";
  char stringThree[500];

  for (int i = 0; i < 500; i++)
  {
    stringThree[i] = (char)i;
  }

  printf("\n*********************************************************");
  printf("\n* THIS TEST IS VERY GOOD TO EXECUTE USING KVASERS       *");
  printf("\n* VITUAL CAN CHANNELS AND HAVE NO HARDWARE CONNECTED    *");
  printf("\n* BUT SHOULD ALSO WORK USING TWO OF YOUR CAN INTERFACES *");
  printf("\n* CONNECTED TO THE SAME CANBUS (FOR EXAMPLE 2 CANUSB ON *");
  printf("\n* THE SAME CAN-BUS RUNNING ON 500kbit!)                 *");
  printf("\n*********************************************************");

  //getch();


  setAdapter(canAdapter_DEFAULT);

  LSSMaster *lss_master = new LSSMaster();

  if (lss_master->canHardwareConnect(0, 500000) != CANOPEN_OK)
  {
	  return 0;
  }

  LSSSlave *lss_slave = new LSSSlave();

  if (lss_slave->canHardwareConnect(1, 500000) != CANOPEN_OK)
  {
	  return 0;
  }

  lss_slave->setDeviceParameters(0x1234, 0x2345, 0x3456, 0x4567);

  if (lss_master->switchModeGlobal(0) != CANOPEN_OK)
  {
	  return 0;
  }

  if (lss_master->switchModeSelectiveVendorId(0x1234) != CANOPEN_OK)
  {
	  return 0;
  }


  if (lss_master->switchModeSelectiveProductCode(0x2345) != CANOPEN_OK)
  {
	  return 0;
  }


  if (lss_master->switchModeSelectiveRevisionNumber(0x3456) != CANOPEN_OK)
  {
	  return 0;
  }


  if (lss_master->switchModeSelectiveSerialNumber(0x4567) != CANOPEN_OK)
  {
	  return 0;
  }

  u8 errorCode = 0xff;
  u8 specError = 0xff;

  if (lss_master->configureNodeId(5, &errorCode, &specError) != CANOPEN_OK)
  {
	  return 0;
  }




  Sleep(500);

  Sleep(2000);

  Sleep(10000);





#if 0


  EDS_DCF *eds;

  eds = new EDS_DCF();


  u16 res;

  ret = eds->fileRegister("\\Eds\\BK5120.eds");
  if (ret == CANOPEN_OK) {
    printf("\nEds file OK!");
  } else {
    printf("\nEds file failure!");
  }


  u16 mandatory_objects;
  ret = eds->mandatoryObjectsReadAmount( &mandatory_objects );
  if (ret == CANOPEN_OK) {
    printf("\nFound %d Mandatory Objects in EDS", mandatory_objects);
  } else {
    printf("\nMandatory objects read failure!");
  }

  u16 object_index;
  for (int i=1; i<=mandatory_objects; i++)
  {
    ret = eds->mandatoryObjectReadAtIndex( i, &object_index);
    if (ret == CANOPEN_OK) {
      printf("\nMandatory object %d is %x!", i, object_index);
    } else {
      printf("\nMandatory objects read failure!");
    }
  }

  u16 optional_objects;
  ret = eds->optionalObjectsReadAmount( &optional_objects );
  if (ret == CANOPEN_OK) {
    printf("\nFound %d Mandatory Objects in EDS", optional_objects);
  } else {
    printf("\nMandatory objects read failure!");
  }


  for (int i=1; i<=optional_objects; i++)
  {
    ret = eds->optionalObjectReadAtIndex( i, &object_index);
    if (ret == CANOPEN_OK) {
      printf("\nOptional object %d is %x!", i, object_index);
    } else {
      printf("\nOptional objects read failure!");
    }
  }

  ObjectData obj_data;

  ret = eds->fileDataRead( 0x1005, &obj_data );
  if (ret == CANOPEN_OK) {
    printf("\nFound %d Mandatory Objects in EDS", optional_objects);
  } else {
    printf("\nMandatory objects read failure!");
  }

  ret = eds->fileDataRead( 0x1018, 0x0, &obj_data );
  if (ret == CANOPEN_OK) {
    printf("\nFound %d Mandatory Objects in EDS", optional_objects);
  } else {
    printf("\nMandatory objects read failure!");
  }

  ret = eds->canHardwareConnect( 0, 500000 );
  if ( ret != CANOPEN_OK ) {
    exit(0);
  }

  ret = eds->connect( 3 );
  if ( ret != CANOPEN_OK ) {
    exit(0);
  }


  ret = eds->fileDataRead( 0x1018, 0x1, &obj_data );
  if (ret == CANOPEN_OK) {
    printf("\nFound %d Mandatory Objects in EDS", optional_objects);
  } else {
    printf("\nMandatory objects read failure!");
  }

  ret = eds->nodeDataRead( 0x1018, 0x1, &obj_data );
  if ( ret != CANOPEN_OK ) {
    exit(0);
  }

  for (int i=1; i<=mandatory_objects; i++)
  {
    ret = eds->mandatoryObjectReadAtIndex( i, &object_index );
    if ( ret != CANOPEN_OK ) {
      exit(0);
    }
    ret = eds->fileDataRead( object_index, &obj_data );
    if (ret == CANOPEN_OK) {
      if ( obj_data.data_type > 8 )
        printf("LONG!");
      if ( obj_data.sub_number_valid )
      {
        for ( int i = 0; i < obj_data.sub_number; i++ )
        {
          ret = eds->nodeDataRead( object_index, i, &obj_data );
          if ( ret != CANOPEN_OK ) {
            printf("WARNING!");
          } else {
            if ( obj_data.actual_value_valid )
            {
              printf("%08x\n", obj_data.actual_value);
            }
          }
        }
      } else {
        ret = eds->nodeDataRead( object_index, 0, &obj_data );
        if ( ret != CANOPEN_OK ) {
          exit(0);
        }
        if ( obj_data.actual_value_valid )
        {
          printf("%08x\n", obj_data.actual_value);
        }

      }
    }
  }

  for (int i=1; i<=optional_objects; i++)
  {
    ret = eds->optionalObjectReadAtIndex( i, &object_index );
    if ( ret != CANOPEN_OK ) {
      exit(0);
    }
    ret = eds->fileDataRead( object_index, &obj_data );
    if (ret == CANOPEN_OK) {
      if ( obj_data.data_type > 8 )
        printf("LONG!");
      if ( obj_data.sub_number_valid )
      {
        for ( int i = 0; i < obj_data.sub_number; i++ )
        {
          ret = eds->nodeDataRead( object_index, i, &obj_data );
          if ( ret != CANOPEN_OK ) {
            printf("WARNING!");
          }
          if ( obj_data.actual_value_valid )
          {
            printf("%08x\n", obj_data.actual_value);
          }
        }
      } else {
        ret = eds->nodeDataRead( object_index, 0, &obj_data );
        if ( ret != CANOPEN_OK ) {
          exit(0);
        }
        if ( obj_data.actual_value_valid )
        {
          printf("%08x\n", obj_data.actual_value);
        }
      }
    } else {
      exit(0);
    }
  }

  for (int i=1; i<=optional_objects; i++)
  {
    ret = eds->optionalObjectReadAtIndex( i, &object_index );
    if ( ret != CANOPEN_OK ) {
      exit(0);
    }
    ret = eds->fileDataRead( object_index, &obj_data );
    if (ret == CANOPEN_OK) {
      if ( obj_data.data_type > 8 )
        printf("LONG!");
      if ( obj_data.sub_number_valid )
      {
        for ( int i = 0; i < obj_data.sub_number; i++ )
        {
          ret = eds->nodeDataWriteDefault( object_index, i, &obj_data );
          if ( ret != CANOPEN_OK ) {
            printf("WARNING!");
          }
          if ( obj_data.actual_value_valid )
          {
            printf("%08x\n", obj_data.actual_value);
          }
        }
      } else {
        ret = eds->nodeDataWriteDefault( object_index, 0, &obj_data );
        if ( ret != CANOPEN_OK ) {
          printf("WRITE FAILED");
        }
        if ( obj_data.actual_value_valid )
        {
          printf("%08x\n", obj_data.default_value);
        }
      }
    } else {
      exit(0);
    }
  }

#endif

  emcy_server = new EmcyServer();
  if ( emcy_server->canHardwareConnect( 0, 500000) != CANOPEN_OK )
  {
      return 0;
  }

  if (emcy_server->registerEmergencyMessageCallBack(NULL, emcy_callback) != CANOPEN_OK)
  {
      return 0;
  }

  emcy_client = new EmcyClient();
  if ( emcy_client->canHardwareConnect( 1, 500000) != CANOPEN_OK )
  {
      return 0;
  }

  if (emcy_client->sendEmcyMessage(2, 0x1234, 0x56, NULL) != CANOPEN_OK)
  {
      return 0;
  }



  if (emcy_client->canHardwareDisconnect() != CANOPEN_OK)
  {
      return 0;
  }

  if (emcy_server->canHardwareDisconnect() != CANOPEN_OK)
  {
      return 0;
  }

  Sleep(500);

  TransmitPDO* tpdo = new TransmitPDO();
  ReceivePDO *rpdo = new ReceivePDO();

  if ( tpdo->canHardwareConnect(0, 500000) != CANOPEN_OK )
  {
      return 0;
  }

  if ( rpdo->canHardwareConnect(1, 500000) != CANOPEN_OK )
  {
      return 0;
  }

  if ( tpdo->setData(0x123, (u8*)"12345678", 8) != CANOPEN_OK )
  {
      return 0;
  }


  if ( tpdo->transmitPdo() != CANOPEN_OK )
  {
      return 0;
  }

  if ( tpdo->periodicTransmission(true) != CANOPEN_OK )
  {
      return 0;
  }

  Sleep(5000);

  if ( tpdo->periodicTransmission(false) != CANOPEN_OK )
  {
      return 0;
  }

  Sleep(1000);

  if ( rpdo->setCobid(0x123) != CANOPEN_OK )
  {
    return 0;
  }


  if ( rpdo->registerReceivePdoMessageCallBack(rpdo, rpdo_callback) != CANOPEN_OK )
  {
    return 0;
  }

  if ( tpdo->periodicTransmission(true) != CANOPEN_OK )
  {
      return 0;
  }

  Sleep(10000);

  if (rpdo->canHardwareDisconnect() != CANOPEN_OK)
  {
      return 0;
  }

  if (tpdo->canHardwareDisconnect() != CANOPEN_OK)
  {
      return 0;
  }

  //canOpenStatus unlock_res = CanInterface :: unlockCanopenLibrary( "C:\\dev\\closed_rep\\trunk\\canopen_stack_and_tools\\visual_studio\\canopenDLL\\Debug\\canopen_lic.txt", "Ulrik" );

  can_monitor = new CanMonitor();
  can_monitor2 = new CanMonitor();
  
  if ( can_monitor->canHardwareConnect( 0, 500000) != CANOPEN_OK )
  {
      return 0;
  }


  if ( can_monitor->canHardwareDisconnect() != CANOPEN_OK )
  {
    return 0;
  }

  if ( can_monitor->canHardwareConnect( 0, 500000) != CANOPEN_OK )
  {
      return 0;
  }


  if ( can_monitor->registerCanReceiveCallback( NULL, canSpy ) != CANOPEN_OK)
  {
      return 0;
  }

  if ( can_monitor2->canHardwareConnect( 1, 500000) != CANOPEN_OK )
  {
      return 0;
  }

  can_monitor2->canWrite(0x10, (u8*)"monkey", sizeof("monkey"), 0);

  Sleep(100);

  if (strcmp( (char*)latest_received_can_frame, "monkey" ) != 0)
  {
    return 0;
  }



  printf("\n*************************************");
  printf("\n* Configuring Client and Server SDO *");
  printf("\n*************************************");


  nmtMaster = new NMTMaster();
  nmtSlave = new NMTSlave();
  
  int i = 10;

  while (i)
  {

    printf("a");
    if (nmtMaster->canHardwareConnect(1, 500000) != CANOPEN_OK)
    {
      return 0;
    }

    printf("b");
    delete nmtMaster;
    printf("b2");
    delete nmtSlave;
    printf("c");
    nmtMaster = new NMTMaster();
    printf("c2");
    nmtSlave = new NMTSlave();

    printf("d");
    if (nmtMaster->canHardwareConnect(1, 500000) != CANOPEN_OK)
    {
      return 0;
    }
    printf("e");
    if (nmtMaster->canHardwareDisconnect() != CANOPEN_OK)
    {
      return 0;
    }
    printf("f");
    i--;
  }
  
  if (nmtMaster->canHardwareConnect(1, 500000) != CANOPEN_OK)
  {
    return 0;
  }
  

  if (nmtSlave->canHardwareConnect(0, 500000) != CANOPEN_OK)
  {
    return 0;
  }

  if (nmtSlave->canHardwareDisconnect() != CANOPEN_OK)
  {
    return 0;
  }

  if (nmtSlave->canHardwareConnect(0, 500000) != CANOPEN_OK)
  {
    return 0;
  }


  if (nmtSlave->nodeSetId(2) != CANOPEN_OK)
  {
    return 0;
  }

  if (nmtSlave->registerLocalNodeStateChangeCallback(localNodeOperationalState, &nmtSlave) != CANOPEN_OK)
  {
    return 0;
  }


  if (nmtMaster->registerRemoteNodeStateCallback((NMTOperationalStateFunPtr)nodeOperationalState, NULL ) != CANOPEN_OK)
  {
    return 0;
  }

  //if (nmtMaster->nodeGuardPollStart(2, 3000) != CANOPEN_OK)
  //{
  //  return 0;
  //}


  //getch();


  //if (nmtMaster->nodeStart( 2 ) != CANOPEN_OK)
  //{
  //  return 0;
  //}

  //getch();

  clientSDO = new ClientSDO();
  delete clientSDO;

  clientSDO = new ClientSDO();
 
  if (clientSDO->canHardwareConnect(0, 500000) != CANOPEN_OK)
  {
    return 0;
  }

  delete clientSDO;


  clientSDO = new ClientSDO();
 
  if (clientSDO->canHardwareConnect(0, 500000) != CANOPEN_OK)
  {
    return 0;
  }


  if (clientSDO->canHardwareDisconnect() != CANOPEN_OK)
  {
    return 0;
  }


  if (clientSDO->connect(3) != CANOPEN_OK)
  {
    return 0;
  }

  if (serverSDO.canHardwareConnect(1, 500000) != CANOPEN_OK)
  {
    return 0;
  }

  if (serverSDO.nodeSetId(3) != CANOPEN_OK)
  {
    return 0;
  }

  printf("\n\nDONE!\n");

  printf("\n*******************************");
  printf("\n* Register callbacks in DLL!  *");
  printf("\n*******************************");


  if (serverSDO.registerObjectReadCallback(objectRead, NULL) != CANOPEN_OK)
  {

    return 0;
  };

  if (serverSDO.registerObjectWriteCallback(objectWrite, NULL) != CANOPEN_OK)
  {
    return 0;
  };

  if (serverSDO.registerObjectGetAttributesCallback(getAttributesObject, NULL) != CANOPEN_OK)
  {
    return 0;
  };

  printf("\n\nDONE!\n");



  // BECKHOFF 5120 demo.

/*
  if ((ret = clientSDO.objectRead( 0x100a, 0x0, receiveBuffer, sizeof(receiveBuffer), &receiveBufferValid, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
	  getch();
	  return 0;
  }
  else
  {
	  int i = 0;
	  while (i < receiveBufferValid)
	  {
	    printf("[%02x] [%c]\n", receiveBuffer[i], receiveBuffer[i]);
	    i++;
	  }
  }


  if ((ret = clientSDO.objectRead( 0x1008, 0x0, receiveBuffer, sizeof(receiveBuffer), &receiveBufferValid, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
	  getch();
	  return 0;
  }
  else
  {
	  int i = 0;
	  while (i < receiveBufferValid)
	  {
	    printf("[%02x] [%c]\n", receiveBuffer[i], receiveBuffer[i]);
	    i++;
	  }
  }

  if ((ret = clientSDO.objectRead( 0x1009, 0x0, receiveBuffer, sizeof(receiveBuffer), &receiveBufferValid, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
	  getch();
	  return 0;
  }
  else
  {
	  int i = 0;
	  while (i < receiveBufferValid)
	  {
	    printf("[%02x] [%c]\n", receiveBuffer[i], receiveBuffer[i]);
	    i++;
	  }
  }

  printf("Read DeviceType\n");
  if ((ret = clientSDO.objectRead( 0x1000, 0x0, receiveBuffer, sizeof(receiveBuffer), &receiveBufferValid, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
	  getch();
	  return 0;
  }
  else
  {
	  int i = 0;
	  while (i < receiveBufferValid)
	  {
	    printf("[%02x] [%c]\n", receiveBuffer[i], receiveBuffer[i]);
	    i++;
	  }
  }

  u32 deviceType;
  if ((ret = clientSDO.objectRead( 0x1000, 0x0, &deviceType, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
	  getch();
	  return 0;
  }
  else
  {
    printf("\nDeviceType: 0x%08x", deviceType);
  }

  u8 errorRegister;
  if ((ret = clientSDO.objectRead( 0x1001, 0x0, &errorRegister, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
	  getch();
	  return 0;
  }
  else
  {
    printf("\nErrorRegister: 0x%02x", errorRegister);
  }


  u32 numberPDOs;
  if ((ret = clientSDO.objectRead( 0x1004, 0x0, &numberPDOs, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
  }
  else
  {
    printf("\numberPDOs: 0x%02x", numberPDOs);
  }

  u32 syncId;
  if ((ret = clientSDO.objectRead( 0x1005, 0x0, &syncId, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
	  getch();
	  return 0;
  }
  else
  {
    printf("\nsyncId: 0x%08x", syncId);
  }

  u16 guardTime;
  if ((ret = clientSDO.objectRead( 0x100c, 0x0, &guardTime, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
	  getch();
	  return 0;
  }
  else
  {
    printf("\nguardTime: 0x%04x", guardTime);
  }

  if ((ret = clientSDO.objectWrite(0x100c, 0x0, (u16)1000, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
	  getch();
	  return 0;
  }
  else
  {
    printf("\nNew guardTime value written successfully!");
  }

  if ((ret = clientSDO.objectRead( 0x100c, 0x0, &guardTime, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
	  getch();
	  return 0;
  }
  else
  {
    printf("\nguardTime: 0x%04x", guardTime);
  }

  if ((ret = clientSDO.objectWrite(0x100d, 0x0, (u8)3, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
	  getch();
	  return 0;
  }
  else
  {
    printf("\nLife time factor value written successfully!");
  }


  getch();
  return 0;


*/

  //return 0;


   
  printf("\n***************************************************");
  printf("\n* Press any key to do BLOCK TRANSFER (test 1/5)!  *");
  printf("\n***************************************************");

  getch();

  if (clientSDO->canHardwareConnect(0, 500000) != CANOPEN_OK)
  {
    return 0;
  }

  for (int i=0; i<sizeof(txbufferBuffer); i++)
  {
    txbufferBuffer[i] = (i & 0xf) + 100;
  }


  if ((ret = clientSDO->objectWriteBlock(0x666, 0x5, 0, (unsigned char*)txbufferBuffer, 5000, &canOpenErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectWriteBlock failed, %d", ret);  
	getch(); 
	return 0;
  }  

  if ((ret = clientSDO->objectWriteBlock(0x1234, 0x5, 0, (unsigned char*)stringOne, sizeof(stringOne), &canOpenErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectWriteBlock failed, %d", ret);  
	getch();
	return 0;
  }




  printf("\n\nDONE!\n");

  printf("\n***************************************************");
  printf("\n* Press any key to do BLOCK TRANSFER (test 2/5)!  *");
  printf("\n***************************************************");

  getch();

  if ((ret = clientSDO->objectWriteBlock(0x1234, 0x5, 0, (unsigned char*)stringTwo, sizeof(stringTwo), &canOpenErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectWriteBlock failed, %d", ret);  
	getch();
	return 0;
  }

  printf("\n\nDONE!\n");

  printf("\n***************************************************");
  printf("\n* Press any key to do BLOCK TRANSFER (test 3/5)!  *");
  printf("\n***************************************************");

  getch();

  if ((ret = clientSDO->objectWriteBlock(0x1234, 0x5, 0x0, (unsigned char*)stringOne, sizeof(stringOne), &canOpenErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectWriteBlock failed, %d", ret);  
	getch();
	return 0;
  }

  printf("\n\nDONE!\n");

  printf("\n***************************************************");
  printf("\n* Press any key to do BLOCK TRANSFER (test 4/5)!  *");
  printf("\n***************************************************");

  getch();

  if ((ret = clientSDO->objectWriteBlock(0x666, 0x5, 0, (unsigned char*)stringThree, sizeof(stringThree), &canOpenErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectWriteBlock failed, %d", ret);  
	getch(); 
	return 0;
  }

  printf("\n\nDONE!\n");

  printf("\n***************************************************");
  printf("\n* Press any key to do BLOCK TRANSFER (test 4/5)!  *");
  printf("\n***************************************************");

  getch();

  for (int i=0; i<sizeof(txbufferBuffer); i++)
  {
    txbufferBuffer[i] = (i & 0xf) + 100;
  }


  if ((ret = clientSDO->objectWriteBlock(0x666, 0x5, 0, (unsigned char*)txbufferBuffer, sizeof(txbufferBuffer), &canOpenErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectWriteBlock failed, %d", ret);  
	getch(); 
	return 0;
  }

  printf("\n\nDONE!\n");

  printf("\n***************************************************");
  printf("\n* Press any key to do BLOCK TRANSFER (test 5/5)!  *");
  printf("\n***************************************************");

  getch();

  for (int i=0; i<sizeof(txbufferBuffer); i++)
  {
    txbufferBuffer[i] = (i & 0xf) + 100;
  }

  if ((ret = clientSDO->objectWriteBlock(0x667, 0x5, 0, (unsigned char*)"Should not work", sizeof("Should not work"), &canOpenErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectWriteBlock failed as expected!!, %d", ret);
	if (ret == CANOPEN_REMOTE_NODE_ABORT)
	{
	  printf("\nreceived canopen error code : 0x%4x, that is OK!", canOpenErr);
	}
	printf ("\n\nTest finished OK!!");
  }
  else
  {
	printf("\nDemo failed, it should have been a remote transfer abort here...");
	getch();
	return 0;
  }

  printf("\n\nDONE!\n");

#if FULL_TEST  
  
  printf("\n****************************************************");
  printf("\n* Press any key to do EXPEDITED WRITE (test 1/1)!  *");
  printf("\n****************************************************");

  getch();


  if ((ret = clientSDO->objectWrite(0x1234, 0x5, (unsigned char*)"abc", sizeof("abc"), &canOpenErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectWrite failed, %d", ret);  
	getch();
	return 0;
  }

  printf("\n\nDONE!\n");

  printf("\n****************************************************");
  printf("\n* Press any key to do SEGMENTED WRITE (test 1/3)!  *");
  printf("\n****************************************************");

  getch();

  if ((ret = clientSDO->objectWrite(0x1235, 0x5, (unsigned char*)"Segmented transfer protocol", sizeof("Segmented transfer protocol"), &canOpenErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectWrite failed, %d", ret);  
	getch();
	return 0;
  }

  printf("\n\nDONE!\n");

  printf("\n****************************************************");
  printf("\n* Press any key to do SEGMENTED WRITE (test 2/3)!  *");
  printf("\n****************************************************");

  getch();


  if ((ret = clientSDO->objectWrite(0x1235, 0x5, (unsigned char*)"THIS IS ALSO SEGMENTED TRANSFER PROTOCOL IN ACTION!!!", sizeof("THIS IS ALSO SEGMENTED TRANSFER PROTOCOL IN ACTION!!!"), &canOpenErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectWrite failed, %d", ret);  
	getch();
	return 0;
  }

  printf("\n\nDONE!\n");

  printf("\n****************************************************");
  printf("\n* Press any key to do SEGMENTED WRITE (test 3/3)!  *");
  printf("\n****************************************************");

  getch();

  if ((ret = clientSDO->objectWrite(0x1235, 0x5, (unsigned char*)"Let's try segmented transfer once more...", sizeof("Let's try segmented transfer once more..."), &canOpenErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectWrite failed, %d", ret);  
	getch();
	return 0;
  }

  printf("\n\nDONE!\n");

  printf("\n****************************************************");
  printf("\n* Press any key to do SEGMENTED WRITE (test 3/4)!  *");
  printf("\n****************************************************");

  getch();

  for (int i=0; i<sizeof(txbufferBuffer); i++)
  {
    txbufferBuffer[i] = (i & 0xf) + 100;
  }


  if ((ret = clientSDO->objectWrite(0x1235, 0x5, txbufferBuffer, 1000, &canOpenErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectWrite failed, %d", ret);  
	getch();
	return 0;
  }



#endif

  printf("\n\nDONE!\n");

  printf("\n****************************************************");
  printf("\n* Press any key to do SEGMENTED READ (test 1/1)!  *");
  printf("\n****************************************************");

  getch();


  if ((ret = clientSDO->objectRead( 0x5678, 0x5, receiveBuffer, 128000, &receiveBufferValid, &coErr)) != CANOPEN_OK)
  {
	printf("\nclientSDO.objectRead failed, returned %d", ret);
	if (ret == CANOPEN_REMOTE_NODE_ABORT)
	{
	  printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	}
	getch();
	return 0;
  }
  else
  {
	int i = 0;
	while (i < receiveBufferValid)
	{
	  printf("[%02x] ", receiveBuffer[i]);
	  i++;
	}
  }

  printf("\n****************************************************");
  printf("\n* Press any key to do EXPEITED READ (test 1/1)!    *");
  printf("\n****************************************************");

  getch();


  if ((ret = clientSDO->objectRead( 10, 10, receiveBuffer, 4, &receiveBufferValid, &coErr)) != CANOPEN_OK)
  {
	  printf("\nclientSDO.objectRead failed, returned %d", ret);
	  if (ret == CANOPEN_REMOTE_NODE_ABORT)
	  {
	    printf("\nRemote node sent abort transfer error code: 0x%08x", coErr);
	  }
	  getch();
	  return 0;
  }
  else
  {
	  int i = 0;
	  while (i < receiveBufferValid)
	  {
	    printf("[%02x] ", receiveBuffer[i]);
	    i++;
	  }
  }

  printf("\n\nDONE! Press any key to quit!\n");

  getch();
   
  return 0;

}