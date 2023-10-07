unit canlib;
//
// Copyright 1995-2020 by KVASER AB
// WWW: http://www.kvaser.com
//
// This software is furnished under a license and may be used and copied
// only in accordance with the terms of such license.
//

// This unit is part of the interface for Delphi to CANLIB.
//
// This unit is automatically generated.

{$MINENUMSIZE 4}

interface

uses
  Windows;

const
  {$IF Defined(WIN32)}
  canlib32 = 'canlib32.dll';
  _PU = '';
  {$ELSE}
    {$MESSAGE Error 'Unsupported platform'}
  {$ENDIF}

const
  _WINDOWS = 1;
  canEVENT_RX = 32000;
  canEVENT_TX = 32001;
  canEVENT_ERROR = 32002;
  canEVENT_STATUS = 32003;
  canEVENT_ENVVAR = 32004;
  canEVENT_BUSONOFF = 32005;
  canEVENT_REMOVED = 32006;
  canNOTIFY_NONE = 0;
  canNOTIFY_RX = $0001;
  canNOTIFY_TX = $0002;
  canNOTIFY_ERROR = $0004;
  canNOTIFY_STATUS = $0008;
  canNOTIFY_ENVVAR = $0010;
  canNOTIFY_BUSONOFF = $0020;
  canNOTIFY_REMOVED = $0040;
  canSTAT_ERROR_PASSIVE = $00000001;
  canSTAT_BUS_OFF = $00000002;
  canSTAT_ERROR_WARNING = $00000004;
  canSTAT_ERROR_ACTIVE = $00000008;
  canSTAT_TX_PENDING = $00000010;
  canSTAT_RX_PENDING = $00000020;
  canSTAT_RESERVED_1 = $00000040;
  canSTAT_TXERR = $00000080;
  canSTAT_RXERR = $00000100;
  canSTAT_HW_OVERRUN = $00000200;
  canSTAT_SW_OVERRUN = $00000400;
  canSTAT_OVERRUN = (canSTAT_HW_OVERRUN or canSTAT_SW_OVERRUN);
  canMSG_MASK = $00ff;
  canMSG_RTR = $0001;
  canMSG_STD = $0002;
  canMSG_EXT = $0004;
  canMSG_WAKEUP = $0008;
  canMSG_NERR = $0010;
  canMSG_ERROR_FRAME = $0020;
  canMSG_TXACK = $0040;
  canMSG_TXRQ = $0080;
  canMSG_DELAY_MSG = $0100;
  canMSG_SINGLE_SHOT = $1000000;
  canMSG_TXNACK = $2000000;
  canMSG_ABL = $4000000;
  canFDMSG_MASK = $ff0000;
  canFDMSG_EDL = $010000;
  canFDMSG_FDF = $010000;
  canFDMSG_BRS = $020000;
  canFDMSG_ESI = $040000;
  canMSGERR_MASK = $ff00;
  canMSGERR_HW_OVERRUN = $0200;
  canMSGERR_SW_OVERRUN = $0400;
  canMSGERR_STUFF = $0800;
  canMSGERR_FORM = $1000;
  canMSGERR_CRC = $2000;
  canMSGERR_BIT0 = $4000;
  canMSGERR_BIT1 = $8000;
  canMSGERR_OVERRUN = $0600;
  canMSGERR_BIT = $C000;
  canMSGERR_BUSERR = $F800;
  canTRANSCEIVER_LINEMODE_NA = 0;
  canTRANSCEIVER_LINEMODE_SWC_SLEEP = 4;
  canTRANSCEIVER_LINEMODE_SWC_NORMAL = 5;
  canTRANSCEIVER_LINEMODE_SWC_FAST = 6;
  canTRANSCEIVER_LINEMODE_SWC_WAKEUP = 7;
  canTRANSCEIVER_LINEMODE_SLEEP = 8;
  canTRANSCEIVER_LINEMODE_NORMAL = 9;
  canTRANSCEIVER_LINEMODE_STDBY = 10;
  canTRANSCEIVER_LINEMODE_TT_CAN_H = 11;
  canTRANSCEIVER_LINEMODE_TT_CAN_L = 12;
  canTRANSCEIVER_LINEMODE_OEM1 = 13;
  canTRANSCEIVER_LINEMODE_OEM2 = 14;
  canTRANSCEIVER_LINEMODE_OEM3 = 15;
  canTRANSCEIVER_LINEMODE_OEM4 = 16;
  canTRANSCEIVER_RESNET_NA = 0;
  canTRANSCEIVER_RESNET_MASTER = 1;
  canTRANSCEIVER_RESNET_MASTER_STBY = 2;
  canTRANSCEIVER_RESNET_SLAVE = 3;
  canTRANSCEIVER_TYPE_UNKNOWN = 0;
  canTRANSCEIVER_TYPE_251 = 1;
  canTRANSCEIVER_TYPE_252 = 2;
  canTRANSCEIVER_TYPE_DNOPTO = 3;
  canTRANSCEIVER_TYPE_W210 = 4;
  canTRANSCEIVER_TYPE_SWC_PROTO = 5;
  canTRANSCEIVER_TYPE_SWC = 6;
  canTRANSCEIVER_TYPE_EVA = 7;
  canTRANSCEIVER_TYPE_FIBER = 8;
  canTRANSCEIVER_TYPE_K251 = 9;
  canTRANSCEIVER_TYPE_K = 10;
  canTRANSCEIVER_TYPE_1054_OPTO = 11;
  canTRANSCEIVER_TYPE_SWC_OPTO = 12;
  canTRANSCEIVER_TYPE_TT = 13;
  canTRANSCEIVER_TYPE_1050 = 14;
  canTRANSCEIVER_TYPE_1050_OPTO = 15;
  canTRANSCEIVER_TYPE_1041 = 16;
  canTRANSCEIVER_TYPE_1041_OPTO = 17;
  canTRANSCEIVER_TYPE_RS485 = 18;
  canTRANSCEIVER_TYPE_LIN = 19;
  canTRANSCEIVER_TYPE_KONE = 20;
  canTRANSCEIVER_TYPE_CANFD = 22;
  canTRANSCEIVER_TYPE_CANFD_LIN = 24;
  canTRANSCEIVER_TYPE_LINX_LIN = 64;
  canTRANSCEIVER_TYPE_LINX_J1708 = 66;
  canTRANSCEIVER_TYPE_LINX_K = 68;
  canTRANSCEIVER_TYPE_LINX_SWC = 70;
  canTRANSCEIVER_TYPE_LINX_LS = 72;
  canINVALID_HANDLE = (-1);
  WM_USER = $0400;
  WM__CANLIB = (WM_USER+16354);
  canOPEN_EXCLUSIVE = $0008;
  canOPEN_REQUIRE_EXTENDED = $0010;
  canOPEN_ACCEPT_VIRTUAL = $0020;
  canOPEN_OVERRIDE_EXCLUSIVE = $0040;
  canOPEN_REQUIRE_INIT_ACCESS = $0080;
  canOPEN_NO_INIT_ACCESS = $0100;
  canOPEN_ACCEPT_LARGE_DLC = $0200;
  canOPEN_CAN_FD = $0400;
  canOPEN_CAN_FD_NONISO = $0800;
  canOPEN_INTERNAL_L = $1000;
  canFILTER_ACCEPT = 1;
  canFILTER_REJECT = 2;
  canFILTER_SET_CODE_STD = 3;
  canFILTER_SET_MASK_STD = 4;
  canFILTER_SET_CODE_EXT = 5;
  canFILTER_SET_MASK_EXT = 6;
  canFILTER_NULL_MASK = 0;
  canDRIVER_NORMAL = 4;
  canDRIVER_SILENT = 1;
  canDRIVER_SELFRECEPTION = 8;
  canDRIVER_OFF = 0;
  canBITRATE_1M = (-1);
  canBITRATE_500K = (-2);
  canBITRATE_250K = (-3);
  canBITRATE_125K = (-4);
  canBITRATE_100K = (-5);
  canBITRATE_62K = (-6);
  canBITRATE_50K = (-7);
  canBITRATE_83K = (-8);
  canBITRATE_10K = (-9);
  canFD_BITRATE_500K_80P = (-1000);
  canFD_BITRATE_1M_80P = (-1001);
  canFD_BITRATE_2M_80P = (-1002);
  canFD_BITRATE_2M_60P = (-1007);
  canFD_BITRATE_4M_80P = (-1003);
  canFD_BITRATE_8M_60P = (-1004);
  canFD_BITRATE_8M_80P = (-1005);
  BAUD_1M = (-1);
  BAUD_500K = (-2);
  BAUD_250K = (-3);
  BAUD_125K = (-4);
  BAUD_100K = (-5);
  BAUD_62K = (-6);
  BAUD_50K = (-7);
  BAUD_83K = (-8);
  kvREMOTE_TYPE_NOT_REMOTE = 0;
  kvREMOTE_TYPE_WLAN = 1;
  kvREMOTE_TYPE_LAN = 2;
  kvLOGGER_TYPE_NOT_A_LOGGER = 0;
  kvLOGGER_TYPE_V1 = 1;
  kvLOGGER_TYPE_V2 = 2;
  canCHANNELDATA_CHANNEL_CAP = 1;
  canCHANNELDATA_TRANS_CAP = 2;
  canCHANNELDATA_CHANNEL_FLAGS = 3;
  canCHANNELDATA_CARD_TYPE = 4;
  canCHANNELDATA_CARD_NUMBER = 5;
  canCHANNELDATA_CHAN_NO_ON_CARD = 6;
  canCHANNELDATA_CARD_SERIAL_NO = 7;
  canCHANNELDATA_TRANS_SERIAL_NO = 8;
  canCHANNELDATA_CARD_FIRMWARE_REV = 9;
  canCHANNELDATA_CARD_HARDWARE_REV = 10;
  canCHANNELDATA_CARD_UPC_NO = 11;
  canCHANNELDATA_TRANS_UPC_NO = 12;
  canCHANNELDATA_CHANNEL_NAME = 13;
  canCHANNELDATA_DLL_FILE_VERSION = 14;
  canCHANNELDATA_DLL_PRODUCT_VERSION = 15;
  canCHANNELDATA_DLL_FILETYPE = 16;
  canCHANNELDATA_TRANS_TYPE = 17;
  canCHANNELDATA_DEVICE_PHYSICAL_POSITION = 18;
  canCHANNELDATA_UI_NUMBER = 19;
  canCHANNELDATA_TIMESYNC_ENABLED = 20;
  canCHANNELDATA_DRIVER_FILE_VERSION = 21;
  canCHANNELDATA_DRIVER_PRODUCT_VERSION = 22;
  canCHANNELDATA_MFGNAME_UNICODE = 23;
  canCHANNELDATA_MFGNAME_ASCII = 24;
  canCHANNELDATA_DEVDESCR_UNICODE = 25;
  canCHANNELDATA_DEVDESCR_ASCII = 26;
  canCHANNELDATA_DRIVER_NAME = 27;
  canCHANNELDATA_CHANNEL_QUALITY = 28;
  canCHANNELDATA_ROUNDTRIP_TIME = 29;
  canCHANNELDATA_BUS_TYPE = 30;
  canCHANNELDATA_DEVNAME_ASCII = 31;
  canCHANNELDATA_TIME_SINCE_LAST_SEEN = 32;
  canCHANNELDATA_REMOTE_OPERATIONAL_MODE = 33;
  canCHANNELDATA_REMOTE_PROFILE_NAME = 34;
  canCHANNELDATA_REMOTE_HOST_NAME = 35;
  canCHANNELDATA_REMOTE_MAC = 36;
  canCHANNELDATA_MAX_BITRATE = 37;
  canCHANNELDATA_CHANNEL_CAP_MASK = 38;
  canCHANNELDATA_CUST_CHANNEL_NAME = 39;
  canCHANNELDATA_IS_REMOTE = 40;
  canCHANNELDATA_REMOTE_TYPE = 41;
  canCHANNELDATA_LOGGER_TYPE = 42;
  canCHANNELDATA_HW_STATUS = 43;
  canCHANNELDATA_FEATURE_EAN = 44;
  canCHANNELDATA_BUS_PARAM_LIMITS = 45;
  canCHANNELDATA_CLOCK_INFO = 46;
  canCHANNELDATA_CHANNEL_CAP_EX = 47;
  canCHANNEL_IS_EXCLUSIVE = $0001;
  canCHANNEL_IS_OPEN = $0002;
  canCHANNEL_IS_CANFD = $0004;
  canCHANNEL_IS_LIN = $0010;
  canCHANNEL_IS_LIN_MASTER = $0020;
  canCHANNEL_IS_LIN_SLAVE = $0040;
  canHWTYPE_NONE = 0;
  canHWTYPE_VIRTUAL = 1;
  canHWTYPE_LAPCAN = 2;
  canHWTYPE_CANPARI = 3;
  canHWTYPE_PCCAN = 8;
  canHWTYPE_PCICAN = 9;
  canHWTYPE_USBCAN = 11;
  canHWTYPE_PCICAN_II = 40;
  canHWTYPE_USBCAN_II = 42;
  canHWTYPE_SIMULATED = 44;
  canHWTYPE_ACQUISITOR = 46;
  canHWTYPE_LEAF = 48;
  canHWTYPE_PC104_PLUS = 50;
  canHWTYPE_PCICANX_II = 52;
  canHWTYPE_MEMORATOR_II = 54;
  canHWTYPE_MEMORATOR_PRO = 54;
  canHWTYPE_USBCAN_PRO = 56;
  canHWTYPE_IRIS = 58;
  canHWTYPE_BLACKBIRD = 58;
  canHWTYPE_MEMORATOR_LIGHT = 60;
  canHWTYPE_MINIHYDRA = 62;
  canHWTYPE_EAGLE = 62;
  canHWTYPE_BAGEL = 64;
  canHWTYPE_BLACKBIRD_V2 = 64;
  canHWTYPE_MINIPCIE = 66;
  canHWTYPE_USBCAN_KLINE = 68;
  canHWTYPE_ETHERCAN = 70;
  canHWTYPE_USBCAN_LIGHT = 72;
  canHWTYPE_USBCAN_PRO2 = 74;
  canHWTYPE_PCIE_V2 = 76;
  canHWTYPE_MEMORATOR_PRO2 = 78;
  canHWTYPE_LEAF2 = 80;
  canHWTYPE_MEMORATOR_V2 = 82;
  canHWTYPE_CANLINHYBRID = 84;
  canHWTYPE_DINRAIL = 86;
  canHWTYPE_U100 = 88;
  canHWTYPE_LEAF3 = 90;
  
  canCHANNEL_CAP_EXTENDED_CAN = $00000001;
  canCHANNEL_CAP_BUS_STATISTICS = $00000002;
  canCHANNEL_CAP_ERROR_COUNTERS = $00000004;
  canCHANNEL_CAP_RESERVED_2 = $00000008;
  canCHANNEL_CAP_GENERATE_ERROR = $00000010;
  canCHANNEL_CAP_GENERATE_OVERLOAD = $00000020;
  canCHANNEL_CAP_TXREQUEST = $00000040;
  canCHANNEL_CAP_TXACKNOWLEDGE = $00000080;
  canCHANNEL_CAP_VIRTUAL = $00010000;
  canCHANNEL_CAP_SIMULATED = $00020000;
  canCHANNEL_CAP_RESERVED_1 = $00040000;
  canCHANNEL_CAP_CAN_FD = $00080000;
  canCHANNEL_CAP_CAN_FD_NONISO = $00100000;
  canCHANNEL_CAP_SILENT_MODE = $00200000;
  canCHANNEL_CAP_SINGLE_SHOT = $00400000;
  canCHANNEL_CAP_LOGGER = $00800000;
  canCHANNEL_CAP_REMOTE_ACCESS = $01000000;
  canCHANNEL_CAP_SCRIPT = $02000000;
  canCHANNEL_CAP_LIN_HYBRID = $04000000;
  canCHANNEL_CAP_IO_API = $08000000;
  canCHANNEL_CAP_DIAGNOSTICS = $10000000;
  canCHANNEL_CAP_EX_BUSPARAMS_TQ = $0000000000000001;
  canCHANNEL_OPMODE_NONE = 1;
  canCHANNEL_OPMODE_INFRASTRUCTURE = 2;
  canCHANNEL_OPMODE_RESERVED = 3;
  canCHANNEL_OPMODE_ADHOC = 4;
  canDRIVER_CAP_HIGHSPEED = $00000001;
  canIOCTL_PREFER_EXT = 1;
  canIOCTL_PREFER_STD = 2;
  canIOCTL_CLEAR_ERROR_COUNTERS = 5;
  canIOCTL_SET_TIMER_SCALE = 6;
  canIOCTL_SET_TXACK = 7;
  canIOCTL_GET_RX_BUFFER_LEVEL = 8;
  canIOCTL_GET_TX_BUFFER_LEVEL = 9;
  canIOCTL_FLUSH_RX_BUFFER = 10;
  canIOCTL_FLUSH_TX_BUFFER = 11;
  canIOCTL_GET_TIMER_SCALE = 12;
  canIOCTL_SET_TXRQ = 13;
  canIOCTL_GET_EVENTHANDLE = 14;
  canIOCTL_SET_BYPASS_MODE = 15;
  canIOCTL_SET_WAKEUP = 16;
  canIOCTL_GET_DRIVERHANDLE = 17;
  canIOCTL_MAP_RXQUEUE = 18;
  canIOCTL_GET_WAKEUP = 19;
  canIOCTL_SET_REPORT_ACCESS_ERRORS = 20;
  canIOCTL_GET_REPORT_ACCESS_ERRORS = 21;
  canIOCTL_CONNECT_TO_VIRTUAL_BUS = 22;
  canIOCTL_DISCONNECT_FROM_VIRTUAL_BUS = 23;
  canIOCTL_SET_USER_IOPORT = 24;
  canIOCTL_GET_USER_IOPORT = 25;
  canIOCTL_SET_BUFFER_WRAPAROUND_MODE = 26;
  canIOCTL_SET_RX_QUEUE_SIZE = 27;
  canIOCTL_SET_USB_THROTTLE = 28;
  canIOCTL_GET_USB_THROTTLE = 29;
  canIOCTL_SET_BUSON_TIME_AUTO_RESET = 30;
  canIOCTL_GET_TXACK = 31;
  canIOCTL_SET_LOCAL_TXECHO = 32;
  canIOCTL_SET_ERROR_FRAMES_REPORTING = 33;
  canIOCTL_GET_CHANNEL_QUALITY = 34;
  canIOCTL_GET_ROUNDTRIP_TIME = 35;
  canIOCTL_GET_BUS_TYPE = 36;
  canIOCTL_GET_DEVNAME_ASCII = 37;
  canIOCTL_GET_TIME_SINCE_LAST_SEEN = 38;
  canIOCTL_GET_TREF_LIST = 39;
  canIOCTL_TX_INTERVAL = 40;
  canIOCTL_SET_BRLIMIT = 43;
  canIOCTL_SET_USB_THROTTLE_SCALED = 41;
  canIOCTL_SET_THROTTLE_SCALED = 41;
  canIOCTL_GET_USB_THROTTLE_SCALED = 42;
  canIOCTL_GET_THROTTLE_SCALED = 42;
  canIOCTL_RESET_OVERRUN_COUNT = 44;
  canIOCTL_LIN_MODE = 45;
  canVERSION_CANLIB32_VERSION = 0;
  canVERSION_CANLIB32_PRODVER = 1;
  canVERSION_CANLIB32_PRODVER32 = 2;
  canVERSION_CANLIB32_BETA = 3;
  canOBJBUF_TYPE_AUTO_RESPONSE = $01;
  canOBJBUF_TYPE_PERIODIC_TX = $02;
  canOBJBUF_AUTO_RESPONSE_RTR_ONLY = $01;
  canVERSION_DONT_ACCEPT_LATER = $01;
  canVERSION_DONT_ACCEPT_BETAS = $02;
  kvLED_ACTION_ALL_LEDS_ON = 0;
  kvLED_ACTION_ALL_LEDS_OFF = 1;
  kvLED_ACTION_LED_0_ON = 2;
  kvLED_ACTION_LED_0_OFF = 3;
  kvLED_ACTION_LED_1_ON = 4;
  kvLED_ACTION_LED_1_OFF = 5;
  kvLED_ACTION_LED_2_ON = 6;
  kvLED_ACTION_LED_2_OFF = 7;
  kvLED_ACTION_LED_3_ON = 8;
  kvLED_ACTION_LED_3_OFF = 9;
  kvLED_ACTION_LED_4_ON = 10;
  kvLED_ACTION_LED_4_OFF = 11;
  kvLED_ACTION_LED_5_ON = 12;
  kvLED_ACTION_LED_5_OFF = 13;
  kvLED_ACTION_LED_6_ON = 14;
  kvLED_ACTION_LED_6_OFF = 15;
  kvLED_ACTION_LED_7_ON = 16;
  kvLED_ACTION_LED_7_OFF = 17;
  kvLED_ACTION_LED_8_ON = 18;
  kvLED_ACTION_LED_8_OFF = 19;
  kvLED_ACTION_LED_9_ON = 20;
  kvLED_ACTION_LED_9_OFF = 21;
  kvLED_ACTION_LED_10_ON = 22;
  kvLED_ACTION_LED_10_OFF = 23;
  kvLED_ACTION_LED_11_ON = 24;
  kvLED_ACTION_LED_11_OFF = 25;
  kvBUSTYPE_NONE = 0;
  kvBUSTYPE_PCI = 1;
  kvBUSTYPE_PCMCIA = 2;
  kvBUSTYPE_USB = 3;
  kvBUSTYPE_WLAN = 4;
  kvBUSTYPE_PCI_EXPRESS = 5;
  kvBUSTYPE_ISA = 6;
  kvBUSTYPE_VIRTUAL = 7;
  kvBUSTYPE_PC104_PLUS = 8;
  kvBUSTYPE_LAN = 9;
  kvBUSTYPE_GROUP_VIRTUAL = 1;
  kvBUSTYPE_GROUP_LOCAL = 2;
  kvBUSTYPE_GROUP_REMOTE = 3;
  kvBUSTYPE_GROUP_INTERNAL = 4;
  kvENVVAR_TYPE_INT = 1;
  kvENVVAR_TYPE_FLOAT = 2;
  kvENVVAR_TYPE_STRING = 3;
  kvEVENT_TYPE_KEY = 1;
  kvSCRIPT_STOP_NORMAL = 0;
  kvSCRIPT_STOP_FORCED = -9;
  kvSCRIPT_REQUEST_TEXT_UNSUBSCRIBE = 1;
  kvSCRIPT_REQUEST_TEXT_SUBSCRIBE = 2;
  kvSCRIPT_REQUEST_TEXT_ALL_SLOTS = 255;
  kvSCRIPT_STATUS_LOADED = 1;
  kvSCRIPT_STATUS_RUNNING = 2;
  canTXEDATA_FILE_VERSION = 1;
  canTXEDATA_COMPILER_VERSION = 2;
  canTXEDATA_DATE = 3;
  canTXEDATA_DESCRIPTION = 4;
  canTXEDATA_SOURCE = 5;
  canTXEDATA_SIZE_OF_CODE = 6;
  canTXEDATA_IS_ENCRYPTED = 7;
  kvDEVICE_MODE_INTERFACE = $00;
  kvDEVICE_MODE_LOGGER = $01;
  canTIMER_CYCLIC = $01;
  canTIMER_EXPENSIVE = $02;
  kvIO_INFO_GET_MODULE_TYPE = 1;
  kvIO_INFO_GET_DIRECTION = 2;
  kvIO_INFO_GET_PIN_TYPE = 4;
  kvIO_INFO_GET_NUMBER_OF_BITS = 5;
  kvIO_INFO_GET_RANGE_MIN = 6;
  kvIO_INFO_GET_RANGE_MAX = 7;
  kvIO_INFO_GET_DI_LOW_HIGH_FILTER = 8;
  kvIO_INFO_GET_DI_HIGH_LOW_FILTER = 9;
  kvIO_INFO_GET_AI_LP_FILTER_ORDER = 10;
  kvIO_INFO_GET_AI_HYSTERESIS = 11;
  kvIO_INFO_GET_MODULE_NUMBER = 14;
  kvIO_INFO_GET_SERIAL_NUMBER = 15;
  kvIO_INFO_GET_FW_VERSION = 16;
  kvIO_INFO_SET_DI_LOW_HIGH_FILTER = 8;
  kvIO_INFO_SET_DI_HIGH_LOW_FILTER = 9;
  kvIO_INFO_SET_AI_LP_FILTER_ORDER = 10;
  kvIO_INFO_SET_AI_HYSTERESIS = 11;
  kvIO_MODULE_TYPE_DIGITAL = 1;
  kvIO_MODULE_TYPE_ANALOG = 2;
  kvIO_MODULE_TYPE_RELAY = 3;
  kvIO_MODULE_TYPE_INTERNAL = 4;
  kvIO_PIN_TYPE_DIGITAL = 1;
  kvIO_PIN_TYPE_ANALOG = 2;
  kvIO_PIN_TYPE_RELAY = 3;
  kvIO_PIN_DIRECTION_IN = 4;
  kvIO_PIN_DIRECTION_OUT = 8;

