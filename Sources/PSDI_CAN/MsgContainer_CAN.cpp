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
 * @brief     Implementation of CMsgContainerCAN class
 * @authors   Anish Kumar, Arunkumar Karri
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CMsgContainerCAN class
 */

/* Project includes */
#include "PSDI_CAN/stdafx_CAN.h"
#include "include/Utils_Macro.h"
#include "include/error.h"
#include "Include/BaseDefs.h"
#include "Include/Can_Error_defs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_Datatypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "NodeSimEx/BaseNodeSim.h"
#include "NodeSimEx/NodeSimEx_extern.h"
#include "MsgContainer_CAN.h"

const int nBitsIn4Bytes          = 32;

const int TX_MESSAGE = 0x20000000;  /**< bitwise OR to make it a Tx message */
const int RX_MESSAGE = 0xdfffffff;  /**< bitwise AND to make it a Rx message */

#define MAKE_RTR_MESSAGE_TYPE(MSGID)         (MSGID | 0x80000000)
#define MAKE_EXTENDED_MESSAGE_TYPE(MSGID)    (MSGID | 0x40000000)
#define MAKE_STANDARD_MESSAGE_TYPE(MSGID)    (MSGID & 0xBfffffff)
#define MAKE_DEFAULT_MESSAGE_TYPE(MSGID)     (MSGID & 0x3fffffff)
#define MAKE_RX_TX_MESSAGE(MSGID, MSGTYPE)   ((MSGTYPE) ? (MSGID) : ((MSGID) | TX_MESSAGE))
#define MAKE_CHANNEL_SPECIFIC_MESSAGE(MSGID, CHANNELID) ( ((unsigned __int64)((UINT)(MSGID))) | (((__int64)(CHANNELID)) << nBitsIn4Bytes) )
#define MAKE_ERROR_MESSAGE_TYPE(ERRID)    (ERRID | 0x40000000)

CMsgContainerCAN::CMsgContainerCAN(void)
{
    InitializeCriticalSection(&m_sCritSecDataSync);
    InitializeCriticalSection(&m_omCritSecFilter);
}

CMsgContainerCAN::~CMsgContainerCAN(void)
{
    DeleteCriticalSection(&m_sCritSecDataSync);
    DeleteCriticalSection(&m_omCritSecFilter);
}

void CMsgContainerCAN::InitTimeParams(void)
{
    SYSTEMTIME CurrSysTime;
    UINT64 unAbsTime;
    if (NULL != m_pouDIL_CAN_Interface)
    {
        m_pouDIL_CAN_Interface->getTimeModeMapping(CurrSysTime, unAbsTime);
        m_ouFormatCAN.setTimeParameters(CurrSysTime, unAbsTime);
    }
}

__int64 CMsgContainerCAN::nCreateMapIndexKey( LPVOID pMsgData )
{
    STCANDATA* pouCANData = (STCANDATA*)pMsgData;
    STCAN_MSG& sMsg = pouCANData->m_uDataInfo.m_sCANMsg;

    // Form message to get message index in the CMap
    int nMsgID = 0;
    if (IS_ERR_MESSAGE(pouCANData->m_ucDataType))
    {
        nMsgID = MAKE_RX_TX_MESSAGE( usProcessCurrErrorEntry(pouCANData->m_uDataInfo.m_sErrInfo),
                                     IS_RX_MESSAGE(pouCANData->m_ucDataType));
    }
    else
    {
        nMsgID = MAKE_RX_TX_MESSAGE( sMsg.m_unMsgID,
                                     IS_RX_MESSAGE(pouCANData->m_ucDataType));
    }

    nMsgID = MAKE_DEFAULT_MESSAGE_TYPE(nMsgID);
    // For extended message
    if (sMsg.m_ucEXTENDED)
    {
        nMsgID = MAKE_EXTENDED_MESSAGE_TYPE(nMsgID);
    }
    // Apply Channel Information
    __int64 n64MapIndex = MAKE_CHANNEL_SPECIFIC_MESSAGE( nMsgID,
                          sMsg.m_ucChannel );
    return n64MapIndex;
}

/**
 * converts STCANDATA into SFRAMEINFO_BASIC_CAN
 */
static void vFormatCANDataMsg(STCANDATA* pMsgCAN,
                              tagSFRAMEINFO_BASIC_CAN* CurrDataCAN)

