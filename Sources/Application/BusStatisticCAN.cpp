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
 * @author    venkatanarayana Makam
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

/* Project includes */
#include "StdAfx.h"
#include "BusStatisticCAN.h"
#include "Include/Can_Error_Defs.h"
#include "DIL_Interface/DIL_Interface_Extern.h"
#include "TimeManager.h"

/**
 * Can interface to Bus Statistics
 */
void* CBusStatisticCAN:: sm_pouBSCan;

/**
 * @param[in] pBSCan contain the CBusStatisticCAN object
 * @return Function return Status
 *
 * Updates the busstatistics parameters using
 * updateBusStatistics Function
 */
int readBusStatisticsDataBuffer(CBusStatisticCAN* pBSCan)
{
    ASSERT(pBSCan != NULL);
    while (pBSCan->m_ouCanBufFSE.getMessageCount() > 0)
    {
        static STCANDATA sCanData;
        //sCanData.m_lTickCount.QuadPart;
        int Result = pBSCan->m_ouCanBufFSE.readFromBuffer(&sCanData);
        if (Result == ERR_READ_MEMORY_SHORT)
        {
            TRACE("ERR_READ_MEMORY_SHORT");
        }
        else if (Result == EMPTY_APP_BUFFER)
        {
            TRACE("EMPTY_APP_BUFFER");
        }
        else
        {
            pBSCan->updateBusStatistics(sCanData);
        }

    }
    return 0;
}

/**
 * @param[in] pVoid contains CBusStatisticCAN pointer
 *
 * Thread Function that is used to read the driver data and
 * updates the Bus Statistics.
 */
DWORD WINAPI BusStatisticsDataReadThreadProc(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == NULL)
    {
        return ((DWORD)-1);
    }

    CBusStatisticCAN* pBusStatistics = static_cast<CBusStatisticCAN*> (pThreadParam->m_pBuffer);

    if (pBusStatistics == NULL)
    {
        return ((DWORD)-1);
    }
    bool bLoopON = true;
    pThreadParam->m_unActionCode = INVOKE_FUNCTION;
    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                readBusStatisticsDataBuffer(pBusStatistics); // Retrieve message from the driver
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
                //Nothing at this moment
            }
            break;
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
            default:
            {

            }
            break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}

CBusStatisticCAN::CBusStatisticCAN(void)
{
    InitializeCriticalSection(&m_omCritSecBS);
    m_ouReadThread.m_hActionEvent = m_ouCanBufFSE.getNotifyEvent();
    m_nTimerHandle = NULL;
    //Initialise Number of Bits in Standard CAN and FD Messages
    UINT unBitsStdMsg[] = {51, 60, 70, 79, 89, 99, 108, 118, 127};
    UINT unBitsStdFDMsgCRC15[] = {54, 64, 73, 83, 93, 102, 112, 121, 131};
    UINT unBitsStdFDMsgCRC17[] = {172, 210};     // 12, 16
    UINT unBitsStdFDMsgCRC21[] = {253, 292, 369, 522, 676}; // 20, 24, 32, 48, 64

    memcpy(m_unBitsStdMsg, unBitsStdMsg, 9*sizeof(UINT));
    memcpy(&m_unBitsFDMsgCRC15[0], unBitsStdFDMsgCRC15, 9*sizeof(UINT));
    memcpy(&m_unBitsFDMsgCRC17[0], unBitsStdFDMsgCRC17, 2*sizeof(UINT));
    memcpy(&m_unBitsFDMsgCRC21[0], unBitsStdFDMsgCRC21, 5*sizeof(UINT));

    //Initialise Number of Bits in Extended CAN and FD Messages
    UINT unBitsExtMsg[] = {75, 84, 94, 103, 113, 123, 132, 142, 151};
    UINT unBitsExtFDMsgCRC15[] = {77, 87, 96, 106, 115, 125, 135, 144, 154};
    UINT unBitsExtFDMsgCRC17[] = {195, 233};    // 12, 16
    UINT unBitsExtFDMsgCRC21[] = {276, 315, 391, 545, 699}; // 20, 24, 32, 48, 64

    memcpy(m_unBitsExdMsg, unBitsExtMsg, 9*sizeof(UINT));
    memcpy(&m_unBitsFDMsgCRC15[1], unBitsExtFDMsgCRC15, 9*sizeof(UINT));
    memcpy(&m_unBitsFDMsgCRC17[1], unBitsExtFDMsgCRC17, 2*sizeof(UINT));
    memcpy(&m_unBitsFDMsgCRC21[1], unBitsExtFDMsgCRC21, 5*sizeof(UINT));

    m_nFactorSTDFrame = 0;
    m_nFactorEXTDFrame = 0;
    m_nFactorSTDRTRFrame = 0;
    m_nFactorEXTDRTRFrame = 0;
    m_nFactorErrorFrame = 0;
    m_dDiffTime = 0;
}

CBusStatisticCAN::~CBusStatisticCAN(void)
{

    m_ouReadThread.bTerminateThread();      // Terminate read thread
    m_ouCanBufFSE.clearMessageBuffer();    // clear can buffer
    DeleteCriticalSection(&m_omCritSecBS);  // delete critical section
    if( m_nTimerHandle != NULL )
    {
        KillTimer(NULL, m_nTimerHandle);
    }
}

HRESULT CBusStatisticCAN::doInitialization(void)
{
    if (DIL_GetInterface(CAN, (void**)&m_pouDIL_CAN) == S_OK)
    {
        DWORD dwClientId = 0;
        m_pouDIL_CAN->registerClient(TRUE, dwClientId, CAN_MONITOR_NODE);
        m_pouDIL_CAN->manageMessageBuffer(MSGBUF_ADD, dwClientId, &m_ouCanBufFSE);
    }
    initializeBusStatisticsData();

    sm_pouBSCan = this;

    //Start the read thread

    return startBusStatisticsReadThread()?S_OK:S_FALSE;
}

