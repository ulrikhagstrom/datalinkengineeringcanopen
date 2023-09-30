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

// canlib32.cpp : Defines the entry point for the DLL application.
//

#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>
#include <tchar.h>
#include <winerror.h>

#include "canopenlib_hw.h"
#include "canusb_porting.h"

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <conio.h>
#include <shlwapi.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif


#ifdef _MANAGED
#pragma managed(pop)
#endif

#define COM_PORT_BAUDRATE   57600
#define MAX_CAN_DEVICES     4

static CanPortDataStruct can_port_data_devices[MAX_CAN_DEVICES];

canOpenStatus comPortRead(CanPortDataStruct* can_com, int expected_data_length,
  int max_data_length, void* data, int* data_length);

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

static int findFreeCanPortDataSlot(void)
{
  int com_port_data_index = -1;
  for (int i = 0; i < MAX_CAN_DEVICES; i++) {
    if (can_port_data_devices[i].in_use == false) {
      com_port_data_index = i;
      break;
    }
  }
  return com_port_data_index;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

BOOL APIENTRY DllMain(HMODULE hModule,
  DWORD  ul_reason_for_call,
  LPVOID lpReserved
)
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    break;
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus put_can_message_in_queue(CanPortDataStruct* p, CanMessageStruct* can_frame)
{
  canOpenStatus res;

  EnterCriticalSection(&p->rx_queue_cs); // We need to imlplement a "faked" echo mechanism.

  int put_pos = p->rx_queue_put_pos;

  if (++put_pos >= RX_QUEUE_SIZE)
    put_pos = 0;

  if (put_pos != p->rx_queue_get_pos) {
    memcpy(&(p->rx_queue[p->rx_queue_put_pos]), can_frame, sizeof(CanMessageStruct));
    p->rx_queue_put_pos = put_pos;
    res = CANOPEN_OK;
  }
  else {
    res = CANOPEN_ERROR_CAN_LAYER_OVRFLOW;
  }

  LeaveCriticalSection(&p->rx_queue_cs);

  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus get_can_message_from_queue(CanPortDataStruct* p,
  CanMessageStruct* can_frame)
{
  canOpenStatus res;
  int get_pos = p->rx_queue_get_pos;

  if (get_pos != p->rx_queue_put_pos) {
    memcpy(can_frame, &(p->rx_queue[p->rx_queue_get_pos]), sizeof(CanMessageStruct));
    if (++get_pos >= RX_QUEUE_SIZE)
      get_pos = 0;
    p->rx_queue_get_pos = get_pos;
    res = CANOPEN_OK;
  }
  else {
    res = CANOPEN_ERROR_NO_MESSAGE;
  }
  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

DWORD WINAPI canUsbRxThread(void* can_port_data)
{
  char rx_buf[100];
  int bytes_read;
  DWORD last_error = 0;

  CanPortDataStruct* p = (CanPortDataStruct*)can_port_data;
  while (p->can_usb_rx_thread_running)
  {
    if (comPortRead(p, 1, 1, rx_buf, &bytes_read) == CANOPEN_OK)
    {
      if (bytes_read == 0) {
        continue;
      }
      switch (rx_buf[0])
      {
      case 'V':
        // Response to version request.
        if (comPortRead(p, 5, 5, rx_buf, &bytes_read) == CANOPEN_OK) {
          memcpy(p->version_result, rx_buf, 5);
          SetEvent(p->version_result_event);
          break;
        }
        break;
      case 'N':
        // Response to serial number request.
        if (comPortRead(p, 5, 5, rx_buf, &bytes_read) == CANOPEN_OK) {
          memcpy(p->version_result, rx_buf, 5);
          SetEvent(p->serial_number_event);
        }
        break;
      case 'F':
        // Response to status flags request.
        if (comPortRead(p, 3, 3, rx_buf, &bytes_read) == CANOPEN_OK) {
          memcpy(p->status_flags, rx_buf, 3);
          SetEvent(p->status_flags_event);
        }
        break;
      case 'z':
      case 'Z':
        // Successful TX queueing.
        if (comPortRead(p, 1, 1, rx_buf, &bytes_read) == CANOPEN_OK) {
          memcpy(p->tx_message_result, rx_buf, 1);
          SetEvent(p->tx_message_result_event);
        }
        break;
      case 13: // [CR]
      case 7: // [Bell]
        // Successful command.
        if (rx_buf[0] == 13) {
          p->command_result = true;
        }
        else {
          p->command_result = false;
        }
        SetEvent(p->command_result_event);
        break;
      case 't':
        // Get identifier and dlc.
        if (comPortRead(p, 4, 4, rx_buf, &bytes_read) == CANOPEN_OK) {
          CanMessageStruct can_frame;
          can_frame.flags = 0;
          sscanf_s(rx_buf, "%03x", &can_frame.id);
          sscanf_s(&rx_buf[3], "%1x", &can_frame.dlc);
          // Get the CAN data.
          unsigned int dlc = can_frame.dlc > 8 ? 8 : can_frame.dlc;
          if (comPortRead(p, (dlc * 2 + 1), (2 * 8 + 1),
            rx_buf, &bytes_read) == CANOPEN_OK) {
            //unsigned char can_data[8];
            int data_byte;
            for (unsigned int i = 0; i < dlc; i++) {
              sscanf_s(&rx_buf[i * 2], "%2x", &data_byte);
              can_frame.data[i] = (char)data_byte;
            }
            (void)put_can_message_in_queue(p, &can_frame);
          }
        }
        break;
      case 'T':
        // Get identifier and dlc.
        if (comPortRead(p, 9, 9, rx_buf, &bytes_read) == CANOPEN_OK) {
          CanMessageStruct can_frame;
          can_frame.flags = CAN_MSG_EXT;
          sscanf_s(rx_buf, "%08x", &can_frame.id);
          sscanf_s(&rx_buf[8], "%1x", &can_frame.dlc);
          // Get the CAN data.
          unsigned int dlc = can_frame.dlc > 8 ? 8 : can_frame.dlc;
          if (comPortRead(p, (dlc * 2 + 1), (2 * 8 + 1), rx_buf,
            &bytes_read) == CANOPEN_OK) {
            // unsigned char can_data[8];
            int data_byte;
            for (unsigned int i = 0; i < dlc; i++) {
              sscanf_s(&rx_buf[i * 2], "%2x", &data_byte);
              can_frame.data[i] = (char)data_byte;
            }
            (void)put_can_message_in_queue(p, &can_frame);
          }
        }
        break;
      case 'r':
        // Get identifier and dlc.
        if (comPortRead(p, 4, 4, rx_buf, &bytes_read) == CANOPEN_OK) {
          CanMessageStruct can_frame;
          can_frame.flags = CAN_MSG_RTR;
          sscanf_s(rx_buf, "%03x", &can_frame.id);
          sscanf_s(&rx_buf[3], "%1x", &can_frame.dlc);
          // Get the CAN data.
          if (comPortRead(p, 1, 1, rx_buf, &bytes_read) == CANOPEN_OK) {
            (void)put_can_message_in_queue(p, &can_frame);
          }
        }
        break;
      case 'R':
        // Get identifier and dlc.
        if (comPortRead(p, 9, 9, rx_buf, &bytes_read) == CANOPEN_OK) {
          CanMessageStruct can_frame;
          can_frame.flags = CAN_MSG_RTR;
          sscanf_s(rx_buf, "%08x", &can_frame.id);
          sscanf_s(&rx_buf[8], "%1x", &can_frame.dlc);
          // Get the CAN data.
          if (comPortRead(p, 1, 1, rx_buf, &bytes_read) == CANOPEN_OK) {
            (void)put_can_message_in_queue(p, &can_frame);
          }
        }
        break;
      default:
        break;
      }
    }
  }
  return 0;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortLibraryInit(void)
{
  // Make sure that all CAN ports are available.
  for (int i = 0; i < MAX_CAN_DEVICES; i++) {
    can_port_data_devices[i].in_use = false;
    can_port_data_devices[i].echo_enabled = false;
  }
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus comPortClose(int can_port_data_index)
{
  if (can_port_data_devices[can_port_data_index].in_use)
  {
    CloseHandle(can_port_data_devices[can_port_data_index].com_port_handle);
    CloseHandle(can_port_data_devices[can_port_data_index].os_write_port.hEvent);
    CloseHandle(can_port_data_devices[can_port_data_index].os_read_port.hEvent);
    CloseHandle(can_port_data_devices[can_port_data_index].version_result_event);
    CloseHandle(can_port_data_devices[can_port_data_index].status_flags_event);
    CloseHandle(can_port_data_devices[can_port_data_index].tx_message_result_event);
    CloseHandle(can_port_data_devices[can_port_data_index].command_result_event);
    can_port_data_devices[can_port_data_index].can_usb_rx_thread_running = false;
    can_port_data_devices[can_port_data_index].in_use = false;
    // Make sure that the rx-thread is terminated.
    WaitForSingleObject(
      can_port_data_devices[can_port_data_index].rx_thread_handle, 5000);
  }
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus comPortOpen(int com_port, int* can_port_data_index)
{
  TCHAR portName[16];
  _stprintf(portName, _T("com%d:"), com_port);
  DCB com_port_settings;
  canOpenStatus err = CANOPEN_OK;

  int can_port_index = findFreeCanPortDataSlot();
  if (can_port_index < 0) {
    err = CANOPEN_OUT_OF_CAN_INTERFACES;
  }
  else {
    *can_port_data_index = can_port_index;
  }

  if (err == CANOPEN_OK) {
    can_port_data_devices[can_port_index].com_port_handle =
      CreateFile(portName,
        GENERIC_READ | GENERIC_WRITE, // dwDesiredAccess
        0,                            // dwShareMode
        NULL,                         // lpSecurityAttributes
        OPEN_EXISTING,                // dwCreationDistribution
        FILE_FLAG_OVERLAPPED,         // dwFlagsAndAttributes
        0);                           // hTemplateFile
    if (can_port_data_devices[can_port_index].com_port_handle ==
      INVALID_HANDLE_VALUE) {
      err = CANOPEN_ERROR_DRIVER;
    }
  }

  if (err == CANOPEN_OK) {
    com_port_settings.DCBlength = sizeof(DCB);
    // Get the default port setting information.
    if (!GetCommState(can_port_data_devices[can_port_index].com_port_handle,
      &com_port_settings)) {
      CloseHandle(can_port_data_devices[can_port_index].com_port_handle);
      err = CANOPEN_ERROR_DRIVER;
    }
    if (err == CANOPEN_OK) {
      com_port_settings.BaudRate = COM_PORT_BAUDRATE; // Default for CANUSB devices.
      com_port_settings.fDtrControl = DTR_CONTROL_DISABLE; // DTR flow control type
      com_port_settings.fRtsControl = RTS_CONTROL_DISABLE; // RTS flow control
      com_port_settings.fBinary = TRUE;
      com_port_settings.fParity = FALSE;              // No parity checking
      com_port_settings.fOutxCtsFlow = FALSE;         // No CTS output flow control
      com_port_settings.fOutxDsrFlow = FALSE;         // No DSR output flow control
      com_port_settings.fDsrSensitivity = FALSE;      // DSR sensitivity
      com_port_settings.fTXContinueOnXoff = TRUE;     // XOFF continues Tx
      com_port_settings.fOutX = FALSE;                // No XON/XOFF out flow control
      com_port_settings.fInX = FALSE;                 // No XON/XOFF in flow control
      com_port_settings.fErrorChar = FALSE;           // Disable error replacement
      com_port_settings.fNull = FALSE;                // Disable null stripping
      com_port_settings.fAbortOnError = FALSE;        // Do not abort reads/writes on error
      com_port_settings.ByteSize = 8;                 // Number of bits/byte, 4-8
      com_port_settings.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space
      com_port_settings.StopBits = ONESTOPBIT;        // 0,1,2 = 1, 1.5, 2
    }
  }
  /*
  if (!SetCommState( can_port_data_devices[can_port_index].com_port_handle,
    &com_port_settings )) {
    err = CANOPEN_ERROR_DRIVER;
  }
  */


  if (err == CANOPEN_OK) {
    COMMTIMEOUTS timeouts;
    // We want read operatins to succeed at once if there are anything to read,
    // or return after a short timeout.
    // By adjusting the number of bytes to read according to if we are waiting
    // for SOF or receiving a frame, we can avoid being waken up too often.
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 1000;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    if (!SetCommTimeouts(can_port_data_devices[can_port_index].com_port_handle,
      &timeouts))
      err = CANOPEN_ERROR_DRIVER;
  }

  if (err == CANOPEN_OK) {
    can_port_data_devices[can_port_index].in_use = true;

    ZeroMemory(&can_port_data_devices[can_port_index].os_write_port,
      sizeof(can_port_data_devices[can_port_index].os_write_port));
    can_port_data_devices[can_port_index].os_write_port.hEvent =
      CreateEvent(NULL, TRUE, FALSE, NULL);
    ZeroMemory(&can_port_data_devices[can_port_index].os_read_port,
      sizeof(can_port_data_devices[can_port_index].os_read_port));
    can_port_data_devices[can_port_index].os_read_port.hEvent =
      CreateEvent(NULL, TRUE, FALSE, NULL);
    can_port_data_devices[can_port_index].version_result_event =
      CreateEvent(NULL, TRUE, FALSE, NULL);
    can_port_data_devices[can_port_index].status_flags_event =
      CreateEvent(NULL, TRUE, FALSE, NULL);
    can_port_data_devices[can_port_index].tx_message_result_event =
      CreateEvent(NULL, TRUE, FALSE, NULL);
    can_port_data_devices[can_port_index].command_result_event =
      CreateEvent(NULL, TRUE, FALSE, NULL);

    can_port_data_devices[can_port_index].rx_queue_put_pos = 0;
    can_port_data_devices[can_port_index].rx_queue_get_pos = 0;

    InitializeCriticalSection(&can_port_data_devices[can_port_index].rx_queue_cs);
    InitializeCriticalSection(&can_port_data_devices[can_port_index].tx_message_queue_cs);

    can_port_data_devices[can_port_index].rx_thread_handle =
      CreateThread(NULL,                                           // lpThreadAttributes
        0,                                                         // dwStackSize
        canUsbRxThread,                                            // lpStartAddress
        &can_port_data_devices[can_port_index],                    // lpParameter
        CREATE_SUSPENDED,                                          // wCreationFlags
        &can_port_data_devices[can_port_index].can_usb_rx_thread_id); // pThreadId

    can_port_data_devices[can_port_index].can_usb_rx_thread_running = true;
    SetThreadPriority(can_port_data_devices[can_port_index].rx_thread_handle,
      THREAD_PRIORITY_ABOVE_NORMAL);
    ResumeThread(can_port_data_devices[can_port_index].rx_thread_handle);
  }
  return err;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus comPortRead(CanPortDataStruct* can_com, int expected_data_length,
  int max_data_length, void* data, int* data_length)
{
  DWORD res;
  DWORD bytes_read;
  canOpenStatus ret = CANOPEN_OK;

  ResetEvent(can_com->os_read_port.hEvent);
  if (!ReadFile(can_com->com_port_handle, data, expected_data_length,
    &bytes_read, &can_com->os_read_port)) {
    DWORD last_error = GetLastError();
    if (last_error != ERROR_IO_PENDING) {
      ret = CANOPEN_ERROR;
    }
    else {
      res = WaitForSingleObject(can_com->os_read_port.hEvent, INFINITE);
      if (res != WAIT_OBJECT_0) {
        ret = CANOPEN_ERROR;
      }
      if (!GetOverlappedResult(can_com->com_port_handle,
        &can_com->os_read_port, &bytes_read, TRUE)) {
        ret = CANOPEN_ERROR;
      }
    }
  }
  *data_length = (int)bytes_read;
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

canOpenStatus comPortWrite(CanPortDataStruct* can_com, void* data, int data_length)
{
  canOpenStatus err = CANOPEN_OK;
  DWORD bytes_written;
  DWORD bytes_to_write;

  EnterCriticalSection(&can_com->tx_message_queue_cs); // Can be executed from application and timer therads (or even more).

  if (data_length == 0)
    bytes_to_write = (DWORD)strlen((char*)data);
  else
    bytes_to_write = data_length;

  ResetEvent(can_com->os_write_port.hEvent);
  if (!WriteFile(can_com->com_port_handle, data, bytes_to_write,
    &bytes_written, &can_com->os_write_port)) {
    if (GetLastError() != ERROR_IO_PENDING) {
      err = CANOPEN_ERROR_DRIVER;
    }
    else {
      DWORD res = WaitForSingleObject(can_com->os_write_port.hEvent, INFINITE);
      if (res == WAIT_OBJECT_0) {
        if (!GetOverlappedResult(can_com->com_port_handle,
          &can_com->os_write_port, &bytes_written, FALSE)) {
          err = CANOPEN_ERROR_DRIVER;
        }
        else if (bytes_written != bytes_to_write) {
          err = CANOPEN_ERROR_DRIVER;
        }
        else
          err = CANOPEN_OK;
      }
      else {
        err = CANOPEN_TIMEOUT;
      }
    }
  }
  else {
    if (bytes_written != bytes_to_write)
      err = CANOPEN_TIMEOUT;
    else
      err = CANOPEN_OK;
  }

  LeaveCriticalSection(&can_com->tx_message_queue_cs); // Can be executed from application and timer therads (or even more).

  return err;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortOpen(int port,
  canPortHandle* handle)
{
  DWORD res;
  canOpenStatus err = comPortOpen(port, handle);
  if (err == CANOPEN_OK) {
    char version_cmd[] = { 'C', 0x0d, 'V', 0x0d }; // Go bus off and then get version.
    ResetEvent(can_port_data_devices[*handle].version_result_event);
    err = comPortWrite(&can_port_data_devices[*handle], version_cmd,
      sizeof(version_cmd));
    if (err == CANOPEN_OK) {
      res = WaitForSingleObject(can_port_data_devices[*handle].version_result_event,
        10000);
      if (res == WAIT_TIMEOUT) { // No response on version request.
        comPortClose(*handle);
        err = CANOPEN_ERROR_HW_NOT_CONNECTED;
      }
    }
  }
  return err;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortClose(canPortHandle handle)
{
  (void)canPortGoBusOff(handle);
  // Go bus off etc ?
  return comPortClose(handle);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortBitrateSet(canPortHandle handle,
  int bitrate)
{
  canOpenStatus res = CANOPEN_OK;
  char bitrate_cmd[] = { 'S', 00, 0x0D };
  char close_cmd[] = { 0xD, 0xD, 0xD, 'C', 0xD };

  // Close CAN in case it is opened, ignore result.
  ResetEvent(can_port_data_devices[handle].command_result_event);
  res = comPortWrite(&can_port_data_devices[handle], close_cmd, sizeof(close_cmd));
  if (res == CANOPEN_OK) {
    DWORD dw_res;
    dw_res = WaitForSingleObject(can_port_data_devices[handle].command_result_event, 5000);
    if (dw_res == WAIT_TIMEOUT) { // No response on version request.
      res = CANOPEN_ERROR_HW_NOT_CONNECTED;
    }
    else {
      if (can_port_data_devices[handle].command_result == false) {
        res = CANOPEN_ERROR_DRIVER;
      }
    }
  }

  res = CANOPEN_OK;
  switch (bitrate)
  {
  case 125000:
    bitrate_cmd[1] = '4';
    break;
  case 250000:
    bitrate_cmd[1] = '5';
    break;
  case 500000:
    bitrate_cmd[1] = '6';
    break;
  case 1000000:
    bitrate_cmd[1] = '8';
    break;
  default:
    res = CANOPEN_UNSUPPORTED_BITRATE;
    break;
  }

  if (res == CANOPEN_OK)
  {
    ResetEvent(can_port_data_devices[handle].command_result_event);
    res = comPortWrite(&can_port_data_devices[handle], bitrate_cmd, sizeof(bitrate_cmd));
    if (res == CANOPEN_OK) {
      DWORD dw_res;
      dw_res = WaitForSingleObject(can_port_data_devices[handle].command_result_event, 5000);
      if (dw_res == WAIT_TIMEOUT) { // No response on version request.
        res = CANOPEN_ERROR_HW_NOT_CONNECTED;
      }
      else {
        if (can_port_data_devices[handle].command_result == false) {
          res = CANOPEN_ERROR_DRIVER;
        }
      }
    }
  }
  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortEcho(canPortHandle handle,
  bool enabled)
{
  // CANUSB or CAN232 doesn't support echo therefore we
  // need to fake this echo mechanism when sending the
  // frame.
  can_port_data_devices[handle].echo_enabled = enabled;
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOn(canPortHandle handle)
{
  canOpenStatus res = CANOPEN_OK;
  char open_cmd[] = { 'O', 0xD };
  ResetEvent(can_port_data_devices[handle].command_result_event);
  res = comPortWrite(&can_port_data_devices[handle], open_cmd, sizeof(open_cmd));
  if (res == CANOPEN_OK) {
    DWORD dw_res;
    dw_res = WaitForSingleObject(can_port_data_devices[handle].command_result_event, 5000);
    if (dw_res == WAIT_TIMEOUT) { // No response on version request.
      res = CANOPEN_ERROR_HW_NOT_CONNECTED;
    }
    else {
      if (can_port_data_devices[handle].command_result == false) {
        res = CANOPEN_ERROR_DRIVER;
      }
    }
  }
  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------


CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortGoBusOff(canPortHandle handle)
{
  canOpenStatus res = CANOPEN_OK;
  char open_cmd[] = { 'C', 0xD };
  ResetEvent(can_port_data_devices[handle].command_result_event);
  res = comPortWrite(&can_port_data_devices[handle], open_cmd, sizeof(open_cmd));
  if (res == CANOPEN_OK) {
    DWORD dw_res;
    dw_res = WaitForSingleObject(can_port_data_devices[handle].command_result_event, 5000);
    if (dw_res == WAIT_TIMEOUT) { // No response on version request.
      res = CANOPEN_ERROR_HW_NOT_CONNECTED;
    }
    else {
      if (can_port_data_devices[handle].command_result == false) {
        res = CANOPEN_ERROR_DRIVER;
      }
    }
  }
  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortWrite(canPortHandle handle,
  long id,
  void* msg,
  unsigned int dlc,
  unsigned int flags)
{
  char transmit_command[100];
  int transmit_command_lenght;
  int header_len = 0;
  char command;
  char* data = (char*)msg;
  canOpenStatus res;
  CanMessageStruct can_frame;

  if (can_port_data_devices[handle].echo_enabled)
  {
    can_frame.id = id;
    can_frame.dlc = dlc;
    can_frame.flags = flags;
    if ((flags & CAN_MSG_RTR) == 0) {
      memcpy(can_frame.data, msg, dlc);
    }
  }
  if (flags & CAN_MSG_RTR)
  {
    if (flags & CAN_MSG_EXT) {
      command = 'R';
    }
    else {
      command = 'r';
    }
  }
  else
  {
    if (flags & CAN_MSG_EXT) {
      command = 'T';
    }
    else {
      command = 't';
    }
  }
  sprintf(transmit_command, "%c%03x%d", command, id, dlc);
  if (flags & CAN_MSG_RTR) {
    dlc = 0;
  }
  header_len = (int)strlen(transmit_command);
  for (unsigned int i = 0; i < dlc; i++) {
    sprintf(&(transmit_command[header_len + i * 2]), "%02x", ((*data) & 0xff));
    data++;
  }

  transmit_command_lenght = header_len + dlc * 2;
  transmit_command[transmit_command_lenght++] = 0xd;
  ResetEvent(can_port_data_devices[handle].command_result_event);
  res = comPortWrite(&can_port_data_devices[handle], transmit_command,
    transmit_command_lenght);
  if (res == CANOPEN_OK) {
    DWORD dw_res;
    dw_res = WaitForSingleObject(can_port_data_devices[handle].tx_message_result_event, 5000);
    if (dw_res == WAIT_TIMEOUT) { // No response on version request.
      res = CANOPEN_ERROR_HW_NOT_CONNECTED;
    }
    else {
      if (can_port_data_devices[handle].command_result == false) {
        res = CANOPEN_ERROR_DRIVER;
      }
    }
  }
  if (res == CANOPEN_OK && can_port_data_devices[handle].echo_enabled)
  {
    put_can_message_in_queue(&can_port_data_devices[handle], &can_frame);
  }
  return res;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus    __stdcall canPortRead(canPortHandle handle,
  long* id,
  void* msg,
  unsigned int* dlc,
  unsigned int* flags)
{
  CanMessageStruct can_frame;
  canOpenStatus ret;

  ret = get_can_message_from_queue(&can_port_data_devices[handle], &can_frame);
  if (ret == CANOPEN_OK)
  {
    *id = can_frame.id;
    *dlc = can_frame.dlc;
    *flags = can_frame.flags;
    memcpy(msg, can_frame.data, can_frame.dlc);
  }
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CANOPENLIB_HW_API   canOpenStatus  __stdcall canPortGetSerialNumber(canPortHandle handle, char* buffer, int bufferLen)
{
  return CANOPEN_ERROR;
}