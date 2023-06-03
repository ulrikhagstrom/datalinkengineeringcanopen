/* Embedded Intelligence CAN interface functions */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define _INCLUDE_INTERNAL_DRIVER_INFO
#include <eican.h>

#define MAX_PORTS_PER_CARD     2
#define MAX_PORTS_TOTAL        10
#define MAX_PORT_NAME          40

// local functions
static void *_AllocLocals( void );
static void  _FreeLocals( void *lcl );
static int   _OpenFile( int port, void *lcl );
static int   _CloseFile( void *lcl );
static int   _SendCMD( void *lcl, uint32_t *cmd );
static int   _RecvMsg( void *lcl, CANCARD_MSG *can );
static int   _SendMsg( void *lcl, CANCARD_MSG *can );

// local data
static int totPorts = -1;
static char portNames[ MAX_PORTS_TOTAL ][ MAX_PORT_NAME ];

/**
 * Default constructor for EIcan class
 */
EIcan::EIcan( void )
{
   local = 0;
   bps = EICAN_BITRATE_1000000;
}

/**
 * EIcan destructor.  Closes the port if it was open.
 */
EIcan::~EIcan()
{
   Close();
}

/**
 * Open a CAN port.
 *
 * Parameters:
 *   port  The port number to open.  Ports are numbered starting at zero.
 *         Single channel cards have one port number / card.  Dual channel
 *         cards have two ports / card.
 *
 *   baud  The baud rate to set the port to.  This should be one of the 
 *         values defined in the eican.h header file.
 *
 * Returns:
 *    An error code or zero on success.
 */
int EIcan::Open( int port )
{
   int err = 0;

   // Allocate a structure to hold local data
   local = _AllocLocals();
   if( local == NULL )
      return EICAN_ERR_ALLOC;

   // Open a handle to the device driver
   err = _OpenFile( port, local );
   if( err )
   {
      _FreeLocals( local );
      local = 0;
   }

   return err;
}

/**
 * Close the CAN port.
 *
 * Returns:
 *    An error code or zero on success.
 */
int EIcan::Close( void )
{
   if( !local ) return 0;

   // Go offline
   Offline();

   // Close the handle to the driver
   int err = _CloseFile( local );

   // Free local data
   _FreeLocals( local );
   local = 0;
   return err;
}

/**
 * Set the bit rate to use when going on-line
 *
 * Parameters:
 *   rate  The baud rate to use.  This should be one of the 
 *         values defined in the eican.h header file.
 *         Actual rates in bits/sec are also accepted
 *
 * Returns:
 *    An error code or zero on success.
 */
int EIcan::SetBitRate( int rate )
{
   switch( rate )
   {
      case EICAN_BITRATE_1000000:
      case EICAN_BITRATE_800000:
      case EICAN_BITRATE_500000:
      case EICAN_BITRATE_250000:
      case EICAN_BITRATE_125000:
      case EICAN_BITRATE_100000:
      case EICAN_BITRATE_50000:
      case EICAN_BITRATE_20000:
         bps = rate;
         break;

      case 1000000: bps = EICAN_BITRATE_1000000; break;
      case 800000:  bps = EICAN_BITRATE_800000; break;
      case 500000:  bps = EICAN_BITRATE_500000; break;
      case 250000:  bps = EICAN_BITRATE_250000; break;
      case 125000:  bps = EICAN_BITRATE_125000; break;
      case 100000:  bps = EICAN_BITRATE_100000; break;
      case 50000:   bps = EICAN_BITRATE_50000; break;
      case 20000:   bps = EICAN_BITRATE_20000; break;

      default:
         return EICAN_ERR_BAD_PARAM;
   }
   return 0;
}

/**
 * Connect to the CAN bus;
 *
 * Parameters:
 *   baud  The baud rate to set the port to.  This should be one of the 
 *         values defined in the eican.h header file.
 *
 * Returns:
 *    An error code or zero on success.
 */
int EIcan::Online( void )
{
   int err = 0;
   uint32_t cmd[64];

   assert( local != NULL );

   // Set the port's baud rate
   cmd[0] = (EICAN_CMD_SETBPS<<16) | 1;
   cmd[1] = bps;
   err = _SendCMD( local, cmd );
   if( err ) return err;

   // Open the CAN port
   cmd[0] = (EICAN_CMD_OPENPORT<<16);
   return _SendCMD( local, cmd );
}