BOOL CBusStatisticCAN::startUpdation(BOOL bStart)
{
    if(bStart == TRUE)
    {
        if(m_nTimerHandle == NULL)
        {
            m_nTimerHandle = ::SetTimer(NULL, 0, 1000, onTimeWrapper);
        }
    }
    else
    {
        KillTimer(NULL, m_nTimerHandle);
        m_nTimerHandle = NULL;
    }
    return 0;
}

HRESULT CBusStatisticCAN::resetBusStatistics(void)
{
    EnterCriticalSection(&m_omCritSecBS);

    initializeBusStatisticsData();
    LeaveCriticalSection(&m_omCritSecBS);
    return 0L;
}

HRESULT CBusStatisticCAN::getTotalMessageCount(UINT unChannelIndex, Direction eDir,
        BYTE byIdType, BYTE byMsgType, UINT& nMsgCount)
{
    /*-----------------------------------------------------------------------------
        S.no         eDir          byIdType                    byMsgType
        ---------------------------------------------------------------------------
        1.          DIR_RX      TYPE_ID_CAN_STANDARD        TYPE_MSG_CAN_RTR
        2.          DIR_RX      TYPE_ID_CAN_STANDARD        TYPE_MSG_CAN_NON_RTR
        3.          DIR_RX      TYPE_ID_CAN_STANDARD        TYPE_MSG_CAN_ALL
        4.          DIR_RX      TYPE_ID_CAN_EXTENDED        TYPE_MSG_CAN_RTR
        5.          DIR_RX      TYPE_ID_CAN_EXTENDED        TYPE_MSG_CAN_NON_RTR
        6.          DIR_RX      TYPE_ID_CAN_EXTENDED        TYPE_MSG_CAN_ALL
        7.          DIR_RX      TYPE_ID_CAN_ALL             TYPE_MSG_CAN_RTR
        8.          DIR_RX      TYPE_ID_CAN_ALL             TYPE_MSG_CAN_NON_RTR
        9.          DIR_RX      TYPE_ID_CAN_ALL             TYPE_MSG_CAN_ALL

        10.         DIR_TX      TYPE_ID_CAN_STANDARD        TYPE_MSG_CAN_RTR
        11.         DIR_TX      TYPE_ID_CAN_STANDARD        TYPE_MSG_CAN_NON_RTR
        12.         DIR_TX      TYPE_ID_CAN_STANDARD        TYPE_MSG_CAN_ALL
        13.         DIR_TX      TYPE_ID_CAN_EXTENDED        TYPE_MSG_CAN_RTR
        14.         DIR_TX      TYPE_ID_CAN_EXTENDED        TYPE_MSG_CAN_NON_RTR
        15.         DIR_TX      TYPE_ID_CAN_EXTENDED        TYPE_MSG_CAN_ALL
        16.         DIR_TX      TYPE_ID_CAN_ALL             TYPE_MSG_CAN_RTR
        17.         DIR_TX      TYPE_ID_CAN_ALL             TYPE_MSG_CAN_NON_RTR
        18.         DIR_TX      TYPE_ID_CAN_ALL             TYPE_MSG_CAN_ALL

        19          DIR_ALL     TYPE_ID_CAN_STANDARD        TYPE_MSG_CAN_RTR
        20          DIR_ALL     TYPE_ID_CAN_STANDARD        TYPE_MSG_CAN_NON_RTR
        21          DIR_ALL     TYPE_ID_CAN_STANDARD        TYPE_MSG_CAN_ALL
        22          DIR_ALL     TYPE_ID_CAN_EXTENDED        TYPE_MSG_CAN_RTR
        23          DIR_ALL     TYPE_ID_CAN_EXTENDED        TYPE_MSG_CAN_NON_RTR
        24          DIR_ALL     TYPE_ID_CAN_EXTENDED        TYPE_MSG_CAN_ALL
        25          DIR_ALL     TYPE_ID_CAN_ALL             TYPE_MSG_CAN_RTR
        26          DIR_ALL     TYPE_ID_CAN_ALL             TYPE_MSG_CAN_NON_RTR
        27          DIR_ALL     TYPE_ID_CAN_ALL             TYPE_MSG_CAN_ALL
    -----------------------------------------------------------------------------*/
    EnterCriticalSection(&m_omCritSecBS);

    if( eDir == DIR_RX )
    {
        if( byIdType == TYPE_ID_CAN_STANDARD )
        {
            if( byMsgType == TYPE_MSG_CAN_RTR )
            {
                //Case1:(eDir=DIR_RX; byIdType=TYPE_ID_CAN_STANDARD;byMsgType=TYPE_MSG_CAN_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxSTD_RTRMsgCount;
            }
            else if( byMsgType == TYPE_MSG_CAN_NON_RTR)
            {
                //Case2:(eDir=DIR_RX; byIdType=TYPE_ID_CAN_STANDARD;byMsgType=TYPE_MSG_CAN_NON_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxSTDMsgCount;
            }
            else
            {
                //Case3:(eDir=DIR_RX; byIdType=TYPE_ID_CAN_STANDARD;byMsgType=TYPE_MSG_CAN_NON_ALL
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxSTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxSTDMsgCount;
            }
        }
        else if( byIdType == TYPE_ID_CAN_EXTENDED )
        {
            if( byMsgType == TYPE_MSG_CAN_RTR )
            {
                //Case4:(eDir=DIR_RX; byIdType=TYPE_ID_CAN_EXTENDED;byMsgType=TYPE_MSG_CAN_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxEXTD_RTRMsgCount;
            }
            else if( byMsgType == TYPE_MSG_CAN_NON_RTR)
            {
                //Case5:(eDir=DIR_RX; byIdType=TYPE_ID_CAN_EXTENDED;byMsgType=TYPE_MSG_CAN_NON_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxEXTDMsgCount;
            }
            else
            {
                //Case6:(eDir=DIR_RX; byIdType=TYPE_ID_CAN_EXTENDED;byMsgType=TYPE_MSG_CAN_ALL
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxEXTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxEXTDMsgCount;
            }
        }
        else
        {
            if( byMsgType == TYPE_MSG_CAN_RTR )
            {
                //Case7:(eDir=DIR_RX; byIdType=TYPE_ID_CAN_ALL;byMsgType=TYPE_MSG_CAN_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxSTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxEXTD_RTRMsgCount;
            }
            else if( byMsgType == TYPE_MSG_CAN_NON_RTR)
            {
                //Case8:(eDir=DIR_RX; byIdType=TYPE_ID_CAN_ALL;byMsgType=TYPE_MSG_CAN_NON_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxSTDMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxEXTDMsgCount;
            }
            else
            {
                //Case9:(eDir=DIR_RX; byIdType=TYPE_ID_CAN_ALL;byMsgType=TYPE_MSG_CAN_ALL
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxSTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxSTDMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxEXTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxEXTDMsgCount;
            }
        }
    }

    else if( eDir == DIR_TX )
    {
        if( byIdType == TYPE_ID_CAN_STANDARD )
        {
            if( byMsgType == TYPE_MSG_CAN_RTR )
            {
                //Case10:eDir=DIR_TX; byIdType=TYPE_ID_CAN_STANDARD;byMsgType=TYPE_MSG_CAN_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unTxSTD_RTRMsgCount;
            }
            else if( byMsgType == TYPE_MSG_CAN_NON_RTR)
            {
                //Case11:eDir=DIR_TX; byIdType=TYPE_ID_CAN_STANDARD;byMsgType=TYPE_MSG_CAN_NON_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unTxSTDMsgCount;
            }
            else
            {
                //Case12:eDir=DIR_TX; byIdType=TYPE_ID_CAN_STANDARD;byMsgType=TYPE_MSG_CAN_ALL
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unTxSTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxSTDMsgCount;
            }
        }
        else if( byIdType == TYPE_ID_CAN_EXTENDED )
        {
            if( byMsgType == TYPE_MSG_CAN_RTR )
            {
                //Case13:eDir=DIR_TX; byIdType=TYPE_ID_CAN_EXTEND;byMsgType=TYPE_MSG_CAN_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unTxEXTD_RTRMsgCount;
            }
            else if( byMsgType == TYPE_MSG_CAN_NON_RTR)
            {
                //Case14:eDir=DIR_TX; byIdType=TYPE_ID_CAN_EXTEND;byMsgType=TYPE_MSG_CAN_NON_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unTxEXTDMsgCount;
            }
            else
            {
                //Case15:eDir=DIR_TX; byIdType=TYPE_ID_CAN_EXTEND;byMsgType=TYPE_MSG_CAN_ALL
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unTxEXTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxEXTDMsgCount;
            }
        }
        else
        {
            if( byMsgType == TYPE_MSG_CAN_RTR )
            {
                //Case16:eDir=DIR_TX; byIdType=TYPE_ID_CAN_ALL;byMsgType=TYPE_MSG_CAN_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unTxSTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxEXTD_RTRMsgCount;
            }
            else if( byMsgType == TYPE_MSG_CAN_NON_RTR)
            {
                //Case17:eDir=DIR_TX; byIdType=TYPE_ID_CAN_ALL;byMsgType=TYPE_MSG_CAN_NON_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unTxSTDMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxEXTDMsgCount;
            }
            else
            {
                //Case18:eDir=DIR_TX; byIdType=TYPE_ID_CAN_ALL;byMsgType=TYPE_MSG_CAN_ALL
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unTotalTxMsgCount;
            }
        }
    }


    else
    {
        if( byIdType == TYPE_ID_CAN_STANDARD )
        {
            if( byMsgType == TYPE_MSG_CAN_NON_RTR )
            {
                //Case19:eDir=DIR_ALL; byIdType=TYPE_ID_CAN_STANDARD;byMsgType=TYPE_MSG_CAN_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxSTDMsgCount+
                            m_sBusStatistics[unChannelIndex].m_unTxSTDMsgCount;

            }
            else if( byMsgType == TYPE_MSG_CAN_RTR)
            {
                //Case20:eDir=DIR_ALL; byIdType=TYPE_ID_CAN_STANDARD;byMsgType=TYPE_MSG_CAN_NON_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxSTD_RTRMsgCount+
                            m_sBusStatistics[unChannelIndex].m_unRxSTD_RTRMsgCount;
            }
            else
            {
                //Case21:eDir=DIR_ALL; byIdType=TYPE_ID_CAN_STANDARD;byMsgType=TYPE_MSG_CAN_ALL
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxSTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxSTDMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxSTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxSTDMsgCount;
            }
        }
        else if( byIdType == TYPE_ID_CAN_EXTENDED )
        {
            if( byMsgType == TYPE_MSG_CAN_RTR )
            {
                //Case22:eDir=DIR_ALL; byIdType=TYPE_ID_CAN_EXTENDED;byMsgType=TYPE_MSG_CAN_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxEXTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxEXTD_RTRMsgCount;
            }
            else if( byMsgType == TYPE_MSG_CAN_NON_RTR)
            {
                //Case23:eDir=DIR_ALL; byIdType=TYPE_ID_CAN_EXTENDED;byMsgType=TYPE_MSG_CAN_NON_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxEXTDMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxEXTDMsgCount;
            }
            else
            {
                //Case24:eDir=DIR_ALL; byIdType=TYPE_ID_CAN_EXTENDED;byMsgType=TYPE_MSG_CAN_ALL
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxEXTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxEXTDMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxEXTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxEXTDMsgCount;
            }
        }
        else
        {
            if( byMsgType == TYPE_MSG_CAN_RTR )
            {
                //Case25:eDir=DIR_ALL; byIdType=TYPE_ID_CAN_ALL;byMsgType=TYPE_MSG_CAN_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxSTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxEXTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxSTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxEXTD_RTRMsgCount;
            }
            else if( byMsgType == TYPE_MSG_CAN_NON_RTR)
            {
                //Case26:eDir=DIR_ALL; byIdType=TYPE_ID_CAN_ALL;byMsgType=TYPE_MSG_CAN_NON_RTR
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxSTDMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxEXTDMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxSTDMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxEXTDMsgCount;
            }
            else
            {
                //Case27:eDir=DIR_ALL; byIdType=TYPE_ID_CAN_ALL;byMsgType=TYPE_MSG_CAN_ALL
                nMsgCount = m_sBusStatistics[unChannelIndex].m_unRxSTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxSTDMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxEXTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unRxEXTDMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxSTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxSTDMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxEXTD_RTRMsgCount +
                            m_sBusStatistics[unChannelIndex].m_unTxEXTDMsgCount;
            }
        }
    }
    LeaveCriticalSection(&m_omCritSecBS);
    return 0;
}

