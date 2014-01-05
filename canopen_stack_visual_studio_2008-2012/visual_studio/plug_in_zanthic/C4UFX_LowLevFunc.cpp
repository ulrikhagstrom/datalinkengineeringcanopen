// This file contains all of the low level routines required to access
// the CAN-4-USB-FX interface.  This file will be used by the sample
// programs written in C as well as the DLL's used by VB

// Written by Steven D. Letkeman BSc, Zanthic Technologies Inc.
// With code taken from the EZ-USB examples

// Version 1.3
// see end of file for revision history


#include "stdafx.h"
#include <winioctl.h>
#include "CyAPI.h"			// this is new for the FX model

CCyUSBDevice *USBDevice[10];
unsigned char DriverEnabled[10]={0,0,0,0,0,0,0,0,0,0};  // flag devices if opened

// ****************************************************************************
short int __stdcall SetCANBaud(unsigned char InterfaceNum,unsigned char DeviceNum,short int Baud)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
// Baud=10,20,50,100,125,250,500,800,1000 in kbps
{
unsigned char USBOutBuf[4];
unsigned char USBInBuf[65];
short int Result;
unsigned char BVal;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

switch (Baud)
 {
   case 10  : BVal=0; break;
   case 20  : BVal=1; break;
   case 50  : BVal=2; break;
   case 100 : BVal=3; break;
   case 125 : BVal=4; break;
   case 250 : BVal=5; break;
   case 500 : BVal=6; break;
   case 800 : BVal=7; break;
   case 1000: BVal=8; break;
   default:return(ERRIMPROPERCALLINGPARAMETER);break;
 }

USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = SETBAUDREQ;					// command
USBOutBuf[2] = BVal;

Result = USBRawCommand(InterfaceNum, USBOutBuf, 3, USBInBuf);

if (Result<0) return(Result);				// return error
if (USBInBuf[0]!=SETBAUDRESP)		 
  return (ERRIMPROPERRESPONSE);				//improper response from node
else
  return(ACK);
}

// ****************************************************************************
short int __stdcall GetInfo(unsigned char InterfaceNum,unsigned char DeviceNum,unsigned char *Version,unsigned char *Feature
                           ,unsigned char *Manufact,unsigned char *CANCont)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
// Verson will get filled with two bytes for major and minor version
// Feature is an 8 bit flag byte to show which features are availble
// Manufact will be a string up to 20 characters of the board manufacturer name
// CANCont is a byte array with the first character containing the number of CAN
// controllers within this board.  The next bytes are a value to show what type of 
// CAN controller this is 
{
unsigned char USBOutBuf[4];
unsigned char USBInBuf[65];
short int Result,pntr=4;
unsigned char c;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = GETSTATSREQ;  // command

Result = USBRawCommand(InterfaceNum, USBOutBuf, 2, USBInBuf);
if (Result<0) return(Result); // return error
if (USBInBuf[0]==GETSTATSRESP) 
 {
 *Version++=USBInBuf[1];  
 *Version++=USBInBuf[2];  
 *Feature=USBInBuf[3];
 while(USBInBuf[pntr]!=0)
  *Manufact++=USBInBuf[pntr++]; // load name till 0

 *Manufact++=USBInBuf[pntr++]; // load 0
 
 c=USBInBuf[pntr++];			// number of controllers
 *CANCont++=c;					// save to buffer
 while (c--)					// loop through the number of controllers
  *CANCont++=USBInBuf[pntr++];	// save their type to the buffer		

 return(ACK);
 }
else
 return (ERRIMPROPERRESPONSE); 
}

// ****************************************************************************
short int __stdcall ResetInterface(unsigned char InterfaceNum,unsigned char DeviceNum)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
// This function will execute the internal reset command on the CAN controller
// This function used to be called Reset for versions before the FX
{
unsigned char USBOutBuf[3];
unsigned char USBInBuf[65];
short int Result;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = INITREQ;  // command

Result = USBRawCommand(InterfaceNum, USBOutBuf, 2, USBInBuf);
if (Result<0) return(Result); // return error
if (USBInBuf[0]==INITRESP) 
 return(ACK);
else
 return (ERRIMPROPERRESPONSE); //improper response from node
}

