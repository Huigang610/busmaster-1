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
 * @file      DIL_Interface/DIL_CAN.h
 * @brief     Definition file for CDIL_CAN class.
 * @author    Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CDIL_CAN class.
 */

#pragma once

/* Project includes */
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "Include/Basedefs.h"
#include "BaseDIL_CAN.h"
#include "BaseDIL_CAN_Controller.h"

class CDIL_CAN : public CBaseDIL_CAN
{
public:
    /**
     * Constructor
     */
    CDIL_CAN();

    /**
     * Destructor
     */
    ~CDIL_CAN();

    /**
     * The router is realised as a set of function pointers
     * Initialisation with dummy functions
     */
    BOOL InitInstance(void);

    /**
     * Exit Instance
     */
    int ExitInstance(void);

    /**
	 * Variable to maintain currently selected Driver ID
	 */
    DWORD driverId;
    
	/**
	 * Variable to store previously selected Driver ID
	 */
    DWORD m_dwOldDriverID;

    /**
	 * member variable to hold the pointer of currently selected controller interface
	 */
    CBaseDIL_CAN_Controller* m_pBaseDILCAN_Controller;
    
	/**
	 * member variable to hold the previously selected controller interface
	 */
    CBaseDIL_CAN_Controller* m_pOldBaseDILCAN_Controller;

    /**
	 * Variable to hold handle to currently selected controller DIL
	 */
    HMODULE m_hDll;
    
	/**
	 * Variable to hold handle to previously selected controller DIL
	 */
    HMODULE m_hOldDll;

    /* Overloaded functions */
    DWORD getDriverList(DILLIST* List);
    HRESULT selectDriver(DWORD dwDriverID, HWND hWndParent, Base_WrapperErrorLogger* pILog);
    HRESULT registerClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT manageMessageBuffer(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
    DWORD getSelectedDriver(void);
    HRESULT performInitOperations(void);
    HRESULT performClosureOperations(void);
    HRESULT getTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL);
    HRESULT listHardwareInterfaces(InterfaceHardwareList& sSelHwInterface, INT& nCount);
    HRESULT selectHardwareInterfaces(const InterfaceHardwareList& sSelHwInterface, INT nCount);
    HRESULT deselectHardwareInterfaces(void);
    HRESULT displayConfigurationDialog(PSCONTROLLER_DETAILS InitData, int& Length);
    HRESULT setConfigurationData(PSCONTROLLER_DETAILS InitData, int Length);
    HRESULT startHardware(void);
    HRESULT stopHardware(void);
    HRESULT sendMessage(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT getControllerParameters(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    virtual HRESULT setControllerParameters(int nValue, ECONTR_PARAM eContrparam);
    HRESULT  getErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);

private:
    /**
     * Helper Function for Dummy Interface
     */
    void vSelectInterface_Dummy(void);

    /* Function pointers */
    HRESULT (*m_pfPerformInitOperations)(void);
    HRESULT (*m_pfPerformClosureOperations)(void);
    HRESULT (*m_pfGetTimeModeMapping)(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount);
    HRESULT (*m_pfListHwInterfaces)(InterfaceHardwareList& asSelHwInterface, INT& nCount);
    HRESULT (*m_pfSelectHwInterface)(const InterfaceHardwareList& asSelHwInterface, INT nCount);
    HRESULT (*m_pfDeselectHardwareInterfaces)(void);
    HRESULT (*m_pfDisplayConfigDlg)(PSCONTROLLER_DETAILS InitData, int& Length);
    HRESULT (*m_pfSetConfigData)(PCHAR pInitData, int Length);
    HRESULT (*m_pfStartHardware)(void);
    HRESULT (*m_pfStopHardware)(void);
    HRESULT (*m_pfSendMsg)(DWORD dwClientID, const STCAN_MSG& pouFlxTxMsg);
    HRESULT (*m_pfManageMsgBuf)(BYTE, DWORD ClientID, CBaseCANBufFSE*);
    HRESULT (*m_pfRegisterClient)(BOOL bRegister, DWORD&, char*);
    HRESULT (*m_pfGetControllerParams)(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT (*m_pfGetErrorCount)(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
};