HRESULT CBusStatisticCAN::getTotalErrorCount(UINT unChannelIndex, Direction eDir,
        UINT& nErrCount)
{
    EnterCriticalSection(&m_omCritSecBS);

    //eDir == DIR_RX
    if( DIR_RX == eDir )
    {
        nErrCount = m_sBusStatistics[ unChannelIndex ].m_unErrorRxCount;
    }
    //eDir == DIR_TX
    else if( DIR_TX == eDir )
    {
        nErrCount = m_sBusStatistics[ unChannelIndex ].m_unErrorTxCount;
    }
    //eDir == DIR_ALL
    else
    {
        nErrCount = m_sBusStatistics[ unChannelIndex ].m_unErrorTotalCount;
    }
    LeaveCriticalSection(&m_omCritSecBS);
    return 0;
}

HRESULT CBusStatisticCAN::getAverageMessageCountPerSecond(UINT unChannelIndex,
        Direction eDir, BYTE byIdType, double& dMsgRate)
{

    /*-----------------------------------------------------------------------------
        S.no         eDir          byIdType
    ------------------------------------------------
        1.         DIR_RX       TYPE_ID_CAN_STANDARD
        2.         DIR_RX       TYPE_ID_CAN_EXTENDED
        3.         DIR_RX       TYPE_ID_CAN_ALL
        4.         DIR_TX       TYPE_ID_CAN_STANDARD
        5.         DIR_TX       TYPE_ID_CAN_EXTENDED
        6.         DIR_TX       TYPE_ID_CAN_ALL
        7.         DIR_ALL      TYPE_ID_CAN_STANDARD
        8.         DIR_ALL      TYPE_ID_CAN_EXTENDED
        9.         DIR_ALL      TYPE_ID_CAN_ALL
    -----------------------------------------------------------------------------*/

    EnterCriticalSection(&m_omCritSecBS);

    if( eDir == DIR_RX )
    {
        if( byIdType == TYPE_ID_CAN_STANDARD)
        {
            //eDir == DIR_RX, byIdType = TYPE_ID_CAN_STANDARD
            dMsgRate = m_sBusStatistics[unChannelIndex].m_dRxSTDMsgRate;
        }
        else if( byIdType == TYPE_ID_CAN_EXTENDED )
        {
            //eDir == DIR_RX, byIdType = TYPE_ID_CAN_EXTENDED
            dMsgRate = m_sBusStatistics[unChannelIndex].m_dRxEXTMsgRate;
        }
        else
        {
            dMsgRate = m_sBusStatistics[unChannelIndex].m_dTotalRxMsgRate;
        }
    }
    else if( eDir == DIR_TX )
    {
        //eDir == DIR_TX, byIdType = TYPE_ID_CAN_STANDARD
        if( byIdType == TYPE_ID_CAN_STANDARD )
        {
            dMsgRate = m_sBusStatistics[unChannelIndex].m_dTxSTDMsgRate;
        }
        //eDir == DIR_TX, byIdType = TYPE_ID_CAN_EXTENDED
        else if( byIdType == TYPE_ID_CAN_EXTENDED )
        {
            dMsgRate = m_sBusStatistics[unChannelIndex].m_dTxEXTMsgRate;
        }
        //eDir == DIR_TX, byIdType = TYPE_ID_CAN_ALL
        else
        {
            dMsgRate = m_sBusStatistics[unChannelIndex].m_dTotalTxMsgRate;
        }
    }
    else
    {
        //eDir == DIR_ALL, byIdType = TYPE_ID_CAN_STANDARD
        if( byIdType == TYPE_ID_CAN_STANDARD )
        {
            dMsgRate = m_sBusStatistics[unChannelIndex].m_dTxSTDMsgRate +
                       m_sBusStatistics[unChannelIndex].m_dRxSTDMsgRate;
        }
        //eDir == DIR_ALL, byIdType = TYPE_ID_CAN_EXTENDED
        else if( byIdType == TYPE_ID_CAN_EXTENDED )
        {
            dMsgRate = m_sBusStatistics[unChannelIndex].m_dTxEXTMsgRate +
                       m_sBusStatistics[unChannelIndex].m_dRxEXTMsgRate;
        }
        //eDir == DIR_ALL, byIdType = TYPE_ID_CAN_ALL
        else
        {
            dMsgRate = m_sBusStatistics[unChannelIndex].m_unMsgPerSecond;
        }
    }
    // dMsgRate = (double)(dMsgRate / m_dDiffTime);
    LeaveCriticalSection(&m_omCritSecBS);
    return 0L;
}