// ****************************************************************************
short int __stdcall SendCANMess(unsigned char InterfaceNum,unsigned char DeviceNum,long ID,
								unsigned char CB1,unsigned char CB2,unsigned char* databytes)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
// ID is the CAN ID, right justified with bit 31=1 for extended, 30=1 for RTR
// CB1=Command Byte 1. Bit 2=Txwait bit, bit 1&0 are priority
// CB2=Command Byte 2. Upper 4 bits = Message object, Lower 4 bits=Number of data bytes
// databytes is a pointer to type unsigned char of 0-8 data bytes of data
{
unsigned char USBOutBuf[16];
unsigned char USBInBuf[65];
unsigned char c,numbytes;
short int Result;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = SENDCANREQ;  // command
USBOutBuf[2] = (unsigned char)(ID>>24);
USBOutBuf[3] = (unsigned char)(ID>>16);
USBOutBuf[4] = (unsigned char)(ID>>8);
USBOutBuf[5] = (unsigned char)ID;
USBOutBuf[6]=CB1;
USBOutBuf[7]=CB2;
numbytes=CB2&0x0F; // get number of bytes
if (numbytes>8) return(ERRIMPROPERCALLINGPARAMETER);

for (c=0;c<numbytes;c++)  
  USBOutBuf[8+c]=*(databytes+c);

Result = USBRawCommand(InterfaceNum, USBOutBuf, numbytes+8, USBInBuf);
if (Result<0) return(Result); // return error
if (USBInBuf[0]!=SENDCANRESP) return (ERRIMPROPERRESPONSE); //improper response from node
if ((USBInBuf[0]==SENDCANRESP)&&(USBInBuf[1]==ACK)) return(1); // success
// else an error occurred
*databytes++=USBInBuf[2]; // first error code
*databytes++=USBInBuf[3]; // second error code

return(NACK);
}


// ****************************************************************************
short int __stdcall ReadReg(unsigned char InterfaceNum,unsigned char DeviceNum,unsigned char Address,
                            unsigned char Numbytes,unsigned char *buffer)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
// Address is the start address within the CAN controller(0-255)
// Numbytes is the number of bytes to read (1-60)
// buffer is the user buffer that will get the values written to
{
unsigned char USBOutBuf[4];
unsigned char USBInBuf[65];
short int Result,c;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

if (Numbytes>60) return(ERRIMPROPERCALLINGPARAMETER);  // too many

USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = READREGREQ;  // command
USBOutBuf[2] = Address;
USBOutBuf[3] = Numbytes;

Result = USBRawCommand(InterfaceNum, USBOutBuf, 4, USBInBuf);
if (Result<0) return(Result); // return error
if (USBInBuf[0]==READREGRESP) 
 {
 if (USBInBuf[1]==ACK)
  {
  for (c=2;c<(2+Numbytes);c++)
   *buffer++=USBInBuf[c]; // load up data
  return(Numbytes);
  }
 return(NACK);
 }
else return (ERRIMPROPERRESPONSE); //improper response from node
}

// ****************************************************************************
short int __stdcall WriteReg(unsigned char InterfaceNum,unsigned char DeviceNum,unsigned char Address,
                             unsigned char Numbytes,unsigned char *buffer)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
// Address is the start address within the CAN controller(0-255)
// Numbytes is the number of bytes to write (1-60)
// buffer is the user buffer that contains the values to be written 
{
unsigned char USBOutBuf[65];
unsigned char USBInBuf[65];
short int Result,c;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

if (Numbytes>60) return(ERRIMPROPERCALLINGPARAMETER);  // too many
USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = WRITEREGREQ;  // command
USBOutBuf[2] = Address;
USBOutBuf[3] = Numbytes;
for (c=0;c<Numbytes;c++)
 USBOutBuf[4+c]=*buffer++;

Result = USBRawCommand(InterfaceNum, USBOutBuf, 4+Numbytes, USBInBuf);
if (Result<0) return(Result); // return error
if (USBInBuf[0]==WRITEREGRESP) 
 {
 if (USBInBuf[1]==ACK) return(ACK);
 return(NACK);
 }
return (ERRIMPROPERRESPONSE); //improper response from node
}

