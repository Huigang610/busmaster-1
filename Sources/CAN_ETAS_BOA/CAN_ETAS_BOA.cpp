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
 * @brief     Source file for BOA DIL functions
 * @author    Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for BOA DIL functions
 * Defines the initialization routines for the DLL.
 */

/* C includes */
#include <search.h>             /* For qsort */

/* C++ includes */
#include <string>
#include <vector>

/* Project includes */
#include "CAN_ETAS_BOA_stdafx.h"
#include "CAN_ETAS_BOA.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "Include/BaseDefs.h"
#include "Include/DIL_CommonDefs.h"
#include "Include/Can_Error_Defs.h"
#include "DIL_Interface/BaseDIL_CAN_Controller.h"
#include "HardwareListing.h"
#include "ChangeRegisters_CAN_ETAS_BOA.h"
#include "../Application/MultiLanguage.h"
#include "Utility/MultiLanguageSupport.h"
#define USAGE_EXPORT
#include "CAN_ETAS_BOA_Extern.h"

/* ETAS BOA includes */
#if BOA_VERSION == BOA_VERSION_1_4
#include "EXTERNAL/BOA 1.4/Include/OCI/ocican.h"
#include "EXTERNAL/BOA 1.4/Include/CSI/csisfs.h"
#elif BOA_VERSION == BOA_VERSION_1_5
#include "EXTERNAL/BOA 1.5/Include/OCI/ocican.h"
#include "EXTERNAL/BOA 1.5/Include/CSI/csisfs.h"
#ifdef BOA_VERSION_1_5_FD
#include "EXTERNAL/BOA 1.5/OCI/ocicanfd.h"
#endif
#elif BOA_VERSION == BOA_VERSION_1_6
#include "EXTERNAL/BOA 1.6/Include/OCI/ocican.h"
#include "EXTERNAL/BOA 1.6/Include/CSI/csisfs.h"
#endif

/**
 * The one and only CCAN_ETAS_BOA object
 */
CCAN_ETAS_BOA theApp;

/**
 * CCAN_ETAS_BOA initialization
 */
static HINSTANCE ghLangInst=NULL;

BOOL CCAN_ETAS_BOA::InitInstance()
{
    // Begin of Multiple Language support
    if ( CMultiLanguage::m_nLocales <= 0 )    // Not detected yet
    {
        CMultiLanguage::DetectLangID(); // Detect language as user locale
        CMultiLanguage::DetectUILanguage();    // Detect language in MUI OS
    }
    TCHAR szModuleFileName[MAX_PATH];        // Get Module File Name and path
    int ret = ::GetModuleFileName(theApp.m_hInstance, szModuleFileName, MAX_PATH);
    if ( ret == 0 || ret == MAX_PATH )
    {
        ASSERT(FALSE);
    }
    // Load resource-only language DLL. It will use the languages
    // detected above, take first available language,
    // or you can specify another language as second parameter to
    // LoadLangResourceDLL. And try that first.
    ghLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
    if (ghLangInst)
    {
        AfxSetResourceHandle( ghLangInst );
    }
    // End of Multiple Language support

    CWinApp::InitInstance();

    return TRUE;
}

const BYTE FILTER_ADD = 0x01;
const BYTE FILTER_REMOVE = 0x02;
const BYTE QUEUE_ADD = 0x01;
const BYTE QUEUE_DESTROY = 0x02;

/**
 * Channel information
 */
typedef struct tagCHANNEL
{
    /** URI of the Controller */
    OCI_URIName uri;

    /** Controller handle */
    OCI_ControllerHandle controllerHandle;

    /** Controller configuration */
    OCI_CANConfiguration canConfiguration;

    /* Creating OCI_CANFD_Configuration only if BOA_VERSION_1_5_FD is defined */
#ifdef BOA_VERSION_1_5_FD
    /** CAN FD Controller configuration */
    OCI_CANFD_Configuration canFdConfiguration;
#endif

    /** can controller properties */
    OCI_CANControllerProperties canControllerProperties;

    /** can receive queue handle */
    OCI_QueueHandle canRxQueueHandle;

    /** can receive queue configuration */
    OCI_CANRxQueueConfiguration canRxQueueConfiguration;

    /** can transmit queue handle */
    OCI_QueueHandle canTxQueueHandle;

    /** can transmit queue configuration */
    OCI_CANTxQueueConfiguration canTxQueueConfiguration;

    /** can receive filter */
    OCI_CANRxFilter canRxFilter;

    /** can event filter */
    OCI_CANEventFilter canEventFilter;

    /** error frame filter */
    OCI_CANErrorFrameFilter canErrorFrameFilter;

    /** internal error event filter */
    OCI_InternalErrorEventFilter internalErrorEventFilter;

    /** timer capabilities */
    OCI_TimerCapabilities timerCapabilities;

    /** Controller state */
    UCHAR controllerState;

    /** Controller Tx error counter */
    UCHAR txErrorCounter;

    /** Controller Rx error counter */
    UCHAR rxErrorCounter;

    /** Controller peak Rx error counter */
    UCHAR peakRxErrorCounter;

    /** Controller peak Tx error counter */
    UCHAR peakTxErrorCounter;

    /** resolution */
    FLOAT resolution;
} SCHANNEL;

#define MAX_BUFF_ALLOWED 16
#define MAX_CLIENT_ALLOWED 16
static UINT clientCount = 0;

/**
 * Client Buffer map
 */
class SCLIENTBUFMAP
{
public:
    /** client ID */
    DWORD clientId;

    /** client buffer */
    CBaseCANBufFSE* clientBuffer[MAX_BUFF_ALLOWED];

    /** client name */
    std::string clientName;

    /** buffer count */
    UINT bufferCount;

    SCLIENTBUFMAP() :
        clientName("")
    {
        clientId = 0;
        bufferCount = 0;

        for (INT i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            clientBuffer[i] = NULL;
        }
    }
};

/**
 * Array of clients
 */
static std::vector<SCLIENTBUFMAP> sg_asClientToBufMap(MAX_CLIENT_ALLOWED);

const INT MAX_MAP_SIZE = 3000;

typedef struct tagAckMap
{
    UINT messageId;
    UINT clientId;
    UINT channel;

    BOOL operator == (const tagAckMap& RefObj)
    {
        return ((messageId == RefObj.messageId) && (channel == RefObj.channel));
    }
} SACK_MAP;

typedef std::list<SACK_MAP> CACK_MAP_LIST;
static CACK_MAP_LIST sg_asAckMapBuf;
static CRITICAL_SECTION sg_CritSectForAckBuf;       // To make it thread safe

/**
 * Channel instances
 */
static std::vector<SCHANNEL> sg_asChannel(defNO_OF_CHANNELS);

/**
 * Number of current channel selected
 */
static UINT sg_nNoOfChannels = 0;

static BOOL sg_bIsDriverRunning = FALSE;

static HWND sg_hOwnerWnd = NULL;

const BYTE CREATE_MAP_TIMESTAMP = 0x1;

const BYTE CALC_TIMESTAMP_READY = 0x2;

/**
 * Current state machine
 */
static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;

static HANDLE sg_hEvent = NULL;

static CRITICAL_SECTION sg_DIL_CriticalSection;

static INT sg_anSelectedItems[CHANNEL_ALLOWED];

/* Timer variables */
static SYSTEMTIME currentSystemTime;
static UINT64 timeStamp = 0;
static LARGE_INTEGER queryTickCount;
static LARGE_INTEGER frequency;

/* Required libraries */
static HMODULE sg_hLibCSI = NULL;
static HMODULE sg_hLibOCI = NULL;

/* Declarations of CSI API pointers */
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC1)(const char* uriName, CSI_NodeRange range, CSI_Tree* *tree);
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC2)(CSI_Tree* tree);
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC3)(CSI_Tree* tree, const BOA_UuidVersion* uuid, OCI_URIName uriName[], int size, int* count);

/* Macro definitions */
#if BOA_VERSION == BOA_VERSION_1_4
#define BOA_REGISTRY_LOCATION "SOFTWARE\\ETAS\\BOA\\1.4"
#define LIB_CSL_NAME    "dll-csiBind_1_4.dll"
#define LIB_OCI_NAME    "dll-ocdProxy_1_4.dll"
#elif BOA_VERSION == BOA_VERSION_1_5
#define BOA_REGISTRY_LOCATION "SOFTWARE\\ETAS\\BOA\\1.5"
#define LIB_CSL_NAME    "dll-csiBind_1_5.dll"
#define LIB_OCI_NAME    "dll-ocdProxy_1_5.dll"
#elif BOA_VERSION == BOA_VERSION_1_6
#define BOA_REGISTRY_LOCATION "SOFTWARE\\ETAS\\BOA\\1.6"
#define LIB_CSL_NAME    "dll-csiBind_1_6.dll"
#define LIB_OCI_NAME    "dll-ocdProxy_1_6.dll"
#endif

/**
 * CSI pointers table
 */
typedef struct tagCSI_VTABLE
{
    PROC1 createProtocolTree;
    PROC2 destroyProtocolTree;
    PROC3 getUriForUuid;
} CSI_VTABLE;

/**
 * Complete pointers of OCI, OCI CAN, CSI
 */
typedef struct tagBOA_POINTER_TABLE
{
    CSI_VTABLE       m_sCSI;
    OCI_CAN_VTable   m_sOCI;
} SBOA_POINTER_TABLE;

static SBOA_POINTER_TABLE sBOA_PTRS;

/**
 * Starts code for the state machine
 */
enum
{
    STATE_DRIVER_SELECTED    = 0x0,
    STATE_HW_INTERFACE_LISTED,
    STATE_HW_INTERFACE_SELECTED,
    STATE_CONNECTED
};

BYTE sg_bCurrState = STATE_DRIVER_SELECTED;

/**
 * CDIL_CAN_ETAS_BOA class definition
 */
