/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file      DIL_Datatypes.h
 * @brief     Defines DIL related data types
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines DIL related data types
 */

#pragma once

/* C++ includes */
#include <string>

/* Project includes */
#include "../Include/BaseDefs.h"

#define CAN_MONITOR_NODE "CAN_MONITOR"
const int CAN_MONITOR_NODE_INDEX = 0;
const int CAN_MONITOR_CLIENT_ID  = 1;

#define J1939_MONITOR_NODE "J1939_MONITOR"
const UINT64 J1939_ECU_NAME     = 0x8000000000000001;

const BYTE MSGBUF_ADD = 0x1;
const BYTE MSGBUF_CLEAR = 0x0;

enum ECONTR_STATUS : unsigned short
{
    RESET_STATE = 0,    // reset
    INITIALISED,        // stopped / initialized
    WAITING,            // started / waiting for startup completion
    NORMAL_ACTIVE,      // started / normal active (running)
    NORMAL_PASSIVE,     // started / normal passiv
    NOT_DEFINED,        // started / halt mode
};

enum ECONTR_PARAM
{
    NUMBER_HW = 0,
    NUMBER_CONNECTED_HW,
    PEAK_ERR_CNT,
    DRIVER_STATUS,
    ERR_CNT,
    HW_MODE,
    CNTR_STATUS,
    CON_TEST,
};

/**
 * Controller Status Message
 */
typedef struct s_STATUSMSG
{
    ECONTR_STATUS controllerStatus; /**< Current controller state */
} s_STATUSMSG;

enum
{
    DRIVER_CAN_STUB = 0,
    DRIVER_CAN_PEAK_USB,
    DRIVER_CAN_ICS_NEOVI,
    DRIVER_CAN_ETAS_BOA,
    DRIVER_CAN_ETAS_ES581,
    DRIVER_CAN_VECTOR_XL,
    DRIVER_CAN_KVASER_CAN,
    DRIVER_CAN_MHS,
    DRIVER_CAN_IXXAT,
    DRIVER_CAN_VSCOM,
    DIL_TOTAL,          // Its value must be <= MAX_DILS
    DAL_NONE            = ~0x0
};

#define  MAX_CHAR             1024

class InterfaceHardware
{
public:
    unsigned long  interfaceId;
    unsigned long  vendor;
    unsigned char  networkId;
	std::string    interfaceName;
	std::string    description;
	std::string    deviceName;

    InterfaceHardware()
    {
        interfaceId = 0;
        vendor = 0;
        networkId = 0;
        interfaceName = "";
        description = "";
        deviceName = "";
    }

    InterfaceHardware(const InterfaceHardware& objRef)
    {
        interfaceId = objRef.interfaceId;
        vendor = objRef.vendor;
        networkId = objRef.networkId;
        interfaceName = objRef.interfaceName;
        description = objRef.description;
        deviceName = objRef.deviceName;
    }

    InterfaceHardware& operator=(InterfaceHardware& objRef)
    {
        interfaceId = objRef.interfaceId;
        vendor = objRef.vendor;
        networkId = objRef.networkId;
        interfaceName = objRef.interfaceName;
        description = objRef.description;
        deviceName = objRef.deviceName;
        return *this;
    }
};

typedef InterfaceHardware InterfaceHardwareList[defNO_OF_CHANNELS];

class DILINFO
{
public:
	std::string    name;
    unsigned long  driverId;
    unsigned int   resourceId;

    DILINFO()
    {
        name = "";
        driverId = 0;
        resourceId = 0;
    }

    DILINFO(const DILINFO& objRef)
    {
        name = objRef.name;
        driverId = objRef.driverId;
        resourceId = objRef.resourceId;
    }

    DILINFO& operator=(DILINFO& objRef)
    {
        name = objRef.name;
        driverId = objRef.driverId;
        resourceId = objRef.resourceId;
        return *this;
    }
};

const int MAX_DILS = 16;
const int MAX_HW = 32;
typedef DILINFO DILLIST[MAX_DILS];
