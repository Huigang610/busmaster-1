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
 * @file      DIL_Interface/DILC_Dummy.h
 * @brief     Declaration file for dummy DIL functions
 * @authors   Ratnadip Choudhury, Pradeep Kadoor, Arunkumar Karri
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Declaration file for dummy DIL functions
 */

/* Project includes */
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_DataTypes.h"
#include "BaseDIL_CAN_Controller.h"

class CDIL_CAN_DUMMY : public CBaseDIL_CAN_Controller
{
public:
    /* Overloaded functions */
    HRESULT performInitOperations(void);
    HRESULT performClosureOperations(void);
    HRESULT getTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL);
    HRESULT listHardwareInterfaces(InterfaceHardwareList& sSelHwInterface, INT& nCount);
    HRESULT selectHardwareInterface(const InterfaceHardwareList& sSelHwInterface, INT nCount);
    HRESULT deselectHardwareInterface(void);
    HRESULT displayConfigurationDialog(PSCONTROLLER_DETAILS InitData, int& Length);
    HRESULT setConfigurationData(PSCONTROLLER_DETAILS InitData, int Length);
    HRESULT startHardware(void);
    HRESULT stopHardware(void);
    HRESULT getCurrentStatus(s_STATUSMSG& StatusData);
    HRESULT sendMessage(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT getControllerParameters(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT setControllerParameters(int nValue, ECONTR_PARAM eContrparam);
    HRESULT getErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT setApplicationParameters(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
    HRESULT manageMessageBuffer(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
    HRESULT registerClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT loadDriverLibrary(void);
    HRESULT unloadDriverLibrary(void);
};