class CDIL_CAN_ETAS_BOA : public CBaseDIL_CAN_Controller
{
public:
    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    HRESULT performInitOperations(void);
    HRESULT performClosureOperations(void);
    HRESULT getTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL);
    HRESULT listHardwareInterfaces(InterfaceHardwareList& sSelHwInterface, INT& nCount);
    HRESULT selectHardwareInterface(const InterfaceHardwareList& sSelHwInterface, INT nCount);
    HRESULT deselectHardwareInterface(void);
    HRESULT displayConfigurationDialog(PSCONTROLLER_DETAILS InitData, int& Length);
    HRESULT setConfigurationData(PSCONTROLLER_DETAILS InitData, int Length);
    HRESULT startHardware(void);
    HRESULT stopHardware(void);
    HRESULT getCurrentStatus(s_STATUSMSG& StatusData);
    HRESULT sendMessage(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT getControllerParameters(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT setControllerParameters(int nValue, ECONTR_PARAM eContrparam);
    HRESULT getErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);

    // Specific function set
    HRESULT setApplicationParameters(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
    HRESULT manageMessageBuffer(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
    HRESULT registerClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT loadDriverLibrary(void);
    HRESULT unloadDriverLibrary(void);
};
static CDIL_CAN_ETAS_BOA* sg_pouDIL_CAN_ETAS_BOA = NULL;

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Returns the interface to controller
 */
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( NULL == sg_pouDIL_CAN_ETAS_BOA )
    {
        if ((sg_pouDIL_CAN_ETAS_BOA = new CDIL_CAN_ETAS_BOA) == NULL)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) sg_pouDIL_CAN_ETAS_BOA;  /* Doesn't matter even if sg_pouDIL_CAN_Kvaser is null */

    return hResult;
}

/**
 * Error logger
 */
static Base_WrapperErrorLogger* sg_pIlog   = NULL;

/**
 * Declarations of ProcessCanData. This is a call back function
 * which will be called by BOA framework whenever there is a msg
 */
static void (OCI_CALLBACK ProcessCanData)(void* userData, struct OCI_CANMessage* msg);

/**
 * Declarations of ProcessEvents. This is a call back function
 * which will be called by BOA framework whenever there are internal events
 */
static void (OCI_CALLBACK ProcessEvents)(void* userData, struct OCI_CANMessage* msg);

/**
 * CallBack function used by the qsort Function
 */
INT nCallBackStrCompareFn( const void* str1, const void* str2)
{
    return( strcmp((char*)str1,(char*)str2) );
}

static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf)
{
    BOOL bExist = FALSE;
    for (UINT i = 0; i < sClientObj.bufferCount; i++)
    {
        if (pBuf == sClientObj.clientBuffer[i])
        {
            bExist = TRUE;
            i = sClientObj.bufferCount; //break the loop
        }
    }
    return bExist;
}

static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove)
{
    BOOL bReturn = TRUE;
    for (UINT i = 0; i < unCount; i++)
    {
        if (RootBufferArray[i] == BufferToRemove)
        {
            if (i < (unCount - 1)) //If not the last bufffer
            {
                RootBufferArray[i] = RootBufferArray[unCount - 1];
            }
            unCount--;
        }
    }
    return bReturn;
}

/**
 * Gets the CSI API function pointer from the cslproxy.dll
 */
BOOL bGetBOAInstallationPath(std::string& pcPath)
{
    USES_CONVERSION;

    BOOL bResult = FALSE;
    LONG lError = 0;
    HKEY sKey;
    // Get the installation path for BOA
    lError = RegOpenKeyEx( HKEY_LOCAL_MACHINE, BOA_REGISTRY_LOCATION, 0, KEY_READ, &sKey);
    // If the registry key open successfully, get the value in "path"
    // sub key
    if(lError==ERROR_SUCCESS)
    {
        ULONG ulType = REG_SZ;
        BYTE acGCCPath[1024];
        DWORD dwSize = sizeof(acGCCPath);
        lError = RegQueryValueEx(sKey, "path", 0, &ulType, acGCCPath, &dwSize);
        pcPath = A2T((char*)acGCCPath);
        RegCloseKey(sKey);
        bResult = TRUE;
    }
    return bResult;
}

/**
 * Gets the CSI API function pointer from the cslproxy.dll
 */
HRESULT GetCSI_API_Pointers(HMODULE hLibCSI)
{
    HRESULT hResult = S_OK;
    if (hLibCSI != NULL)
    {
        if ((sBOA_PTRS.m_sCSI.createProtocolTree        = (PROC1)GetProcAddress(hLibCSI, "CSI_CreateProtocolTree")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sCSI.destroyProtocolTree  = (PROC2)GetProcAddress(hLibCSI, "CSI_DestroyProtocolTree")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sCSI.getUriForUuid        = (PROC3)GetProcAddress(hLibCSI, "CSI_GetUriForUuid")) == NULL)
        {
            hResult = S_FALSE;
        }
    }
    return hResult;
}

/**
 * Gets the OCI API function pointer from the ocdproxy.dll
 */
