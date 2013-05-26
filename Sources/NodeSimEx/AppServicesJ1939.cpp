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
 * @file AppServicesJ1939.cpp
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 */

/* Project includes */
#include "NodeSimEx_stdafx.h"
#include "Include/Basedefs.h"
#include "Include/DIL_Commondefs.h"
#include "Datatypes/J1939_Datatypes.h"
#include "ExecuteManager.h"
#include "GlobalObj.h"
#include "AppServicesJ1939.h"

UINT g_unGoOnline_J1939(BOOL bEnable, HMODULE /*hModule*/)
{
    HRESULT hResult = S_FALSE;
    /*CExecuteFunc* pmCEexecuteFunc =
                CExecuteManager::ouGetExecuteManager(J1939).pmGetNodeObject(hModule);*/
    if (bEnable == TRUE)
    {
        hResult = CGlobalObj::GetIJ1939DIL()->goOnline();
    }
    else
    {
        hResult = CGlobalObj::GetIJ1939DIL()->goOffline();
    }
    return ((hResult == S_OK) ? 1 : 0);
}

UINT g_unSendMsg_J1939(STJ1939_MSG* psTxMsg, HMODULE hModule)
{
    UINT Return = 0;
    VALIDATE_POINTER_RETURN_VAL(psTxMsg, Return);
    CExecuteFunc* pmCEexecuteFunc =
        CExecuteManager::ouGetExecuteManager(J1939).pmGetNodeObject(hModule);

    if (pmCEexecuteFunc != NULL)
    {
        BOOL bMsgTxFlag = pmCEexecuteFunc->bGetMsgTxFlag();
        if (bMsgTxFlag)
        {
            sNODEINFO sNode(J1939);
            pmCEexecuteFunc->vGetNodeInfo(sNode);
            if (CGlobalObj::GetIJ1939DIL()->sendJ1939Message(sNode.m_dwClientId,
                    psTxMsg->m_sMsgProperties.m_byChannel,
                    psTxMsg->m_sMsgProperties.m_eType,
                    psTxMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(),
                    psTxMsg->m_pbyData,
                    psTxMsg->m_unDLC,
                    psTxMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPriority,
                    ADDRESS_NULL,
                    psTxMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific) == S_OK)
            {
                Return = 1;
            }
        }
    }
    return Return;
}

UINT g_unRequestPGN_J1939(UINT PGN, BYTE DestAdres, UINT Channel, HMODULE hModule)
{
    UINT Return = 0;
    CExecuteFunc* pmCEexecuteFunc =
        CExecuteManager::ouGetExecuteManager(J1939).pmGetNodeObject(hModule);

    if (pmCEexecuteFunc != NULL)
    {
        BOOL bMsgTxFlag = pmCEexecuteFunc->bGetMsgTxFlag();
        if (bMsgTxFlag)
        {
            sNODEINFO sNode(J1939);
            pmCEexecuteFunc->vGetNodeInfo(sNode);
            if (CGlobalObj::GetIJ1939DIL()->requestPgn(sNode.m_dwClientId,
                    Channel, PGN, DEFAULT_PRIORITY, ADDRESS_NULL, DestAdres) == S_OK)
            {
                Return = 1;
            }
        }
    }
    return Return;
}

UINT g_unSendAckMsg_J1939(UINT PGN, BYTE AckType, BYTE DestAdres, UINT Channel,
                          HMODULE hModule)
{
    UINT Return = 0;
    CExecuteFunc* pmCEexecuteFunc =
        CExecuteManager::ouGetExecuteManager(J1939).pmGetNodeObject(hModule);

    if (pmCEexecuteFunc != NULL)
    {
        BOOL bMsgTxFlag = pmCEexecuteFunc->bGetMsgTxFlag();
        if (bMsgTxFlag)
        {
            sNODEINFO sNode(J1939);
            pmCEexecuteFunc->vGetNodeInfo(sNode);
            if (CGlobalObj::GetIJ1939DIL()->sendAcknowledgementMessage(sNode.m_dwClientId,
                    Channel,
                    (ETYPE_ACK)AckType,
                    PGN, 0, DestAdres)
                    == S_OK)
            {
                Return = 1;
            }
        }
    }
    return Return;
}

