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
 * @file      MsgBufFSE.h
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

const int SIZE_APP_BUFFER       = 20000;

/**
 * This is the concrete template class of a circular queue where each entry is
 * of fixed size. Implemented as a template class so as to cater to any data type.
 * Here SMSGBUFFER is the data type in operation.
 */
template <typename SMSGBUFFER>
class CMsgBufFSE : public CBaseMsgBufFSE<SMSGBUFFER>
{
public:
    CMsgBufFSE();
    ~CMsgBufFSE();

	/**
	 * @brief To read an entry from the circular queue
	 * @param[out] psMsgBuffer The target message entry.
	 * @return EMPTY_APP_BUFFER if buffer is empty; else CALL_SUCCESS.
	 *
	 * Reads a message entry and advances the read index. On
	 * successful reading operation the present entry is
	 * invalidated to make room for a new entry.
	 */
    HRESULT readFromBuffer(SMSGBUFFER* psMsgBuffer);

	/**
	 * @brief To read an entry from the circular queue
	 * @param[out] psMsgBuffer The target message entry.
	 * @return EMPTY_APP_BUFFER if buffer is empty; else CALL_SUCCESS.
	 *
	 * Reads a message entry and advances the read index. On
	 * successful reading operation the present entry is
	 * invalidated to make room for a new entry.
	 */
	HRESULT readFromBuffer(SMSGBUFFER* psMsgBuffer, __int64 nSlotId);

	/**
	 * @brief To read an entry from the circular queue
	 * @param[out] psMsgBuffer The target message entry.
	 * @return EMPTY_APP_BUFFER if buffer is empty; else CALL_SUCCESS.
	 *
	 * Reads a message entry and advances the read index. On
	 * successful reading operation the present entry is
	 * invalidated to make room for a new entry.
	 */
	HRESULT readFromBuffer(SMSGBUFFER* psMsgBuffer, int nIndex);

	/**
	 * @brief To write an entry into the circular queue
	 * @param[in] psMsgBuffer The source message entry.
	 * @return ERR_FULL_APP_BUFFER if buffer is full; else CALL_SUCCESS.
     *
	 * Writes a message entry and advances the write index.
	 */
	HRESULT writeIntoBuffer(SMSGBUFFER* psMsgBuffer);

	/**
	 * Writes a message entry and advances the write index.
	 *
	 * @param[in] psMsgBuffer The source message entry.
	 * @return ERR_FULL_APP_BUFFER if buffer is full; else CALL_SUCCESS.
	 */
	HRESULT writeIntoBuffer(const SMSGBUFFER* psMsgBuffer, __int64 nSlotId, int& nIndex);

	/**
	 * Returns the number of unread entries in the queue.
	 *
	 * @return Number of message entries
	 */
    int getMessageCount(void) const;
    
    /**
	 * To set the current queue length
	 */
	int setBufferSize(int nMsgDataSize);

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

protected:
	/**
	 * The data buffer
	 */
    SMSGBUFFER m_asMsgBuffer[SIZE_APP_BUFFER];

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
	 * This information will be used frequently.
	 */
    int m_nMsgSize;

	/**
	 * Event to be signalled when there is at least one message
	 */
    HANDLE m_hNotifyingEvent;
};