/**
 * Disconnect from the CAN bus
 *
 * Returns:
 *    An error code or zero on success.
 */
int EIcan::Offline( void )
{
   int err = 0;
   uint32_t cmd[64];

   assert( local != NULL );

   // Close the CAN port
   cmd[0] = (EICAN_CMD_CLOSEPORT<<16);
   return _SendCMD( local, cmd );
}

/**
 * Read the next CAN message from the port.  This function will block until
 * a message is available, or until the passed timeout expires.
 *
 * Parameters:
 *   frame Pointer to the CAN message structure that will be filled in on success.
 *
 *   timeout Timeout in milliseconds.  Zero timeouts indicate that the thread should
 *           never block if no data is available.  Negative timeouts indicate that the
 *           thread should wait forever.
 *
 * Returns:
 *    An error code or zero on success.
 *
 */
int EIcan::Recv( CanFrame *frame, int32_t timeout )
{
   CANCARD_MSG can;
   int i, err;

   assert( local != NULL );
   assert( frame != NULL );

   can.timeout = timeout;

   err = _RecvMsg( local, &can );
   if( err ) return err;

   frame->id         = can.id;
   frame->timestamp  = can.timestamp;
   frame->length     = can.flags & CANFLG_LENGTH;

   if( can.flags & CANFLG_ERROR )
      frame->type = CAN_FRAME_ERROR;
   else if( can.flags & CANFLG_RTR )
      frame->type =CAN_FRAME_REMOTE;
   else
      frame->type = CAN_FRAME_DATA;

   if( frame->length > 8 ) frame->length = 8;
   if( can.flags & CANFLG_EXTENDED )
      frame->id |= 0x20000000;

   for( i=0; i<frame->length; i++ )
      frame->data[i] = can.data[i];

   return 0;
}

/**
 * Write a message to the CAN network.
 *
 * Parameters:
 *   frame Pointer to the CAN message to transmit.
 *
 *   timeout Timeout in milliseconds.  Zero timeouts indicate that the thread should
 *           never block if no data is available.  Negative timeouts indicate that the
 *           thread should wait forever.
 *
 * Returns:
 *    An error code or zero on success.
 *
 */
int EIcan::Xmit( CanFrame *frame, int32_t timeout )
{
   CANCARD_MSG can;
   int i;

   assert( local != NULL );
   assert( frame != NULL );

   if( frame->length > 8 )
      return EICAN_ERR_BAD_PARAM;

   can.timeout = timeout;
   can.id      = frame->id;
   can.flags   = frame->length;

   switch( frame->type )
   {
      case CAN_FRAME_DATA:
         break;

      case CAN_FRAME_REMOTE:
         can.flags |= CANFLG_RTR;
         break;

      default:
         return EICAN_ERR_BAD_PARAM;
   }

   if( frame->id & 0x20000000 )
      can.flags |= CANFLG_EXTENDED;

   for( i=0; i<frame->length; i++ )
      can.data[i] = frame->data[i];

   // Send the message
   return _SendMsg( local, &can );
}

/**
 * Read information about the CAN card.
 *
 * Parameters:
 *   info Pointer to a structure which will be filled in by the call.
 *
 * Returns:
 *    An error code or zero on success.
 *
 */
int EIcan::ReadCardInfo( CanCardInfo *info )
{
   int err;
   uint32_t cmd[64];

   assert( local != NULL );

   // Read the firmware version info
   cmd[0] = (EICAN_CMD_GETVERSION<<16);
   err = _SendCMD( local, cmd );
   if( err ) return err;

   info->hwType  = cmd[1];
   info->bootVer = cmd[2];
   info->fwVer   = cmd[3];
   info->fpgaVer = cmd[4];

   cmd[0] = (EICAN_CMD_DRIVERVER<<16);
   err = _SendCMD( local, cmd );
   if( err ) return err;

   info->driverVer = cmd[1];

   cmd[0] = (EICAN_CMD_GETPARAM<<16) | 1;
   cmd[1] = EICAN_PARAM_SERIAL;
   err = _SendCMD( local, cmd );
   if( err ) return err;

   info->serial = cmd[1];

   return 0;
}