HRESULT GetOCI_API_Pointers(HMODULE hLibOCI)
{
    HRESULT hResult = S_OK;
    if (hLibOCI != NULL)
    {
        if ((sBOA_PTRS.m_sOCI.createCANController = (PF_OCI_CreateCANController)
                GetProcAddress(hLibOCI, "OCI_CreateCANController")) == NULL)
        {
            hResult = S_FALSE;
        }
        if ((sBOA_PTRS.m_sOCI.destroyCANController = (PF_OCI_DestroyCANController)
                GetProcAddress(hLibOCI, "OCI_DestroyCANController")) == NULL)
        {
            hResult = S_FALSE;
        }
        if ((sBOA_PTRS.m_sOCI.openCANController = (PF_OCI_OpenCANController)
                GetProcAddress(hLibOCI, "OCI_OpenCANController")) == NULL)
        {
            hResult = S_FALSE;
        }
#ifdef BOA_VERSION_1_5_FD
        {
            if ((sBOA_PTRS.m_sOCI.openCANFDController = (PF_OCI_OpenCANFDController)
                    GetProcAddress(hLibOCI, "OCI_OpenCANFDController")) == NULL)
            {
                hResult = S_FALSE;
            }
        }
#endif
        if ((sBOA_PTRS.m_sOCI.closeCANController = (PF_OCI_CloseCANController)
                GetProcAddress(hLibOCI, "OCI_CloseCANController")) == NULL)
        {
            hResult = S_FALSE;
        }
        if ((sBOA_PTRS.m_sOCI.getCANConfiguration = (PF_OCI_GetCANConfiguration)
                GetProcAddress(hLibOCI, "OCI_GetCANConfiguration")) == NULL)
        {
            hResult = S_FALSE;
        }
#ifdef BOA_VERSION_1_5_FD
        {
            if ((sBOA_PTRS.m_sOCI.getCANFDConfiguration = (PF_OCI_GetCANFDConfiguration)
                    GetProcAddress(hLibOCI, "OCI_GetCANFDConfiguration")) == NULL)
            {
                hResult = S_FALSE;
            }
        }
#endif
        if ((sBOA_PTRS.m_sOCI.getCANControllerProperties = (PF_OCI_GetCANControllerProperties)
                GetProcAddress(hLibOCI, "OCI_GetCANControllerProperties")) == NULL)
        {
            hResult = S_FALSE;
        }
        if ((sBOA_PTRS.m_sOCI.setCANControllerProperties = (PF_OCI_SetCANControllerProperties)
                GetProcAddress(hLibOCI, "OCI_SetCANControllerProperties")) == NULL)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.canioVTable.getCANControllerCapabilities = (PF_OCI_GetCANControllerCapabilities)
                GetProcAddress(hLibOCI, "OCI_GetCANControllerCapabilities")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.getCANControllerStatus = (PF_OCI_GetCANControllerStatus)
                  GetProcAddress(hLibOCI, "OCI_GetCANControllerStatus")) == NULL)
        {
            hResult = S_FALSE;
        }

        else if ((sBOA_PTRS.m_sOCI.canioVTable.createCANTxQueue = (PF_OCI_CreateCANTxQueue)
                  GetProcAddress(hLibOCI, "OCI_CreateCANTxQueue")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.writeCANData = (PF_OCI_WriteCANData)
                  GetProcAddress(hLibOCI, "OCI_WriteCANData")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.destroyCANTxQueue = (PF_OCI_DestroyCANTxQueue)
                  GetProcAddress(hLibOCI, "OCI_DestroyCANTxQueue")) == NULL)
        {
            hResult = S_FALSE;
        }

        else if ((sBOA_PTRS.m_sOCI.canioVTable.createCANRxQueue = (PF_OCI_CreateCANRxQueue)
                  GetProcAddress(hLibOCI, "OCI_CreateCANRxQueue")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.readCANData = (PF_OCI_ReadCANData)
                  GetProcAddress(hLibOCI, "OCI_ReadCANData")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.destroyCANRxQueue = (PF_OCI_DestroyCANRxQueue)
                  GetProcAddress(hLibOCI, "OCI_DestroyCANRxQueue")) == NULL)
        {
            hResult = S_FALSE;
        }

        else if ((sBOA_PTRS.m_sOCI.canioVTable.addCANFrameFilter = (PF_OCI_AddCANFrameFilter)
                  GetProcAddress(hLibOCI, "OCI_AddCANFrameFilter")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.removeCANFrameFilter = (PF_OCI_RemoveCANFrameFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveCANFrameFilter")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.addCANBusEventFilter = (PF_OCI_AddCANBusEventFilter)
                  GetProcAddress(hLibOCI, "OCI_AddCANBusEventFilter")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.removeCANBusEventFilter = (PF_OCI_RemoveCANBusEventFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveCANBusEventFilter")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.addCANErrorFrameFilter = (PF_OCI_AddCANErrorFrameFilter)
                  GetProcAddress(hLibOCI, "OCI_AddCANErrorFrameFilter")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.removeCANErrorFrameFilter = (PF_OCI_RemoveCANErrorFrameFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveCANErrorFrameFilter")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerStatus = (PF_OCI_GetTimerStatus)
                  GetProcAddress(hLibOCI, "OCI_GetTimerStatus")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerStatus = (PF_OCI_GetTimerStatus)
                  GetProcAddress(hLibOCI, "OCI_GetTimerStatus")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerCapabilities = (PF_OCI_GetTimerCapabilities)
                  GetProcAddress(hLibOCI, "OCI_GetTimerCapabilities")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerValue = (PF_OCI_GetTimerValue)
                  GetProcAddress(hLibOCI, "OCI_GetTimerValue")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.addTimerEventFilter = (PF_OCI_AddTimerEventFilter)
                  GetProcAddress(hLibOCI, "OCI_AddTimerEventFilter")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.removeTimerEventFilter = (PF_OCI_RemoveTimerEventFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveTimerEventFilter")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.errorVTable.getError = (PF_OCI_GetError)
                  GetProcAddress(hLibOCI, "OCI_GetError")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.errorVTable.addInternalErrorEventFilter = (PF_OCI_AddInternalErrorEventFilter)
                  GetProcAddress(hLibOCI, "OCI_AddInternalErrorEventFilter")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.errorVTable.removeInternalErrorEventFilter = (PF_OCI_RemoveInternalErrorEventFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveInternalErrorEventFilter")) == NULL)
        {
            hResult = S_FALSE;
        }
    }
    return hResult;
}

/**
 * Search for all connected Hardware, that supports the OCI
 * CAN interface and deliver the URI location of the hardware.
 */
BOA_ResultCode OCI_FindCANController(OCI_URIName uriName[], INT nSize, INT* nFound)
{
    OCI_ErrorCode ec;

    /* Container for search results */
    CSI_Tree* sfsTree = NULL;

    /* Specify that we want to search for nodes which implement v1.1.0.0 of OCI_CAN. */
    static const BOA_UuidVersion ociCanUuid = { UUID_OCICAN, {1,1,0,0} };

    /* Specify that we want to search for any kind of node, not just hardware nodes */
    const CSI_NodeRange nodeRange = {CSI_NODE_MIN, CSI_NODE_MAX};

    /* Search for all connected hardware and latch the result for further processing */
    ec = (*(sBOA_PTRS.m_sCSI.createProtocolTree))("", nodeRange, &sfsTree);
    if (BOA_SUCCEEDED(ec))
    {
        /* Find the URIs for all nodes which implement v1.1.0.0 of OCI_CAN. */
        ec = (*(sBOA_PTRS.m_sCSI.getUriForUuid))(sfsTree, &ociCanUuid, uriName, nSize, nFound);
        if (BOA_SUCCEEDED(ec))
        {
            /* Clean up the protocol tree. */
            ec = (*(sBOA_PTRS.m_sCSI.destroyProtocolTree))(sfsTree);
        }
    }

    return ec;
}

/**
 * @return Returns true if found else false.
 *
 * unClientIndex will have index to client array which has clientId dwClientID.
 */
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult = FALSE;

    for (UINT i = 0; i < sg_asClientToBufMap.size(); i++)
    {
        if (sg_asClientToBufMap[i].clientId == dwClientID)
        {
            unClientIndex = i;
            i = clientCount; //break the loop
            bResult = TRUE;
            break;
        }
    }
    return bResult;
}

/**
 * @return TRUE if client exists else FALSE
 *
 * Checks for the existance of the client with the name pcClientName.
 */
static BOOL bClientExist(std::string pcClientName, INT& Index)
{
    for (UINT i = 0; i < sg_asClientToBufMap.size(); i++)
    {
        if (pcClientName == sg_asClientToBufMap[i].clientName)
        {
            Index = i;
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * @return TRUE if client removed else FALSE
 *
 * Removes the client with client id dwClientId.
 */
static BOOL bRemoveClient(DWORD dwClientId)
{
    BOOL bResult = FALSE;
    if (clientCount > 0)
    {
        UINT unClientIndex = 0;
        if (bGetClientObj(dwClientId, unClientIndex))
        {
            sg_asClientToBufMap[unClientIndex].clientId = 0;
            sg_asClientToBufMap[unClientIndex].clientName = "";

            for (INT i = 0; i < MAX_BUFF_ALLOWED; i++)
            {
                sg_asClientToBufMap[unClientIndex].clientBuffer[i] = NULL;
            }
            sg_asClientToBufMap[unClientIndex].bufferCount = 0;
            if ((unClientIndex + 1) < clientCount)
            {
                sg_asClientToBufMap[unClientIndex] = sg_asClientToBufMap[clientCount - 1];
            }
            clientCount--;
            bResult = TRUE;
        }
    }
    return bResult;
}

/**
 * @return TRUE if client exists else FALSE
 *
 * Searches for the client with the id dwClientId.
 */
static BOOL bClientIdExist(const DWORD& dwClientId)
{
    BOOL bReturn = FALSE;

    for (UINT i = 0; i < sg_asClientToBufMap.size(); i++)
    {
        if (sg_asClientToBufMap[i].clientId == dwClientId)
        {
            bReturn = TRUE;
            i = clientCount; // break the loop
        }
    }
    return bReturn;
}

/**
 * Returns the available slot
 */
static DWORD dwGetAvailableClientSlot()
{
    DWORD nClientId = 2;
    for (INT i = 0; i < MAX_CLIENT_ALLOWED; i++)
    {
        if (bClientIdExist(nClientId))
        {
            nClientId += 1;
        }
        else
        {
            i = MAX_CLIENT_ALLOWED; //break the loop
        }
    }
    return nClientId;
}

/**
 * Pushes an entry into the list at the last position
 */
void vMarkEntryIntoMap(const SACK_MAP& RefObj)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    sg_asAckMapBuf.push_back(RefObj);
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
}

BOOL bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    BOOL bResult = FALSE;
    CACK_MAP_LIST::iterator  iResult =
        find( sg_asAckMapBuf.begin(), sg_asAckMapBuf.end(), RefObj );

    //if ((*iResult).clientId > 0)
    if (iResult != sg_asAckMapBuf.end())
    {
        bResult = TRUE;
        ClientID = (*iResult).clientId;
        sg_asAckMapBuf.erase(iResult);
    }
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
    return bResult;
}

/**
 * @param[in] nChannel Channel information
 *
 * Initialize default values for controller configuration
 */
void vInitializeControllerConfig(UINT nChannel)
{
    sg_asChannel[nChannel].canConfiguration.baudrate = 500000;
    sg_asChannel[nChannel].canConfiguration.samplePoint = 70;
    sg_asChannel[nChannel].canConfiguration.samplesPerBit = OCI_CAN_THREE_SAMPLES_PER_BIT;
    sg_asChannel[nChannel].canConfiguration.BTL_Cycles = 10;
    sg_asChannel[nChannel].canConfiguration.SJW = 4;
    sg_asChannel[nChannel].canConfiguration.syncEdge = OCI_CAN_SINGLE_SYNC_EDGE;
    sg_asChannel[nChannel].canConfiguration.physicalMedia = OCI_CAN_MEDIA_HIGH_SPEED;
    sg_asChannel[nChannel].canConfiguration.selfReceptionMode = OCI_SELF_RECEPTION_ON;
    //Set controller property to SUSPENDED
    sg_asChannel[nChannel].canControllerProperties.mode = OCI_CONTROLLER_MODE_SUSPENDED;

#ifdef BOA_VERSION_1_5_FD
    sg_asChannel[nChannel].canFdConfiguration.can.baudrate = 500000;
    sg_asChannel[nChannel].canFdConfiguration.can.samplePoint = 70;
    sg_asChannel[nChannel].canFdConfiguration.can.samplesPerBit = OCI_CAN_THREE_SAMPLES_PER_BIT;
    sg_asChannel[nChannel].canFdConfiguration.can.BTL_Cycles = 10;
    sg_asChannel[nChannel].canFdConfiguration.can.SJW = 4;
    sg_asChannel[nChannel].canFdConfiguration.can.syncEdge = OCI_CAN_SINGLE_SYNC_EDGE;
    sg_asChannel[nChannel].canFdConfiguration.can.physicalMedia = OCI_CAN_MEDIA_HIGH_SPEED;
    sg_asChannel[nChannel].canFdConfiguration.can.selfReceptionMode = OCI_SELF_RECEPTION_ON;
    //Set controller property to SUSPENDED
    sg_asChannel[nChannel].canControllerProperties.mode = OCI_CONTROLLER_MODE_SUSPENDED;

    /* Set CAN FD default values */
    sg_asChannel[nChannel].canFdConfiguration.dataBitRate                = 2000000;
    sg_asChannel[nChannel].canFdConfiguration.dataSamplePoint            = 70;
    sg_asChannel[nChannel].canFdConfiguration.dataBTL_Cycles             = 10;
    sg_asChannel[nChannel].canFdConfiguration.dataSJW                    = 04;
    sg_asChannel[nChannel].canFdConfiguration.txDelayCompensationControl = OCI_CANFD_TX_DELAY_COMPENSATION_OFF;
    sg_asChannel[nChannel].canFdConfiguration.txDelayCompensationQuanta  = 0;
    sg_asChannel[nChannel].canFdConfiguration.rxCompatibility            = OCI_CANFD_RX_USE_RXFD_MESSAGE_PADDING;
    sg_asChannel[nChannel].canFdConfiguration.txCompatibility            = OCI_CANFD_TX_USE_DBR;
#endif
}

/**
 * @param[in] nChannel Channel information
 *
 * Initialize default values for queue confgiuration
 */
void vInitializeQueueConfig(UINT nChannel)
{
    /* configure Rx Queue*/
    sg_asChannel[nChannel].canRxQueueConfiguration.onFrame.function = ProcessCanData;
    sg_asChannel[nChannel].canRxQueueConfiguration.onEvent.function = ProcessEvents;
    sg_asChannel[nChannel].canRxQueueConfiguration.selfReceptionMode = OCI_SELF_RECEPTION_ON;

	/* configure Tx Queue*/
    sg_asChannel[nChannel].canTxQueueConfiguration.reserved = 0;
}

/**
 * @param[in] nChannel Channel information
 *
 * Initialize default values for filter confgiuration
 */
void vInitializeFilterConfig(UINT nChannel)
{
    /* configure frame filter*/
    sg_asChannel[nChannel].canRxFilter.frameIDMask = 0;
    sg_asChannel[nChannel].canRxFilter.frameIDValue = 0;
    sg_asChannel[nChannel].canRxFilter.tag = 0;

    /* configure event filter*/
    sg_asChannel[nChannel].canEventFilter.destination = OCI_EVENT_DESTINATION_CALLBACK;
    sg_asChannel[nChannel].canEventFilter.eventCode =
        OCI_CAN_BUS_EVENT_STATE_ACTIVE |
        OCI_CAN_BUS_EVENT_STATE_PASSIVE |
        OCI_CAN_BUS_EVENT_STATE_ERRLIMIT |
        OCI_CAN_BUS_EVENT_STATE_BUSOFF |
        OCI_CAN_BUS_EVENT_FAULT_TOLERANT_SINGLE_WIRE ;
    sg_asChannel[nChannel].canEventFilter.tag = 0;

    /* configure error filter */
    sg_asChannel[nChannel].canErrorFrameFilter.destination = OCI_EVENT_DESTINATION_CALLBACK;
    sg_asChannel[nChannel].canErrorFrameFilter.errorFrame =
        OCI_CAN_ERR_TYPE_BITSTUFF |
        OCI_CAN_ERR_TYPE_FORMAT |
        OCI_CAN_ERR_TYPE_ACK |
        OCI_CAN_ERR_TYPE_BIT |
        OCI_CAN_ERR_TYPE_BIT_RECSV_BUT_DOMINANT |
        OCI_CAN_ERR_TYPE_BIT_DOMINANT_BUT_RECSV |
        OCI_CAN_ERR_TYPE_CRC |
        OCI_CAN_ERR_TYPE_OVERLOAD |
        OCI_CAN_ERR_TYPE_OTHER;
    sg_asChannel[nChannel].canErrorFrameFilter.tag = 0;

    /* configure internal error filter */
    sg_asChannel[nChannel].internalErrorEventFilter.eventCode = OCI_INTERNAL_GENERAL_ERROR;
    sg_asChannel[nChannel].internalErrorEventFilter.tag = 0;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Adds or removes the HW filter to/from the channel.
 */
HRESULT ManageFilters(BYTE byCode, UINT nChannel)
{
    HRESULT hResult = S_FALSE;
    BOA_ResultCode ErrCode = OCI_FAILURE;
    if (byCode == FILTER_ADD)
    {
        // Add Frame filter
        ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.addCANFrameFilter))
                  (sg_asChannel[nChannel].canRxQueueHandle,
                   &(sg_asChannel[nChannel].canRxFilter), 1);
        if (BOA_SUCCEEDED(ErrCode))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not add frame filter"));
        }

        // Add Event filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.addCANBusEventFilter))
                      (sg_asChannel[nChannel].canRxQueueHandle,
                       &(sg_asChannel[nChannel].canEventFilter), 1);

            if (BOA_FAILED(ErrCode))
            {
                hResult = S_FALSE;
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not add event filter"));
            }
        }

        // Add Error filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.addCANErrorFrameFilter))
                      (sg_asChannel[nChannel].canRxQueueHandle,
                       &(sg_asChannel[nChannel].canErrorFrameFilter), 1);
            if (BOA_FAILED(ErrCode))
            {
                hResult = S_FALSE;
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not add error filter"));
            }
        }
        // Add internal error filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.errorVTable.addInternalErrorEventFilter))
                      (sg_asChannel[nChannel].canRxQueueHandle,
                       &(sg_asChannel[nChannel].internalErrorEventFilter), 1);
            if (BOA_FAILED(ErrCode))
            {
                hResult = S_FALSE;
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not add error filter"));
            }
        }

    }
    else if (byCode == FILTER_REMOVE)
    {
        // Remove Frame filter
        ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.removeCANFrameFilter))
                  (sg_asChannel[nChannel].canRxQueueHandle,
                   &(sg_asChannel[nChannel].canRxFilter), 1);
        if (BOA_SUCCEEDED(ErrCode))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not remove frame filter"));
        }

        // Remove Event filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.removeCANBusEventFilter))
                      (sg_asChannel[nChannel].canRxQueueHandle,
                       &(sg_asChannel[nChannel].canEventFilter), 1);
            if (BOA_FAILED(ErrCode))
            {
                hResult = S_FALSE;
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not remove event filter"));
            }
        }

        // Remove Error filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.removeCANErrorFrameFilter))
                      (sg_asChannel[nChannel].canRxQueueHandle,
                       &(sg_asChannel[nChannel].canErrorFrameFilter), 1);
            if (BOA_FAILED(ErrCode))
            {
                hResult = S_FALSE;
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not remove error filter"));
            }
        }
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.errorVTable.removeInternalErrorEventFilter))
                      (sg_asChannel[nChannel].canRxQueueHandle,
                       &(sg_asChannel[nChannel].internalErrorEventFilter), 1);
            if (BOA_FAILED(ErrCode))
            {
                hResult = S_FALSE;
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not add error filter"));
            }
        }
    }
    return hResult;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Adds or removes the queue to/from the channel.
 */
