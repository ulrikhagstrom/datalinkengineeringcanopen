/*
 *                   Copyright 2002-2017 by KVASER AB, SWEDEN
 *
 *                        WWW: http://www.kvaser.com
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license.
 *
 */

#ifndef KVADBLIB_H
#define KVADBLIB_H

/**
 * \file kvaDbLib.h
 * \brief Definitions for the Database API.
 * \details
 * \defgroup grp_kvadb  kvaDbLib
 * \brief Library for creating, reading, writing and modifying databases.
 *
 * \note Please note that allowed characters in identiers are digits, letters and "_" (underscore). <br>
 * Identifiers shall also start with a letter or underscore.
 * \defgroup kvadb_database    Databases
 * \brief Initialization, setting protocols and flags.
 * \ingroup grp_kvadb
 * \defgroup kvadb_messages    Messages
 * \brief Add, delete and modify messages.
 * \ingroup grp_kvadb
 * \defgroup kvadb_signals     Signals
 * \brief Add, delete and modify signals in messages.
 * \ingroup grp_kvadb
 * \defgroup kvadb_nodes       Nodes
 * \brief Add, delete and modify nodes.
 * \ingroup grp_kvadb
 * \defgroup kvadb_attributes   Attributes
 * \brief Add, delete and modify attributes for messages, signals and nodes.
 * \ingroup grp_kvadb
 */

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

/**
 *
 * \name kvaDbErr_xxx
 * \anchor kvaDbErr_xxx
 *
 * The following codes are returned by most kvaDbLib API calls.
 *
 * \note Error codes are always negative.
 * @{
 */
typedef enum {
    kvaDbOK                      =  0,  ///< OK - no error
    kvaDbErr_Fail                = -1,  ///< General failure
    kvaDbErr_NoDatabase          = -2,  ///< No database was found
    kvaDbErr_Param               = -3,  ///< One or more of the parameters in call is erronous
    kvaDbErr_NoMsg               = -4,  ///< No message was found
    kvaDbErr_NoSignal            = -5,  ///< No signal was found
    kvaDbErr_Internal            = -6,  ///< An internal error occured in the library
    kvaDbErr_DbFileOpen          = -7,  ///< Could not open the database file
    kvaDbErr_DatabaseInternal    = -8,  ///< An internal error occured in the database handler
    kvaDbErr_NoNode              = -9,  ///< Could not find the database node
    kvaDbErr_NoAttrib            = -10, ///< No attribute found
    kvaDbErr_OnlyOneAllowed      = -11, ///< An identical kvaDbLib structure already exists (and only one database at a time can be used).
    kvaDbErr_WrongOwner          = -12, ///< Wrong owner for attribute
    kvaDbErr_InUse               = -13, ///< An item is in use
    kvaDbErr_BufferSize          = -14, ///< The supplied buffer is too small to hold the result
    kvaDbErr_DbFileParse         = -15, ///< Could not parse the database file
} KvaDbStatus;
/** @} */

/**
 * \name Kvaser Database Message Flags
 * \anchor KVADB_MESSAGE_xxx
 *
 * @{
 */
#define KVADB_MESSAGE_STD      0x00000000  ///< Message is a standard CAN message
#define KVADB_MESSAGE_EXT      0x80000000  ///< Message is an extended CAN message
#define KVADB_MESSAGE_J1939    0x00000001  ///< Message uses J1939 protocol.
#define KVADB_MESSAGE_WAKEUP   0x00000002  ///< Message is a wakeup frame. This flag is currently not used.
/** @} */

/**
 * \name Kvaser CAN Message Flags
 *
 * @{
 */
#define canMSG_STD              0x0002    ///< Message has a standard (11-bit) identifier
#define canMSG_EXT              0x0004    ///< Message has an extended (29-bit) identifier
#define canFDMSG_FDF            0x010000  ///< Message is an FD message (CAN FD)
#define canFDMSG_BRS            0x020000  ///< Message is sent/received with bit rate switch (CAN FD)
/** @} */

/**
 * \name CANDBSignalEncoding API Signal Encoding
 *
 * The following values are used by \ref kvaDbGetSignalEncoding() and
 * \ref kvaDbSetSignalEncoding()
 * @{
 */
typedef enum {
  kvaDb_Intel     = 0, ///< Intel
  kvaDb_Motorola  = 1, ///< kvaDb_Motorola
} KvaDbSignalEncoding;
/** @} */

/**
 * \name CANDBSignalRepresentation API Signal Representation
 *
 * The type \ref KvaDbSignalType is used by e.g. \ref kvaDbGetSignalRepresentationType().
 * @{
 */
typedef enum  {
   kvaDb_Invalid,   ///< Invalid representation
   kvaDb_Signed,    ///< Signed integer
   kvaDb_Unsigned,  ///< Unsigned integer
   kvaDb_Float,     ///< Float, strictly 32 bit long
   kvaDb_Double     ///< Double, strictly 64 bit long
} KvaDbSignalType;
/** @} */

/**
 * \name CANDBAttributeRepresentation API Attribute Representation
 *
 * The type \ref KvaDbAttributeType is used by e.g. kvaDbGetAttributeDefinitionType()
 * and kvaDbGetAttributeType().
 * @{
 */
typedef enum {
   kvaDb_AttributeInvalid       = 0,  ///< Invalid attribute
   kvaDb_AttributeInteger       = 1,  ///< Integer attribute
   kvaDb_AttributeFloat         = 3,  ///< Float attribute
   kvaDb_AttributeEnumeration   = 4,  ///< Enumeration attribute
   kvaDb_AttributeString        = 5,  ///< String attribute
} KvaDbAttributeType;
/** @} */

/**
 * \name CANDBAttributeOwner API Attribute Owner
 * \anchor kvadb_AttributeOwnerXXX
 *
 * The type \ref KvaDbAttributeOwner is used by e.g. \ref kvaDbGetAttributeDefinitionOwner(),
 * @{
 */
typedef enum {
   kvaDb_AttributeOwnerInvalid  = 0,  ///< Invalid owner
   kvaDb_AttributeOwnerDb       = 1,  ///< Database owner
   kvaDb_AttributeOwnerMessage  = 2,  ///< Message owner
   kvaDb_AttributeOwnerNode     = 3,  ///< Node owner
   kvaDb_AttributeOwnerSignal   = 4,  ///< Signal owner
   kvaDb_AttributeOwnerEnv      = 5,  ///< Environment owner
} KvaDbAttributeOwner;
/** @} */

/**
 * \name CANDBProtocolType API Protocol Type
 * \anchor KvaDbProtocolType
 *
 * The type \ref KvaDbProtocolType is used by e.g. \ref kvaDbGetProtocol() and \ref kvaDbSetProtocol()
 * @{
 */
typedef enum {
  kvaDb_ProtocolCan = 0,       ///< Can protocol
  kvaDb_ProtocolVan = 1,       ///< Van protocol
  kvaDb_ProtocolLin = 2,       ///< Lin protocol
  kvaDb_ProtocolMOST = 3,      ///< MOST protocol
  kvaDb_ProtocolFlexRay = 4,   ///< FlexRay protocol
  kvaDb_ProtocolBEAN = 5,      ///< BEAN protocol
  kvaDb_ProtocolEthernet = 6,  ///< Ethernet protocol
  kvaDb_ProtocolAFDX = 7,      ///< AFDX protocol
  kvaDb_ProtocolJ1708 = 8,     ///< J1708 protocol
  kvaDb_ProtocolCanFD = 9,     ///< Can FD protocol
  kvaDb_ProtocolUnknown = 10,  ///< Unknown or not specified protocol
} KvaDbProtocolType;
/** @} */

/**
 * \name KvaDbProtocolproperties Protocol Properties type
 *
 * Struct specifying limits of a given protocol. Used in \ref
 * kvaDbGetProtocolProperties()
 *
 * \note maxMessageDlc provides only raw message dlc limit (e.g. 4 bits for CAN).
 *
 * \sa \kvaDbMsgDlcToBytes()
 */
typedef struct {
  unsigned int maxMessageDlc;
  unsigned int maxSignalLength;
} KvaDbProtocolProperties;

// Database flags

/**
 * \name CANDBDatabaseFlags API Database Flags
 * \anchor KVADB_DATABASE_xxx
 *
 * The following value is used by \ref kvaDbGetFlags().
 */
#define KVADB_DATABASE_J1939   0x0001  ///< The database uses the J1939 protocol

/** 
 * \name Kvaser Database Handle Types
 * @{
 */
typedef void* KvaDbHnd;              ///< Database handle
typedef void* KvaDbMessageHnd;       ///< Message handle
typedef void* KvaDbSignalHnd;        ///< Signal handle
typedef void* KvaDbNodeHnd;          ///< Node handle
typedef void* KvaDbAttributeHnd;     ///< Attribute handle
typedef void* KvaDbAttributeDefHnd;  ///< Attribute definition handle
typedef void* KvaDbEnumValueHnd;     ///< Enumeration value handle
/** @} */

/**
 * \name Kvaser Database Multiplexer Mode values
 * \anchor KVADB_MUX_xxx
 * @{
*/
#define KVADB_MUX_SIGNAL -2  ///< Multiplex mode value of a multiplexer signal
#define KVADB_MUX_INDEPENDENT -1 ///< Multiplex mode value of an independent signal
/** @} */

/**
 * \ingroup kvadb_database
 * This function creates a new database handle.
 *
 * \param[out] dh  A handle to a new database
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbClose()
 */
KvaDbStatus WINAPI kvaDbOpen(KvaDbHnd *dh);