{
    if (RX_FLAG == pMsgCAN->m_ucDataType)
    {
        CurrDataCAN->direction = DIR_RX;
    }
    else
    {
        CurrDataCAN->direction = DIR_TX;
    }

    CurrDataCAN->channel = pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucChannel;

    if (pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucEXTENDED != 0)
    {
        CurrDataCAN->idType = TYPE_ID_CAN_EXTENDED;
    }
    else
    {
        CurrDataCAN->idType = TYPE_ID_CAN_STANDARD;
    }

    if ( pMsgCAN->m_uDataInfo.m_sCANMsg.m_bCANFD )
    {
        CurrDataCAN->messageType = TYPE_MSG_CAN_FD;
    }
    else if (pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucRTR != 0)
    {
        CurrDataCAN->messageType = TYPE_MSG_CAN_RTR;
    }
    else
    {
        CurrDataCAN->messageType = TYPE_MSG_CAN_NON_RTR;
    }


    CurrDataCAN->frameId = pMsgCAN->m_uDataInfo.m_sCANMsg.m_unMsgID;
}

BOOL CMsgContainerCAN::bTobeBlocked(STCANDATA& sCanData)
{
    static SFRAMEINFO_BASIC_CAN sBasicCanInfo;
    vFormatCANDataMsg(&sCanData, &sBasicCanInfo);

    EnterCriticalSection(&m_omCritSecFilter);
    BOOL bBlock = m_sFilterCAN.bToBeBlocked(sBasicCanInfo);
    LeaveCriticalSection(&m_omCritSecFilter);
    return bBlock;
}

BOOL CMsgContainerCAN::bIsTransitionInState( UINT unChannel,
        BYTE byRxError,
        BYTE byTxError )
{
    BOOL bIsTransition = FALSE;
    // Based on the value of transmit and receive error counter decide
    // the current error state

    if ((byTxError <= 127) && (byRxError <= 127))
    {
        // Error Active Mode
        if (m_eCurrErrorState[ unChannel ] != ERROR_ACTIVE)
        {
            bIsTransition = TRUE;
            m_eCurrErrorState[ unChannel ] = ERROR_ACTIVE;
        }
    }
    else if (byTxError == 255)
        // The sudden shift to the third state is to avoid
        // "else if ((byTxError > 127) || (byRxError > 127))"
    {
        // Bus off
        if (m_eCurrErrorState[ unChannel ] != ERROR_BUS_OFF)
        {
            bIsTransition = TRUE;
            m_eCurrErrorState[ unChannel ] = ERROR_BUS_OFF;
        }
    }
    else
    {
        // Error passive
        if (m_eCurrErrorState[ unChannel ] != ERROR_PASSIVE)
        {
            bIsTransition = TRUE;
            m_eCurrErrorState[ unChannel ] = ERROR_PASSIVE;
        }
    }

    return bIsTransition;
}

void CMsgContainerCAN::vProcessNewData(STCANDATA& sCanData)
{

    // append : 1. Add item, get count, SetItemCount.
    // OW     : 1. Get ID, Create map index, Search if it is present in map
    //             if present change the offset and time stamp
    //          2. if not present and (get count < max count) save the Buffer index,
    //             ListCtrl index, time stamp, make offest = time stamp in disp entry
    //             and add to the map and array
    //          3. if not present and (get count = max count), do nothing
    //          4. SetItemcount

    if ( IS_A_MESSAGE(sCanData.m_ucDataType) )
    {

        // Add to append buffer
        // If its the very first entry, the time stamp must
        if (m_sCANReadDataSpl.m_lTickCount.QuadPart != 0) // be 0 and will
        {
            // retain such value.
            m_sCANReadDataSpl.m_nDeltime = _abs64(sCanData.m_lTickCount.QuadPart -
                                                  m_sCANReadDataSpl.m_lTickCount.QuadPart);
        }

        STCANDATA* pStcan = &m_sCANReadDataSpl;
        *pStcan = sCanData;
        if (!bTobeBlocked(sCanData))
        {
            m_ouAppendCanBuf.writeIntoBuffer(&m_sCANReadDataSpl);

            if (NULL != m_pRxMsgCallBack)
            {
                m_pRxMsgCallBack((void*)&sCanData, CAN);
            }
        }
    }
    else //Add the error messages
    {
        // Add to append buffer
        // If its the very first entry, the time stamp must
        if (m_sCANReadDataSpl.m_lTickCount.QuadPart != 0) // be 0 and will
        {
            // retain such value.
            m_sCANReadDataSpl.m_nDeltime = _abs64(sCanData.m_lTickCount.QuadPart -
                                                  m_sCANReadDataSpl.m_lTickCount.QuadPart);
        }
        STCANDATA* pStcan = &m_sCANReadDataSpl;
        *pStcan = sCanData;
        m_ouAppendCanBuf.writeIntoBuffer(&m_sCANReadDataSpl);

        if (NULL != m_pRxMsgCallBack)
        {
            m_pRxMsgCallBack((void*)&sCanData, CAN);
        }
    }

}

