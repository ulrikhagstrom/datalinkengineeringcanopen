#ifndef		_LowLevFunc_H
#define		_LowLevFunc_H

short int __stdcall ResetInterface(unsigned char InterfaceNum,unsigned char DeviceNum);
short int __stdcall SetCANBaud(unsigned char InterfaceNum,unsigned char DeviceNum,short int Baud);
short int __stdcall GetInfo(unsigned char InterfaceNum,unsigned char DeviceNum,unsigned char *Version,unsigned char *Feature
                           ,unsigned char *Manufact,unsigned char *CANCont);
short int __stdcall Reset(unsigned char InterfaceNum,unsigned char DeviceNum);
short int __stdcall SendCANMess(unsigned char InterfaceNum,unsigned char DeviceNum,long ID,
								unsigned char CB1,unsigned char CB2,unsigned char* databytes);
short int __stdcall GetCANMess(unsigned char InterfaceNum,unsigned char DeviceNum,unsigned char *buffer);

short int __stdcall SetLEDPattern(unsigned char InterfaceNum,unsigned char DeviceNum,
								  unsigned char Mode, unsigned char *Pattern);
short int USBRawCommand (unsigned char InterfaceNum,unsigned char *Outbuffer, unsigned char  NumBytes, unsigned char *Inbuffer);
short int OpenDriver(unsigned char InterfaceNum);
short int  __stdcall NewInterface(unsigned char InterfaceNum);
// USB protocol definitions
#define ACK 1
#define NACK 0
#define GETSTATSREQ  2           
#define GETSTATSRESP  3
#define INITREQ 4
#define INITRESP 5
#define SETBAUDREQ 6
#define SETBAUDRESP 7
#define SETFILTERREQ 8
#define SETFILTERRESP 9
#define SENDCANREQ 0x10
#define SENDCANRESP 0x11
#define RECCANREQ 0x12
#define RECCANRESP 0x13
#define GETCANMESREQ 0x14
#define GETCANMESRESP 0x15
#define READREGREQ 0x16
#define READREGRESP 0x17
#define WRITEREGREQ 0x18
#define WRITEREGRESP 0x19
// 0x1A is disable message object
#define GETNUMMESREQ 0x1C       // get the number of messages in queue but don't do anything with them
#define GETNUMMESRESP 0x1D
#define CLEARMESREQ 0x1E
#define CLEARMESRESP 0x1F
#define ENABLETSREQ 0x20
#define ENABLETSRESP 0x21
#define DISABLETSREQ 0x22
#define DISABLETSRESP 0x23
#define SETLEDPATREQ 0x24			// set user LED pattern request
#define SETLEDPATRESP 0x25			// set user LED pattern response

// The following are errors that may be returned by these functions
#define ERRNOINTERFACEPRESENT -1
#define ERRIMPROPERCALLINGPARAMETER -3
#define ERRIMPROPERRESPONSE -4
#define ERRBULKWRITEFAILED -7
#define ERRBULKREADFAILED -8
#define ERRLOADDRIVER -11

// The following are errors that are defined by any function used by the Zanthic 
// CAN interfaces
// -1 for generic fail (no board present)
// -2 for no response
// -3 for improper calling parameter
// -4 for improper response
// -5 if more than one unit responded
// -6 if system is busy at this time
// -7 for failed in bulk write
// -8 for failed in bulk read
// -9 for interface not responding
// -10 if interface config file not loaded properly
// -11=start driver routine failed
// -12=load driver routine failed
// -13=could not open device
// -14=could not open ISR device
// -15=could not open ISR thread
// -16=driver already loaded











#endif