UINT g_unClaimAddress_J1939(BYTE Address, UINT Channel, HMODULE hModule)
{
    UINT Return = 0;
    CExecuteFunc* pmCEexecuteFunc =
        CExecuteManager::ouGetExecuteManager(J1939).pmGetNodeObject(hModule);

    if (pmCEexecuteFunc != NULL)
    {
        BOOL bMsgTxFlag = pmCEexecuteFunc->bGetMsgTxFlag();
        if (bMsgTxFlag)
        {
            sNODEINFO sNode(J1939);
            pmCEexecuteFunc->vGetNodeInfo(sNode);
            if (CGlobalObj::GetIJ1939DIL()->claimAddress(sNode.m_dwClientId,
                    Channel,Address)
                    == S_OK)
            {
                Return = 1;
            }
        }
    }
    return Return;
}

UINT g_unRequestAddress_J1939(UINT Channel, HMODULE hModule)
{
    UINT Return = 0;
    CExecuteFunc* pmCEexecuteFunc =
        CExecuteManager::ouGetExecuteManager(J1939).pmGetNodeObject(hModule);

    if (pmCEexecuteFunc != NULL)
    {
        BOOL bMsgTxFlag = pmCEexecuteFunc->bGetMsgTxFlag();
        if (bMsgTxFlag)
        {
            sNODEINFO sNode(J1939);
            pmCEexecuteFunc->vGetNodeInfo(sNode);
            if (CGlobalObj::GetIJ1939DIL()->requestAddress(sNode.m_dwClientId,
                    Channel)
                    == S_OK)
            {
                Return = 1;
            }
        }
    }
    return Return;
}

UINT g_unCommandAddress_J1939(UINT64 EcuName, BYTE NewAddress, UINT Channel,
                              HMODULE hModule)
{
    UINT Return = 0;
    CExecuteFunc* pmCEexecuteFunc =
        CExecuteManager::ouGetExecuteManager(J1939).pmGetNodeObject(hModule);

    if (pmCEexecuteFunc != NULL)
    {
        BOOL bMsgTxFlag = pmCEexecuteFunc->bGetMsgTxFlag();
        if (bMsgTxFlag)
        {
            sNODEINFO sNode(J1939);
            pmCEexecuteFunc->vGetNodeInfo(sNode);
            if (CGlobalObj::GetIJ1939DIL()->commandAddress(sNode.m_dwClientId,
                    Channel,
                    EcuName,
                    NewAddress)
                    == S_OK)
            {
                Return = 1;
            }
        }
    }
    return Return;
}

UINT g_unSetTimeout_J1939(BYTE TimeoutType, UINT TimeoutValue, HMODULE /*hModule*/)
{
    UINT Return = 0;
    if (CGlobalObj::GetIJ1939DIL()->configureTimeout((ETYPE_TIMEOUT) TimeoutType,
            TimeoutValue)
            == S_OK)
    {
        Return = 1;
    }
    return Return;
}
/******************************************************************************
    Function Name    :  gbWtiteToLog_J1939
    Input(s)         :  char* string to write in Log Files
    Output           :  TRUE if call is successs otherwise FALSE
    Functionality    :  This function will send a message to mainframe to write
                        the input string in all log files
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Venkatanarayana Makam
    Date Created     :  27/06/2012
    -M-difications    :
******************************************************************************/
BOOL gbWtiteToLog_J1939(char* pcString)
{
    BOOL Result = FALSE;
    if (CGlobalObj::sm_hWndMDIParentFrame != NULL)
    {
        ::SendMessage(CGlobalObj::sm_hWndMDIParentFrame, WM_FROM_USER_DLL, (WPARAM)WRITE_TO_LOGFILE_J1939,(LPARAM)pcString);
        Result = TRUE;
    }
    return Result;
}