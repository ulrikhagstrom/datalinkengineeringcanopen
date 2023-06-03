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

#ifndef TIMER_CLASS_H
#define TIMER_CLASS_H

#include <windows.h>
#include "CANopenLibDefines.h"

class TimeClass
{
  public:
    typedef canOpenStatus   (*TimeHandlerFuncPtr)( void *p ); // Callback to be used for handling timer related tasks.

    typedef struct 
    {
      TimeHandlerFuncPtr    users_callback;                 // callback function pointer to be used.
      void                  *users_context;                // Pointer to object.
      unsigned long         users_period_time;           // The configured period time.
      DWORD                 callback_last_expired;  // time_stamp when timer was expired last time.
    }  TimerCallbackInfo;

    TimeClass();
    ~TimeClass();
    static TimeClass*     getTimeInterface( void ); //Singleton.
    void                  removeTimeInterface( void ); 
    canOpenStatus         registerPeriodicCallback( TimeHandlerFuncPtr callback_function, 
                              void *context, 
                              unsigned long period_time, 
                              int* periodic_timer_index 
                          ); // Singleton
    canOpenStatus         unregisterPeriodicCallback( int periodic_timer_index );
    static unsigned long  readTimer(void);
  protected:
  private:
    void                  initTimerParams( void );
    canOpenStatus         startTimerThread( void );
    canOpenStatus         stopTimerThread( void );
    static DWORD WINAPI   timerThread( PVOID p );

    static TimeClass      *time_object_singleton;  
    TimerCallbackInfo     timer_callbacks_array[ MAX_TIMER_CALLBACKS ];
    HANDLE                timer_thread_handle;
    static bool           is_timer_thread_running;
    int                   num_of_timer_users;
    HANDLE                timer_callback_dispatcher_mutex;
};
  

#endif 
