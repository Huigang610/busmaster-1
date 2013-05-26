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
 * \file      DIL_Interface/DILI_J1939.cpp
 * \brief     Interface file for J1939 BUS
 * \author    Pradeep Kadoor, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for J1939 BUS
 */

/* Project includes */
#include "DIL_Interface_stdafx.h"
#include "Include/BaseDefs.h"
#include "Datatypes/MsgBufAll_Datatypes.h"
#include "DIL_J1939/DIL_J1939_Extern.h"
#include "DILI_J1939.h"

BOOL CDILI_J1939::InitInstance(void)
{
    return TRUE;
}

int CDILI_J1939::ExitInstance(void)
{
    return 0;
}

HRESULT CDILI_J1939::initialize(Base_WrapperErrorLogger* pILog, CBaseDIL_CAN* pIDIL_CAN)
{
    if (pIDIL_CAN != NULL)
    {
        return DILJ_Initialise(pILog, pIDIL_CAN);
    }
    else
    {
        return S_FALSE;
    }
}

HRESULT CDILI_J1939::uninitialize(void)
{
    return DILJ_Uninitialise();
}

HRESULT CDILI_J1939::registerClient(BOOL bRegister, char* pacNodeName,
        UINT64 un64ECUName, BYTE byPrefAdres,
        DWORD& dwClientId)
{
    return DILJ_RegisterClient(bRegister, pacNodeName, un64ECUName,
                               byPrefAdres, dwClientId);
}

HRESULT CDILI_J1939::manageMessageBuffer(BYTE bAction, DWORD dwClientID,
                                        CBaseMsgBufVSE* pBufObj)
{
    return DILJ_ManageMsgBuf(bAction, dwClientID, pBufObj);
}

HRESULT CDILI_J1939::sendJ1939Message (DWORD dwClientId, UINT unChannel, EJ1939_MSG_TYPE eMsgType, UINT32 unPGN,
        BYTE* pbyData, UINT unDLC, BYTE byPriority, BYTE bySrc, BYTE byDestAdres)
{
    return DILJ_SendJ1939Msg (dwClientId, unChannel, eMsgType, unPGN, pbyData,
                              unDLC, byPriority, bySrc, byDestAdres);

}

HRESULT CDILI_J1939::getNodeName(BYTE byAddress, char* acNodeName)
{
    return DILJ_NM_GetNodeName(byAddress, acNodeName);
}

HRESULT CDILI_J1939::getNodeAddress(BYTE& byAddress, DWORD dwClient)
{
    return DILJ_NM_GetByteAddres(byAddress, dwClient);
}

HRESULT CDILI_J1939::requestAddress(DWORD dwClient, UINT unChannel, BYTE byPriority,
        BYTE bySrc, BYTE byDestAddress)
{
    return DILJ_NM_RequestAddress(dwClient, unChannel, byPriority, bySrc, byDestAddress);
}

BOOL CDILI_J1939::isAddressClaimed(BYTE byAddress)
{
    return DILJ_NM_bIsAddressClaimed(byAddress);
}

HRESULT CDILI_J1939::claimAddress(DWORD dwClientId, UINT unChannel, BYTE byAddress, BYTE byPriority)
{
    return DILJ_NM_ClaimAddress(dwClientId, unChannel, byAddress, byPriority);
}

HRESULT CDILI_J1939::commandAddress(DWORD dwClient, UINT unChannel, UINT64 unECU_NAME,
        BYTE byNewAddress, BYTE byPriority, BYTE bySrc, BYTE byDestAddress)
{
    return DILJ_NM_CommandAddress(dwClient, unChannel, unECU_NAME, byNewAddress,
                                  byPriority, bySrc, byDestAddress);
}

HRESULT CDILI_J1939::requestPgn(DWORD dwClient, UINT unChannel, UINT32 unPGN,
                                      BYTE byPriority, BYTE bySrc, BYTE byDestAddress)
{
    return DILJ_RequestPGN(dwClient, unChannel, unPGN, byPriority, bySrc, byDestAddress);
}

HRESULT CDILI_J1939::sendAcknowledgementMessage(DWORD dwClient, UINT unChannel, ETYPE_ACK eAckType, UINT32 unPGN, BYTE bySrc, BYTE byAddresAck)
{
    return DILJ_SendAckMsg(dwClient, unChannel, eAckType, unPGN, bySrc, byAddresAck);
}

HRESULT CDILI_J1939::goOnline()
{
    return DILJ_GoOnline();
}

HRESULT CDILI_J1939::goOffline()
{
    return DILJ_GoOffline();
}

HRESULT CDILI_J1939::getTimeout(ETYPE_TIMEOUT eTimeOutType, UINT& unMiliSeconds)
{
    return DILJ_GetTimeOut(eTimeOutType, unMiliSeconds);
}

HRESULT CDILI_J1939::configureTimeout(ETYPE_TIMEOUT eTimeOutType, UINT unMiliSeconds)
{
    return DILJ_ConfigureTimeOut(eTimeOutType, unMiliSeconds);
}

HRESULT CDILI_J1939::getTimeModeMapping(SYSTEMTIME& CurrSysTime,
        UINT64& unAbsTime)
{
    return DILJ_GetTimeModeMapping(CurrSysTime, unAbsTime);
}

BOOL CDILI_J1939::isOnline(void)
{
    return DILJ_bIsOnline();
}

HRESULT CDILI_J1939::setCallbackFunction(DWORD dwClientId, ETYPE_CLBCK_FN eClBckFnType,
        void* pvClBckFn)
{
    return DILJ_SetCallBckFuncPtr(dwClientId, eClBckFnType, pvClBckFn);
}