void CMsgContainerCAN::vRetrieveDataFromBuffer()
{
    EnterCriticalSection(&m_sCritSecDataSync);
    while (m_ouMCCanBufFSE.getMessageCount() > 0)
    {
        STCANDATA sCanData;
        if (m_ouMCCanBufFSE.readFromBuffer(&sCanData) == CALL_SUCCESS)
        {
            vProcessNewData(sCanData);
        }
    }
    LeaveCriticalSection(&m_sCritSecDataSync);
}

BOOL CMsgContainerCAN:: bStartReadThread()
{
    int bResult = TRUE;
    HRESULT hResult;
    if (NULL != m_pouDIL_CAN_Interface)
    {
        //if (m_pouDIL_CAN_Interface->registerClient(TRUE, m_dwClientId, L"PSDI_CAN") == S_OK)
        {
            hResult = m_pouDIL_CAN_Interface->manageMessageBuffer(MSGBUF_ADD, m_dwClientId, &m_ouMCCanBufFSE);
        }
    }

    bResult = CMsgContainerBase::bStartReadThread(m_ouMCCanBufFSE.getNotifyEvent());
    return bResult;
}

HRESULT CMsgContainerCAN:: hToggleDILBufferRead(BOOL bRead)
{
    HRESULT hResult = S_FALSE;
    if (NULL != m_pouDIL_CAN_Interface)
    {
        if(bRead)
        {
            hResult = m_pouDIL_CAN_Interface->manageMessageBuffer(MSGBUF_ADD, m_dwClientId, &m_ouMCCanBufFSE);
        }
        else
        {
            hResult = m_pouDIL_CAN_Interface->manageMessageBuffer(MSGBUF_CLEAR, m_dwClientId, &m_ouMCCanBufFSE);
        }
    }
    return hResult;
}

BOOL CMsgContainerCAN:: bStopReadThread()
{
    BOOL bReturn = CMsgContainerBase::bStopReadThread();
    if (NULL != m_pouDIL_CAN_Interface)
    {
        m_pouDIL_CAN_Interface->manageMessageBuffer(MSGBUF_CLEAR, m_dwClientId, &m_ouMCCanBufFSE);
    }
    return bReturn;
}

void CMsgContainerCAN::vEditClearAll()
{
    m_ouOWCanBuf.clearMessageBuffer();
    m_ouAppendCanBuf.clearMessageBuffer();
    memset(&m_sCANReadDataSpl, 0, sizeof(m_sCANReadDataSpl));
}

int CMsgContainerCAN::nGetAppendBufferCount()
{
    return m_ouAppendCanBuf.getBufferLength();
}

int CMsgContainerCAN::nGetOWBufferCount()
{
    return m_ouOWCanBuf.getBufferLength();
}

HRESULT CMsgContainerCAN::hReadFromOWBuffer(void* psMsg, __int64 nMapIndex)
{
    //sTMCNET_MSG *psMcNetMsg = (sTMCNET_MSG*)psMsg;
    return m_ouOWCanBuf.readFromBuffer((STCANDATA*)psMsg, nMapIndex);
}

HRESULT CMsgContainerCAN::hReadFromAppendBuffer(void* pvMsg, int nMsgIndex)
{
    STCANDATA* psMsg = (STCANDATA*)pvMsg;
    static STCANDATASPL sCanMsgSpl;
    HRESULT hResult =  m_ouAppendCanBuf.readFromBuffer(&sCanMsgSpl, nMsgIndex);
    STCANDATA* psTemp = (STCANDATA*)&sCanMsgSpl;
    *psMsg = *psTemp;
    return hResult;
}

void CMsgContainerCAN::vSaveOWandGetDetails( void* pMsg,
        __int64& dwMapIndex,
        __int64& dwTimeStamp,
        UINT& nMsgCode,
        int& nBufferIndex )
{
    STCANDATA* pouCANData = (STCANDATA*)pMsg;
    dwTimeStamp = pouCANData->m_lTickCount.QuadPart;
    nMsgCode   = pouCANData->m_uDataInfo.m_sCANMsg.m_unMsgID;
    dwMapIndex =  nCreateMapIndexKey((LPVOID)pouCANData);
    //Now write into the array
    m_ouOWCanBuf.writeIntoBuffer(pouCANData, dwMapIndex, nBufferIndex);
}

