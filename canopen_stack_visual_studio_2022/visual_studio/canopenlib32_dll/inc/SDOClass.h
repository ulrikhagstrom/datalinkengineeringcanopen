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

#ifndef SDO_CLASS_H
#define SDO_CLASS_H


#include "CanInterfaceClass.h"
#include "TimerClass.h"
#include "CanOpenErrorCodes.h"
#include "CanConnectionClass.h"

#define CALLBACK_PERIOD_TIME                  2000
#define ONGOING_TRANSFER_INACTIVITY_TIMEOUT   5000

//Client command specifiers.
#define CLI_CMD_INIT_DL_REQ     1  //9.2.2.2.2 Initiate SDO Download Protocol
#define CLI_CMD_DL_SEG_REQ      0  //9.2.2.2.3 Download SDO Segment Protocol
#define CLI_CMD_INIT_UL_REQ     2  //9.2.2.2.5 Initiate SDO Upload Protocol
#define CLI_CMD_UL_SEG_REQ      3  //9.2.2.2.6 Upload SDO Segment Protocol
#define CLI_CMD_BLOCK_DL_REQ    6  //9.2.2.2.9 Initiate SDO Block Download Protocol

//Server command specifiers.
#define SRV_CMD_INIT_DL_RESP    3  //9.2.2.2.2 Initiate SDO Download Protocol
#define SRV_CMD_DL_SEG_RESP     1  //9.2.2.2.3 Download SDO Segment Protocol
#define SRV_CMD_INIT_UL_RESP    2  //9.2.2.2.5 Initiate SDO Upload Protocol
#define SRV_CMD_UL_SEG_RESP     0  //9.2.2.2.6 Upload SDO Segment Protocol


class  SDO : public CanConnection
{
  public:
    CANOPENDLL_API SDO();
    CANOPENDLL_API ~SDO();
  protected:

    //
    //  CAN-bus connection.
    //
    canOpenStatus     writeToCanBus(void);
    canOpenStatus     readFromCanBus(void);
    canOpenStatus     getFrameData(unsigned long id, unsigned char *data, unsigned int dlc);

    //9.2.2.2.2 Initiate SDO Download Protocol 
    //9.2.2.2.5 Initiate SDO Upload Protocol
    //9.2.2.2.7 Abort SDO Transfer Protocol
    canOpenStatus     setMultiplexor(u16 object_index, u8 sub_index);
    canOpenStatus     getMultiplexor(u16 *object_index, u8 *sub_index);
   
    //9.2.2.2.2 Initiate SDO Download Protocol
    //9.2.2.2.5 Initiate SDO Upload Protocol
    //9.2.2.2.7 Abort SDO Transfer Protocol
    canOpenStatus     setData(u32 val, u8 valid, u8 *n);
    canOpenStatus     getData(u32 *val, u8 *valid, u8 n);

    //9.2.2.2.3 Download SDO Segment Protocol
    //9.2.2.2.6 Upload SDO Segment Protocol
    canOpenStatus     setSegData(u8 *buffer, u8 valid);
    canOpenStatus     getSegData(u8 *buffer, u8 *valid, u8 n);
    canOpenStatus     setSegData(u8 valid);

    //9.2.2.2.10 Download SDO Block Segment Protocol
    canOpenStatus     setBlkSegData(void);
    canOpenStatus     setBlkSegData(u8 bytes_in_segment, u8 c);
    canOpenStatus     getDlBlkSegHeader(u8 *c, u8 *seqNo); // 9.2.2.2.10 Download SDO Block Segment Protocol 


    //9.2.2.2.7 Abort SDO Transfer Protocol (Both Client and Server SDO)
    canOpenStatus     setCliSrvCmdSpcAbrtTrnsf(void);
    canOpenStatus     getCliSrvCmdSpcAbrtTrnsf(void);
    canOpenStatus     setCliSrvAbortTransfer(u16 object_index, u8 sub_index, u32 error_code);
    canOpenStatus     getCliSrvAbortTranfer(u16 *object_indexP, u8 *sub_indexP, u32 *error_code);
    canOpenStatus     hndlRemoteAbort(void);
    canOpenStatus     hndlInvalidCmdSpec(void);

    canOpenStatus     registerPeriodicTimerCallback(TimeClass::TimeHandlerFuncPtr callback_function_pointer, void *context, unsigned long period_time);
    canOpenStatus     unregisterPeriodicTimerCallback( void );

    // misc
    canOpenStatus     clrTxCanDataField(u8 from, u8 to);
    canOpenStatus     setLatestEventTimestamp(void); // qqq semaphore protection?
    unsigned long     getLatestEventTimestamp(void); // -- "" --

    // Gets the rest of the application buffer.
    u32               getRestAppBuffer(void);
    canOpenStatus     incrementApplicationBuffertOffset(u16 bytes);
    canOpenStatus     decrementapplication_buffertr(u16 bytes);

    // Transfers are being syncronized with a toggle.
    void              toggle(void);

    COBID             cobid_tx;  // COBID for TX:ing SDO.
    u8                can_data_tx[8]; /* The can frame data for TX. */

    COBID             cobid_rx;  // COBID for RX-SDO.
    u8                can_data_rx[8]; /* The can frame data for RX. */

    u32               remote_node_error_code;   // Error code received from remote node.
    u8                node_id;       // Node id
    u8                t; // Toggle flag.
    unsigned long     event_time_stamp;
    canOpenStatus     transfer_result;

    u8                *application_buffer;
    u32               application_buffer_offset;
    u32               application_buffer_length;
    u16               application_object_index;
    u8                application_sub_index;
    u32               *application_valid_bytes;
    CanOpenErrorCode  *application_canopen_error_code;

    u8                block_transfer_segment_count;

    int               periodic_timer_index;
    u8                valid_bytes_in_last_segment_in_last_block;
    HANDLE            rx_tx_mutex;

private:
};

#endif