type
  canStatus = Integer;
  PcanStatus = ^canStatus;

const
  (**
   * <b>Normal successful completion</b>; The driver is just fine, and really
   * believes it carried out your command to everyone's satisfaction.
   *)
  canOK = 0;
  (**
   * <b>Error in one or more parameters</b>; a parameter
   * specified in the call was invalid, out of range, or so. This status code
   * will also be returned when the call is not implemented.
   *)
  canERR_PARAM = -1;
  (**
   * <b>There were no messages to read</b>; A function tried to read a message,
   * but there was no message to read.
   *)
  canERR_NOMSG = -2;
  (**
   * <b>Specified device or channel not found.</b> There is no hardware
   * available that matches the given search criteria. For example, you may
   * have specified \ref canOPEN_REQUIRE_EXTENDED but there's no controller
   * capable of extended CAN. You may have specified a channel number that is
   * out of the range for the hardware in question. You may have requested
   * exclusive access to a channel, but the channel is already occupied.
   *)
  canERR_NOTFOUND = -3;
  (**
   * <b>Out of memory</b>; A memory allocation failed.
   *)
  canERR_NOMEM = -4;
  (**
   * <b>No channels available</b>; There is indeed hardware matching the
   * criteria you specified, but there are no channels available, or the
   * channel you specified is already occupied.
   *)
  canERR_NOCHANNELS = -5;
  /// Interrupted by signals
  canERR_INTERRUPTED = -6;
  (**
   * <b>Timeout occurred</b>; A function waited for something to happen (for
   * example, the arrival of a message), but that something didn't happen.
   *)
  canERR_TIMEOUT = -7;
  (**
   * <b>The library is not initialized</b>; The driver is not
   * initialized. \ref canInitializeLibrary() was probably not called?
   *)
  canERR_NOTINITIALIZED = -8;
  (**
   * <b>Out of handles</b>; No handles are available inside canlib32. The
   * application has too many handles open (i.e. has called \ref canOpenChannel() too
   * many times, or there's a memory leak somewhere.)
   *
   * \note We are not talking about Windows handles here, it's CANLIB's own
   * internal handles.
   *)
  canERR_NOHANDLES = -9;
  (**
   * <b>Handle is invalid</b>; The CANLIB handle you specified (if the API call
   * includes a handle) is not valid. Ensure you are passing the handle and
   * not, for example, a channel number.
   *)
  canERR_INVHANDLE = -10;
  /// Error in the ini-file (16-bit only)
  canERR_INIFILE = -11;
  (**
   * <b>Driver type not supported</b>; CAN driver mode is not supported by the
   * present hardware.
   *)
  canERR_DRIVER = -12;
  (**
   * <b>Transmit buffer overflow</b>; The transmit queue was full, so the
   * message was dropped.
   *)
  canERR_TXBUFOFL = -13;
  /// Reserved
  canERR_RESERVED_1 = -14;
  (**
   * <b>A hardware error has occurred</b>; Something probably related to the
   * hardware happened. This could mean that the device does not respond (IRQ
   * or address conflict?), or that the response was invalid or unexpected
   * (faulty card?).
   *)
  canERR_HARDWARE = -15;
  (**
   * <b>A driver DLL can't be found or loaded</b>; (One of) the DLL(s)
   * specified in the registry failed to load. This could be a driver
   * installation problem.
   *)
  canERR_DYNALOAD = -16;
  (**
   * <b>A DLL seems to have wrong version</b>; DLL version mismatch. (One of)
   * the DLL(s) specified in the registry is - probably - too old, or - less
   * likely - too new.
   *)
  canERR_DYNALIB = -17;
  (**
   * <b>Error when initializing a DLL</b>; Something failed when a device
   * driver was being initialized. In other words, we can open the driver but
   * it makes a lot of fuss about something we don't understand.
   *)
  canERR_DYNAINIT = -18;
  /// Operation not supported by hardware or firmware
  canERR_NOT_SUPPORTED = -19;
  /// Reserved
  canERR_RESERVED_5 = -20;
  /// Reserved
  canERR_RESERVED_6 = -21;
  /// Reserved
  canERR_RESERVED_2 = -22;
  (**
   * <b>Can't find or load kernel driver</b>; A device driver (kernel mode
   * driver for NT, VxD for W95/98) failed to load; or the DLL could not open
   * the device. Privileges? Driver file missing?
   *)
  canERR_DRIVERLOAD = -23;
  (**
   * <b>DeviceIOControl failed</b>; Use Win32 GetLastError() to learn what
   * really happened.
   *)
  canERR_DRIVERFAILED = -24;
  /// Can't find req'd config s/w (e.g. CS/SS)
  canERR_NOCONFIGMGR = -25;
  /// The card was removed or not inserted
  canERR_NOCARD = -26;
  /// Reserved
  canERR_RESERVED_7 = -27;
  (**
   * <b>Error (missing data) in the Registry</b>; A registry key is missing,
   * invalid, malformed, has gone for lunch or what not. can_verify.exe might
   * provide some insight.
   *)
  canERR_REGISTRY = -28;
  /// The license is not valid.
  canERR_LICENSE = -29;
  (**
   * <b>Internal error in the driver</b>; Indicates an error condition in the
   * driver or DLL, which couldn't be properly handled. Please contact the
   * friendly support at support@kvaser.com.
   *)
  canERR_INTERNAL = -30;
  (**
   * <b>Access denied</b>; This means that you tried to set the bit rate on a
   * handle to which you haven't got init access or you tried to open a channel
   * that already is open with init access. See \ref canOpenChannel() for more
   * information about init access.
   *)
  canERR_NO_ACCESS = -31;
  (**
   * <b>Not implemented</b>; The requested feature or function is not
   * implemented in the device you are trying to use it on.
   *)
  canERR_NOT_IMPLEMENTED = -32;
  (**
   * <b>Device File error</b>; An error has occured when trying to access a
   * file on the device.
   *)
  canERR_DEVICE_FILE = -33;
  (**
   * <b>Host File error</b>; An error has occured when trying to access a file on the host.
   *
   *)
  canERR_HOST_FILE = -34;
  (**
   * <b>Disk error</b>; A disk error has occurred. Verify that the disk is
   * initialized.
   *)
  canERR_DISK = -35;
  (**
   * <b>CRC error</b>; The CRC calculation did not match the expected result.
   *)
  canERR_CRC = -36;
  (**
   * <b>Configuration Error</b>; The configuration is corrupt.
   *)
  canERR_CONFIG = -37;
  (**
   * <b>Memo Error</b>; Other configuration error.
   *)
  canERR_MEMO_FAIL = -38;
  (**
   * <b>Script Fail</b>; A script has failed.
   *
   * \note This code represents several different failures, for example:
   * - Trying to load a corrupt file or not a .txe file
   * - Trying to start a t script that has not been loaded
   * - Trying to load a t script compiled with the wrong version of the t compiler
   * - Trying to unload a t script that has not been stopped
   * - Trying to use an envvar that does not exist
   *)
  canERR_SCRIPT_FAIL = -39;
  (**
   * <b>The t script version dosen't match the version(s) that the device firmware supports.</b>;
   *)
  canERR_SCRIPT_WRONG_VERSION = -40;
  (**
   * <b>The compiled t script container file format is of a version which is not supported by this version of canlib.</b>;
   *)
  canERR_SCRIPT_TXE_CONTAINER_VERSION = -41;
  (**
   * <b>An error occured while trying to parse the compiled t script file.</b>;
   *)
  canERR_SCRIPT_TXE_CONTAINER_FORMAT = -42;
  (**
   * <b>The buffer provided was not large enough to contain the requested data.</b>;
   *)
  canERR_BUFFER_TOO_SMALL = -43;
  (**
   * <b>The I/O pin doesn't exist or the I/O pin type doesn't match the called
     function, e.g. trying to use input pins as outputs or use digital pins as analog pins. </b>;
   *)
  canERR_IO_WRONG_PIN_TYPE = -44;
  (**
   * <b>The I/O pin configuration is not confirmed. Use kvIoConfirmConfig()
     to confirm the configuration.</b>;
   *)
  canERR_IO_NOT_CONFIRMED = -45;
  (**
   * <b>The I/O pin configuration has changed after last call to kvIoConfirmConfig. Use kvIoConfirmConfig()
     to confirm the new configuration. </b>;
   *)
  canERR_IO_CONFIG_CHANGED = -46;
  (**
   * <b>The previous I/O pin value has not yet changed the output and is still pending.
     This happens when e.g. kvIoPinSetAnalog() is called twice on the same pin
     within a short time. </b>;
   *)
  canERR_IO_PENDING = -47;
  (**
   * <b>There is no valid I/O pin configuration.
   * </b>
   *)
  canERR_IO_NO_VALID_CONFIG = -48;
  /// Reserved
  canERR__RESERVED = -49;

type
  // Forward declarations
  PInt64 = ^Int64;
  PkvBusParamsTq = ^kvBusParamsTq;
  PkvClockInfo = ^kvClockInfo;
  PkvBusParamLimits = ^kvBusParamLimits;
  PcanUserIoPortData = ^canUserIoPortData;
  PcanBusStatistics_s = ^canBusStatistics_s;
  PkvTimeDomainData_s = ^kvTimeDomainData_s;
  PkvIoModuleDigital = ^kvIoModuleDigital;
  PkvIoModuleInternal = ^kvIoModuleInternal;
  PkvIoModuleRelay = ^kvIoModuleRelay;
  PkvIoModuleAnalog = ^kvIoModuleAnalog;

  (**
   *\b Constraints
   \verbatim
   Constraints that must be fulfilled when opening channel in classic CAN Mode:
  
     tq         = 1 + prop + phase1 + phase2
     tq        >= 3
     sjw       <= min(phase1, phase2)
     prescaler >= 1
   \endverbatim
   *
   \verbatim
     Constraints that must be fulfilled when opening channel in CAN FD Mode:
  
     arbitration.tq         = 1 + arbitration.prop + arbitration.phase1 + arbitration.phase2
     arbitration.tq        >= 3
     arbitration.sjw       <= min(arbitration.phase1, arbitration.phase2)
     arbitration.prescaler >= 1
     arbitration.prescaler <= 2
  
     data.tq         = 1 + data.phase1 + data.phase2
     data.tq        >= 3
     data.sjw       <= min(data.phase1, data.phase2)
     data.prop       = 0
     data.prescaler  = arbitration.prescaler
   \endverbatim
   *
   * Used in \ref canSetBusParamsTq, \ref canSetBusParamsFdTq, \ref canGetBusParamsTq and \ref canGetBusParamsTq
   *)
  kvBusParamsTq = record
    (** Total bit time, in number of time quanta. *)
    tq: Integer;
    (** Phase segment 1, in number of time quanta *)
    phase1: Integer;
    (** Phase segment 2, in number of time quanta *)
    phase2: Integer;
    (** Sync jump width, in number of time quanta *)
    sjw: Integer;
    (** Propagation segment, in number of time quanta *)
    prop: Integer;
    (** Prescaler *)
    prescaler: Integer;
  end;

  (** Handle to an opened circuit, created with \ref canOpenChannel(). *)
  canHandle = Integer;

  (**
   * Returned when using \ref canCHANNELDATA_CLOCK_INFO
   *
   * Returns clock characteristics for device.
   *
   * The device clock frequency can then be calculated as:
   *
   * double frequency = numerator/denominator * 10 ** power_of_ten;
   *)
  kvClockInfo = record
    (** The version of this struct, currently 1 *)
    version: Integer;
    (** The numerator part of the device clock frequency. *)
    numerator: Integer;
    (** The denominator part of the device clock frequency. *)
    denominator: Integer;
    (** The power_of_ten part of the device clock frequency. *)
    power_of_ten: Integer;
    (** The accuracy (in ppm) of the device clock. *)
    accuracy_ppm: Integer;
  end;

  (**
   * Returned when using \ref canCHANNELDATA_BUS_PARAM_LIMITS
   *
   * This struct shows the low level limits of the parameters.
   *
   * Note that seg1 = prop + phase1 and seg2 = phase2
   *)
  kvBusParamLimits = record
    (** The version of this struct, currently 1 *)
    version: Integer;
    (** Number of bits used to specify brp and d_brp *)
    brp_size: Integer;
    (** Number of bits used to specify prop + phase1 and d_phase1. *)
    seg1_size: Integer;
    (** Number of bits used to specify phase2 and d_phase2. *)
    seg2_size: Integer;
    (** Number of bits used to specify sjw and d_sjw *)
    sjw_size: Integer;
  end;

  (** Used in \ref canIOCTL_SET_USER_IOPORT and \ref canIOCTL_GET_USER_IOPORT. *)
  canUserIoPortData = record
    /// Port number used in e.g. \ref canIOCTL_SET_USER_IOPORT
    portNo: Cardinal;
    /// Port value used in e.g. \ref canIOCTL_SET_USER_IOPORT
    portValue: Cardinal;
  end;

  (**
   * This struct is returned by \ref canGetBusStatistics()
   *
   * The values are cleared when the corresponding channel goes on bus.
   *)
  canBusStatistics_s = record
    /// Number of received standard (11-bit identifiers) data frames.
    stdData: Cardinal;
    /// Number of received standard (11-bit identifiers) remote frames.
    stdRemote: Cardinal;
    /// Number of received extended (29-bit identifiers) data frames.
    extData: Cardinal;
    /// Number of received extended (29-bit identifiers) remote frames.
    extRemote: Cardinal;
    /// Number of error frames
    errFrame: Cardinal;
    (**
     * The bus load, expressed as an integer in the interval 0 - 10000
     * representing 0.00% - 100.00% bus load.
     *)
    busLoad: Cardinal;
    /// The number of overruns detected by the hardware, firmware or driver.
    overruns: Cardinal;
  end;

  canBusStatistics = canBusStatistics_s;
  PcanBusStatistics = ^canBusStatistics;
  (** Used for time domain handling. *)
  kvTimeDomain = Pointer;
  PkvTimeDomain = ^kvTimeDomain;
  (** Contains status codes according to \ref canSTAT_xxx. *)
  kvStatus = canStatus;

  (**
   *
   * Used for time domain handling.
   *)
  kvTimeDomainData_s = record
    /// number of MagiSync&tm; groups
    nMagiSyncGroups: Integer;
    /// number of MagiSync&tm; members
    nMagiSyncedMembers: Integer;
    /// number of non MagiSync&tm; interfaces
    nNonMagiSyncCards: Integer;
    /// number of non MagiSync&tm; members
    nNonMagiSyncedMembers: Integer;
  end;

  kvTimeDomainData = kvTimeDomainData_s;
  PkvTimeDomainData = ^kvTimeDomainData;

  (**
   * \ref kvCallback_t is used by the function \ref kvSetNotifyCallback()
   *
   * The callback function, on Windows required to be a \c __stdcall function, should be called with the following arguments:
   *
   * \li hnd - the handle of the CAN channel where the event happened.
   * \li context - the context pointer you passed to \ref kvSetNotifyCallback().
   * \li notifyEvent - one of the \ref canNOTIFY_xxx notification codes.
   *
   * \note It is really the \ref canNOTIFY_xxx codes, and not the
   *  \ref canEVENT_xxx codes that the \ref canSetNotify() API is using.
   *
   * \note The default calling convention for C/C++ programs on Windows is \c __cdecl.
   * <br>On Linux, the default calling convention is used.
   *
   * \sa \ref section_user_guide_send_recv_asynch_callback
   *)
  kvCallback_t = procedure(hnd: Integer; context: Pointer; notifyEvent: Cardinal); stdcall;
  (**
   *
   * A handle to a t-script envvar.
   * Returned by the function \ref kvScriptEnvvarOpen().
   *)
  kvEnvHandle = Int64;

  (**
   * This define is used in \ref kvIoGetModulePins() and \ref kvIoSetModulePins.
   *
   * This struct represents a digital add-on module.
   *)
  kvIoModuleDigital = record
    (** The type of the module. See: \ref kvIO_MODULE_TYPE_xxx. *)
    &type: Integer;
    (** Digital Out *)
    DO1: Byte;
    (** Digital Out *)
    DO2: Byte;
    (** Digital Out *)
    DO3: Byte;
    (** Digital Out *)
    DO4: Byte;
    (** Digital Out *)
    DO5: Byte;
    (** Digital Out *)
    DO6: Byte;
    (** Digital Out *)
    DO7: Byte;
    (** Digital Out *)
    DO8: Byte;
    (** Digital Out *)
    DO9: Byte;
    (** Digital Out *)
    DO10: Byte;
    (** Digital Out *)
    DO11: Byte;
    (** Digital Out *)
    DO12: Byte;
    (** Digital Out *)
    DO13: Byte;
    (** Digital Out *)
    DO14: Byte;
    (** Digital Out *)
    DO15: Byte;
    (** Digital Out *)
    DO16: Byte;
    (** Digital In *)
    DI1: Byte;
    (** Digital In *)
    DI2: Byte;
    (** Digital In *)
    DI3: Byte;
    (** Digital In *)
    DI4: Byte;
    (** Digital In *)
    DI5: Byte;
    (** Digital In *)
    DI6: Byte;
    (** Digital In *)
    DI7: Byte;
    (** Digital In *)
    DI8: Byte;
    (** Digital In *)
    DI9: Byte;
    (** Digital In *)
    DI10: Byte;
    (** Digital In *)
    DI11: Byte;
    (** Digital In *)
    DI12: Byte;
    (** Digital In *)
    DI13: Byte;
    (** Digital In *)
    DI14: Byte;
    (** Digital In *)
    DI15: Byte;
    (** Digital In *)
    DI16: Byte;
  end;

  (**
   * /struct
   * This define is used in \ref kvIoGetModulePins() and \ref kvIoSetModulePins().
   * This strcut represents a digital internal module
   *)
  kvIoModuleInternal = record
    (** The type of the module. See: \ref kvIO_MODULE_TYPE_xxx.. *)
    &type: Integer;
    (** Digital Out *)
    &DO: Byte;
    (** Digital In *)
    DI: Byte;
  end;

  (**
   * /struct
   * This define is used in \ref kvIoGetModulePins() and \ref kvIoSetModulePins().
   * This struct represents a relay add-on module.
   *)
  kvIoModuleRelay = record
    (** The type of the module. See: \ref kvIO_MODULE_TYPE_xxx.. *)
    &type: Integer;
    (** Relay Out *)
    RO1: Byte;
    (** Relay Out *)
    RO2: Byte;
    (** Relay Out *)
    RO3: Byte;
    (** Relay Out *)
    RO4: Byte;
    (** Relay Out *)
    RO5: Byte;
    (** Relay Out *)
    RO6: Byte;
    (** Relay Out *)
    RO7: Byte;
    (** Relay Out *)
    RO8: Byte;
    (** Digital In *)
    DI1: Byte;
    (** Digital In *)
    DI2: Byte;
    (** Digital In *)
    DI3: Byte;
    (** Digital In *)
    DI4: Byte;
    (** Digital In *)
    DI5: Byte;
    (** Digital In *)
    DI6: Byte;
    (** Digital In *)
    DI7: Byte;
    (** Digital In *)
    DI8: Byte;
  end;

  (**
   *
   * This define is used in \ref kvIoGetModulePins() and \ref kvIoSetModulePins().
   * This struct represents an analog add-on module.
   *)
  kvIoModuleAnalog = record
    (** The type of the module. See: \ref kvIO_MODULE_TYPE_xxx.. *)
    &type: Integer;
    (** Analog Out *)
    AO1: Single;
    (** Analog Out *)
    AO2: Single;
    (** Analog Out *)
    AO3: Single;
    (** Analog Out *)
    AO4: Single;
    (** Analog In *)
    AI1: Single;
    (** Analog In *)
    AI2: Single;
    (** Analog In *)
    AI3: Single;
    (** Analog In *)
    AI4: Single;
  end;

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static void canInitializeLibrary();</b>
 *
 * \source_delphi   <b>procedure canInitializeLibrary;    </b>
 * \source_end
 * This function must be called before any other functions is used.  It will
 * initialize the driver.
 *
 * You may call \ref canInitializeLibrary() more than once. The actual
 * initialization will take place only once.
 *
 * Any errors encountered during library initialization will be "silent" and an
 * appropriate \ref canERR_xxx error code will be returned later on when
 * \ref canOpenChannel() (or any other API call that requires initialization) is
 * called.
 *
 * \sa \ref page_user_guide_init
 *
 *)
procedure canInitializeLibrary(); stdcall;
  external canlib32 name _PU + 'canInitializeLibrary';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.canStatus canEnumHardwareEx(out Int32 channelCount);</b>
 *
 * \source_delphi   <b>function canEnumHardwareEx(var channelCount: Integer): canStatus;</b>
 * \source_end
 *
 * \param[out]  channelCount Number of channels present.
 *
 * This function will re-enumerate all currently available CAN channels while
 * not affecting already opened channel handles.
 *
 * \note When using this function, make sure your program does not keep any
 * references to CANlib channel numbers since these numbers may change.<br>
 * On Linux, no re-enumeration is needed since enumeration takes place
 * when a device is plugged in or unplugged.
 *
 * \sa \ref section_user_guide_enumerate_hw
 *
 *)
function canEnumHardwareEx(var channelCount: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canEnumHardwareEx';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canClose(CanHandle handle);</b>
 *
 * \source_delphi   <b>function canClose(handle: canHandle): canStatus;</b>
 * \source_end
 *
 * Closes the channel associated with the handle. If no other threads
 * are using the CAN circuit, it is taken off bus. The handle can not be
 * used for further references to the channel, so any variable containing
 * it should be zeroed.
 *
 * \ref canClose() will almost always return \ref canOK; the specified handle is closed
 * on an best-effort basis.
 *
 * \param[in]  hnd  An open handle to a CAN channel.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_init_sel_channel_close
 * \sa \ref canOpenChannel(), \ref canBusOn(), \ref canBusOff()
 *)
