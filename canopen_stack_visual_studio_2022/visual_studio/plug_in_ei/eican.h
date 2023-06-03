/**************************************************************
 * Public interface to the Embedded Intelligence CAN driver
 *************************************************************/
#ifndef _DEF_INC_EI_CAN
#define _DEF_INC_EI_CAN

// Define some generic data types.
#ifdef WIN32 
typedef unsigned short uint16_t;
typedef signed   short int16_t;
typedef unsigned long  uint32_t;
typedef signed   long  int32_t;
typedef unsigned char  uint8_t;
typedef signed   char  int8_t;
#else
#  include <stdint.h>
#endif

// Bit rate codes
#define EICAN_BITRATE_1000000       1
#define EICAN_BITRATE_800000        2
#define EICAN_BITRATE_500000        3
#define EICAN_BITRATE_250000        4
#define EICAN_BITRATE_125000        5
#define EICAN_BITRATE_100000        6
#define EICAN_BITRATE_50000         7
#define EICAN_BITRATE_20000         8

// Error codes which may be returned by these functions
#define EICAN_ERR_OK                0x00       // No error
#define EICAN_ERR_UNKNOWN_CMD       0x01       // Passed command ID was unknown
#define EICAN_ERR_BAD_PARAM         0x02       // Illegal parameter passed
#define EICAN_ERR_PORT_OPEN         0x03       // Specified CAN port is already open
#define EICAN_ERR_PORT_CLOSED       0x04       // Specified CAN port is not open
#define EICAN_ERR_CARD_BUSY         0x05       // Card command area is busy
#define EICAN_ERR_INTERNAL          0x06       // Some sort of internal device failure
#define EICAN_ERR_TIMEOUT           0x07       // Card failed to respond to command
#define EICAN_ERR_SIGNAL            0x08       // Signal received by driver
#define EICAN_ERR_MISSING_DATA      0x09       // Not enough data was passed
#define EICAN_ERR_CMDMUTEX_HELD     0x0A       // The command mutex was being held
#define EICAN_ERR_QUEUECTRL         0x0B       // The CAN message queue head/tail was invalid
#define EICAN_ERR_FLASH             0x0C       // Failed to erase/program flash memory
#define EICAN_ERR_NOTERASED         0x0D       // Attempt to write firmware before erasing flash
#define EICAN_ERR_FLASHFULL         0x0E       // Too much data sent when programming flash
#define EICAN_ERR_UNKNOWN_IOCTL     0x0F       // Specified IOCTL code was unknown
#define EICAN_ERR_CMD_TOO_SMALL     0x10       // Command passed without required header
#define EICAN_ERR_CMD_TOO_BIG       0x11       // Command passed with too much data
#define EICAN_ERR_CMD_IN_PROGRESS   0x12       // Command already in progress on card
#define EICAN_ERR_CAN_DATA_LENGTH   0x13       // More then 8 bytes of data sent with CAN message
#define EICAN_ERR_QUEUE_FULL        0x14       // Transmit queue is full
#define EICAN_ERR_QUEUE_EMPTY       0x15       // Receive queue is full
#define EICAN_ERR_READ_ONLY         0x16       // Parameter is read only
#define EICAN_ERR_MEMORYTEST        0x17       // Memory read/write test failure
#define EICAN_ERR_ALLOC             0x18       // Memory allocation failure
#define EICAN_ERR_CMDFINISHED       0x19       // Used internally by driver
#define EICAN_ERR_DRIVER            0x1A       // Generic device driver error
#define EICAN_ERR_CANT_OPEN_PORT    0x100      // Unable to obtain handle to device driver

// Types of CAN frames supported by the driver
#define CAN_FRAME_DATA                  0   /// Standard CAN data frame
#define CAN_FRAME_REMOTE                1   /// Remote frame
#define CAN_FRAME_ERROR                 2   /// Error frame - contains info about error

// Error bit-masks sent to host in error frames
#define EICAN_ERRBIT_STUFF       0x00000001
#define EICAN_ERRBIT_FORM        0x00000002
#define EICAN_ERRBIT_CRC         0x00000004
#define EICAN_ERRBIT_ACK         0x00000008
#define EICAN_ERRBIT_BIT0        0x00000010
#define EICAN_ERRBIT_BIT1        0x00000020
#define EICAN_ERRBIT_OVERFLOW    0x00010000

/**
Low level CAN data frame.
This class is used to represent the basic frame of information that is
passed over the CAN network.

A frame of CAN data consists of a message ID value (either 11 or 29 bits
depending on whether standard or extended frames are in use), 0 to 8 bytes
of data, and some special attributes.
*/
struct CanFrame
{
   /// Identifies the frame type.  
   uint8_t type;

   /// Gives the number of bytes of data included in the frame.
   /// The length of a frame may range from 0 to 8 bytes
   uint8_t length;

   /// The CAN message ID.
   /// If bit 29 is clear, this is a standard 11 bit ID (bits 0-10 hold the ID).
   /// If bit 29 is set, this is an extended 29 bit ID (bits 0-28 hold the ID).
   /// Bits 30 and 31 are not presently used.
   uint32_t id;

   /// On received messages, gives an accurate time stamp in microseconds
   /// of the message receive time
   uint32_t timestamp;

   /// Holds any data sent with the frame.  A frame may be
   /// accompanied by 0 to 8 bytes of data.
   uint8_t data[8];
};