/**
 * \ingroup kvadb_database
 * This loads a database file into a handle created with \ref kvaDbOpen().
 *
 * \note This function is deprecated and will be replaced by \ref kvaDbCreate()
 *
 * \param[in] dh        A handle database handle
 * \param[in] filename  A database file
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbCreate()
 */
KvaDbStatus WINAPI kvaDbAddFile(KvaDbHnd dh, const char *filename);

/**
 * \ingroup kvadb_database
 * This function creates a new database in a handle created with
 * \ref kvaDbOpen(). It is used when there is no file to read data from.
 *
 * \note This function is deprecated and will be replaced by \ref kvaDbCreate().
 *
 * \param[in] dh        A database handle
 * \param[in] filename  The the dummy filename to use for this database
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbCreate()
 */
KvaDbStatus WINAPI kvaDbSetDummyFileName(KvaDbHnd dh, const char *filename);

/**
 * \ingroup kvadb_database
 * This function creates a new database in a handle created with
 * \ref kvaDbOpen(). There are three ways to call this function:
 *
 * \li To load data from an existing database file, set \a localName to \c NULL
 * and set \a filename to the database file.
 *
 * \li To add an empty database, set \a localName to any name and set \a
 * filename to \c NULL.
 *
 * \li To load data from an existing database file and give it a new name, set
 * \a localName to the new name and set \a filename to the database file.
 * In this case, it will attempt to use period as decimal separator by setting
 * an appropriate locale.
 * Note that setting locale is not thread safe.
 *
 * \param[in] dh         A database handle
 * \param[in] localName  The name of the database or \c NULL. The database name
 *                       is used to create qualified names for messages and signals.
 * \param[in] filename   The database file or \c NULL
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbOpen(), \ref kvaDbGetMsgQualifiedName(),
 * \ref kvaDbGetSignalQualifiedName()
 */
KvaDbStatus WINAPI kvaDbCreate(KvaDbHnd dh, const char *localName, const char *filename);

/**
 * \ingroup kvadb_database
 *
 * Get the name of a database as a null-terminated string.
 * A truncated string will be returned if \a buf is too small and status will be kvaDbOK.
 *
 * \param[in] dh         A database handle.
 * \param[out] buf       Buffer to receive database name. The database name
 *                       is used to create qualified names for messages and signals.
 * \param[in] buflen     Buffer size in bytes.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbOpen(), \ref kvaDbGetMsgQualifiedName(),
 * \ref kvaDbGetSignalQualifiedName()
 */
KvaDbStatus WINAPI kvaDbGetDatabaseName(KvaDbHnd dh, char *buf, size_t buflen);

/**
 * \ingroup kvadb_database
 *
 * This function gets the version of the KVADBLIB API DLL (kvadblib.dll).
 *
 * \param[out] major          Major version number.
 * \param[out] minor          Minor version number.
 * \param[out] build          Build number.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 */
KvaDbStatus WINAPI kvaDbGetVersion(int *major, int *minor, int *build);

/**
 * \ingroup kvadb_database
 *
 * Get the written description for an error code as a null-terminated string.
 * If \a buf is too small, kvaDbErr_Param will be returned.
 *
 * \param[in]  error         The error code.
 * \param[out] buf           Buffer to receive error text.
 * \param[in]  buflen        Buffer size in bytes.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 */
KvaDbStatus WINAPI kvaDbGetErrorText(KvaDbStatus error, char *buf, size_t buflen);

/**
 * \ingroup kvadb_database
 * This function retrieves error messages. 
 * Run directly after function when recieving error code.
 * Currently only applies to {kvaDbAddFile, kvaDbCreate, kvaDbReadFile}
 *
 * \param[out] buf            Buffer to receive error message.
 * \param[out] buflen         Buffer size in bytes.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_Param (negative) if failure
 *
 * \sa \ref kvaDbOpen()
 */
KvaDbStatus WINAPI kvaDbGetLastParseError(char *buf, unsigned int *buflen);

/**
 * \ingroup kvadb_database
 * This function closes an open database handle.
 *
 * \param[in] dh  A handle to a database
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbOpen()
 */
KvaDbStatus WINAPI kvaDbClose(KvaDbHnd dh);

/**
 * \ingroup kvadb_database
 * Write a database to file.
 * This function will attempt to use period as decimal separator
 * by setting an appropriate locale.
 * Note that setting locale is not thread safe.
 *
 * \param[in] dh        A handle to a database
 * \param[in] filename  The name of the database file
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbReadFile()
 */
KvaDbStatus WINAPI kvaDbWriteFile(KvaDbHnd dh, char* filename);

/**
 * \ingroup kvadb_database
 * Load a database from file into a handle created with \ref kvaDbOpen().
 * This function will attempt to use period as decimal separator by setting
 * an appropriate locale.
 * Note that setting locale is not thread safe.
 *
 * \param[out]  dh        A handle to a database
 * \param[in]   filename  The name of the database file.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbCreate()
 */
KvaDbStatus WINAPI kvaDbReadFile(KvaDbHnd dh, char* filename);

/**
 * \ingroup kvadb_database
 * Get the database flags.
 *
 * \param[in]  dh     A handle to a database
 * \param[out] flags  The database flags, \ref KVADB_DATABASE_xxx
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbReadFile()
 *
 * \todo check \ref kvaDbReadFile(), we should perhaps link to \ref kvaDbCreate()?
 */
KvaDbStatus WINAPI kvaDbGetFlags(KvaDbHnd dh, unsigned int *flags);

/**
 * \ingroup kvadb_database
 * Reads the protocol attribute value of a database
 *
 * \param[in]  dh    A handle to a database
 * \param[out] prot  A handle to protocol type, \ref KvaDbProtocolType
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetProtocol(), \ref kvaDbGetProtocolProperties
 *
 */
KvaDbStatus WINAPI kvaDbGetProtocol(KvaDbHnd dh, KvaDbProtocolType *prot);


/**
 * \ingroup kvadb_database
 * Returns properties of a given protocol, see \ref KvaDbProtocolProperties
 *
 * \param[in]  prot  Protocol type, \ref KvaDbProtocolType
 * \param[out] prop  A handle to protocol properties, \ref KvaDbProtocolProperties
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetProtocol()
 */
KvaDbStatus WINAPI kvaDbGetProtocolProperties(KvaDbProtocolType prot, KvaDbProtocolProperties *prop);


/**
 * \ingroup kvadb_messages
 * Get the first message in a database.
 *
 * \param[in]  dh  A handle to a database
 * \param[out] mh  A handle to the first message
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetNextMsg()
 */
KvaDbStatus WINAPI kvaDbGetFirstMsg(KvaDbHnd dh, KvaDbMessageHnd *mh);

/**
 * \ingroup kvadb_messages
 * Get a handle to the next message in a database. Should be called after
 * \ref kvaDbGetFirstMsg().
 *
 * \param[in]  dh  A database handle
 * \param[out] mh  A message handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetFirstMsg()
 */
KvaDbStatus WINAPI kvaDbGetNextMsg(KvaDbHnd dh, KvaDbMessageHnd *mh);

/**
 * \ingroup kvadb_messages
 * Get a handle to a message with a specific identifier.
 * If the message requested has an extended id, the id should
 * have the \ref KVADB_MESSAGE_EXT flag set.
 *
 * \param[in]  dh  A database handle
 * \param[in]  id  The selected message identifier
 * \param[out] mh  A message handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetMsgByName()
 * \sa \ref kvaDbGetMsgByIdEx()
 */
KvaDbStatus WINAPI kvaDbGetMsgById(KvaDbHnd dh,
                                   unsigned int id,
                                   KvaDbMessageHnd *mh);



/**
 * \ingroup kvadb_messages
 * Get a handle to a message with a specific identifier.
 * If the requested message has an extended id, \ref KVADB_MESSAGE_EXT
 * should be set on the flags parameter.
 * Bits 31..29 of the id are ignored.
 *
 * \param[in]  dh     A database handle
 * \param[in]  id     The selected message identifier
 * \param[in]  flags  The flags, \ref KVADB_MESSAGE_xxx
 * \param[out] mh     A message handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetMsgByName()
 */
KvaDbStatus WINAPI kvaDbGetMsgByIdEx(KvaDbHnd dh,
                                   unsigned int id,
                                   unsigned int flags,
                                   KvaDbMessageHnd *mh);



/**
 * \ingroup kvadb_messages
 * Get a handle to a message with a specific PGN-identifier.
 * If the message is marked as J1939, checks if the PGN-part is equal.
 * The message identifier needs to contain the \ref KVADB_MESSAGE_EXT flag.

 * \param[in]  dh  A database handle
 * \param[in]  id  The selected message identifier
 * \param[out] mh  A message handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetMsgByName()
 * \sa \ref kvaDbGetMsgByPGNEx()
 */
KvaDbStatus WINAPI kvaDbGetMsgByPGN(KvaDbHnd dh,
                                    unsigned int id,
                                    KvaDbMessageHnd *mh);


/**
 * \ingroup kvadb_messages
 * Get a handle to a J1939 message using the PGN-identifier within the CAN id.
 * All bits in the id that are not part of the PGN are ignored.

 * \param[in]  dh  A database handle
 * \param[in]  id  The selected message identifier
 * \param[out] mh  A message handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetMsgByName()
 */
KvaDbStatus WINAPI kvaDbGetMsgByPGNEx(KvaDbHnd dh,
                                    unsigned int id,
                                    KvaDbMessageHnd *mh);

