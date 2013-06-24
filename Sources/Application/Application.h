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
 * @file      Application.h
 * @brief     CCANMonitorApp class implementation file
 * @author    Anish kumar
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CCANMonitorApp class implementation file
 */

#pragma once

/* Project includes */
#include "Include/BaseDefs.h"
#include "PSDI_Extern.h"
#include "CommonClass/MsgContainerBase.h"
#include "include/ModuleID.h"
#include "FrameProcessor/FrameProcessor_extern.h"
#include "FrameProcessor/BaseFrameProcessor_CAN.h"
#include "Struct.h"
#include "Utility/Utility.h"
#include "BUSMASTER_Interface.h"
#include "BUSMASTER.h"
#include "MainFrm.h"
#include "DIL_interface/BaseDIL_CAN.h"
#include "InterfaceGetter.h"

extern CCANMonitorApp theApp;

extern SBUSSTATISTICS g_sBusStatistics[ defNO_OF_CHANNELS ];
CCANBufFSE g_ouCanBufForCOM;
CPARAM_THREADPROC g_ouCOMReadThread;
static HANDLE g_hndPIPE[8]      = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
static HANDLE g_hndEvent[8]     = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
static SHORT  g_shUniqueID[8]   = {-1,-1,-1,-1,-1,-1,-1,-1};
static BYTE   g_bytClientCount  = 0;
static bool   g_bInitCOMThread = false;

/* PIPE related declarations */
#define BASE_PIPENAME   "\\\\.\\Pipe\\"
#define PIPE_TIMEOUT    500
#define PIPE_BUFFER_SIZE 131072
#define PASSTHRUMSG_SIZE sizeof(CAN_MSGS)

class CApplication : public CCmdTarget
{
public:
    CApplication(void);
    ~CApplication(void);

    virtual void OnFinalRelease()
    {
        CCmdTarget::OnFinalRelease();
    }

    void vInitializeCOMReadBuffer();
    BOOL bStartCOMReadThread();
    BOOL bStopCOMReadThread();
    void ReadCOMDataBuffer();

    DECLARE_DYNCREATE(CApplication)
    DECLARE_OLECREATE(CApplication)
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()

protected:
    // Connection point for ISMCLink interface
    BEGIN_CONNECTION_PART(CApplication, AppEvents)
    CONNECTION_IID(IID__IAppEvents)
    END_CONNECTION_PART(AppEvents)

    DECLARE_CONNECTION_MAP()

public:
    enum
    {
        dispidConnect               = 1L,
        dispidSetHwFilter           = 2L,
        dispidGetHwFilter           = 3L,
        dispidBuildAllDll           = 4L,
        dispidBuildLoadAllDll       = 5L,
        dispidLoadAllDll            = 6L,
        dispidUnLoadAllDll          = 7L,
        dispidStartTxMsgBlock       = 8L,
        dispidStopTxMsgBlock        = 9L,
        dispidReSetSW               = 11L,
        dispidStartLogging          = 12L,
        dispidDisplayWindow         = 13L,
        dispidSetBaudRate           = 14L,
        dispidSetControllerMode     = 15L,
        dispidGetMsgInfo            = 16L,
        dispidGetControllerMode     = 17L,
        dispidSendKeyValue          = 18L,
        dispidLoadConfiguration     = 19L,
        dispidImportDatabase        = 20L,
        dispidEnableDisableHandlers = 21L,
        dispidGetErrorCounter       = 22L,
        dispidSendCANMSg            = 23L,
        dispidGetNetworkStatistics  = 24L,
        dispidGetBaudRate           = 25L,
        dispidStopLogging           = 26L,
        dispidWriteToLogFile        = 27L,
        dispidSaveConfiguration     = 28L,
        dispidAddLoggingBlock       = 29L,
        dispidSaveConfigurationAs   = 30L,
        dispidAddTxBlock            = 31L,
        dispidGetTxBlockCount       = 32L,
        dispidGetTxBlock            = 33L,
        dispidDeleteTxBlock         = 34L,
        dispidClearTxBlockList      = 35L,
        dispidAddMsgToTxBlock       = 36L,
        dispidGetMsgCount           = 37L,
        dispidGetMsgFromTxBlock     = 38L,
        dispidDeleteMsgFromTxBlock  = 39L,
        dispidClearMsgList          = 40L,
        dispidAddFilterScheme       = 41L,
        dispidGetFilterScheme       = 42L,
        dispidGetFilterSchCount     = 43L,
        dispidUpdateFilterSch       = 44L,
        dispidGetFilterCountInSch   = 45L,
        dispidGetFilterInFilterSch  = 46L,
        dispidDeleteFilterInSch     = 47L,
        dispidEnableFilterSch       = 48L,
        dispidAddSimulatedSystem      = 49L,
        dispidGetSimulatedSystemCount = 50L,
        dispidGetSimulatedSystemName  = 51L,
        dispidDeleteSimulatedSystem   = 52L,
        dispidRemoveLoggingBlock      = 53L,
        dispidGetLoggingBlockCount    = 54L,
        dispidClearLoggingBlockList   = 55L,
        dispidGetLoggingBlock         = 56L,
        dispidRegisterClientForRx     = 57L,
        dispidUnRegisterClient        = 58L
    };