HRESULT CBusStatisticCAN::getAverageErrorCountPerSecond(UINT unChannelIndex,
        Direction eDir, double& dErrCount)
{
    EnterCriticalSection(&m_omCritSecBS);

    //eDir == DIR_RX
    if( eDir == DIR_RX )
    {
        dErrCount = m_sBusStatistics[unChannelIndex].m_dErrorRxRate;
    }
    //eDir == DIR_TX
    else if( eDir == DIR_TX )
    {
        dErrCount = m_sBusStatistics[unChannelIndex].m_dErrorTxRate;
    }
    //eDir == DIR_ALL
    else
    {
        dErrCount = m_sBusStatistics[unChannelIndex].m_dErrorRate;
    }
    LeaveCriticalSection(&m_omCritSecBS);
    return 0L;
}

HRESULT CBusStatisticCAN::getBusLoad(UINT unChannelIndex, eLOAD eLoad, double& dBusLoad)
{
    EnterCriticalSection(&m_omCritSecBS);

    //eLoad = AVERAGE
    if( eLoad == AVERAGE )
    {
        dBusLoad = m_sBusStatistics[unChannelIndex].m_dAvarageBusLoad;
    }
    //eLoad = PEAK
    else if( eLoad == PEAK )
    {
        dBusLoad = m_sBusStatistics[unChannelIndex].m_dPeakBusLoad;
    }
    //eLoad = CURRENT
    else
    {
        dBusLoad = m_sBusStatistics[unChannelIndex].m_dBusLoad;
    }
    LeaveCriticalSection(&m_omCritSecBS);
    return 0L;
}