function canClose(handle: canHandle): canStatus; stdcall;
  external canlib32 name _PU + 'canClose';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canBusOn(CanHandle handle);</b>
 *
 * \source_delphi   <b>function canBusOn(handle: canHandle): canStatus;    </b>
 * \source_end
 *
 * Takes the specified channel on-bus.
 *
 * If you are using multiple handles to the same physical channel, for example
 * if you are writing a threaded application, you must call \ref canBusOn() once for
 * each handle. The same applies to \ref canBusOff() - the physical channel will not
 * go off bus until the last handle to the channel goes off bus.
 *
 * \param[in]  hnd  An open handle to a CAN channel.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_bus_on_off
 * \sa \ref canBusOff(), \ref canResetBus()
 *
 *)
function canBusOn(handle: canHandle): canStatus; stdcall;
  external canlib32 name _PU + 'canBusOn';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canBusOff(CanHandle handle);</b>
 *
 * \source_delphi   <b>function canBusOff(handle: canHandle): canStatus; </b>
 * \source_end
 *
 * Takes the specified handle off-bus. If no other handle is active on the same
 * channel, the channel will also be taken off-bus
 *
 * \param[in]  hnd  An open handle to a CAN channel.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_bus_on_off
 * \sa \ref canBusOn(), \ref canResetBus()
 *
 *)
function canBusOff(handle: canHandle): canStatus; stdcall;
  external canlib32 name _PU + 'canBusOff';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canSetBusParams(CanHandle handle, Int32 freq, Int32 tseg1, Int32 tseg2, Int32 sjw, Int32 noSamp); </b>
 *
 * \source_delphi   <b>function canSetBusParams(handle: canHandle; freq: Longint; tseg1, tseg2, sjw, noSamp, syncmode: Cardinal): canStatus;     </b>
 * \source_end
 *
 * This function sets the nominal bus timing parameters for the specified CAN
 * controller.
 *
 * The library provides default values for \a tseg1, \a tseg2, \a sjw and \a
 * noSamp when \a freq is specified to one of the pre-defined constants,
 * \ref canBITRATE_xxx for classic CAN and \ref canFD_BITRATE_xxx for CAN FD.
 *
 * If \a freq is any other value, no default values are supplied by the
 * library.
 *
 * If you are using multiple handles to the same physical channel, for example
 * if you are writing a threaded application, you must call \ref canBusOff() once
 * for each handle. The same applies to \ref canBusOn() - the physical channel will
 * not go off bus until the last handle to the channel goes off bus.
 *
 * \note The value of sjw should normally be less than tseg1 and tseg2.
 *
 * Use \ref canSetBusParamsC200() to set the bus timing parameters in the
 * ubiquitous 82c200 bit-timing register format.
 *
 * \param[in]  hnd       An open handle to a CAN controller.
 * \param[in]  freq      Bit rate (measured in bits per second); or one of the
 *                       predefined constants (\ref canBITRATE_xxx for classic
 *                       CAN and \ref canFD_BITRATE_xxx for CAN FD).
 * \param[in]  tseg1     Time segment 1, that is, the number of quanta from (but not
 *                       including) the Sync Segment to the sampling point.
 * \param[in]  tseg2     Time segment 2, that is, the number of quanta from the sampling
 *                       point to the end of the bit.
 * \param[in]  sjw       The Synchronization Jump Width.
 * \param[in]  noSamp    The number of sampling points; can be 1 or 3.
 * \param[in]  syncmode  Unsupported and ignored.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_init_bit_rate_can,
 * \ref section_user_guide_init_bit_rate_canfd,
 * \sa \ref canSetBusParamsFd(), \ref canSetBusParamsC200(), \ref canGetBusParams()
 * \sa \ref canSetBusParamsTq(), \ref canSetBusParamsFdTq(), \ref canGetBusParamsTq()
 *
 *)
function canSetBusParams(handle: canHandle; freq: Longint; tseg1, tseg2, sjw, noSamp, syncmode: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canSetBusParams';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canSetBusParamsTq(CanHandle handle, kvBusParamsTq nominal);</b>
 *
 * \source_delphi   <b>function canSetBusParamsTq(hnd: canHandle; nominal: kvBusParamsTq): canStatus;</b>
 * \source_end
 *
 * Set bus parameters for the specified CAN controller (classic CAN).
 *
 * If the channel is opened (see \ref canOpenChannel()) with flags \ref canOPEN_CAN_FD or \ref canOPEN_CAN_FD_NONISO,
 * use \ref canSetBusParamsFdTq() instead.
 *
 * To get device specific limits of bus parameters, see \ref canCHANNELDATA_BUS_PARAM_LIMITS.
 *
 * The <a href="https://www.kvaser.com/support/calculators/can-fd-bit-timing-calculator/"> Kvaser Bit Timing calculator</a>,
 * available on the Kvaser website, can be used to calculate specific bit rates.
 *
 * \param[in] hnd      A handle to an open CAN circuit.
 * \param[in] nominal  See \ref kvBusParamsTq
 *
 *
 * return  canOK (zero)  if success
 * return  canERR_xxx    (negative) if failure
 *)
function canSetBusParamsTq(hnd: canHandle; nominal: kvBusParamsTq): canStatus; stdcall;
  external canlib32 name _PU + 'canSetBusParamsTq';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canSetBusParamsFd(CanHandle handle, Int32 freq_brs, Int32 tseg1_brs, Int32 tseg2_brs, Int32 sjw_brs);</b>
 *
 * \source_delphi   <b>function canSetBusParamsFd(hnd: canHandle; freq_brs: Longint; tseg1_brs, tseg2_brs, sjw_brs: Cardinal): canStatus;</b>
 * \source_end
 *
 * This function sets the data phase bus timing parameters for the specified
 * CAN controller.
 *
 * The library provides default values for \a tseg1_brs, \a tseg2_brs and
 * \a sjw_brs when \a freq_brs is specified to one of the pre-defined
 * constants, \ref canFD_BITRATE_xxx.
 *
 * If \a freq_brs is any other value, no default values are supplied
 * by the library.
 *
 * \param[in]  hnd        An open handle to a CAN controller.
 * \param[in]  freq_brs   CAN FD data bit rate (measured in bits per second); or
 *                        one of the predefined constants \ref
 *                        canFD_BITRATE_xxx.
 * \param[in]  tseg1_brs  Time segment 1, that is, the number of quanta from (but not
 *                        including) the Sync Segment to the sampling point.
 * \param[in]  tseg2_brs  Time segment 2, that is, the number of quanta from the sampling
 *                        point to the end of the bit.
 * \param[in]  sjw_brs    The Synchronization Jump Width.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canSetBusParamsFdTq(), \ref canGetBusParamsFdTq()
 *)
function canSetBusParamsFd(hnd: canHandle; freq_brs: Longint; tseg1_brs, tseg2_brs, sjw_brs: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canSetBusParamsFd';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canSetBusParamsFdTq(CanHandle handle, kvBusParamsTq arbitration, kvBusParamsTq data);</b>
 *
 * \source_delphi   <b>function canSetBusParamsFdTq(hnd: canHandle; arbitration: kvBusParamsTq; data: kvBusParamsTq): canStatus;</b>
 * \source_end
 *
 * Set bus parameters for the specified CAN controller.
 *
 * The channel \b MUST be opened with flags \ref canOPEN_CAN_FD or \ref canOPEN_CAN_FD_NONISO, see \ref canOpenChannel().
 * In the case of classic can, use \ref canSetBusParamsTq() instead.
 *
 * To get device specific limits of bus parameters, see \ref canCHANNELDATA_BUS_PARAM_LIMITS.
 *
 * The <a href="https://www.kvaser.com/support/calculators/can-fd-bit-timing-calculator/"> Kvaser Bit Timing calculator</a>,
 * available on the Kvaser website, can be used to calculate specific bit rates.
 *
 * \param[in] hnd      A handle to an open CAN circuit.
 * \param[in] arbitration  See \ref kvBusParamsTq
 * \param[in] data     See \ref kvBusParamsTq
 *
 *
 * return  canOK (zero)  if success
 * return  canERR_xxx    (negative) if failure
 *)
function canSetBusParamsFdTq(hnd: canHandle; arbitration: kvBusParamsTq; data: kvBusParamsTq): canStatus; stdcall;
  external canlib32 name _PU + 'canSetBusParamsFdTq';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canGetBusParams(CanHandle handle, out Int64 freq, out Int32 tseg1, out Int32 tseg2, out Int32 sjw, out Int32 noSamp);</b>
 *
 * \source_delphi   <b>function canGetBusParams(handle: canHandle; var freq: Longint; var tseg1, tseg2, sjw, noSamp, syncmode: Cardinal): canStatus;     </b>
 * \source_end
 *
 * This function retrieves the current nominal bus parameters for the specified
 * channel.
 *
 * The anatomy of a CAN bit is discussed in detail at Kvaser's
 * web site at <a href="http://www.kvaser.com">www.kvaser.com</a>.
 *
 * \param[in]  hnd       An open handle to a CAN controller.
 * \param[out] freq      Bit rate (bits per second).
 * \param[out] tseg1     Time segment 1, that is, the number of quanta from (but not
 *                       including) the Sync Segment to the sampling point.
 * \param[out] tseg2     Time segment 2, that is, the number of quanta from the sampling
 *                       point to the end of the bit.
 * \param[out] sjw       The Synchronization Jump Width.
 * \param[out] noSamp    The number of sampling points; can be 1 or 3.
 * \param[out] syncmode  Unsupported, always read as one.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_init_bit_rate_can, \ref section_user_guide_init_bit_rate_can
 * \sa \ref canSetBusParams(), \ref canSetBusParamsC200()
 *
 *)
function canGetBusParams(handle: canHandle; var freq: Longint; var tseg1, tseg2, sjw, noSamp, syncmode: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canGetBusParams';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canGetBusParamsTq(CanHandle handle, out kvBusParamsTq nominal);</b>
 *
 * \source_delphi   <b>function canGetBusParamsTq(handle: canHandle; var nominal: kvBusParamsTq): canStatus;     </b>
 * \source_end
 *
 * Get bus parameters for the specified CAN controller.
 *
 * If the channel is opened (see \ref canOpenChannel()) with flags \ref canOPEN_CAN_FD or \ref canOPEN_CAN_FD_NONISO,
 * use \ref canGetBusParamsFdTq() instead.
 *
 * \param[in]  hnd      A handle to an open CAN circuit.
 * \param[out] nominal  See \ref kvBusParamsTq.
 *
 * return  canOK (zero)  if success
 * return  canERR_xxx    (negative) if failure
 *)
function canGetBusParamsTq(handle: canHandle; var nominal: kvBusParamsTq): canStatus; stdcall;
  external canlib32 name _PU + 'canGetBusParamsTq';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canGetBusParamsFd(CanHandle handle, out Int64 freq_brs, out Int32 tseg1_brs, out Int32 tseg2_brs, out Int32 sjw_brs);</b>
 *
 * \source_delphi   <b>function canGetBusParamsFd(hnd: canHandle; var freq_brs: Longint; var tseg1_brs, tseg2_brs, sjw_brs: Cardinal): canStatus;</b>
 * \source_end
 *
 * This function retrieves the current data bus parameters for the specified
 * CAN FD channel.
 *
 * \param[in]  hnd         An open handle to a CAN FD controller.
 * \param[out] freq_brs    Bit rate (bits per second).
 * \param[out] tseg1_brs   Time segment 1, that is, the number of quanta from (but not
 *                         including) the Sync Segment to the sampling point.
 * \param[out] tseg2_brs   Time segment 2, that is, the number of quanta from the sampling
 *                         point to the end of the bit.
 * \param[out] sjw_brs     The Synchronization Jump Width.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *)
function canGetBusParamsFd(hnd: canHandle; var freq_brs: Longint; var tseg1_brs, tseg2_brs, sjw_brs: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canGetBusParamsFd';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canGetBusParamsFdTq(CanHandle handle, out kvBusParamsTq nominal, out kvBusParamsTq data);</b>
 *
 * \source_delphi   <b>function canGetBusParamsFdTq(handle: canHandle; var nominal: kvBusParamsTq; var data: kvBusParamsTq): canStatus;     </b>
 * \source_end
 *
 * Get bus parameters for the specified CAN controller.
 *
 * If the channel is \b NOT opened (see \ref canOpenChannel()) with flags \ref canOPEN_CAN_FD or \ref canOPEN_CAN_FD_NONISO,
 * use \ref canGetBusParamsTq() instead.
 *
 * \param[in]  hnd      A handle to an open CAN circuit.
 * \param[out] nominal  See \ref kvBusParamsTq.
 * \param[out] data     See \ref kvBusParamsTq.
 *
 * return  canOK (zero)  if success
 * return  canERR_xxx    (negative) if failure
 *)
function canGetBusParamsFdTq(handle: canHandle; var nominal: kvBusParamsTq; var data: kvBusParamsTq): canStatus; stdcall;
  external canlib32 name _PU + 'canGetBusParamsFdTq';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canSetBusOutputControl(CanHandle handle, Int32 drivertype);</b>
 *
 * \source_delphi   <b>function canSetBusOutputControl(handle: canHandle; drivertype: Cardinal): canStatus;     </b>
 * \source_end
 *
 * This function sets the driver type for a CAN controller. This corresponds
 * loosely to the bus output control register in the CAN controller, hence the
 * name of this function. CANlib does not allow for direct manipulation of the
 * bus output control register; instead, symbolic constants are used to select
 * the desired driver type.
 *
 * \note Not all CAN driver types are supported on all cards.
 *
 * \param[in]  hnd         A handle to an open circuit.
 * \param[in]  drivertype  Can driver type, \ref canDRIVER_xxx)
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canGetBusOutputControl()
 *)
function canSetBusOutputControl(handle: canHandle; drivertype: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canSetBusOutputControl';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canGetBusOutputControl(CanHandle handle, out Int32 drivertype);</b>
 *
 * \source_delphi   <b>function canGetBusOutputControl(handle: canHandle; var drivertype: Cardinal): canStatus;  </b>
 * \source_end
 *
 * This function retrieves the current CAN controller driver type.
 * This corresponds loosely to the bus output control register in the
 * CAN controller, hence the name of this function. CANlib does not
 * allow for direct manipulation of the bus output control register;
 * instead, symbolic constants are used to select the desired driver
 * type.
 *
 * \note Don't confuse the CAN controller driver type with the bus driver
 *       type. The CAN controller is not connected directly to the CAN bus;
 *       instead, it is connected to a bus transceiver circuit which interfaces
 *       directly to the bus. The "CAN controller driver type" we are talking
 *       about here refers to the mode which the CAN controller uses to drive
 *       the bus transceiver circuit.
 *
 * \note Silent Mode is not supported by all CAN controllers.
 *
 * \param[in]  hnd         An open handle to a CAN circuit.
 * \param[out] drivertype  A pointer to an unsigned int which receives the
 *                         current driver type. The driver type can be either
 *                         \ref canDRIVER_NORMAL or \ref canDRIVER_SILENT.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canSetBusOutputControl()
 *)
function canGetBusOutputControl(handle: canHandle; var drivertype: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canGetBusOutputControl';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canAccept(CanHandle handle, Int32 envelope, Int32 flag);</b>
 *
 * \source_delphi   <b>function canAccept(handle: canHandle; envelope: Longint; flag: Cardinal): canStatus;     </b>
 * \source_end
 *
 * This routine sets the message acceptance filters on a CAN channel.
 *
 * On some boards the acceptance filtering is done by the CAN hardware; on
 * other boards (typically those with an embedded CPU,) the acceptance
 * filtering is done by software. \ref canAccept() behaves in the same way for all
 * boards, however.
 *
 * \ref canSetAcceptanceFilter() and \ref canAccept() both serve the same purpose but the
 * former can set the code and mask in just one call.
 *
 * If you want to remove a filter, call \ref canAccept() with the mask set to 0.
 *
 * \note You can set the extended code and mask only on CAN boards that support
 *       extended identifiers.
 *
 * \note Acceptance filters for 11-bit and 29-bit (ext) are independent, meaning 11-bit filters
 *  will not affect 29-bit can frames and vice versa.
 *
 * \note Not all CAN boards support different masks for standard and extended
 *       CAN identifiers.
 *
 * \param[in]  hnd       An open handle to a CAN circuit.
 * \param[in]  envelope  The mask or code to set.
 * \param[in]  flag      Any of \ref canFILTER_SET_CODE_STD,
 *                       \ref canFILTER_SET_MASK_STD,
 *                       \ref canFILTER_SET_CODE_EXT or
 *                       \ref canFILTER_SET_MASK_EXT
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_filters
 * \sa \ref canSetAcceptanceFilter()
 *)
function canAccept(handle: canHandle; envelope: Longint; flag: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canAccept';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canReadStatus(CanHandle handle, out Int64 flags);</b>
 *
 * \source_delphi   <b>function canReadStatus(handle: canHandle; var flags: Cardinal): canStatus;     </b>
 * \source_end
 *
 * Returns the status for the specified circuit. flags points to a longword
 * which receives a combination of the \ref canSTAT_xxx flags.
 *
 * \note \ref canReadStatus() returns the latest known status of the specified
 *       circuit. If a status change happens precisely when \ref canReadStatus() is
 *       called, it may not be reflected in the returned result.
 *
 * \param[in]  hnd    A handle to an open circuit.
 * \param[out] flags  Pointer to a \c DWORD which receives the status flags;
 *                    this is a combination of any of the \ref canSTAT_xxx.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 *)
function canReadStatus(handle: canHandle; var flags: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canReadStatus';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canReadErrorCounters(CanHandle handle, out Int32 txErr, out Int32 rxErr, out Int32 ovErr);</b>
 *
 * \source_delphi   <b>function canReadErrorCounters(handle: canHandle; var txErr, rxErr, ovErr: Cardinal): canStatus;     </b>
 * \source_end
 *
 * Reads the error counters of the CAN controller.
 *
 * \ref canReadErrorCounters() returns the latest known values of the error counters
 * in the specified circuit. If the error counters change values precisely when
 * \ref canReadErrorCounters() is called, it may not be reflected in the returned
 * result.
 *
 * It is allowed to pass \c NULL as the value of the \a txErr, \a rxErr, and \a
 * ovErr parameters.
 *
 * Use \ref canIoCtl() to clear the counters.
 *
 * \note Not all CAN controllers provide access to the error counters;
 *       in this case, an educated guess is returned.
 *
 * \param[in]  hnd    A handle to an open circuit.
 * \param[out] txErr  A pointer to a \c DWORD which receives the transmit error
 *                    counter.
 * \param[out] rxErr  A pointer to a \c DWORD which receives the receive error
 *                    counter.
 * \param[out] ovErr  A pointer to a \c DWORD which receives the number of
 *                    overrun errors.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canIoCtl()
 *)
function canReadErrorCounters(handle: canHandle; var txErr, rxErr, ovErr: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canReadErrorCounters';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canWrite(CanHandle handle, Int32 id, Byte[] msg, Int32 dlc, Int32 flag);</b>
 *
 * \source_delphi   <b>function canWrite(handle: canHandle; id: Longint; msg: Pointer; dlc: Cardinal; flag: Cardinal): canStatus;     </b>
 * \source_end
 *
 * This function sends a CAN message. The call returns immediately after queuing
 * the message to the driver.
 *
 * If you are using the same channel via multiple handles, note that the
 * default behaviour is that the different handles will "hear" each other just as
 * if each handle referred to a channel of its own. If you open, say, channel 0
 * from thread A and thread B and then send a message from thread A, it will be
 * "received" by thread B.
 * This behaviour can be changed using \ref canIOCTL_SET_LOCAL_TXECHO.
 *
 * \note The message has been queued for transmission when this calls return.
 *       It has not necessarily been sent.
 *
 * \param[in]  hnd       A handle to an open CAN circuit.
 * \param[in]  id        The identifier of the CAN message to send.
 * \param[in]  msg       A pointer to the message data, or \c NULL.
 * \param[in]  dlc       The length of the message in bytes.<br>
                         For Classic CAN dlc can be at most 8, unless \ref canOPEN_ACCEPT_LARGE_DLC is used.<br>
                         For CAN FD dlc can be one of the following 0-8, 12, 16, 20, 24, 32, 48, 64.
 * \param[in]  flag      A combination of message flags, \ref canMSG_xxx
 *                       (including \ref canFDMSG_xxx if the CAN FD protocol is
 *                       enabled).
 *                       Use this parameter to send extended (29-bit) frames
 *                       and/or remote frames. Use \ref canMSG_EXT and/or
 *                       \ref canMSG_RTR for this purpose.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_sending
 * \sa \ref canWriteSync(), \ref canWriteWait()
 *
 *)
function canWrite(handle: canHandle; id: Longint; msg: Pointer; dlc: Cardinal; flag: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canWrite';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canWriteSync(CanHandle handle, Int64 timeout);</b>
 *
 * \source_delphi   <b>function canWriteSync(handle: canHandle; timeout: Cardinal): canStatus;     </b>
 * \source_end
 *
 * Waits until all CAN messages for the specified handle are sent, or the
 * timeout period expires.
 *
 * \param[in]  hnd       A handle to an open CAN circuit.
 * \param[in]  timeout   The timeout in milliseconds. 0xFFFFFFFF gives an
 *                       infinite timeout.
 *
 * \return \ref canOK (zero) if the queue emptied before the timeout period came to
 *         its end.
 * \return \ref canERR_TIMEOUT (negative) not all messages were transmitted when
 *         the timeout occurred.
 * \return \ref canERR_PARAM (negative) This could be caused by an erroneous
 *         parameter, or if you have turned TXACKs off (by using \ref canIoCtl())
 *         because if you do you can't use this call. The driver simply doesn't
 *         know when all the messages are sent!
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canWrite(), \ref canWriteWait()
 *)
function canWriteSync(handle: canHandle; timeout: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canWriteSync';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canRead(CanHandle handle, out Int32 id, Byte[] msg, out Int32 dlc, out Int32 flag, out Int64 time);</b>
 *
 * \source_delphi   <b>function canRead(handle: canHandle; var id: Longint; msg: Pointer; var dlc: Cardinal; var flag: Cardinal; var time: Cardinal): canStatus;     </b>
 * \source_end
 *
 * Reads a message from the receive buffer. If no message is available, the
 * function returns immediately with return code \ref canERR_NOMSG.
 *
 * If you are using the same channel via multiple handles, note that the
 * default behaviour is that the different handles will "hear" each other just as
 * if each handle referred to a channel of its own. If you open, say, channel 0
 * from thread A and thread B and then send a message from thread A, it will be
 * "received" by thread B.
 * This behaviour can be changed using \ref canIOCTL_SET_LOCAL_TXECHO.
 *
 * It is allowed to pass \c NULL as the value of \a id, \a msg, \a dlc, \a
 * flag, and \a time.
 *
 * \param[in]  hnd   A handle to an open circuit.
 * \param[out] id    Pointer to a buffer which receives the CAN identifier.
 *                   This buffer will only get the identifier. To determine
 *                   whether this identifier was standard (11-bit) or extended
 *                   (29-bit), and/or whether it was remote or not, or if it
 *                   was an error frame, examine the contents of the flag
 *                   argument.
 * \param[out] msg   Pointer to the buffer which receives the message data. This
 *                   buffer must be large enough (i.e. 8 bytes for classic CAN
 *                   and up to 64 bytes for CAN FD).
 * \param[out] dlc   Pointer to a buffer which receives the message length.
 * \param[out] flag  Pointer to a buffer which receives the message flags,
 *                   which is a combination of the \ref canMSG_xxx (including
 *                   \ref canFDMSG_xxx if the CAN FD protocol is enabled) and
 *                   \ref canMSGERR_xxx values.
 * \param[out] time  Pointer to a buffer which receives the message time stamp.
 *
 * \return \ref canOK (zero) if a message was read.
 * \return \ref canERR_NOMSG (negative) if there was no message available.
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_reading, \ref
 * page_user_guide_send_recv_mailboxes, \ref
 * section_user_guide_send_recv_reading
 * page_user_guide_time Time Measurement
 *
 * \sa \ref canReadSpecific(), \ref canReadSpecificSkip(), \ref canReadSync(),
 *     \ref canReadSyncSpecific(), \ref canReadWait()
 *
 *)
