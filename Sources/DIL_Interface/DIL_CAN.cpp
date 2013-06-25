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
 * @brief     Interface file for CAN BUS
 * @author    Pradeep Kadoor, Arunkumar Karri
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CAN BUS
 */

/* Project includes */
#include "DIL_Interface_stdafx.h"
#include "include/Error.h"
#include "include/basedefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DILC_Dummy.h"
#include "CAN_MHS/CAN_MHS_Extern.h"
#include "CAN_PEAK_USB/CAN_PEAK_USB_Extern.h"
#include "CAN_ICS_neoVI/CAN_ICS_neoVI_Extern.h"
#include "CAN_STUB/CAN_STUB_Extern.h"
#include "CAN_ETAS_BOA/CAN_ETAS_BOA_Extern.h"
#include "CAN_Vector_XL/CAN_Vector_XL_Extern.h"
#include "CAN_Kvaser_CAN/CAN_Kvaser_CAN_Extern.h"
#include "Dil_CAN.h"
#include "Utility/MultiLanguageSupport.h"

typedef HRESULT (__stdcall* GETIDIL_CAN_CONTROLLER)(void** ppvInterface);
static GETIDIL_CAN_CONTROLLER pfGetIDILCAN_Controller;
static CDIL_CAN_DUMMY* sg_pouDIL_CAN_DUMMY = new CDIL_CAN_DUMMY;

class ENTRY_DIL
{
public:
    DWORD           m_dwDIL;
    std::string          m_acDIL;
};

static ENTRY_DIL sg_ListDIL[] =
{
    /* simulation should be the first entry... */
    {DRIVER_CAN_STUB,       "&Simulation"       },
    /* ...all other drivers should be in alphabetical order */
    {DRIVER_CAN_ETAS_BOA,   "ETAS &BOA"         },
    {DRIVER_CAN_ETAS_ES581, "ETAS &ES581"       },
    {DRIVER_CAN_ICS_NEOVI,  "&IntrepidCS neoVI" },
    {DRIVER_CAN_IXXAT,      "I&XXAT VCI"        },
    {DRIVER_CAN_KVASER_CAN, "&Kvaser CAN"       },
    {DRIVER_CAN_MHS,        "&MHS Tiny-CAN"     },
    {DRIVER_CAN_PEAK_USB,   "&PEAK USB"         },
    {DRIVER_CAN_VECTOR_XL,  "&Vector XL"        },
    {DRIVER_CAN_VSCOM,      "VScom &CAN-API"    },
};

CDIL_CAN::CDIL_CAN()
{
    m_hDll = NULL;
    m_hOldDll = NULL;
    driverId = DAL_NONE;
    pfGetIDILCAN_Controller = NULL;
    m_pBaseDILCAN_Controller = NULL;
    m_pOldBaseDILCAN_Controller = NULL;
    vSelectInterface_Dummy();
}

CDIL_CAN::~CDIL_CAN()
{
    //Free the currently selected DIL library
    if ( m_hDll )
    {
        FreeLibrary(m_hDll);
        m_hDll = NULL;
    }
    //Free the previosuly selected DIL library
    if ( m_hOldDll )
    {
        FreeLibrary(m_hOldDll);
        m_hOldDll = NULL;
    }
}

int CDIL_CAN::ExitInstance()
{
    return 0;
}

BOOL CDIL_CAN::InitInstance()
{
    vSelectInterface_Dummy();

    return TRUE;
}

void CDIL_CAN::vSelectInterface_Dummy(void)
{
    m_pBaseDILCAN_Controller = (CBaseDIL_CAN_Controller*) sg_pouDIL_CAN_DUMMY;
}

DWORD CDIL_CAN::getDriverList(DriverList* List)
{
    DWORD Result = 0;

    if (List != NULL)
    {
        for (int i = 0; i < sizeof(sg_ListDIL)/sizeof(ENTRY_DIL); i++)
        {
            (*List)[i].name = sg_ListDIL[i].m_acDIL;
            (*List)[i].driverId = sg_ListDIL[i].m_dwDIL;
            Result++;
        }
    }

    return Result;
}