HRESULT CBusStatisticCAN::getErrorCounter(UINT unChannelIndex, Direction eDir,
        eLOAD eLoad, UCHAR& ucErrCounter)
{
    EnterCriticalSection(&m_omCritSecBS);

    if( eDir == DIR_RX )
    {
        //eDir == DIR_RX; eLoad == CURRENT
        if( eLoad == CURRENT )
        {
            ucErrCounter = m_sBusStatistics[unChannelIndex].m_ucRxErrorCounter;
        }
        //eDir == DIR_RX; eLoad == PEAK
        else if( eLoad == PEAK )
        {
            ucErrCounter = m_sBusStatistics[unChannelIndex].m_ucRxPeakErrorCount;
        }
    }
    else if( eDir == DIR_TX )
    {
        //eDir == DIR_TX; eLoad == CURRENT
        if( eLoad == CURRENT )
        {
            ucErrCounter = m_sBusStatistics[unChannelIndex].m_ucTxErrorCounter;
        }
        //eDir == DIR_TX; eLoad == PEAK
        else if( eLoad == PEAK )
        {
            ucErrCounter = m_sBusStatistics[unChannelIndex].m_ucTxPeakErrorCount;
        }
    }
    else
    {
        //eDir == DIR_ALL; eLoad == CURRENT
        if( eLoad == CURRENT )
        {
            ucErrCounter = m_sBusStatistics[unChannelIndex].m_ucRxErrorCounter +
                           m_sBusStatistics[unChannelIndex].m_ucTxErrorCounter;
        }
        //eDir == DIR_ALL; eLoad == PEAK
        else if( eLoad == PEAK )
        {
            ucErrCounter = m_sBusStatistics[unChannelIndex].m_ucRxPeakErrorCount +
                           m_sBusStatistics[unChannelIndex].m_ucTxPeakErrorCount;
        }
    }
    LeaveCriticalSection(&m_omCritSecBS);
    return 0L;
}

HRESULT CBusStatisticCAN::setBaudRate(UINT unChannelIndex, double dBaudRate)
{
    EnterCriticalSection(&m_omCritSecBS);

    ASSERT(unChannelIndex < defNO_OF_CHANNELS);
    m_sBusStatistics[unChannelIndex].m_dBaudRate = dBaudRate;
    LeaveCriticalSection(&m_omCritSecBS);
    return 0L;
}

UCHAR CBusStatisticCAN::getControllerStatus(UINT unChannelIndex)
{
    return m_sBusStatistics[unChannelIndex].m_ucStatus;
}

HRESULT CBusStatisticCAN::getBusStatistics(UINT unChannelIndex, SBUSSTATISTICS& sBusStatistics)
{
    sBusStatistics = m_sBusStatistics[unChannelIndex];
    return S_OK;
}

void CALLBACK CBusStatisticCAN::onTimeWrapper(HWND, UINT, UINT_PTR, DWORD)
{
    CBusStatisticCAN* pTempClass = static_cast<CBusStatisticCAN*> (sm_pouBSCan); // cast the void pointer
    pTempClass->calculateBusParameters(); // call non-static function
}

BOOL CBusStatisticCAN::startBusStatisticsReadThread(void)
{
    m_ouReadThread.m_pBuffer = this;
    m_ouReadThread.bStartThread(BusStatisticsDataReadThreadProc);

    return TRUE;
}

