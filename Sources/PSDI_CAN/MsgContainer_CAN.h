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
 * @brief     Definition of CAN message container class
 * @authors   Anish Kumar, Arunkumar Karri
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CAN message container class
 */

#pragma once

/* Project includes */
#include "afxtempl.h"
#include "Utility/Utility_Thread.h"
#include "Application/MsgWndHdrInfo.h"
#include "CommonClass/GUI_FormatMsgCAN.h"
#include "include/BaseDefs.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/MsgBufCANVFSE.h"
#include "DataTypes/MsgBufVVSE.h"
#include "DataTypes/Filter_Datatypes.h"
#include "CommonClass/RefTimeKeeper.h"
#include "CommonClass/MsgContainerBase.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/DIL_Interface_extern.h"

typedef CMsgBufCANVFSE<STCANDATA> CCANBufVFSE;
typedef void (*MSG_RX_CALL_BK)(void* pParam, BusType eBusType);

typedef struct stCanDataSpl : public STCANDATA
{
    __int64      m_nDeltime;    /**< To save delta time in case of append mode */

    stCanDataSpl()
    {
        m_nDeltime = 0;
    }
} STCANDATASPL;

typedef CMsgBufCANVFSE<STCANDATASPL> CMsgBufCANVFSEspl;

class CMsgContainerCAN: public CMsgContainerBase
{
public:
    CMsgContainerCAN(void);
    ~CMsgContainerCAN(void);

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

    void vSetMsgLength(CString strMsgLength);

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
    __int64 nCreateMapIndexKey( LPVOID pMsgData );

private:
    CCANBufFSE              m_ouMCCanBufFSE;
    CCANBufVFSE             m_ouOWCanBuf;
    CMsgBufCANVFSEspl       m_ouAppendCanBuf;
    SFORMATTEDDATA_CAN      m_sOutFormattedData;
    STCANDATASPL            m_sCANReadDataSpl;
    CFormatMsgCAN           m_ouFormatCAN;
    DWORD                   m_dwClientId;
    CBaseDIL_CAN*           m_pouDIL_CAN_Interface;
    CRITICAL_SECTION m_sCritSecDataSync;
    CRITICAL_SECTION m_omCritSecFilter;
    SFILTERAPPLIED_CAN m_sFilterCAN;
    eERROR_STATE m_eCurrErrorState[ defNO_OF_CHANNELS ];

    /**
     * Process a new Rx/Tx msg
     */
    void vProcessNewData(STCANDATA& sCanData);

    BOOL bIsTransitionInState( UINT unChannel,
                               BYTE byRxError,
                               BYTE byTxError );

    BOOL bTobeBlocked(STCANDATA& sCanData);

    /**
     * Processes the current Error entry and returns the Error code.
     *
     * @return Error ID
     */
    USHORT usProcessCurrErrorEntry(SERROR_INFO& sErrInfo);
};
