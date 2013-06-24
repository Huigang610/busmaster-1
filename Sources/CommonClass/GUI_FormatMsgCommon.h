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
 * @brief     Definition of CFormatMsgCommon class
 * @author    Anish kumar
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CFormatMsgCommon class
 */

#pragma once

/* Project includes */
#include "CommonClass/RefTimeKeeper.h"

class CFormatMsgCommon: public CRefTimeKeeper
{
public:
    CFormatMsgCommon(void);

	/**
	 * Format time details
	 *
	 * @param[in] bExprnFlag
	 *   Details of time mode
	 * @param[in] TimeStamp
	 *   Msg time stamp, Rel time in case of Rel. mode
	 * @param[in] acTime 
	 *   Buffer to store formatted time
	 */
	void vCalculateAndFormatTM(BYTE bExprnFlag, UINT64 TimeStamp,char acTime[]);

	void vCalAndFormatTM_Offline(BYTE bExprnFlag,  UINT64 TimeStamp, char acTime[]);

protected:
	/**
	 * @brief      Format Time Stamp
	 * @param[in]  dwTimeStamp time stamp to be formatted
	 * @param[out] acTime Buffer to store formatted time
	 *
	 * Format Time Stamp
	 */
	void vFormatTimeStamp(DWORD dwTimeStamp, char acTime[]);
};
