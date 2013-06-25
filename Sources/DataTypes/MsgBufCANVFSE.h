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
 * @brief     Defines and implements the template class for circular queue
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines and implements the template class for circular queue
 */

#pragma once

/* Project includes */
#include "include/Error.h"
#include "BaseMsgBufAll.h"

const int SIZE_APP_CAN_BUFFER = 5000;

/**
 * This is the concrete template class of a circular queue where each entry is
 * of fixed size. Implemented as a template class so as to cater to any data type.
 * Here SMSGBUFFER is the data type in operation.
 */
template <typename SMSGBUFFER>
class CMsgBufCANVFSE
{
public:
    CMsgBufCANVFSE();
    ~CMsgBufCANVFSE();

    /**
     * Reads a message entry and advances the read index. On
     * successful reading operation the present entry is
     * invalidated to make room for a new entry.
     *
     * @param[out] psMsgBuffer The target message entry.
     * @return EMPTY_APP_BUFFER if buffer is empty; else CALL_SUCCESS.
     */
    int readFromBuffer(SMSGBUFFER* psMsgBuffer,__int64 nSlotId);

    /**
     * Reads a message entry and advances the read index. On
     * successful reading operation the present entry is
     * invalidated to make room for a new entry.
     *
     * @param[out] psMsgBuffer The target message entry.
     * @return EMPTY_APP_BUFFER if buffer is empty; else CALL_SUCCESS.
     */
    int readFromBuffer(SMSGBUFFER* psMsgBuffer,int nIndex);

    /**
     * Writes a message entry and advances the write index.
     *
     * @param[in] psMsg The source message entry.
     * @return ERR_FULL_APP_BUFFER if buffer is full; else CALL_SUCCESS.
     */
    int writeIntoBuffer(const SMSGBUFFER* psMsg, __int64 nSlotId, int& nIndex);

    /**
     * Writes a message entry and advances the write index.
     *
     * @param[in] psMsg The source message entry.
     * @return ERR_FULL_APP_BUFFER if buffer is full; else CALL_SUCCESS.
     */
    int writeIntoBuffer(const SMSGBUFFER* psMsg);

    /**
     * Returns the number of unread entries in the queue.
     *
     * @return Number of message entries
     */
    int getBufferLength(void) const;

    /**
     * Clears the message buffer and resets the indices.
     */
    void clearMessageBuffer(void);

    /**
     * Returns handle of the event that gets signalled when
     * a message entry is added.
     *
     * @return The notifying event handle
     */
    HANDLE getNotifyEvent(void) const;

    /**
     * Reorders the list according to the sorting key specified.
     *
     * @param[in] nField The field to be used as the sorting key.
     */
    void vDoSortBuffer(int nField,bool bAscending);

    /**
     * Reorders the Index Map Array according to the order specified.
     */
    void vDoSortIndexMapArray();

    /**
     * Returns the Slot ID of the index specified in m_omIdIndexMap.
     *
     * @param[in] nIndex The Index at which the SlotID needs to be pickef from.
     */
    void nGetMapIndexAtID(int nIndex,__int64& nMapIndex);

protected:
    /**
     * The data buffer
     */
    SMSGBUFFER m_asMsgBuffer[SIZE_APP_CAN_BUFFER];

    /**
     * To make it thread safe
     */
    CRITICAL_SECTION m_CritSectionForGB;

    /**
     * Current read index
     */
    int m_nIndexRead;

    /**
     * Current write index
     */
    int m_nIndexWrite;

    /**
     * Number of message entries
     */
    int m_nMsgCount;

    /**
     * At the beginning we need to store size of a message entry.
     * This information will be used frequently
     */
    int m_nMsgSize;

    /**
     * Event to be signalled when there is at least one message
     */
    HANDLE m_hNotifyingEvent;

private:
    CMap<__int64, __int64, int, int> m_omIdIndexMap;
};

