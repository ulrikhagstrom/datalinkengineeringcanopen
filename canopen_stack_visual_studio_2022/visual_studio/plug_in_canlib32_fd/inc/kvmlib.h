/*
 *                   Copyright 2015 by KVASER AB, SWEDEN
 *                        WWW: http://www.kvaser.com
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license.
 */

#ifndef KVMLIB_H
#define KVMLIB_H

/**
 * \file kvmlib.h
 * \brief   Library for accessing Kvaser Memorator (2nd generation) devices.
 * \details
 *    This library is used to extract log data, initialize disk, read and
 *    write configuration to a device, handle on device databases and
 *    more.
 * \defgroup grp_kvm  kvmlib
 * \brief Library for accessing Kvaser Memorator (2nd generation) devices
 * \details
 *    This library is used to extract log data, initialize disk, read and
 *    write configuration to a device, handle on device databases and
 *    more.
 *
 * \defgroup kvm_initialization   Initialization
 * \ingroup grp_kvm
 * \defgroup kvm_connection       Device Connection
 * \ingroup grp_kvm
 * \defgroup kvm_disk_operations  Disk Operations
 * \ingroup grp_kvm
 * \defgroup kvm_data_extraction  Log File Operations
 * \ingroup grp_kvm
 * \defgroup kvm_system_information  System Information
 * \ingroup grp_kvm
 * \defgroup kvm_rtc              Real Time Clock
 * \ingroup grp_kvm
 * \defgroup kvm_configuration    Configuration
 * \ingroup grp_kvm
 * \defgroup kvm_database         Databases
 * \ingroup grp_kvm
 * \defgroup kvm_files            File Operations
 * \ingroup grp_kvm
 */

#include <windows.h>
#include <stdint.h>
#include <stdio.h>

