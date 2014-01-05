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
    *             Copyright (C) 2009-2010 Ulrik Hagström.             *
    *******************************************************************
    
    CANopen API (C++/C#) is available under a dual license: 
    
    1. GNU General Public License:
    ------------------------------

    Copyright (C) 2009-2010 Ulrik Hagström.

    This program is free software; you can redistribute it and/or modify 
    it under the terms of the GNU General Public License as published by 
    the Free Software Foundation; either version 3 of the License, 
    or (at your option) any later version.

    This program is distributed in the hope that it will be useful, 
    but WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
    See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License 
    along with this program; if not, see <http://www.gnu.org/licenses>.

    Additional permission under GNU GPL version 3 section 7:

    If you modify this Program, or any covered work, by linking or combining 
    it with 'CAN-dispatcher Library' (can_dispatcher.lib) (or a modified 
    version of that library), containing parts covered by the terms of 
    "CANopen API (C++/C#) Commercial License", the licensors 
    of this Program grant you additional permission to convey the resulting 
    work. {Corresponding Source for a non-source form of such a combination 
    shall include the source code for the parts of 'CAN-dispatcher Library' 
    used as well as that of the covered work.}


    2. CANopen API (C++/C#) Commercial License:
    -------------------------------------------

    This license allows for the use of CANopen API (C++/C#) 
    in closed-source projects. This license is available for a very reasonable 
    fee and also includes technical support. For details, please read the 
    CANopen API (C++/C#) Commercial License (see license.txt).
*/

#include "CANopenLibDefines.h"

#ifndef CANOPENLIB_HW_DYN_LOAD_H
#define CANOPENLIB_HW_DYN_LOAD_H

#ifdef CANLIB32_EXPORTS
#define CANOPENLIB_HW_API  /*extern "C"*/ __declspec(dllexport) 
#else
#define CANOPENLIB_HW_API __declspec(dllimport)
#endif

/***************
*
*   CANopen-lib hardware port plugin DLL (canopenlib_hw.dll).
*
****************/

typedef int      canPortHandle;




#endif