function canRead(handle: canHandle; var id: Longint; msg: Pointer; var dlc: Cardinal; var flag: Cardinal; var time: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canRead';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canReadWait(CanHandle handle, out Int32 id, Byte[] msg, out Int32 dlc, out Int32 flag, out Int64 time, Int64 timeout);</b>
 *
 * \source_delphi   <b>function canReadWait(handle: canHandle; var id: Longint; msg: Pointer; var dlc: Cardinal; var flag: Cardinal; var time: Cardinal; timeout: Cardinal): canStatus;     </b>
 * \source_end
 *
 * Reads a message from the receive buffer. If no message is available, the
 * function waits until a message arrives or a timeout occurs.
 *
 * If you are using the same channel via multiple handles, note that the
 * default behaviour is that the different handles will "hear" each other just as
 * if each handle referred to a channel of its own. If you open, say, channel 0
 * from thread A and thread B and then send a message from thread A, it will be
 * "received" by thread B.
 * This behaviour can be changed using \ref canIOCTL_SET_LOCAL_TXECHO.
 *
 * It is allowed to pass \c NULL as the value of \a id, \a msg, \a dlc, \a
 * flag, and \a time.
 *
 * \param[in]   hnd    A handle to an open circuit.
 * \param[out]  id     Pointer to a buffer which receives the CAN identifier.
 *                     This buffer will only get the identifier. To determine
 *                     whether this identifier was standard (11-bit) or extended
 *                     (29-bit), and/or whether it was remote or not, or if it
 *                     was an error frame, examine the contents of the flag
 *                     argument.
 * \param[out]  msg    Pointer to the buffer which receives the message data.
 *                     This buffer must be large enough (i.e. 8 bytes for
 *                     classic CAN and up to 64 bytes for CAN FD).
 * \param[out]  dlc    Pointer to a buffer which receives the message length.
 * \param[out]  flag   Pointer to a buffer which receives the message flags,
 *                     which is a combination of the \ref canMSG_xxx (including
 *                     \ref canFDMSG_xxx if the CAN FD protocol is enabled) and
 *                     \ref canMSGERR_xxx values.
 * \param[out] time    Pointer to a buffer which receives the message time stamp.
 * \param[in]  timeout If no message is immediately available, this parameter
 *                     gives the number of milliseconds to wait for a message
 *                     before returning. 0xFFFFFFFF gives an infinite timeout.
 *
 * \return \ref canOK (zero) if a message was read.
 * \return \ref canERR_NOMSG (negative) if there was no message available.
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canRead(), \ref canReadSpecific(), \ref canReadSpecificSkip(),
 *  \ref canReadSyncSpecific(), \ref canReadSync()
 *
 * \sa \ref page_user_guide_time Time Measurement
 *)
function canReadWait(handle: canHandle; var id: Longint; msg: Pointer; var dlc: Cardinal; var flag: Cardinal; var time: Cardinal; timeout: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canReadWait';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canReadSpecific(CanHandle handle, Int32 id, Byte[] msg, out Int32 dlc, out Int32 flag, out Int64 time);</b>
 *
 * \source_delphi   <b>function canReadSpecific(handle: canHandle; id: Longint; msg: Pointer; var dlc: Cardinal; var flag: Cardinal; var time: Cardinal): canStatus;     </b>
 * \source_end
 *
 * Reads a message with a specified identifier from the receive buffer. Any
 * preceding message not matching the specified identifier will be kept
 * in the receive buffer. If no message with the specified identifier is
 * available, the function returns immediately with an error code.
 *
 * If you are using the same channel via multiple handles, note that the
 * default behaviour is that the different handles will "hear" each other just as
 * if each handle referred to a channel of its own. If you open, say, channel 0
 * from thread A and thread B and then send a message from thread A, it will be
 * "received" by thread B.
 * This behaviour can be changed using \ref canIOCTL_SET_LOCAL_TXECHO.
 *
 * It is allowed to pass \c NULL as the value of \a msg, \a dlc, \a
 * flag, and \a time.
 *
 * \note Use \ref canReadSpecific only if you for some reason must keep the unread
 *       messages in the queue. If this is not the case, consider using
 *       \ref canReadSpecificSkip() or \ref canRead() for better performance.
 *
 * \param[in]   hnd    A handle to an open circuit.
 * \param[in]   id     The desired CAN identifier.
 * \param[out]  msg    Pointer to the buffer which receives the message data.
 *                     This buffer must be large enough (i.e. 8 bytes for
 *                     classic CAN and up to 64 bytes for CAN FD).
 * \param[out]  dlc    Pointer to a buffer which receives the message length.
 * \param[out]  flag   Pointer to a buffer which receives the message flags,
 *                     which is a combination of the \ref canMSG_xxx (including
 *                     \ref canFDMSG_xxx if the CAN FD protocol is enabled) and
 *                     \ref canMSGERR_xxx values.
 * \param[out] time    Pointer to a buffer which receives the message time stamp.
 *
 * \return \ref canOK (zero) if a message was read.
 * \return \ref canERR_NOMSG (negative) if a matching message was not found.
 *         There might be other messages in the queue, though.
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref page_user_guide_send_recv_mailboxes, \ref
 * section_user_guide_send_recv_reading, \ref
 * page_user_guide_time Time Measurement
 * \sa \ref canRead(), \ref canReadSpecificSkip(), \ref canReadSync(), \ref canReadSyncSpecific(),
 * \ref canReadWait()
 *
 *)
function canReadSpecific(handle: canHandle; id: Longint; msg: Pointer; var dlc: Cardinal; var flag: Cardinal; var time: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canReadSpecific';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canReadSync(CanHandle handle, Int64 timeout);</b>
 *
 * \source_delphi   <b>function canReadSync(handle: canHandle; timeout: Cardinal): canStatus;     </b>
 * \source_end
 *
 * Waits until the receive buffer contains at least one message or a timeout
 * occurs.
 *
 * If you are using the same channel via multiple handles, note that the
 * default behaviour is that the different handles will "hear" each other just as
 * if each handle referred to a channel of its own. If you open, say, channel 0
 * from thread A and thread B and then send a message from thread A, it will be
 * "received" by thread B.
 * This behaviour can be changed using \ref canIOCTL_SET_LOCAL_TXECHO.
 *
 * \param[in]  hnd      A handle to an open circuit.
 * \param[in]  timeout  The timeout in milliseconds. 0xFFFFFFFF gives an
 *                      infinite timeout.
 *
 * \return \ref canOK (zero) if the queue contains the desired message.
 * \return \ref canERR_TIMEOUT (negative) if a timeout occurs before a message
 *         arrived.
 * \return \ref canERR_xxx (negative) if the call fails.
 *
 * \sa \ref canRead(), \ref canReadSpecific(), \ref canReadSpecificSkip(),
 * \ref canReadSyncSpecific(), \ref canReadWait()
 *)
function canReadSync(handle: canHandle; timeout: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canReadSync';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canReadSyncSpecific(CanHandle handle, Int32 id, Int64 timeout);</b>
 *
 * \source_delphi   <b>function canReadSyncSpecific(handle: canHandle; id: Longint; timeout: Cardinal): canStatus;  </b>
 * \source_end
 *
 * Waits until the receive queue contains a message with the specified id, or a
 * timeout occurs..
 *
 * If you are using the same channel via multiple handles, note that the
 * default behaviour is that the different handles will "hear" each other just as
 * if each handle referred to a channel of its own. If you open, say, channel 0
 * from thread A and thread B and then send a message from thread A, it will be
 * "received" by thread B.
 * This behaviour can be changed using \ref canIOCTL_SET_LOCAL_TXECHO.
 *
 * \param[in]  hnd      A handle to an open circuit.
 * \param[in]  id       The desired message identifier.
 * \param[in]  timeout  The timeout in milliseconds. 0xFFFFFFFF gives an
 *                      infinite timeout.
 *
 * \return \ref canOK (zero) if the queue contains the desired message.
 * \return \ref canERR_TIMEOUT (negative) if a timeout occurs before the specified
 *         message arrived.
 * \return \ref canERR_xxx (negative) if the call fails.
 *
 * \sa \ref canRead(), \ref canReadSpecific(), \ref canReadSpecificSkip(),
 * \ref canReadSync(), \ref canReadWait()
 *)
function canReadSyncSpecific(handle: canHandle; id: Longint; timeout: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canReadSyncSpecific';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canReadSpecificSkip(CanHandle hnd, Int32 id, Byte[] msg, out Int32 dlc, out Int32 flag, out Int64 time);</b>
 *
 * \source_delphi   <b>function canReadSpecificSkip(handle: canHandle; id: Longint; msg: Pointer; var dlc: Cardinal; var flag: Cardinal; var time: Cardinal): canStatus;     </b>
 * \source_end
 *
 * Reads a message with a specified identifier from the receive buffer. Any
 * preceding message not matching the specified identifier will be removed
 * in the receive buffer. If no message with the specified identifier is
 * available, the function returns immediately with an error code.
 *
 * If you are using the same channel via multiple handles, note that the
 * default behaviour is that the different handles will "hear" each other just as
 * if each handle referred to a channel of its own. If you open, say, channel 0
 * from thread A and thread B and then send a message from thread A, it will be
 * "received" by thread B.
 * This behaviour can be changed using \ref canIOCTL_SET_LOCAL_TXECHO.
 *
 * It is allowed to pass \c NULL as the value of \a msg, \a dlc, \a
 * flag, and \a time.
 *
 * \param[in]   hnd    A handle to an open circuit.
 * \param[in]   id     The desired CAN identifier.
 * \param[out]  msg    Pointer to the buffer which receives the message data.
 *                     This buffer must be large enough (i.e. 8 bytes for
 *                     classic CAN and up to 64 bytes for CAN FD).
 * \param[out]  dlc    Pointer to a buffer which receives the message length.
 * \param[out]  flag   Pointer to a buffer which receives the message flags,
 *                     which is a combination of the \ref canMSG_xxx (including
 *                     \ref canFDMSG_xxx if the CAN FD protocol is enabled) and
 *                     \ref canMSGERR_xxx values.
 * \param[out] time    Pointer to a buffer which receives the message time stamp.
 *
 * \return \ref canOK (zero) if a matching message was found.
 * \return \ref canERR_NOMSG if there was no matching message available. All other
           messages (if any!) were discarded.
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_reading, \ref
 * page_user_guide_time Time Measurement
 * \sa \ref canRead(), \ref canReadSpecific(), \ref canReadSync(),
 * \ref canReadSyncSpecific(), \ref canReadWait()
 *)
function canReadSpecificSkip(handle: canHandle; id: Longint; msg: Pointer; var dlc: Cardinal; var flag: Cardinal; var time: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canReadSpecificSkip';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canSetNotify(CanHandle handle, IntPtr win_handle, Int32 aNotifyFlags);</b>
 *
 * \source_delphi   <b>function canSetNotify(handle: canHandle; aHWnd: HWND; aNotifyFlags: Cardinal): canStatus;     </b>
 * \source_end
 *
 * This function associates a window handle with the CAN circuit. A
 * \c WM__CANLIB message is sent to that window when certain events
 * (specified by the \ref canNOTIFY_xxx flags) occur.
 *
 * When an event take place, a \c WM__CANLIB message will be sent to the window
 * whose handle is aHWnd. This \c WM__CANLIB message will have:
 *
 * \li \c WPARAM handle to the circuit where the event occurred
 * \li \c HIWORD(LPARAM) 0
 * \li \c LOWORD(LPARAM) \ref canEVENT_xxx
 *
 * In the routine that handles \c WM__CANLIB, you can call the CANlib API
 * functions (for example, \ref canRead()) using the handle found in \c wParam.
 *
 * \param[in]  hnd          A handle to an open CAN circuit.
 * \param[in] aHWnd         Handle of the window which will receive the
 *                          notification messages.
 * \param[in] aNotifyFlags  The events specified with \ref canNOTIFY_xxx, for
 *                          which callback should be called.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \n In the routine that handles \c WM__CANLIB, you must call \ref canRead() repeatedly
 *    until it returns \ref canERR_NOMSG, regardless of the \c LPARAM value. This will
 *    flush the driver's internal event queues. If you fail to do this, no more
 *    events will be reported.
 *
 * \sa \ref section_user_guide_send_recv_asynch
 *)
function canSetNotify(handle: canHandle; aHWnd: HWND; aNotifyFlags: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canSetNotify';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canTranslateBaud(int bitrate, out Int32 freq, out Int32 tseg1, out Int32 tseg2, out Int32 sjw, out Int32 nosamp);</b>
 *
 * \source_delphi   <b>function canTranslateBaud(var freq: Longint; var tseg1, tseg2, sjw, noSamp, syncMode: Cardinal): canStatus;     </b>
 * \source_end
 *
 * This function translates the \ref canBITRATE_xxx constants to their corresponding
 * bus parameter values. At return, this \a freq contains the actual bit rate
 * (in bits per second). \a TSeg1 is the number of quanta (less one) in a bit
 * before the sampling point. \a TSeg2 is the number of quanta after the
 * sampling point.
 *
 * \param[in,out] freq      A pointer to a \c DWORD which contains the \ref canBITRATE_xxx
 *                          constant to translate. The bitrate constant value is
 *                          overwritten with the frequency value.
 * \param[out]    tseg1     A pointer to a buffer which receives the Time segment 1,
 *                          that is, the number of quanta from (but not including)
 *                          the Sync Segment to the sampling point.
 * \param[out]    tseg2     A pointer to a buffer which receives the Time segment 2,
 *                          that is, the number of quanta from the sampling point to
 *                          the end of the bit.
 * \param[out]    sjw       A pointer to a buffer which receives the Synchronization
 *                          Jump Width.
 * \param[out]    nosamp    A pointer to a buffer which receives the number of
 *                          sampling points.
 * \param[out]    syncMode  Unsupported, always read as zero. May be set to NULL.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canSetBusParams()
 *)
function canTranslateBaud(var freq: Longint; var tseg1, tseg2, sjw, noSamp, syncMode: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canTranslateBaud';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus Canlib.kvBitrateToBusParamsTq(Int32 freqA, out kvBusParamsTq nominal);</b>
 *
 * \source_delphi   <b>function kvBitrateToBusparamsTq(const freq: Integer; var nominal: kvBusParamsTq): canStatus;</b>
 * \source_end
 *
 * This function translates the \ref canBITRATE_xxx constants to corresponding
 * bus parameter values. At return \a nominal contains parameter values for the
 * specified \a freq. If the channel is opened in CAN FD mode, see \ref kvBitrateToBusParamsFdTq().
 *
 * \param[in]     freq      Bitrate constant, \ref canBITRATE_xxx.
 * \param[in,out] nominal   A pointer to a \ref kvBusParamsTq, upon return receives
 *                          the bus busparamters specified by \a freq.
 *
 * \return \ref canOK (zero) is success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canSetBusParamsTq(), \ref kvBitrateToBusParamsFdTq()
 *)
function kvBitrateToBusParamsTq(freq: Integer; nominal: PkvBusParamsTq): canStatus; stdcall;
  external canlib32 name _PU + 'kvBitrateToBusParamsTq';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus Canlib.kvBitrateToBusParamsFdTq(Int32 freqA, Int32 freqD, out kvBusParamsTq arbitration, out kvBusParamsTq data);</b>
 *
 * \source_delphi   <b>function kvBitrateToBusParamsFdTq (const freqA, freqD: Integer; var arbitration, data: kvBusParamsTq): canStatus;</b>
 *
 * This function translates the \ref canFD_BITRATE_xxx constants to corresponding
 * bus parameter values. At return \a arbitration contains parameter values for the
 * specified \a freqA, and \a data contains parameter values for the specified
 * \a freqD. If the channel is opened in classic CAN mode see \ref kvBitrateToBusParamsTq().
 *
 * \param[in]     freqA       Bitrate constant, \ref canFD_BITRATE_xxx.
 * \param[in]     freqD       Bitrate constant, \ref canFD_BITRATE_xxx.
 * \param[in,out] arbitration A pointer to a \ref kvBusParamsTq, upon return receives
 *                            the bus busparamters specified by \a freqA.
 * \param[in,out] data        A pointer to a \ref kvBusParamsTq, upon return receives
 *                            the bus busparamters specified by \a freqD.
 *
 * \return \ref canOK (zero) is success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canSetBusParamsFdTq(), \ref kvBitrateToBusParamsTq()
 *)
function kvBitrateToBusParamsFdTq(freqA: Integer; freqD: Integer; arbitration: PkvBusParamsTq; data: PkvBusParamsTq): canStatus; stdcall;
  external canlib32 name _PU + 'kvBitrateToBusParamsFdTq';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.canStatus canGetErrorText(Canlib.canStatus err, out string buf_str);</b>
 *
 * \source_delphi   <b>function canGetErrorText(err: canStatus; buf: PAnsiChar; bufsiz: Cardinal): canStatus;     </b>
 * \source_end
 *
 * This function translates an error code (\ref canERR_xxx)
 * to a human-readable, English text.
 *
 * \param[in]     err     The error code.
 * \param[out]    buf     The buffer which is to receive the text, which is a
 *                        zero-terminated string (provided the buffer is large enough.)
 * \param[in]     bufsiz  The length of the input buffer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_canstatus
 *
 *)
function canGetErrorText(err: canStatus; buf: PAnsiChar; bufsiz: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canGetErrorText';

(**
 * \ingroup  can_general
 *
 * \source_cs       <b>static Int16 canGetVersion();</b>
 *
 * \source_delphi   <b>function canGetVersion: Word;     </b>
 * \source_end
 *

 * This API call returns the version of the CANlib API DLL (canlib32.dll).  The
 * most significant byte is the major version number and the least significant
 * byte is the minor version number.
 *
 * The actual version of the different driver files can be obtained by studying
 * the version resources in each of the files.
 *
 * \note The version number of the canlib32.dll file is not related to the
 *       product version of CANlib you are using. CANlib consists of several
 *       driver and DLL files. To obtain the product version, use
 *       \ref canGetVersionEx().
 *
 * \return version number of canlib32.dll
 *
 * \note Linux returns version number from libcanlib.so
 *
 * \sa \ref page_user_guide_version
 * \sa \ref canGetVersionEx()
 *     \ref canProbeVersion()
 *
 *)
function canGetVersion(): Word; stdcall;
  external canlib32 name _PU + 'canGetVersion';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.canStatus canIoCtl(CanHandle handle, Int32 func, out Int32 val);<br>
                       static Canlib.canStatus canIoCtl(CanHandle handle, Int32 func, out String str_buf);<br>
                       static Canlib.canStatus canIoCtl(CanHandle handle, Int32 func, ref object obj_buf);</b>
 *
 * \source_delphi   <b>function canIoCtl(handle: canHandle; func: Cardinal; buf: Pointer; buflen: Cardinal): canStatus;     </b>
 * \source_end
 *
 * This API call performs several different functions (\ref canIOCTL_xxx). The
 * functions are handle-specific unless otherwise noted; this means that they
 * affect only the handle you pass to \ref canIoCtl(), whereas other open
 * handles will remain unaffected.  The contents of \a buf after the call is
 * dependent on the function code you specified.
 *
 * \param[in]     hnd     A handle to an open circuit.
 * \param[in]     func    A \ref canIOCTL_xxx function code
 * \param[in,out] buf     Pointer to a buffer containing function-dependent data;
                          or a \c NULL pointer for certain function codes. The
                          buffer can be used for both input and output
                          depending on the function code. See \ref canIOCTL_xxx.
 * \param[in]     buflen  The length of the buffer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 *)
function canIoCtl(handle: canHandle; func: Cardinal; buf: Pointer; buflen: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canIoCtl';

(**
 * \ingroup Obsolete
 *
 * \source_delphi   <b>function canReadTimer(handle: canHandle): Longint;     </b>
 * \source_end
 *
 * \warning Obsolete! \ref kvReadTimer() should be used instead.
 *)
function canReadTimer(handle: canHandle): Longint; stdcall;
  external canlib32 name _PU + 'canReadTimer';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static CanHandle canOpenChannel(Int32 channel, Int32 flags);</b>
 *
 * \source_delphi   <b>function canOpenChannel(channel: Integer; flags: Integer): canHandle;     </b>
 * \source_end
 *
 * Opens a CAN channel (circuit) and returns a handle which is used
 * in subsequent calls to CANlib.
 *
 *
 * Channel numbering is dependent on the installed hardware. The first channel
 * always has number 0.
 *
 * For example,
 *
 * \li If you have a single LAPcan, the channels are numbered 0 and 1.
 *
 * \li If you have a USBcan Professional, the channels are numbered 0-1
 *     according to the labels on the cables.
 *
 * \li The virtual channels come after all physical channels.
 *
 * If you are using multiple threads, note that the returned handle is usable
 * only in the context of the thread that created it. That is, you must call
 * \ref canOpenChannel() in each of the threads in your application that uses the
 * CAN bus. You can open the same channel from multiple threads, but you must
 * call \ref canOpenChannel() once per thread.
 *
 * If you are using the same channel via multiple handles, note that the
 * default behaviour is that the different handles will "hear" each other just as
 * if each handle referred to a channel of its own. If you open, say, channel 0
 * from thread A and thread B and then send a message from thread A, it will be
 * "received" by thread B.
 * This behaviour can be changed using \ref canIOCTL_SET_LOCAL_TXECHO.
 *
 * \note The handle returned may be zero which is perfectly valid.
 *
 * \param[in]  channel  The number of the channel. Channel numbering is hardware
 *                      dependent.
 * \param[in]  flags    A combination of \ref canOPEN_xxx flags
 *
 * \return Returns a handle to the opened circuit, or \ref canERR_xxx
 *         (negative) if the call failed.
 *
 * \sa \ref page_user_guide_chips_channels,  \ref section_user_guide_virtual
 * \sa \ref canClose(), \ref canGetNumberOfChannels(), \ref canGetChannelData(), \ref canIoCtl()
 *
 *)
function canOpenChannel(channel: Integer; flags: Integer): canHandle; stdcall;
  external canlib32 name _PU + 'canOpenChannel';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.canStatus canGetNumberOfChannels(out Int32 channelCount);</b>
 *
 * \source_delphi   <b>function canGetNumberOfChannels(var channelCount: Integer): canStatus;     </b>
 * \source_end
 *
 * This function returns the number of available CAN channels in the
 * computer. The virtual channels are included in this number.
 *
 * \param[out] channelCount  A pointer to a \c DWORD which will receive the current
 *                           number of channels.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_unique_device, \ref section_user_guide_virtual
 * \sa \ref canGetChannelData()
 *)