	/**
	 * Connect/disconnect the tool
	 */
    HRESULT Connect(BOOL bConnect)
    {
        HRESULT hResult = E_POINTER;
        CFlags* pFlags = GetIFlags();
        CMainFrame* pMainFrm = GetIMainFrame();

        if ((NULL != pFlags) && (NULL != pMainFrm))
        {
            if ((BOOL) pFlags->nGetFlagStatus(CONNECTED) != bConnect)
            {
                if ( bConnect )
                {
                    vInitializeCOMReadBuffer();
                }
                pMainFrm->COM_ConnectTool();
                hResult = S_OK;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /**
	 * Provide's the error count
	 */
    HRESULT GetErrorCounter(UCHAR* Tx, UCHAR* Rx, INT nChannel)
    {
        HRESULT hResult = E_POINTER;
        CBaseDIL_CAN* pDIL_CAN = GetICANDIL();

        if (NULL != pDIL_CAN)
        {
            SERROR_CNT sErrorCount;
            hResult = pDIL_CAN->getErrorCount(sErrorCount, nChannel, ERR_CNT);

            if (S_OK == hResult)
            {
                *Tx = sErrorCount.m_ucTxErrCount;
                *Rx = sErrorCount.m_ucRxErrCount;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /**
	 * sends the message to registered clients
	 *
	 * @param[in] sMsg message details
	 */
    void vSendCANMsgToClients(CAN_MSGS sMsg)
    {
        const CPtrArray* pConnections = m_xAppEvents.GetConnections ();
        ASSERT (pConnections != NULL);
        int nConnections = pConnections->GetSize ();
        if (nConnections)
        {
            for (int i=0; i<nConnections; i++)
            {
                _IAppEvents* pInterface = (_IAppEvents*) (pConnections->GetAt (i));
                ASSERT (pInterface != NULL);

                // Outgoing!
                HRESULT hr = pInterface->OnMessageReceived (sMsg);
                hr = S_OK;
            }
        }
    }

    /**
	 * sends the message
	 *
	 * @param[in] psMsg message details
	 */
    HRESULT SendCANMSg(CAN_MSGS* psMsg)
    {
        HRESULT hResult = E_POINTER;
        CBaseDIL_CAN* pDIL_CAN = GetICANDIL();
        CMainFrame* pMainFrm = GetIMainFrame();

        if ((NULL != pDIL_CAN) && (NULL != psMsg))
        {
            STCAN_MSG sTempMsg = {'\0'};
            sTempMsg.m_ucChannel  = psMsg->m_ucChannel;
            sTempMsg.m_ucDataLen  = psMsg->m_ucDataLen;
            sTempMsg.m_ucEXTENDED = psMsg->m_bEXTENDED;
            sTempMsg.m_ucRTR      = psMsg->m_bRTR;
            sTempMsg.m_unMsgID    = psMsg->m_unMsgID;

            // Copy the data bytes
            for (int i = 0; i < sTempMsg.m_ucDataLen; i++)
            {
                sTempMsg.m_ucData[i] = psMsg->m_ucData[i];
            }
            /* Client ID should be set to CAN_MONITOR_NODE ID */
            hResult = pDIL_CAN->sendMessage(pMainFrm->dwGetMonitorNodeClientID()
                                             , sTempMsg);
        }

        return hResult;
    }

    /**
	 * Provides the Message structure
	 *
	 * @param[in] MsgName message name
	 * @param[out] psMsgStruct message structure
	 */
    HRESULT GetMsgInfo(BSTR MsgName, sMESSAGESTRUCT* psMsgStruct)
    {
        HRESULT hResult = E_POINTER;
        CMsgSignal* pMsgSignal = GetIMsgDB();

        if (NULL != pMsgSignal)
        {
            char acStr[256] = {'\0'};
            BSTR_2_PCHAR(MsgName, acStr, 256);
            CString omMsgName(acStr);
            sMESSAGE* psMessage = pMsgSignal->psGetMessagePointer(omMsgName);

            if (NULL != psMessage)
            {
                psMsgStruct->m_dMessageCode = psMessage->m_unMessageCode;
                psMsgStruct->m_dNumberOfSignals = psMessage->m_unNumberOfSignals;
                psMsgStruct->m_dMessageLength = psMessage->m_unMessageLength;
                psMsgStruct->m_bMessageFrameFormat = psMessage->m_bMessageFrameFormat;
                psMsgStruct->m_nMsgDataFormat = psMessage->m_nMsgDataFormat;
                hResult = S_OK;
            }
        }

        return hResult;
    }

    /**
	 * Provides all the statistics
	 *
	 * @param[in] nChannel Channel no., statistic structure
	 * @param[out] psStat Statistics of the channel
	 */
    HRESULT GetNetworkStatistics(int nChannel, sBUSSTATISTICS_USR* psStat)
    {
        HRESULT hResult = E_POINTER;
        CBaseBusStatisticCAN* pBusStatCAN = GetICANBusStat();

        if ((NULL != pBusStatCAN) && (NULL != psStat))
        {
            SBUSSTATISTICS sBusStat = {'\0'};
            hResult = pBusStatCAN->getBusStatistics((UINT) nChannel, sBusStat);
            memcpy(psStat, &sBusStat, sizeof(SBUSSTATISTICS));
        }

        return hResult;
    }

    /**
	 * Post the message to CMainframe LoadAllDll
	 */
    HRESULT LoadAllDll()
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            CStringArray omStrBuildFiles;

            if (pMainFrm->COM_bDllLoad(&omStrBuildFiles))
            {
                hResult = S_OK;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /**
	 * Post the message to CMainframe UnLoadAllDll
	 */
    HRESULT UnLoadAllDll()
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            CStringArray omStrBuildFiles;

            if (pMainFrm->COM_bDllUnload(&omStrBuildFiles))
            {
                hResult = S_OK;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /**
	 * It post the key message to main frame
	 *
	 * @param[in] KeyVal Key's Ascii value
	 */
    HRESULT SendKeyValue(UCHAR KeyVal)
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            pMainFrm->vKeyPressedInMsgWnd((WPARAM) KeyVal, (LPARAM) 0);
            hResult = S_OK;
        }

        return hResult;
    }

    /**
	 * enable/disable handlers
	 *
	 * @param[in] bEnable enable/disable
	 * @param[in] eType handler type
	 */
    HRESULT EnableDisableHandlers(BOOL bEnable, eHandlerType eType)
    {
        HRESULT hResult = E_POINTER;
        CFlags* pFlags = GetIFlags();
        CMainFrame* pMainFrm = GetIMainFrame();

        if ((NULL != pFlags) && (NULL != pMainFrm))
        {
            if (pFlags->nGetFlagStatus(CONNECTED))
            {
                switch (eType)
                {
                    case all:
                    {
                        if (pFlags->nGetFlagStatus(ALL_HANDLER) != bEnable)
                        {
                            pMainFrm->COM_EnableAllHandlers(bEnable);
                        }

                        hResult = S_OK;
                    }
                    break;

                    case timers:
                    {
                        // If timer button is enabled then only call the function
                        if (pFlags->nGetFlagStatus(TIMERBUTTON) != bEnable)
                        {
                        }

                        hResult = S_OK;
                    }
                    break;

                    case key:
                    {
                        if (pFlags->nGetFlagStatus(KEY_HANDLER_ON) != bEnable)
                        {
                        }

                        hResult = S_OK;
                    }
                    break;

                    case message :
                    {
                        if (pFlags->nGetFlagStatus(MSGHANDLERBUTTON) != bEnable)
                        {
                        }

                        hResult = S_OK;
                    }
                    break;

                    case error :
                    {
                        if (pFlags->nGetFlagStatus(ERROR_HANDLER) != bEnable)
                        {
                        }

                        hResult = S_OK;
                    }
                    break;

                    default:
                    {
                        hResult = E_INVALIDARG;
                    }
                }
            }
        }

        return hResult;
    }

    /**
	 * Calls the mainframe class function to load the configuration
	 */
    HRESULT LoadConfiguration(BSTR FileName)
    {
        HRESULT hResult = E_POINTER;
        CFlags* pFlags = GetIFlags();
        CMainFrame* pMainFrm = GetIMainFrame();

        if ((NULL != pFlags) && (NULL != pMainFrm))
        {
            if ((BOOL) pFlags->nGetFlagStatus(CONNECTED) == FALSE) // Loading of
            {
                // configuration is done iff the present node is disconnected.
                char acStr[_MAX_PATH] = {'\0'};
                BSTR_2_PCHAR(FileName, acStr, _MAX_PATH);
                CString omFilePath(acStr);

                if (pMainFrm->nLoadConfigFile(omFilePath) == 1)
                {
                    hResult = S_OK;
                }
                else
                {
                    hResult = E_FAIL;
                }
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /**
	 * import the database file
	 */
    HRESULT ImportDatabase(BSTR DBFilePath)
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            char acStr[_MAX_PATH] = {'\0'};
            BSTR_2_PCHAR(DBFilePath, acStr, _MAX_PATH);
            CString omFilePath(acStr);
            hResult = pMainFrm->dLoadDataBaseFile(omFilePath, TRUE);
        }

        return hResult;
    }

    /**
	 * Save the Configurations in file
	 */
    HRESULT SaveConfiguration()
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            if (pMainFrm->COM_SaveConfigFile())
            {
                hResult = S_OK;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /**
	 * Save the configuration to new file
	 *
	 * @param[in] ConfigPath Config File path
	 */
    HRESULT SaveConfigurationAs(BSTR ConfigPath)
    {
        char acStr[_MAX_PATH] = {'\0'};
        BSTR_2_PCHAR(ConfigPath, acStr, _MAX_PATH);
        CString omConfigPath(acStr);
        HRESULT hResult = E_FAIL;

        if (theApp.COM_nSaveConfiguration(omConfigPath) == defCONFIG_FILE_SUCCESS)
        {
            hResult = S_OK;
        }

        return hResult;
    }

	/**
	 * Starts sending messages if tool is connected
	 */
    HRESULT StartTxMsgBlock()
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            pMainFrm->COM_SendMessage();
            hResult = S_OK;
        }

        return hResult;
    }

    /**
	 * Stops sending messages
	 */
    HRESULT StopTxMsgBlock()
    {
        return StartTxMsgBlock();
    }

    /**
	 * Get Tx block count
	 *
	 * @param[out] Result No. of Tx blocks available
	 */
    HRESULT GetTxBlockCount (USHORT* Result)
    {
        *Result = (USHORT) TX_unGetTxBlockCount();
        return S_OK;
    }

    /**
	 * Delete Tx Block
	 */
    HRESULT DeleteTxBlock (USHORT /*BlockIndex*/)
    {
        return TX_vDeleteTxBlockMemory();
    }

    /**
	 * Clear Tx Block List
	 */
    HRESULT ClearTxBlockList(void)
    {
        return TX_vDeleteTxBlockMemory();
    }

	/**
     * Enables / disables a filter scheme
	 */
    HRESULT EnableFilterSch(EFILTERMODULE eModule, BOOL bEnable)
    {
        HRESULT hResult;

        switch (eModule)
        {
            case Log:
            {
                CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

                if (NULL != pCANLogger)
                {
                    USHORT LogBlocks = pCANLogger->FPC_GetLoggingBlockCount();
                    hResult = S_OK;

                    for (USHORT i = 0; (i < LogBlocks) && (S_OK == hResult); i++)
                    {
                        hResult = pCANLogger->FPC_EnableFilter(i, bEnable);
                    }
                }
                else
                {
                    hResult = E_POINTER;
                }
            }
            break;

            case Display:
            {
                //hResult = PSDI_EnableFilterApplied(bEnable);
                CMsgContainerBase* pouMsgContainerIntrf = NULL;
                PSDI_GetInterface(CAN, (void**)&pouMsgContainerIntrf);

                if(pouMsgContainerIntrf)
                {
                    pouMsgContainerIntrf->EnableFilterApplied(bEnable);
                }
            }
            break;

            default:
            {
                hResult = E_INVALIDARG;
                ASSERT(FALSE);
            }
            break;
        }

        return hResult;
    }

    /**
	 * Start logging if it is stopped
	 */
    HRESULT StartLogging()
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_EnableLogging(TRUE);
        }

        return hResult;
    }

    /**
	 * disable logging of messages
	 */
    HRESULT StopLogging()
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_EnableLogging(FALSE);
        }

        return hResult;
    }

    /**
	 * Writes a string to the log file specified
	 */
    HRESULT WriteToLogFile(USHORT /*BlockIndex*/, BSTR bstrStr)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            char acStr[256] = {'\0'};
            BSTR_2_PCHAR(bstrStr, acStr, 256);
            CString omStr(acStr);
            hResult = pCANLogger->FPC_LogString(omStr);
        }

        return hResult;
    }