HRESULT ManageQueue(BYTE byCode, UINT nChannel)
{
    HRESULT hResult = S_FALSE;
    BOA_ResultCode Err = OCI_ERR_FLAG_ERROR;
    if (byCode == QUEUE_ADD)
    {
        //Create CAN Rx queue
        Err = (*(sBOA_PTRS.m_sOCI.canioVTable.createCANRxQueue))
              (sg_asChannel[nChannel].controllerHandle,
               &(sg_asChannel[nChannel].canRxQueueConfiguration),
               &(sg_asChannel[nChannel].canRxQueueHandle));
        if (BOA_SUCCEEDED(Err))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not create rx queue"));
        }
        //Create CAN Tx queue
        if (hResult == S_OK)
        {
            Err = (*(sBOA_PTRS.m_sOCI.canioVTable.createCANTxQueue))
                  (sg_asChannel[nChannel].controllerHandle,
                   &(sg_asChannel[nChannel].canTxQueueConfiguration),
                   &(sg_asChannel[nChannel].canTxQueueHandle));
            if (BOA_FAILED(Err))
            {
                hResult = S_FALSE;
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not create tx queue"));
            }
        }
    }
    else if (byCode == QUEUE_DESTROY)
    {
        //Destroy CAN Rx queue
        Err = (*(sBOA_PTRS.m_sOCI.canioVTable.destroyCANRxQueue))(sg_asChannel[nChannel].canRxQueueHandle);
        if (BOA_SUCCEEDED(Err))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not create rx queue"));
        }
        //Create CAN Tx queue
        if (hResult == S_OK)
        {
            Err = (*(sBOA_PTRS.m_sOCI.canioVTable.destroyCANTxQueue))(sg_asChannel[nChannel].canTxQueueHandle);
            if (BOA_FAILED(Err))
            {
                hResult = S_FALSE;
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not create tx queue"));
            }
        }
    }
    return hResult;
}

/**
 * copies from OCI_CANRxMessage struct into STCANDATA struct
 */
void vCopyOCI_CAN_RX_2_DATA(const OCI_CANRxMessage* SrcMsg, STCANDATA* DestMsg)
{
    DestMsg->m_uDataInfo.m_sCANMsg.m_unMsgID = SrcMsg->frameID;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucDataLen = SrcMsg->dlc;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucEXTENDED   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_EXTENDED) == OCI_CAN_MSG_FLAG_EXTENDED)? 1 : 0;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucRTR   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_REMOTE_FRAME) == OCI_CAN_MSG_FLAG_REMOTE_FRAME)? 1 : 0;
    DestMsg->m_ucDataType   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_SELFRECEPTION) == OCI_CAN_MSG_FLAG_SELFRECEPTION)? TX_FLAG : RX_FLAG;

    UINT Channel = DestMsg->m_uDataInfo.m_sCANMsg.m_ucChannel;

    if (Channel > defNO_OF_CHANNELS)
    {
        Channel = 1;//Take appropriate action
    }

    DestMsg->m_lTickCount.QuadPart = (LONGLONG)(SrcMsg->timeStamp * sg_asChannel[Channel - 1].resolution);
    memcpy(DestMsg->m_uDataInfo.m_sCANMsg.m_ucData, SrcMsg->data, sizeof(DestMsg->m_uDataInfo.m_sCANMsg.m_ucData));
}

/**
 * copies from OCI_CANRxMessage struct into STCANDATA struct
 */
#ifdef BOA_VERSION_1_5_FD
void vCopyOCI_CAN_FD_RX_2_DATA(const OCI_CANFD_RxMessage* SrcMsg, STCANDATA* DestMsg)
{
    DestMsg->m_uDataInfo.m_sCANMsg.m_unMsgID = SrcMsg->frameID;

    DestMsg->m_uDataInfo.m_sCANMsg.m_ucDataLen = SrcMsg->size;

    DestMsg->m_uDataInfo.m_sCANMsg.m_ucEXTENDED   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_EXTENDED) == OCI_CAN_MSG_FLAG_EXTENDED)? 1 : 0;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucRTR   = 0;
    DestMsg->m_ucDataType   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_SELFRECEPTION) == OCI_CAN_MSG_FLAG_SELFRECEPTION)? TX_FLAG : RX_FLAG;

    UINT Channel = DestMsg->m_uDataInfo.m_sCANMsg.m_ucChannel;

    if (Channel > defNO_OF_CHANNELS)
    {
        Channel = 1;//Take appropriate action
    }
    DestMsg->m_lTickCount.QuadPart = (LONGLONG)(SrcMsg->timeStamp * sg_asChannel[Channel - 1].resolution);
    memcpy(DestMsg->m_uDataInfo.m_sCANMsg.m_ucData, SrcMsg->data, SrcMsg->size);
}
#endif

/**
 * @return Returns channel number
 *
 * Gets the channel with handle OCI_ControllerHandle
 */
UINT nGetChannel(const OCI_ControllerHandle hHandle)
{
    UINT i;
    for (i = 0; i < sg_nNoOfChannels; i++)
    {
        if (sg_asChannel[i].controllerHandle == hHandle)
        {
            break;
        }
    }
    return (i+1);
}

/**
 * Create time mod mapping
 */
void vCreateTimeModeMapping(HANDLE hEvent)
{
    WaitForSingleObject(hEvent, INFINITE);
    GetLocalTime(&currentSystemTime);
    //Query Tick Count
    QueryPerformanceCounter(&queryTickCount);
}

/**
 * This function writes the message to the corresponding clients buffer
 */
static void vWriteIntoClientsBuffer(STCANDATA& sCanData)
{
    //Write into the client's buffer and Increment message Count
    if (sCanData.m_ucDataType == TX_FLAG)
    {
        static SACK_MAP sAckMap;
        UINT ClientId = 0;
        static UINT Index = (UINT)-1;
        sAckMap.channel = sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel;
        sAckMap.messageId = sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID;

        if (bRemoveMapEntry(sAckMap, ClientId))
        {
            BOOL bClientExists = bGetClientObj(ClientId, Index);
            for (UINT i = 0; i < clientCount; i++)
            {
                //Tx for monitor nodes and sender node
                if (/*(i == CAN_MONITOR_NODE_INDEX)  ||*/(bClientExists && (i == Index)))
                {
                    for (UINT j = 0; j < sg_asClientToBufMap[i].bufferCount; j++)
                    {
                        sg_asClientToBufMap[i].clientBuffer[j]->writeIntoBuffer(&sCanData);
                    }
                }
                else
                {
                    //Send the other nodes as Rx.
                    for (UINT j = 0; j < sg_asClientToBufMap[i].bufferCount; j++)
                    {
                        static STCANDATA sTempCanData;
                        sTempCanData = sCanData;
                        sTempCanData.m_ucDataType = RX_FLAG;
                        sg_asClientToBufMap[i].clientBuffer[j]->writeIntoBuffer(&sTempCanData);
                    }
                }
            }
        }
    }
    else // provide it to everybody
    {
        for (UINT i = 0; i < clientCount; i++)
        {
            for (UINT j = 0; j < sg_asClientToBufMap[i].bufferCount; j++)
            {
                sg_asClientToBufMap[i].clientBuffer[j]->writeIntoBuffer(&sCanData);
            }
        }
    }
}

/**
 * Processes Rx msg and writes into regiastered clients buffer.
 */
void vProcessRxMsg(void* userData, struct OCI_CANMessage* msg)
{
    /* First calculate timestamp for first message*/
    EnterCriticalSection(&sg_DIL_CriticalSection);

    static STCANDATA sCanData;
    static OCI_ControllerHandle hHandle;

    hHandle = (OCI_ControllerHandle)userData;
    sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)nGetChannel(hHandle);

    if ( msg->type == OCI_CAN_RX_MESSAGE )
    {
        //sCanData.m_uDataInfo.m_sCANMsg.m_bCANFDMsg = false;
        vCopyOCI_CAN_RX_2_DATA(&(msg->data.rxMessage), &sCanData);
        sCanData.m_uDataInfo.m_sCANMsg.m_bCANFD = false;
    }
#ifdef BOA_VERSION_1_5_FD
    /*Check if its a CAN FD message */
    else if ( msg->type ==  OCI_CANFD_RX_MESSAGE )
    {
        //sCanData.m_uDataInfo.m_sCANMsg.m_bCANFDMsg = true;
        vCopyOCI_CAN_FD_RX_2_DATA(&(msg->data.canFDRxMessage), &sCanData);
        sCanData.m_uDataInfo.m_sCANMsg.m_bCANFD = true;
    }
#endif

    if (sg_byCurrState == CREATE_MAP_TIMESTAMP)
    {
        //timeStamp = sCanData.m_lTickCount.QuadPart;
        SetEvent(sg_hEvent);
        vCreateTimeModeMapping(sg_hEvent);
        sg_byCurrState = CALC_TIMESTAMP_READY;


        LARGE_INTEGER g_QueryTickCount;
        QueryPerformanceCounter(&g_QueryTickCount);
        UINT64 unConnectionTime;
        unConnectionTime = ((g_QueryTickCount.QuadPart * 10000) / frequency.QuadPart) - timeStamp;
        //Time difference should be +ve value
        if(sCanData.m_lTickCount.QuadPart >= unConnectionTime)
        {
            timeStamp  = (LONGLONG)(sCanData.m_lTickCount.QuadPart - unConnectionTime);
        }
        else
        {
            timeStamp  = (LONGLONG)(unConnectionTime - sCanData.m_lTickCount.QuadPart);
        }
    }

    //Write the msg into registered client's buffer
    vWriteIntoClientsBuffer(sCanData);

    LeaveCriticalSection(&sg_DIL_CriticalSection);
}