/**
 * \ingroup kvadb_messages
 * Get a handle to a message with a specific name.
 *
 * \param[in]  dh        A databse handle
 * \param[in]  msg_name  The message name to search for
 * \param[out] mh        A message handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetMsgById()
 */
KvaDbStatus WINAPI kvaDbGetMsgByName(KvaDbHnd dh,
                                     const char *msg_name,
                                     KvaDbMessageHnd *mh);

/**
 * \ingroup kvadb_messages
 *
 * Get the name of a message as a null-terminated string.
 * A truncated string will be returned if \a buf is too small and status will be kvaDbOK.
 *
 * \param[in]  mh      A message handle
 * \param[out] buf     The buffer that will hold the message name
 * \param[in]  buflen  The length of the buffer.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetMsgName(), \ref kvaDbGetMsgQualifiedName()
 */
KvaDbStatus WINAPI kvaDbGetMsgName(KvaDbMessageHnd mh, char *buf, size_t buflen);

/**
 * \ingroup kvadb_messages
 *
 * Get the qualified message name, which is the database name and the message
 * name separated by a dot, as a null-terminated string.
 * An empty string will be returned if \a buf is too small and status will be kvaDbOK.
 *
 * \param[in]  mh      A message handle
 * \param[out] buf     The buffer that will hold the qualified message name
 * \param[in]  buflen  The length of the buffer
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetMsgName()
 */
KvaDbStatus WINAPI kvaDbGetMsgQualifiedName(KvaDbMessageHnd mh, char *buf, size_t buflen);

/**
 * \ingroup kvadb_messages
 *
 * Get the comment for a message as a null-terminated string.
 * A truncated string will be returned if \a buf is too small and status will be kvaDbOK.
 * 
 * \param[in]  mh      A message handle
 * \param[out] buf     The buffer that will hold the message comment
 * \param[in]  buflen  The length of the buffer
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetMsgComment()
 */
KvaDbStatus WINAPI kvaDbGetMsgComment(KvaDbMessageHnd mh, char *buf, size_t buflen);

/**
 * \ingroup kvadb_messages
 * Get message identifier and flags.
 * If message is of extended CAN format, its id will contain
 * \ref KVADB_MESSAGE_EXT flag.
 * This call is deprecated. Consider using \ref kvaDbGetMsgIdEx()
 * and \ref kvaDbGetMsgFlags() instead.
 *
 * \param[in]  mh     A message handle.
 * \param[out] id     The identifier.
 * \param[out] flags  The flags, \ref KVADB_MESSAGE_xxx
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetMsgId()
 */
KvaDbStatus WINAPI kvaDbGetMsgId(KvaDbMessageHnd mh,
                                 unsigned int *id,
                                 unsigned int *flags);

                                 /**
 * \ingroup kvadb_messages
 * Get the message identifier.
 * The message identifier will not contain any flags.
 *
 * \param[in] mh     A message handle
 * \param[out] id     The message identifier
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 *
 * \sa \ref kvaDbSetMsgIdEx()
 */
KvaDbStatus WINAPI kvaDbGetMsgIdEx(KvaDbMessageHnd mh, unsigned int *id);

/**
 * \ingroup kvadb_messages
 * Get the message flags, \ref KVADB_MESSAGE_xxx
 *
 * \param[in] mh     A message handle
 * \param[out] flags  The message flags, \ref KVADB_MESSAGE_xxx
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 *
 * \sa \ref kvaDbSetMsgFlags()
 */
KvaDbStatus WINAPI kvaDbGetMsgFlags(KvaDbMessageHnd mh, unsigned int *flags);

/**
 * \ingroup kvadb_messages
 * Get relevant message attributes expressed as CAN message flags, see \ref canMSG_xxx and \ref canFDMSG_xxx
 *
 * Note that canFDMSG_BRS will never be returned for non-CAN FD frames, even though the CANFD_BRS attribute
 * was set in the .dbc file.
 *
 * \param[in]   mh     A message handle
 * \param[out]  flags  The message flags, \ref KVADB_MESSAGE_xxx
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 */
KvaDbStatus WINAPI kvaDbGetCanMsgFlags(KvaDbMessageHnd mh, unsigned int *flags);

/**
 * \ingroup kvadb_messages
 * Get the data length code for a message.
 *
 * \param[in]  mh   A message handle
 * \param[out] dlc  The data length code
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetMsgDlc()
 */
KvaDbStatus WINAPI kvaDbGetMsgDlc(KvaDbMessageHnd mh, int *dlc);

/**
 * \ingroup kvadb_messages
 * Get send node for a message
 *
 * \param[in]  mh         A message handle
 * \param[out] nh         A node handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbSetMsgSendNode()
 */
KvaDbStatus WINAPI kvaDbGetMsgSendNode(KvaDbMessageHnd mh, KvaDbNodeHnd *nh);

/**
 * \ingroup kvadb_messages
 * Add a new message to a database.
 *
 * \param[in]  dh  A database handle
 * \param[out] mh  A handle to the new message
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbDeleteMsg()
 */
KvaDbStatus WINAPI kvaDbAddMsg(KvaDbHnd dh, KvaDbMessageHnd *mh);

/**
 * \ingroup kvadb_messages
 * Delete a message from a database.
 *
 * \param[in] dh  A database handle
 * \param[in] mh  A handle to the message that will be deleted
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbAddMsg()
 */
KvaDbStatus WINAPI kvaDbDeleteMsg(KvaDbHnd dh, KvaDbMessageHnd mh);

/**
 * \ingroup kvadb_database
 * Set the database flags.
 *
 * \note This feature is currently not enabled.
 *
 * \param[in] dh     A database handle
 * \param[in] flags  The flags to set, \ref KVADB_DATABASE_xxx
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetFlags()
 */
KvaDbStatus WINAPI kvaDbSetFlags(KvaDbHnd dh, unsigned int flags);

/**
 * \ingroup kvadb_database
 * Writes the protocol attribute value of a database
 *
 * \param[in]  dh    A handle to a database
 * \param[in]  prot  Protocol type value, \ref KvaDbProtocolType
 */
KvaDbStatus WINAPI kvaDbSetProtocol(KvaDbHnd dh, KvaDbProtocolType prot);

/**
 * \ingroup kvadb_messages
 * Set the message name.
 *
 * \param[in] mh   A message handle
 * \param[in] buf  The buffer that contains the message name.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \note Please note that allowed characters in identiers are digits, letters and "_" (underscore). <br>
 * Identifiers shall also start with a letter or underscore.
 *
 * \sa \ref kvaDbGetMsgName()
 */
KvaDbStatus WINAPI kvaDbSetMsgName(KvaDbMessageHnd mh, char *buf);

/**
 * \ingroup kvadb_messages
 * Set the message comment.
 *
 * \param[in] mh   A message handle
 * \param[in] buf The buffer that contains the message comment
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetMsgComment()
 */
KvaDbStatus WINAPI kvaDbSetMsgComment(KvaDbMessageHnd mh, char *buf);

/**
 * \ingroup kvadb_messages
 * Set the message identifier and flags.
 * If message is of extended CAN format, its id should contain
 * \ref KVADB_MESSAGE_EXT flag.
 * This call is deprecated, consider using \ref kvaDbSetMsgIdEx()
 * and \ref kvaDbSetMsgFlags() instead.
 *
 * \param[in] mh     A message handle
 * \param[in] id     The message identifier
 * \param[in] flags  The message flags, \ref KVADB_MESSAGE_xxx
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 *
 * \sa \ref kvaDbGetMsgId()
 */
KvaDbStatus WINAPI kvaDbSetMsgId(KvaDbMessageHnd mh, unsigned int id, unsigned int flags);

/**
 * \ingroup kvadb_messages
 * Set the message identifier.
 * The message identifier should not contain any flags.
 *
 * \param[in] mh     A message handle
 * \param[in] id     The message identifier
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 *
 * \sa \ref kvaDbGetMsgIdEx()
 */
KvaDbStatus WINAPI kvaDbSetMsgIdEx(KvaDbMessageHnd mh, unsigned int id);

/**
 * \ingroup kvadb_messages
 * Set the message flags, \ref KVADB_MESSAGE_xxx
 *
 * \param[in] mh     A message handle
 * \param[in] flags  The message flags, \ref KVADB_MESSAGE_xxx
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 *
 * \sa \ref kvaDbGetMsgFlags()
 */
KvaDbStatus WINAPI kvaDbSetMsgFlags(KvaDbMessageHnd mh, unsigned int flags);


/**
 * \ingroup kvadb_messages
 * Set the data length code for a message.
 *
 * \param[in] mh   A message handle
 * \param[in] dlc  The data length code
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetMsgDlc()
 */
KvaDbStatus WINAPI kvaDbSetMsgDlc(KvaDbMessageHnd mh, int dlc);

/**
 * \ingroup kvadb_messages
 * Set send node for a message
 *
 * \param[in] mh   A message handle
 * \param[in] nh   A node handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetMsgSendNode()
 */
KvaDbStatus WINAPI kvaDbSetMsgSendNode(KvaDbMessageHnd mh, KvaDbNodeHnd nh);

/**
 * \ingroup kvadb_messages
 * Returns handle to a multiplexer signal of a given message.
 * If there are no multiplexer signal, KvaDbOk will be returned
 * and \a *sh will be \c NULL.
 *
 * \param[in]  mh     A message handle
 * \param[out] sh     Multiplexer signal handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetMsgMux(KvaDbMessageHnd mh, KvaDbSignalHnd *sh);

/**
 * \ingroup kvadb_signals
 * Get the first signal in a message.
 *
 * \param[in]  mh  A message handle
 * \param[out] sh  A handle to the first signal
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetNextSignal()
 */