HRESULT CDIL_CAN::selectDriver(DWORD dwDriverID, HWND hWndOwner,
                               Base_WrapperErrorLogger* pILog)
{
    USES_CONVERSION;

    HRESULT hResult = S_FALSE;

    if (getSelectedDriver() == dwDriverID)
    {
        hResult = DAL_ALREADY_SELECTED;
    }
    else if (pILog == NULL)
    {
        hResult = ERRLGR_INVALID;
    }
    else
    {
        if (hWndOwner == NULL)
        {
            /* Log a warning message informing about the invalidity of the
            owner window handle. */
        }

        //Save the currently selected DIL library handle
        if ( m_hDll )
        {
            //FreeLibrary(m_hDll);
            m_hOldDll = m_hDll;
            m_pOldBaseDILCAN_Controller = m_pBaseDILCAN_Controller;
            m_dwOldDriverID = driverId;
            m_hDll = NULL;
        }

        switch(dwDriverID)
        {
            case DRIVER_CAN_PEAK_USB:
                m_hDll = LoadLibrary("CAN_PEAK_USB.dll");
                break;

            case DRIVER_CAN_ICS_NEOVI:
            case DRIVER_CAN_ETAS_ES581:
                m_hDll = LoadLibrary("CAN_ICS_neoVI.dll");
                break;

            case DRIVER_CAN_ETAS_BOA:
                m_hDll = LoadLibrary("CAN_ETAS_BOA.dll");
                break;

            case DRIVER_CAN_VECTOR_XL:
                m_hDll = LoadLibrary("CAN_Vector_XL.dll");
                break;

            case DRIVER_CAN_IXXAT:
                m_hDll = LoadLibrary("CAN_IXXAT_VCI.dll");
                break;

            case DRIVER_CAN_KVASER_CAN:
                m_hDll = LoadLibrary("CAN_Kvaser_CAN.dll");
                break;

            case DRIVER_CAN_STUB:
                m_hDll = LoadLibrary("CAN_STUB.dll");
                break;

            case DRIVER_CAN_MHS:
                m_hDll = LoadLibrary("CAN_MHS.dll");
                break;

            case DRIVER_CAN_VSCOM:
                m_hDll = LoadLibrary("CAN_VSCOM.dll");
                break;

            case DAL_NONE:
                performClosureOperations();
                vSelectInterface_Dummy();
                driverId = DAL_NONE;
                return hResult;

            default:
                hResult = DAL_INVALID;
                driverId = DAL_NONE;
                return hResult;
        }

        if (m_hDll == NULL)
        {
            hResult = ERR_LOAD_DRIVER;
            pILog->logMessage(A2T(__FILE__), __LINE__, _("Load library failed..."));
        }
        else
        {
            // First select the dummy interface
            //selectDriver((DWORD)DAL_NONE, hWndOwner, pILog);

            pILog->logMessage(A2T(__FILE__), __LINE__, _("Load library successful..."));
            pfGetIDILCAN_Controller = (GETIDIL_CAN_CONTROLLER)GetProcAddress(m_hDll, "GetIDIL_CAN_Controller");
            pfGetIDILCAN_Controller((void**)&m_pBaseDILCAN_Controller);
            if ( m_pBaseDILCAN_Controller )
            {
                // set the application parameters.
                m_pBaseDILCAN_Controller->setApplicationParameters(hWndOwner, pILog);
                hResult = m_pBaseDILCAN_Controller->loadDriverLibrary();
                switch (hResult)
                {
                    case S_OK:
                    case DLL_ALREADY_LOADED:
                        driverId = dwDriverID;
                        hResult = S_OK;
                        break;

                    default:
                        hResult = ERR_LOAD_DRIVER;
                        pILog->logMessage(A2T(__FILE__), __LINE__, _("Load library failed..."));
                        /* New Code */
                        /* Get rid of current DIL library */
                        if ( m_pBaseDILCAN_Controller )
                        {
                            m_pBaseDILCAN_Controller->performClosureOperations();
                            m_pBaseDILCAN_Controller->unloadDriverLibrary();
                        }
                        FreeLibrary(m_hDll);
                        if ( m_hOldDll )
                        {
                            m_hDll = m_hOldDll;
                            m_pBaseDILCAN_Controller = m_pOldBaseDILCAN_Controller;
                            driverId =  m_dwOldDriverID;
                        }
                        /* New Code */
                        break;
                }
            }
        }
    }

    return hResult;
}

HRESULT CDIL_CAN::manageMessageBuffer(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
    return m_pBaseDILCAN_Controller->manageMessageBuffer(bAction, ClientID, pBufObj);
}

HRESULT CDIL_CAN::registerClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
    return m_pBaseDILCAN_Controller->registerClient(bRegister, ClientID, pacClientName);
}

