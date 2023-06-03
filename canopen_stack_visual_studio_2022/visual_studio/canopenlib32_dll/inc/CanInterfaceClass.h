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

#ifndef CAN_INTERFACE_CLASS_H
#define CAN_INTERFACE_CLASS_H


#include <windows.h>
#include "CANopenLibDefines.h"
#include "CANopenLibHelper.h"
//#include "canopenlib_hw.h"
#include "canopenlib_hw_dynload.h"
#include "license.h"



//
// CAN informaiton flags.
//
#define CAN_MSG_RTR              0x0001      // Message is a remote request
#define CAN_MSG_EXT              0x0002      // Message has a standard ID

typedef struct 
{
  DispatcherCanFuncPtr                        can_consumer_callback;  // pointer to callback function for processing messages.
  ProtocolImplementationStateMachineFuncPtr   protocol_state_machine_callback;  // Pointer to a transfer helper callback func that is being executed in the same thread as process message to avoid unsafe-thread errors.
  void                                        *context;   // Addtional info, typically object.
} DispatcherConfiguration;

// Class that handles the interfacing to the CAN layer.
class CANOPENDLL_API CanInterface
{
  public:
    CanInterface ( int port_index );
    ~CanInterface();
    static CanInterface   *getCanInterface(int port);
    static CanInterface   *getCanInterfaceNoCreate(int port);
    HANDLE                can_frame_dispatcher_thread_mutex;

    static canOpenStatus  unlockCanopenLibrary(char *license_file, char *unlock_code);

    canOpenStatus         removeCanInterface(void);

    canPortHandle         canHandle;

    canOpenStatus         registerCanMessageHandler(
      void *context, 
      DispatcherCanFuncPtr can_consumer_callback, 
      ProtocolImplementationStateMachineFuncPtr protocol_state_machine_callback,
      int *can_message_handler_index
                          );

    canOpenStatus           unregisterCanMessageHandler(
                              int can_message_handler_index
                            );

    canOpenStatus           canOpenPort(int port, int bitrate);
    canOpenStatus           canClosePort(void);
    
    canOpenStatus           canGoBusOn(void);
    canOpenStatus           canGoBusOff(void);

    canOpenStatus           canRead(long * id, void * msg, unsigned int * dlc, 
      unsigned int * flags);
    canOpenStatus           canWrite(long id, void * msg, unsigned int dlc, 
      unsigned int flags); 

    canOpenStatus           canLibraryInit(void); // wrap CAN initalize library call for example.

    canOpenStatus           canOpenHardwarePort(int port);
    canOpenStatus           canCloseHardwarePort(int port);

    canOpenStatus           canSetBitrate(int bitrate);

    canOpenStatus           canGetSerialNumber(char *buffer, int bufferLen);

    canOpenStatus           canFrameDispatcher(
                                unsigned long id, 
                                unsigned char *data, 
                                unsigned int dlc, 
                                unsigned int flags);
    canOpenStatus           protocolImplementationDispatcher(DWORD ticks);
    bool                    is_can_dispatcher_thread_running;
    char                    adapter_serial_number[50];

	canOpenStatus           canDispatcherPerformance(
                                int sleepNoMessageFromCanInterface,
                                int sleepProcessedCanInterface);
    int                     sleep_no_message_from_can_interface;
    int                     sleep_processed_can_interface;

  protected:
  private:
    static CanInterface     *canInterfaceSingleton[ MAX_CAN_INTERFACES ];
    DispatcherConfiguration dispatcherConfiguration[ MAX_PROCESS_MSG_CALLBACKS ];
    static DWORD WINAPI     canFrameDispatcherThread( PVOID p );
    HINSTANCE               hCanLib;
    HANDLE                  can_frame_dispatcher_thread_handle;
    bool                    drivers_initialized;
    static bool             driver_licensed;
    bool                    can_port_opened;
    bool                    can_port_bus_on;


    
  
    int                     port_index;
    int                     port_users;  // Counter for keeping track on the number of users of the port.
    HANDLE                  port_mutex;

    HANDLE                  can_message_dispatcher_mutex;
    void                    startDispatcherThread(void);
    void                    stopDispatcherThread(void);
};

#endif
