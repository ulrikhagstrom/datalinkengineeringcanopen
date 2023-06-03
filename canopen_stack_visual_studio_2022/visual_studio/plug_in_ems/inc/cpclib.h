/****************************************************************************
*                                                                           
*      Copyright (c) 2000, 2001, 2002-2005 by EMS Dr. Thomas Wuensche        
*                                                                           
*                  - All rights reserved -                                  
*                                                                           
* This code is provided "as is" without warranty of any kind, either        
* expressed or implied, including but not limited to the liability          
* concerning the freedom from material defects, the fitness for parti-      
* cular purposes or the freedom of proprietary rights of third parties.     
*                                                                           
*****************************************************************************
* Module name.: --                                                          
*****************************************************************************
* Include file: none                                                        
*****************************************************************************
* Project.....: Windows/Linux Driver Development Kit                              
* Filename....: cpclib.h                                                    
* Authors.....: Dr. Thomas Wuensche [TW]
*               Gerhard Uttenthaler [GU]
*               Christian Schoett   [CS]
*               Sebastian Haas      [SH]
*****************************************************************************
* Short descr.: Header file for CPC interfaces library                      
*****************************************************************************
* Description.: contains the function declarations for the CPC interfaces   
*               libary                                                      
*****************************************************************************
*                            History                                        
*****************************************************************************
* Version  Date        Author  Remark                                       
*                                                                           
* 01.71    02.08.2002  CS      - extracted the library function prototypes  
*                                from cpc.h into this file                  
* 01.72    24.03.2005  CS      - modified the compiler switch determining
*                                the running operating system
*                                _WIN32: a Windows OS is running
*                                else  : a Linux OS is running
*                              - added the macro to obtain the devices file
*                                descriptor for Linux OS
* 01.73    12.04.2005  GU      - Windows and Linux version of this file
*                                merged into one file
****************************************************************************/

#ifndef CPCLIB_HEADER
#define CPCLIB_HEADER

// check the operating system used
#ifdef _WIN32

	// on Windows OS we use a byte alignment of 1
  #pragma pack(push, 1)
  
  // set the calling conventions for the library function calls
	#define CALL_CONV __stdcall
  
#else

	// Linux does not use this calling convention
  #define CALL_CONV
  
	// this macro is needed to obtain the devices file descriptor
	#define CPC_GetFdByHandle(h)   ((CPC_GetInitParamsPtr((h)))->chanparams.fd)

  #ifndef CAN_MAX_DEVICE
  	#ifdef PLATFORM_GSMTEST
    	#define  CAN_MAX_DEVICE    6
	  #elif PLATFORM_ETHERCAN
  	  #define  CAN_MAX_DEVICE    1
	  #elif PLATFORM_INT1000
  	  #define  CAN_MAX_DEVICE    1
	  #else // PLATFORM_UNSPECIFIED
  	  #define  CAN_MAX_DEVICE    1
	  #endif
	#endif

	extern unsigned int CPCLIBverbose;

#endif


#ifdef	__cplusplus
extern"C"{
#endif	// __cplusplus

/* library related functions */
char*	CALL_CONV CPC_GetLibVersion	   (void);
	
/* interface and channel related functions */
int   CALL_CONV CPC_OpenChannel       (char *channel);
int   CALL_CONV CPC_CloseChannel      (int handle);
int   CALL_CONV CPC_CANInit           (int handle, unsigned char confirm);
int   CALL_CONV CPC_CANExit           (int handle, unsigned char confirm);

/* synchronous functions */
int   CALL_CONV CPC_GetCANState       (int handle);
char* CALL_CONV CPC_GetInfo           (int handle, unsigned char source, unsigned char type);

CPC_INIT_PARAMS_T* CALL_CONV CPC_GetInitParamsPtr(int handle);
int   CALL_CONV CPC_ClearMSGQueue     (int handle);
#define CPC_BufferClear               CPC_ClearMSGQueue

int   CALL_CONV CPC_ClearCMDQueue     (int handle, unsigned char confirm);

int   CALL_CONV CPC_GetMSGQueueCnt(int handle);
#define CPC_GetBufferCnt               CPC_GetMSGQueueCnt

int   CALL_CONV CPC_SendMsg           (int handle, unsigned char confirm, CPC_CAN_MSG_T* pCANMsg);
int   CALL_CONV CPC_SendXMsg          (int handle, unsigned char confirm, CPC_CAN_MSG_T* pCANMsg);
int   CALL_CONV CPC_SendRTR           (int handle, unsigned char confirm, CPC_CAN_MSG_T* pCANMsg);
int   CALL_CONV CPC_SendXRTR          (int handle, unsigned char confirm, CPC_CAN_MSG_T* pCANMsg);
int   CALL_CONV CPC_Control           (int handle, unsigned short value);
CPC_MSG_T* CALL_CONV CPC_WaitForMType (int handle, int mtype);

char * CALL_CONV CPC_DecodeErrorMsg(int error);

/* functions for the asynchronous interface */
int   CALL_CONV CPC_AddHandler        (int handle, void (CALL_CONV *handler)(int handle, const CPC_MSG_T* pCPCMsg));
int   CALL_CONV CPC_RemoveHandler     (int handle, void (CALL_CONV *handler)(int handle, const CPC_MSG_T* pCPCMsg));
int   CALL_CONV CPC_AddHandlerEx      (int handle, void (CALL_CONV *handlerEx)(int handle, const CPC_MSG_T* pCPCMsg, void *customPointer), void *customPointer);
int   CALL_CONV CPC_RemoveHandlerEx   (int handle, void (CALL_CONV *handlerEx)(int handle, const CPC_MSG_T* pCPCMsg, void *customPointer));

int	  CALL_CONV CPC_WaitForEvent      (int handle, int timeout, unsigned char event);

CPC_MSG_T* CALL_CONV CPC_Handle       (int handle);

int   CALL_CONV CPC_RequestCANParams  (int handle, unsigned char confirm);
int   CALL_CONV CPC_RequestCANState   (int handle, unsigned char confirm);
int   CALL_CONV CPC_RequestInfo       (int handle, unsigned char confirm, unsigned char source, unsigned char type);

/* currently not implemented */
CPC_CAN_PARAMS_T* CALL_CONV CPC_GetCANParams(int handle);
int   CALL_CONV CPC_GetBusload        (int handle);

int   CALL_CONV CPC_ReadMsg           (int handle, CPC_CAN_MSG_T* pCANMsg);
int   CALL_CONV CPC_Filter            (int handle, short, unsigned short, unsigned short);

#ifdef	__cplusplus
}
#endif	// __cplusplus

#ifdef _WIN32
	#pragma pack(pop)
#endif

#endif // CPCLIB_HEADER