DWORD CDIL_CAN::getSelectedDriver(void)
{
    return driverId;
}

HRESULT CDIL_CAN::performInitOperations(void)
{
    return m_pBaseDILCAN_Controller->performInitOperations();
}

HRESULT CDIL_CAN::performClosureOperations(void)
{
    HRESULT hResult =  S_FALSE;
    if ( m_pBaseDILCAN_Controller )
    {
        m_pBaseDILCAN_Controller->performClosureOperations();
        m_pBaseDILCAN_Controller->unloadDriverLibrary();
    }

    vSelectInterface_Dummy();
    driverId = DAL_NONE;
    return hResult;
}

HRESULT CDIL_CAN::getTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
    return m_pBaseDILCAN_Controller->getTimeModeMapping(CurrSysTime, TimeStamp, QueryTickCount);
}

HRESULT CDIL_CAN::listHardwareInterfaces(InterfaceHardwareList& sSelHwInterface, INT& nCount)
{
    HRESULT hr = m_pBaseDILCAN_Controller->listHardwareInterfaces(sSelHwInterface, nCount);

    if ( hr != S_OK && m_hOldDll )
    {
        /* If it is a not same Dll, Ex: in case of ES581, IntrepidCS */
        if ( m_hOldDll != m_hDll )
        {
            /* Get rid of current DIL library */
            if ( m_pBaseDILCAN_Controller )
            {
                m_pBaseDILCAN_Controller->performClosureOperations();
                m_pBaseDILCAN_Controller->unloadDriverLibrary();
            }
            FreeLibrary(m_hDll);
        }

        /* Retain old DIL selection */
        m_hDll = m_hOldDll;
        m_pBaseDILCAN_Controller = m_pOldBaseDILCAN_Controller;
        driverId = m_dwOldDriverID;
    }
    else if ( m_hOldDll )
    {
        /* If it is a not same Dll, Ex: in case of ES581, IntrepidCS */
        if ( m_hOldDll != m_hDll )
        {
            if ( m_pOldBaseDILCAN_Controller )
            {
                m_pOldBaseDILCAN_Controller->performClosureOperations();
                m_pOldBaseDILCAN_Controller->unloadDriverLibrary();
            }
            /* Get rid of old DIL library */
            FreeLibrary(m_hOldDll);
        }
    }
    m_hOldDll = NULL;

    return hr;
}

HRESULT CDIL_CAN::selectHardwareInterfaces(const InterfaceHardwareList& sSelHwInterface, INT nCount)
{
    return m_pBaseDILCAN_Controller->selectHardwareInterface(sSelHwInterface, nCount);
}

HRESULT CDIL_CAN::deselectHardwareInterfaces(void)
{
    return m_pBaseDILCAN_Controller->deselectHardwareInterface();
}

HRESULT CDIL_CAN::displayConfigurationDialog(PSCONTROLLER_DETAILS InitData, int& Length)
{
    return m_pBaseDILCAN_Controller->displayConfigurationDialog(InitData, Length);
}

HRESULT CDIL_CAN::setConfigurationData(PSCONTROLLER_DETAILS pInitData, int Length)
{
    return m_pBaseDILCAN_Controller->setConfigurationData(pInitData, Length);
}

HRESULT CDIL_CAN::startHardware(void)
{
    return m_pBaseDILCAN_Controller->startHardware();
}

HRESULT CDIL_CAN::stopHardware(void)
{
    return m_pBaseDILCAN_Controller->stopHardware();
}

HRESULT CDIL_CAN::sendMessage(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
    return m_pBaseDILCAN_Controller->sendMessage(dwClientID, sCanTxMsg);
}

HRESULT CDIL_CAN::getControllerParameters(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    if ( m_pBaseDILCAN_Controller )
    {
        return m_pBaseDILCAN_Controller->getControllerParameters(lParam, nChannel, eContrParam);
    }
    else
    {
        return S_FALSE;
    }
}

HRESULT CDIL_CAN::setControllerParameters(int nValue, ECONTR_PARAM eContrparam)
{
    if ( m_pBaseDILCAN_Controller )
    {
        return m_pBaseDILCAN_Controller->setControllerParameters(nValue, eContrparam);
    }
    else
    {
        return S_FALSE;
    }
}

HRESULT  CDIL_CAN::getErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    return m_pBaseDILCAN_Controller->getErrorCount(sErrorCnt, nChannel, eContrParam);
}