void CBusStatisticCAN::initializeBusStatisticsData(void)
{
    EnterCriticalSection(&m_omCritSecBS);

    for( int nChannelIndex = 0; nChannelIndex < defNO_OF_CHANNELS; nChannelIndex++ )
    {
        // Reset whole statucture
        memset( &m_sBusStatistics[ nChannelIndex ] , 0, sizeof(SBUSSTATISTICS) );
        memset( &m_sPrevStatData[ nChannelIndex ] , 0, sizeof(SBUSSTATISTICS) );
        // Set Baud rate Manually
        m_sBusStatistics[ nChannelIndex ].m_dBaudRate = _tstof(defBAUDRATE);

        memset( &m_sSubBusStatistics[nChannelIndex], 0, sizeof(SSUBBUSSTATISTICS) );
    }


    memset( &m_unPrevStandardCount, 0, sizeof( m_unPrevStandardCount ) );
    memset( &m_unPrevExtendedCount, 0, sizeof( m_unPrevExtendedCount ) );
    memset( &m_unPrevStandardRTRCount, 0, sizeof( m_unPrevStandardRTRCount ) );
    memset( &m_unPrevExtendedRTRCount, 0, sizeof( m_unPrevExtendedRTRCount ) );
    memset( &m_unPrevErrorTotalCount, 0, sizeof( m_unPrevErrorTotalCount ) );



    m_nFactorSTDFrame     = defBITS_STD_FRAME + defBITS_INTER_FRAME;
    m_nFactorEXTDFrame    = defBITS_EXTD_FRAME + defBITS_INTER_FRAME;
    m_nFactorSTDRTRFrame  = m_nFactorSTDFrame;
    m_nFactorEXTDRTRFrame = m_nFactorEXTDFrame;
    m_nFactorErrorFrame   = defBITS_ERROR_FRAME + defBITS_INTER_FRAME;
    m_dDiffTime           = defTIME_INTERVAL;
    m_unPreviousTime = -1;
    LeaveCriticalSection(&m_omCritSecBS);
}

void CBusStatisticCAN::calculateDifferentialTime(void)
{

    // check if the previous time value is not stored. take 1.0s as initial
    // value for calculation.
    if(m_unPreviousTime != -1 )
    {
        m_dDiffTime         = CTimeManager::nCalculateCurrTimeStamp(FALSE) -
                              m_unPreviousTime;
        m_unPreviousTime += static_cast<UINT>(m_dDiffTime);
        m_dDiffTime         = m_dDiffTime / defDIV_FACT_FOR_SECOND;
    }
    else
    {
        m_unPreviousTime = CTimeManager::nCalculateCurrTimeStamp(FALSE);
    }
}

void CBusStatisticCAN::updateBusStatistics(STCANDATA& sCanData)
{
    EnterCriticalSection(&m_omCritSecBS);

    // CANFD Frame
    m_sCurrEntry = sCanData.m_uDataInfo.m_sCANMsg;

    int nCurrentChannelIndex = sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel - 1;
    INT nDLC = sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen;

    if ((nCurrentChannelIndex < 0) || (nCurrentChannelIndex > (defNO_OF_CHANNELS - 1)))
    {
        nCurrentChannelIndex = 0;   //take appropriate action
    }
    if(m_sCurrEntry.m_ucRTR == 1)
        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unDLCCount+=
            sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen;

    //is it Tx Message
    if(IS_TX_MESSAGE(sCanData.m_ucDataType))
    {
        if(IS_ERR_MESSAGE(sCanData.m_ucDataType))
        {
            m_sSubBusStatistics[nCurrentChannelIndex].m_unErrorTxCount++;
        }
        else
        {
            m_sSubBusStatistics[nCurrentChannelIndex].m_unTotalTxMsgCount++;
            if (m_sCurrEntry.m_ucRTR == 0) // Non RTR message
            {
                /* Check for CAN FD messages */
                if ( !sCanData.m_uDataInfo.m_sCANMsg.m_bCANFD )
                {
                    if (m_sCurrEntry.m_ucEXTENDED == 0)
                    {
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTxSTDMsgCount++;
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsStdMsg[nDLC];
                    }
                    else
                    {
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTxEXTDMsgCount++;
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsExdMsg[nDLC];
                    }
                }
                /* Indicates a CAN FD message */
                else
                {
                    int nIndex = (m_sCurrEntry.m_ucEXTENDED == 0) ? 0:1;
                    if ( m_sCurrEntry.m_ucEXTENDED == 0 )
                    {
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTxSTDMsgCount++;
                    }
                    else
                    {
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTxEXTDMsgCount++;
                    }

                    if ( nDLC <=8 )
                    {
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC15[nIndex][nDLC];
                    }
                    else if ( nDLC > 8  && nDLC <= 16 )
                    {
                        switch ( nDLC )
                        {
                            case 12:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC17[nIndex][0];
                                break;
                            case 16:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC17[nIndex][1];
                                break;
                        }
                    }
                    else if ( nDLC > 16 )
                    {
                        switch ( nDLC )
                        {
                            case 20:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC21[nIndex][0];
                                break;
                            case 24:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC21[nIndex][1];
                                break;
                            case 32:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC21[nIndex][2];
                                break;
                            case 48:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC21[nIndex][3];
                                break;
                            case 64:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC21[nIndex][4];
                                break;
                        }
                    }
                }
            }
            else // RTR message
            {
                if (m_sCurrEntry.m_ucEXTENDED == 0)
                {
                    m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTxSTD_RTRMsgCount++;
                    m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsStdMsg[0];
                }
                else
                {
                    m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTxEXTD_RTRMsgCount++;
                    m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsExdMsg[0];
                }
            }
        }
    }
    //is it Rx Message
    else if(IS_RX_MESSAGE(sCanData.m_ucDataType))
    {
        if(IS_ERR_MESSAGE(sCanData.m_ucDataType))
        {
            m_sSubBusStatistics[nCurrentChannelIndex].m_unErrorTxCount++;
        }
        else
        {
            m_sSubBusStatistics[nCurrentChannelIndex].m_unTotalRxMsgCount++;
            if (m_sCurrEntry.m_ucRTR == 0) // Non RTR message
            {
                /* Check for CAN FD messages */
                if ( !sCanData.m_uDataInfo.m_sCANMsg.m_bCANFD )
                {
                    if (m_sCurrEntry.m_ucEXTENDED == 0)
                    {
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unRxSTDMsgCount++;
                        //m_sSubBusStatistics[ nCurrentChannelIndex ].m_unSTDMsgBits = nDLC + floor((double)(nDLC / 5 + TYPE_STD_CONST1));
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsStdMsg[nDLC];
                    }
                    else
                    {
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unRxEXTDMsgCount++;
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsExdMsg[nDLC];
                    }
                }
                /* Indicates a CAN FD message */
                else
                {
                    int nIndex = (m_sCurrEntry.m_ucEXTENDED == 0) ? 0:1;
                    if ( m_sCurrEntry.m_ucEXTENDED == 0 )
                    {
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unRxSTDMsgCount++;
                    }
                    else
                    {
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unRxEXTDMsgCount++;
                    }

                    if ( nDLC <=8 )
                    {
                        m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC15[nIndex][nDLC];
                    }
                    else if ( nDLC > 8  && nDLC <= 16 )
                    {
                        switch ( nDLC )
                        {
                            case 12:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC17[nIndex][0];
                                break;
                            case 16:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC17[nIndex][1];
                                break;
                        }
                    }
                    else if ( nDLC > 16 )
                    {
                        switch ( nDLC )
                        {
                            case 20:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC21[nIndex][0];
                                break;
                            case 24:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC21[nIndex][1];
                                break;
                            case 32:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC21[nIndex][2];
                                break;
                            case 48:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC21[nIndex][3];
                                break;
                            case 64:
                                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsFDMsgCRC21[nIndex][4];
                                break;
                        }
                    }
                }
            }
            else // RTR message
            {
                if (m_sCurrEntry.m_ucEXTENDED == 0)
                {
                    m_sSubBusStatistics[ nCurrentChannelIndex ].m_unRxSTD_RTRMsgCount++;
                    m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsStdMsg[0];
                }
                else
                {
                    m_sSubBusStatistics[ nCurrentChannelIndex ].m_unRxEXTD_RTRMsgCount++;
                    m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsExdMsg[0];
                }
            }
        }
    }
    else
    {
        //Is it is Error
        m_sSubBusStatistics[nCurrentChannelIndex].m_unErrorTotalCount++;
        if (sCanData.m_uDataInfo.m_sErrInfo.m_ucErrType == ERROR_BUS)
        {
            // Update Statistics information
            m_sSubBusStatistics[ nCurrentChannelIndex ].m_unErrorTotalCount++;
            USHORT usErrorID = sCanData.m_uDataInfo.m_sErrInfo.m_ucReg_ErrCap /*& 0xE0*/;
            // Received message
            if (usErrorID & 0x20)
            {
                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unErrorRxCount++;
            }
            else
            {
                m_sSubBusStatistics[ nCurrentChannelIndex ].m_unErrorTxCount++;
            }
            m_sSubBusStatistics[ nCurrentChannelIndex ].m_unTotalBitsperSec += defBITS_ERR_FRAME;
        }
    }

    LeaveCriticalSection(&m_omCritSecBS);
}