/**
 * Processes Tx msg.
 */
void vProcessTxMsg(void* /*userData*/, OCI_CANMessage* /*msg*/)
{
}

/**
 * Copies OCI_CANErrorFrameMessage struct into STCANDATA.
 */
void vCopyOCI_CAN_ERR_2_DATA(const OCI_CANErrorFrameMessage* SrcMsg, STCANDATA* DestMsg)
{
    DestMsg->m_uDataInfo.m_sCANMsg.m_unMsgID = SrcMsg->frameID;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucDataLen = SrcMsg->dlc;
    DestMsg->m_lTickCount.QuadPart = SrcMsg->timeStamp;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucEXTENDED   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_EXTENDED) == OCI_CAN_MSG_FLAG_EXTENDED)? 1 : 0;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucRTR   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_REMOTE_FRAME) == OCI_CAN_MSG_FLAG_REMOTE_FRAME)? 1 : 0;
    DestMsg->m_ucDataType = ERR_FLAG;
    BOOL bIsTx = (SrcMsg->flags & OCI_CAN_MSG_FLAG_SELFRECEPTION) == OCI_CAN_MSG_FLAG_SELFRECEPTION;
    if (bIsTx)
    {
        DestMsg->m_uDataInfo.m_sErrInfo.m_ucTxErrCount += 1;
    }
    else
    {
        DestMsg->m_uDataInfo.m_sErrInfo.m_ucRxErrCount += 1;
    }
    switch (SrcMsg->type)
    {
        case OCI_CAN_ERR_TYPE_BITSTUFF:
        {
            if (bIsTx)
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = STUFF_ERROR_TX;
            }
            else
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = STUFF_ERROR_RX;
            }
        }
        break;
        case OCI_CAN_ERR_TYPE_FORMAT:
        {
            if (bIsTx)
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = FORM_ERROR_TX;
            }
            else
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = FORM_ERROR_RX;
            }
        }
        break;
        case OCI_CAN_ERR_TYPE_BIT:
        {
            if (bIsTx)
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = BIT_ERROR_TX;
            }
            else
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = BIT_ERROR_RX;
            }
        }
        break;
        case OCI_CAN_ERR_TYPE_OTHER:
        {
            if (bIsTx)
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = OTHER_ERROR_TX;
            }
            else
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = OTHER_ERROR_RX;
            }
        }
        break;
        case OCI_CAN_ERR_TYPE_CRC:
        {
            if (bIsTx)
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = CRC_ERROR_TX;
            }
            else
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = CRC_ERROR_RX;
            }
        }
        break;
        case OCI_CAN_ERR_TYPE_OVERLOAD:
        case OCI_CAN_ERR_TYPE_BIT_DOMINANT_BUT_RECSV:
        case OCI_CAN_ERR_TYPE_ACK:
        case OCI_CAN_ERR_TYPE_BIT_RECSV_BUT_DOMINANT:
        default:
        {
            DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = ERROR_UNKNOWN;
        }
        break;
    }
}

/**
 * Updates Tx & Rx error counter of the nChannel
 */
void vUpdateErrorCounter(UCHAR ucTxError, UCHAR ucRxError, UINT nChannel)
{
    // Update Tx Error counter and peak Tx Error Counter
    sg_asChannel[nChannel].txErrorCounter = sg_asChannel[nChannel].txErrorCounter + ucTxError;
    // Update Peak Value
    if( ucTxError > sg_asChannel[nChannel].peakTxErrorCounter )
    {
        sg_asChannel[nChannel].peakTxErrorCounter = ucTxError;
    }
    // Update Rx Error counter and peak Rx Error Counter
    sg_asChannel[nChannel].rxErrorCounter = sg_asChannel[nChannel].rxErrorCounter + ucRxError;
    // Update Peak Value
    if( ucRxError > sg_asChannel[nChannel].peakRxErrorCounter )
    {
        sg_asChannel[nChannel].peakRxErrorCounter = ucRxError;
    }
}

/**
 * Processes error msg and writes into registered clients buffer.
 */
void vProcessErrMsg(void* userData, struct OCI_CANMessage* msg)
{
    static STCANDATA sCanData;
    vCopyOCI_CAN_ERR_2_DATA(&(msg->data.errorFrameMessage), &sCanData);
    int32* pUserData = (int32*)userData;
    sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)nGetChannel(*pUserData);
    sCanData.m_uDataInfo.m_sErrInfo.m_ucChannel = sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel;
    // Update channel error counter
    vUpdateErrorCounter(sCanData.m_uDataInfo.m_sErrInfo.m_ucTxErrCount,
                        sCanData.m_uDataInfo.m_sErrInfo.m_ucRxErrCount,
                        sCanData.m_uDataInfo.m_sErrInfo.m_ucChannel);
    //Write the msg into registered client's buffer
    for (UINT i = 0; i < clientCount; i++)
    {
        for (UINT j = 0; j < sg_asClientToBufMap[i].bufferCount; j++)
        {
            sg_asClientToBufMap[i].clientBuffer[j]->writeIntoBuffer(&sCanData);
        }
    }
}

/**
 * Callback function called by the BOA framework
 * when there is presence of msg in the bus.
 */
static void (OCI_CALLBACK ProcessCanData)(void* userData, struct OCI_CANMessage* msg)
{
    switch (msg->type)
    {
        case OCI_CAN_RX_MESSAGE:
#ifdef BOA_VERSION_1_5_FD
        case OCI_CANFD_RX_MESSAGE:
#endif
            vProcessRxMsg(userData, msg);
            break;
        case OCI_CAN_TX_MESSAGE:
#ifdef BOA_VERSION_1_5_FD
        case OCI_CANFD_TX_MESSAGE:
#endif
            vProcessTxMsg(userData, msg);
            break;
        case OCI_CAN_ERROR_FRAME:
            vProcessErrMsg(userData, msg);
            break;
    }
}

/**
 * processes bus event.
 */
static void vProcessBusEvent(void* /*userData*/, struct OCI_CANMessage* /*msg*/)
{

}

/**
 * processes internal event.
 */
void vProcessInternalErrEvent(void* /*userData*/, struct OCI_CANMessage* /*msg*/)
{

}

/**
 * process queue event.
 */
void vProcessQueueEvent(void* /*userData*/, struct OCI_CANMessage* /*msg*/)
{

}

/**
 * process tmer event.
 */
void vProcessTimerEvent(void* /*userData*/, struct OCI_CANMessage* msg)
{
    switch (msg->data.timerEventMessage.eventCode)
    {
        case OCI_TIMER_EVENT_SYNC_LOCK:
            SetEvent(sg_hEvent);
            vCreateTimeModeMapping(sg_hEvent);
            break;
        case OCI_TIMER_EVENT_SYNC_LOSS:
            //Nothing at this moment
            break;
    }
}

/**
 * Callback function called by the BOA framework
 * when there is internal bus event.
 */
void (OCI_CALLBACK ProcessEvents)(void* userData, struct OCI_CANMessage* msg)
{
    switch (msg->type)
    {
        case OCI_CAN_BUS_EVENT:
            vProcessBusEvent(userData, msg);
            break;
        case OCI_CAN_INTERNAL_ERROR_EVENT:
            vProcessInternalErrEvent(userData, msg);
            break;
        case OCI_CAN_QUEUE_EVENT:
            vProcessQueueEvent(userData, msg);
            break;
        case OCI_CAN_TIMER_EVENT:
            vProcessTimerEvent(userData, msg);
            break;
    }
}

/**
 * @return Returns S_OK for success, S_FALSE for failure
 *
 * Sets the application params.
 */
HRESULT CDIL_CAN_ETAS_BOA::setApplicationParameters(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    sg_hOwnerWnd = hWndOwner;
    sg_pIlog = pILog;
    return S_OK;
}

/**
 * @return Returns S_OK for success, S_FALSE for failure
 *
 * Unloads the driver library.
 */
HRESULT CDIL_CAN_ETAS_BOA::unloadDriverLibrary(void)
{
    /* Unload OCI library */
    if (sg_hLibOCI != NULL)
    {
        FreeLibrary(sg_hLibOCI);
    }
    /* Unload CSI library */
    if (sg_hLibCSI != NULL)
    {
        FreeLibrary(sg_hLibCSI);
    }

    /* Invalidate all API pointers */
    memset(&sBOA_PTRS, 0, sizeof (SBOA_POINTER_TABLE));
    return S_OK;
}

/**
 * @return Returns S_OK for success, S_FALSE for failure
 *
 * Registers the buffer pBufObj to the client ClientID
 */
