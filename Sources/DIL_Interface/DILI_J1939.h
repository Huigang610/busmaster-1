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
 * \file DIL_IJ1939.h
 * \author Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 */

#pragma once

/* Project includes */
#include "BaseDIL_J1939.h"

class CDILI_J1939 : public CBaseDILI_J1939
{
public:
    /**
     * Init Instance
     */
    BOOL InitInstance(void);

    /**
     * Exit Instance
     */
    int ExitInstance(void);

    /* Overloaded functions */
    HRESULT initialize(Base_WrapperErrorLogger* pILog, CBaseDIL_CAN* pouIDIL_CAN);
    HRESULT uninitialize(void);
    HRESULT registerClient(BOOL bRegister, char* pacNodeName,
                                 UINT64 un64ECUName, BYTE byPrefAdres, DWORD& dwClientId);
    HRESULT manageMessageBuffer(BYTE bAction, DWORD ClientID,
                               CBaseMsgBufVSE* pBufObj);
    HRESULT sendJ1939Message (DWORD dwClient, UINT unChannel,
                                EJ1939_MSG_TYPE eMsgType, UINT32 unPGN,
                                BYTE* pbyData, UINT unDLC,
                                BYTE byPriority = DEFAULT_PRIORITY,
                                BYTE bySrc = ADDRESS_NULL,
                                BYTE byDestAdress = ADDRESS_ALL);
    HRESULT sendAcknowledgementMessage(DWORD dwClient, UINT unChannel,
                             ETYPE_ACK eAckType, UINT32 unPGN,
                             BYTE bySrc,
                             BYTE byAddresAck);
    HRESULT requestPgn(DWORD dwClient,
                             UINT unChannel, UINT32 unPGN,
                             BYTE byPriority = DEFAULT_PRIORITY,
                             BYTE bySrc = ADDRESS_NULL,
                             BYTE byDestAddress = ADDRESS_ALL);
    HRESULT goOnline();
    HRESULT goOffline();
    HRESULT getNodeName(BYTE byAddress, char* acNodeName);
    HRESULT getNodeAddress(BYTE& byAddress, DWORD dwClient);
    BOOL isAddressClaimed(BYTE byAddress);
    HRESULT claimAddress(DWORD dwClientId, UINT unChannel, BYTE byAddress, BYTE byPriority = DEFAULT_PRIORITY);
    HRESULT requestAddress(DWORD dwClient, UINT unChannel,
                                    BYTE byPriority = DEFAULT_PRIORITY,
                                    BYTE bySrc = ADDRESS_NULL,
                                    BYTE byDestAddress = ADDRESS_ALL);
    HRESULT commandAddress(DWORD dwClient, UINT unChannel,
                                    UINT64 unECU_NAME,
                                    BYTE byNewAddress,
                                    BYTE byPriority = DEFAULT_PRIORITY,
                                    BYTE bySrc = ADDRESS_NULL,
                                    BYTE byDestAddress = ADDRESS_ALL);
    HRESULT getTimeout(ETYPE_TIMEOUT eTimeOutType, UINT& unMiliSeconds);
    HRESULT configureTimeout(ETYPE_TIMEOUT eTimeOutType, UINT unMiliSeconds);
    HRESULT getTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& unAbsTime);
    BOOL isOnline(void);
    HRESULT setCallbackFunction(DWORD dwClientId, ETYPE_CLBCK_FN eClBckFnType,
                                    void* pvClBckFn);
};