// ****************************************************************************
short int __stdcall EnableTS(unsigned char InterfaceNum,unsigned char DeviceNum)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
{
unsigned char USBOutBuf[65];
unsigned char USBInBuf[65];
short int Result;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = ENABLETSREQ;  // command

Result = USBRawCommand(InterfaceNum, USBOutBuf, 2, USBInBuf);
if (Result<0) return(Result); // return error
if (USBInBuf[0]==ENABLETSRESP) 
 {
 if (USBInBuf[1]==ACK) return(ACK);
 return(NACK);
 }
return (ERRIMPROPERRESPONSE); //improper response from node
}

// ****************************************************************************
short int __stdcall DisableTS(unsigned char InterfaceNum,unsigned char DeviceNum)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
{
unsigned char USBOutBuf[65];
unsigned char USBInBuf[65];
short int Result;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = DISABLETSREQ;  // command

Result = USBRawCommand(InterfaceNum, USBOutBuf, 2, USBInBuf);
if (Result<0) return(Result); // return error
if (USBInBuf[0]==DISABLETSRESP) 
 {
 if (USBInBuf[1]==ACK) return(ACK);
 return(NACK);
 }
return (ERRIMPROPERRESPONSE); //improper response from node
}

// ****************************************************************************
short int __stdcall RecCANMess(unsigned char InterfaceNum,unsigned char DeviceNum,long ID,unsigned char CB1)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
// ID is the CAN ID, right justified with bit 31=1 for extended
// CB1=Command Byte 1. upper 4 bits = Message object to use
{
unsigned char USBOutBuf[8];
unsigned char USBInBuf[65];
short int Result;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = RECCANREQ;  // command
USBOutBuf[2] = (unsigned char)(ID>>24);
USBOutBuf[3] = (unsigned char)(ID>>16);
USBOutBuf[4] = (unsigned char)(ID>>8);
USBOutBuf[5] = (unsigned char)ID;
USBOutBuf[6] = CB1;

Result = USBRawCommand(InterfaceNum, USBOutBuf, 7, USBInBuf);
if (Result<0) return(Result); // return error
if (USBInBuf[0]!=RECCANRESP) return (ERRIMPROPERRESPONSE); //improper response from node

return(ACK);
}

// ****************************************************************************
short int __stdcall GetCANMess(unsigned char InterfaceNum,unsigned char DeviceNum,unsigned char *buffer)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one

// The packet that is returned from the interface is as follows
// Byte 0=Command Response
// Byte 1=Ack or Nack (no further data)
// Byte 2=# of messages in this packet (always 1 from this revision on)
// Byte 3=number of bytes in this record including this byte (min=7, max=16+4(timestamping))
// Byte 4=Control 1 (upper 4 bits is MO, lower four bits are data length)
// Byte 5=Control 2 (status bits IDE,RTR,Reserved,SRR,RXRTR,Filt2,Filt1,Filt0)
// Byte 6=ID1
// Byte 7=ID2
// Byte 8=ID3
// Byte 9=ID4
// Byte 10...=data as needed (0-8 bytes)
// optional 3 byte timestamp

// The packet returned to the user is the same, minus the first three bytes, so
// Byte 0=number of bytes in this record including this byte (min=7, max=16+4(timestamping))
// Byte 1=Control 1 (upper 4 bits is MO, lower four bits are data length)
// Byte 2=Control 2 (status bits IDE,RTR,Reserved,SRR,RXRTR,Filt2,Filt1,Filt0)
// Byte 3=ID1
// Byte 4=ID2
// Byte 5=ID3
// Byte 6=ID4
// Byte 7...=data as needed (0-8 bytes)
// optional 3 byte timestamp

