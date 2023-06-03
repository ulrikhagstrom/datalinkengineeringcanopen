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

#include <windows.h>
#include <sys/stat.h>
#include "DCFFileClass.h"

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------

DCFFile :: DCFFile(char *file)
{
    memset(this->fileName, 0, FILE_NAME_LENGTH);
    memcpy(this->fileName, file, strnlen(file, FILE_NAME_LENGTH));
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------

DCFFile :: ~DCFFile()
{
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------

bool FileExists(char* filename) 
{
    struct stat fileInfo;
    return stat(filename, &fileInfo) == 0;
}

DcfItem * DCFFile :: getItemData(char *itemName)
{
    char tempData[256];
    DcfItem *dcfItem = new DcfItem();

    GetPrivateProfileString(itemName, "AccessType", "", tempData, 255, (LPCSTR)this->fileName);
    if (strcmp(tempData, "rw") == 0)
    {
        dcfItem->accessType = READ_AND_WRITE;
    }
    else if (strcmp(tempData, "ro") == 0)
    {
        dcfItem->accessType = READ_ONLY;
    }
    else
    {
        delete dcfItem;
        return NULL;
    }

    GetPrivateProfileString(itemName, "ObjectType", "", tempData, 255, (LPCSTR)this->fileName);
    if (strlen(tempData) > 0)
    {
        dcfItem->objectType = strtol(tempData, NULL, 0);
    }
    else
    {
        delete dcfItem;
        return NULL;
    }

    GetPrivateProfileString(itemName, "DefaultValue", "", tempData, 255, (LPCSTR)this->fileName);

    int defaultValueStrLen = strlen(tempData);
    if (defaultValueStrLen > 0)
    {
        strcpy((char *)dcfItem->data, tempData);
        dcfItem->dataLen = defaultValueStrLen;
    }
    else
    {
        dcfItem->dataLen = 0;
    }

    GetPrivateProfileString(itemName, "DataType", "", tempData, 255, (LPCSTR)this->fileName);
    if (strlen(tempData) > 0)
    {
        dcfItem->dataType = strtol(tempData, NULL, 0);
        switch (dcfItem->dataType)
        {
            case 2:
            case 5:
                dcfItem->dataTypeToLen = 1;
                break;
            case 3:
            case 6:
                dcfItem->dataTypeToLen = 2;
                break;
            case 4:
            case 7:
                dcfItem->dataTypeToLen = 4;
                break;
            default:
                dcfItem->dataTypeToLen = dcfItem->dataLen;
        }

    }
    else
    {
        delete dcfItem;
        return NULL;
    }



    GetPrivateProfileString(itemName, "ObjFlags", "", tempData, 255, (LPCSTR)this->fileName);

    if (strlen(tempData) > 0)
    {
        dcfItem->objFlags = strtol(tempData, NULL, 0);
    }
    else
    {
        dcfItem->objFlags = 0;
    }

    return dcfItem;
}


bool DCFFile :: GetObjectsToConfigureFromFile(char *objectType)
{
    char pResult[256];

    if (FileExists(this->fileName))
        GetPrivateProfileString(objectType, "SupportedObjects", "0", pResult, 255, (LPCSTR)this->fileName);
    else
        return false;

    int supportedObjects = atoi(pResult);

    char objectIndexHexStr[256];
    char objectIndexCountStr[256];

    for (int i = 1; i <= supportedObjects;  i++)
    {
        sprintf(objectIndexCountStr, "%d", i);

        GetPrivateProfileString(objectType, objectIndexCountStr, "", objectIndexHexStr, 255, (LPCSTR)this->fileName);

        char noSubIndexes[10];

        GetPrivateProfileString(&objectIndexHexStr[2], "SubNumber", "0", noSubIndexes, 255, (LPCSTR)this->fileName);

        int subIndexCount = atoi(noSubIndexes);

        char defaultValue[10];
        if (subIndexCount == 0)
        {
            DcfItem *t = getItemData(&objectIndexHexStr[2]);
            
            if (t != NULL)
            {
                t->objectIndex = (u16)strtol(objectIndexHexStr, NULL, 0);
                t->subIndex = (u8)0;
                memcpy(&this->dcfItems[this->dcfItemsCount++], t, sizeof(DcfItem));
                delete t;
            }
        }
        else
        {
            for (int j = 0; j < subIndexCount; j++)
            {
                char objectSubIndex[10];
                sprintf(objectSubIndex, "%ssub%X", &objectIndexHexStr[2], j);

                DcfItem *t = getItemData(objectSubIndex);
                if (t != NULL)
                {
                    t->objectIndex = (u16)strtol(objectIndexHexStr, NULL, 0);
                    t->subIndex = (u8)j;
                    memcpy(&this->dcfItems[this->dcfItemsCount++], t, sizeof(DcfItem));
                    delete t;
                }
            }
        }
    }

    return true;
}

void DCFFile :: GetObjectsToConfigureFromFile()
{
    memset(dcfItems, 0, sizeof(dcfItems));
    dcfItemsCount = 0;

    GetObjectsToConfigureFromFile("MandatoryObjects");
    GetObjectsToConfigureFromFile("OptionalObjects");
}

