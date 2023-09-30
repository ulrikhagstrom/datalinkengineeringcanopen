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

#include "TimerClass.h"

TimeClass* TimeClass::time_object_singleton = NULL;  // This stupid thing is nesscary in c++, otherwise it will be unresolved external.
bool TimeClass::is_timer_thread_running = 0;

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
TimeClass::TimeClass()
{
  timer_callback_dispatcher_mutex = CreateMutex(NULL, FALSE, NULL);
  (void)initTimerParams();
  (void)startTimerThread();
  return;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
TimeClass :: ~TimeClass()
{
  this->stopTimerThread();
  CloseHandle(timer_callback_dispatcher_mutex);
}


unsigned long TimeClass::readTimer(void)
{
  return ((unsigned long)::GetTickCount());
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TimeClass::initTimerParams(void)
{
  time_object_singleton = NULL; //Static, can not use "this" pointer.
  for (int i = 0; i < MAX_TIMER_CALLBACKS; i++)
  {
    this->timer_callbacks_array[i].users_callback = NULL;
    this->timer_callbacks_array[i].users_period_time = 0;
    this->timer_callbacks_array[i].users_context = NULL;
  }
  this->num_of_timer_users = 0;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus TimeClass::stopTimerThread(void)
{
  this->is_timer_thread_running = false;
  WaitForSingleObject(timer_thread_handle, INFINITE);
  return CANOPEN_OK;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus TimeClass::startTimerThread(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  DWORD tid;
  if (is_timer_thread_running == false)
  {
    timer_thread_handle = CreateThread(NULL, 0, timerThread,
      (LPVOID)0, CREATE_SUSPENDED, &tid);
    is_timer_thread_running = true;
    (void)ResumeThread(timer_thread_handle);
  }
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
DWORD WINAPI TimeClass::timerThread(PVOID p)
{
  int i = 0;
  DWORD now = 0;
  TimeClass* time_object = TimeClass::getTimeInterface();
  TimerCallbackInfo* callback_info = NULL;

  while (is_timer_thread_running)
  {
    now = TimeClass::readTimer();
    while (i < MAX_TIMER_CALLBACKS)
    {
      callback_info = &(time_object->timer_callbacks_array[i]);
      if (callback_info->users_callback != NULL &&
        callback_info->users_context != NULL &&
        callback_info->users_period_time != 0)
      {
        // Check if expired.
        if ((now - callback_info->callback_last_expired) > callback_info->users_period_time)
        {
          callback_info->callback_last_expired = now;
          callback_info->users_callback(callback_info->users_context);
        }
      }
      i++;
    }
    i = 0;
    Sleep(50);
  }
  return 0;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus TimeClass::registerPeriodicCallback(
  TimeHandlerFuncPtr callback_function, void* context, unsigned long period_time,
  int* periodic_timer_index)
{
  canOpenStatus ret = CANOPEN_ERROR;
  bool  free_slot_found = false;
  bool  end_of_list_found = false;
  int   i = 0;

  WaitForSingleObject(timer_callback_dispatcher_mutex, INFINITE);
  while ((free_slot_found == false) && (end_of_list_found == false))
  {
    if (i < MAX_TIMER_CALLBACKS)
    {
      if (timer_callbacks_array[i].users_callback == NULL)
      {
        free_slot_found = true;
      }
      else
      {
        i++;
      }
    }
    if (i == MAX_TIMER_CALLBACKS)
    {
      end_of_list_found = true;
    }
  }
  if (free_slot_found == true && end_of_list_found == false)
  {
    timer_callbacks_array[i].callback_last_expired = 0;
    timer_callbacks_array[i].users_callback = callback_function;
    timer_callbacks_array[i].users_period_time = period_time;
    timer_callbacks_array[i].users_context = context;
    *periodic_timer_index = i;
    ret = CANOPEN_OK;
  }
  else
  {
    ret = CANOPEN_OUT_OF_MEM;
  }
  ReleaseMutex(timer_callback_dispatcher_mutex);
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus TimeClass::unregisterPeriodicCallback(int periodic_timer_index)
{
  canOpenStatus ret = CANOPEN_ERROR;
  WaitForSingleObject(timer_callback_dispatcher_mutex, INFINITE);
  if ((periodic_timer_index >= 0) &&
    (periodic_timer_index < MAX_TIMER_CALLBACKS))
  {
    timer_callbacks_array[periodic_timer_index].users_callback = NULL;
    ret = CANOPEN_OK;
  }
  ReleaseMutex(timer_callback_dispatcher_mutex);
  return ret;
}

//------------------------------------------------------------------------
// Singleton implementation of the TimeClass.
//------------------------------------------------------------------------
TimeClass* TimeClass::getTimeInterface(void)
{
  if (time_object_singleton == NULL)
  {
    time_object_singleton = new TimeClass();
  }
  time_object_singleton->num_of_timer_users++; // At least 1 or more.
  return time_object_singleton;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TimeClass::removeTimeInterface(void)
{
  // Check number of users, delete instance if possible.
  this->num_of_timer_users--;
  if (this->num_of_timer_users == 0)
  {
    time_object_singleton = NULL;
    delete this;
  }
}



