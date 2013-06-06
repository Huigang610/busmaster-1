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
 * @file      DIL_Interface/DILC_Dummy.cpp
 * @brief     Source file for dummy DIL functions
 * @authors   Ratnadip Choudhury, Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for dummy DIL functions
 */

/* Project includes */
#include "DIL_Interface_stdafx.h"
#include "DILC_Dummy.h"
#include "include/Error.h"
#include "include/basedefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DataTypes/MsgBufAll_DataTypes.h"

HRESULT CDIL_CAN_DUMMY::performInitOperations(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::performClosureOperations(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::getTimeModeMapping(SYSTEMTIME& /*CurrSysTime*/, UINT64& /*TimeStamp*/, LARGE_INTEGER* /*QueryTickCount*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::listHardwareInterfaces(InterfaceHardwareList& /*asSelHwInterface*/, INT& /*nCount*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::selectHardwareInterface(const InterfaceHardwareList& /*asSelHwInterface*/, INT /*nCount*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::deselectHardwareInterface(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::displayConfigurationDialog(PSCONTROLLER_DETAILS /*InitData*/, int& /*Length*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::setConfigurationData(PSCONTROLLER_DETAILS /*pInitData*/, int /*Length*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::startHardware(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::stopHardware(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::getCurrentStatus(s_STATUSMSG& /*StatusData*/)
{
    return WARN_DUMMY_API;
}



HRESULT CDIL_CAN_DUMMY::sendMessage(DWORD /*dwClientID*/, const STCAN_MSG& /*sCanTxMsg*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::manageMessageBuffer(BYTE /*bAction*/, DWORD /*ClientID*/, CBaseCANBufFSE* /*pBufObj*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::registerClient(BOOL /*bRegister*/, DWORD& /*ClientID*/, char* /*pacClientName*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::getControllerParameters(LONG& /*lParam*/, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::setControllerParameters(int nValue, ECONTR_PARAM eContrparam)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::setApplicationParameters(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::getErrorCount(SERROR_CNT& /*sErrorCnt*/, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::loadDriverLibrary(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::unloadDriverLibrary(void)
{
    return WARN_DUMMY_API;
}
