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
 * @brief     Definition file for signal watch data types.
 * @author    Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for signal watch data types.
 */

#pragma once

/* Project includes */
#include "Include/BaseDefs.h"

/**
 * Database parameters
 */
typedef struct _tagDBPARAMS
{
    BusType   m_eBus;
    CString     m_omBusName;
    CString     m_omDBPath;
    CString     m_omIdFieldName;
    UINT        m_unMaxMsgLen;
    void**      m_ppvActiveDB;
    void**      m_ppvImportedDBs;
} SDBPARAMS;

static SDBPARAMS sg_asDbParams[BUS_TOTAL] =
{
    {CAN,     "CAN",    "", "Message ID :",  8,0,0},
    {MCNET,   "MCNET",  "", "Message Code :",  MAX_DATA_LEN_MCNET,0,0},
    {J1939,   "J1939",  "", "PGN :",  MAX_DATA_LEN_J1939,0,0},
    {FLEXRAY, "FLEXRAY","", "FRAME ID :",  128,0,0},
    {LIN,     "LIN",    "", "Msg ID :",  0,0,0},
    {MOST,    "MOST",   "", "Msg ID :",  0,0,0}
};

enum eWAVEFORMTYPE
{
    eWave_NONE = 0,
    eWave_SINE,
    eWave_TRIANGLE,
    eWave_COS,
    eWave_SAWTOOTH
};

struct sINTERPRETSIGNALINFO
{
    CString m_omStrSignalName;
    __int64 m_un64RawValue;
    double m_dPhyValue;
    CString m_omStrSignalDescValue;
    CString m_omStrUnit;
    unsigned char   m_ucSigLength;

    sINTERPRETSIGNALINFO(): m_omStrSignalName(""),
        m_un64RawValue(0),
        m_dPhyValue(0),
        m_omStrSignalDescValue(""),
        m_omStrUnit(""),
        m_ucSigLength(0)
    {
    }
};

typedef sINTERPRETSIGNALINFO SINTERPRETSIGNALINFO;
typedef SINTERPRETSIGNALINFO* PINTERPRETSIGNALINFO;
typedef CArray<SINTERPRETSIGNALINFO,SINTERPRETSIGNALINFO> CSignalInfoArray;

typedef union _SIG_VALUE
{
    __int64  n64Value;
    unsigned __int64  un64Value;
} SIG_VALUE;

class CSignalDescVal
{
public:
    CSignalDescVal();
    ~CSignalDescVal();
    CSignalDescVal& operator=(const CSignalDescVal& RefObj);
    void vClearNext(void);

    /**
     * Name of the Signal ID
     */
    CString m_omStrSignalDescriptor;

    SIG_VALUE m_DescValue;

    /**
     * Next Signal desc and value
     */
    CSignalDescVal* m_pouNextSignalSignalDescVal;
};

struct sWaveformInfo
{
    eWAVEFORMTYPE m_eSignalWaveType;

    /**
     * Maximum amplitude
     */
    float m_fAmplitude;

    float m_fFrequency;

    /**
     * Of each time amplitude calculation. Value depends
     * on the sampling frequency.
     */
    float m_fGranularity;

    sWaveformInfo();

    sWaveformInfo& operator=(const sWaveformInfo& RefObj);
    static CString omGetWaveformName(eWAVEFORMTYPE eWaveform);
};

struct sSigWaveMap
{
    CString m_omSigName;
    sWaveformInfo sWaveInfo;

    sSigWaveMap();

    sSigWaveMap& operator=(const sSigWaveMap& RefObj);
    BOOL operator==(const sSigWaveMap& RefObj) const;
};

typedef CList<sSigWaveMap, sSigWaveMap&> CSigWaveMapList;

struct SSigGeneration
{
    UINT m_nMsgID;
    float m_fDefaultAmplitude;
    CSigWaveMapList m_omSigWaveMapList;

    SSigGeneration();
    ~SSigGeneration();

    void Reset(void);
    BOOL operator ==(const SSigGeneration& RefObj) const;
    SSigGeneration& operator=(const SSigGeneration& RefObj);
};

typedef CList<SSigGeneration, SSigGeneration&> CSigGenerationInfoList;

struct sSIGNALS
{
    /**
     * Signal Name
     */
    CString m_omStrSignalName;

    /**
     * Signal definition start byte in Msg, Zero index
     */
    UINT m_unStartByte;

    /**
     * Signal Length, One indexed
     */
    UINT m_unSignalLength;

    /**
     * Signal start bit, Zero indexed
     */
    BYTE m_byStartBit;

    /**
     * Signal Type
     */
    BYTE m_bySignalType;

    SIG_VALUE m_SignalMinValue;

    SIG_VALUE m_SignalMaxValue;

    /**
     * Signal Multiplication Factor
     */
    float m_fSignalFactor;

    /**
     * Signal Offset value.
     */
    float m_fSignalOffset;

    /**
     * Signal Measurement Unit
     */
    CString m_omStrSignalUnit;

    /**
     * Signal endianness.
     */
    DataFormat m_eFormat;

    /**
     * Signal Type ID and value List
     */
    CSignalDescVal* m_oSignalIDVal;

    /**
     * Next Signal
     */
    sSIGNALS* m_psNextSignalList;

    /**
     * Call this function to set the data bits of a signal with the given value
     */
    static void vSetSignalValue(sSIGNALS* pouCurrSignal, UCHAR aucData[8],
                                UINT64 u64SignVal);

    /**
     * Call this function to get the bit mask for a signal. In a bit mask all
     * bits petaining to but this signal is 0.
     */
    static UINT64 un64GetBitMask(sSIGNALS* pouCurrSignal);
};


struct sMESSAGE
{
    /**
     * Message Name
     */
    CString m_omStrMessageName;

    /**
     * Message Code
     */
    UINT m_unMessageCode;

    /**
     * Number Of Signals a Message has
     */
    UINT m_unNumberOfSignals;

    /**
     * Message Length
     */
    UINT m_unMessageLength;

    /**
     * Message Frame Format
     */
    BOOL m_bMessageFrameFormat;

    /**
     * Signal associated with Message
     */
    sSIGNALS* m_psSignals;

    /**
     * Matrix to validate overlapping signals
     */
    BYTE m_bySignalMatrix[1785];

    /**
     * 1-Intel, 0-Motorola
     */
    int m_nMsgDataFormat;
};

typedef struct tagSSIGNALINFO
{
    CString     m_omSigName;
    CString     m_omRawValue;
    CString     m_omEnggValue;
    CString     m_omUnit;
} SSignalInfo;

typedef CArray<SSignalInfo, SSignalInfo> SSignalInfoArray;

typedef struct tagSMSGENTRY
{
    sMESSAGE*  m_psMsg;
    tagSMSGENTRY*    m_psNext;

    tagSMSGENTRY();
    ~tagSMSGENTRY();

    static void vClearMsgList(tagSMSGENTRY*& psMsgRoot);
    static void vClearSignalList(sSIGNALS* psSignals);

    static sMESSAGE* psCopyMsgVal(sMESSAGE* psMsg);
    static sSIGNALS* psCopySignalVal(sSIGNALS* psSignal);

    static BOOL bUpdateMsgList(tagSMSGENTRY*& Root, sMESSAGE* psMsg);
    static sSIGNALS* psCopySignalList(sSIGNALS* psSignal);
    static BOOL bGetMsgPtrFromMsgId(const tagSMSGENTRY* psRoot,UINT unMsgId, sMESSAGE*& pMsg);
} SMSGENTRY;