function canGetNumberOfChannels(var channelCount: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canGetNumberOfChannels';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.canStatus canGetChannelData(Int32 channel, Int32 item, out object buffer);</b>
 *
 * \source_delphi   <b>function canGetChannelData(channel, item: Integer; var buffer; bufsize: size_t): canStatus;     </b>
 * \source_end
 *
 * This function can be used to retrieve certain pieces of information about a channel.
 *
 * \note You must pass a channel number and not a channel handle.
 *
 * \param[in]  channel  The number of the channel you are interested in. Channel
 *                        numbers are integers in the interval beginning at 0
 *                        (zero) and ending at the value returned by
 *                        \ref canGetNumberOfChannels() minus 1.
 * \param[in]  item  This parameter specifies what data to obtain for the
 *                        specified channel. The value is one of the constants
 *                        \ref canCHANNELDATA_xxx.
 * \param[out] buffer     The address of a buffer which is to receive the data.
 * \param[in]  bufsize    The size of the buffer to which the buffer parameter
 *                        points.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_unique_device
 * \sa \ref canGetNumberOfChannels()
 * \sa \ref canGetHandleData()
 *)
function canGetChannelData(channel, item: Integer; var buffer; bufsize: size_t): canStatus; stdcall;
  external canlib32 name _PU + 'canGetChannelData';

(**
 * \ingroup Notification and Waiting
 *
 * \source_cs       <b>static Canlib.canStatus canWaitForEvent(CanHandle hnd, Int64 timeout);</b>
 *
 * \source_delphi   <b>function canWaitForEvent(hnd: canHandle; timeout: Cardinal): canStatus;     </b>
 * \source_end
 *
 * Waits for an event (of any kind) to happen at the specified CAN circuit, or
 * a timeout to occur. An event in this context means for example the arrival
 * of a CAN message or a CAN bus status change, but it can also be an event
 * internal to the driver.
 *
 * \param[in] hnd      A handle to an open CAN circuit.
 * \param[in] timeout  The number of milliseconds to wait before the call
 *                     returns, if no event occurs. 0xFFFFFFFF gives an
 *                     infinite timeout.
 *
 * \return \ref canOK (zero) if an event happened during the specified time
 *         period.
 * \return \ref canERR_TIMEOUT (negative) if nothing happened during the specified
 *         time period.
 * \return \ref canERR_xxx (negative) if failure.
 *
 * \sa \ref section_user_guide_send_recv_asynch_event
 * \sa \ref canRead()
 *
 * \note Not implemented in linux.
 *)
function canWaitForEvent(hnd: canHandle; timeout: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canWaitForEvent';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canSetBusParamsC200(CanHandle hnd, byte btr0, byte btr1);</b>
 *
 * \source_delphi   <b>function canSetBusParamsC200(hnd: canHandle; btr0, btr1: byte): canStatus;     </b>
 * \source_end
 *
 * This function sets the bus timing parameters using the same
 * convention as the 82c200 CAN controller (which is the same as many
 * other CAN controllers, for example, the 82527.)
 *
 * To calculate the bit timing parameters, you can use the bit timing
 * calculator that is included with CANlib SDK. Look in the BIN directory.
 *
 * 82c200 Bit Timing
 *
 * \li \a btr0 [b7..b6]: SJW - 1
 * \li \a btr0 [b5..b0]: Prescaler -1
 * \li \a btr1 [b7]: \c 1: 3 samples, \c 0: 1 samples
 * \li \a btr1 [b6..b4]: tseg2 - 1
 * \li \a btr1 [b3..b0]: tseg1 - 2
 *
 * \note CANlib will always behave as if the clock frequency is 16 MHz. It does
 * not matter if the device has a different physical clock, since this will be
 * compensated for by the driver.
 *
 * \param[in] hnd   A handle to an open CAN circuit.
 * \param[in] btr0  The desired bit timing, formatted as the contents of the
 *                  BTR0 register in the 82c200.
 * \param[in] btr1  The desired bit timing, formatted as the contents of the
 *                  BTR1 register in the 82c200.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 *
 * \sa \ref section_user_guide_init_bit_rate_can, \ref canSetBusParams()
 * \sa \ref canSetBusParamsTq()
 *)
function canSetBusParamsC200(hnd: canHandle; btr0, btr1: byte): canStatus; stdcall;
  external canlib32 name _PU + 'canSetBusParamsC200';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canSetDriverMode(CanHandle hnd, Int32 lineMode, Int32 resNet);</b>
 *
 * \source_delphi   <b>function canSetDriverMode(hnd: canHandle; lineMode, resNet: Integer): canStatus;     </b>
 * \source_end
 *
 * This function sets the current CAN bus driver mode. This is
 * typically a mode like sleep, wakeup, standby, fast mode, etc. The
 * different modes are almost always hardware dependent and requires
 * special DRVcan cables. As an example, the DRVcan S implements J2411
 * compliant single-wire CAN and supports four line modes, namely
 * Normal, Sleep, Fast and Wakeup.
 *
 * Standard ISO 11898 CAN do not support any of these bus driver modes.
 *
 * \note The bus driver mode is typically used to control things like one- or
 * two-wire mode, sleep mode, and so on. It requires special support in the CAN
 * driver circuit.
 *
 * \param[in] hnd       An open handle to a CAN circuit.
 * \param[in] lineMode  An int which defines the line mode,
 *                      \ref canTRANSCEIVER_LINEMODE_xxx.
 * \param[in] resNet    An int which defines the resnet mode. Set this parameter to
 *                      \ref canTRANSCEIVER_RESNET_NA unless you have good reasons to set it
 *                      to something else.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canGetDriverMode()
 *
 * \note Not implemented in linux.
 *)
function canSetDriverMode(hnd: canHandle; lineMode, resNet: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canSetDriverMode';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canGetDriverMode(CanHandle hnd, out Int32 lineMode, out Int32 resNet);</b>
 *
 * \source_delphi   <b>function canGetDriverMode(hnd: canHandle; var lineMode: Integer; var resNet: Integer): canStatus;     </b>
 * \source_end
 *
 * This function retrieves the current CAN bus driver mode. This is typically a
 * mode like sleep, wakeup, standby, fast mode, etc. The different modes are
 * almost always hardware dependent and requires special DRVcan cables. As an
 * example, the DRVcan S implements J2411 compliant single-wire CAN and
 * supports four line modes, namely Normal, Sleep, Fast and Wakeup.
 *
 * Standard ISO 11898 CAN do not support any of these bus driver modes.
 *
 * \note The bus driver mode is typically used to control things like one- or
 * two-wire mode, sleep mode, and so on. It requires special support in the CAN
 * driver circuit.
 *
 * \param[in]  hnd       An open handle to a CAN circuit.
 * \param[out] lineMode  A pointer to an int which receives the current line
 *                       mode (\ref canTRANSCEIVER_LINEMODE_xxx).
 * \param[out] resNet    A pointer to an int which receives the current resnet
 *                       mode.  This value is usually
 *                       \ref canTRANSCEIVER_RESNET_NA except for special DRVcan
 *                       cables.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canSetDriverMode()
 *
 * \note Not implemented in linux.
 *
 *)
function canGetDriverMode(hnd: canHandle; var lineMode: Integer; var resNet: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canGetDriverMode';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Int32 canGetVersionEx(Int32 itemCode);</b>
 *
 * \source_delphi   <b>function canGetVersionEx(itemCode: Cardinal): Cardinal;     </b>
 * \source_end
 *
 * This function returns various version numbers from the driver routines.
 *
 * \param[in] itemCode  Specifies which version number to retrieve. See
 *                      \ref canVERSION_CANLIB32_xxx
 *
 * \return The return value is desired version number.
 *
 * \sa \ref page_user_guide_version
 *)
function canGetVersionEx(itemCode: Cardinal): Cardinal; stdcall;
  external canlib32 name _PU + 'canGetVersionEx';

(**
 * \ingroup NamedParameterSettings
 *
 * \source_cs       <b>static Canlib.canStatus canParamGetCount();</b>
 *
 * \source_delphi   <b>function canParamGetCount(): canStatus;     </b>
 * \source_end
 *
 * This function returns the number of entries in the table of named
 * channels.
 *
 * \return The number of channels (zero or positive)
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_misc_named_parameters
 *
 *)
function canParamGetCount(): canStatus; stdcall;
  external canlib32 name _PU + 'canParamGetCount';

(**
 * \ingroup NamedParameterSettings
 *
 * \source_cs       <b>static Canlib.canStatus canParamCommitChanges();</b>
 *
 * \source_delphi   <b>function canParamCommitChanges(): canStatus;   </b>
 * \source_end
 *
 * This function writes the current set of named parameters to the
 * Registry. Previous entries are erased.
 *
 * The named parameters are stored in the
 * \c HKEY_LOCAL_MACHINE\\SOFTWARE\\KVASER \c AB\\CANLIB32\\PredefinedBitrates
 * key in the Registry.
 *
 * \note You must have Administrator's rights to write to the Registry.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_misc_named_parameters
 *
 *)
function canParamCommitChanges(): canStatus; stdcall;
  external canlib32 name _PU + 'canParamCommitChanges';

(**
 * \ingroup NamedParameterSettings
 *
 * \source_cs       <b>static Canlib.canStatus canParamDeleteEntry(Int32 index);</b>
 *
 * \source_delphi   <b>function canParamDeleteEntry(index: Integer): canStatus;      </b>
 * \source_end
 *
 * This function deletes the entry in the table of named parameter settings
 * with the given index. The entries below (i.e. with higher indices) the
 * deleted entry are moved up one step in the table.
 *
 * The named parameters are stored in the
 * \c HKEY_LOCAL_MACHINE\\SOFTWARE\\KVASER \c AB\\CANLIB32\\PredefinedBitrates
 * key in the Registry.
 *
 * \param[in]  index  The index of the entry to delete.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_misc_named_parameters
 * \sa \ref canParamCreateNewEntry(), \ref canParamCommitChanges()
 *)