KvaDbStatus WINAPI kvaDbGetFirstSignal(KvaDbMessageHnd mh, KvaDbSignalHnd *sh);

/**
 * \ingroup kvadb_signals
 * Get the next signal in a message. Should be called after
 * \ref kvaDbGetFirstSignal().
 *
 * \param[in]  mh  A message handle
 * \param[out] sh  A handle to the next signal
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetFirstSignal
 */
KvaDbStatus WINAPI kvaDbGetNextSignal(KvaDbMessageHnd mh, KvaDbSignalHnd *sh);

/**
 * \ingroup kvadb_signals
 * Get a signal handle by seaching for the signals's name.
 *
 * \param[in]  mh   A message handle
 * \param[in]  buf  The buffer that contains the name of the signal to search for
 * \param[out] sh   A handle to the signal
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetFirstMsg()
 */
KvaDbStatus WINAPI kvaDbGetSignalByName(KvaDbMessageHnd mh, char *buf, KvaDbSignalHnd *sh);

/**
 * \ingroup kvadb_signals
 * Sets signal's multiplex mode. If value is equal to KVADB_MUX_SIGNAL, marks signal as a multiplexer.
 *
 * \param[in]  sh     A signal handle
 * \param[in] mux   Signal's multiplex mode
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * \sa \ref KVADB_MUX_xxx
 */

KvaDbStatus WINAPI kvaDbSetSignalMode(KvaDbSignalHnd sh, int mux);

/**
 * \ingroup kvadb_signals
 * Gets mux from multiplexer mode value of given signal.
 * See \ref KVADB_MUX_xxx for special (negative) values of multiplexer mode.
 *
 * \param[in]  sh     A signal handle
 * \param[out] mux   Signal's multiplex mode
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * \sa \ref KVADB_MUX_xxx
*/

KvaDbStatus WINAPI kvaDbGetSignalMode(KvaDbSignalHnd sh, int *mux);

/**
 * \ingroup kvadb_signals
 * Get handle to message, which given signal is associated with.
 *
 * \param[in]  sh    A signal handle
 * \param[out] mh    Signal's message
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbStoreSignalValuePhys()
*/

KvaDbStatus WINAPI kvaDbGetSignalMessage(KvaDbSignalHnd sh, KvaDbMessageHnd *mh);

/**
 * \ingroup kvadb_signals
 * Retrieve signal's physical (floating-point) value from can data.
 *
 * This function is deprecated and should not be used.
 * Consider \ref kvaDbRetrieveSignalValuePhys() instead
 *
 * \param[in]  sh    A signal handle
 * \param[out] f     The converted value
 * \param[in]  data  The data buffer to be converted
 * \param[in]  len   The length of the data buffer
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbStoreSignalValuePhys()
*/


KvaDbStatus WINAPI kvaDbGetSignalValueFloat(KvaDbSignalHnd sh, double *f, void *data, size_t len);

/**
 * \ingroup kvadb_signals
  * Retrieve signal's raw (integer) value from can data.
  *
  * This function is deprecated and should not be used.
  * Consider \ref kvaDbRetrieveSignalValueRaw() instead
  *
  * \param[in]  sh    A signal handle
  * \param[out] f     The converted value
  * \param[in]  data  The data buffer to be converted
  * \param[in]  len   The length of the data buffer
  *
  * \return \ref kvaDbOK (zero) if success
  * \return \ref kvaDbErr_xxx (negative) if failure
  *
  * \sa \ref kvaDbStoreSignalValueRaw()
*/
KvaDbStatus WINAPI kvaDbGetSignalValueInteger(KvaDbSignalHnd sh, int *f, void *data, size_t len);

/**
 * \ingroup kvadb_signals
 * Retrieve signal's physical (floating-point) value from can data.
 *
 * \param[in]  sh    A signal handle
 * \param[out] value The converted value
 * \param[in]  data  The data buffer to be converted
 * \param[in]  len   The length of the data buffer
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbStoreSignalValuePhys()
*/
KvaDbStatus WINAPI kvaDbRetrieveSignalValuePhys(KvaDbSignalHnd sh, double *value, void *data, size_t len);

/**
 * \ingroup kvadb_signals
  * Retrieve signal's raw (integer) value from can data.
  *
  * \param[in]  sh    A signal handle
  * \param[out] value The converted value
  * \param[in]  data  The data buffer to be converted
  * \param[in]  len   The length of the data buffer
  *
  * \return \ref kvaDbOK (zero) if success
  * \return \ref kvaDbErr_xxx (negative) if failure
  *
  * \sa \ref kvaDbStoreSignalValueRaw()
*/
KvaDbStatus WINAPI kvaDbRetrieveSignalValueRaw(KvaDbSignalHnd sh, int *value, void *data, size_t len);

/**
 * \ingroup kvadb_signals
  * Retrieve signal's raw (integer) value from can data.
  *
  * \param[in]  sh    A signal handle
  * \param[out] value The converted value (64-bits)
  * \param[in]  data  The data buffer to be converted
  * \param[in]  len   The length of the data buffer
  *
  * \return \ref kvaDbOK (zero) if success
  * \return \ref kvaDbErr_xxx (negative) if failure
  *
  * \sa \ref kvaDbStoreSignalValueRaw64()
*/
KvaDbStatus WINAPI kvaDbRetrieveSignalValueRaw64(KvaDbSignalHnd sh, uint64_t *value, void *data, size_t len);

/**
 * \ingroup kvadb_signals
 * Retrieve signal's  enumeration (string) value from can data.
 *
 * \param[in]  sh     A signal handle
 * \param[out] buf    The buffer that will hold the enumeration string
 * \param[in]  buflen The length of the buffer.
 * \param[in]  data   The data buffer to be converted
 * \param[in]  len    The length of the data buffer
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbStoreSignalValueRaw()
 */

KvaDbStatus WINAPI kvaDbGetSignalValueEnum(KvaDbSignalHnd sh, char *buf, size_t buflen, void *data, size_t len);


/**
 * \ingroup kvadb_signals
 * Get the minimum and maximum values from a signal.
 *
 * \param[in]  sh      A signal handle
 * \param[out] minval  The minimum value
 * \param[out] maxval  The maximum value
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetSignalValueLimits()
 */
KvaDbStatus WINAPI kvaDbGetSignalValueLimits(KvaDbSignalHnd sh, double *minval, double *maxval);

/**
 * \ingroup kvadb_signals
 * Get the scale factor and offset from a signal.
 *
 * \param[in]  sh      A signal handle
 * \param[out] factor  The signal scale factor
 * \param[out] offset  The signal offset
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetSignalValueScaling()
 */
KvaDbStatus WINAPI kvaDbGetSignalValueScaling(KvaDbSignalHnd sh, double *factor, double *offset);

/**
 * \ingroup kvadb_signals
 * Get the size and position of the signal value.
 *
 * \param[in]  sh        A signal handle
 * \param[out] startbit  The bit where the signal value starts
 * \param[out] length    The length of the signal value in bits
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetSignalValueSize()
 */
KvaDbStatus WINAPI kvaDbGetSignalValueSize(KvaDbSignalHnd sh, int *startbit, int *length);

/**
 * \ingroup kvadb_signals
 * 
 * Get the name of a signal as a null-terminated string.
 * A truncated string will be returned if \a buf is too small and status will be kvaDbOK.
 * 
 * \param[in]  sh      A signal handle.
 * \param[out] buf     The signal name.
 * \param[in]  buflen  The length of the buffer that will hold the signal name.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetSignalName()
 */
KvaDbStatus WINAPI kvaDbGetSignalName(KvaDbSignalHnd sh, char *buf, size_t buflen);

/**
 * \ingroup kvadb_signals
 *
 * Get the qualified signal name, which is the database, message and signal
 * names separated by dots, as a null-terminated string.
 * An empty string will be returned if \a buf is too small and status will be kvaDbOK.
 * 
 * \param[in]  sh      A signal handle
 * \param[out] buf     The qualified signal name
 * \param[in]  buflen  The length of the buffer that will hold the qualified
 *                     signal name
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetSignalName()
 */
KvaDbStatus WINAPI kvaDbGetSignalQualifiedName(KvaDbSignalHnd sh, char *buf, size_t buflen);

/**
 * \ingroup kvadb_signals
 *
 * Get the comment for a signal as a null-terminated string.
 * A truncated string will be returned if \a buf is too small and status will be kvaDbOK.
 *
 * \param[in]  sh      A signal handle
 * \param[out] buf     The signal comment
 * \param[in]  buflen  The length of the buffer that will hold the signal comment
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetSignalComment()
 */
KvaDbStatus WINAPI kvaDbGetSignalComment(KvaDbSignalHnd sh, char *buf, size_t buflen);

/**
 * \ingroup kvadb_signals
 * 
 * Get the signal unit as a null-terminated string.
 * A truncated string will be returned if \a buf is too small and status will be kvaDbOK.
 *  
 * \param[in]  sh      A signal handle
 * \param[out] buf     The signal unit
 * \param[in]  buflen  The length of the buffer that will hold the signal unit
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetSignalUnit()
 */
KvaDbStatus WINAPI kvaDbGetSignalUnit(KvaDbSignalHnd sh, char *buf, size_t buflen);

/**
 * \ingroup kvadb_signals
 * Get the signal encoding.
 *
 * \param[in]  sh  A signal handle
 * \param[out] e   The signal encoding, Intel or Motorola, \ref KvaDbSignalEncoding
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetSignalEncoding()
 */