HRESULT CDIL_CAN_ETAS_BOA::manageMessageBuffer(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
    HRESULT hResult = S_FALSE;
    if (ClientID != 0)
    {
        UINT unClientIndex;
        if (bGetClientObj(ClientID, unClientIndex))
        {
            SCLIENTBUFMAP& sClientObj = sg_asClientToBufMap[unClientIndex];
            if (bAction == MSGBUF_ADD)
            {
                //Add msg buffer
                if (pBufObj != NULL)
                {
                    if (sClientObj.bufferCount < MAX_BUFF_ALLOWED)
                    {
                        if (bIsBufferExists(sClientObj, pBufObj) == FALSE)
                        {
                            sClientObj.clientBuffer[sClientObj.bufferCount++] = pBufObj;
                            hResult = S_OK;
                        }
                        else
                        {
                            hResult = ERR_BUFFER_EXISTS;
                        }
                    }
                }
            }
            else if (bAction == MSGBUF_CLEAR)
            {
                //clear msg buffer
                //clear msg buffer
                if (pBufObj != NULL) //REmove only buffer mentioned
                {
                    bRemoveClientBuffer(sClientObj.clientBuffer, sClientObj.bufferCount, pBufObj);
                }
                else //Remove all
                {
                    for (UINT i = 0; i < sClientObj.bufferCount; i++)
                    {
                        sClientObj.clientBuffer[i] = NULL;
                    }
                    sClientObj.bufferCount = 0;
                }
                hResult = S_OK;
            }
            else
            {
                ////ASSERT(FALSE);
            }
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }
    else
    {
        if (bAction == MSGBUF_CLEAR)
        {
            //clear msg buffer
            for (UINT i = 0; i < clientCount; i++)
            {
                manageMessageBuffer(MSGBUF_CLEAR, sg_asClientToBufMap[i].clientId, NULL);
            }
            hResult = S_OK;
        }
    }

    return hResult;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Registers a client to the DIL. ClientID will have client id
 * which will be used for further client related calls
 */
HRESULT CDIL_CAN_ETAS_BOA::registerClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
    HRESULT hResult = S_FALSE;
    if (bRegister)
    {
        if (clientCount < MAX_CLIENT_ALLOWED)
        {
            INT Index = 0;
            if (!bClientExist(pacClientName, Index))
            {
                //Currently store the client information
                if (strcmp(pacClientName, CAN_MONITOR_NODE) == 0)
                {
                    //First slot is reserved to monitor node
                    ClientID = 1;
                    sg_asClientToBufMap[0].clientName = pacClientName;
                    sg_asClientToBufMap[0].clientId = ClientID;
                    sg_asClientToBufMap[0].bufferCount = 0;
                }
                else
                {
                    if (!bClientExist(CAN_MONITOR_NODE, Index))
                    {
                        Index = clientCount + 1;
                    }
                    else
                    {
                        Index = clientCount;
                    }
                    ClientID = dwGetAvailableClientSlot();
                    sg_asClientToBufMap[Index].clientName = pacClientName;
                    sg_asClientToBufMap[Index].clientId = ClientID;
                    sg_asClientToBufMap[Index].bufferCount = 0;
                }
                clientCount++;
                hResult = S_OK;
            }
            else
            {
                ClientID = sg_asClientToBufMap[Index].clientId;
                hResult = ERR_CLIENT_EXISTS;
            }
        }
        else
        {
            hResult = ERR_NO_MORE_CLIENT_ALLOWED;
        }
    }
    else
    {
        if (bRemoveClient(ClientID))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }

    return hResult;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Loads BOA related libraries. Updates BOA API pointers
 */
HRESULT CDIL_CAN_ETAS_BOA::loadDriverLibrary(void)
{
    HRESULT hResult = S_FALSE;
    std::string acPath;
    /* Get BOA installation path from the registery */
    bGetBOAInstallationPath(acPath);
    /* Load cslproxy.dll library */
    std::string acLIB_CSL = "";
    acLIB_CSL.append(acPath);
    acLIB_CSL.append("\\");
    acLIB_CSL.append(LIB_CSL_NAME);
    /* LoadLibraryEx instead of LoadLibrary seems to be necessary under Windows 7 when the library is not in DLL search path (system32) */
    sg_hLibCSI = LoadLibraryEx(acLIB_CSL.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (sg_hLibCSI != NULL)
    {
        hResult = GetCSI_API_Pointers(sg_hLibCSI);

        /* Load the OCI library to use CAN controller */
        if (hResult == S_OK)
        {
            std::string acLIB_OCI;
            acLIB_OCI.append(acPath);
            acLIB_OCI.append("\\");
            acLIB_OCI.append(LIB_OCI_NAME);
            sg_hLibOCI = LoadLibraryEx(acLIB_OCI.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

            if (sg_hLibOCI != NULL)
            {
                hResult = GetOCI_API_Pointers(sg_hLibOCI);
                if (hResult != S_OK)
                {
                    sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not get OCI function pointers"));
                }
                else
                {
                    sg_bIsDriverRunning = TRUE;
                }
            }
            else
            {
                hResult = S_FALSE;
                std::string acErr;
                acErr.append(acLIB_OCI);
                acErr.append(" ");
                acErr.append(_("failed to load"));
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, acErr);
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("Please have a look at: https://github.com/rbei-etas/busmaster/wiki/Hardware-support"));
            }
        }
        else
        {
            sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not get CSI function pointers"));
        }
    }
    else
    {
        std::string acErr;
        acErr.append(acLIB_CSL);
        acErr.append(" ");
        acErr.append(_("failed to load"));
        sg_pIlog->logMessage(A2T(__FILE__), __LINE__, acErr);
        sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("Please have a look at: https://github.com/rbei-etas/busmaster/wiki/Hardware-support"));
    }
    return hResult;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Performs intial operations.
 * Initializes filter, queue, controller config with default values.
 */
HRESULT CDIL_CAN_ETAS_BOA::performInitOperations(void)
{
    /* Create critical section for ensuring thread
       safeness of read message function */
    InitializeCriticalSection(&sg_DIL_CriticalSection);
    /* Create an event for timestamp calculations*/
    sg_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    /*Initial Channel info with default values.
      1. Controller configuration
      2. Queue configuration
      3. Filter configuration*/

    for (UINT i = 0; i < defNO_OF_CHANNELS; i++)
    {
        /* Controller configuration default values*/
        vInitializeControllerConfig(i);
        /* Queue configuration default values*/
        vInitializeQueueConfig(i);
        /* Filter configuration default values*/
        vInitializeFilterConfig(i);
    }
    //Register monitor client
    DWORD dwClient = 0;
    registerClient(TRUE, dwClient, CAN_MONITOR_NODE);

    //Initialize the selected channel items array to -1
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }

    return S_OK;
}

/**
 * @return TRUE for success, FALSE for failure
 *
 * Copies the controller config values into channel's
 * controller config structure.
 */
static BOOL bLoadDataFromContr(PSCONTROLLER_DETAILS pControllerDetails)
{
    BOOL bReturn = FALSE;
    // If successful
    if (pControllerDetails != NULL)
    {
        char* pcStopStr = NULL;
        for( INT i = 0; i < defNO_OF_CHANNELS; i++ )
        {
#ifndef BOA_VERSION_1_5_FD
            sg_asChannel[i].canConfiguration.baudrate =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrBaudrate.c_str(),
                                            &pcStopStr, 10));
            //code commented to resolve the BOA message transmission issue (#389)
            //sg_asChannel[i].canConfiguration.baudrate
            //= (sg_asChannel[i].canConfiguration.baudrate * 1000);
            sg_asChannel[i].canConfiguration.samplesPerBit =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrSampling.c_str(),
                                            &pcStopStr, 10));
            sg_asChannel[i].canConfiguration.samplePoint =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrSamplePercentage.c_str(),
                                            &pcStopStr, 10));
            sg_asChannel[i].canConfiguration.SJW =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrSjw.c_str(),
                                            &pcStopStr, 10));
            sg_asChannel[i].canConfiguration.syncEdge = OCI_CAN_SINGLE_SYNC_EDGE;
            sg_asChannel[i].canConfiguration.selfReceptionMode = OCI_SELF_RECEPTION_ON;
            sg_asChannel[i].canConfiguration.BTL_Cycles = 10;
            sg_asChannel[i].canConfiguration.physicalMedia = OCI_CAN_MEDIA_HIGH_SPEED;
#else
            sg_asChannel[i].canFdConfiguration.can.baudrate =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrBaudrate.c_str(),
                                            &pcStopStr, 10));
            sg_asChannel[i].canFdConfiguration.can.baudrate
                = (sg_asChannel[i].canFdConfiguration.can.baudrate * 1000);
            sg_asChannel[i].canFdConfiguration.can.samplesPerBit =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrSampling.c_str(),
                                            &pcStopStr, 10));
            sg_asChannel[i].canFdConfiguration.can.samplePoint =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrSamplePercentage.c_str(),
                                            &pcStopStr, 10));
            sg_asChannel[i].canFdConfiguration.can.SJW =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrSjw.c_str(),
                                            &pcStopStr, 10));
            sg_asChannel[i].canFdConfiguration.can.syncEdge = OCI_CAN_SINGLE_SYNC_EDGE;
            sg_asChannel[i].canFdConfiguration.can.selfReceptionMode = OCI_SELF_RECEPTION_ON;
            sg_asChannel[i].canFdConfiguration.can.BTL_Cycles = 10;
            sg_asChannel[i].canFdConfiguration.can.physicalMedia = OCI_CAN_MEDIA_HIGH_SPEED;

            /* Set CAN FD parameters */
            sg_asChannel[i].canFdConfiguration.dataBitRate                   = pControllerDetails[i].m_unDataBitRate;
            sg_asChannel[i].canFdConfiguration.dataSamplePoint               = pControllerDetails[i].m_unDataSamplePoint;
            sg_asChannel[i].canFdConfiguration.dataBTL_Cycles                = pControllerDetails[i].m_unDataBTL_Cycles;
            sg_asChannel[i].canFdConfiguration.dataSJW                       = pControllerDetails[i].m_unDataSJW;
            sg_asChannel[i].canFdConfiguration.txDelayCompensationControl    = (OCI_CANFD_TxDelayCompensation)
                    pControllerDetails[i].m_bTxDelayCompensationON;
            sg_asChannel[i].canFdConfiguration.txDelayCompensationQuanta     = pControllerDetails[i].m_unTxDelayCompensationQuanta;
            sg_asChannel[i].canFdConfiguration.rxCompatibility               = (OCI_CANFD_RxConfig)
                    pControllerDetails[i].m_bytRxCompatibility;
            sg_asChannel[i].canFdConfiguration.txCompatibility               = (OCI_CANFD_TxConfig)
                    pControllerDetails[i].m_bytTxCompatibility;
#endif
        }
        bReturn = TRUE;
    }
    return bReturn;
}

/**
 * Copies from STCAN_MSG structure into OCI_CANTxMessage message structure
 */
void vCopy_2_OCI_CAN_Data(OCI_CANTxMessage& DestMsg, const STCAN_MSG& SrcMsg)
{
    DestMsg.res     = 0;
    DestMsg.frameID = SrcMsg.m_unMsgID;
    DestMsg.dlc     = SrcMsg.m_ucDataLen;
    DestMsg.flags   = SrcMsg.m_ucEXTENDED ? OCI_CAN_MSG_FLAG_EXTENDED : 0;
    DestMsg.flags   |= SrcMsg.m_ucRTR ? OCI_CAN_MSG_FLAG_REMOTE_FRAME : 0;
    memcpy(DestMsg.data, SrcMsg.m_ucData, sizeof(UCHAR) * 8);
}


#ifdef BOA_VERSION_1_5_FD
BOOL vCopy_2_OCI_CANFD_Data(OCI_CANFD_TxMessage& DestMsg, const STCAN_MSG& SrcMsg)
{
    DestMsg.res     = 0;
    DestMsg.frameID = SrcMsg.m_unMsgID;
    DestMsg.res     = SrcMsg.m_ucDataLen;
    if(SrcMsg.m_ucDataLen>8 && (SrcMsg.m_ucDataLen != 12)&& (SrcMsg.m_ucDataLen != 16)&&(SrcMsg.m_ucDataLen != 20)&&(SrcMsg.m_ucDataLen != 32)
            && (SrcMsg.m_ucDataLen != 48)&& (SrcMsg.m_ucDataLen != 64))
    {
        AfxMessageBox("Unsupported Data Length. It should be 0..8, 12, 16, 20, 24, 32, 48 or 64");
        return FALSE;
    }
    DestMsg.flags   = SrcMsg.m_ucEXTENDED ? OCI_CAN_MSG_FLAG_EXTENDED : 0;
    DestMsg.flags   |= SrcMsg.m_ucRTR ? OCI_CAN_MSG_FLAG_REMOTE_FRAME : 0;
    memcpy(DestMsg.data, SrcMsg.m_ucData, sizeof(UCHAR) * 8);
    return TRUE;
}
#endif

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Performs closure operations.
 */