HRESULT CMsgContainerCAN::ApplyFilterScheme(void* pvFilterApplied)
{
    HRESULT hResult = S_FALSE;
    SFILTERAPPLIED_CAN* psFilterCAN = (SFILTERAPPLIED_CAN*)pvFilterApplied;
    if (psFilterCAN != NULL)
    {
        EnterCriticalSection(&m_omCritSecFilter);
        //        if (m_sFilterCAN.bClone(*psFilterCAN) == TRUE)
        if (m_sFilterCAN.bClone(*psFilterCAN) == true)
        {
            //            hResult = TRUE;
            hResult = S_OK;
        }
        LeaveCriticalSection(&m_omCritSecFilter);
    }
    return hResult;
}

HRESULT CMsgContainerCAN::GetFilterScheme(void* pvFilterApplied)
{
    HRESULT hResult = S_FALSE;
    SFILTERAPPLIED_CAN* psFilterCAN = (SFILTERAPPLIED_CAN*)pvFilterApplied;
    if (psFilterCAN != NULL)
    {
        //        if (psFilterCAN->bClone(m_sFilterCAN) == TRUE)
        if (psFilterCAN->bClone(m_sFilterCAN) == true)
        {
            //hResult = TRUE;
            hResult = S_OK;
        }
    }
    return hResult;
}

HRESULT CMsgContainerCAN::EnableFilterApplied(BOOL bEnable)
{
    EnterCriticalSection(&m_omCritSecFilter);
    m_sFilterCAN.m_bEnabled = bEnable;
    LeaveCriticalSection(&m_omCritSecFilter);
    return S_OK;
}

HRESULT CMsgContainerCAN::hUpdateFormattedMsgStruct(int nListIndex,
        int& nMsgCode,
        BYTE bExprnFlag_Disp,
        __int64 nTimeOffset)
{
    HRESULT hResult = S_FALSE;
    nMsgCode  = 0;
    static STCANDATA sCANCurrData;
    static STCANDATASPL sCANCurrDataSpl;
    memset( &m_sOutFormattedData, 0, sizeof(m_sOutFormattedData) );
    if( IS_MODE_APPEND(bExprnFlag_Disp) )
    {
        //INT nType = 0;
        //INT nSize =sizeof(sCANCurrDataSpl);
        //In append mode providing interpret state is not required
        //hResult = m_ouAppendCanBuf.readEntry(nType, (BYTE*)&sCANCurrDataSpl, nSize, nListIndex, 0);
        hResult = m_ouAppendCanBuf.readFromBuffer(&sCANCurrDataSpl, nListIndex);
        sCANCurrData = *((STCANDATA*)&sCANCurrDataSpl);
        if (IS_TM_REL_SET(bExprnFlag_Disp))
        {
            //If relative time then the time will be delta time, for formatting
            sCANCurrData.m_lTickCount.QuadPart = sCANCurrDataSpl.m_nDeltime;
        }
    }
    else
    {
        hResult = m_ouOWCanBuf.readFromBuffer(&sCANCurrData, nListIndex);
        if (hResult == S_OK)
        {
            if (IS_TM_REL_SET(bExprnFlag_Disp))
            {
                //If relative time then the time will be delta time, for formatting
                sCANCurrData.m_lTickCount.QuadPart -= nTimeOffset;
            }
        }
    }

    if (hResult == S_OK)
    {
        m_ouFormatCAN.vFormatCANDataMsg(&sCANCurrData,
                                        &m_sOutFormattedData,
                                        bExprnFlag_Disp);

        //If Message is erroneous, return S_FALSE
        if(IS_ERR_MESSAGE(sCANCurrData.m_ucDataType))
        {
            nMsgCode = usProcessCurrErrorEntry(sCANCurrData.m_uDataInfo.m_sErrInfo);
            return S_FALSE;
        }

        //Now add the name of message if present in database else show the code
        nMsgCode = sCANCurrData.m_uDataInfo.m_sCANMsg.m_unMsgID;
    }
    else
    {
        memset (&m_sOutFormattedData, 0, sizeof(m_sOutFormattedData));
        nMsgCode = -1;
    }

    return hResult;
}

void CMsgContainerCAN::vSetCurrMsgName(CString strMsgNameOrCode)
{
    CMsgContainerBase::bCopyStringTocharArr (m_sOutFormattedData.m_acMsgDesc, strMsgNameOrCode,
            sizeof(m_sOutFormattedData.m_acMsgDesc));
}

void CMsgContainerCAN::vSetMsgLength(CString strsgLength)
{
    CMsgContainerBase::bCopyStringTocharArr (m_sOutFormattedData.m_acDataLen, strsgLength,
            sizeof(m_sOutFormattedData.m_acDataLen));

    m_sOutFormattedData.m_byDataLength = atoi(strsgLength);
}

