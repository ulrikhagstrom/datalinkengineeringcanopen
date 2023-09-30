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

#ifndef CANLIB32_CANUSB_PORTING_H
#define CANLIB32_CANUSB_PORTING_H

#define RX_QUEUE_SIZE 100

typedef struct {
  long id;
  unsigned char data[8];
  unsigned int dlc;
  unsigned int flags;
} CanMessageStruct;


typedef struct {
  bool in_use;
  int  com_port;
  HANDLE com_port_handle;
  int opts;

  OVERLAPPED os_write_port; // Used in comPuts()
  OVERLAPPED os_read_port;  // Used in rxThread()

  HANDLE rx_thread_handle;
  DWORD can_usb_rx_thread_id;

  bool can_usb_rx_thread_running;
  bool can_bus_on;
  bool echo_enabled;

  HANDLE command_result_event;
  bool command_result; // 
  HANDLE version_result_event;
  char version_result[5];
  HANDLE serial_number_event;
  char serial_number[5];
  HANDLE status_flags_event;
  char status_flags[5];
  HANDLE tx_message_result_event;
  char tx_message_result[1];

  CanMessageStruct rx_queue[RX_QUEUE_SIZE];
  int rx_queue_put_pos;
  int rx_queue_get_pos;

  CRITICAL_SECTION rx_queue_cs;
  CRITICAL_SECTION tx_message_queue_cs;

} CanPortDataStruct;


#endif