KvaDbStatus WINAPI kvaDbGetSignalEncoding(KvaDbSignalHnd sh, KvaDbSignalEncoding *e);

/**
 * \ingroup kvadb_signals
 * Get the signal representation type.
 *
 * \param[in]  sh  A signal handle
 * \param[out] t   The signal representation type, \ref KvaDbSignalType
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetSignalRepresentationType()
 */
KvaDbStatus WINAPI kvaDbGetSignalRepresentationType(KvaDbSignalHnd sh, KvaDbSignalType *t);

/**
 * \ingroup kvadb_signals
 * Get the signal representation type.
 *
 * \note This function is deprecated and will be replaced by
 * \ref kvaDbGetSignalRepresentationType()
 *
 * \param[in]  sh
 * \param[out] t   The signal representation type, \ref KvaDbSignalType
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbSetSignalRepresentationType()
 */
KvaDbStatus WINAPI kvaDbGetSignalPresentationType(KvaDbSignalHnd sh, KvaDbSignalType *t);

/**
 * \ingroup kvadb_signals
 * Add a new signal to a message.
 *
 * \param[in]  mh  A message handle
 * \param[out] sh  A handle to the new signal
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbDeleteSignal()
 */
KvaDbStatus WINAPI kvaDbAddSignal(KvaDbMessageHnd mh, KvaDbSignalHnd *sh);

/**
 * \ingroup kvadb_signals
 * Delete a signal from a message.
 *
 * \param[in] mh  A message handle
 * \param[in] sh  A handle to the signal to delete
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbAddSignal()
 */
KvaDbStatus WINAPI kvaDbDeleteSignal(KvaDbMessageHnd mh, KvaDbSignalHnd sh);

/**
 * \ingroup kvadb_signals
 * Set the value limits for a signal.
 *
 * \param[in] sh      A signal handle
 * \param[in] minval  The minimum signal value
 * \param[in] maxval  The maximum signal value
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetSignalValueLimits()
 */
KvaDbStatus WINAPI kvaDbSetSignalValueLimits(KvaDbSignalHnd sh,
                                             double minval,
                                             double maxval);

/**
 * \ingroup kvadb_signals
 * Set the scale factor and offset for a signal.
 *
 * \param[in] sh      A signal handle
 * \param[in] factor  The scale factor for the signal value
 * \param[in] offset  The offset for the signal value
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetSignalValueScaling()
 */
KvaDbStatus WINAPI kvaDbSetSignalValueScaling(KvaDbSignalHnd sh,
                                              double factor,
                                              double offset);

/**
 * \ingroup kvadb_signals
 * Set the value position and size for a signal.
 *
 * \param[in] sh        A signal handle
 * \param[in] startbit  The starting bit of the signal's value
 * \param[in] length    The length of the value in bits
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetSignalValueSize()
 */
KvaDbStatus WINAPI kvaDbSetSignalValueSize(KvaDbSignalHnd sh,
                                           int startbit,
                                           int length);

/**
 * \ingroup kvadb_signals
 * Set the signal  name.
 *
 * \param[in] sh   A signal handle
 * \param[in] buf  The buffer that contains the signal name
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \note Please note that allowed characters in identiers are digits, letters and "_" (underscore). <br>
 * Identifiers shall also start with a letter or underscore.
 *
 * \sa \ref kvaDbGetSignalName()
 */
KvaDbStatus WINAPI kvaDbSetSignalName(KvaDbSignalHnd sh, char *buf);

/**
 * \ingroup kvadb_signals
 * Set the signal comment.
 *
 * \param[in] sh   A signal handle
 * \param[in] buf  The buffer that contains the signal comment
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetSignalComment()
 */
KvaDbStatus WINAPI kvaDbSetSignalComment(KvaDbSignalHnd sh, char *buf);

/**
 * \ingroup kvadb_signals
 * Set the signal unit.
 *
 * \param[in] sh   A signal handle
 * \param[in] buf  The buffer that contains the unit for the signal
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetSignalUnit()
 */
KvaDbStatus WINAPI kvaDbSetSignalUnit(KvaDbSignalHnd sh, char *buf);

/**
 * \ingroup kvadb_signals
 * Set the signal encoding.
 *
 * \param[in] sh  A signal handle
 * \param[in] e   The signal encoding, Intel or Motorola, \ref KvaDbSignalEncoding
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetSignalEncoding()
 */
KvaDbStatus WINAPI kvaDbSetSignalEncoding(KvaDbSignalHnd sh, KvaDbSignalEncoding e);

/**
 * \ingroup kvadb_signals
 * Set the signal representation type.
 *
 * \param[in] sh  A signal handle
 * \param[in] t   The signal representation type, \ref KvaDbSignalType
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetSignalRepresentationType()
 */
KvaDbStatus WINAPI kvaDbSetSignalRepresentationType(KvaDbSignalHnd sh, KvaDbSignalType t);

/**
 * \ingroup kvadb_nodes
 * Get the first node.
 *
 * \param[in]  dh  A database handle
 * \param[out] nh  A node handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetNextNode()
 */
KvaDbStatus WINAPI kvaDbGetFirstNode(KvaDbHnd dh, KvaDbNodeHnd *nh);

/**
 * \ingroup kvadb_nodes
 * Get the next node. Should be called after kvaDbGetFirstNode().
 *
 * \param[in]  dh  A database handle
 * \param[out] nh  A handle to the next node
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetNodeByName()
 */
KvaDbStatus WINAPI kvaDbGetNextNode(KvaDbHnd dh, KvaDbNodeHnd *nh);

/**
 * \ingroup kvadb_nodes
 * Get a node by seaching for the node's name.
 *
 * \param[in]  dh         A database handle
 * \param[in]  node_name  The buffer that contains the node name to search for
 * \param[out] nh         A node handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetNodeByName(KvaDbHnd dh, const char *node_name, KvaDbNodeHnd *nh);

/**
 * \ingroup kvadb_attributes
 * Get the value of a node attribute as an integer.
 * This call is deprecated, it is equivalent to sequence of
 * \ref kvaDbGetNodeAttributeByName() and \ref kvaDbGetAttributeValueInt(),
 * which should be used instead.
 *
 * \param[in]  nh        A node handle
 * \param[in]  attrName  The buffer that contains the attribute name
 * \param[out] val       The value of the attribute
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetFirstNode()
 */
KvaDbStatus WINAPI kvaDbGetNodeAttributeIntByName(KvaDbNodeHnd nh, const char *attrName, int *val);

/**
 * \ingroup kvadb_nodes
 * Add a new node to a database.
 *
 * \param[in]  dh  A database handle
 * \param[out] nh  A handle to the new node
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbAddNode(KvaDbHnd dh, KvaDbNodeHnd *nh);

/**
 * \ingroup kvadb_nodes
 * Delete a node from a database.
 *
 * \param[in] dh  A database handle
 * \param[in] nh  A handle to the node that will be deleted
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbDeleteNode(KvaDbHnd dh, KvaDbNodeHnd nh);

/**
 * \ingroup kvadb_nodes
 * Set the node name.
 *
 * \param[in] nh   A node handle
 * \param[in] buf  The buffer that contains the node name.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \note Please note that allowed characters in identiers are digits, letters and "_" (underscore). <br>
 * Identifiers shall also start with a letter or underscore.
 *
 */
KvaDbStatus WINAPI kvaDbSetNodeName(KvaDbNodeHnd nh, char *buf);

/**
 * \ingroup kvadb_nodes
 * 
 * Get the name of a node as a null-terminated string.
 * A truncated string will be returned if \a buf is too small and status will be kvaDbOK.
 *  
 * \param[in] nh   A node handle
 * \param[out] buf  The buffer that will contain the node name
 * \param[in] buflen  The length of the data buffer
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetNodeName(KvaDbNodeHnd nh, char *buf, size_t buflen);

/**
 * \ingroup kvadb_nodes
 * Set the node comment.
 *
 * \param[in] nh   A node handle
 * \param[in] buf  The buffer that contains the node comment
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbSetNodeComment(KvaDbNodeHnd nh, char *buf);

/**
 * \ingroup kvadb_nodes
 * 
 * Get the comment for a node as a null-terminated string.
 * A truncated string will be returned if \a buf is too small and status will be kvaDbOK.
 *  
 * \param[in] nh   A node handle
 * \param[out] buf  The buffer that will contain the node comment
 * \param[in] buflen The length of the data buffer
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetNodeComment(KvaDbNodeHnd nh, char *buf, size_t buflen);

/**
 * \ingroup kvadb_nodes
 * Add a receiving node to a signal.
 *
 * \param[in] sh   A signal handle
 * \param[in] nh   A node handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbAddReceiveNodeToSignal(KvaDbSignalHnd sh, KvaDbNodeHnd nh);

/**
 * \ingroup kvadb_nodes
 * Remove a receiving node from a signal.
 *
 * \param[in] sh   A signal handle
 * \param[in] nh   A node handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbRemoveReceiveNodeFromSignal(KvaDbSignalHnd sh, KvaDbNodeHnd nh);

/**
 * \ingroup kvadb_nodes
 * Check if a signal contains a specific receiving node
 *
 * \param[in] sh   A signal handle
 * \param[in] nh   A node handle
 *
 * \return \ref kvaDbOK (zero) if the signal contains the node
 * \return \ref kvaDbErr_NoNode if the signal doesn't contain the node
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbSignalContainsReceiveNode(KvaDbSignalHnd sh, KvaDbNodeHnd nh);


/**
 * \ingroup kvadb_signals
 * Convert a physical signal value to data bytes.
 *
 * \param[in]  sh        A signal handle
 * \param[out] can_data  The buffer that will hold the converted value
 * \param[in]  len       The length of the data buffer
 * \param[in]  value     The physical signal value to convert
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetSignalValueFloat()
 */