// You can tell if there is a timestamp if 
// Byte 0 != (data length+8)  
{
unsigned char USBOutBuf[3];
unsigned char USBInBuf[65];
short int Result,c;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

 USBOutBuf[0] = DeviceNum;
 USBOutBuf[1] = GETCANMESREQ;  // command
  
 Result = USBRawCommand(InterfaceNum, USBOutBuf, 2, USBInBuf);
 if (Result<0) return(Result); // return error

 if (USBInBuf[0]!=GETCANMESRESP) return (ERRIMPROPERRESPONSE); //improper response from node
 if (USBInBuf[1]==0) return(0); // no new messages (fix for proper form!)

 for (c=0;c<USBInBuf[3];c++)	// 3rd byte holds the number of bytes in packet
 *buffer++=USBInBuf[c+3];		// save to third location because first 3 have already been looked at

return(1);
}

// ****************************************************************************
short int __stdcall GetNumCANMess(unsigned char InterfaceNum,unsigned char DeviceNum)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
{
unsigned char USBOutBuf[65];
unsigned char USBInBuf[65];
short int Result;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = GETNUMMESREQ;  // command

Result = USBRawCommand(InterfaceNum, USBOutBuf, 2, USBInBuf);
if (Result<0) return(Result); // return error
if (USBInBuf[0]==GETNUMMESRESP) 
 return((USBInBuf[1]*0x100)+USBInBuf[2]);
else
 return (ERRIMPROPERRESPONSE); //improper response from node
}

// ****************************************************************************
short int __stdcall ClearCANMess(unsigned char InterfaceNum,unsigned char DeviceNum)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
// Clear the receive buffer
{
unsigned char USBOutBuf[3];
unsigned char USBInBuf[65];
short int Result;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = CLEARMESREQ;  // command

Result = USBRawCommand(InterfaceNum, USBOutBuf, 2, USBInBuf);
if (Result<0) return(Result); // return error
if (USBInBuf[0]==CLEARMESRESP) 
 return(1);
else
 return (ERRIMPROPERRESPONSE); //improper response from node
}


// ****************************************************************************
short int __stdcall SetFilters(unsigned char InterfaceNum,unsigned char DeviceNum,
                               unsigned char FilterNum,   long FilterVal)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
// FilterNum is a value from 0-5 for the MCP2510
// FilterVal is a 4 byte long with the filter values
// The MSB filter value will determine if this is a 29 bit ID
// if not set then it is 11 bit ID and only the least sig 11 bits are used
{
unsigned char USBOutBuf[7];
unsigned char USBInBuf[65];
short int Result,mask=1;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

if (FilterNum>5) return(ERRIMPROPERCALLINGPARAMETER); //out of range

USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = SETFILTERREQ;  // command

USBOutBuf[2]=mask<<FilterNum; //0=1,1=2,2=4 etc
USBOutBuf[3]=(unsigned char)(FilterVal>>24);
USBOutBuf[4]=(unsigned char)(FilterVal>>16);
USBOutBuf[5]=(unsigned char)(FilterVal>>8);
USBOutBuf[6]=(unsigned char)FilterVal;

Result = USBRawCommand(InterfaceNum, USBOutBuf, 7, USBInBuf);
if (Result<0) return(Result); // return error
if (USBInBuf[0]==SETFILTERRESP) 
 return(1);
else
 return (ERRIMPROPERRESPONSE); //improper response from node
}


// ****************************************************************************
short int __stdcall SetLEDPattern(unsigned char InterfaceNum,unsigned char DeviceNum,
								  unsigned char Mode, unsigned char *Pattern)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// DeviceNum is which CAN controller within an interface if more than one