function canParamDeleteEntry(index: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canParamDeleteEntry';

(**
 * \ingroup NamedParameterSettings
 *
 * \source_cs       <b>static Int32 canParamCreateNewEntry();</b>
 *
 * \source_delphi   <b>function canParamCreateNewEntry(): canStatus;     </b>
 * \source_end
 *
 * This function creates a new entry in the table of named parameter
 * settings.
 *
 * The named parameters are stored in the
 * \c HKEY_LOCAL_MACHINE\\SOFTWARE\\KVASER \c AB\\CANLIB32\\PredefinedBitrates
 * key in the Registry.
 *
 * \return The index of the created entry (zero or positive) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_misc_named_parameters
 * \sa \ref canParamCommitChanges(), \ref canParamDeleteEntry()
 *
 *)
function canParamCreateNewEntry(): canStatus; stdcall;
  external canlib32 name _PU + 'canParamCreateNewEntry';

(**
 * \ingroup NamedParameterSettings
 *
 * \source_cs       <b>static Canlib.canStatus canParamSwapEntries(Int32 index1, Int32 index2);</b>
 *
 * \source_delphi   <b>function canParamSwapEntries(index1, index2: Integer): canStatus;      </b>
 * \source_end
 *
 * This function swaps two entries in the list of named
 * parameters.
 *
 * The named parameters are stored in the
 * \c HKEY_LOCAL_MACHINE\\SOFTWARE\\KVASER \c AB\\CANLIB32\\PredefinedBitrates
 * key in the Registry.
 *
 * \param[in]  index1  The first of the two entries that are to be swapped in the
 *                     named parameters list.
 * \param[in]  index2  The second of the two entries that are to be swapped in the
 *                     named parameters list.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_misc_named_parameters
 * \sa \ref canParamCommitChanges()
 *
 *)
function canParamSwapEntries(index1, index2: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canParamSwapEntries';

(**
 * \ingroup NamedParameterSettings
 *
 * \source_cs       <b>static Canlib.canStatus canParamGetName(Int32 index, ref String str_buf);</b>
 *
 * \source_delphi   <b>function canParamGetName(index: Integer; buffer: PAnsiChar; maxlen: Integer): canStatus;      </b>
 * \source_end
 *
 * This function returns the name of a given entry in the list of
 * named parameters.
 *
 * The named parameters are stored in the
 * \c HKEY_LOCAL_MACHINE\\SOFTWARE\\KVASER \c AB\\CANLIB32\\PredefinedBitrates
 * key in the Registry.
 *
 * \param[in]  index   The index of the entry in the named parameters list, whose
 *                     name is to be returned.
 * \param[out] buffer  A pointer to a buffer that is to receive a
 *                     \c NULL terminated string which contains the name. The
 *                     buffer is allocated and deallocated by the user.
 * \param[in]  maxlen  The length of the buffer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_misc_named_parameters
 * \sa \ref canParamSetName(), \ref canParamCommitChanges()
 *)
function canParamGetName(index: Integer; buffer: PAnsiChar; maxlen: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canParamGetName';

(**
 * \ingroup NamedParameterSettings
 *
 * \source_cs       <b>static Int32 canParamGetChannelNumber(Int32 index);</b>
 *
 * \source_delphi   <b>function canParamGetChannelNumber(index: Integer): canStatus;     </b>
 * \source_end
 *
 * This function returns the channel number of the entry with the
 * given index in the table of named parameter settings.
 *
 * \param[in] index The index of the entry in the table of named parameter
 *                  settings.
 *
 * \return The channel number of the entry in question (zero or positive)
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_misc_named_parameters
 * \sa \ref canParamCommitChanges(), \ref canParamGetChannelNumber()
 *)
function canParamGetChannelNumber(index: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canParamGetChannelNumber';

(**
 * \ingroup NamedParameterSettings
 *
 * \source_cs       <b>static Canlib.canStatus canParamGetBusParams(Int32 index, out Int32 bitrate, out Int32 tseg1, out Int32 tseg2, out Int32 sjw, out Int32 noSamp);</b>
 *
 * \source_delphi   <b>function canParamGetBusParams(index: Integer; var bitrate: Longint; var tseg1: Cardinal; var tseg2: Cardinal; var sjw: Cardinal; var nosamp: Cardinal): canStatus;    </b>
 * \source_end
 *
 * This function retrieves the bus parameters associated with the
 * entry with the given index in the table of named parameter
 *   settings.
 *
 * \param[in] index     The index of the entry in the table of named parameter
 *                      settings.
 * \param[out] bitrate  Bit rate (bits per second).
 * \param[out] tseg1    Time segment 1, that is, the number of quanta from (but
 *                      not including) the Sync Segment to the sampling point.
 * \param[out] tseg2    Time segment 2, that is, the number of quanta from the
 *                      sampling point to the end of the bit.
 * \param[out] sjw      The Synchronization Jump Width.
 * \param[out] noSamp   The number of sampling points; can be 1 or 3.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_misc_named_parameters
 * \sa \ref canParamCommitChanges(), \ref canParamSetBusParams()
 *)
function canParamGetBusParams(index: Integer; var bitrate: Longint; var tseg1: Cardinal; var tseg2: Cardinal; var sjw: Cardinal; var nosamp: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canParamGetBusParams';

(**
 * \ingroup NamedParameterSettings
 *
 * \source_cs       <b>static Canlib.canStatus canParamSetName(Int32 index, String str_buf);</b>
 *
 * \source_delphi   <b>function canParamSetName(index: Integer; buffer: PAnsiChar): canStatus;    </b>
 * \source_end
 *
 * This function sets or changes the name of a named parameter.
 *
 * \param[in]  index  The index of the named parameter whose name is to be
 *                    changed or set.
 * \param[out] buffer A pointer to a \c NULL terminated string that contains the
 *                    new name. If the string is longer than the maximum
 *                    allowed name length, it is truncated.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_misc_named_parameters
 * \sa \ref canParamCommitChanges(), \ref canParamGetName()
 *)
function canParamSetName(index: Integer; buffer: PAnsiChar): canStatus; stdcall;
  external canlib32 name _PU + 'canParamSetName';

(**
 * \ingroup NamedParameterSettings
 *
 * \source_cs       <b>static Canlib.canStatus canParamSetChannelNumber(Int32 index, Int32 channel);</b>
 *
 * \source_delphi   <b>function canParamSetChannelNumber(index, channel: Integer): canStatus;     </b>
 * \source_end
 *
 * This function sets the channel number for a specified entry in the list of
 * named parameters. Channels are numbered from 0 and up.
 *
 * \param[in] index    The index of the entry in the named parameter list whose
 *                     channel number is to be set.
 * \param[in] channel  The channel number.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_misc_named_parameters
 * \sa \ref canParamCommitChanges(), \ref canParamGetChannelNumber()
 *)
function canParamSetChannelNumber(index, channel: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canParamSetChannelNumber';

(**
 * \ingroup NamedParameterSettings
 *
 * \source_cs       <b>static Canlib.canStatus canParamSetBusParams(Int32 index, Int32 bitrate, Int32 tseg1, Int32 tseg2, Int32 sjw, Int32 noSamp);</b>
 *
 * \source_delphi   <b>function canParamSetBusParams(index: Integer; bitrate: Longint; tseq1, tseq2, sjw, noSamp: Cardinal): canStatus;     </b>
 * \source_end
 *
 * This function sets or changes the bus parameters for a given entry in the
 * list of named parameters.
 *
 * \note The bus parameters are not checked for validity.
 *
 * \param[in]  index   The index of the entry in the named parameter list whose
 *                     parameters are to be set or changed.
 * \param[in] bitrate  Bit rate (measured in bits per second); or one of the
 *                     predefined constants \ref canBITRATE_xxx.

 * \param[in] tseg1    Time segment 1, that is, the number of quanta from (but not
 *                     including) the Sync Segment to the sampling point.
 * \param[in] tseg2    Time segment 2, that is, the number of quanta from the
 *                     sampling point to the end of the bit.

 * \param[in]  sjw     The Synchronization Jump Width.
 * \param[in]  noSamp  The number of sampling points; can be 1 or 3.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_misc_named_parameters
 * \sa \ref canParamCommitChanges(), \ref canParamGetBusParams()
 *)
function canParamSetBusParams(index: Integer; bitrate: Longint; tseq1, tseq2, sjw, noSamp: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canParamSetBusParams';

(**
 * \ingroup NamedParameterSettings
 *
 * \source_cs       <b>static Canlib.canStatus canParamFindByName(String str_name);</b>
 *
 * \source_delphi   <b>function canParamFindByName(const Name: PAnsiChar): canStatus;     </b>
 * \source_end
 *
 * This function returns the index of the parameter setting with the
 * given name.
 *
 * \param[in] name A pointer to a string containing the name of the setting.
 *
 * \return The index of the setting (zero or positive) if success.
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_misc_named_parameters
 * \sa \ref canParamCommitChanges()
 *)
function canParamFindByName(const Name: PAnsiChar): canStatus; stdcall;
  external canlib32 name _PU + 'canParamFindByName';

(**
 * \ingroup ObjectBuffers
 *
 * \source_cs       <b>static Canlib.canStatus canObjBufFreeAll(CanHandle hnd);</b>
 *
 * \source_delphi   <b>function canObjBufFreeAll(handle: canHandle): canStatus;     </b>
 * \source_end
 *
 * Deallocates all object buffers on the specified handle. The
 * buffers cannot be referenced after this operation.
 *
 * \param[in]  hnd  An open handle to a CAN circuit.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_obj_buf
 * \sa \ref canObjBufFree(), \ref canObjBufAllocate()
 *)
function canObjBufFreeAll(handle: canHandle): canStatus; stdcall;
  external canlib32 name _PU + 'canObjBufFreeAll';

(**
 * \ingroup ObjectBuffers
 *
 * \source_cs       <b>static Canlib.canStatus canObjBufAllocate(CanHandle hnd, Int32 type);</b>
 *
 * \source_delphi   <b>function canObjBufAllocate(handle: canHandle; tp: Integer): canStatus;     </b>
 * \source_end
 *
 * Allocates an object buffer associated with a handle to a CAN
 * circuit.
 *
 * \param[in] hnd   An open handle to a CAN circuit.
 * \param[in] type  The type of the buffer. Must be one of \ref canOBJBUF_TYPE_xxx
 *
 * \return A buffer index (zero or positive) if success.
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_obj_buf
 * \sa \ref canObjBufFree(), \ref canObjBufFreeAll()
 *)
function canObjBufAllocate(handle: canHandle; tp: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canObjBufAllocate';

(**
 * \ingroup ObjectBuffers
 *
 * \source_cs       <b>static Canlib.canStatus canObjBufFree(CanHandle hnd, Int32 idx);</b>
 *
 * \source_delphi   <b>function canObjBufFree(handle: canHandle; idx: Integer): canStatus;      </b>
 * \source_end
 *
 * Deallocates the object buffer with the specified index. The buffer
 * can not be referenced after this operation.
 *
 * \param[in] hnd  An open handle to a CAN circuit.
 * \param[in] idx  The object buffer to deallocate.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_obj_buf
 * \sa \ref canObjBufFreeAll(), \ref canObjBufAllocate(),
 *)
function canObjBufFree(handle: canHandle; idx: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canObjBufFree';

(**
 * \ingroup ObjectBuffers
 *
 * \source_cs       <b>static Canlib.canStatus canObjBufWrite(CanHandle hnd, Int32 idx, Int32 id, Byte[] msg, Int32 dlc, Int32 flags);</b>
 *
 * \source_delphi   <b>function canObjBufWrite(handle: canHandle; idx, id: Integer; var msg; dlc, flags: cardinal): canStatus;     </b>
 * \source_end
 *
 * Defines the contents of a specific object buffer.
 *
 * \param[in] hnd   An open handle to a CAN circuit.
 * \param[in] idx   The index of the object buffer whose contents is to be
 *                  defined.
 * \param[in] id    The CAN identifier of the message.
 * \param[in] msg   Points to the contents of the message.
 * \param[in] dlc   The length of the message in bytes.<br>
                    For Classic CAN dlc can be at most 8, unless \ref canOPEN_ACCEPT_LARGE_DLC is used.<br>
                    For CAN FD dlc can be one of the following 0-8, 12, 16, 20, 24, 32, 48, 64.
 * \param[in] flags Message flags; a combination of the \ref canMSG_xxx flags.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_obj_buf
 *)
function canObjBufWrite(handle: canHandle; idx, id: Integer; var msg; dlc, flags: cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canObjBufWrite';

(**
 * \ingroup ObjectBuffers
 *
 * \source_cs       <b>static Canlib.canStatus canObjBufSetFilter(CanHandle hnd, Int32 idx, Int32 code, Int32 mask);</b>
 *
 * \source_delphi   <b>function canObjBufSetFilter(handle: canHandle; idx: Integer; code, mask: Cardinal): canStatus;      </b>
 * \source_end
 *
 * Defines a message reception filter on the specified object buffer.
 * Messages not matching the filter are discarded.
 *
 * \note For an auto response buffer, set the code and mask that together define
 * the identifier(s) that trigger(s) the automatic response.
 *
 * \param[in] hnd   An open handle to a CAN circuit.
 * \param[in] idx   The index of the object buffer on which the filter is to be
 *                  set.
 * \param[in] code  The acceptance code in the filter.
 * \param[in] mask  The acceptance mask in the filter.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_obj_buf
 *)
function canObjBufSetFilter(handle: canHandle; idx: Integer; code, mask: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canObjBufSetFilter';

(**
 * \ingroup ObjectBuffers
 *
 * \source_cs       <b>static Canlib.canStatus canObjBufSetFlags(CanHandle hnd, Int32 idx, Int32 flags);</b>
 *
 * \source_delphi   <b>function canObjBufSetFlags(handle: canHandle; idx: Integer; flags: Cardinal): canStatus;     </b>
 * \source_end
 *
 * Sets object buffer flags on a specified object buffer.
 *
 * \param[in] hnd    An open handle to a CAN circuit.
 * \param[in] idx    The buffer on which the flags are to be set.
 * \param[in] flags  Specifies a combination of zero or more of the
 *                   \ref canOBJBUF_AUTO_RESPONSE_xxx flag values
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_obj_buf
 *)
function canObjBufSetFlags(handle: canHandle; idx: Integer; flags: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canObjBufSetFlags';

(**
 * \ingroup ObjectBuffers
 *
 * \source_cs       <b>static Canlib.canStatus canObjBufSetPeriod(CanHandle hnd, Int32 idx, Int32 period);</b>
 *
 * \source_delphi   <b>function canObjBufSetPeriod(handle: canHandle; idx: Integer; period: Cardinal): canStatus;     </b>
 * \source_end
 *
 * The \ref canObjBufSetPeriod function sets the transmission period for an auto
 * transmission object buffer.
 *
 * \param[in] hnd     An open handle to a CAN channel.
 * \param[in] idx     The index of a CAN object buffer.
 * \param[in] period  The transmission interval, in microseconds.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_obj_buf
 *)
function canObjBufSetPeriod(handle: canHandle; idx: Integer; period: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canObjBufSetPeriod';

(**
 * \ingroup ObjectBuffers
 *
 * \source_cs       <b>static Canlib.canStatus canObjBufSetMsgCount(CanHandle hnd, Int32 idx, Int32 count);</b>
 *
 * \source_delphi   <b>function canObjBufSetMsgCount(handle: canHandle; idx: Integer; count: Cardinal): canStatus;     </b>
 * \source_end
 *
 * The \ref canObjBufSetMsgCount function sets the message count for an auto
 * transmit object buffer.
 *
 * \param[in] hnd    An open handle to a CAN channel.
 * \param[in] idx    The index of a CAN object buffer.
 * \param[in] count  The message count.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_obj_buf
 *)
function canObjBufSetMsgCount(handle: canHandle; idx: Integer; count: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canObjBufSetMsgCount';

(**
 * \ingroup ObjectBuffers
 *
 * \source_cs       <b>static Canlib.canStatus canObjBufEnable(CanHandle hnd, Int32 idx);</b>
 *
 * \source_delphi   <b>function canObjBufEnable(handle: canHandle; idx: Integer): canStatus;     </b>
 * \source_end
 *
 * Enables the object buffer with the specified index.
 *
 * \param[in] hnd  An open handle to a CAN circuit.
 * \param[in] idx  The index of the object buffer to enable.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_obj_buf
 * \sa \ref canObjBufDisable()
 *)
function canObjBufEnable(handle: canHandle; idx: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canObjBufEnable';

(**
 * \ingroup ObjectBuffers
 *
 * \source_cs       <b>static Canlib.canStatus canObjBufDisable(CanHandle hnd, Int32 idx);</b>
 *
 * \source_delphi   <b>function canObjBufDisable(handle: canHandle; idx: Integer): canStatus;     </b>
 * \source_end
 *
 * Disables the object buffer with the specified index.
 *
 * \param[in] hnd  An open handle to a CAN circuit.
 * \param[in] idx  The index of the buffer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_obj_buf
 * \sa \ref canObjBufEnable()
 *)
function canObjBufDisable(handle: canHandle; idx: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canObjBufDisable';

(**
 * \ingroup ObjectBuffers
 *
 * \source_cs       <b>static Canlib.canStatus canObjBufSendBurst(CanHandle hnd, Int32 idx, Int32 burstlen);</b>
 *
 * \source_delphi   <b>function canObjBufSendBurst(handle: canHandle; idx: Integer; burstLen: Cardinal): canStatus;      </b>
 * \source_end
 *
 * The canObjBufSendBurst function sends a burst of CAN messages. You have to
 * set up an object buffer first with the message to send. The messages will be
 * sent as fast as possible from the hardware.
 *
 * This function is inteneded for certain diagnostic applications.
 *
 * \param[in] hnd       An open handle to a CAN channel.
 * \param[in] idx       The index of a CAN object buffer.
 * \param[in] burstlen  The number of messages to send.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_obj_buf
 *)
function canObjBufSendBurst(handle: canHandle; idx: Integer; burstLen: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canObjBufSendBurst';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static bool canProbeVersion(CanHandle hnd, Int32 major, Int32 minor, Int32 oem_id, Int32 flags);</b>
 *
 * \source_delphi   <b>function canProbeVersion(handle: canHandle; major, minor, oem_id: Integer; flags: Cardinal): Boolean;     </b>
 * \source_end
 *
 * This function checks whether a specific version of CANlib is installed on
 * the system.
 *
 * The default behaviour of \ref canProbeVersion is to accept
 *
 * \li the version specified by \a major and \a minor, and
 * \li any later version, and
 * \li all beta versions.
 *
 * You get the default behaviour by setting \a flags to 0. Use any
 * combination of the \ref canVERSION_xxx flags to modify the behaviour.
 *
 * \note Different handles might have different driver versions installed. This
 * should not normally be the case but it might happen anyway. You should check
 * the version for each handle you open, e.g. directly after calling
 * \ref canOpenChannel().
 *
 * \param[in] hnd     A handle to an open circuit.
 * \param[in] major   The major version number of the version to test for.
 * \param[in] minor   The minor version number of the version to test for.
 * \param[in] oem_id  Reserved, must be zero.
 * \param[in] flags   Any combination of the \ref canVERSION_xxx flags, or 0.
 *
 * \return TRUE if the specified version of CANlib is installed on the system.
 *
 * \sa \ref page_user_guide_version
 * \sa \ref canGetVersion(), \ref canGetVersionEx(), \ref canGetChannelData()
 *)
function canProbeVersion(handle: canHandle; major, minor, oem_id: Integer; flags: Cardinal): Boolean; stdcall;
  external canlib32 name _PU + 'canProbeVersion';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canResetBus(CanHandle handle);</b>
 *
 * \source_delphi   <b>function canResetBus(handle: canHandle): canStatus;     </b>
 * \source_end
 *
 * This function tries to reset a CAN bus controller by taking the channel off
 * bus and then on bus again (if it was on bus before the call to \ref canResetBus().)
 *
 * This function will affect the hardware (and cause a real reset of the CAN
 * chip) only if \a hnd is the only handle open on the channel. If there
 * are other open handles, this operation will not affect the hardware.
 *
 * \param[in] hnd  A handle to an open circuit.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canBusOn(), \ref canBusOff()
 *)
function canResetBus(handle: canHandle): canStatus; stdcall;
  external canlib32 name _PU + 'canResetBus';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canWriteWait(CanHandle handle, Int32 id, Byte[] msg, Int32 dlc, Int32 flag, Int64 timeout);</b>
 *
 * \source_delphi   <b>function canWriteWait(handle: canHandle; id: Longint; var msg; dlc, flag, timeout: Cardinal): canStatus;     </b>
 * \source_end
 *
 * This function sends a CAN message. It returns when the message is sent, or
 * the timeout expires.
 *
 * This is a convenience function that combines \ref canWrite() and \ref canWriteSync().
 *
 * If you are using the same channel via multiple handles, note that the
 * default behaviour is that the different handles will "hear" each other just as
 * if each handle referred to a channel of its own. If you open, say, channel 0
 * from thread A and thread B and then send a message from thread A, it will be
 * "received" by thread B.
 * This behaviour can be changed using \ref canIOCTL_SET_LOCAL_TXECHO.
 *
 * \param[in]  hnd       A handle to an open CAN circuit.
 * \param[in]  id        The identifier of the CAN message to send.
 * \param[in]  msg       A pointer to the message data, or \c NULL.
 * \param[in]  dlc       The length of the message in bytes.<br>
                         For Classic CAN dlc can be at most 8, unless \ref
                         canOPEN_ACCEPT_LARGE_DLC is used.<br> For CAN FD dlc
                         can be one of the following 0-8, 12, 16, 20, 24, 32,
                         48, 64.
 * \param[in]  flag      A combination of message flags, \ref canMSG_xxx.
 *                       Use this parameter to send extended (29-bit) frames
 *                       and/or remote frames. Use \ref canMSG_EXT and/or
 *                       \ref canMSG_RTR for this purpose.
 * \param[in] timeout    The timeout, in milliseconds. 0xFFFFFFFF gives an
 *                       infinite timeout.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *)
function canWriteWait(handle: canHandle; id: Longint; var msg; dlc, flag, timeout: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'canWriteWait';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.canStatus canUnloadLibrary();</b>
 *
 * \source_delphi   <b>function canUnloadLibrary: Integer;     </b>
 * \source_end
 *
 * Use this function if you are loading canlib32.dll dynamically (that is,
 * using the Win32 API \c LoadLibrary) and need to unload it using the Win32
 * API \c FreeLibrary. \ref canUnloadLibrary() will free allocated memory, unload
 * the DLLs canlib32.dll has loaded and de-initialize data structures. You must
 * call \ref canInitializeLibrary() again to use the API functions in canlib32.dll.
 *
 * Calling \ref canUnloadLibrary() makes any open \ref CanHandle invalid. The proper
 * procedure for using \ref canUnloadLibrary() is:
 *
 *   -# Close all active handles using \ref canBusOff() and \ref canClose().
 *   -# At this point all handles earlier returned by \ref canOpenChannel() should be freed.
 *   -# Call \ref canUnloadLibrary().
 *   -# Call \ref canInitializeLibrary().
 *   -# You can now setup your handles to Kvaser devices from scratch
 *      (ie. \ref canOpenChannel(), \ref canSetBusParams(), \ref canBusOn(), etc).
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canInitializeLibrary()
 *)
function canUnloadLibrary(): canStatus; stdcall;
  external canlib32 name _PU + 'canUnloadLibrary';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canSetAcceptanceFilter(CanHandle hnd, Int32 code, Int32 mask, Int32 is_extended);</b>
 *
 * \source_delphi   <b>function canSetAcceptanceFilter(handle: canHandle; code, mask: Cardinal; is_extended: Integer): canStatus;     </b>
 * \source_end
 *
 * This routine sets the message acceptance filters on a CAN channel.
 *
 * Format of \a code and \a mask:
 *
 * \li A binary 1 in a mask means "the corresponding bit in the code is
 *     relevant"
 * \li A binary 0 in a mask means "the corresponding bit in the code is not
 *     relevant"
 * \li A relevant binary 1 in a code means "the corresponding bit in the
 *     identifier must be 1"
 * \li A relevant binary 0 in a code means "the corresponding bit in the
 *     identifier must be 0"
 *
 * In other words, the message is accepted if ((code XOR id) AND mask) == 0.
 *
 * \a extended should be set to:
 *
 * \li \c 0 (FALSE): if the code and mask shall apply to 11-bit CAN identifiers.
 * \li \c 1 (TRUE): if the code and mask shall apply to 29-bit CAN identifiers.
 *
 * If you want to remove a filter, call \ref canSetAcceptanceFilter() with the mask
 * set to \c 0.
 *
 * On some boards the acceptance filtering is done by the CAN hardware; on
 * other boards (typically those with an embedded CPU,) the acceptance
 * filtering is done by software.  \ref canSetAcceptanceFilter() behaves in the same
 * way for all boards, however.
 *
 * \ref canSetAcceptanceFilter() and \ref canAccept() both serve the same purpose but the
 * former can set the code and mask in just one call.
 *
 * \note You can set the extended code and mask only on CAN boards that support
 *       extended identifiers.
 *
 * \note Acceptance filters for 11-bit and 29-bit (ext) are independent, meaning 11-bit filters
 *  will not affect 29-bit can frames and vice versa.
 *
 * \note Not all CAN boards support different masks for standard and
 *       extended CAN identifiers.
 *
 * \note Not implemented in linux.
 *
 * \param[in] hnd          An open handle to a CAN circuit.
 * \param[in] code         The acceptance code to set.
 * \param[in] mask         The acceptance mask to set.
 * \param[in] is_extended  Select 29-bit CAN identifiers.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_send_recv_filters
 * \sa  \ref canAccept()
 *)
function canSetAcceptanceFilter(handle: canHandle; code, mask: Cardinal; is_extended: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canSetAcceptanceFilter';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canFlushReceiveQueue(CanHandle hnd);</b>
 *
 * \source_delphi   <b>function canFlushReceiveQueue(handle: canHandle): canStatus;     </b>
 * \source_end
 *
 * This function removes all received messages from the handle's receive queue.
 * Other handles open to the same channel are not affected by this
 * operation. That is, only the messages belonging to the handle you are
 * passing to \ref canFlushReceiveQueue are discarded.
 *
 * \note This call has the same effect as calling \ref canIoCtl() with a function
 * code of \ref canIOCTL_FLUSH_RX_BUFFER.
 *
 * \param[in] hnd  A handle to an open circuit.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canFlushTransmitQueue()
 *)
function canFlushReceiveQueue(handle: canHandle): canStatus; stdcall;
  external canlib32 name _PU + 'canFlushReceiveQueue';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canFlushTransmitQueue(CanHandle hnd);</b>
 *
 * \source_delphi   <b>function canFlushTransmitQueue(handle: canHandle): canStatus;     </b>
 * \source_end
 *
 * This function removes all messages pending transmission from the
 * transmit queue of the circuit.
 *
 * \note If there are other handles open to the same circuit, they are also
 * flushed.
 *
 * \note This call has the same effect as calling \ref canIoCtl() with a function
 * code of \ref canIOCTL_FLUSH_TX_BUFFER.
 *
 * \param[in] hnd  A handle to an open circuit.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canFlushReceiveQueue()
 *)
function canFlushTransmitQueue(handle: canHandle): canStatus; stdcall;
  external canlib32 name _PU + 'canFlushTransmitQueue';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvGetApplicationMapping(Int32 busType, String appName, Int32 appChannel, out Int32 resultingChannel);</b>
 *
 * \source_delphi   <b>function kvGetApplicationMapping(busType: Integer; appName: PAnsiChar; appChannel: Integer; var resultingChannel: Integer): canStatus;     </b>
 * \source_end
 *
 * \note The \ref kvGetApplicationMapping function is presently not implemented.
 *
 * \param busType
 * \param appName
 * \param appChannel
 * \param resultingChannel
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 *)
function kvGetApplicationMapping(busType: Integer; appName: PAnsiChar; appChannel: Integer; var resultingChannel: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'kvGetApplicationMapping';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvBeep(CanHandle hnd, Int32 freq, Int32 duration);</b>
 *
 * \source_delphi   <b>function kvBeep(handle: canHandle; freq: Integer; duration: Cardinal): canStatus;     </b>
 * \source_end
 *
 * The \ref kvBeep function emits a sound of a specific frequency and duration from
 * the loudspeaker on the device.
 *
 * \note This function requires that a loudspeaker be present on the hardware.
 *
 * \param[in] hnd       An open handle to a CAN channel.
 * \param[in] freq      The frequency (in Hertz) of the sound.
 * \param[in] duration  The duration of the sound, in milliseconds.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 *)
function kvBeep(handle: canHandle; freq: Integer; duration: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvBeep';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvSelfTest(CanHandle hnd, out Int32 presults);</b>
 *
 * \source_delphi   <b>function kvSelfTest(handle: canHandle; var presults: Cardinal): canStatus;     </b>
 * \source_end
 *
 * The \ref kvSelfTest function runs a built-in self test in the device. Note that
 * not all devices supports built-in self tests.
 *
 * \param[in]  hnd       An open hnd to a CAN channel.
 * \param[out] presults  A pointer to a 32-bit unsigned integer where the
 *                       results of the self test will be placed.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 *)
function kvSelfTest(handle: canHandle; var presults: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvSelfTest';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvFlashLeds(CanHandle hnd, Int32 action, Int32 timeout);</b>
 *
 * \source_delphi   <b>function kvFlashLeds(handle: canHandle; action: Integer; timeout: Integer): canStatus;     </b>
 * \source_end
 *
 * The \ref kvFlashLeds function will turn the LEDs on the device on or off.
 *
 * \param[in] hnd
 * \param[in] action   One of the \ref kvLED_ACTION_xxx constants, defining
 *                     which LED to turn on or off.
 * \param[in] timeout  Specifies the time, in milliseconds, during which the
 *                     action is to be carried out. When the timeout expires,
 *                     the LED(s) will return to its ordinary function.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 *)
function kvFlashLeds(handle: canHandle; action: Integer; timeout: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'kvFlashLeds';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canRequestChipStatus(CanHandle hnd);</b>
 *
 * \source_delphi   <b>function canRequestChipStatus(handle: canHandle): canStatus;     </b>
 * \source_end
 *
 * The canRequestChipStatus function requests that the hardware report the chip
 * status (bus on/error passive status etc.) to the driver. The chip status can
 * later be retrieved using the \ref canReadStatus() function.
 *
 * \note The \ref canRequestChipStatus() function is asynchronous, that is, it
 * completes before the answer is returned from the hardware.  The time between
 * a call to \ref canRequestChipStatus() and the point in time where the chip status
 * is actually available via a call to \ref canReadStatus() is not
 * defined. \ref canReadStatus() always returns the latest data reported by the
 * hardware.
 *
 * \param[in] hnd An open handle to a CAN channel.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canReadStatus()
 *)
function canRequestChipStatus(handle: canHandle): canStatus; stdcall;
  external canlib32 name _PU + 'canRequestChipStatus';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canRequestBusStatistics(CanHandle hnd);</b>
 *
 * \source_delphi   <b>function canRequestBusStatistics(handle: canHandle): canStatus;     </b>
 * \source_end
 *
 * The \ref canRequestBusStatistics function requests bus statistics from the
 * hardware. The bus statistics figures can be retrieved later by a call to the
 * \ref canGetBusStatistics function.
 *
 * \note The time between a call to \ref canRequestBusStatistics() and the point in
 * time where the bus statistics is actually available via a call to
 * \ref canGetBusStatistics() is not defined. Typically, you would call
 * \ref canRequestBusStatistics() from your application periodically (for example,
 * once per second) to request the data from the driver and then call
 * \ref canGetBusStatistics() with the same rate to obtain the latest reported data.
 *
 * \param[in] hnd   An open handle to a CAN channel.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canGetBusStatistics()
 *)
function canRequestBusStatistics(handle: canHandle): canStatus; stdcall;
  external canlib32 name _PU + 'canRequestBusStatistics';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canGetBusStatistics(CanHandle hnd, out Canlib.canBusStatistics stat);</b>
 *
 * \source_delphi   <b>function canGetBusStatistics(handle: canHandle; var stat: canBusStatistics; bufsiz: size_t): canStatus;     </b>
 * \source_end
 *
 * The \ref canGetBusStatistics() function retrieves the latest bus statistics
 * figures reported by the driver.  You request the bus statistics from the
 * driver by calling the \ref canRequestBusStatistics() function.
 *
 * \param[in]  hnd     An open handle to a CAN channel.
 * \param[out] stat    A pointer to a \ref canBusStatistics struct that will receive
 *                     the bus statistics figures.
 * \param[in]  bufsiz  The size, in bytes, of the stat buffer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canRequestBusStatistics()
 *)
function canGetBusStatistics(handle: canHandle; var stat: canBusStatistics; bufsiz: size_t): canStatus; stdcall;
  external canlib32 name _PU + 'canGetBusStatistics';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canSetBitrate(CanHandle hnd, Int32 bitrate);</b>
 *
 * \source_delphi   <b>function canSetBitrate(handle: canHandle; bitrate: Integer): canStatus;     </b>
 * \source_end
 *
 * The \ref canSetBitrate() function sets the nominal bit rate of the specified
 * CAN channel. The sampling point is recalculated and kept as close as
 * possible to the value before the call.
 *
 * \param[in] hnd      An open handle to a CAN channel.
 * \param[in] bitrate  The new bit rate, in bits/second.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canSetBusParamsC200(), \ref canSetBusParams(), \ref canGetBusParams()
 *)
function canSetBitrate(handle: canHandle; bitrate: Integer): canStatus; stdcall;
  external canlib32 name _PU + 'canSetBitrate';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvAnnounceIdentity(CanHandle hnd, Object buffer);</b>
 *
 * \source_delphi   <b>function kvAnnounceIdentity(handle: canHandle; var buf; bufsiz: size_t): canStatus;     </b>
 * \source_end
 *
 * The \ref kvAnnounceIdentity function is used by certain OEM applications.
 *
 * \param[in]  hnd     An open handle to a CAN channel.
 * \param[out] buf     A pointer to the data to be sent to the driver.
 * \param[in]  bufsiz  The size, in bytes, of the buffer that buf points to.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *)
function kvAnnounceIdentity(handle: canHandle; var buf; bufsiz: size_t): canStatus; stdcall;
  external canlib32 name _PU + 'kvAnnounceIdentity';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvAnnounceIdentityEx(CanHandle hnd, Int32 type, Byte[] data);</b>
 *
 * \source_delphi   <b>function kvAnnounceIdentityEx(handle: canHandle; tp: Integer; var buf; bufsiz: size_t): canStatus;     </b>
 *
 * \source_end
 * The \ref kvAnnounceIdentityEx function is used by certain OEM applications.
 *
 * \param[in]  hnd     An open handle to a CAN channel.
 * \param[in]  type    Type of announcement.
 * \param[out] buf     A pointer to the data to be sent to the driver.
 * \param[in]  bufsiz  The size, in bytes, of the buffer that buf points to.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *)
function kvAnnounceIdentityEx(handle: canHandle; tp: Integer; var buf; bufsiz: size_t): canStatus; stdcall;
  external canlib32 name _PU + 'kvAnnounceIdentityEx';

(**
 * \ingroup CAN
 *
 * \source_cs       <b>static Canlib.canStatus canGetHandleData(CanHandle hnd, Int32 item, out object buffer);</b>
 *
 * \source_delphi   <b>function canGetHandleData(handle: canHandle; item: Integer; var Buffer; bufsize: size_t): canStatus;     </b>
 * \source_end
 *
 * \note This function can be used to retrieve certain pieces of information about an open handle to a CANlib channel.
 *
 * \param[in]  hnd      An open handle to a CAN channel.
 * \param[in]  item     This parameter specifies what data to obtain for the specified handle. The value is one of the constants \ref canCHANNELDATA_xxx
 * \param[out] buffer   The address of a buffer which is to receive the data.
 * \param[in]  bufsize  The size of the buffer to which the buffer parameter points.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_unique_device
 * \sa \ref canGetChannelData()
 *)
function canGetHandleData(handle: canHandle; item: Integer; var Buffer; bufsize: size_t): canStatus; stdcall;
  external canlib32 name _PU + 'canGetHandleData';

(**
 * \ingroup TimeDomainHandling
 *
 * \source_cs       <b>static Canlib.kvStatus kvTimeDomainCreate(out object domain);</b>
 *
 * \source_delphi   <b>function kvTimeDomainCreate(var domain: kvTimeDomain): kvStatus;     </b>
 * \source_end
 *
 * This routine creates an empty time domain.
 *
 * The variable is set by this function and then used in later calls to
 * other functions using a \ref kvTimeDomain.
 *
 * Time domains created by \ref kvTimeDomainCreate() can be destroyed with a
 * call to \ref kvTimeDomainDelete().
 *
 * \note A time domain is a set of channels with a common time base.
 *
 * \param[out] domain  A pointer to a caller allocated, opaque variable of type
 *                     \ref kvTimeDomain that holds data to identify a particular
 *                     time domain.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_time_domain
 * \sa \ref kvTimeDomainDelete()
 *)
function kvTimeDomainCreate(var domain: kvTimeDomain): kvStatus; stdcall;
  external canlib32 name _PU + 'kvTimeDomainCreate';

(**
 * \ingroup TimeDomainHandling
 *
 * \source_cs       <b>static Canlib.kvStatus kvTimeDomainDelete(object domain);</b>
 *
 * \source_delphi   <b>function kvTimeDomainDelete(domain: kvTimeDomain): kvStatus;     </b>
 * \source_end
 *
 * This is a cleanup routine that deletes all members of a domain and then
 * deletes the domain itself.
 *
 * \note A time domain is a set of channels with a common time base.
 *
 * \param[in] domain  An opaque variable set by \ref kvTimeDomainCreate() that
 *                    identifies the domain to be deleted.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_time_domain
 * \sa \ref kvTimeDomainCreate()
 *)
function kvTimeDomainDelete(domain: kvTimeDomain): kvStatus; stdcall;
  external canlib32 name _PU + 'kvTimeDomainDelete';

(**
 * \ingroup TimeDomainHandling
 *
 * \source_cs       <b>static Canlib.kvStatus kvTimeDomainResetTime(object domain);</b>
 *
 * \source_delphi   <b>function kvTimeDomainResetTime(domain: kvTimeDomain): kvStatus;     </b>
 * \source_end
 *
 * This routine resets the time on all members of a time domain.
 *
 * After a call to this routine timestamps from all channels with MagiSync&tm;
 * running have no offset at all any longer. The same applies for channels that
 * reside on the same physical interface.
 *
 * \note A time domain is a set of channels with a common time base.
 *
 * \param[in] domain  An opaque variable set by \ref kvTimeDomainCreate() that
 *                    identifies the domain to reset the time on.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_time_domain
 * \sa \ref kvTimeDomainCreate()
 *)
function kvTimeDomainResetTime(domain: kvTimeDomain): kvStatus; stdcall;
  external canlib32 name _PU + 'kvTimeDomainResetTime';

(**
 * \ingroup TimeDomainHandling
 *
 * \source_cs       <b>static Canlib.kvStatus kvTimeDomainGetData(object domain, Canlib.kvTimeDomainData data);</b>
 *
 * \source_delphi   <b>function kvTimeDomainGetData(domain: kvTimeDomain; var data: kvTimeDomainData; bufsiz: size_t): kvStatus;     </b>
 * \source_end
 *
 * This routine collects some data on a time domain.
 *
 * \note A time domain is a set of channels with a common time base.
 *
 * \param[in]  domain  An opaque variable set by \ref kvTimeDomainCreate() that
 *                     identifies the domain to add a handle to.
 * \param[out] data    A pointer to a \ref kvTimeDomainData that is to be filled by
 *                     the function.
 * \param[in]  bufsiz  The size in bytes of the \ref kvTimeDomainData struct.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_time_domain
 * \sa \ref kvTimeDomainCreate()
 *)
function kvTimeDomainGetData(domain: kvTimeDomain; var data: kvTimeDomainData; bufsiz: size_t): kvStatus; stdcall;
  external canlib32 name _PU + 'kvTimeDomainGetData';

(**
 * \ingroup TimeDomainHandling
 *
 * \source_cs       <b>static Canlib.kvStatus kvTimeDomainAddHandle(object domain, CanHandle hnd);</b>
 *
 * \source_delphi   <b>function kvTimeDomainAddHandle(domain: kvTimeDomain; handle: canHandle): kvStatus;     </b>
 * \source_end
 *
 * This routine adds an open channel handle to a domain.
 *
 * \note A time domain is a set of channels with a common time base.
 *
 * \param[in] domain  An opaque variable set by \ref kvTimeDomainCreate()
 *                    that identifies the domain to add a handle to.
 * \param[in] hnd     A handle to an open channel.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_time_domain
 * \sa \ref kvTimeDomainCreate(), \ref kvTimeDomainRemoveHandle()
 *)
function kvTimeDomainAddHandle(domain: kvTimeDomain; handle: canHandle): kvStatus; stdcall;
  external canlib32 name _PU + 'kvTimeDomainAddHandle';

(**
 * \ingroup TimeDomainHandling
 *
 * \source_cs       <b>static Canlib.kvStatus kvTimeDomainRemoveHandle(object domain, CanHandle hnd);</b>
 *
 * \source_delphi   <b>function kvTimeDomainRemoveHandle(domain: kvTimeDomain; handle: canHandle): kvStatus;     </b>
 * \source_end
 *
 * This routine removes an open channel handle from a domain.
 *
 * \note A time domain is a set of channels with a common time base.
 *
 * \param[in] domain  An opaque variable set by \ref kvTimeDomainCreate()
 *                    that identifies the domain to remove a handle from.
 * \param[in] hnd     A handle to an open channel.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvTimeDomainCreate(), \ref kvTimeDomainAddHandle()
 *)
function kvTimeDomainRemoveHandle(domain: kvTimeDomain; handle: canHandle): kvStatus; stdcall;
  external canlib32 name _PU + 'kvTimeDomainRemoveHandle';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvSetNotifyCallback(CanHandle hnd, Canlib.kvCallbackDelegate callback, IntPtr context, Int32 notifyFlags);</b>
 *
 * \source_delphi   <b>function kvSetNotifyCallback(handle: canHandle; callback: kvCallback_t; context: Pointer; notifyFlags: Cardinal): canStatus;     </b>
 * \source_end
 *
 * The \ref kvSetNotifyCallback() function registers a callback function which is
 * called when certain events occur.
 *
 * You can register at most one callback function per handle at any time.
 *
 * To remove the callback, call \ref kvSetNotifyCallback() with a \c NULL pointer in
 * the callback argument.
 *
 * \note The callback function is called in the context of a high-priority
 * thread created by CANlib. You should take precaution not to do any time
 * consuming tasks in the callback.  You must also arrange the synchronization
 * between the callback and your other threads yourself.
 *
 * \param[in] hnd          An open handle to a CAN channel.
 * \param[in] callback     A pointer to a callback function of type
 *                         \ref kvCallback_t
 * \param[in] context      A pointer to arbitrary user-defined context data which
 *                         is passed to the callback function.
 * \param[in] notifyFlags  One or more of the \ref canNOTIFY_xxx flags.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canSetNotify()
 *)
function kvSetNotifyCallback(handle: canHandle; callback: kvCallback_t; context: Pointer; notifyFlags: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvSetNotifyCallback';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvGetSupportedInterfaceInfo(Int32 index, out String hwName, out Int32 hwType, out Int32 hwBusType);</b>
 *
 * \source_delphi   <b>function kvGetSupportedInterfaceInfo(index: Integer; hwName: PAnsiChar; nameLen: size_t; var hwType: Integer; var hwBusType: Integer): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvGetSupportedInterfaceInfo function returns information about the
 * different supported hardware types in the installed version of CANlib.
 *
 * This function is used to enumerate all the supported hardware types in the
 * installed version of CANlib. It does not return a complete list of all
 * supported devices, nor does it return a list of the presently installed
 * hardware. The returned data indicates which device families are supported.
 *
 * For example, a returned set of data might be:
 * \li \a hwType = \ref canHWTYPE_MEMORATOR_PRO
 * \li \a hwBusType = \ref kvBUSTYPE_USB
 * \li \a hwName = \c "Kvaser Memorator Professional"
 *
 * This means that
 * \li the presently installed version of CANlib supports members in the Kvaser
 *     Memorator Pro family (e.g. the HS/HS and the HS/LS),
 * \li the members of the Kvaser Memorator Pro family are USB devices,
 * \li the members of the Kvaser Memorator Pro family use the
 *     \ref canHWTYPE_MEMORATOR_PRO hardware type.
 *
 * The \ref kvGetSupportedInterfaceInfo() function is intended to help
 * application designers build a bus-oriented display of the different
 * installed and/or supported Kvaser devices in the computer.
 *
 * \note Not inplemented in linux.
 *
 * \param[in] index       Use this parameter to enumerate the different supported
 *                        hardware types. Start with index = 0, and then call
 *                        \ref kvGetSupportedInterfaceInfo again() with index =
 *                        1,2,3,... until the function returns an error code.
 * \param[out] hwName     A pointer to a buffer that will receive the name of the
 *                        hardware family, as a zero-terminated ASCII string.
 * \param[in]  nameLen    The length of the hwName buffer.
 * \param[out] hwType     Pointer to a 32-bit integer that will receive the
 *                        hardware type (one of the \ref canHWTYPE_xxx
 *                        constants.)
 * \param[out] hwBusType  Pointer to a 32-bit integer that will receive the bus
 *                        type (one of the \ref kvBUSTYPE_xxx constants.)
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canBusOn(), \ref canResetBus()
 *)
function kvGetSupportedInterfaceInfo(index: Integer; hwName: PAnsiChar; nameLen: size_t; var hwType: Integer; var hwBusType: Integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvGetSupportedInterfaceInfo';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvReadDeviceCustomerData(CanHandle hnd, Int32 userNumber, Int32 itemNumber, Byte[] data, Int32 bufsize);</b>
 *
 * \source_delphi   <b>function kvReadDeviceCustomerData(hnd: canHandle; userNumber, itemNumber: Integer; var data; bufsize: size_t): kvStatus;     </b>
 * \source_end
 *
 * Reading customer data works with Kvaser Leaf (of all
 * types), Kvaser USBcan Professional, Kvaser Memorator Professional, Kvaser Eagle and
 * Kvaser Memorator Light. To write customer data use external tools.
 *
 * \note Not implemented in linux.
 *
 * \param[in]  hnd        An open handle to a CAN channel.
 * \param[in]  userNumber Assigned by Kvaser.
 * \param[in]  itemNumber Must be zero (reserved)
 * \param[out] data       A pointer to a buffer of up to 8 bytes where
 *                        the result will be placed.
 * \param[in]  bufsiz     The size of the buffer that data points at.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *)
function kvReadDeviceCustomerData(hnd: canHandle; userNumber, itemNumber: Integer; var data; bufsize: size_t): kvStatus; stdcall;
  external canlib32 name _PU + 'kvReadDeviceCustomerData';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptStart(CanHandle hnd, Int32 slotNo);</b>
 *
 * \source_delphi   <b>function kvScriptStart(const hnd: canHandle; slotNo: integer): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptStart() function starts a loaded script.
 *
 * \param[in] hnd     An open handle to a CAN channel.
 * \param[in] slotNo  The slot with the loaded script we want to start.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_start_stop
 * \sa \ref kvScriptLoadFile(), \ref kvScriptStop()
 *)
function kvScriptStart(const hnd: canHandle; slotNo: integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptStart';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptStop(CanHandle hnd, Int32 slotNo, Int32 mode);</b>
 *
 * \source_delphi   <b>function kvScriptStop(const hnd: canHandle; slotNo: integer; mode: integer): kvStatus;     </b>
 * \source_end
 *
 *  The \ref kvScriptStop() function stops a started script.
 *
 * \param[in] hnd     An open handle to a CAN channel.
 * \param[in] slotNo  The slot with the loaded and running script we want to stop.
 * \param[in] mode    Stop mode of type \ref kvSCRIPT_STOP_xxx
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_start_stop
 * \sa \ref kvScriptLoadFile(), \ref kvScriptStart()
 *)
function kvScriptStop(const hnd: canHandle; slotNo: integer; mode: integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptStop';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptUnload(CanHandle hnd, Int32 slotNo);</b>
 *
 * \source_delphi   <b>function kvScriptUnload(const hnd: canHandle; slotNo: integer): kvStatus;     </b>
 * \source_end
 *
 *  The \ref kvScriptUnload() function unloads a stopped script.
 *
 * \param[in] hnd     An open handle to a CAN channel.
 * \param[in] slotNo  The slot with the loaded and stopped script we want to unload.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_loading
 * \sa \ref kvScriptLoadFile(), \ref kvScriptStop()
 *)
function kvScriptUnload(const hnd: canHandle; slotNo: integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptUnload';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptSendEvent(CanHandle hnd, Int32 slotNo, Int32 eventType, Int32 eventNo, Int32 data);</b>
 *
 * \source_delphi   <b>function kvScriptSendEvent(const hnd: canHandle; slotNo: integer; eventType: integer; eventNo: integer; data: Cardinal): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptSendEvent() function sends an event of a type, and an event
 * number and associated data to a script running in a specific slot.
 *
 * \note Not implemented in linux.
 *
 * \param[in] hnd        An open handle to a CAN channel.
 * \param[in] slotNo     The slot where the script was loaded and is running.
 * \param[in] eventType  The event to send, of type \ref kvEVENT_xxx
 * \param[in] eventNo    The event's number.
 * \param[in] data       The event's data.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_send_event
 *)
function kvScriptSendEvent(const hnd: canHandle; slotNo: integer; eventType: integer; eventNo: integer; data: Cardinal): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptSendEvent';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static kvEnvHandle kvScriptEnvvarOpen(CanHandle hnd, String envvarName, out Int32 envvarType, out Int32 envvarSize);</b>
 *
 * \source_delphi   <b>function kvScriptEnvvarOpen(const hnd: canHandle; envvarName: PAnsiChar; var envvarType: Integer; var envvarSize: Integer): kvEnvHandle;     </b>
 * \source_end
 *
 * The \ref kvScriptEnvvarOpen() opens an existing envvar and returns a handle to it.
 *
 * \param[in]  hnd         An open handle to a CAN channel.
 * \param[in]  envvarName  The envvar's name; a pointer to a \c NULL terminated
 *                         array of chars.
 * \param[out] envvarType  A pointer to a 32-bit integer that will receive the
 *                         \ref kvENVVAR_TYPE_xxx type.
 * \param[out] envvarSize  A pointer to a 32-bit integer that will receive the
 *                         size of the envvar in bytes.
 *
 * \return A \ref kvEnvHandle handle (positive) to an envvar if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_envvar
 * \sa \ref kvScriptEnvvarClose()
 *)
function kvScriptEnvvarOpen(const hnd: canHandle; envvarName: PAnsiChar; var envvarType: Integer; var envvarSize: Integer): kvEnvHandle; stdcall;
  external canlib32 name _PU + 'kvScriptEnvvarOpen';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptEnvvarClose(kvEnvHandle eHnd);</b>
 *
 * \source_delphi   <b>function kvScriptEnvvarClose(const eHnd: kvEnvHandle): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptEnvvarClose() function closes an open envvar.
 *
 * \param[in] eHnd  An open handle to an envvar.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvScriptEnvvarOpen()
 *)
function kvScriptEnvvarClose(const eHnd: kvEnvHandle): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptEnvvarClose';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptEnvvarSetInt(kvEnvHandle eHnd, Int32 val);</b>
 *
 * \source_delphi   <b>function kvScriptEnvvarSetInt(const eHnd: kvEnvHandle; val: Integer): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptEnvvarSetInt() sets the value of an \c int envvar.
 *
 * \param[in] eHnd  An open handle to an envvar.
 * \param[in] val   The new value.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_envvar
 * \sa \ref kvScriptEnvvarOpen(), \ref kvScriptEnvvarGetInt(), \ref kvScriptEnvvarSetFloat(),
 * \ref kvScriptEnvvarSetData()
 *)
function kvScriptEnvvarSetInt(const eHnd: kvEnvHandle; val: Integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptEnvvarSetInt';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptEnvvarGetInt(kvEnvHandle eHnd, out Int32 val);</b>
 *
 * \source_delphi   <b>function kvScriptEnvvarGetInt(const eHnd: kvEnvHandle; var val: Integer): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptEnvvarGetInt() function retrieves the value of an \c int envvar.
 *
 * \param[in]  eHnd An open handle to an envvar.
 * \param[out] val  The current value.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_envvar
 * \sa \ref kvScriptEnvvarOpen(), \ref kvScriptEnvvarSetInt(), \ref kvScriptEnvvarGetFloat(),
 * \ref kvScriptEnvvarGetData()
 *
 *)
function kvScriptEnvvarGetInt(const eHnd: kvEnvHandle; var val: Integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptEnvvarGetInt';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptEnvvarSetFloat(kvEnvHandle eHnd, Single val);</b>
 *
 * \source_delphi   <b>function kvScriptEnvvarSetFloat(const eHnd: kvEnvHandle; val: Single): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptEnvvarSetFloat() sets the value of a \c float envvar.
 *
 * \note Not implemented in linux.
 *
 * \param[in] eHnd  An open handle to an envvar.
 * \param[in] val   The new value.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_envvar
 * \sa \ref kvScriptEnvvarOpen(), \ref kvScriptEnvvarGetFloat(), \ref kvScriptEnvvarSetInt(),
 * \ref kvScriptEnvvarSetData()
 *)
function kvScriptEnvvarSetFloat(const eHnd: kvEnvHandle; val: Single): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptEnvvarSetFloat';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptEnvvarGetFloat(kvEnvHandle eHnd, out Single val);</b>
 *
 * \source_delphi   <b>function kvScriptEnvvarGetFloat(const eHnd: kvEnvHandle; var val: Single): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptEnvvarGetFloat() function retrieves the value of a \c float envvar.
 *
 * \note Not implemented in linux.
 *
 * \param[in]  eHnd  An open handle to an envvar.
 * \param[out] val   A pointer to a \c float where the retrieved result should be
 *                   stored.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_envvar
 * \sa \ref kvScriptEnvvarOpen(), \ref kvScriptEnvvarSetFloat(), \ref kvScriptEnvvarGetInt(),
 * \ref kvScriptEnvvarGetData()
 *)