KvaDbStatus WINAPI kvaDbStoreSignalValuePhys(KvaDbSignalHnd sh, void *can_data, int len, double value);

/**
 * \ingroup kvadb_signals
 * Convert a raw signal value to data bytes.
 *
 * \param[in]  sh        A signal handle
 * \param[out] can_data  The buffer that will hold the converted value
 * \param[in]  len       The length of the data buffer
 * \param[in]  value     The raw signal value to convert
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbRetrieveSignalValueRaw()
 */
KvaDbStatus WINAPI kvaDbStoreSignalValueRaw(KvaDbSignalHnd sh, void *can_data, int len, int value);

/**
 * \ingroup kvadb_signals
 * Convert a raw signal value to data bytes.
 *
 * \param[in]  sh        A signal handle
 * \param[out] can_data  The buffer that will hold the converted value
 * \param[in]  len       The length of the data buffer
 * \param[in]  value     The raw signal value to convert (64-bits)
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbRetrieveSignalValueRaw64()
 */
KvaDbStatus WINAPI kvaDbStoreSignalValueRaw64(KvaDbSignalHnd sh, void *can_data, int len, uint64_t value);

/**
 * \ingroup kvadb_attributes
 * Get a handle to the first attribute defined in the database.
 *
 * \param[in]  dh   A handle to a database
 * \param[out] adh  First attribute definition handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetNextAttributeDefinition()
 */
KvaDbStatus WINAPI kvaDbGetFirstAttributeDefinition(KvaDbHnd dh, KvaDbAttributeDefHnd *adh);

/**
 * \ingroup kvadb_attributes
 * Get a handle to an attribute definition with specified name.
 *
 * \param[in]  dh   A handle to a database
 * \param[in]  attrName  The attribute name to search for
 * \param[out] adh  An attribute definition handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetAttributeDefinitionByName(KvaDbHnd dh, const char *attrName, KvaDbAttributeDefHnd *adh);

/**
 * \ingroup kvadb_attributes
 * Get a handle to the following attribute defined.
 *
 * \param[in]  adh   An existing handle to an attribute definition
 * \param[out] nadh  Handle to next attribute definition
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetFirstAttributeDefinition()
 */
KvaDbStatus WINAPI kvaDbGetNextAttributeDefinition(KvaDbAttributeDefHnd adh, KvaDbAttributeDefHnd *nadh);

/**
 * \ingroup kvadb_attributes
 * Get the type for an attribute definition.
 *
 * \param[in]  adh   A handle to an attribute definition
 * \param[out] at    The type of the attribute definition
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetAttributeDefinitionOwner()
 */
KvaDbStatus WINAPI kvaDbGetAttributeDefinitionType(KvaDbAttributeDefHnd adh, KvaDbAttributeType *at);

/**
 * \ingroup kvadb_attributes
 * Get the type for an attribute definition.
 *
 * \param[in]  adh   A handle to an attribute definition
 * \param[out] ao    The owner of the attribute definition, \ref kvadb_AttributeOwnerXXX
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetAttributeDefinitionType()
 */
KvaDbStatus WINAPI kvaDbGetAttributeDefinitionOwner(KvaDbAttributeDefHnd adh, KvaDbAttributeOwner *ao);

/**
 * \ingroup kvadb_attributes
 * 
 * Get the name of an attribute definition as a null-terminated string.
 * A truncated string will be returned if \a buf is too small and status will be kvaDbOK.
 *  
 * \param[in]  adh     A handle to an attribute
 * \param[out] buf     The buffer that will hold the attribute name
 * \param[in]  buflen  The length of the buffer.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetAttributeDefinitionType()
 */
KvaDbStatus WINAPI kvaDbGetAttributeDefinitionName(KvaDbAttributeDefHnd adh, char *buf, size_t buflen);

/**
 * \ingroup kvadb_attributes
 * Get limits and default value for an attribute definition of type integer.
 *
 * \param[in]  adh   A handle to an attribute definition
 * \param[out] def   The default value of the attribute definition
 * \param[out] min   The min value of the attribute definition
 * \param[out] max   The max value of the attribute definition
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetAttributeDefinitionInt(KvaDbAttributeDefHnd adh, int *def, int *min, int *max);

/**
 * \ingroup kvadb_attributes
 * Get limits and default value for an attribute definition of type float.
 *
 * \param[in]  adh   A handle to an attribute definition
 * \param[out] def   The default value of the attribute definition
 * \param[out] min   The min value of the attribute definition
 * \param[out] max   The max value of the attribute definition
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetAttributeDefinitionFloat(KvaDbAttributeDefHnd adh, float *def, float *min, float *max);

/**
 * \ingroup kvadb_attributes
 * 
 * Get default value for an attribute definition of type string as a null-terminated string.
 * A truncated string will be returned if \a buf is too small and status will be kvaDbOK. 
 *  
 * \param[in]  adh     A handle to an attribute definition
 * \param[out] buf     The buffer that holds the default value of the attribute
 * \param[out] buflen  The length of the buffer.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetAttributeDefinitionString(KvaDbAttributeDefHnd adh, char *buf, size_t buflen);

/**
 * \ingroup kvadb_attributes
 * Get default value for an attribute definition of type enumeration.
 *
 * \param[in]  adh   A handle to an attribute definition
 * \param[out] def   The default value of the attribute definition
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetAttributeDefinitionEnumeration(KvaDbAttributeDefHnd adh, int *def);

/**
 * \ingroup kvadb_attributes
 * Add a new attribute definition to a database.
 *
 * \param[in]  dh    A handle to a database
 * \param[out] adh   A handle to a new attribute definition
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbAddAttributeDefinition(KvaDbHnd dh, KvaDbAttributeDefHnd *adh);

/**
 * \ingroup kvadb_attributes
 * Delete a database attribute definition.
 *
 * \note If there are attributes using the attribute definition \a adh, the call will fail and
 *  return \ref kvaDbErr_InUse. Delete these attributes and the call will succeed.
 *
 * \param[in]  dh    A handle to a database
 * \param[in] adh    A handle to a attribute definition
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbDeleteAttributeDefinition(KvaDbHnd dh, KvaDbAttributeDefHnd adh);

/**
 * \ingroup kvadb_attributes
 * Set the name of an attribute definition.
 * It is not allowed to have several attribute definitions with same name.
 *
 * \param[in]  adh     A handle to a attribute definition
 * \param[in]  adName  The name of the attribute definition
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \note Please note that allowed characters in identiers are digits, letters and "_" (underscore). <br>
 * Identifiers shall also start with a letter or underscore.
 *
 */
KvaDbStatus WINAPI kvaDbSetAttributeDefinitionName(KvaDbAttributeDefHnd adh, const char *adName);