USHORT CMsgContainerCAN::usProcessCurrErrorEntry(SERROR_INFO& sErrInfo)
{
    // Get the Error code
    USHORT usErrorID;
    // Get the channel number
    CHAR nChannel = sErrInfo.m_ucChannel - 1;
    if( nChannel < 0 || nChannel >= defNO_OF_CHANNELS )
    {
        ASSERT( FALSE );
        // Take prevension
        nChannel = 0;
    }

    // Decide which module(s) to notify by analysing the error code
    // Accordingly notify the modules by sending/posting message
    if (sErrInfo.m_ucErrType == ERROR_BUS)
    {
        usErrorID = sErrInfo.m_ucReg_ErrCap /*& 0xE0*/;
    }
    else if (sErrInfo.m_ucErrType == ERROR_WARNING_LIMIT_REACHED)
    {
        // Reaching warning limit isn't considered as an error.
        // In case of this interrupt there is no need for display to
        // be updated.
        // Use Channel ID as High Byte of WPARAM
        usErrorID = ERROR_UNKNOWN;
    }
    // Handle interrupts. This is not error message and assign error code
    // to unknown
    // Interrupts are state transition indicators but not real error message
    else if (sErrInfo.m_ucErrType == ERROR_INTERRUPT)
    {
        // Use Channel ID as High Byte of WPARAM
        usErrorID = ERROR_UNKNOWN;
    }
    else
    {
        usErrorID = sErrInfo.m_ucErrType;
    }

    return usErrorID;
}

void CMsgContainerCAN::vClearFormattedMsgStruct()
{
    memset(&m_sOutFormattedData, 0, sizeof(m_sOutFormattedData));
}

void CMsgContainerCAN::vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL& sHdrColStruct,
        char* pomDataPtrArr[MAX_MSG_WND_COL_CNT],
        BYTE bExprnFlag_Disp)
{
    //Time mode
    if (IS_TM_ABS_SET(bExprnFlag_Disp)) //for Absolute non-reset timestamp
    {
        pomDataPtrArr[sHdrColStruct.m_byTimePos] = m_sOutFormattedData.m_acTimeAbs;
    }

    else if (IS_TM_REL_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byTimePos] = m_sOutFormattedData.m_acTimeRel;
    }

    else if (IS_TM_SYS_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byTimePos] = m_sOutFormattedData.m_acTimeSys;
    }

    // Dec/Hex mode for DATA and ID
    if (IS_NUM_HEX_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byDataPos] = m_sOutFormattedData.m_acDataHex;
        pomDataPtrArr[sHdrColStruct.m_byIDPos]   = m_sOutFormattedData.m_acMsgIDHex;
    }
    else if (IS_NUM_DEC_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byDataPos] = m_sOutFormattedData.m_acDataDec;
        pomDataPtrArr[sHdrColStruct.m_byIDPos]   = m_sOutFormattedData.m_acMsgIDDec;
    }

    //Rest will always be same
    pomDataPtrArr[sHdrColStruct.m_byDLCPos]       = m_sOutFormattedData.m_acDataLen;
    pomDataPtrArr[sHdrColStruct.messageTypePos]   = m_sOutFormattedData.m_acType;
    pomDataPtrArr[sHdrColStruct.m_byRxTxPos]      = m_sOutFormattedData.m_acMsgDir;
    pomDataPtrArr[sHdrColStruct.m_byCodeNamePos]  = m_sOutFormattedData.m_acMsgDesc;
    pomDataPtrArr[sHdrColStruct.m_byChannel]      = m_sOutFormattedData.m_acChannel;
}

BOOL CMsgContainerCAN::bGetDilInterFace()
{
    BOOL bFound = FALSE;
    DIL_GetInterface(CAN, (void**)&m_pouDIL_CAN_Interface);
    if (NULL != m_pouDIL_CAN_Interface)
    {
        bFound = TRUE;
    }
    return bFound;
}

void CMsgContainerCAN::SetClientID(DWORD dwClientID)
{
    m_dwClientId = dwClientID;
}

void CMsgContainerCAN::DoSortBuffer(int nField,bool bAscending)
{
    m_ouAppendCanBuf.vDoSortBuffer(nField,bAscending);
    m_ouOWCanBuf.vDoSortBuffer(nField,bAscending);
}

void CMsgContainerCAN::GetMapIndexAtID(int nIndex,__int64& nMapIndex)
{
    m_ouOWCanBuf.nGetMapIndexAtID(nIndex,nMapIndex);
}