/*********************************************************************************
 * Code below this section is operating system specific.
 ********************************************************************************/
#ifdef WIN32

#include <windows.h>
typedef struct
{
   HANDLE hndl;
} DriverLocal;

// Allocate a structure to hold operating system specific local data
static void *_AllocLocals( void )
{
   return malloc( sizeof(DriverLocal) );
}

static void _FreeLocals( void *lcl )
{
   free( lcl );
}

// Figure out how many CAN ports the specified card has 
// by reading it's hardware type
static int CountPorts( const char *name, int card )
{
   char buff[MAX_PORT_NAME];
   uint32_t cmd[64];
   DriverLocal lcl;
   int err;

   _snprintf_s( buff, MAX_PORT_NAME, _TRUNCATE, "\\\\.\\%s%02d\\0", name, card );
   lcl.hndl = CreateFileA( buff, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 
                          NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, NULL );
   if( lcl.hndl == INVALID_HANDLE_VALUE )
      return 0;

   // Read version info
   cmd[0] = (EICAN_CMD_GETVERSION<<16);
   err = _SendCMD( &lcl, cmd );
   CloseHandle( lcl.hndl );
   if( err ) return 0;

   switch( cmd[1] )
   {
      case 0x00000001:
      case 0x00000100:
      case 0x00000200:
         return 2;

      default:
         return 1;
   }
}

static void ScanPorts( void )
{
   // Depending on the driver used, the device files could have different names.
   const char *cardNames[] = { "eican", "copleycan", 0 };
   int i, c, p;

   // Only scan once
   if( totPorts >= 0 ) return;
   totPorts = 0;

   for( i=0; cardNames[i]; i++ )
   {
      for( c=0; totPorts<MAX_PORTS_TOTAL; c++ )
      {
         int ports = CountPorts( cardNames[i], c );
         if( ports < 1 ) break;

         for( p=0; (p<ports) && (totPorts<MAX_PORTS_TOTAL); p++, totPorts++ )
            _snprintf_s( portNames[totPorts], MAX_PORT_NAME, _TRUNCATE, "\\\\.\\%s%02d\\%d", cardNames[i], c, p );
      }
   }
}

// Open the specified driver and initialize the local data structure 
// as necessary.
static int _OpenFile( int port, void *lcl )
{
   DriverLocal *local = (DriverLocal *)lcl;

   ScanPorts();
   if( port >= totPorts )
      return EICAN_ERR_CANT_OPEN_PORT;

   // Open the device file for this port
   local->hndl = CreateFileA( portNames[port], GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 
                              NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, NULL );
   if( local->hndl == INVALID_HANDLE_VALUE )
      return EICAN_ERR_CANT_OPEN_PORT;
   return 0;
}

int _CloseFile( void *lcl )
{
   DriverLocal *local = (DriverLocal *)lcl;
   CloseHandle( local->hndl );
   return 0;
}

int DoIoCtl( DriverLocal *local, uint32_t code, void *buff, uint32_t inBytes, 
                             uint32_t *outBytes, int32_t timeout )
{
   OVERLAPPED overlap;
   DWORD tot, ret;
   int err;
   BOOL ok;

   memset( &overlap, 0, sizeof(OVERLAPPED) );
   overlap.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

   ok = DeviceIoControl( local->hndl, code, buff, inBytes, buff, *outBytes, &tot, &overlap );
   if( ok ) return EICAN_ERR_DRIVER;

   err = GetLastError();
   if( err != ERROR_IO_PENDING )
      return EICAN_ERR_DRIVER;

   if( timeout < 0 ) timeout = INFINITE;
   ret = WaitForSingleObject( overlap.hEvent, timeout );

   // If the wait timed out, try another wait with zero timeout.  Windows
   // will sometimes timeout incorrectly.
   if( ret == WAIT_TIMEOUT )
      ret = WaitForSingleObject( overlap.hEvent, 0 );

   switch( ret )
   {
      case WAIT_OBJECT_0:
         err = 0;
         break;

      case WAIT_TIMEOUT:
         err = EICAN_ERR_TIMEOUT;
         break;

      default:
         err = EICAN_ERR_DRIVER;
         break;
   }

   if( err )
   {
      CancelIo( local->hndl );
      CloseHandle( overlap.hEvent );
      return err;
   }

   ok = GetOverlappedResult( local->hndl, &overlap, &tot, FALSE );

   *outBytes = tot;

   CloseHandle( overlap.hEvent );
   return 0;
}