template <typename SMSGBUFFER>
CMsgBufCANVFSE<SMSGBUFFER>::CMsgBufCANVFSE()
{
    m_nMsgSize = sizeof(SMSGBUFFER);
    clearMessageBuffer();
    InitializeCriticalSection(&m_CritSectionForGB);
    m_hNotifyingEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

template <typename SMSGBUFFER>
CMsgBufCANVFSE<SMSGBUFFER>::~CMsgBufCANVFSE()
{
    CloseHandle(m_hNotifyingEvent);
    m_hNotifyingEvent = NULL;
    DeleteCriticalSection(&m_CritSectionForGB);
}

template <typename SMSGBUFFER> void CMsgBufCANVFSE<SMSGBUFFER>::
clearMessageBuffer(void)
{
    memset((BYTE*) m_asMsgBuffer, 0, SIZE_APP_CAN_BUFFER * m_nMsgSize);
    m_omIdIndexMap.RemoveAll();
    m_nIndexRead = 0;
    m_nIndexWrite = 0;
    m_nMsgCount = 0;
}

template <typename SMSGBUFFER>
int CMsgBufCANVFSE<SMSGBUFFER>::readFromBuffer(SMSGBUFFER* psMsg, int nIndex)
{
    HRESULT nResult = S_OK;

#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexRead > SIZE_APP_CAN_BUFFER));
#endif

    // Lock the buffer
    EnterCriticalSection(&m_CritSectionForGB);

    // Check entry indexed by m_nIndexRead. If this particular entry
    if (m_nMsgCount == 0)
    {
        nResult = EMPTY_APP_BUFFER;
    }
    else if (nIndex >= SIZE_APP_CAN_BUFFER)
    {
        nResult = ERR_INVALID_INDEX;
    }
    else
    {
        INT TempIndex = m_nIndexRead + nIndex;
        if (m_nIndexRead + nIndex >= SIZE_APP_CAN_BUFFER)
        {
            TempIndex -= SIZE_APP_CAN_BUFFER;
        }
        *psMsg = m_asMsgBuffer[TempIndex];
    }
    // Unlock the buffer
    LeaveCriticalSection(&m_CritSectionForGB);

    return nResult;
}

template <typename SMSGBUFFER>
int CMsgBufCANVFSE<SMSGBUFFER>::readFromBuffer(SMSGBUFFER* psMsg, __int64 nSlotId)
{
    HRESULT nResult = CALL_SUCCESS;
    int nIndex;

#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexRead > SIZE_APP_CAN_BUFFER));
#endif

    // Lock the buffer
    EnterCriticalSection(&m_CritSectionForGB);

    // Check entry indexed by m_nIndexRead. If this particular entry
    if (m_nMsgCount == 0)
    {
        nResult = EMPTY_APP_BUFFER;
    }
    else
    {
        if (m_omIdIndexMap.Lookup(nSlotId, nIndex))
        {
            *psMsg = m_asMsgBuffer[nIndex];
        }
        else
        {
            nResult = ERR_INVALID_SLOT;
        }
    }
    // Unlock the buffer
    LeaveCriticalSection(&m_CritSectionForGB);

    return nResult;
}

template <typename SMSGBUFFER>
int CMsgBufCANVFSE<SMSGBUFFER>::writeIntoBuffer(const SMSGBUFFER* psMsg,
        __int64 nSlotId, int& nIndex)

{
    int nResult = CALL_SUCCESS;

#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexWrite > SIZE_APP_CAN_BUFFER));
#endif

    EnterCriticalSection(&m_CritSectionForGB); // Lock the buffer

    if (m_nMsgCount == SIZE_APP_CAN_BUFFER) // Check for buffer overflow
    {
        nResult = ERR_FULL_APP_BUFFER;
    }
    else
    {
        if (m_omIdIndexMap.Lookup(nSlotId, nIndex))
        {
            m_asMsgBuffer[nIndex] = *psMsg;
        }
        else
        {
            nIndex = m_nMsgCount;
            m_asMsgBuffer[m_nMsgCount] = *psMsg;
            m_omIdIndexMap[nSlotId] = m_nMsgCount;
            ++m_nMsgCount;
        }
        SetEvent(m_hNotifyingEvent);
    }

    LeaveCriticalSection(&m_CritSectionForGB); // Unlock the buffer

    return nResult;
}

template <typename SMSGBUFFER>
int CMsgBufCANVFSE<SMSGBUFFER>::writeIntoBuffer(const SMSGBUFFER* psMsg)

{
    int nResult = CALL_SUCCESS;

#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexWrite > SIZE_APP_CAN_BUFFER));
#endif

    EnterCriticalSection(&m_CritSectionForGB); // Lock the buffer

    if (m_nMsgCount == SIZE_APP_CAN_BUFFER) // Check for buffer overflow
    {
        m_asMsgBuffer[m_nIndexRead] = *psMsg;
        m_nIndexRead++;
        if (m_nIndexRead == SIZE_APP_CAN_BUFFER)
        {
            m_nIndexRead = 0;
        }
        nResult = ERR_FULL_APP_BUFFER;
    }
    else
    {
        m_asMsgBuffer[m_nMsgCount] = *psMsg;
        ++m_nMsgCount;
        SetEvent(m_hNotifyingEvent);
    }

    LeaveCriticalSection(&m_CritSectionForGB); // Unlock the buffer

    return nResult;
}

template <typename SMSGBUFFER> int CMsgBufCANVFSE<SMSGBUFFER>::
getBufferLength(void) const
{
    return m_nMsgCount;
}

template <typename SMSGBUFFER> HANDLE CMsgBufCANVFSE<SMSGBUFFER>::
getNotifyEvent(void) const
{
    return m_hNotifyingEvent;
}

template <typename SMSGBUFFER> void CMsgBufCANVFSE<SMSGBUFFER>::
vDoSortBuffer(int nField,bool bAscending)
{
    SMSGBUFFER::vSetSortField(nField);
    SMSGBUFFER::vSetSortAscending(bAscending);
    qsort((void*) m_asMsgBuffer, (size_t) getBufferLength(),
          sizeof(SMSGBUFFER), SMSGBUFFER::DoCompareIndiv);
    //After sorting Start index has to be reset
    m_nIndexRead = 0;

    //in case of Append Mode the count will be 0
    if(m_omIdIndexMap.GetCount() ==0)
    {
        return;
    }

    vDoSortIndexMapArray();
}

template <typename SMSGBUFFER> void CMsgBufCANVFSE<SMSGBUFFER>::
vDoSortIndexMapArray()
{
    for(int nCnt = 0; nCnt< m_omIdIndexMap.GetCount(); nCnt++)
    {
        __int64 nSlotID = SMSGBUFFER::GetSlotID(m_asMsgBuffer[nCnt]);
        m_omIdIndexMap.SetAt(nSlotID, nCnt);
    }
}

template <typename SMSGBUFFER> void CMsgBufCANVFSE<SMSGBUFFER>::
nGetMapIndexAtID(int nIndex,__int64& nMapIndex)
{
    POSITION pos = m_omIdIndexMap.GetStartPosition();
    int nLocalIndex=0;
    __int64 nSlotID;
    while( pos != NULL )
    {
        m_omIdIndexMap.GetNextAssoc(pos, nSlotID, nLocalIndex);
        if(nIndex == nLocalIndex)
        {
            nMapIndex = nSlotID;
            return;
        }
    }
}
