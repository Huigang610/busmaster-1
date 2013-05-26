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
 * @file      BaseDIL_CAN_Controller.h
 * @brief     Base class for CDIL_CAN class.
 * @author    Arunkumar Karri
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Base class for CAN controller classes.
 */

#pragma once

class CBaseDIL_CAN_Controller
{
public:
    /* Function pointers */
    virtual HRESULT performInitOperations(void) = 0;
    virtual HRESULT performClosureOperations(void) = 0;
    virtual HRESULT getTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL) = 0;
    virtual HRESULT listHardwareInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount) = 0;
    virtual HRESULT selectHardwareInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount) = 0;
    virtual HRESULT deselectHardwareInterface(void) = 0;
    virtual HRESULT displayConfigurationDialog(PSCONTROLLER_DETAILS InitData, int& Length) = 0;
    virtual HRESULT setConfigurationData(PSCONTROLLER_DETAILS InitData, int Length) = 0;
    virtual HRESULT startHardware(void) = 0;
    virtual HRESULT stopHardware(void) = 0;
    virtual HRESULT getCurrentStatus(s_STATUSMSG& StatusData) = 0;
    virtual HRESULT sendMessage(DWORD dwClientID, const STCAN_MSG& sCanTxMsg) = 0;
    virtual HRESULT getControllerParameters(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam) = 0;
    virtual HRESULT setControllerParameters(int nValue, ECONTR_PARAM eContrparam) = 0;
    virtual HRESULT getErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam) = 0;
    virtual HRESULT setApplicationParameters(HWND hWndOwner, Base_WrapperErrorLogger* pILog) = 0;
    virtual HRESULT manageMessageBuffer(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj) = 0;
    virtual HRESULT registerClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) = 0;
    virtual HRESULT loadDriverLibrary(void) = 0;
    virtual HRESULT unloadDriverLibrary(void) = 0;
};
