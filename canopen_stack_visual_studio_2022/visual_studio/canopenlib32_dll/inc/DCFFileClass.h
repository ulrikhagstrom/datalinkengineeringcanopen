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

#include <stdio.h>
#include <string.h>

#include "CANopenLibHelper.h"

#ifndef DCF_FILE_CLASS_H
#define DCF_FILE_CLASS_H

#define FILE_NAME_LENGTH 200

typedef enum {
  READ_ONLY = 0,
  WRITE_ONLY = 1,
  READ_AND_WRITE = 2
} AccessType;

class DcfItem
{
public:
  int dataLen;
  u8 data[256];
  u16 objectIndex;
  u8 subIndex;
  int objectType;
  int dataType;
  int dataTypeToLen;
  AccessType accessType;
  int objFlags;
};


class DCFFile
{
public:
  DCFFile(char* file);
  DCFFile(int a);
  ~DCFFile();

  void GetObjectsToConfigureFromFile();
  DcfItem dcfItems[1000];
  int dcfItemsCount;

private:
  char fileName[FILE_NAME_LENGTH];
  DcfItem* getItemData(char* itemName);
  bool GetObjectsToConfigureFromFile(char* objectType);
};

#endif