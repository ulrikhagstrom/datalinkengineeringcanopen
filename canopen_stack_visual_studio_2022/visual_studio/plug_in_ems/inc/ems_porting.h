/*
       h t t p : / / 
        _____          _   _                                      
       / ____|   /\   | \ | |                                     
      | |       /  \  |  \| | ___  _ __   ___ _ __    _ __  _   _ 
      | |      / /\ \ | . ` |/ _ \| '_ \ / _ \ '_ \  | '_ \| | | |
      | |____ / ____ \| |\  | (_) | |_) |  __/ | | |_| | | | |_| |
       \_____/_/    \_\_| \_|\___/| .__/ \___|_| |_(_)_| |_|\__,_|
                                  | |                             
                                  |_|                              

                      CANopen API (C++/C#) by CANopen.nu.
            Copyright (C) 2009 Ulrik Hagstrom (ulrik@canopen.nu)

  CANopen API (C++/C#) is available under a dual license: 
  
  1. CANopen API (C++/C#) can be used under the GNU General Public License, 
  version 3.0. This license allows for free use, but also requires that your 
  own code using CANopen API (C++/C#) be released under the same license. 
  For details, please read version 3.0 of the GPL (see gpl-3.0.txt).

  2. CANopen API (C++/C#) can be used under the CANopen API (C++/C#) 
  Commercial License. This license allows for the use of CANopen API (C++/C#) 
  in closed-source projects. This license is available for a very reasonable 
  fee and also includes technical support. For details, please read the 
  CANopen API (C++/C#) Commercial License (see license.txt).

*/

#ifndef CANLIB32_EMS_PORTING_H
#define CANLIB32_EMS_PORTING_H

#define RX_QUEUE_SIZE 100

typedef struct {
  long id;
  unsigned char data[8];
  unsigned int dlc;
  unsigned int flags;
} CanMessageStruct;


typedef struct {
  bool in_use;
  int can_port_index;
  int device_handle;
  int opts;
  

  HANDLE rx_thread_handle;
  DWORD can_usb_rx_thread_id;

  bool can_usb_rx_thread_running;
  bool can_bus_on;
  bool echo_enabled;

/*
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
*/
  HANDLE rx_event;

  CanMessageStruct rx_queue[RX_QUEUE_SIZE];
  int rx_queue_put_pos;
  int rx_queue_get_pos;

  CRITICAL_SECTION rx_queue_cs;
  CRITICAL_SECTION tx_message_queue_cs;
  CRITICAL_SECTION usb_handle_cs;

  CPC_INIT_PARAMS_T* device_params;

} CanPortDataStruct;


#endif