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
 * @brief     Definition file for log data types.
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for log data types.
 */

#pragma once

/* Project includes */
#include "include/BaseDefs.h"

/**
 * Log Trigger State.
 * Triggering means the state to which logging operation should transit.
 */
typedef enum LogTriggerState
{
    NONE    = 0,
    START   = 1,    /**< To start logging */
    STOP    = 2,    /**< To stop logging */
    BOTH    = 3,    /**< Shuttle */
    STOPPED = 4     /**< Logging already stopped */
} LogTriggerState;

/**
 * Triggering operation with all the necessary parameters.
 */
typedef struct tagLogTrigger
{
    LogTriggerState triggerType;

	/**
	 * Start Trigger ID
	 */
    UINT startId;

	/**
	 * Stop Trigger ID
	 */
    UINT stopId;
} SLOGTRIGGER, *PSLOGTRIGGER;

const USHORT ID_INVALID = (USHORT) -1;
const USHORT ID_MAX = (USHORT) ~0;

/**
 * The below structure describes a logging block fulfilling the condition of
 * necessity and sufficience.
 */
typedef struct tagLogInfo
{
    tagLogInfo();

	/**
	 * Logging block identifier
	 */
    USHORT m_ushID;

	/**
	 * To indicate if it needs updation
	 */
    BOOL m_bIsUpdated;

	/**
	 * To indicate if its enabled
	 */
    BOOL m_bEnabled;

	/**
	 * The time mode - system / relative
	 */
    eTimerMode m_eLogTimerMode;

	/**
	 * Numeric mode - hexadecimal / decimal
	 */
    eFormat m_eNumFormat;

	/**
	 * Mode - overwrite / append
	 */
    eMode m_eFileMode;

	/**
	 * To indicate if Absolute Time Stamp is Reseted
	 */
    BOOL m_bResetAbsTimeStamp;

	/**
	 * The current channel
	 */
    TYPE_CHANNEL m_ChannelSelected;

	/**
	 * Log file name with absolute path
	 */
    char m_sLogFileName[_MAX_PATH];

	/**
	 * The triggering condition
	 */
    SLOGTRIGGER m_sLogTrigger;

	/**
	 * To clear the logging information
	 */
    void vClear(void);

	/**
	 * To get the size of the block in bytes
	 */
    UINT unGetSize(void) const;

	/**
	 * To get configuration data
	 */
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

	/**
	 * To get configuration data
	 */
    BOOL pbGetConfigData(xmlNodePtr pxmlNodePtr) const;

	/**
	 * To set configuration data
	 */
    BYTE* pbSetConfigData(BYTE* pbTarget, BYTE bytLogVersion);

	INT nSetConfigData(xmlNodePtr pNode);
} SLOGINFO;
