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
 * @brief     Definition of J1939 message container class
 * @authors   Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of J1939 message container class
 */

#pragma once

/* Project includes */
#include "afxtempl.h"
#include "Utility/Utility_Thread.h"
#include "Application/MsgWndHdrInfo.h"
#include "CommonClass/GUI_FormatMsgJ1939.h"
#include "include/BaseDefs.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/Filter_Datatypes.h"
#include "CommonClass/RefTimeKeeper.h"
#include "CommonClass/MsgContainerBase.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/BaseDIL_J1939.h"
#include "DIL_Interface/DIL_Interface_extern.h"
#include "DataTypes/MsgBufVVSE.h"

typedef void (*MSG_RX_CALL_BK)(void* pParam, BusType eBusType);

typedef struct stJ1939MsgSpl: public STJ1939_MSG
{
    __int64      m_nDeltime;    /**< To save delta time in case of append mode */

    stJ1939MsgSpl()
    {
        m_nDeltime = 0;
    }

    void vGetDataStream(BYTE* pbyData) const;

    int nGetSize() const;

    void vCopyFromJ1939Msg(const STJ1939_MSG& ouSource, UINT64 u64DelTS);
} STJ1939MSGSPL;

class CMsgContainerJ1939: public CMsgContainerBase
{
public:
    CMsgContainerJ1939(void);
    ~CMsgContainerJ1939(void);

    /**
     * Initialize the reference time
     */
    void InitTimeParams(void);

    /**
     * Read data from DIL buffer
     */
    void vRetrieveDataFromBuffer();

    /**
     * Get Dil interface pointer
     */
    BOOL bGetDilInterFace();

    /**
     * Clear all the storage for UI
     */
    void vEditClearAll();

    int nGetAppendBufferCount();

    int nGetOWBufferCount();

    /**
     * Start the read thread
     */
    BOOL bStartReadThread();

    /**
     * Stop the read thread
     */
    BOOL bStopReadThread();

    /**
     * Provide format data structure pointers to the user
     */
    void vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL& sHdrColStruct,
                                     char* pomDataPtrArr[MAX_MSG_WND_COL_CNT],
                                     BYTE bExprnFlag_Disp);

    HRESULT hReadFromOWBuffer(void* psMsg, __int64 nMapIndex);

    HRESULT hReadFromAppendBuffer(void* psMsg, int nMsgIndex);

    /**
     * Format the requested Msg and save it in Format data
     * structure which is accessible from the User module
     */
    HRESULT hUpdateFormattedMsgStruct(int nListIndex, int& nMsgCode,
                                      BYTE bExprnFlag_Disp, __int64 nTimeOffset = 0);

    /**
     * Current msg name from DB
     */
    void vSetCurrMsgName(CString strMsgNameOrCode);

    void vSetMsgLength(CString strsgLength);

    /**
     * Clear format data structure pointers
     */
    void vClearFormattedMsgStruct();

    /**
     * Save to OW buffer and provide the details requested
     * by receive child/ dll user class
     */
    void vSaveOWandGetDetails(void* pMsg, __int64& dwMapIndex,
                              __int64& dwTimeStamp, UINT& nMsgCode, int& nBufferIndex );

    void vResetFormattedData();

    void SetClientID(DWORD dwClientID);

    void DoSortBuffer(int nField,bool bAscending);

    void GetMapIndexAtID(int nIndex,__int64& nMapIndex);

    HRESULT hToggleDILBufferRead(BOOL bRead);

    HRESULT ApplyFilterScheme(void* pvFilterApplied);

    HRESULT GetFilterScheme(void* pvFilterApplied);

    HRESULT EnableFilterApplied(BOOL bEnable);

    /**
     * Creates a key for SmsgDispEntry map
     */
    __int64 nCreateMapIndexKey(LPVOID pMsgData);

private:
    CMsgBufVSE              m_ouVSEBufJ1939;
    CJ1939BufVFSE           m_ouOWJ1939Buf;
    CMsgBufVVSE             m_ouAppendJ1939Buf;
    STJ1939MSGSPL           m_sJ1939ReadMsgSpl;
    SFORMATTEDATA_J1939     m_sOutFormattedData;
    CFormatMsgJ1939         m_ouFormatJ1939;
    STJ1939_MSG             m_sJ1939Data;
    BYTE*                   m_pbyJ1939Data;
    DWORD                   m_dwClientId;
    CBaseDILI_J1939*        m_pouDIL_J1939;
    CRITICAL_SECTION m_sCritSecDataSync;
    CRITICAL_SECTION m_omCritSecFilter;
    SFILTERAPPLIED_J1939 m_sFilterJ1939;

    /**
     * Process a new Rx/Tx msg
     */
    void vProcessNewData(STJ1939_MSG& sJ1939Msg);

    USHORT ushCalculateStrLen(bool bForHex, USHORT ushLength);

    /**
     * Check for the block status of J1939 message
     */
    BOOL bTobeBlocked(STJ1939_MSG& sJ1939Data);
};