function kvScriptEnvvarGetFloat(const eHnd: kvEnvHandle; var val: Single): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptEnvvarGetFloat';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptEnvvarSetData(kvEnvHandle eHnd, byte[] buf, Int32 start_index, Int32 data_len);</b>
 *
 * \source_delphi   <b>function kvScriptEnvvarSetData(const eHnd: kvEnvHandle; var buf; start_index: Integer; data_len: Integer): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptEnvvarSetData() function sets a range of data bytes in an envvar.
 *
 * \note Not implemented in linux.
 *
 * \param[in] eHnd         An open handle to an envvar.
 * \param[in] buf          A pointer to a data area with the new values.
 * \param[in] start_index  The start index of the envvar's data range that we
 *                         want to update.
 * \param[in] data_len     The length in bytes of the envvar's data range that
 *                         we want to update.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_envvar
 * \sa \ref kvScriptEnvvarOpen(), \ref kvScriptEnvvarGetData(), \ref kvScriptEnvvarSetInt(),
 * \ref kvScriptEnvvarSetFloat()
 *)
function kvScriptEnvvarSetData(const eHnd: kvEnvHandle; var buf; start_index: Integer; data_len: Integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptEnvvarSetData';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptEnvvarGetData(kvEnvHandle eHnd, out byte[] buf, Int32 start_index, Int32 data_len);</b>
 *
 * \source_delphi   <b>function kvScriptEnvvarGetData(const eHnd: kvEnvHandle; var buf; start_index: Integer; data_len: Integer): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptEnvvarGetData() function retrieves a range of data bytes from an envvar.
 *
 * \note Not implemented in linux.
 *
 * \param[in]  eHnd         An open handle to an envvar.
 * \param[out] buf          A pointer to a data area where the retrieved data
 *                          range should be stored.
 * \param[in]  start_index  The start index of the data range.
 * \param[in]  data_len     The length in bytes of the data range.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_envvar
 * \sa \ref kvScriptEnvvarOpen(), \ref kvScriptEnvvarSetData(), \ref kvScriptEnvvarGetInt(),
 * \ref kvScriptEnvvarGetFloat()
 *)
function kvScriptEnvvarGetData(const eHnd: kvEnvHandle; var buf; start_index: Integer; data_len: Integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptEnvvarGetData';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptLoadFileOnDevice(CanHandle hnd, Int32 slotNo, String localFile);</b>
 *
 * \source_delphi   <b>function kvScriptLoadFileOnDevice(hnd: canHandle; slotNo: Integer; localFile: PAnsiChar): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptLoadFileOnDevice() function loads a compiled script file (.txe)
 * stored on the device (SD card) into a script slot on the device.
 *
 * \param[in] hnd        An open handle to a CAN channel.
 * \param[in] slotNo     The slot where to load the script.
 * \param[in] localFile  The script file name; a pointer to a \c NULL terminated
 *                       array of chars.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_loading
 * \sa \ref kvScriptLoadFile(), \ref kvFileCopyToDevice(), \ref kvScriptStart(),
 * \ref kvScriptStop()
 *)
function kvScriptLoadFileOnDevice(hnd: canHandle; slotNo: Integer; localFile: PAnsiChar): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptLoadFileOnDevice';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptLoadFile(CanHandle hnd, Int32 slotNo, String filePathOnPC);</b>
 *
 * \source_delphi   <b>function kvScriptLoadFile(hnd: canHandle; slotNo: Integer; filePathOnPC: PAnsiChar): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptLoadFile() function loads a compiled script file (.txe) stored
 * on the host (PC) into a script slot on the device.
 *
 * \note The canHandle is used to determine what channel is set as the default
 * channel for the loaded script. If your canHandle was opened via a device's
 * second channel, the default channel number will be set to 1 (the numbering
 * of channel on the card starts from 0).
 *
 * \param[in] hnd           An open handle to a CAN channel.
 * \param[in] slotNo        The slot where to load the script.
 * \param[in] filePathOnPC  The script file name; a pointer to a \c NULL
 *                          terminated array of chars.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref section_user_guide_kvscript_loading
 * \sa \ref kvScriptLoadFileOnDevice(), \ref kvFileCopyToDevice(), \ref kvScriptStart(),
 * \ref kvScriptStop()
 *)
function kvScriptLoadFile(hnd: canHandle; slotNo: Integer; filePathOnPC: PAnsiChar): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptLoadFile';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptRequestText(CanHandle hnd, Int32 slot, Int32 request);</b>
 *
 * \source_delphi   <b>function kvScriptRequestText(hnd: canHandle; slotNo: cardinal; request: cardinal): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptRequestText() Sets up a printf subscription to a
 * selected script slot.
 * Read the printf messages with \ref kvScriptGetText().
 *
 * \param[in] hnd          An open handle to a CAN channel.
 * \param[in] slot         The slot to subscribe to.
 * \param[in] request      Subscription request i.e. \ref kvSCRIPT_REQUEST_TEXT_xxx.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 *)
function kvScriptRequestText(hnd: canHandle; slotNo: cardinal; request: cardinal): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptRequestText';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptGetText(CanHandle hnd, out Int32 slot, out Int64 time, out Int32 flags, out String buf);</b>
 *
 * \source_delphi   <b>function kvScriptGetText(hnd: canHandle; var slot: integer; var time: Cardinal; var flags: Cardinal; buf: PAnsiChar; bufsize: size_t): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvScriptGetText() Reads a printf from a subscribed script slot.
 * Set up a subscription with \ref kvScriptRequestText().
 *
 * \param[in] hnd           An open handle to a CAN channel.
 * \param[out] slot         The slot where the printf originated.
 * \param[out] time         The printf timestamp.
 * \param[out] flags        Printf flags. A combination of \ref canSTAT_xxx flags.
 * \param[out] buf          Buffer to hold the printf string.
 * \param[in] bufsize       Size of the buffer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 *)
function kvScriptGetText(hnd: canHandle; var slot: integer; var time: Cardinal; var flags: Cardinal; buf: PAnsiChar; bufsize: size_t): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptGetText';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptStatus(CanHandle hnd, Int32 slotNo, out Int32 status);</b>
 *
 * \source_delphi   <b>function kvScriptStatus(hnd: canHandle; slot: integer; var status: Cardinal): kvStatus;</b>
 * \source_end
 *
 * The \ref kvScriptStatus() function reads the current status of a script slot.
 *
 * \param[in] hnd           An open handle to a CAN channel.
 * \param[in] slot          The slot which status we want.
 * \param[out] status       The script status, as \ref kvSCRIPT_STATUS_xxx flag bits
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *)
function kvScriptStatus(hnd: canHandle; slot: integer; var status: Cardinal): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptStatus';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptGetMaxEnvvarSize(CanHandle hnd, out Int32 size);</b>
 *
 * \source_delphi   <b>function kvScriptGetMaxEnvvarSize(hnd: canHandle; var envvarSize: Integer): kvStatus;</b>
 * \source_end
 *
 * The \ref kvScriptGetMaxEnvvarSize() function returns the maximum size of an envvar.
 *
 * \note Not implemented in linux.
 *
 * \param[in] hnd           An open handle to a CAN channel.
 * \param[out] envvarSize   The maximum size of an envvar.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *)
