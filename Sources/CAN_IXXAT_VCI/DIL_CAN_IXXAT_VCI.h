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
 * @brief
 *      Implements the access to IXXAT VCI (Windows driver) based interfaces.
 * @remarks
 *    The available CAN channels will be stored in the m_arrInternalChannel array.
 *    This is necessary because the VCI works with 64 bit values and the Busmaster
 *    works with 32 bit values for the device access.
 */
#pragma once

#include "CAN_IXXAT_VCI_stdafx.h"
#include "AllIncludes.h"
#include "ClientList.h"
#include "IxxatCanChannel.h"



// forward declaration to make it possible to compile
// without including files outside the normal
// Busmaster includes paths
typedef struct _VCIDEVICEINFO VCIDEVICEINFO;



/**
 * @class CDIL_CAN_IXXAT_VCI
 *
 * @brief
 *  This class is the DIL interface for IXXAT VCI based
 *  CAN interfaces.
 *
 */
class CDIL_CAN_IXXAT_VCI : CBaseDIL_CAN_Controller
{
public:
    CDIL_CAN_IXXAT_VCI();
    ~CDIL_CAN_IXXAT_VCI();
    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    HRESULT performInitOperations(void);
    HRESULT performClosureOperations(void);
    HRESULT getTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL);
    HRESULT listHardwareInterfaces(InterfaceHardwareList& sSelHwInterface, INT& nCount);
    HRESULT selectHardwareInterface(const InterfaceHardwareList& sSelHwInterface, INT nCount);
    HRESULT deselectHardwareInterface(void);
    //HRESULT displayConfigurationDialog(PCHAR& InitData, int& Length);
    HRESULT displayConfigurationDialog(PSCONTROLLER_DETAILS InitData, int& Length);
    //HRESULT setConfigurationData(PCHAR pInitData, int Length);
    HRESULT setConfigurationData(PSCONTROLLER_DETAILS InitData, int Length);
    HRESULT startHardware(void);
    HRESULT stopHardware(void);
    HRESULT getCurrentStatus(s_STATUSMSG& StatusData);
    HRESULT sendMessage(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT getControllerParameters(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT setControllerParameters(int nValue, ECONTR_PARAM eContrparam);
    HRESULT getErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);

    // Specific function set
    HRESULT setApplicationParameters(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
    HRESULT registerClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName);
    HRESULT manageMessageBuffer(BYTE byAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
    HRESULT loadDriverLibrary(void);
    HRESULT unloadDriverLibrary(void);
    void    LogMessage(BOOL bShowOnlyInDebug, LPCTSTR pFormat, ...);

protected:

    BOOL m_bDriverAccessLoaded; ///< flag to indicate wether the IXXAT VCI functions has loaded
    int  m_iNumberOfCANChannelsTotal; ///< Number of all available IXXAT CAN channels


    Base_WrapperErrorLogger* m_pILog; ///< The error logger object
    HWND m_hOwnerWndHandle; ///< The owner window handle

    CClientList m_ClientList; ///< List with all of registered clients

    /**
     * Starts code for the state machine
     */
    enum
    {
        STATE_HARDWARE_NONE = 0x0,
        STATE_DRIVER_LOADED,
        STATE_HW_INTERFACE_LISTED,
        STATE_HW_INTERFACE_SELECTED,
        STATE_CONNECTED
    };

    BYTE m_byCurrHardwareState; // <- current state of the hardware access

    CIxxatCanChannel m_arrIxxatCanChannels[CHANNEL_CAN_MAX];    ///< internal can channel list
    CIxxatCanChannel m_arrTmpIxxatCanChannels[CHANNEL_CAN_MAX];  ///< internal can channel list Temp

    InterfaceHardwareList m_sSelHwInterface;
    INT m_anSelectedItems[CHANNEL_ALLOWED];

    int  VciDeviceInfoAddToArray(int iStartPosArray, VCIDEVICEINFO* pVciDevInfo, InterfaceHardwareList& sSelHwInterface);

private:
    SYSTEMTIME    m_sCurrSysTime; ///< Current system time, set in setApplicationParameters()
    UINT64        m_qwTimeStamp;  ///< The time stamp variable, not used.
    long long int m_qwQueryTickCount; ///< The query tick count, not used.
};