    /**
	 * Registers a client for receiving Rx data through PIPE and event
	 */
    HRESULT RegisterClientForRx(USHORT usUniqueID, BSTR* pEventName, BSTR* pPIPEName)
    {
        HRESULT hResult = S_OK;

        /* if not more than 8 clients connected for Rx */
        if ( g_bytClientCount < 8 )
        {
            for ( int i = 0; i < 8 ; i++ )
            {
                if ( g_shUniqueID[i] == -1 )
                {
                    hResult = hCreatePipeAndEventForClient(usUniqueID, g_hndPIPE[i], g_hndEvent[i], pEventName, pPIPEName);
                    g_shUniqueID[i] = usUniqueID;

                    g_bytClientCount++;
                    break;
                }
            }
        }
        else
        {
            hResult = S_FALSE;
        }

        return hResult;
    }

    /**
	 * Unregisters a client for receiving Rx data
	 */
    HRESULT UnRegisterClient(USHORT usUniqueID)
    {
        HRESULT hResult = S_OK;

        for ( int i = 0; i < 8 ; i++ )
        {
            /* if the client ID exists */
            if ( usUniqueID == g_shUniqueID[i] )
            {
                /* Close all communication objects */

                DisconnectNamedPipe(g_hndPIPE[i]);
                CloseHandle(g_hndPIPE[i]);
                g_hndPIPE[i] = NULL;

                CloseHandle(g_hndEvent[i]);
                g_hndEvent[i] = NULL;

                g_shUniqueID[i] = -1;
                g_bytClientCount--;

                break;
            }
            /* if not found */
            else if ( i == 7 )
            {
                hResult = S_FALSE;
            }
        }


        return hResult;
    }