HRESULT CDIL_CAN_ETAS_BOA::performClosureOperations(void)
{
    HRESULT hResult = S_OK;

    stopHardware();
    //deselect hw interface
    hResult = deselectHardwareInterface();

    /* close the existing handle */
    CloseHandle(sg_hEvent);
    sg_hEvent = NULL;

    /* Remove all the existing clients */
    UINT ClientIndex = 0;
    while (clientCount > 0)
    {
        bRemoveClient(sg_asClientToBufMap[ClientIndex].clientId);
    }

    /* Delete the critical section */
    DeleteCriticalSection(&sg_DIL_CriticalSection);
    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_DRIVER_SELECTED;
    }
    return hResult;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Gets the time mode mapping of the hardware. CurrSysTime
 * will be updated with the system time ref.
 * TimeStamp will be updated with the corresponding timestamp.
 */
HRESULT CDIL_CAN_ETAS_BOA::getTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
    CurrSysTime = currentSystemTime;
    TimeStamp   = timeStamp;
    if(QueryTickCount != NULL)
    {
        *QueryTickCount = queryTickCount;
    }
    return S_OK;
}

/**
 * @return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This function will popup hardware selection dialog and gets the user selection of channels.
 *
 */
int ListHardwareInterfaces(HWND hParent, DWORD /*dwDriver*/, InterfaceHardware* psInterfaces, int* pnSelList, int& nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CWnd objMainWnd;
    objMainWnd.Attach(hParent);
    CHardwareListing HwList(psInterfaces, nCount, pnSelList, &objMainWnd);
    INT nRet = HwList.DoModal();
    objMainWnd.Detach();

    if ( nRet == IDOK)
    {
        nCount = HwList.nGetSelectedList(pnSelList);
        return 0;
    }
    else
    {
        return -1;
    }
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Lists the hardware interface available. sSelHwInterface
 * will contain the user selected hw interface.
 */
HRESULT CDIL_CAN_ETAS_BOA::listHardwareInterfaces(InterfaceHardwareList& asSelHwInterface, INT& nCount)
{
    //VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_DRIVER_SELECTED, ERR_IMPROPER_STATE);
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;
    OCI_URIName acURI[defNO_OF_CHANNELS];
    INT nFound = 0;

    UINT unDefaultChannelCnt = nCount;

    if (BOA_SUCCEEDED(OCI_FindCANController(acURI, defNO_OF_CHANNELS, &nFound)))
    {
        nCount = nFound;
        qsort((void*)acURI, nCount, sizeof(OCI_URIName), nCallBackStrCompareFn);

        if (nCount > 0)//Success only if there exists alteast one hw
        {
            InterfaceHardware psHWInterface[defNO_OF_CHANNELS];
            //set the current number of channels
            nCount = min(nCount, defNO_OF_CHANNELS);

            for (UINT i = 0; i < nCount; i++)
            {
                psHWInterface[i].interfaceId = 0;
                psHWInterface[i].vendor = 0;
                psHWInterface[i].deviceName = "";
                psHWInterface[i].interfaceName = acURI[i];
                psHWInterface[i].description = acURI[i];
            }

            if (nCount > 1)// List hw interface if there are more than one hw
            {
                /* If the default channel count parameter is set, prevent displaying the hardware selection dialog */
                if ( unDefaultChannelCnt && nCount >= unDefaultChannelCnt )
                {
                    for (UINT i = 0; i < unDefaultChannelCnt; i++)
                    {
                        sg_anSelectedItems[i] = i;
                    }
                    nCount  = unDefaultChannelCnt;
                }
                else if ( ListHardwareInterfaces(NULL, DRIVER_CAN_ETAS_BOA, psHWInterface, sg_anSelectedItems, nCount) != 0 )
                {
                    /* return if user cancels hardware selection */
                    return HW_INTERFACE_NO_SEL;
                }
            }

            sg_nNoOfChannels = min(nCount, defNO_OF_CHANNELS);
            for (UINT nList = 0; nList < sg_nNoOfChannels; nList++)
            {
                asSelHwInterface[nList].interfaceName = psHWInterface[sg_anSelectedItems[nList]].interfaceName;
                asSelHwInterface[nList].description = psHWInterface[sg_anSelectedItems[nList]].description;
                asSelHwInterface[nList].interfaceId = 100 + nList; // Give a dummy number
            }

            sg_bCurrState = STATE_HW_INTERFACE_LISTED;
            hResult = S_OK;
        }
        else
        {
            hResult = NO_HW_INTERFACE;
        }
    }
    return hResult;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Selects the hardware interface selected by the user.
 */
HRESULT CDIL_CAN_ETAS_BOA::selectHardwareInterface(const InterfaceHardwareList& asSelHwInterface, INT /*nCount*/)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;

    //First select only supported number of HW interfaces
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        strcpy_s(sg_asChannel[i].uri, asSelHwInterface[i].interfaceName.c_str());
    }

    // Create the controller instance.
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        BOA_ResultCode err =  (*(sBOA_PTRS.m_sOCI.createCANController))(sg_asChannel[i].uri,
                              &(sg_asChannel[i].controllerHandle));
        if (BOA_SUCCEEDED(err))
        {
            /* Assign to userdata of QueueCfg. This will be useful to differentiate
            between the controller */
            sg_asChannel[i].canRxQueueConfiguration.onFrame.userData = (void*)sg_asChannel[i].controllerHandle;
            sg_asChannel[i].canRxQueueConfiguration.onEvent.userData = (void*)sg_asChannel[i].controllerHandle;
            BOA_ResultCode ErrorCode = OCI_FAILURE;

#ifndef BOA_VERSION_1_5_FD
            //configure the controller first
            ErrorCode = (*(sBOA_PTRS.m_sOCI.openCANController))(sg_asChannel[i].controllerHandle,
                        &(sg_asChannel[i].canConfiguration),
                        &(sg_asChannel[i].canControllerProperties));
#else
            //configure the controller first
            ErrorCode = (*(sBOA_PTRS.m_sOCI.openCANFDController))(sg_asChannel[i].controllerHandle,
                        &(sg_asChannel[i].canFdConfiguration),
                        &(sg_asChannel[i].canControllerProperties));
#endif
            if (BOA_SUCCEEDED(ErrorCode))
            {
                if (ManageQueue(QUEUE_ADD, i) == S_OK)
                {
                    if (ManageFilters(FILTER_ADD, i) == S_OK)
                    {
                        hResult = (*(sBOA_PTRS.m_sOCI.timeVTable.getTimerCapabilities))(sg_asChannel[i].controllerHandle,
                                  &(sg_asChannel[i].timerCapabilities));
                        if (hResult == S_OK)
                        {
                            sg_asChannel[i].resolution = (float)10000 /
                                                         (float)(sg_asChannel[i].timerCapabilities.tickFrequency);
                        }
                        else
                        {
                            hResult = ERR_LOAD_HW_INTERFACE;
                            sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not get timer resolution"));
                        }
                    }
                    else
                    {
                        hResult = ERR_LOAD_HW_INTERFACE;
                        sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not add filters"));
                    }
                }
                else
                {
                    hResult = ERR_LOAD_HW_INTERFACE;
                    sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not create rx queue"));
                }

            }
            else
            {
                hResult = ERR_LOAD_HW_INTERFACE;
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not open controller"));
            }
        }
        else
        {
            hResult = ERR_LOAD_HW_INTERFACE;
            sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not create controller"));
        }
    }

    //Check for the success
    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    }
    return hResult;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Deselects the selected hardware interface.
 */
HRESULT CDIL_CAN_ETAS_BOA::deselectHardwareInterface(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        if ((hResult = ManageFilters(FILTER_REMOVE, i)) == S_OK)
        {
            if ((hResult = ManageQueue(QUEUE_DESTROY, i)) == S_OK)
            {
                if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.closeCANController))(sg_asChannel[i].controllerHandle)))
                {
                    if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.destroyCANController))(sg_asChannel[i].controllerHandle)))
                    {
                        hResult |= S_OK;
                    }
                    else
                    {
                        hResult = S_FALSE;
                        sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not destroy controller"));
                    }
                }
                else
                {
                    hResult = S_FALSE;
                    sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not close controller"));
                }
            }
            else
            {
                hResult = S_FALSE;
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not destroy the queue"));
            }
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not remove the filter"));
        }
    }

    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_HW_INTERFACE_LISTED;
    }
    return hResult;
}

/**
 * @return TRUE for success, FALSE for failure
 *
 * Call back function called from ConfigDialogDIL
 */
