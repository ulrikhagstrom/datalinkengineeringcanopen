#include "CanConnectionClass.h"


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

CanConnection::CanConnection()
{
  this->can_hardware_is_initiated = false;
  this->can_interface = NULL;
  this->can_message_handler_index = -1;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  CanConnection :: canHardwareInit(int port, int bitrate, 
  DispatcherCanFuncPtr can_frame_handler_function, 
  ProtocolImplementationStateMachineFuncPtr transfer_protocol_handler_function)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if ( this->can_hardware_is_initiated )
  {
    // Make it impossible to initated more than once, 
    // to for example other CAN port.
    return CANOPEN_ERROR_HW_CONNECTED;
  }

  this->can_interface = ::CanInterface::getCanInterface(port);
  if (this->can_interface != NULL)
  {
    ret = this->can_interface->canOpenPort( port, bitrate );
    if (ret == CANOPEN_OK)
    {
      ret = this->can_interface->canGoBusOn();
    } 
  }

  if (ret == CANOPEN_OK)
  {
    ret = this->can_interface->registerCanMessageHandler(this, can_frame_handler_function, 
      transfer_protocol_handler_function, &this->can_message_handler_index);
    this->can_hardware_is_initiated = true;
  }
  return ret;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus CanConnection :: canHardwareConnect(u8 port, u32 bitrate)
{
  return this->canHardwareInit(port, bitrate, NULL, NULL);
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus  CanConnection :: canHardwareDisconnect(void)
{
  canOpenStatus ret = CANOPEN_ERROR;
  if ( this->can_hardware_is_initiated == false )
  {
    ret = CANOPEN_ERROR_HW_NOT_CONNECTED;
  }
  else
  {
    if ( this->can_interface != NULL )
    {
      ret = this->can_interface->unregisterCanMessageHandler( this->can_message_handler_index );
      if (ret == CANOPEN_OK)
      {
        ret = this->can_interface->canClosePort();
      }
    }
    this->can_message_handler_index = - 1;
    this->can_hardware_is_initiated = false;
    this->can_interface = NULL;
    ret = CANOPEN_OK;
  }  
  return ret;
}
