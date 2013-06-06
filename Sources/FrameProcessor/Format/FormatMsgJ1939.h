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
 * @file      FormatMsgJ1939.h
 * @author    Anish Kumar
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

/* Project includes */
#include "Datatypes/J1939_Datatypes.h"
#include "FormatMsgCommon.h"

class CFormatMsgJ1939: public CFormatMsgCommon
{
public:
    void vInit();

	void vFormatJ1939DataMsg(STJ1939_MSG* pMsgMcANet,
                             SFORMATTEDATA_J1939* CurrDataJ1939,
                             BYTE bExprnFlag_Log);

private:
    void vFormatTime(BYTE bExprnFlag, PSTJ1939_MSG psJ1939BData,
                     PSFORMATTEDATA_J1939 psJ1939FData);
    
	void vFormatDataAndId(BYTE bExprnFlag, PSTJ1939_MSG psJ1939BData,
                          PSFORMATTEDATA_J1939 psJ1939FData);
};