/*
 * Write a command to the card.
 * @param lcl Local parameters
 * @param cmd Array of command data.  This array should be at least 64 words in length.
 * @return An error, or null on success.
 */
static int _SendCMD( void *lcl, uint32_t *cmd )
{
   int sendBytes, err;
   uint32_t tot;
   DriverLocal *local = (DriverLocal *)lcl;

   // Find the number of bytes of data sent with the message.
   sendBytes = 4 * ((cmd[0] & 0x3F) + 1);

   tot = 256;
   err = DoIoCtl( local, EICAN_IOCTL_CMD, cmd, sendBytes, &tot, 5000 );
   if( err ) return err;

   return cmd[0]>>16;
}

static int _RecvMsg( void *lcl, CANCARD_MSG *can )
{
   DriverLocal *local = (DriverLocal *)lcl;
   uint32_t tot = sizeof(CANCARD_MSG);

   int err = DoIoCtl( local, EICAN_IOCTL_RECVCAN, can, sizeof(CANCARD_MSG), &tot, can->timeout );
   if( err ) return err;

   if( tot == 4 )
      err = *(int32_t *)can;

   return err;
}

static int _SendMsg( void *lcl, CANCARD_MSG *can )
{
   DriverLocal *local = (DriverLocal *)lcl;
   uint32_t tot = sizeof(CANCARD_MSG);

   int err = DoIoCtl( local, EICAN_IOCTL_SENDCAN, can, sizeof(CANCARD_MSG), &tot, can->timeout );
   if( err ) return err;

   if( tot == 4 )
      err = *(int32_t *)can;
   return err;
}

#else

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

typedef struct
{
   int fd;
} DriverLocal;

// Allocate a structure to hold operating system specific local data
static void *_AllocLocals( void )
{
   return malloc( sizeof(DriverLocal));
}

static void _FreeLocals( void *lcl )
{
   free( lcl );
}

static void ScanPorts( void )
{
   // Depending on the driver used, the device files could have different names.
   const char *cardNames[] = { "eican", "copleycan", 0 };
   int i, p, fd;

   // Only scan once
   if( totPorts >= 0 ) return;
   totPorts = 0;

   for( i=0; cardNames[i]; i++ )
   {
      for( p=0; totPorts<MAX_PORTS_TOTAL; p++ )
      {
         snprintf( portNames[totPorts], MAX_PORT_NAME, "/dev/%s%02d", cardNames[i], p );

         fd = open( portNames[totPorts], O_RDWR );
         if( fd < 0 ) break;
         close(fd);
         totPorts++;
      }
   }
}

// Open the specified driver and initialize the local data structure 
// as necessary.
static int _OpenFile( int port, void *lcl )
{
   DriverLocal *local = (DriverLocal *)lcl;

   ScanPorts();

   if( port >= totPorts ) 
      return EICAN_ERR_CANT_OPEN_PORT;

   // Convert the name into a device name
   local->fd = open( portNames[port], O_RDWR );
   if( local->fd < 0 )
      return EICAN_ERR_CANT_OPEN_PORT;
   return 0;
}

static int _CloseFile( void *lcl )
{
   DriverLocal *local = (DriverLocal *)lcl;
   close(local->fd);
   return 0;
}

/**
 * Write a command to the card
 */
static int _SendCMD( void *lcl, uint32_t *cmd )
{
   DriverLocal *local = (DriverLocal *)lcl;
   int err = ioctl( local->fd, EICAN_IOCTL_CMD, cmd );
   if( !err ) err = cmd[0]>>16;
   return err;
}

static int _RecvMsg( void *lcl, CANCARD_MSG *can )
{
   DriverLocal *local = (DriverLocal *)lcl;
   return ioctl( local->fd, EICAN_IOCTL_RECVCAN, can );
}

static int _SendMsg( void *lcl, CANCARD_MSG *can )
{
   DriverLocal *local = (DriverLocal *)lcl;
   return ioctl( local->fd, EICAN_IOCTL_SENDCAN, can );
}

#endif

