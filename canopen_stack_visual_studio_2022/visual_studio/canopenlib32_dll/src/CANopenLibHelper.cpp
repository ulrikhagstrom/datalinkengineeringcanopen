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

#include "CANopenLibHelper.h"
#include <memory.h>
#include <stdio.h>
#include <Windows.h>

canAdapter selectedAdapter = canAdapter_DEFAULT; // Global, this is ulgy, I think.

void CANOPENDLL_API setAdapter(canAdapter userAdapter)
{
  selectedAdapter = userAdapter;
}

canAdapter CANOPENDLL_API getAdapter()
{
  return selectedAdapter;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
#ifdef HAS_LONG_LONG
void val2buf(u64 val, u8* buf, u8 len) {
  while (len) {
    *buf = (u8)(val & 0xff);
    val >>= 8;
    buf++;
    len--;
  }
}
#endif

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void val2buf(u32 val, u8* buf, u8 len) {
  while (len) {
    *buf = (u8)(val & 0xff);
    val >>= 8;
    buf++;
    len--;
  }
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
u32 buf2val(u8* buf, u8 len) {
  u32 val = 0;
  buf += len; /* Points to MSB+1 (we use predecrement or CodeGuard will complain on buffer underrun) */
  while (len--)
    val = (val << 8) + *--buf;
  return val;
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
u16 getU16Val(u8* p, u8 offset)
{
  return (u16)(buf2val((p + offset), 2));
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
u32 getU32Val(u8* p, u8 offset)
{
  return (u32)(buf2val((p + offset), 4));
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
#ifdef HAS_LONG_LONG
u64 getU64Val(u8* p, u8 offset)
{
  return (u64)(buf2val((p + offset), 8));
}
#endif

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void setU16Val(u16 val, u8* p, u8 offset)
{
  val2buf((u32)val, p + offset, 2);
  return;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void setU32Val(u32 val, u8* p, u8 offset)
{
  val2buf(val, p + offset, 4);
  return;
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
#ifdef HAS_LONG_LONG
void setU64Val(u64 val, u8* p, u8 offset)
{
  val2buf(val, p + offset, 8);
  return;
}
#endif

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
u8 getU8Val(u8* p, u8 offset)
{
  return (u8)(buf2val((p + offset), 1));
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void setU8Val(u8 val, u8* p, u8 offset)
{
  val2buf((u32)val, (p + offset), 1);
  return;
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
u32 getU24Val(u8* p, u8 offset)
{
  return (u32)(buf2val((p + offset), 3));
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void setU24Val(u32 val, u8* p, u8 offset)
{
  val2buf(val, (p + offset), 3);
  return;
}


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void DebugLogToFile(char* string)
{
#ifdef WRITE_TO_LOGFILE
  FILE* out;

  if ((out = fopen("canopenlib_logfile.txt", "a")) == NULL) {
    MessageBox(NULL,
      "Can not open 'c:\canopenlib_logfile.txt' for writing!",
      "CAN NOT OPEN LOGFILE", MB_OK);
  }

  fprintf(out, "%s", string);
  fclose(out);
#endif
}

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
canOpenStatus DebugExitErrorValueLogToFile(canOpenStatus ret)
{
#ifdef WRITE_TO_LOGFILE
  FILE* out;

  if ((out = fopen("canopenlib_logfile.txt", "a")) == NULL) {
    MessageBox(NULL,
      "Can not open 'c:\canopenlib_logfile.txt' for writing!",
      "CAN NOT OPEN LOGFILE", MB_OK);
  }

  fprintf(out, "Exit with code: %d\n", ret);
  fclose(out);
#endif
  return ret;
}