// When a CAN frame with a type code indicating that it contains error information
// is received, you can cast it to this structure to more easily access the error
// details
struct CanErrorInfo
{
   /// Frame type, always CAN_FRAME_ERROR for error frames
   uint8_t type;

   /// Gives the number of bytes of additional information available
   uint8_t length;

   /// Error mask identifying what types of errors occurred
   uint32_t mask;

   /// Timestamp when the error was received
   uint32_t timestamp;

   /// Up to 8 additional bytes of data can be passed.  Currently only the first two 
   /// are used:
   ///  Transmit error count
   ///  Receive error count
   uint8_t data[8];
};

// This structure is used to return information about the card
struct CanCardInfo
{
   /// Card serial number
   uint32_t serial;   

   /// Hardware type info.  THe returned code identifies 
   /// the type of board
   uint32_t hwType;

   /// Firmare version info.  There are three pieces of firmware
   /// in the card; the boot loader (only used briefly at startup),
   /// the FPGA image, and the main firmware.
   /// Bits 0-7 contain a release number
   /// Bits 8-15 contain the minor version number
   /// Bits 16-23 contain the major version number
   /// Bits 24-31 are reserved.
   /// For example, version 1.02.07 would be encoded 0x00010207
   uint32_t fwVer;
   uint32_t bootVer;
   uint32_t fpgaVer;

   /// Driver version number, encoded as above
   uint32_t driverVer;
};

class EIcan
{
   void *local;
   int bps;
public:
   EIcan( void );
   ~EIcan();
   int Open( int port );
   int Close( void );

   int SetBitRate( int rate );

   int Online( void );
   int Offline( void );

   int Recv( CanFrame *frame, int32_t timeout );
   int Xmit( CanFrame *frame, int32_t timeout );
   int ReadCardInfo( CanCardInfo *info );
};

// Everything below this is really only for use by the API code itself.
// This ifdef just reduces the chances of clashing with other software.
#ifdef _INCLUDE_INTERNAL_DRIVER_INFO

/**************************************************************
 * The following structure is used to pass a CAN message 
 * frame to/from the CAN card.
 *************************************************************/
struct CANCARD_MSG
{
   int32_t  timeout;          // Reserved for use by driver
   uint32_t timestamp;        // Timestamp (microseconds) for receive messages
   uint32_t id;               // CAN message ID
   uint32_t flags;            // Various flags defined below
   uint8_t  data[8];          // CAN message data
};

// These values are used in the flags member of the CANCARD_MSG structure
#define CANFLG_LENGTH                0x0000000F   // Length of message data (bytes)
#define CANFLG_RTR                   0x00000010   // Set for RTR messages
#define CANFLG_SENDNOTIFY            0x00000020   // For transmits, notify on success
#define CANFLG_EXTENDED              0x00000040   // Set for extended messages
#define CANFLG_ERROR                 0x00000080   // Message contains error counter info

// The following command codes are currently defined.
#define EICAN_CMD_OPENPORT           0x01      // Open a CAN channel (no data)
#define EICAN_CMD_CLOSEPORT          0x02      // Close a CAN channel (no data)
#define EICAN_CMD_SETBPS             0x03      // Set bitrate info
#define EICAN_CMD_GETBPS             0x04      // Get bitrate info
#define EICAN_CMD_GETVERSION         0x05      // Get firmware version info
#define EICAN_CMD_SETPARAM           0x09      // Set a parameter
#define EICAN_CMD_GETPARAM           0x0A      // Get a parameter
#define EICAN_CMD_RECV_THRESH        0xF9      // Set the receive buffer interrupt threshold 
#define EICAN_CMD_DRIVERVER          0xFE      // Get driver version
#define EICAN_CMD_RESET              0xFF      // Reset the card

// CAN card parameter IDs
#define EICAN_PARAM_SERIAL               0
#define EICAN_PARAM_MFGINFO              1
#define EICAN_PARAM_STATUS               5
#define EICAN_PARAM_OPTIONS              6
#define EICAN_PARAM_INTINHIBIT           10

// I/O Ctrl codes
#if defined( WIN32 )
#  include <windows.h>
#  include <winioctl.h>
#  define EICAN_IOCTL_CMD           CTL_CODE( FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS )
#  define EICAN_IOCTL_SENDCAN       CTL_CODE( FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS )
#  define EICAN_IOCTL_RECVCAN       CTL_CODE( FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS )
#elif defined( __QNX__ )
#  include <devctl.h>
#  define EICAN_IOCTL_TYPE          0xB7
#  define EICAN_IOCTL_CMD           __DIOTF( EICAN_IOCTL_TYPE, 0x06, long[64]    )
#  define EICAN_IOCTL_SENDCAN       __DIOT ( EICAN_IOCTL_TYPE, 0x04, CANCARD_MSG )
#  define EICAN_IOCTL_RECVCAN       __DIOTF( EICAN_IOCTL_TYPE, 0x05, CANCARD_MSG )
#else
#  include <linux/ioctl.h>
#  define EICAN_IOCTL_TYPE          0xB7
#  define EICAN_IOCTL_SENDCAN       _IOW ( EICAN_IOCTL_TYPE, 0x04, CANCARD_MSG )
#  define EICAN_IOCTL_RECVCAN       _IOWR( EICAN_IOCTL_TYPE, 0x05, CANCARD_MSG )
#  define EICAN_IOCTL_CMD           _IOWR( EICAN_IOCTL_TYPE, 0x06, long        )
#endif
#endif

#endif