    /**
	 * Function to create PIPE handle for a client
	 */
    HRESULT  hCreatePipeAndEventForClient(USHORT usUniqueID, HANDLE& hndPipe, HANDLE& hndEvent, BSTR* pEventName, BSTR* pPIPEName)
    {
        char EventName[32] = {'\0'};
        sprintf_s(EventName, "%X", usUniqueID);
        // followed by the pipe name
        char PipeName[64] = BASE_PIPENAME;
        strcat_s(PipeName, EventName);

        // Pipe name; convert from ASCII string to BSTR
        BSTR bstrPipe = A2BSTR(PipeName);
        *pPIPEName = bstrPipe;
        // Mutex name; convert from ASCII string to BSTR
        BSTR bstrEvent = A2BSTR(EventName);
        *pEventName = bstrEvent;

        bool bProceed = ((bstrPipe != NULL) && (bstrEvent != NULL));

        static HANDLE hEvent = NULL;

        if ( bProceed )
        {
            /* Create named pipe for client for writing*/
            hndPipe = CreateNamedPipe(
                          PipeName,                 // pipe name
                          PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,     // read/write access
                          PIPE_TYPE_BYTE |          // message type pipe
                          PIPE_READMODE_BYTE |      // message-read mode
                          PIPE_NOWAIT,              // blocking mode
                          PIPE_UNLIMITED_INSTANCES, // max. instances
                          PIPE_BUFFER_SIZE,         // output buffer size
                          PASSTHRUMSG_SIZE,         // input buffer size
                          PIPE_TIMEOUT,             // client time-out
                          NULL);                    // no security attribute

            /* Set timeouts */
            COMMTIMEOUTS CommTimeouts;
            //BOOL   bStatus;

            CommTimeouts.ReadIntervalTimeout         = 0;
            CommTimeouts.ReadTotalTimeoutMultiplier  = 0;
            CommTimeouts.ReadTotalTimeoutConstant    = 1;
            CommTimeouts.WriteTotalTimeoutMultiplier = 0;
            CommTimeouts.WriteTotalTimeoutConstant   = 1000;

            //bStatus = SetCommTimeouts(hndPipe,&CommTimeouts);

            /* Create event */
            BOOL bRet = ConnectNamedPipe(hndPipe, NULL);
            /*if ( !bRet )
                return false;*/

            // Generate the communication event
            bProceed = bProceed && ((hndEvent= CreateEvent(NULL, FALSE, FALSE, EventName)) != NULL);

            if ( !bProceed )
            {
                return S_FALSE;
            }
        }
        else
        {
            return S_OK;
        }

        return S_OK;
    }