/**
 * \ingroup kvadb_attributes
 * Set the type of an attribute definition.
 *
 * \param[in]  adh     A handle to a attribute definition
 * \param[in]  adType  The type of the attribute definition
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbSetAttributeDefinitionType(KvaDbAttributeDefHnd adh, KvaDbAttributeType adType);

/**
 * \ingroup kvadb_attributes
 * Set the owner of an attribute definition.
 *
 * \param[in]  adh     A handle to a attribute definition
 * \param[in]  adOwner The owner of the attribute definition
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbSetAttributeDefinitionOwner(KvaDbAttributeDefHnd adh, KvaDbAttributeOwner adOwner);

/**
 * \ingroup kvadb_attributes
 * Add a enumeration value to an attribute definition of enumeration type.  If
 * enumeration entry with specified value already exists, it will be
 * overwritten.  Enumeration entries assume to start with value zero, which is
 * then incremented by one for consequent entries.
 *
 * \note In case \a eValue is out of order, i.e. exceeds maximum value by more
 * than one, or was not started at zero, additional dummy entries with name
 * "reserved" will be added to enumeration.
 *
 * \param[in]  adh     A handle to a attribute definition
 * \param[in]  eName   The name of the enumeration entry
 * \param[in]  eValue  The value of the enumeration entry
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbAddAttributeDefinitionEnum(KvaDbAttributeDefHnd adh, const char* eName, int eValue);

/**
 * \ingroup kvadb_attributes
 * Deletes the first enumeration entry that matches eName and eValue.
 *
 * \param[in]  adh     A handle to a attribute definition
 * \param[in]  eName   The name of the enumeration entry
 * \param[in]  eValue  The value of the enumeration entry
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbDeleteAttributeDefinitionEnum(KvaDbAttributeDefHnd adh, const char* eName, int eValue);

/**
 * \ingroup kvadb_attributes
 * Get value of an enumeration entry by its name in attribute definition.
 *
 * \note If there are several entries with same name, the minimum value of the
 * entries with the name \a eName is returned.
 *
 * \param[in]  adh      A handle to a attribute definition
 * \param[in]  eName    The name of the enumeration entry
 * \param[out] eValue   The value of the enumeration entry
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetAttributeDefinitionEnumValueByName(KvaDbAttributeDefHnd adh, const char* eName, int* eValue);

/**
 * \ingroup kvadb_attributes
 * Get name of an enumeration entry by its value in attribute definition. Name
 * is written to a buffer provided by user. If buffer size is insufficient to
 * fit enumeration name, buffer size parameter will contain required buffer
 * size.
 *
 * \param[in]  adh      A handle to a attribute definition
 * \param[in]  eValue   The value of the enumeration entry
 * \param[out] buf      Buffer to receive enumeration name
 * \param[out] buflen   Buffer size in bytes
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetAttributeDefinitionEnumNameByValue(KvaDbAttributeDefHnd adh, int eValue, char* buf, size_t *buflen);

/**
 * \ingroup kvadb_attributes
 * Get name and value for the first enumeration entry for this handle. Name
 * is written to a buffer provided by user. If buffer size is insufficient to
 * fit enumeration name, buffer size parameter will contain required buffer
 * size.
 *
 * To get the next enumeration entry vall \ref kvaDbGetAttributeDefinitionEnumNext().
 *
 * \param[in]  adh      A handle to a attribute definition
 * \param[out] eValue   The value of the enumeration entry
 * \param[out] eName    Buffer to receive enumeration name
 * \param[in]  buflen   Buffer size in bytes
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetAttributeDefinitionEnumFirst(KvaDbAttributeDefHnd adh, int *eValue, char* eName, size_t *buflen);

/**
 * \ingroup kvadb_attributes
 * \note Shall be called after \ref kvaDbGetAttributeDefinitionEnumFirst().
 *
 * Get name and value for the next enumeration entry for this handle. Name
 * is written to a buffer provided by user. If buffer size is insufficient to
 * fit enumeration name, buffer size parameter will contain required buffer
 * size.
 *
 * \param[in]  adh      A handle to a attribute definition
 * \param[out] eValue   The value of the enumeration entry
 * \param[out] eName    Buffer to receive enumeration name
 * \param[in]  buflen   Buffer size in bytes
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetAttributeDefinitionEnumNext(KvaDbAttributeDefHnd adh, int *eValue, char* eName, size_t *buflen);

/**
 * \ingroup kvadb_attributes
 * Set the default enumeration value of an attribute definition of enumeration type.
 *
 * \param[in]  adh     A handle to a attribute definition
 * \param[in]  dValue  The default value of the enumeration entry
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbSetAttributeDefinitionEnumDefault(KvaDbAttributeDefHnd adh, int dValue);

/**
 * \ingroup kvadb_attributes
 * Set max, min and default value of an attribute definition of integer type.
 *
 * \param[in]  adh     A handle to a attribute definition
 * \param[in]  def     The default value
 * \param[in]  min     The minimum value
 * \param[in]  max     The maximum value
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbSetAttributeDefinitionInt(KvaDbAttributeDefHnd adh, int def, int min, int max);

/**
 * \ingroup kvadb_attributes
 * Set max, min and default value of an attribute definition of floating point type.
 *
 * \param[in]  adh     A handle to a attribute definition
 * \param[in]  def     The default value
 * \param[in]  min     The minimum value
 * \param[in]  max     The maximum value
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbSetAttributeDefinitionFloat(KvaDbAttributeDefHnd adh, float def, float min, float max);

/**
 * \ingroup kvadb_attributes
 * Set the default value of an attribute definition of string.
 *
 * \param[in]  adh     A handle to a attribute definition
 * \param[in]  buf     The default value
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbSetAttributeDefinitionString(KvaDbAttributeDefHnd adh, const char *buf);

/**
 * \ingroup kvadb_attributes
 * Add an attribute to a message.
 *
 * \param[in]  mh    A handle to a message
 * \param[in]  adh   A handle to an attribute definition
 * \param[out] ah    An attribute handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbDeleteMsgAttribute()
 */
KvaDbStatus WINAPI kvaDbAddMsgAttribute(KvaDbMessageHnd mh, KvaDbAttributeDefHnd adh, KvaDbAttributeHnd *ah);

/**
 * \ingroup kvadb_attributes
 * Add an attribute to a database.
 *
 * \param[in]  dh    A handle to a database
 * \param[in]  adh   A handle to an attribute definition
 * \param[out] ah    An attribute handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbDeleteAttribute()
 */
KvaDbStatus WINAPI kvaDbAddAttribute(KvaDbHnd dh, KvaDbAttributeDefHnd adh, KvaDbAttributeHnd *ah);

/**
 * \ingroup kvadb_attributes
 * Delete an attribute from a message.
 *
 * \param[in]  mh    A handle to a message
 * \param[in]  ah    A handle to an attribute
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbAddMsgAttribute()
 */
KvaDbStatus WINAPI kvaDbDeleteMsgAttribute(KvaDbMessageHnd mh, KvaDbAttributeHnd ah);

/**
 * \ingroup kvadb_attributes
 * Delete an attribute from a database.
 *
 * \param[in]  dh    A handle to a message
 * \param[in]  ah    A handle to an attribute
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbAddAttribute()
 */
KvaDbStatus WINAPI kvaDbDeleteAttribute(KvaDbHnd dh, KvaDbAttributeHnd ah);

/**
 * \ingroup kvadb_attributes
 * Add an attribute to a signal.
 *
 * \param[in]  sh    A handle to a signal
 * \param[in]  adh   A handle to an attribute definition
 * \param[out] ah    An attribute handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbDeleteSignalAttribute()
 */
KvaDbStatus WINAPI kvaDbAddSignalAttribute(KvaDbSignalHnd sh, KvaDbAttributeDefHnd adh, KvaDbAttributeHnd *ah);

/**
 * \ingroup kvadb_signals
 * Get the first enumeration value in a signal.
 *
 * \param[in]  sh  A signal handle
 * \param[out] eh  A handle to the first enumeration value
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetNextEnumValue()
 */
KvaDbStatus WINAPI kvaDbGetFirstEnumValue(KvaDbSignalHnd sh, KvaDbEnumValueHnd *eh);

/**
 * \ingroup kvadb_signals
 * Get the next enumeration value in a signal. Should be called after
 * \ref kvaDbGetFirstEnumValue().
 *
 * \param[in]  sh  A signal handle
 * \param[out] eh  A handle to the next enumeration value
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetNextEnumValue
 */
KvaDbStatus WINAPI kvaDbGetNextEnumValue(KvaDbSignalHnd sh, KvaDbEnumValueHnd *eh);

/**
 * \ingroup kvadb_signals
 *
 * Get the enumeration value and name.
 * The name is returned as a null-terminated string.
 * If \a buf is to small, the name will be truncated and status will be kvaDbOK.
 *  
 * \param[in]  eh      An enumeration value handle
 * \param[out] val     The enumeration value
 * \param[out] buf     The buffer that will hold the enumeration name
 * \param[in]  buflen  The length of the buffer
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbGetFirstEnumValue()
 */
KvaDbStatus WINAPI kvaDbGetEnumValue(KvaDbEnumValueHnd eh, int *val, char *buf, size_t buflen);

/**
 * \ingroup kvadb_signals
 *
 * Get the enumeration value and name for the first enumeration value for a signal.
 * The name is returned as a null-terminated string.
 * If \a buf is to small, the name will be truncated and status will be kvaDbOK.
 *  
 * \param[in]  sh      A signal handle.
 * \param[out] eh      An enumeration value handle.
 * \param[out] val     The enumeration value.
 * \param[out] buf     The buffer that will hold the enumeration name.
 * \param[in]  buflen  The length of the buffer.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 */
KvaDbStatus WINAPI kvaDbGetFirstEnumValuePair(KvaDbSignalHnd sh, KvaDbEnumValueHnd *eh, int *val, char *buf, size_t buflen);

/**
 * \ingroup kvadb_signals
 *
 * Get the enumeration value and name for the next enumeration value for a signal.
 * The name is returned as a null-terminated string.
 * If \a buf is to small, the name will be truncated and status will be kvaDbOK.
 * Should be called after \ref kvaDbGetFirstEnumValuePair().
 *  
 * \param[in]  sh      A signal handle.
 * \param[out] eh      An enumeration value handle.
 * \param[out] val     The enumeration value.
 * \param[out] buf     The buffer that will hold the enumeration name.
 * \param[in]  buflen  The length of the buffer.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 */
KvaDbStatus WINAPI kvaDbGetNextEnumValuePair(KvaDbSignalHnd sh, KvaDbEnumValueHnd *eh, int *val, char *buf, size_t buflen);

/**
 * \ingroup kvadb_signals
 * Add a new enumeration value to a signal.
 *
 * \param[in]  sh      A signal handle
 * \param[out] val     The enumeration value
 * \param[out] name    The enumeration name
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbDeleteEnumValue()
 */
KvaDbStatus WINAPI kvaDbAddEnumValue(KvaDbSignalHnd sh, int val, const char* name);

/**
 * \ingroup kvadb_signals
 * Delete an enumeration value from a signal.
 *
 * \param[in]  sh      A signal handle
 * \param[in]  eh      An enumeration value handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa \ref kvaDbAddEnumValue()
 */
KvaDbStatus WINAPI kvaDbDeleteEnumValue(KvaDbSignalHnd sh, KvaDbEnumValueHnd eh);

/**
 * \ingroup kvadb_attributes
 * Delete an attribute from a signal.
 *
 * \param[in]  sh    A handle to a signal
 * \param[in]  ah    A handle to an attribute
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbAddSignalAttribute()
 */
KvaDbStatus WINAPI kvaDbDeleteSignalAttribute(KvaDbSignalHnd sh, KvaDbAttributeHnd ah);

/**
 * \ingroup kvadb_attributes
 * Add an attribute to a node.
 *
 * \param[in]  nh    A handle to a node
 * \param[in]  adh   A handle to an attribute definition
 * \param[out] ah    An attribute handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbDeleteNodeAttribute()
 */
KvaDbStatus WINAPI kvaDbAddNodeAttribute(KvaDbNodeHnd nh, KvaDbAttributeDefHnd adh, KvaDbAttributeHnd *ah);