void CBusStatisticCAN::calculateBusParameters(void)
{
    EnterCriticalSection(&m_omCritSecBS);
    calculateDifferentialTime();

    for(int nChannelIndex =0; nChannelIndex <defNO_OF_CHANNELS; nChannelIndex++)
    {
        m_sBusStatistics[nChannelIndex] = m_sSubBusStatistics[nChannelIndex];
        m_sBusStatistics[nChannelIndex].m_nSamples++;
        m_sBusStatistics[ nChannelIndex ].m_unTotalMsgCount =
            m_sBusStatistics[ nChannelIndex ].m_unTotalTxMsgCount +
            m_sBusStatistics[ nChannelIndex ].m_unTotalRxMsgCount;
        //***** Total Message Rate *****//
        m_sBusStatistics[ nChannelIndex ].m_unMsgPerSecond =
            m_sBusStatistics[ nChannelIndex ].m_unTotalMsgCount -
            m_sPrevStatData[ nChannelIndex ].m_unTotalMsgCount;
        m_sBusStatistics[ nChannelIndex ].m_unMsgPerSecond =
            static_cast<UINT>
            (m_sBusStatistics[ nChannelIndex ].m_unMsgPerSecond / m_dDiffTime );
        // Calculate Error Count & Rate
        m_sBusStatistics[ nChannelIndex ].m_unErrorTotalCount =
            m_sBusStatistics[ nChannelIndex ].m_unErrorRxCount +
            m_sBusStatistics[ nChannelIndex ].m_unErrorTxCount;
        m_sBusStatistics[ nChannelIndex ].m_dErrorRate =
            m_sBusStatistics[ nChannelIndex ].m_unErrorTotalCount -
            m_sPrevStatData[ nChannelIndex ].m_unErrorTotalCount;
        // Transmitted messages
        // Calculate Total Tx Message Rate
        m_sBusStatistics[nChannelIndex].m_dTotalTxMsgRate =
            (m_sBusStatistics[nChannelIndex].m_unTotalTxMsgCount -
             m_sPrevStatData[nChannelIndex].m_unTotalTxMsgCount);
        // Calculate STD Tx Message Rate
        m_sBusStatistics[ nChannelIndex ].m_dTxSTDMsgRate =
            (m_sBusStatistics[ nChannelIndex ].m_unTxSTDMsgCount -
             m_sPrevStatData[ nChannelIndex ].m_unTxSTDMsgCount);
        // Calculate Extended Tx Message Rate
        m_sBusStatistics[ nChannelIndex ].m_dTxEXTMsgRate =
            (m_sBusStatistics[ nChannelIndex ].m_unTxEXTDMsgCount -
             m_sPrevStatData[ nChannelIndex ].m_unTxEXTDMsgCount);
        // Calculate Tx Error Rate
        m_sBusStatistics[ nChannelIndex ].m_dErrorTxRate =
            (m_sBusStatistics[ nChannelIndex ].m_unErrorTxCount -
             m_sPrevStatData[ nChannelIndex ].m_unErrorTxCount);
        // Received messages
        // Calculate Total Rx Message Rate
        m_sBusStatistics[nChannelIndex].m_dTotalRxMsgRate =
            (m_sBusStatistics[nChannelIndex].m_unTotalRxMsgCount -
             m_sPrevStatData[nChannelIndex].m_unTotalRxMsgCount);
        // Calculate STD Rx Message Rate
        m_sBusStatistics[ nChannelIndex ].m_dRxSTDMsgRate =
            (m_sBusStatistics[ nChannelIndex ].m_unRxSTDMsgCount -
             m_sPrevStatData[ nChannelIndex ].m_unRxSTDMsgCount);
        // Calculate Extended Rx Message Rate
        m_sBusStatistics[ nChannelIndex ].m_dRxEXTMsgRate =
            (m_sBusStatistics[ nChannelIndex ].m_unRxEXTDMsgCount -
             m_sPrevStatData[ nChannelIndex ].m_unRxEXTDMsgCount);
        // Calculate Rx Error Rate
        m_sBusStatistics[ nChannelIndex ].m_dErrorRxRate =
            (m_sBusStatistics[ nChannelIndex ].m_unErrorRxCount -
             m_sPrevStatData[ nChannelIndex ].m_unErrorRxCount);
        SERROR_CNT sErrorCounter;
        sErrorCounter.m_ucRxErrCount = 0;
        sErrorCounter.m_ucTxErrCount = 0;

        if (m_pouDIL_CAN->getErrorCount( sErrorCounter, nChannelIndex, ERR_CNT) == S_OK)
        {
            m_sBusStatistics[ nChannelIndex ].m_ucTxErrorCounter =
                sErrorCounter.m_ucTxErrCount;
            m_sBusStatistics[ nChannelIndex ].m_ucRxErrorCounter =
                sErrorCounter.m_ucRxErrCount;
        }

        sErrorCounter.m_ucRxErrCount = 0;
        sErrorCounter.m_ucTxErrCount = 0;

        if (m_pouDIL_CAN->getErrorCount( sErrorCounter, nChannelIndex, PEAK_ERR_CNT) == S_OK)
        {
            m_sBusStatistics[ nChannelIndex ].m_ucTxPeakErrorCount=
                sErrorCounter.m_ucTxErrCount;
            m_sBusStatistics[ nChannelIndex ].m_ucRxPeakErrorCount =
                sErrorCounter.m_ucRxErrCount;
        }

        // Get the controller status
        LPARAM lParam = 0;

        if (m_pouDIL_CAN->getControllerParameters( lParam, nChannelIndex,
                HW_MODE) == S_OK)
        {
            m_sBusStatistics[ nChannelIndex ].m_ucStatus = (UCHAR)lParam;
        }

        //Bus Load
        DOUBLE dBusLoad  = m_sBusStatistics[ nChannelIndex ].m_unTotalBitsperSec / m_dDiffTime;
        m_sBusStatistics[ nChannelIndex ].m_unTotalBitsperSec = 0;
        m_sSubBusStatistics[ nChannelIndex ].m_unTotalBitsperSec = 0;

        if(dBusLoad != 0)
            dBusLoad = dBusLoad /
                       ( m_sBusStatistics[ nChannelIndex ].m_dBaudRate /** defBITS_KBUAD_RATE */);

        // Get the percentage load
        dBusLoad = dBusLoad * defMAX_PERCENTAGE_BUS_LOAD;

        // check for peak load
        if( dBusLoad > m_sBusStatistics[ nChannelIndex ].m_dPeakBusLoad )
        {
            // if peak load is greater than or equal to 100% assign it 99.99%
            if( dBusLoad > defMAX_PERCENTAGE_BUS_LOAD )
            {
                dBusLoad = defMAX_PERCENTAGE_BUS_LOAD_ALLOWED;
            }

            m_sBusStatistics[ nChannelIndex ].m_dPeakBusLoad = dBusLoad ;
        }

        m_sBusStatistics[ nChannelIndex ].m_dBusLoad = dBusLoad ;
        // Calculate avarage bus load
        m_sBusStatistics[ nChannelIndex ].m_dTotalBusLoad += dBusLoad;
        // Increament samples
        // Calculate Avarage bus load
        m_sBusStatistics[ nChannelIndex ].m_dAvarageBusLoad =
            m_sBusStatistics[ nChannelIndex ].m_dTotalBusLoad /
            m_sBusStatistics[ nChannelIndex ].m_nSamples;
        // Initialise previous values with current values.
        m_unPrevStandardCount[ nChannelIndex ] =
            m_sBusStatistics[ nChannelIndex ].m_unRxSTDMsgCount +
            m_sBusStatistics[ nChannelIndex ].m_unTxSTDMsgCount;
        m_unPrevExtendedCount[ nChannelIndex ] =
            m_sBusStatistics[ nChannelIndex ].m_unRxEXTDMsgCount +
            m_sBusStatistics[ nChannelIndex ].m_unTxEXTDMsgCount;
        m_unPrevStandardRTRCount[ nChannelIndex ] =
            m_sBusStatistics[ nChannelIndex ].m_unRxSTD_RTRMsgCount +
            m_sBusStatistics[ nChannelIndex ].m_unTxSTD_RTRMsgCount;
        m_unPrevExtendedRTRCount[ nChannelIndex ] =
            m_sBusStatistics[ nChannelIndex ].m_unRxEXTD_RTRMsgCount +
            m_sBusStatistics[ nChannelIndex ].m_unTxEXTD_RTRMsgCount;
        m_unPrevErrorTotalCount[ nChannelIndex ] =
            m_sBusStatistics[ nChannelIndex ].m_unErrorTotalCount;
        m_sBusStatistics[ nChannelIndex ].m_unDLCCount = 0;
        m_sPrevStatData[ nChannelIndex ] = m_sBusStatistics[ nChannelIndex ];
    }

    LeaveCriticalSection(&m_omCritSecBS);
}
