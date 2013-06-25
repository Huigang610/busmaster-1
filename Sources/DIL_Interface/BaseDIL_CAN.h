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
 * @brief     Base class for CDIL_CAN class.
 * @author    Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Base class for CDIL_CAN class.
 */

#pragma once

/* Project includes */
#include "DataTypes/DIL_DataTypes.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "Datatypes/Base_WrapperErrorLogger.h"

class CBaseDIL_CAN
{
public:
    /**
     * Based on the parameter this function renders number of the driver interface
     * layers supported or available.
     *
     * @req RSI_14_001 getDriverList
     * @req RS_23_01 Getter for the DIL List
     * @param[out] list
     *   List of supported DILs. If List is NULL, only number is returned.
     * @return
     *   Returns number of the DILs implemented
     */
    virtual DWORD getDriverList(DriverList* list) = 0;

    /**
     * This selects a driver abstraction layer (DAL).
     *
     * @req RSI_14_002 selectDriver
     * @req RS_23_02 Selecting a driver from the DIL list
     */
    virtual HRESULT selectDriver(DWORD dwDriverID, HWND hWndParent,
                                 Base_WrapperErrorLogger* pILog) = 0;

    /**
     * registerClient registers/unregisters the client.
     * Only registered client's buffer will be updated on receive of a msg in the bus.
     *
     * @req RSI_14_003 registerClient
     * @req RS_23_04 Registration of a client to simulate a node
     * @param[in] bRegister
     *   - TRUE for registeration
     *   - FALSE for unregisteration
     *
     * @return
     *   - S_OK -->registeration successful
     *   - ERR_CLIENT_EXISTS -->Client already registered. Client Id will be stored in ClientId.
     *   - ERR_NO_CLIENT_EXIST --> No client exists. Usually this happens if we try to remove non existing client.
     *   - ERR_NO_MORE_CLIENT_ALLOWED -->No more clients are allowed to register.
     */
    virtual HRESULT registerClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) = 0;

    /**
     * This function manages the target message buffer list.
     * At present maximum number of entries in the list is kept as 8.
     *
     * @req RSI_14_004 manageMessageBuffer
     * @param[in] bAction
     *   - MSGBUF_ADD, add pBufObj to the target message buffer list.
     *   - MSGBUF_CLEAR, clear the list. pBufObj is ignored.
     */
    virtual HRESULT manageMessageBuffer(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj) = 0;

    /**
     * This returns ID of the driver interface layer presently selected.
     *
     * @req RSI_14_005 getSelectedDriver
     * @req RS_23_03 Getter for the presently selected driver
     */
    virtual DWORD getSelectedDriver(void) = 0;

    /**
     * Call for all initialisation operations.
     *
     * @req RSI_14_006 performInitOperations
     * @req RS_23_08 Carry out initialization operations
     */
    virtual HRESULT performInitOperations(void) = 0;

    /**
     * Call for all uninitialisation operations
     *
     * @req RSI_14_007 performClosureOperations
     * @req RS_23_09 Carry out closure operations
     */
    virtual HRESULT performClosureOperations(void) = 0;

    /**
     * Call this function to get a system time and the time stamp associated with it
     *
     * @req RSI_14_008 getTimeModeMapping
     * @req RS_23_10 Getter for the time mode mapping (usually the 64-bit time stamp by the driver)
     */
    virtual HRESULT getTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL) = 0;

    /**
     * Call this function to list the hardware interfaces available and receive
     * user's choice.
     *
     * @req RSI_14_009 listHardwareInterfaces
     * @req RS_23_11 Listing of the controllers for the current driver
     */
    virtual HRESULT listHardwareInterfaces(InterfaceHardwareList& asSelHwInterface, INT& nCount) = 0;

    /**
     * Call this function to select a particular hardware interface.
     *
     * @req RSI_14_010 selectHardwareInterfaces
     * @req RS_23_12 Selection of a controller from the hardware interface list
     */
    virtual HRESULT selectHardwareInterfaces(const InterfaceHardwareList& asSelHwInterface, INT nCount) = 0;

    /**
     * Call this function to deselect the selected hardware interface.
     *
     * @req RSI_14_011 deselectHardwareInterfaces
     * @req RS_23_13 Deselection of the presently selected controller
     */
    virtual HRESULT deselectHardwareInterfaces(void) = 0;

    /**
     * Function to display the configuration dialog box for the selected DIL. If
     * the dialog box needs to be displayed been initialised, pass the relevant data
     * as InitData. If it is null, the dialog box is uninitialised. This also contains
     * the user's choice as OUT parameter
     *
     * @req RSI_14_012 displayConfigurationDialog
     * @req RS_23_14 Display the configuration dialog box of the present controller
     */
    virtual HRESULT displayConfigurationDialog(PSCONTROLLER_DETAILS InitData, int& Length) = 0;

    /**
     * To set the configuration data for the currently selected DIL. Caller must
     * release the memory.
     *
     * @req RSI_14_013 setConfigurationData
     * @req RS_23_15 Setting of the configuration data for the present controller
     */
    virtual HRESULT setConfigurationData(PSCONTROLLER_DETAILS pInitData, int Length) = 0;

    /**
     * Start the controller
     *
     * @req RSI_14_014 startHardware
     * @req RS_23_16 Start the presently selected controller (or connect)
     */
    virtual HRESULT startHardware(void) = 0;

    /**
     * Stop the controller
     *
     * @req RSI_14_015 stopHardware
     * @req RS_23_17 Stop the presently selected controller (or disconnect)
     */
    virtual HRESULT stopHardware(void) = 0;

    /**
     * Send messages
     *
     * @req RSI_14_017 sendMessage
     * @req RS_23_19 Transmit a frame
     */
    virtual HRESULT sendMessage(DWORD dwClientID, const STCAN_MSG& sCanTxMsg) = 0;

    /**
     * Call to get Controller parameters. Value will be returned stored in lParam
     *
     * @req RSI_14_022 getControllerParameters
     */
    virtual HRESULT getControllerParameters(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam) = 0;

    /**
     * Set Controller parameters.
     */
    virtual HRESULT setControllerParameters(int nValue, ECONTR_PARAM eContrparam) = 0;

    /**
     * Call to Get Error Counts
     *
     * @req RSI_14_023 getErrorCount
     */
    virtual HRESULT getErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam) = 0;
};