/**
 * \ingroup kvadb_attributes
 * Delete an attribute from a node.
 *
 * \param[in]  nh    A handle to a node
 * \param[in]  ah    A handle to an attribute
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbAddNodeAttribute()
 */
KvaDbStatus WINAPI kvaDbDeleteNodeAttribute(KvaDbNodeHnd nh, KvaDbAttributeHnd ah);

/**
 * \ingroup kvadb_attributes
 * Get a handle to the first attribute for a message.
 *
 * \param[in]  mh   A handle to a message
 * \param[out] ah   An attribute definition handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetNextAttribute()
 */
KvaDbStatus WINAPI kvaDbGetFirstMsgAttribute(KvaDbMessageHnd mh, KvaDbAttributeHnd *ah);

/**
 * \ingroup kvadb_attributes
 * Get a handle to an attribute with specified name for a message.
 *
 * \param[in]  mh   A handle to a message
 * \param[in]  attrName  The attribute name to search for
 * \param[out] ah  An attribute definition handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetMsgAttributeByName(KvaDbMessageHnd mh, const char *attrName, KvaDbAttributeHnd *ah);

/**
 * \ingroup kvadb_attributes
 * Get a handle to an attribute with specified name for a database.
 *
 * \param[in]  dh   A handle to a database
 * \param[in]  attrName  The attribute name to search for
 * \param[out] ah  An attribute definition handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetAttributeByName(KvaDbHnd dh, const char *attrName, KvaDbAttributeHnd *ah);

/**
 * \ingroup kvadb_attributes
 * Get a handle to the first attribute for a signal.
 *
 * \param[in]  sh   A handle to a signal
 * \param[out] ah   An attribute handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetNextAttribute()
 */
KvaDbStatus WINAPI kvaDbGetFirstSignalAttribute(KvaDbSignalHnd sh, KvaDbAttributeHnd *ah);

/**
 * \ingroup kvadb_attributes
 * Get a handle to an attribute with specified name for a signal.
 *
 * \param[in]  sh   A handle to a signal
 * \param[in]  attrName  The attribute name to search for
 * \param[out] ah  An attribute definition handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetSignalAttributeByName(KvaDbSignalHnd sh, const char *attrName, KvaDbAttributeHnd *ah);

/**
 * \ingroup kvadb_attributes
 * Get a handle to the first attribute for a node.
 *
 * \param[in]  nh   A handle to a node
 * \param[out] ah   An attribute definition handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetNextAttribute()
 */
KvaDbStatus WINAPI kvaDbGetFirstNodeAttribute(KvaDbNodeHnd nh, KvaDbAttributeHnd *ah);

/**
 * \ingroup kvadb_attributes
 * Get a handle to an attribute with specified name for a node.
 *
 * \param[in]  nh   A handle to a node
 * \param[in]  attrName  The attribute name to search for
 * \param[out] ah  An attribute definition handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbGetNodeAttributeByName(KvaDbNodeHnd nh, const char *attrName, KvaDbAttributeHnd *ah);

/**
 * \ingroup kvadb_attributes
 * Get a handle to the first attribute for a database
 *
 * \param[in]  dh    A handle to an database
 * \param[out] nah   An attribute handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 */
KvaDbStatus WINAPI kvaDbGetFirstAttribute(KvaDbHnd dh, KvaDbAttributeHnd *nah);

/**
 * \ingroup kvadb_attributes
 * Get a handle to the following attribute.
 *
 * \param[in]  ah    A handle to an attribute
 * \param[out] nah   An attribute handle
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetFirstAttribute()
 * \sa kvaDbGetFirstMsgAttribute()
 * \sa kvaDbGetFirstSignalAttribute()
 * \sa kvaDbGetFirstNodeAttribute()
 */
KvaDbStatus WINAPI kvaDbGetNextAttribute(KvaDbAttributeHnd ah, KvaDbAttributeHnd *nah);

/**
 * \ingroup kvadb_attributes
 * Get the type for an attribute.
 *
 * \param[in]  ah   A handle to an attribute
 * \param[out] at   The type of the attribute
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetAttributeName()
 */
KvaDbStatus WINAPI kvaDbGetAttributeType(KvaDbAttributeHnd ah, KvaDbAttributeType *at);

/**
 * \ingroup kvadb_attributes
 *
 * Get the name of an attribute as a null-terminated string.
 * A truncated string will be returned if \a buf is too small and status will be kvaDbOK.
 *  
 * \param[in]  ah      A handle to an attribute
 * \param[out] buf     The buffer that will hold the attribute name
 * \param[in]  buflen  The length of the buffer.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetAttributeType()
 */
KvaDbStatus WINAPI kvaDbGetAttributeName(KvaDbAttributeHnd ah, char *buf, size_t buflen);

/**
 * \ingroup kvadb_attributes
 * Set the value for an attribute of type integer.
 *
 * \param[in]  ah   A handle to an attribute
 * \param[in]  val  The value to set
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetAttributeValueInt()
 */
KvaDbStatus WINAPI kvaDbSetAttributeValueInt(KvaDbAttributeHnd ah, int val);

/**
 * \ingroup kvadb_attributes
 * Get the value for an attribute of type integer.
 *
 * \param[in]  ah   A handle to an attribute
 * \param[out] val  The value of the attribute
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbSetAttributeValueInt()
 */
KvaDbStatus WINAPI kvaDbGetAttributeValueInt(KvaDbAttributeHnd ah, int *val);

/**
 * \ingroup kvadb_attributes
 * Set the value for an attribute of type float.
 *
 * \param[in]  ah   A handle to an attribute
 * \param[in]  val  The value to set
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetAttributeValueFloat()
 */
KvaDbStatus WINAPI kvaDbSetAttributeValueFloat(KvaDbAttributeHnd ah, float val);

/**
 * \ingroup kvadb_attributes
 * Get the value for an attribute of type float.
 *
 * \param[in]  ah   A handle to an attribute
 * \param[out] val  The value of the attribute
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbSetAttributeValueFloat()
 */
KvaDbStatus WINAPI kvaDbGetAttributeValueFloat(KvaDbAttributeHnd ah, float *val);

/**
 * \ingroup kvadb_attributes
 * Set the value for an attribute of type string.
 *
 * \param[in]  ah   A handle to an attribute
 * \param[in] buf     The buffer that holds the attribute value
 * \param[in]  buflen  The length of the buffer.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetAttributeValueString()
 */
KvaDbStatus WINAPI kvaDbSetAttributeValueString(KvaDbAttributeHnd ah, const char *buf, int buflen);

/**
 * \ingroup kvadb_attributes
 *
 * Get the value for an attribute of type string as a null-terminated string.
 * A truncated string will be returned if \a buf is too small and status will be kvaDbOK.
 *  
 * \param[in]  ah   A handle to an attribute
 * \param[out] buf     The buffer that holds the attribute value
 * \param[out]  buflen  The length of the buffer.
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbSetAttributeValueString()
 */
KvaDbStatus WINAPI kvaDbGetAttributeValueString(KvaDbAttributeHnd ah, char *buf, size_t buflen);

/**
 * \ingroup kvadb_attributes
 * Set the value for an attribute of type enumeration.
 *
 * \param[in]  ah   A handle to an attribute
 * \param[in]  val  The value to set
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetAttributeValueEnumeration()
 */
KvaDbStatus WINAPI kvaDbSetAttributeValueEnumeration(KvaDbAttributeHnd ah, int val);

/**
 * \ingroup kvadb_attributes
 * Get the value for an attribute of type enumeration.
 *
 * \param[in]  ah   A handle to an attribute
 * \param[out] val  The value of the attribute
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbSetAttributeValueEnumeration()
 */
KvaDbStatus WINAPI kvaDbGetAttributeValueEnumeration(KvaDbAttributeHnd ah, int *val);


/**
 * \ingroup kvadb_messages
 * Translates message dlc to number of bytes for a given protocol
 *
 * \param[in]  prot      A protocol type
 * \param[in]  dlc       Raw message dlc
 * \param[out] numBytes  Number of bytes
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetProtocol()
 */
KvaDbStatus WINAPI kvaDbMsgDlcToBytes(KvaDbProtocolType prot, unsigned int dlc, unsigned int *numBytes);

/**
 * \ingroup kvadb_messages
 * Translates number of bytes to message dlc for a given protocol
 *
 * \param[in]  prot       A protocol type
 * \param[in]  numBytes   Number of bytes
 * \param[out] dlc        Raw message dlc
 *
 * \return \ref kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 *
 * \sa kvaDbGetProtocol()
 */
KvaDbStatus WINAPI kvaDbBytesToMsgDlc(KvaDbProtocolType prot, unsigned int numBytes, unsigned int *dlc);

/**
 * \page page_kvadblib Database API (kvaDbLib)
 *
 *  \tableofcontents
 *
 *
 * The database library kvaDbLib is used for creating, reading, writing and
 * modifying CAN databases that holds descriptions of packages that is received
 * or sent on the CAN network.
 *
 * A CAN database contains information about messages. Each message has (among
 * other attributes) an identifier, a name and one or several signals. The
 * kvaDbLib library is an API for these CAN databases.
 *
 * - \subpage page_kvadblib_user_guide_intro
 * - \subpage page_kvadblib_example_load_database
 * - \subpage page_kvadblib_using_threads
 * - \subpage page_user_guide_kvadblib_samples
 *
 * For more details, see the module \ref grp_kvadb.
 */

#ifdef __cplusplus
}
#endif

#endif //KVADBLIB_H