template <typename SMSGBUFFER>
CMsgBufFSE<SMSGBUFFER>::CMsgBufFSE()
{
    m_nMsgSize = sizeof(SMSGBUFFER);
    clearMessageBuffer();
    InitializeCriticalSection(&m_CritSectionForGB);
    m_hNotifyingEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

template <typename SMSGBUFFER>
CMsgBufFSE<SMSGBUFFER>::~CMsgBufFSE()
{
    CloseHandle(m_hNotifyingEvent);
    m_hNotifyingEvent = NULL;
    DeleteCriticalSection(&m_CritSectionForGB);
}

template <typename SMSGBUFFER> void CMsgBufFSE<SMSGBUFFER>::
clearMessageBuffer(void)
{
    memset((BYTE*) m_asMsgBuffer, 0, SIZE_APP_BUFFER * m_nMsgSize);
    m_nIndexRead = 0;
    m_nIndexWrite = 0;
    m_nMsgCount = 0;
}

template <typename SMSGBUFFER> HRESULT CMsgBufFSE<SMSGBUFFER>::readFromBuffer(
    SMSGBUFFER* psMsg)
{
    int nResult = CALL_SUCCESS;

#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexRead > SIZE_APP_BUFFER));
#endif

    EnterCriticalSection(&m_CritSectionForGB); // Lock the buffer

    // Check entry indexed by m_nIndexRead. If this particular entry
    if (m_nMsgCount == 0)
    {
        nResult = EMPTY_APP_BUFFER;
    }
    else
    {
        // Copy the current entry and advance the read index by one.
        memcpy(psMsg, &(m_asMsgBuffer[m_nIndexRead++]), m_nMsgSize);
        if (m_nIndexRead == SIZE_APP_BUFFER)
        {
            m_nIndexRead = 0;
        }
        --m_nMsgCount; // Total number of to-be-read entries decremented by 1.
    }

    LeaveCriticalSection(&m_CritSectionForGB); // Unlock the buffer

    return nResult;
}

template <typename SMSGBUFFER> HRESULT CMsgBufFSE<SMSGBUFFER>::writeIntoBuffer(
    SMSGBUFFER* psMsg)
{
    int nResult = CALL_SUCCESS;

#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexWrite > SIZE_APP_BUFFER));
#endif

    EnterCriticalSection(&m_CritSectionForGB); // Lock the buffer

    if (m_nMsgCount == SIZE_APP_BUFFER) // Check for buffer overflow
    {
        nResult = ERR_FULL_APP_BUFFER;
    }
    else
    {
        // Write the source entry and advance the write index by one.
        memcpy (&(m_asMsgBuffer[m_nIndexWrite++]), psMsg, m_nMsgSize);
        if (m_nIndexWrite == SIZE_APP_BUFFER)
        {
            m_nIndexWrite = 0;
        }
        ++m_nMsgCount; // Total number of to-be-read entries incremented by 1.
        SetEvent(m_hNotifyingEvent); // Notify addition of an entry.
    }

    LeaveCriticalSection(&m_CritSectionForGB); // Unlock the buffer

    return nResult;
}

template <typename SMSGBUFFER> int CMsgBufFSE<SMSGBUFFER>::
getMessageCount(void) const
{
    return m_nMsgCount;
}

template <typename SMSGBUFFER> HANDLE CMsgBufFSE<SMSGBUFFER>::
getNotifyEvent(void) const
{
    return m_hNotifyingEvent;
}

template <typename SMSGBUFFER>
HRESULT CMsgBufFSE<SMSGBUFFER>::readFromBuffer(SMSGBUFFER* /*psMsgBuffer*/, __int64 /*nSlotId*/)
{
    return ERR_NOT_SUPPORTED;
}

template <typename SMSGBUFFER>
HRESULT CMsgBufFSE<SMSGBUFFER>::readFromBuffer(SMSGBUFFER* /*psMsgBuffer*/, int /*nIndex*/)
{
    return ERR_NOT_SUPPORTED;
}

template <typename SMSGBUFFER>
HRESULT CMsgBufFSE<SMSGBUFFER>::writeIntoBuffer(const SMSGBUFFER* /*psMsgBuffer*/,
        __int64 /*nSlotId*/, int& /*nIndex*/)
{
    return ERR_NOT_SUPPORTED;
}

template <typename SMSGBUFFER>
int CMsgBufFSE<SMSGBUFFER>::setBufferSize(int /*nMsgDataSize*/)
{
    return ERR_NOT_SUPPORTED;
}