// Mode is one of three choices
// 0 = normal mode, this is the default for the interface
// 1 = user pattern will be displayed until (if) the interface wants to display something and then
//	    the user pattern is over written and will not return to the user mode until the error is cleared
// 2 = user pattern is displayed and will not change unless the user calls this function again
// Pattern is a 20 byte buffer containing the pattern to display, made up of off(0), red(1) or green(2) values
// each value is displayed for 250ms so the entire pattern repeats every 5 seconds
{
unsigned char USBOutBuf[24];
unsigned char USBInBuf[65];
short int Result,c;

Result=OpenDriver(InterfaceNum);
if (Result<=0) return (Result);

USBOutBuf[0] = DeviceNum;
USBOutBuf[1] = SETLEDPATREQ;  // command
USBOutBuf[2] = Mode;
for (c=0;c<20;c++)
 USBOutBuf[3+c] = *Pattern++;

Result = USBRawCommand(InterfaceNum, USBOutBuf, 23, USBInBuf);

if (Result<0) return(Result); // return error
if (USBInBuf[0]==SETLEDPATRESP) 
 {
 if (USBInBuf[1]==ACK) return(ACK);
 return(NACK);
 }
return (ERRIMPROPERRESPONSE); //improper response from node

}


// The following are internal functions called by the above external functions


// ****************************************************************************
short int USBRawCommand (unsigned char InterfaceNum,unsigned char *USBOutBuf, unsigned char  SendNumBytes, unsigned char *USBInBuf)
// USBOutBuf contains the packet being sent to the USB device
// numbytes is the number of bytes to send
// USBInBuf is the returned packet from USB device
{
long SNumB=SendNumBytes;
long RecNumBytes=100;    // this is important, it must be more than what you are expecting back
						//otherwise zero bytes will be returned
unsigned char TRecBuf[256];
int c;

USBDevice[InterfaceNum]->BulkOutEndPt->XferData(USBOutBuf, SNumB); 

USBDevice[InterfaceNum]->BulkInEndPt->XferData(TRecBuf, RecNumBytes); 

// copy the data to the user data pointer
for (c=0;c<RecNumBytes;c++) *USBInBuf++=TRecBuf[c];



return((short int)RecNumBytes);  
}

// ****************************************************************************
short int OpenDriver(unsigned char InterfaceNum)
// Interface Number 0-9 for for than one interface connected (CAN-4-USB only)
// Open the USB driver for this interface if possible
// New for the FX
{
 if (InterfaceNum>9) return (ERRIMPROPERCALLINGPARAMETER);

 if (!DriverEnabled[InterfaceNum])
  {
   USBDevice[InterfaceNum] = new CCyUSBDevice(NULL);   // Create an instance of CCyUSBDevice

   if (USBDevice[InterfaceNum]->Open(InterfaceNum))
     DriverEnabled[InterfaceNum]=1;
   else
    return(ERRLOADDRIVER);
  }
 return(1);
}

// ****************************************************************************
short int  __stdcall NewInterface(unsigned char InterfaceNum)
// Because the above function OpenDriver only runs once when the DLL is
// first accessed, if unplug the interface and then replug it in without
// stopping the interface, the DLL no longer works as the new interface
// requires a new instance.  This function can be called to get a new
// instance if the interface is replaced
// New for the FX
{
 if (InterfaceNum>9) return (ERRIMPROPERCALLINGPARAMETER);

 USBDevice[InterfaceNum] = new CCyUSBDevice(NULL);   // Create an instance of CCyUSBDevice
 if (USBDevice[InterfaceNum]->Open(InterfaceNum))
  DriverEnabled[InterfaceNum]=1;	// should already be 1
 else
  return(ERRLOADDRIVER);

return(1);
}


// ################################################################
// Revision History
// Jan 19, 2006 Version 1.0 released
// May 19, 2006 Version 1.1 switched to cyusb.sys and cyAPI.sys
// Feb 16, 2007 Version 1.2 added NewInterface function
// Nov 2, 2009  Version 1.3 for new CAN-4-USB/FX Guid in inf file for 64 bit driver. updated cyapi.h and .lib
//                             see line 88 in cyapi.h