typedef int8_t      int8;
typedef uint8_t     uint8;
typedef int16_t     int16;
typedef uint16_t    uint16;
typedef int32_t     int32;
typedef uint32_t    uint32;
typedef int64_t     int64;


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name kvmDEVICE_xxx
 * \anchor kvmDEVICE_xxx
 *
 * Device type, used to connect to a Memorator device.
 *
 * @{
 */
#define kvmDEVICE_MHYDRA     0      ///< Kvaser Memorator (2nd generation)
#define kvmDEVICE_MHYDRA_EXT 1      ///< Kvaser Memorator (2nd generation) with extended data capabilities.
/** @} */

/**
 * \name kvmLDF_MAJOR_xxx
 * \anchor kvmLDF_MAJOR_xxx
 *
 * Logged data format (LDF) version.
 *
 * @{
 */
#define kvmLDF_MAJOR_CAN     3  ///< Used in Kvaser Memorator (2nd generation)
#define kvmLDF_MAJOR_CAN64   5  ///< Used in Kvaser Memorator (2nd generation) with extended data capabilities.
/** @} */

/**
 * \brief A handle to a KME file
 */
typedef HANDLE kmeFileHandle;

/**
 * \name kvmFS_xxx
 * \anchor kvmFS_xxx
 *
 * File system used when formatting disk.
 *
 * @{
 */
#define kvmFS_FAT16   0      ///< fat16
#define kvmFS_FAT32   1      ///< fat32
/** @} */

/**
 * \name kvmFILE_xxx
 * \anchor kvmFILE_xxx
 *
 * KME file type, a binary file format representing log data.
 *
 * @{
 */
#define kvmFILE_KME24   0      ///< Deprecated
#define kvmFILE_KME25   1      ///< Deprecated
#define kvmFILE_KME40   2      ///< Kvaser binary format (KME 4.0)
#define kvmFILE_KME50   3      ///< Kvaser binary format (KME 5.0)
#define kvmFILE_KME60   4      ///< Kvaser binary format (KME 6.0) (Experimental)
/** @} */

/**
 * \brief A handle to a Memorator or equivalent KMF file
 */
typedef HANDLE kvmHandle;


/**
 * \name kvmStatus
 * \anchor kvmERR_xxx
 *
 * Generally, a return code greater than or equal to zero means success. A
 * value less than zero means failure.
 *
 * @{
 */

  // If you change here, remember to also change kvmlib.cpp and kvmlibtest...
typedef enum {
  kvmOK                      =  0,  ///< OK!
  kvmFail                    = -1,  ///< Generic error.
  kvmERR_PARAM               = -3,  ///< Error in supplied parameters.
  kvmERR_LOGFILEOPEN         = -8,  ///< Can't find/open log file.
  kvmERR_NOSTARTTIME         = -9,  ///< Start time not found.
  kvmERR_NOLOGMSG            = -10,  ///< No log message found.
  kvmERR_LOGFILEWRITE        = -11,  ///< Error writing log file.
  kvmEOF                     = -12,  ///< End of file found.
  kvmERR_NO_DISK             = -13,  ///< No disk found.
  kvmERR_LOGFILEREAD         = -14,  ///< Error while reading log file.
  kvmERR_QUEUE_FULL          = -20,  ///< Queue is full.
  kvmERR_CRC_ERROR           = -21,  ///< CRC check failed.
  kvmERR_SECTOR_ERASED       = -22,  ///< Sector unexpectadly erased.
  kvmERR_FILE_ERROR          = -23,  ///< File I/O error.
  kvmERR_DISK_ERROR          = -24,  ///< General disk error.
  kvmERR_DISKFULL_DIR        = -25,  ///< Disk full (directory).
  kvmERR_DISKFULL_DATA       = -26,  ///< Disk full (data).
  kvmERR_SEQ_ERROR           = -27,  ///< Unexpected sequence.
  kvmERR_FILE_SYSTEM_CORRUPT = -28,  ///< File system corrupt.
  kvmERR_UNSUPPORTED_VERSION = -29,  ///< Unsupported version.
  kvmERR_NOT_IMPLEMENTED     = -30,  ///< Not implemented.
  kvmERR_FATAL_ERROR         = -31,  ///< Fatal error.
  kvmERR_ILLEGAL_REQUEST     = -32,  ///< Illegal request.
  kvmERR_FILE_NOT_FOUND      = -33,  ///< File not found.
  kvmERR_NOT_FORMATTED       = -34,  ///< Disk not formatted.
  kvmERR_WRONG_DISK_TYPE     = -35,  ///< Wrong disk type.
  kvmERR_TIMEOUT             = -36,  ///< Timeout.
  kvmERR_DEVICE_COMM_ERROR   = -37,  ///< Device communication error.
  kvmERR_OCCUPIED            = -38,  ///< Device occupied.
  kvmERR_USER_CANCEL         = -39,  ///< User abort.
  kvmERR_FIRMWARE            = -40,  ///< Firmware error.
  kvmERR_CONFIG_ERROR        = -41,  ///< Configuration error.
  kvmERR_WRITE_PROT          = -42,  ///< Disk is write protected.
  kvmERR_RESULT_TOO_BIG      = -43,  ///< Result is too big for an out-parameter.

} kvmStatus;
/** @} */

/**
 * \name kvmLogFileType_xxx
 * \anchor kvmLogFileType_xxx
 *
 * Type of log file.
 *
 * @{
 */
#define kvmLogFileType_ERR   0  ///< a log file containing only error frames, and some frames before and after the error frame.
#define kvmLogFileType_ALL   1  ///< a log file with all frames.
/** @} */

/**
 * \name kvm_SWINFO_xxx
 * \anchor kvm_SWINFO_xxx
 *
 * Different types of version information that can be extracted using
 * kvmDeviceGetSoftwareInfo()
 *
 * @{
 */
#define kvm_SWINFO_KVMLIB                   1  ///< Returns the version of kvmlib.
#define kvm_SWINFO_DRIVER                   2  ///< Returns the used driver version information.
#define kvm_SWINFO_FIRMWARE                 3  ///< Returns the device firmware version information.
#define kvm_SWINFO_DRIVER_PRODUCT           4  ///< Obsolete. Returns the product version information.
#define kvm_SWINFO_CONFIG_VERSION_NEEDED    5  ///< Returns the version of the binary format the device requires (param.lif).
#define kvm_SWINFO_CPLD_VERSION             6  ///< Obsolete.
/** @} */

#include <pshpack1.h>

#ifndef canMSG_RTR
/**
 * \name canMSG_xxx
 *
 * The following flags can be found in a kvmLogMsgEx message flags field
 * returned from kvmKmeReadEvent(). All flags and/or combinations of them are
 * meaningful for logged message.
 * \anchor canMSG_xxx
 * @{
 */
#  define canMSG_RTR            0x0001      ///< Message is a remote request
#  define canMSG_STD            0x0002      ///< Message has a standard ID
#  define canMSG_EXT            0x0004      ///< Message has an extended ID
#  define canMSG_ERROR_FRAME    0x0020      ///< Message is an error frame
#  define canMSG_TXACK          0x0040      ///< Message is a TX ACK (msg is really sent)
#  define canMSG_TXRQ           0x0080      ///< Message is a TX REQUEST (msg is transfered to the CAN controller chip)
#  define canMSGERR_OVERRUN     0x0600      ///< Message overrun condition occurred.
#  define canFDMSG_EDL          0x010000    ///< Obsolete, use MSGFLAG_FDF instead
#  define canFDMSG_FDF          0x010000    ///< Message is an FD message (CAN FD)
#  define canFDMSG_BRS          0x020000    ///< Message is sent/received with bit rate switch (CAN FD)
#  define canFDMSG_ESI          0x040000    ///< Sender of the message is in error passive mode (CAN FD)
/** @} */
#endif

/**
 * \name TRIGVAR_TYPE_xxx
 *
 * The following trigger types can be found in a kvmLogTriggerEx message type
 * field.
 * \anchor TRIGVAR_TYPE_xxx
 * @{
 */
#define TRIGVAR_TYPE_MSG_ID         0       ///< Message ID trigger
#define TRIGVAR_TYPE_MSG_DLC        1       ///< Message DLC trigger
#define TRIGVAR_TYPE_MSG_FLAG       2       ///< Message flag trigger
#define TRIGVAR_TYPE_SIGVAL         3       ///< Signal value trigger
#define TRIGVAR_TYPE_EXTERNAL       4       ///< External trigger
#define TRIGVAR_TYPE_TIMER          5       ///< Timer trigger
#define TRIGVAR_TYPE_DISK_FULL      6       ///< Disk is full trigger
#define TRIGVAR_TYPE_STARTUP        9       ///< Startup trigger
/** @} */

/**
 * \brief A CAN message
 */
typedef struct {
  uint32 id;            ///< The message identifier
  int64  timeStamp;     ///< The timestamp in units of 1 nanosecond
  uint32 channel;       ///< The device channel on which the message arrived, 0,1,...
  uint32 dlc;           ///< The length of the message
  uint32 flags;         ///< Message flags \ref canMSG_xxx
  uint8  data[64];      ///< Message data (64 bytes)
} kvmLogMsgEx;

/**
 * \brief A RTC clock message
 */
typedef struct {
  uint32 calendarTime;   ///< RTC date, seconds since 1970-01-01T00:00:00+00:00 (UTC)
  int64  timeStamp;      ///< The timestamp in units of 1 nanosecond
} kvmLogRtcClockEx;


/**
 * \brief A trigger message
 */
typedef struct {
  int32   type;           ///< The type of trigger \ref TRIGVAR_TYPE_xxx
  int32   preTrigger;     ///< Pretrigger time in milliseconds
  int32   postTrigger;    ///< Posttrigger time in milliseconds
  uint32  trigMask;       ///< Bitmask with all active triggers
  int64   timeStamp;      ///< The timestamp in units of 1 nanosecond
} kvmLogTriggerEx;

/**
 * \brief A version message
 */
typedef struct
{
  uint32 lioMajor;     // Lio major version
  uint32 lioMinor;     // Lio minor version
  uint32 fwMajor;      // Firmware major version
  uint32 fwMinor;      // Firmware major version
  uint32 fwBuild;      // Firmware build version
  uint32 serialNumber; // Serial number
  uint32 eanHi;        // EAN high bytes
  uint32 eanLo;        // EAN low bytes
} kvmLogVersionEx;


/**
 * \name kvmLOG_TYPE_xxx
 * \anchor kvmLOG_TYPE_xxx
 *
 * Event types in log
 * @{
 */
#define kvmLOG_TYPE_INVALID    0 ///< Invalid MEMOLOG type
#define kvmLOG_TYPE_CLOCK      1 ///< The type used in kvmLogRtcClockEx
#define kvmLOG_TYPE_MSG        2 ///< The type used in kvmLogMsgEx
#define kvmLOG_TYPE_TRIGGER    3 ///< The type used in kvmLogTriggerEx
#define kvmLOG_TYPE_VERSION    4 ///< The type used in kvmLogVersionEx
/** @} */

/**
 * \brief The union of events used by kvmKmeReadEvent().
 */
typedef struct {
  uint32               type;    ///< \ref kvmLOG_TYPE_xxx, Event types in log
  union {
    kvmLogMsgEx        msg;     ///< A CAN message
    kvmLogRtcClockEx   rtc;     ///< An RTC message
    kvmLogTriggerEx    trig;    ///< A trigger message
    kvmLogVersionEx    ver;     ///< A version message
    uint8              raw[128]; ///< Raw data in a array
  } eventUnion;
} kvmLogEventEx;

#include <poppack.h>

/**
 * \ingroup kvm_initialization
 *
 * \source_cs       <b>static void Initialize();</b>
 * \source_end
 *
 * This function must be called before any other functions are used.  It will
 * initialize the memolib library.
 */
void WINAPI kvmInitialize(void);

/**
 * \ingroup kvm_initialization
 *
 * \source_cs       <b>static Kvmlib.STATUS GetVersion(out Int32 major, out Int32 minor, out Int32 build);</b>
 * \source_end
 *
 * This function returns the version of the KVMLIB API DLL (kvmlib.dll).
 *
 * \param[out] major          Major version number.
 * \param[out] minor          Minor version number.
 * \param[out] build          Build number.
 *
 * \return version number of kvmlib.dll
 *
 */
kvmStatus WINAPI kvmGetVersion(int *major, int *minor, int *build);

/**
 * \ingroup kvm_initialization
 *
 * \source_cs       <b>static Kvmlib.STATUS GetErrorText(Status error, out String buf);</b>
 * \source_end
 *
 * Convert a \ref kvmStatus errorcode to a text.
 *
 * \param[in]  error         The error code to convert.
 * \param[out] buf           Buffer to receive error text.
 * \param[in]  len           Buffer size in bytes.
 */
kvmStatus WINAPI kvmGetErrorText(kvmStatus error, char *buf, size_t len);

/**
 * \ingroup kvm_connection
 *
 * \source_cs       <b>static Kvmlib.STATUS Close(Handle h);</b>
 * \source_end
 *
 * Close the connection to the Memorator (device or file) opened with
 * kvmDeviceOpen() or kvmDeviceMountKmf(). The handle becomes invalid.
 *
 * \param[in]  h         An open kvmHandle.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 * \sa kvmDeviceMountKmf(), kvmDeviceOpen(), kvmDeviceMountKmfEx()
 */
kvmStatus WINAPI kvmClose(kvmHandle h);

/**
 *\ingroup kvm_connection
 *
 * \source_cs       <b>static Kvmlib.Handle DeviceOpen(Int32 cardNr, out Status status, DeviceType deviceType);</b>
 * \source_end
 *
 * Connect to a Memorator device and obtain a handle for subsequent device operations.
 * The argument \a cardNr is the Card Number property (decreased by one) displayed in
 * Kvaser Hardware.
 *
 * \param[in]  cardNr      Card number
 * \param[out] status      \ref kvmOK if completely successful,
 *                         \ref kvmERR_xxx (negative) if failure
 * \param[in]  deviceType  \ref kvmDEVICE_xxx
 *
 * \return Returns an open handle to a Memorator on success.
 *
 * \sa kvmClose(), kvmLogFileMountEx()
 */
kvmHandle    WINAPI kvmDeviceOpen(int32 cardNr,
                                     kvmStatus *status,
                                     int32 deviceType);

/**
 *\ingroup kvm_connection
 *
 * \source_cs       <b>static Kvmlib.STATUS DeviceMountKmf(Handle h);</b>
 * \source_end
 *
 * Mount the log area on the SD card on a connected Kvaser Memorator.
 *
 * \note Must be called after \ref kvmDeviceOpen before any subsequent
 * log operations are called.
 *
 * param[in]  h         An open kvmHandle.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 * kvmClose(), kvmDeviceOpen()
 */
kvmStatus WINAPI kvmDeviceMountKmf(kvmHandle h);

/**
 *\ingroup kvm_connection
 *
 * \source_cs       <b>static Kvmlib.STATUS DeviceMountKmfEx(Handle h, out Int32 ldfMajor, out Int32 ldfMinor);</b>
 * \source_end
 *
 * Mount the log area on the SD card on a connected Kvaser Memorator and
 * return the logger data format (LDF) version.
 *
 * \note Must be called after \ref kvmDeviceOpen before any subsequent
 * log operations are called.
 *
 * param[in]   h          An open kvmHandle.
 * param[out]  ldfMajor   Major LDF version \ref kvmLDF_MAJOR_xxx.
 * param[out]  ldfMinor   Minor LDF version.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 * \sa kvmClose(), kvmDeviceOpen()
 */
kvmStatus WINAPI kvmDeviceMountKmfEx(kvmHandle h, int *ldfMajor, int *ldfMinor);

/**
 * \ingroup kvm_connection
 *
 * \source_cs       <b>static Kvmlib.Handle KmfOpen(String filename, out Status status, DeviceType deviceType);</b>
 * \source_end
 *
 * Open a KMF file on a hard disk or SD card reader and obtain a handle for
 * subsequent operations. \a deviceType is the device type that generated the
 * file.
 *
 * \note Only the path of the argument filename is used. This function will
 * then look for all relevantly named files (e.g. LOG00000.KMF,
 * LOG00001.KMF...) in the directory specified by this path.
 *
 * \param[in]  filename    KMF filename
 * \param[out] status      \ref kvmOK if successful, otherwise
 *                         \ref kvmERR_xxx
 * \param[in]  deviceType  \ref kvmDEVICE_xxx
 *
 * \return Returns an open handle to a Memorator on success, otherwise
 * INVALID_HANDLE_VALUE
 *
 * \sa kvmClose(), kvmDeviceOpen()
 */
kvmHandle    WINAPI kvmKmfOpen(const char *filename,
                                 kvmStatus *status,
                                 int32 deviceType);

/**
 * \ingroup kvm_connection
 *
 * \source_cs       <b>static Kvmlib.Handle KmfOpenEx(String filename, out Status status, DeviceType deviceType, out Int32 ldfMajor, out Int32 ldfMinor);</b>
 * \source_end
 *
 * Open a KMF file on a hard disk or SD card reader and obtain a handle for
 * subsequent operations and return the logger data format (LDF) version.
 * \a deviceType is the device type that generated the file.
 *
 * \note Only the path of the argument filename is used. This function will
 * then look for all relevantly named files (e.g. LOG00000.KMF,
 * LOG00001.KMF...) in the directory specified by this path.
 *
 * \param[in]  filename    KMF filename
 * \param[out] status      \ref kvmOK if successful, otherwise
 *                         \ref kvmERR_xxx
 * \param[in]  deviceType  \ref kvmDEVICE_xxx
 * \param[out] ldfMajor   Major LDF version \ref kvmLDF_MAJOR_xxx.
 * \param[out] ldfMinor   Minor LDF version.

 *
 * \return Returns an open handle to a Memorator on success, otherwise
 * INVALID_HANDLE_VALUE
 *
 * \sa kvmClose(), kvmDeviceOpen()
 */
kvmHandle    WINAPI kvmKmfOpenEx(const char *filename,
                                 kvmStatus *status,
                                 int32 deviceType,
                                 int *ldfMajor,
                                 int *ldfMinor);

/**
 * \ingroup kvm_disk_operations
 *
 * \source_cs       <b>static Kvmlib.STATUS KmfValidate(Handle h);</b>
 * \source_end
 *
 * Check for errors
 *
 * \param[in]  h         An open kvmHandle.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmKmfValidate(kvmHandle h);

/**
 * \ingroup kvm_disk_operations
 *
 * \source_cs       <b>static Kvmlib.STATUS DeviceFormatDisk(Handle h, Int32 filesystem, Int32 reserveSpace, Int32 dbaseSpace);</b>
 * \source_end
 *
 * Format the SD memory card in a connected Memorator.
 *
 *


An item code specifying the type of version to get. \ref kvm_SWINFO_xxx

 * \param[in]  h             An open kvmHandle.
 * \param[in]  fileSystem    A filesystem code, \ref kvmFS_xxx, specifying the type of filesystem to format to.
 * \param[in]  reserveSpace  Space to reserve for user files, in MB.
 * \param[in]  dbaseSpace    Space to reserve for database files, in MB.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 * \sa kvmDeviceDiskSize
 */
kvmStatus WINAPI kvmDeviceFormatDisk(kvmHandle h,
                                   int fileSystem,
                                   uint32 reserveSpace,
                                   uint32 dbaseSpace);

/**
 * \ingroup kvm_data_extraction
 *
 * \source_cs       <b>static Kvmlib.STATUS LogFileGetCount(Kvmlib.Handle h, out Int32 fileCount);</b>
 * \source_end
 *
 * Count the number of log files
 *
 * \param[in]  h             An open kvmHandle.
 * \param[out] fileCount     The number of log files on disk.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmLogFileGetCount(kvmHandle h, uint32 *fileCount);

/**
 * \ingroup kvm_data_extraction
 *
 * \source_cs       <b>static Kvmlib.STATUS LogFileGetType(Kvmlib.Handle h, Int32 fileIndx, out LogFileType logFileType);</b>
 * \source_end
 *
 * Get type of log file.
 *
 * \param[in]  h             An open kvmHandle.
 * \param[in]  fileIndx      Index of the log file.
 * \param[out] logFileType   \ref kvmLogFileType_xxx
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmLogFileGetType(kvmHandle h, uint32 fileIndx, int32 *logFileType);

/**
 * \ingroup kvm_data_extraction
 *
 * \source_cs       <b>static Kvmlib.STATUS LogFileMount(Kvmlib.Handle h, Int32 fileIndx, out Int32 eventCount);</b>
 * \source_end
 *
 * \deprecated Use \ref kvmLogFileMountEx instead.
 *
 * Mount the log file with the specified index. The index starts at 0. The
 * approximate number of events in the log file is returned.
 *
 * \param[in]  h             An open kvmHandle.
 * \param[in]  fileIndx      Index of the log file to open.
 * \param[out] eventCount    The approximate number of events in the log file
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 * \note if return value is \ref kvmERR_RESULT_TOO_BIG,
 * use \ref kvmLogFileMountEx instead.
 *
 * \sa kvmLogFileDismount()
 */
kvmStatus WINAPI kvmLogFileMount(kvmHandle h, uint32 fileIndx,
                                    uint32 *eventCount);

/**
 * \ingroup kvm_data_extraction
 *
 * \source_cs       <b>static Kvmlib.STATUS LogFileMountEx(Kvmlib.Handle h, Int32 fileIndx, out Int64 eventCount);</b>
 * \source_end
 *
 * Mount the log file with the specified index. The index starts at 0. The
 * approximate number of events in the log file is returned.
 *
 * \param[in]  h             An open kvmHandle.
 * \param[in]  fileIndx      Index of the log file to open.
 * \param[out] eventCount    The approximate number of events in the log file
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 * \sa kvmLogFileDismount()
 */
kvmStatus WINAPI kvmLogFileMountEx(kvmHandle h, uint32 fileIndx,
                                    int64 *eventCount);

/**
 * \ingroup kvm_data_extraction
 *
 * \source_cs       <b>static Kvmlib.STATUS LogFileDismount(Kvmlib.Handle h);</b>
 * \source_end
 *
 * Dismount the log file opened with kvmLogFileMountEx().
 * The handle will stay valid.
 *
 * \param[in]  h             An open kvmHandle.
 */
kvmStatus WINAPI kvmLogFileDismount(kvmHandle h);

/**
 * \ingroup kvm_data_extraction
 *
 * \source_cs       <b>static Kvmlib.STATUS LogFileGetStartTime(Kvmlib.Handle h, out Int32 startTime);</b>
 * \source_end
 *
 * Get the time of the first event in the log file. The time is returned in
 * standard unix time format (number of seconds since 1970-01-01T00:00:00+00:00).
 *
 * \param[in]   h            An open kvmHandle.
 * \param[out]  startTime    The time of the first event in the log file (UTC)
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmLogFileGetStartTime (kvmHandle h,
                                         uint32 *startTime);

/**
 * \ingroup kvm_data_extraction
 *
 * \source_cs       <b>static Kvmlib.STATUS LogFileGetEndTime(Kvmlib.Handle h, out Int32 endTime);</b>
 * \source_end
 *
 * Get the time of the first event in the log file. The time is returned in
 * standard unix time format (number of seconds since 1970-01-01T00:00:00+00:00).
 *
 * \param[in]   h            An open kvmHandle.
 * \param[out]  endTime      The time of the last event in the log file (UTC)
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmLogFileGetEndTime (kvmHandle h,
                                       uint32 *endTime);

/**
 * \ingroup kvm_data_extraction
 *
 * \source_cs       <b>static Kvmlib.STATUS LogFileGetCreatorSerial(Kvmlib.Handle h, out Int32 serialNumber);</b>
 * \source_end
 *
 * Get the serialnumber of the interface that created the log file.
 *
 * \param[in]   h              An open kvmHandle.
 * \param[out]  serialNumber   The serialnumber of the interface that created the
 *                             log file.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmLogFileGetCreatorSerial(kvmHandle h,
                                         uint32 *serialNumber);

/**
 * \ingroup kvm_data_extraction
 *
 * \source_cs       <b>static Kvmlib.STATUS LogFileReadEvent(Kvmlib.Handle h, out Log e);</b>
 * \source_end
 *
 * Read an event from a log file opened with kvmLogFileMountEx(). The next call to
 * kvmLogFileReadEvent() will read the next event.
 *
 * \param[in]   h              An open kvmHandle.
 * \param[out]  e              Event from log file.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 */
kvmStatus WINAPI kvmLogFileReadEvent(kvmHandle h, kvmLogEventEx *e);


/**
 * \ingroup kvm_data_extraction
 *
 * \source_cs       <b>static Kvmlib.STATUS LogFileDeleteAll(Kvmlib.Handle h);</b>
 * \source_end
 *
 * Delete all log files from a Memorator.
 *
 * \param[in]  h         An open kvmHandle.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmLogFileDeleteAll(kvmHandle h);

/**
 * \ingroup kvm_sytem_information
 *
 * \source_cs       <b>static Kvmlib.STATUS DeviceDiskStatus(Handle h, out Int32 present);</b>
 * \source_end
 *
 * Check if the SD memory card is present.
 *
 * \note This function is not supported by all devices.
 *
 * \param[in]   h              An open kvmHandle.
 * \param[out]  present        Non-zero means that SD memory card is present.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmDeviceDiskStatus(kvmHandle h, int *present);

/**
 * \ingroup kvm_system_information
 *
 * \source_cs       <b>static Kvmlib.STATUS KmfGetUsage(Handle h, out Int32 totalSectorCount, out Int32 usedSectorCount);</b>
 * \source_end
 *
 * Get disk usage statistics, reported in number of (512 byte) sectors.
 *
 *
 * \param[in]   h                 An open kvmHandle.
 * \param[out]  totalSectorCount  Total number of sectors devoted for logging
 * \param[out]  usedSectorCount   Number of logging sectors used
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmKmfGetUsage(kvmHandle h,
                                           uint32 *totalSectorCount,
                                           uint32 *usedSectorCount);

/**
 * \ingroup kvm_system_information
 *
 * \source_cs       <b>static Kvmlib.STATUS DeviceDiskSize(Handle h, out Int32 diskSize);</b>
 * \source_end
 *
 * Get disk size, reported in number of (512 byte) sectors.
 *
 *
 * \param[in]   h              An open kvmHandle.
 * \param[out]  diskSize       Disk size in number of (512 byte) sectors.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmDeviceDiskSize(kvmHandle h, uint32 *diskSize);

/**
 * \ingroup kvm_system_information
 *
 * \source_cs       <b>static Kvmlib.STATUS DeviceGetSerialNumber(Handle h, out Int32 serial);</b>
 * \source_end
 *
 * Get serial number related to the Memorator handle.
 *
 * \param[in]   h            An open kvmHandle.
 * \param[out]  serial       Serial number of connected device.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmDeviceGetSerialNumber(kvmHandle h, unsigned int *serial);

/**
 * \ingroup kvm_system_information
 *
 * \source_cs       <b>static Kvmlib.STATUS DeviceGetSoftwareInfo(Handle h, VersionInfo itemCode, out Int32 major, out Int32 minor, out Int32 build, out Int32 flags);</b>
 * \source_end
 *
 * Get software version information.
 *
 * \param[in]   h            An open kvmHandle.
 * \param[in]   itemCode     An item code specifying the type of version to get. \ref kvm_SWINFO_xxx
 * \param[out]  major        Major version number
 * \param[out]  minor        Minor version number
 * \param[out]  build        Build number
 * \param[out]  flags        For internal use only
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmDeviceGetSoftwareInfo(kvmHandle h,
                                               int32 itemCode,
                                               unsigned int *major,
                                               unsigned int *minor,
                                               unsigned int *build,
                                               unsigned int *flags);
/**
 * \ingroup kvm_system_information
 *
 * \source_cs       <b>static Kvmlib.STATUS DeviceFlashLeds(Handle h);</b>
 * \source_end
 *
 * Flash all LEDs on the opened Memorator device
 *
 * \param[in]   h            An open kvmHandle.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmDeviceFlashLeds(kvmHandle h);


/**
 * \ingroup kvm_rtc
 *
 * \source_cs       <b>static Kvmlib.STATUS DeviceGetRTC(Handle h, out Int32 t);</b>
 * \source_end
 *
 * Get date and time from the RTC chip.  The time is returned in standard
 * unix time (number of seconds since 1970-01-01T00:00:00+00:00).
 * Only for device handles.
 *
 * \param[in]   h            An open kvmHandle.
 * \param[out]  t            Time in Unix time.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmDeviceGetRTC(kvmHandle h, uint32 *t);

/**
 * \ingroup kvm_rtc
 *
 * \source_cs       <b>static Kvmlib.STATUS DeviceSetRTC(Handle h, Int32 t);</b>
 * \source_end
 *
 * Set date and time in the RTC. The time is returned in standard Unix
 * time (number of seconds since 1970-01-01T00:00:00+00:00).
 * Only for device handles.
 *
 * \param[in]   h            An open kvmHandle.
 * \param[in]   t            Time in Unix time.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmDeviceSetRTC(kvmHandle h, uint32 t);

/**
 * \ingroup kvm_configuration
 *
 * \source_cs       <b>static Kvmlib.STATUS KmfReadConfig(Handle h, out Byte[] buf);</b>
 * \source_end
 *
 * Read binary configuration data (param.lif) from a KMF file.
 *
 * \param[in]   h            An open kvmHandle.
 * \param[out]  buf          A pointer to buffer where the configuration (param.lif) will be written.
 * \param[in]   buflen       The length of the buffer buf.
 * \param[out]  actual_len   The actual length of the configuration written to buf.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmKmfReadConfig(kvmHandle h, void *buf,
                                    size_t buflen, size_t *actual_len);

/**
 * \ingroup kvm_configuration
 *
 * \source_cs       <b>static Kvmlib.STATUS KmfWriteConfig(Handle h, Byte[] buf);</b>
 * \source_end
 *
 * Write binary configuration data (param.lif) to a KMF file.
 *
 * \param[in]   h            An open kvmHandle.
 * \param[in]   buf          A pointer to buffer containing the configuration (param.lif) to be written.
 * \param[in]   buflen       The length of the buffer buf.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmKmfWriteConfig(kvmHandle h, void *buf, size_t buflen);


/**
 * \ingroup kvm_database
 *
 * \source_cs       <b>static Kvmlib.STATUS KmfGetDbaseFile(Handle h, String path, out String filenamebuf);</b>
 * \source_end
 *
 * Read the database file. The database will be extracted to path and the
 * name of the created file copied to filenamebuf.
 *
 * \param[in]   h            An open kvmHandle.
 * \param[in]   path         The path where the database file will be stored.
 * \param[out]  filenamebuf  The filename of the database. (should be greater then 12 bytes)
 * \param[in]   buflen       The lenght of filenamebuf
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 * \sa kvmKmfPutDbaseFile()
 */
kvmStatus WINAPI kvmKmfGetDbaseFile(kvmHandle h, char *path, char *filenamebuf, size_t buflen);

/**
 * \ingroup kvm_database
 *
 * \source_cs       <b>static Kvmlib.STATUS KmfPutDbaseFile(Handle h, String filename);</b>
 * \source_end
 *
 * Write the database file
 *
 * \param[in]   h            An open kvmHandle.
 * \param[in]   filename     The full path and name of the file, e.g. C:\\temp\\myfile.data
 *                           Note that the filename will be truncated to an 8.3 filename.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 * \sa kvmKmfGetDbaseFile()
 */
kvmStatus WINAPI kvmKmfPutDbaseFile(kvmHandle h, char *filename);

/**
 * \ingroup kvm_database
 *
 * \source_cs       <b>static Kvmlib.STATUS KmfEraseDbaseFile(Handle h, Int32 filenumber);</b>
 * \source_end
 *
 * Erase the database file.
 *
 * \param[in]   h            An open kvmHandle.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 */
kvmStatus WINAPI kvmKmfEraseDbaseFile(kvmHandle h);

/**
 * \ingroup kvm_files
 *
 * \source_cs       <b>static Kvmlib.Handle KmeOpenFile(String filename, out Status status, Int32 filetype);</b>
 * \source_end
 *
 * Open a KME file for reading and obtain a handle for subsequent operations.
 *
 * \param[in]   filename     The full path and name of the KME file, e.g. C:\\temp\\myfile.kme
 * \param[in]   fileType     \ref kvmFILE_xxx
 * \param[out]  status       \ref kvmOK (zero) if success
 *                           \ref kvmERR_xxx (negative) if failure
 *
 * \return Returns an open handle to a KME file on success.
 *
 * \sa kvmKmeReadEvent(), kvmKmeCountEventsEx(), kvmKmeCloseFile()
 *
 */
kmeFileHandle    WINAPI kvmKmeOpenFile (const char *filename,
                                    kvmStatus *status,
                                    int32 fileType);

/**
 * \ingroup kvm_files
 *
 * \source_cs       <b>static Kvmlib.STATUS KmeScanFileType(String filename, out Int32 filetype);</b>
 * \source_end
 *
 * Open a KME file and obtain the file format type.
 *
 * \param[in]   filename     The full path and name of the KME file, e.g. C:\\temp\\myfile.kme
 * \param[out]   fileType     \ref kvmFILE_xxx
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 * \sa kvmKmeOpenFile()
 *
 */
kvmStatus    WINAPI kvmKmeScanFileType (const char *filename,
                                        int32 *fileType);

/**
 * \ingroup kvm_files
 *
 * \source_cs       <b>static Kvmlib.Handle KmeCreateFile(String filename, out Status status, Int32 filetype);</b>
 * \source_end
 *
 * Open a KME file for writing and obtain a handle for subsequent operations.
 * Note that kvmKmeCreateFile() will overwrite any existing file and that
 * \ref kvmFILE_KME24 and \ref kvmFILE_KME25 are deprecated formats. Please
 * use \ref kvmFILE_KME40.
 *
 * \param[in]   filename     The full path and name of the KME file, e.g. C:\\temp\\myfile.kme
 * \param[in]   fileType     \ref kvmFILE_xxx
 * \param[out]  status       \ref kvmOK (zero) if success
 *                           \ref kvmERR_xxx (negative) if failure
 *
 * \return Returns an open handle to a KME file on success.
 *
 * \sa kvmKmeWriteEvent(), kvmKmeCountEventsEx(), kvmKmeCloseFile()
 *
 */
kmeFileHandle    WINAPI kvmKmeCreateFile (const char *filename,
                                      kvmStatus *status,
                                      int32 fileType);

/**
 * \ingroup kvm_files
 *
 * \source_cs       <b>static Kvmlib.STATUS KmeReadEvent(Handle h, out Log e);</b>
 * \source_end
 *
 * Read an event from a KME file opened with kvmKmeOpenFile().
 *
 * \param[in]   h              An open handle to a KME file.
 * \param[out]  e              Event from a KME file.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_NOLOGMSG on EOF
 * \return \ref kvmERR_xxx (negative) if failure
*
 * \sa kvmKmeOpenFile(), kvmKmeCountEventsEx(), kvmKmeCloseFile()
 *
 */
kvmStatus    WINAPI kvmKmeReadEvent (kmeFileHandle h, kvmLogEventEx *e);

/**
 * \ingroup kvm_files
 *
 * \source_cs       <b>static Kvmlib.STATUS KmeWriteEvent(Handle h, Log e);</b>
 * \source_end
 *
 * Write an event to a KME file created with kvmKmeCreateFile().
 *
 * \param[in]  h               An open handle to a KME file.
 * \param[in]  e               Event to write.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
*
 * \sa kvmKmeCreateFile(), kvmKmeCountEventsEx(), kvmKmeCloseFile()
 *
 */
kvmStatus    WINAPI kvmKmeWriteEvent (kmeFileHandle h, kvmLogEventEx *e);

/**
 * \ingroup kvm_files
 *
 * \source_cs       <b>static Kvmlib.STATUS KmeCountEvents(Handle h, out Int32 eventCount);</b>
 * \source_end
 *
 * \deprecated Use \ref kvmKmeCountEventsEx instead.
 *
 * Count the number of events in a KME file.
 *
 * \param[in]  h               An open handle to a KME file.
 * \param[out] eventCount      Approximate number of events in a KME file.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 * \note if return value is \ref kvmERR_RESULT_TOO_BIG,
 * use \ref kvmKmeCountEventsEx instead.
 *
 * \sa kvmKmeOpenFile(), kvmKmeCreateFile()
 *
 */
kvmStatus    WINAPI kvmKmeCountEvents(kmeFileHandle h, uint32 *eventCount);

/**
 * \ingroup kvm_files
 *
 * \source_cs       <b>static Kvmlib.STATUS KmeCountEventsEx(Handle h, out Int64 eventCount);</b>
 * \source_end
 *
 * Count the number of events in a KME file.
 *
 * \param[in]  h               An open handle to a KME file.
 * \param[out] eventCount      Approximate number of events in a KME file.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 * \sa kvmKmeOpenFile(), kvmKmeCreateFile()
 *
 */
kvmStatus    WINAPI kvmKmeCountEventsEx(kmeFileHandle h, int64 *eventCount);

/**
 * \ingroup kvm_files
 *
 * \source_cs       <b>static Kvmlib.STATUS KmeCloseFile(Handle h);</b>
 * \source_end
 *
 * Close an open KME file opened with kvmKmeOpenFile() or created
 * with kvmKmeCreateFile(). The handle becomes invalid.
 *
 * \param[in]  h               An open handle to a KME file.
 *
 * \return \ref kvmOK (zero) if success
 * \return \ref kvmERR_xxx (negative) if failure
 *
 * \sa kvmKmeOpenFile(), kvmKmeCreateFile()
 *
 */
kvmStatus    WINAPI kvmKmeCloseFile (kmeFileHandle h);

/**
 * \page page_kvmlib Memorator API (kvmlib)
 *
 *  \tableofcontents
 *
 * \section section_user_guide_kvmlib_1 Description
 *  The kvmlib is used to interact with Kvaser Memorator devices that can
 *  record CAN messages (E.g. Kvaser Memorator Professional 5xHS). You can
 *  download configuration data (e.g. triggers, filters, scripts) allowing you
 *  to disconnect the device from your computer, connect the device to a CAN bus
 *  and let it record the traffic autonomously. When done, you can reconnect the
 *  device with your computer and use kvmlib to get the recorded data.
 *
 * \section section_user_guide_kvmlib_2 Naming convention
 *   The function calls in the Kvaser Memorator API all have names starting with
 *   <b>kvm</b>, e.g \ref kvmDeviceOpen().
 *
 * \section section_user_guide_kvmlib_3 Build an application
 *     You will need to
 *     \code
 *       #include kvmlib.h
 *     \endcode
 *
 *     in your programs and link with kvmlib.dll on Windows and with libkvmlib.so on Linux.
 *
 *     \note
 *     On Windows kvmlib.dll is dependent on kvaMemoLib.dll, kvaMemoLib0600.dll and kvaMemoLib0700.dll.\n
 *     On Linux libkvmlib.so is dependent on libkvamemolib.so and libkvamemolib0700.so.
 *
 * \section section_user_guide_kvmlib_where_to_go_from_here Where to go from here
 * For more details, see the module \ref grp_kvm.
 */

#ifdef __cplusplus
}
#endif

#endif //KVMLIB_H