BOOL Callback_DILBOA(BYTE /*Argument*/, PSCONTROLLER_DETAILS pDatStream, INT /*Length*/)
{
    return (sg_pouDIL_CAN_ETAS_BOA->setConfigurationData( pDatStream, 0) == S_OK);
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Displays the configuration dialog for controller
 */
int DisplayConfigurationDlg(HWND /*hParent*/, DILCALLBACK /*ProcDIL*/,
                            PSCONTROLLER_DETAILS pControllerDetails, UINT nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    int nResult = WARNING_NOTCONFIRMED;

    CChangeRegisters_CAN_ETAS_BOA ouChangeRegister(NULL, pControllerDetails, nCount);
    ouChangeRegister.DoModal();

    nResult = ouChangeRegister.nGetInitStatus();

    return nResult;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Displays the controller configuration dialog.
 * Fields are initialized with values supplied by InitData.
 * InitData will be updated with the user selected values.
 */
HRESULT CDIL_CAN_ETAS_BOA::displayConfigurationDialog(PSCONTROLLER_DETAILS InitData, int& Length)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    VALIDATE_POINTER_RETURN_VAL(InitData, WARN_INITDAT_NCONFIRM);

    USES_CONVERSION;

    INT Result = WARN_INITDAT_NCONFIRM;
    PSCONTROLLER_DETAILS psContrlDets = (PSCONTROLLER_DETAILS)InitData;
    //First initialize with existing hw description
    for (INT i = 0; i < min(Length, (INT)sg_nNoOfChannels); i++)
    {
        psContrlDets[i].hardwareDescription = sg_asChannel[i].uri;
    }
    if (sg_nNoOfChannels > 0)
    {
        Result = DisplayConfigurationDlg(sg_hOwnerWnd, Callback_DILBOA,
                                         psContrlDets, sg_nNoOfChannels);
        switch (Result)
        {
            case WARNING_NOTCONFIRMED:
            {
                Result = WARN_INITDAT_NCONFIRM;
            }
            break;
            case INFO_INIT_DATA_CONFIRMED:
            {
                Length = sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS;
                Result = setConfigurationData(psContrlDets, Length);
                if (Result == S_OK)
                {
                    Result = INFO_INITDAT_CONFIRM_CONFIG;
                }
            }
            break;
            case INFO_RETAINED_CONFDATA:
            {
                Result = INFO_INITDAT_RETAINED;
            }
            break;
            case ERR_CONFIRMED_CONFIGURED: // Not to be addressed at present
            case INFO_CONFIRMED_CONFIGURED:// Not to be addressed at present
            default:
            {
                // Do nothing... default return value is S_FALSE.
            }
            break;
        }
    }

    return Result;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Sets the controller configuration data supplied by InitData.
 */
HRESULT CDIL_CAN_ETAS_BOA::setConfigurationData(PSCONTROLLER_DETAILS pInitData, int /*Length*/)
{
    HRESULT hResult = WARNING_NOTCONFIRMED;

    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    VALIDATE_POINTER_RETURN_VAL(pInitData, hResult);

    BOA_ResultCode ErrCode = OCI_FAILURE;
    PSCONTROLLER_DETAILS pControllerDetails = (PSCONTROLLER_DETAILS)pInitData;
    bLoadDataFromContr(pControllerDetails);
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        //First remove all the filters
        ManageFilters(FILTER_REMOVE, i);
        //First remove all the Rx Tx queues
        ManageQueue(QUEUE_DESTROY, i);
        // if controller is open, close the controller. Do not bother about return value
        ErrCode = (*sBOA_PTRS.m_sOCI.closeCANController)(sg_asChannel[i].controllerHandle);
#ifndef BOA_VERSION_1_5_FD
        //Now load the controller config and open the controller
        ErrCode = (*sBOA_PTRS.m_sOCI.openCANController)(sg_asChannel[i].controllerHandle,
                  &(sg_asChannel[i].canConfiguration),
                  &(sg_asChannel[i].canControllerProperties));
#else
        //Now load the controller config and open the controller
        ErrCode = (*sBOA_PTRS.m_sOCI.openCANFDController)(sg_asChannel[i].controllerHandle,
                  &(sg_asChannel[i].canFdConfiguration),
                  &(sg_asChannel[i].canControllerProperties));
#endif
        /* Fill the hardware description details */
        ((PSCONTROLLER_DETAILS)pInitData)[i].hardwareDescription =
            sg_asChannel[i].uri;

        if (BOA_SUCCEEDED(ErrCode))
        {
            // Rx Tx queue
            if (ManageQueue(QUEUE_ADD, i) == S_OK)
            {
                if (ManageFilters(FILTER_ADD, i) == S_OK)
                {
                    hResult |= S_OK;
                }
            }
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not configure the controller"));
        }
    }
    return hResult;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Starts the controller.
 */
HRESULT CDIL_CAN_ETAS_BOA::startHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.getCANControllerProperties))(sg_asChannel[i].controllerHandle,
                &(sg_asChannel[i].canControllerProperties))))
        {
            if ((sg_asChannel[i].canControllerProperties.mode == OCI_CONTROLLER_MODE_SUSPENDED)
                    || (sg_asChannel[i].canControllerProperties.mode == OCI_CONTROLLER_MODE_RUNNING))
            {
                sg_asChannel[i].canControllerProperties.mode = OCI_CONTROLLER_MODE_RUNNING;
                if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.setCANControllerProperties))(sg_asChannel[i].controllerHandle,
                        &(sg_asChannel[i].canControllerProperties))))
                {
                    hResult |= S_OK;
                }
                else
                {
                    hResult = S_FALSE;
                    sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not start the controller in running mode"));
                }
            }
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not get controller mode"));
        }
    }
    //Check for the success
    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_CONNECTED;
        SetEvent(sg_hEvent);
        vCreateTimeModeMapping(sg_hEvent);

        InitializeCriticalSection(&sg_CritSectForAckBuf);


        QueryPerformanceCounter(&queryTickCount);
        // Get frequency of the performance counter
        QueryPerformanceFrequency(&frequency);
        // Convert it to time stamp with the granularity of hundreds of microsecond
        if ((queryTickCount.QuadPart * 10000) > frequency.QuadPart)
        {
            timeStamp = (queryTickCount.QuadPart * 10000) / frequency.QuadPart;
        }
        else
        {
            timeStamp = (queryTickCount.QuadPart / frequency.QuadPart) * 10000;
        }
    }

    return hResult;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Stops the controller.
 */
HRESULT CDIL_CAN_ETAS_BOA::stopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.getCANControllerProperties))(sg_asChannel[i].controllerHandle,
                &(sg_asChannel[i].canControllerProperties))))
        {
            if (sg_asChannel[i].canControllerProperties.mode == OCI_CONTROLLER_MODE_RUNNING)
            {
                sg_asChannel[i].canControllerProperties.mode = OCI_CONTROLLER_MODE_SUSPENDED;
                if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.setCANControllerProperties))(sg_asChannel[i].controllerHandle,
                        &(sg_asChannel[i].canControllerProperties))))
                {
                    hResult |= S_OK;
                }
                else
                {
                    hResult = S_FALSE;
                    sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not stop the controller in suspended mode"));
                }
            }
            else if (sg_asChannel[i].canControllerProperties.mode == OCI_CONTROLLER_MODE_SUSPENDED)
            {
                hResult |= S_OK;
            }
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not get controller mode"));
        }
    }
    //Check for the success
    if (hResult == S_OK)
    {
        sg_byCurrState = CREATE_MAP_TIMESTAMP;
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
        DeleteCriticalSection(&sg_CritSectForAckBuf);
    }

    return hResult;
}

/**
 * @return S_OK for success, S_FALSE for failure
 *
 * Function to get Controller status
 */
HRESULT CDIL_CAN_ETAS_BOA::getCurrentStatus(s_STATUSMSG& StatusData)
{
    StatusData.controllerStatus = NORMAL_ACTIVE;

    return S_OK;
}

/**
 * Sends STCAN_MSG structure from the client dwClientID.
 */
HRESULT CDIL_CAN_ETAS_BOA::sendMessage(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_FALSE;
    if (bClientIdExist(dwClientID))
    {
        if (sCanTxMsg.m_ucChannel <= sg_nNoOfChannels)
        {
            static OCI_CANMessage sOciCanMsg;
            static OCI_CANTxMessage sOciTxCanMsg;
            static SACK_MAP sAckMap;
            vCopy_2_OCI_CAN_Data(sOciTxCanMsg, sCanTxMsg);
            sOciCanMsg.type = OCI_CAN_TX_MESSAGE;
            sOciCanMsg.reserved = 0;
            uint32 nRemaining = 0;
            memcpy(&(sOciCanMsg.data.txMessage), &(sOciTxCanMsg), sizeof(OCI_CANTxMessage));
            sAckMap.clientId = dwClientID;
            sAckMap.channel  = sCanTxMsg.m_ucChannel;
            sAckMap.messageId    = sOciCanMsg.data.txMessage.frameID;
            vMarkEntryIntoMap(sAckMap);
            BOA_ResultCode ErrCode;
#ifdef BOA_VERSION_1_5_FD
            if(sCanTxMsg.m_bCANFD == false)
            {
                ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.writeCANData))
                          (sg_asChannel[sCanTxMsg.m_ucChannel - 1].canTxQueueHandle, OCI_NO_TIME, &sOciCanMsg, 1, &nRemaining);
            }
            else
            {
                OCI_CANFD_TxMessage     sCANFDMsg;
                if(vCopy_2_OCI_CANFD_Data(sCANFDMsg,sCanTxMsg) == FALSE)
                {
                    return ERR_WRITE_INVALID_SIZE;
                }
                sOciCanMsg.type = OCI_CANFD_TX_MESSAGE;
                memcpy(&(sOciCanMsg.data.txMessage), &(sOciTxCanMsg), sizeof(OCI_CANFD_TxMessage));
                BOA_ResultCode ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.writeCANData))
                                         (sg_asChannel[sCanTxMsg.m_ucChannel - 1].canTxQueueHandle, OCI_NO_TIME, &sOciCanMsg, 1, &nRemaining);
            }
#else
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.writeCANData))
                      (sg_asChannel[sCanTxMsg.m_ucChannel - 1].canTxQueueHandle, OCI_NO_TIME, &sOciCanMsg, 1, &nRemaining);
#endif
            if (BOA_SUCCEEDED(ErrCode))
            {
                hResult = S_OK;
            }
            else
            {
                sg_pIlog->logMessage(A2T(__FILE__), __LINE__, _("could not write CAN data onto bus"));
            }
        }
        else
        {
            hResult = ERR_INVALID_CHANNEL;
        }
    }
    else
    {
        hResult = ERR_NO_CLIENT_EXIST;
    }
    return hResult;
}

/**
 * Gets the controller param eContrParam of the channel.
 * Value stored in lParam.
 */
HRESULT CDIL_CAN_ETAS_BOA::getControllerParameters(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_HW_INTERFACE_SELECTED) || (sg_bCurrState == STATE_CONNECTED))
    {
        switch (eContrParam)
        {
            case NUMBER_HW:
            {
                lParam = sg_nNoOfChannels;
            }
            break;
            case NUMBER_CONNECTED_HW:
            {
                lParam = sg_nNoOfChannels;
            }
            break;
            case DRIVER_STATUS:
            {
                lParam = sg_bIsDriverRunning;
            }
            break;
            case HW_MODE:
            {
                if (nChannel < sg_nNoOfChannels)
                {
                    lParam = defMODE_ACTIVE;
                }
                else
                {
                    //unknown
                    lParam = defCONTROLLER_BUSOFF + 1;
                }

            }
            break;
            case CON_TEST:
            {
                lParam = TRUE;
            }
            break;
            default:
            {
                hResult = S_FALSE;
            }
            break;

        }
    }
    else
    {
        hResult = ERR_IMPROPER_STATE;
    }
    return hResult;
}

HRESULT CDIL_CAN_ETAS_BOA::setControllerParameters(int nValue, ECONTR_PARAM eContrparam)
{
    switch(eContrparam)
    {
        case HW_MODE:
        {
            switch(nValue)
            {
                case defMODE_ACTIVE:
                {
                    for (UINT i = 0; i < sg_nNoOfChannels;)
                    {
                        OCI_CANControllerProperties val;
                        val.mode = OCI_CONTROLLER_MODE_RUNNING;
                        (*(sBOA_PTRS.m_sOCI.setCANControllerProperties))(sg_asChannel[i].controllerHandle,
                                &val);
                        i++;

                    }
                }
                break;
                case defMODE_PASSIVE:
                {
                    for (UINT i = 0; i < sg_nNoOfChannels;)
                    {
                        OCI_CANControllerProperties val;
                        val.mode  = OCI_CONTROLLER_MODE_SUSPENDED;
                        (*(sBOA_PTRS.m_sOCI.setCANControllerProperties))(sg_asChannel[i].controllerHandle,
                                &val);
                        i++;

                    }
                }
                break;
            }
        }
    }
    return S_OK;
}

/**
 * Gets the error counter for corresponding channel.
 */
HRESULT CDIL_CAN_ETAS_BOA::getErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_CONNECTED) || (sg_bCurrState == STATE_HW_INTERFACE_SELECTED))
    {
        if (nChannel <= sg_nNoOfChannels)
        {
            if (eContrParam == ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = sg_asChannel[nChannel].txErrorCounter;
                sErrorCnt.m_ucRxErrCount = sg_asChannel[nChannel].rxErrorCounter;
            }
            else if (eContrParam == PEAK_ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = sg_asChannel[nChannel].peakTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = sg_asChannel[nChannel].peakRxErrorCounter;
            }
        }
        else
        {
            hResult = ERR_INVALID_CHANNEL;
        }
    }
    else
    {
        hResult = ERR_IMPROPER_STATE;
    }
    return hResult;
}