function kvScriptGetMaxEnvvarSize(hnd: canHandle; var envvarSize: Integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvScriptGetMaxEnvvarSize';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvScriptTxeGetData(String filePathOnPC, Int32 item, out object buffer);</b>
 *
 * \source_delphi   <b>function kvScriptTxeGetData(filePathOnPC: PAnsiChar; item: Integer; var buffer; var bufsize: Cardinal): canStatus;</b>
 * \source_end
 *
 * This function can be used to retrieve information from a compiled script file (.txe).
 *
 * \param[in] filePathOnPC  The compiled script file name; a pointer to a \c NULL
 *                          terminated array of chars.
 *
 * \param[in] item          This parameter specifies what data to obtain. Valid values are one of the constants
 *                          \ref canTXEDATA_xxx.
 * \param[out] buffer       The address of a buffer which is to receive the data.
 * \param[in,out] bufsize   The size of the buffer to which the buffer parameter
 *                          points. When the function returns, bufsize contains the number of bytes copied into the buffer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * If the buffer specified by \a buffer and \a bufsize is not large enough to hold the data requested,
 * the function returns \ref canERR_BUFFER_TOO_SMALL and stores the required buffer size in the integer pointed to by \a bufsize.
 * The content of buffer is undefined.
 *
 * If parameter \a buffer is NULL, and bufsize is non-NULL, the function returns \ref canOK and stores the required
 * buffer size in the integer pointed to by \a bufsize.
 *
 *)
function kvScriptTxeGetData(filePathOnPC: PAnsiChar; item: Integer; var buffer; var bufsize: Cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvScriptTxeGetData';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvFileCopyToDevice(CanHandle hnd, String hostFileName, String deviceFileName);</b>
 *
 * \source_delphi   <b>function kvFileCopyToDevice(hnd: canHandle; hostFileName: PAnsiChar; deviceFileName: PAnsiChar): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvFileCopyToDevice() function copies an arbitrary file from the host to
 * the device.
 *
 * \param[in] hnd             An open handle to a CAN channel.
 * \param[in] hostFileName    The host file name; a pointer to a \c NULL terminated
 *                            array of chars.
 * \param[in] deviceFileName  The target device file name; a pointer to a \c NULL
 *                            terminated array of chars.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvFileCopyFromDevice(), \ref kvFileDelete()
 *)
function kvFileCopyToDevice(hnd: canHandle; hostFileName: PAnsiChar; deviceFileName: PAnsiChar): kvStatus; stdcall;
  external canlib32 name _PU + 'kvFileCopyToDevice';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvFileCopyFromDevice(CanHandle hnd, String deviceFileName, String hostFileName);</b>
 *
 * \source_delphi   <b>function kvFileCopyFromDevice(hnd: canHandle; deviceFileName: PAnsiChar; hostFileName: PAnsiChar): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvFileCopyFromDevice() function copies an arbitrary file from the device
 * to the host.
 *
 * \param[in] hnd             An open handle to a CAN channel.
 * \param[in] deviceFileName  The device file name; a pointer to a \c NULL
 *                            terminated array of chars.
 * \param[in] hostFileName    The target host file name; a pointer to a \c NULL terminated
 *                            array of chars.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvFileCopyToDevice()
 *)
function kvFileCopyFromDevice(hnd: canHandle; deviceFileName: PAnsiChar; hostFileName: PAnsiChar): kvStatus; stdcall;
  external canlib32 name _PU + 'kvFileCopyFromDevice';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvFileDelete(CanHandle hnd, String deviceFileName);</b>
 *
 * \source_delphi   <b>function kvFileDelete(hnd: canHandle; deviceFileName: PAnsiChar): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvFileDelete() function deletes a file on the device.
 *
 * \note Deleting system files is not recommended.
 *
 * \param[in] hnd             An open handle to a CAN channel.
 * \param[in] deviceFileName  The file on the device to delete; a pointer
 *                            to a \c NULL terminated array of chars.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvFileCopyToDevice()
 *)
function kvFileDelete(hnd: canHandle; deviceFileName: PAnsiChar): kvStatus; stdcall;
  external canlib32 name _PU + 'kvFileDelete';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvFileGetName(CanHandle hnd, Int32 fileNo, out String name);</b>
 *
 * \source_delphi   <b>function kvFileGetName(hnd: canHandle; fileNo: Integer; name: PAnsiChar; namelen: Integer): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvFileGetName() function returns the name of the file with
 *   number \a fileNo.
 *
 * \param[in]  hnd      An open handle to a CAN channel.
 * \param[in]  fileNo   The number of the file.
 * \param[out] name     A buffer that will contain the name of the
 *                      file. The name is a zero-terminated ASCII string.
 * \param[in]  namelen  The length, in bytes, of the \a name buffer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvFileGetCount(), \ref kvFileGetSystemData()
 *)
function kvFileGetName(hnd: canHandle; fileNo: Integer; name: PAnsiChar; namelen: Integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvFileGetName';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvFileGetCount(CanHandle hnd, out Int32 count);</b>
 *
 * \source_delphi   <b>function kvFileGetCount(hnd: canHandle; var count: Integer): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvFileGetCount() function returns the number of files.
 *
 * \param[in]  hnd    An open handle to a CAN channel.
 * \param[out] count  A pointer to a 32-bit integer that will receive
 *                    the file count.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvFileGetName(), \ref kvFileGetSystemData()
 *)
function kvFileGetCount(hnd: canHandle; var count: Integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvFileGetCount';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.kvStatus kvFileGetSystemData(CanHandle hnd, Int32 itemCode, out Int32 result);</b>
 *
 * \source_delphi   <b>function kvFileGetSystemData(hnd: canHandle; itemCode: Integer; var result: Integer): kvStatus; </b>
 * \source_end
 *
 * The \ref kvFileGetSystemData() function is used for reading disk parameters,
 * e.g. size, max number of (user) files, etc.
 *
 * \note Not yet implemented
 *
 * \param[in]  hnd       An open handle to a CAN channel.
 * \param[in]  itemCode  The item we want information on.
 * \param[out] result    A pointer to a 32-bit integer that will
 *                       receive the result.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 *)
function kvFileGetSystemData(hnd: canHandle; itemCode: Integer; var result: Integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvFileGetSystemData';

(**
 * \ingroup tScript
 *
 * \source_cs       <b>static Canlib.canStatus  kvFileDiskFormat(CanHandle hnd);</b>
 *
 * \source_delphi   <b>function kvFileDiskFormat(hnd: canHandle): kvStatus; </b>
 * \source_end
 *
 * The \ref kvFileDiskFormat() function is used for formating the disk,
 * back to FAT32.
 *
 *
 * \param[in] hnd       An open handle to a CAN channel.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 *)
function kvFileDiskFormat(hnd: canHandle): kvStatus; stdcall;
  external canlib32 name _PU + 'kvFileDiskFormat';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvDeviceSetMode(CanHandle hnd, Int32 mode);</b>
 *
 * \source_delphi   <b>function kvDeviceSetMode(hnd: canHandle; mode: Integer): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvDeviceSetMode() sets the mode.
 *
 * \note The mode is device specific, which means that not all modes are
 * implemented in all products.
 *
 * \param[in] hnd   An open handle to a CAN channel.
 * \param[in] mode  One of the \ref kvDEVICE_MODE_xxx constants,
 *                  defining which mode to use.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDeviceGetMode()
 *)
function kvDeviceSetMode(hnd: canHandle; mode: Integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvDeviceSetMode';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvDeviceGetMode(CanHandle hnd, out Int32 result);</b>
 *
 * \source_delphi   <b>function kvDeviceGetMode(hnd: canHandle; var mode: Integer): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvDeviceGetMode() reads the current device's specific mode.
 *
 * \note The mode is device specific, which means that not all modes are
 * implemented in all products.
 *
 * \param[in]  hnd     An open handle to a CAN channel.
 * \param[out] result  A pointer to a 32-bit integer that will receive the
 *                     \ref kvDEVICE_MODE_xxx value.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDeviceSetMode()
 *)
function kvDeviceGetMode(hnd: canHandle; var mode: Integer): kvStatus; stdcall;
  external canlib32 name _PU + 'kvDeviceGetMode';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvPingRequest(CanHandle hnd, out Int32 requestTime);</b>
 *
 * \source_delphi   <b>function kvPingRequest(hnd: canHandle; var requestTime: Cardinal): kvStatus;     </b>
 * \source_end
 *
 * This function sends an active ping to a device. The ping time can later be
 * retrieved using \ref kvPingGetLatest().
 *
 * \param[in]  hnd          A handle to an open circuit.
 * \param[out] requestTime  Time of request in microseconds. Used for matching answer to request.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *)
function kvPingRequest(hnd: canHandle; var requestTime: Cardinal): kvStatus; stdcall;
  external canlib32 name _PU + 'kvPingRequest';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvPingGetLatest(CanHandle hnd, out Int32 requestTime, out Int32 pingTime);</b>
 *
 * \source_delphi   <b>function kvPingGetLatest(hnd: canHandle; var requestTime: Cardinal; var pingTime: Cardinal): kvStatus;     </b>
 * \source_end
 *
 * This retrieves the latest ping time issued by an earlier call to \ref kvPingRequest()
 *
 * \param[in]  hnd         A handle to an open circuit.
 * \param[out] requestTime Time of request in microseconds. Used for matching answer to request.
 * \param[out] pingTime    Latest value of ping time in milliseconds.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *)
function kvPingGetLatest(hnd: canHandle; var requestTime: Cardinal; var pingTime: Cardinal): kvStatus; stdcall;
  external canlib32 name _PU + 'kvPingGetLatest';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvReadTimer(CanHandle hnd, out Int32 time);</b>
 *
 * \source_delphi   <b>function kvReadTimer(handle: canHandle; var time: Cardinal): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvReadTimer reads the hardware clock on the specified device and returns
 * the value.
 *
 * When the call to \ref kvReadTimer() returns, the time value is already
 * obsolete. The time required for the device firmware, any intermediary buses
 * (like USB,) and the operating system to return the time value is not
 * defined.
 *
 * This call should be used instead of \ref canReadTimer() because it can return an
 * error code if it fails.
 *
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[out] time  A pointer to a 32-bit unsigned integer that will receive
 *                   the time value.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref canReadTimer(), \ref kvReadTimer64()
 *)
function kvReadTimer(handle: canHandle; var time: Cardinal): kvStatus; stdcall;
  external canlib32 name _PU + 'kvReadTimer';

(**
 * \ingroup can_general
 *
 * \source_cs       <b>static Canlib.kvStatus kvReadTimer64(CanHandle hnd, out Int64 time);</b>
 *
 * \source_delphi   <b>function kvReadTimer64(handle: canHandle; var time: Int64): kvStatus;     </b>
 * \source_end
 *
 * The \ref kvReadTimer64 reads the hardware clock on the specified device and
 * returns the value.
 *
 * When the call to \ref kvReadTimer64() returns, the time value is already
 * obsolete. The time required for the device firmware, any intermediary buses
 * (like USB,) and the operating system to return the time value is not
 * defined.
 *
 * This call should be used instead of \ref canReadTimer() because it can return an
 * error code if it fails.
 *
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[out] time  A pointer to a 64-bit signed integer that will receive the
 *                   time value.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvReadTimer(), \ref canReadTimer()
 *)
function kvReadTimer64(handle: canHandle; var time: Int64): kvStatus; stdcall;
  external canlib32 name _PU + 'kvReadTimer64';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.kvStatus kvIoGetNumberOfPins(CanHandle handle, out Int32 pinCount);</b>
 *
 * \source_delphi   <b>function kvIoGetNumberOfPins(hnd: canHandle; var pinCount: cardinal): canStatus;</b>
 * \source_end
 *
 * Get the number of I/O pins available from a device.
 *
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[out] pinCount  A pointer to an int which receives the number of pins.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvIoPinGetInfo(), \ref kv_io
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoGetNumberOfPins(hnd: canHandle; var pinCount: cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoGetNumberOfPins';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.kvStatus kvIoConfirmConfig(CanHandle handle);</b>
 *
 * \source_delphi   <b>function kvIoConfirmConfig(hnd: canHandle): canStatus;</b>
 * \source_end
 *
 * This function is used to confirm configuration. It is required to call this function, before it is possible to use any kvIoPinSetXxx()/kvIoPinGetXxx() function. After a configuration change, module removal or insertion, it is required to confirm the new configuration.
 *
 * \param[in]  hnd   An open handle to a CAN channel.
 *
 * \sa \ref kv_io
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoConfirmConfig(hnd: canHandle): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoConfirmConfig';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.kvStatus kvIoPinGetInfo(CanHandle handle, Int32 pin, kvIOGetInfo item, ref Object data);</b>
 *
 * \source_delphi   <b>function kvIoPinGetInfo(hnd: canHandle; pin: cardinal; item: Integer; var buffer; bufsize: cardinal): canStatus;</b>
 * \source_end
 * This function is used to retrieve I/O pin properties.
 *
 * \param[in]  hnd        An open handle to a CAN channel.
 * \param[in]  pin        The pin number, see \ref kvIoGetNumberOfPins.
 * \param[in]  item       Type of item to retrieve \ref kvIO_INFO_GET_xxx.
 * \param[out] buffer     The address of a buffer which is to receive the data.
 * \param[in]  bufsize    The size of the buffer, matching the size of selected \ref kvIO_INFO_GET_xxx item.
 *
 * \sa \ref kvIoPinSetInfo(), \ref kv_io
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoPinGetInfo(hnd: canHandle; pin: cardinal; item: Integer; var buffer; bufsize: cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoPinGetInfo';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.kvStatus kvIoPinSetInfo(CanHandle handle, Int32 pin, kvIOSetInfo item, Object data);</b>
 *
 * \source_delphi   <b>function kvIoPinSetInfo(hnd: canHandle; pin: cardinal; item: Integer; var buffer; bufsize: cardinal): canStatus;</b>
 * \source_end
 * This function is used to set I/O pin properties, for items that can be changed.
 *
 * \param[in]  hnd        An open handle to a CAN channel.
 * \param[in]  pin        The pin number, see \ref kvIoGetNumberOfPins.
 * \param[in]  item       Type of item to set, see \ref kvIO_INFO_SET_xxx.
 * \param[in]  buffer     The address of a buffer contains the data to set.
 * \param[in]  bufsize    The size of the buffer, matching the size of selected \ref kvIO_INFO_SET_xxx item.
 *
 * \sa \ref kvIoPinGetInfo()
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoPinSetInfo(hnd: canHandle; pin: cardinal; item: Integer; var buffer; bufsize: cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoPinSetInfo';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.kvStatus kvIoPinSetDigital(CanHandle handle, Int32 pin, Int32 value);</b>
 *
 * \source_delphi   <b>function kvIoPinSetDigital(hnd: canHandle; pin: cardinal; value: cardinal): canStatus;</b>
 * \source_end
 * This function is used to set a digital output I/O pin. If \a value is zero,
 * the pin is set LOW. For any non-zero \a value, the pin is set HIGH.
 *
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[in]  pin   The pin number, see \ref kvIoGetNumberOfPins.
 * \param[in]  value An unsigned int which sets a value of the pin.
 *
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoPinSetDigital(hnd: canHandle; pin: cardinal; value: cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoPinSetDigital';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.kvStatus kvIoPinGetDigital(CanHandle handle, Int32 pin, out Int32 value);</b>
 *
 * \source_delphi   <b>function kvIoPinGetDigital(hnd: canHandle; pin: cardinal; var value: cardinal): canStatus;</b>
 * \source_end
 * This function is used to retrieve the value of the specified digital input I/O pin.
 * If the pin is LOW, the integer pointed to by \a value is assigned zero.
 * If the pin is HIGH, the integer pointed to by \a value is assigned a '1'.
 *
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[in]  pin   The pin number, see \ref kvIoGetNumberOfPins.
 * \param[out] value A pointer to an unsigned int which receives the value of the pin.
 *
 * \sa \ref kvIoPinSetDigital()
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoPinGetDigital(hnd: canHandle; pin: cardinal; var value: cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoPinGetDigital';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.kvStatus kvIoPinGetOutputDigital(CanHandle handle, Int32 pin, out Int32 value);</b>
 *
 * \source_delphi   <b>function kvIoPinGetOutputDigital(hnd: canHandle; pin: cardinal; var value: cardinal): canStatus;</b>
 * \source_end
 * This function is used to get the latest set value of a digital output I/O pin.
 * If the latest value written to the pin is LOW, the integer pointed to by \a value is assigned zero.
 * If it is HIGH, the integer pointed to by \a value is assigned a '1'.
 * This function only returns values as they are presented in memory and the actual value on the output pin may therefore differ.
 *
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[in]  pin   The pin number, see \ref kvIoGetNumberOfPins.
 * \param[out] value A pointer to an unsigned int which receives the latest set value of the pin.
 *
 * \sa \ref kvIoPinSetDigital()
 * \note The actual value on the output pin may differ.
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoPinGetOutputDigital(hnd: canHandle; pin: cardinal; var value: cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoPinGetOutputDigital';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.kvStatus kvIoPinSetRelay(CanHandle handle, Int32 pin, Int32 value);</b>
 *
 * \source_delphi   <b>function kvIoPinSetRelay(hnd: canHandle; pin: cardinal; value: cardinal): canStatus;</b>
 * \source_end
 * This function is used to control a relay of the specified I/O pin.
 * If \a value is zero, the relay is set to OFF. For any non-zero \a value,
 * the relay is set to ON.
 *
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[in]  pin   The pin number, see \ref kvIoGetNumberOfPins.
 * \param[in]  value An unsigned int which sets a value of the pin.
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoPinSetRelay(hnd: canHandle; pin: cardinal; value: cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoPinSetRelay';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.kvStatus kvIoPinGetOutputRelay(CanHandle handle, Int32 pin, out Int32 value);</b>
 *
 * \source_delphi   <b>function kvIoPinGetOutputRelay(hnd: canHandle; pin: cardinal; var value: cardinal): canStatus;</b>
 * \source_end
 * This function is used to get the latest set value of a relay I/O pin.
 * If \a value is zero, the relay has been set to OFF. For any non-zero \a value,
 * the relay has been set to ON.
 * This function returns values as they are presented in memory and the actual state on the relay pin may differ.
 *
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[in]  pin   The pin number, see \ref kvIoGetNumberOfPins.
 * \param[in]  value A pointer to an unsigned int which receives the latest set value of the pin.
 *
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoPinGetOutputRelay(hnd: canHandle; pin: cardinal; var value: cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoPinGetOutputRelay';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.kvStatus kvIoPinSetAnalog(CanHandle handle, Int32 pin, float value);</b>
 *
 * \source_delphi   <b>function kvIoPinSetAnalog(hnd: canHandle; pin: cardinal; value: single): canStatus;</b>
 * \source_end
 * This function is used to set the voltage level of the specified analog I/O pin.
 *
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[in]  pin   The pin number, see \ref kvIoGetNumberOfPins.
 * \param[in]  value A float which sets a voltage of the pin.
 *
 * \sa \ref kvIoPinGetAnalog()
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoPinSetAnalog(hnd: canHandle; pin: cardinal; value: single): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoPinSetAnalog';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.kvStatus kvIoPinGetAnalog(CanHandle handle, Int32 pin, out float value);</b>
 *
 * \source_delphi   <b>function kvIoPinGetAnalog(hnd: canHandle; pin: cardinal; var value: single): canStatus;</b>
 * \source_end
 * This function is used to retrieve the voltage level of the specified analog I/O pin.
 *
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[in]  pin   The pin number, see \ref kvIoGetNumberOfPins.
 * \param[out] value A pointer to a float which receives the voltage of the pin.
 *
 * \sa \ref kvIoPinSetAnalog()
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoPinGetAnalog(hnd: canHandle; pin: cardinal; var value: single): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoPinGetAnalog';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.kvStatus kvIoPinGetOutputAnalog(CanHandle handle, Int32 pin, out float value);</b>
 *
 * \source_delphi   <b>function kvIoPinGetOutputAnalog(hnd: canHandle; pin: cardinal; var value: single): canStatus;</b>
 * \source_end
 * This function is used to get the latest set voltage level of an analog I/O pin.
 * This function only returns values as they are presented in memory and the actual value on the output pin may therefore differ.
 *
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[in]  pin   The pin number, see \ref kvIoGetNumberOfPins.
 * \param[out] value A pointer to a float which receives the latest set voltage level of the pin.
 *
 * \sa \ref kvIoPinSetAnalog()
 * \note The actual voltage level on the output pin may differ.
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoPinGetOutputAnalog(hnd: canHandle; pin: cardinal; var value: single): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoPinGetOutputAnalog';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.canStatus kvIoGetModulePins(int hnd, int module, out byte[] buffer, int bufsize);</b>
 *
 * \source_delphi   <b>function kvIoGetModulePins(hnd: canHandle; module: cardinal; var buffer; bufsize: cardinal): canStatus;</b>
 * \source_end
 *
 * This function is used to read all the pins on one module in a single call.
 *
 * \param[in]  hnd      An open handle to a CAN channel.
 * \param[in]  module   The module number, see \ref kvIO_INFO_GET_MODULE_NUMBER.
 * \param[out] buffer   A pointer to a struct that receives the pin values of the module. The struct can be any one of the following depending on the module. The returned type is described in the first byte of the returned struct. See:
 * - \ref kvIoModuleDigital
 * - \ref kvIoModuleAnalog
 * - \ref kvIoModuleRelay
 * \param[in]  bufsize  The size of the struct pointed to by buffer
 *
 * \sa \ref kvIoSetModulePins()
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoGetModulePins(hnd: canHandle; module: cardinal; var buffer; bufsize: cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoGetModulePins';

(**
 * \ingroup kv_io
 *
 * \source_cs       <b>static Canlib.canStatus kvIoSetModulePins(int hnd, int module, byte[] buffer, int bufsize);</b>
 *
 * \source_delphi   <b>function kvIoSetModulePins(hnd: canHandle; module: cardinal; var buffer; bufsize: cardinal): canStatus;</b>
 * \source_end
 *
 * This function is used to set all the pins on one single module in a single call.
 *
 * \param[in]  hnd      An open handle to a CAN channel.
 * \param[in]  module   The module number, see \ref kvIO_INFO_GET_MODULE_NUMBER.
 * \param[out] buffer   A pointer to a struct that contains the module type and pin values to set. The struct can be any one of the following depending on the module.
 * - \ref kvIoModuleDigital
 * - \ref kvIoModuleAnalog
 * - \ref kvIoModuleRelay
 * \param[in]  bufsize  The size of the struct pointed to by buffer
 *
 * \sa \ref kvIoGetModulePins()
 * \note Note The input Pins are ignored.
 * \note Preliminary API that may change.
 * \note Not implemented in Linux.
 *)
function kvIoSetModulePins(hnd: canHandle; module: cardinal; var buffer; bufsize: cardinal): canStatus; stdcall;
  external canlib32 name _PU + 'kvIoSetModulePins';

implementation

end.