    /**
	 * Returns total number of logging blocks defined
	 */
    HRESULT GetLoggingBlockCount(USHORT* BlockCount)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            *BlockCount = pCANLogger->FPC_GetLoggingBlockCount();
            hResult = S_OK;
        }

        return hResult;
    }

    /**
	 * Deletes a logging block
	 */
    HRESULT RemoveLoggingBlock(USHORT BlockIndex)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_RemoveLoggingBlock(BlockIndex);
        }

        return hResult;
    }

    /**
	 * Clears the logging block list
	 */
    HRESULT ClearLoggingBlockList(void)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_ClearLoggingBlockList();
        }

        return hResult;
    }

    /**
	 * Reset software
	 */
    HRESULT ResetSW()
    {
        return E_NOTIMPL;
    }

    /**
	 * Display the windows in BUSMASTER
	 */
    HRESULT DisplayWindow(eWindow /*eWhichWindow*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Add a log file
	 *
	 * @param[in] psLoggingBlock Log file name, time mode, numeric mode, file mode for logging
	 */
    HRESULT AddLoggingBlock(SLOGGINGBLOCK_USR* /*psLoggingBlock*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Add transmission block
	 *
	 * @param[in] psTxBlock Tx block details
	 */
    HRESULT AddTxBlock(STXBLOCK_USR* /*psTxBlock*/)
    {
        return E_NOTIMPL;
    }

	/**
     * Get Tx block details
	 */
    HRESULT GetTxBlock (USHORT /*BlockIndex*/, STXBLOCK_USR* /*psTxBlock*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Add Msg To TxBlock
	 */
    HRESULT AddMsgToTxBlock(USHORT /*BlockIndex*/, CAN_MSGS* /*psMsg*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Get Msg Count of a TxBlock
	 */
    HRESULT getMessageCount(USHORT /*BlockIndex*/, USHORT* /*Result*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Get Msg info From TxBlock
	 */
    HRESULT GetMsgFromTxBlock(USHORT /*BlockIndex*/, USHORT /*MsgIndex*/, CAN_MSGS* /*psMsg*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Delete MsgFrom TxBlock
	 */
    HRESULT DeleteMsgFromTxBlock(USHORT /*BlockIndex*/, USHORT /*FrameIndex*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Clears the Msg list in a Tx block given its index
	 */
    HRESULT ClearMsgList(USHORT /*BlockIndex*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Defines a filter scheme which is identified by its name.
	 */
    HRESULT AddFilterScheme (BSTR /*pcFilterName*/, VARIANT_BOOL /*FilterType*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Given filterscheme index provide filter scheme name and type
	 */
    HRESULT GetFilterScheme(USHORT /*FilterSchINdex*/, BSTR /*pcFilterName*/,
                            VARIANT_BOOL* /*FilterType*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Get Filter Scheme Count
	 */
    HRESULT GetFilterSchCount(USHORT* /*pTotal*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Adds / sets a filter in a filter scheme
	 */
    HRESULT UpdateFilterSch(USHORT /*FilterSchIndex*/, SFILTER_USR* /*psFilter*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Get Filter Count In a filter Scheme
	 */
    HRESULT GetFilterCountInSch(USHORT /*FilterSchIndex*/, USHORT* /*pTotal*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Getter for a filter, given index of the filter scheme
	 * and index of the filter in the filter scheme
	 */
    HRESULT GetFilterInFilterSch(USHORT /*FilterSchIndex*/, USHORT /*FilterIndex*/,
                                 SFILTER_USR* /*psFilter*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Deletes a filter from a filter scheme
	 */
    HRESULT DeleteFilterInSch(USHORT /*FilterSchIndex*/, USHORT /*FilterIndex*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Renders a logging block based on its index in the list
	 */
    HRESULT GetLoggingBlock(USHORT /*BlockIndex*/, void* /*psLoggingBlock*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Add Simulated System in the configuration
	 */
    HRESULT AddSimulatedSystem(BSTR /*pcSimPath*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Get Simulated System Count
	 */
    HRESULT GetSimulatedSystemCount(USHORT* /*pSimTotal*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Get the Simulated System Name
	 */
    HRESULT GetSimulatedSystemName(USHORT /*SimSysIndex*/, BSTR* /*pcSimPath*/)
    {
        return E_NOTIMPL;
    }

    /**
	 * Delete the given Simulated System
	 */
    HRESULT DeleteSimulatedSystem(USHORT /*SimSysIndex*/)
    {
        return E_NOTIMPL;
    }

    BEGIN_INTERFACE_PART(LocalClass, IApplication)
    STDMETHOD(GetTypeInfoCount)(UINT FAR* pctinfo);
    STDMETHOD(GetTypeInfo)(
        UINT itinfo,
        LCID lcid,
        ITypeInfo FAR* FAR* pptinfo);
    STDMETHOD(GetIDsOfNames)(
        REFIID riid,
        OLECHAR FAR* FAR* rgszNames,
        UINT cNames,
        LCID lcid,
        DISPID FAR* rgdispid);
    STDMETHOD(Invoke)(
        DISPID dispidMember,
        REFIID riid,
        LCID lcid,
        WORD wFlags,
        DISPPARAMS FAR* pdispparams,
        VARIANT FAR* pvarResult,
        EXCEPINFO FAR* pexcepinfo,
        UINT FAR* puArgErr);

    STDMETHOD(GetMsgInfo)(BSTR MsgName,sMESSAGESTRUCT*  sMsgStruct);
    STDMETHOD(GetNetworkStatistics)(int nChannel, sBUSSTATISTICS_USR* sStat);
    STDMETHOD(SendCANMSg)(CAN_MSGS* sMsg);
    STDMETHOD(GetErrorCounter)(UCHAR* Tx, UCHAR* Rx, INT nChannel);
    STDMETHOD(EnableDisableHandlers)(BOOL bEnable,eHandlerType eType);
    STDMETHOD(ImportDatabase)(BSTR DBFilePath);
    STDMETHOD(LoadConfiguration)(BSTR FileName);
    STDMETHOD(SendKeyValue)(UCHAR keyval);
    //STDMETHOD(GetControllerMode)(eControllerMode *eMode);
    //STDMETHOD(SetControllerMode)(eControllerMode eMode);
    //STDMETHOD(GetBaudRate)(int nChannelNo, BYTE *bBTR0,BYTE *bBTR1);
    //STDMETHOD(SetBaudRate)(int nChannelNo, BYTE bBTR0,BYTE bBTR1);
    STDMETHOD(DisplayWindow)(eWindow eWhichWindow);
    STDMETHOD(WriteToLogFile)(USHORT BlockIndex, BSTR pcStr);
    STDMETHOD(StopLogging)();
    STDMETHOD(StartLogging)();
    STDMETHOD(ResetSW)();
    STDMETHOD(StopTxMsgBlock)();
    STDMETHOD(StartTxMsgBlock)();
    STDMETHOD(UnLoadAllDll)();
    STDMETHOD(LoadAllDll)();
    //STDMETHOD(BuildLoadAllDll)();
    //STDMETHOD(BuildAllDll)();
    //STDMETHOD(GetHwFilter)(BOOL *pnExtended, double *dBeginMsgId, double *dEndMsgId);
    //STDMETHOD(SetHwFilter)(BOOL pnExtended, double dBeginMsgId, double dEndMsgId);
    STDMETHOD(Connect)(BOOL bConnect);
    STDMETHOD(SaveConfiguration)();
    STDMETHOD(AddLoggingBlock)(SLOGGINGBLOCK_USR* psLoggingBlock);
    STDMETHOD(SaveConfigurationAs) (BSTR ConfigPath);
    STDMETHOD(AddTxBlock) (STXBLOCK_USR* psTxBlock);
    STDMETHOD(GetTxBlockCount) (USHORT* Result);
    STDMETHOD(GetTxBlock) (USHORT BlockIndex, STXBLOCK_USR* psTxBlock);
    STDMETHOD(DeleteTxBlock) (USHORT BlockIndex);
    STDMETHOD(ClearTxBlockList) ();
    STDMETHOD(AddMsgToTxBlock) (USHORT BlockIndex, CAN_MSGS* psMsg);
    STDMETHOD(GetMsgCount) (USHORT BlockIndex, USHORT* Result);
    STDMETHOD(GetMsgFromTxBlock)(USHORT BlockIndex, USHORT MsgIndex, CAN_MSGS* psMsg);
    STDMETHOD(DeleteMsgFromTxBlock) (USHORT BlockIndex, USHORT MsgIndex);
    STDMETHOD(ClearMsgList) (USHORT BlockIndex);
    STDMETHOD(AddFilterScheme) (BSTR pcFilterName, VARIANT_BOOL  FilterType);
    STDMETHOD(GetFilterScheme) (USHORT FilterSchINdex, BSTR pcFilterName, VARIANT_BOOL*  FilterType);
    STDMETHOD(GetFilterSchCount) (USHORT* pTotal);
    STDMETHOD(UpdateFilterSch) (USHORT FilterSchIndex, SFILTER_USR* psFilter);
    STDMETHOD(GetFilterCountInSch) (USHORT FilterSchIndex, USHORT* pTotal);
    STDMETHOD(GetFilterInFilterSch) (USHORT FilterSchIndex, USHORT FilterIndex, SFILTER_USR* psFilter);
    STDMETHOD(DeleteFilterInSch) (USHORT FilterSchIndex, USHORT FilterIndex);
    STDMETHOD(EnableFilterSch) (EFILTERMODULE eModule, BOOL Enable);
    STDMETHOD(AddSimulatedSystem)(BSTR pcSimPath);
    STDMETHOD(GetSimulatedSystemCount)(USHORT* pSimTotal);
    STDMETHOD(GetSimulatedSystemName)(USHORT SimSysIndex, BSTR* pcSimPath);
    STDMETHOD(DeleteSimulatedSystem)(USHORT SimSysIndex);
    STDMETHOD(RemoveLoggingBlock)(USHORT BlockIndex);
    STDMETHOD(GetLoggingBlockCount)(USHORT* BlockCount);
    STDMETHOD(ClearLoggingBlockList)(void);
    STDMETHOD(GetLoggingBlock)(USHORT BlockIndex, SLOGGINGBLOCK_USR* psLoggingBlock);
    STDMETHOD(RegisterClientForRx)( USHORT usUniqueID, BSTR* pEventName, BSTR* pPIPEName );
    STDMETHOD(UnRegisterClient)(USHORT usUniqueID);
    END_INTERFACE_PART(LocalClass)
};
