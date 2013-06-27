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

class DriverEntry
{
public:
    DWORD id;
    std::string name;
};

static DriverEntry sg_ListDIL[] =
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
    dllHandle = NULL;
    oldDllHandle = NULL;
    driverId = DAL_NONE;
    pfGetIDILCAN_Controller = NULL;
    driver = NULL;
    oldDriver = NULL;
    selectDummyInterface();
}

CDIL_CAN::~CDIL_CAN()
{
    /* Free the currently selected DIL library */
    if (dllHandle)
    {
        FreeLibrary(dllHandle);
        dllHandle = NULL;
    }

    /* Free the previosuly selected DIL library */
    if (oldDllHandle)
    {
        FreeLibrary(oldDllHandle);
        oldDllHandle = NULL;
    }
}

BOOL CDIL_CAN::InitInstance()
{
    selectDummyInterface();

    return TRUE;
}

void CDIL_CAN::selectDummyInterface(void)
{
    driver = (CBaseDIL_CAN_Controller*) sg_pouDIL_CAN_DUMMY;
}

DWORD CDIL_CAN::getDriverList(DriverList* List)
{
    DWORD Result = 0;

    if (List != NULL)
    {
        for (int i = 0; i < sizeof(sg_ListDIL)/sizeof(DriverEntry); i++)
        {
            (*List)[i].name = sg_ListDIL[i].name;
            (*List)[i].driverId = sg_ListDIL[i].id;
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
        /* Save the currently selected DIL library handle */
        if ( dllHandle )
        {
            oldDllHandle = dllHandle;
            oldDriver = driver;
            oldDriverId = driverId;
            dllHandle = NULL;
        }

        switch(dwDriverID)
        {
            case DRIVER_CAN_PEAK_USB:
                dllHandle = LoadLibrary("CAN_PEAK_USB.dll");
                break;

            case DRIVER_CAN_ICS_NEOVI:
            case DRIVER_CAN_ETAS_ES581:
                dllHandle = LoadLibrary("CAN_ICS_neoVI.dll");
                break;

            case DRIVER_CAN_ETAS_BOA:
                dllHandle = LoadLibrary("CAN_ETAS_BOA.dll");
                break;

            case DRIVER_CAN_VECTOR_XL:
                dllHandle = LoadLibrary("CAN_Vector_XL.dll");
                break;

            case DRIVER_CAN_IXXAT:
                dllHandle = LoadLibrary("CAN_IXXAT_VCI.dll");
                break;

            case DRIVER_CAN_KVASER_CAN:
                dllHandle = LoadLibrary("CAN_Kvaser_CAN.dll");
                break;

            case DRIVER_CAN_STUB:
                dllHandle = LoadLibrary("CAN_STUB.dll");
                break;

            case DRIVER_CAN_MHS:
                dllHandle = LoadLibrary("CAN_MHS.dll");
                break;

            case DRIVER_CAN_VSCOM:
                dllHandle = LoadLibrary("CAN_VSCOM.dll");
                break;

            case DAL_NONE:
                performClosureOperations();
                selectDummyInterface();
                driverId = DAL_NONE;
                return hResult;

            default:
                hResult = DAL_INVALID;
                driverId = DAL_NONE;
                return hResult;
        }

        if (dllHandle == NULL)
        {
            hResult = ERR_LOAD_DRIVER;
            pILog->logMessage(A2T(__FILE__), __LINE__, _("Load library failed..."));
        }
        else
        {
            // First select the dummy interface
            //selectDriver((DWORD)DAL_NONE, hWndOwner, pILog);

            pILog->logMessage(A2T(__FILE__), __LINE__, _("Load library successful..."));
            pfGetIDILCAN_Controller = (GETIDIL_CAN_CONTROLLER)GetProcAddress(dllHandle, "GetIDIL_CAN_Controller");
            pfGetIDILCAN_Controller((void**)&driver);
            if ( driver )
            {
                // set the application parameters.
                driver->setApplicationParameters(hWndOwner, pILog);
                hResult = driver->loadDriverLibrary();
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
                        if ( driver )
                        {
                            driver->performClosureOperations();
                            driver->unloadDriverLibrary();
                        }
                        FreeLibrary(dllHandle);
                        if ( oldDllHandle )
                        {
                            dllHandle = oldDllHandle;
                            driver = oldDriver;
                            driverId =  oldDriverId;
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
	if (!driver)
			return S_FALSE;

    return driver->manageMessageBuffer(bAction, ClientID, pBufObj);
}

HRESULT CDIL_CAN::registerClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
	if (!driver)
			return S_FALSE;

    return driver->registerClient(bRegister, ClientID, pacClientName);
}

DWORD CDIL_CAN::getSelectedDriver(void)
{
    return driverId;
}

HRESULT CDIL_CAN::performInitOperations(void)
{
	if (!driver)
			return S_FALSE;

    return driver->performInitOperations();
}

HRESULT CDIL_CAN::performClosureOperations(void)
{
	if (!driver)
			return S_FALSE;

	driver->performClosureOperations();
	driver->unloadDriverLibrary();
    return S_OK;
}

HRESULT CDIL_CAN::getTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
	if (!driver)
			return S_FALSE;

    return driver->getTimeModeMapping(CurrSysTime, TimeStamp, QueryTickCount);
}

HRESULT CDIL_CAN::listHardwareInterfaces(InterfaceHardwareList& sSelHwInterface, INT& nCount)
{
	if (!driver)
			return S_FALSE;

    HRESULT hr = driver->listHardwareInterfaces(sSelHwInterface, nCount);

    if ( hr != S_OK && oldDllHandle )
    {
        /* If it is a not same Dll, Ex: in case of ES581, IntrepidCS */
        if ( oldDllHandle != dllHandle )
        {
            /* Get rid of current DIL library */
            if ( driver )
            {
                driver->performClosureOperations();
                driver->unloadDriverLibrary();
            }
            FreeLibrary(dllHandle);
        }

        /* Retain old DIL selection */
        dllHandle = oldDllHandle;
        driver = oldDriver;
        driverId = oldDriverId;
    }
    else if ( oldDllHandle )
    {
        /* If it is a not same Dll, Ex: in case of ES581, IntrepidCS */
        if ( oldDllHandle != dllHandle )
        {
            if ( oldDriver )
            {
                oldDriver->performClosureOperations();
                oldDriver->unloadDriverLibrary();
            }
            /* Get rid of old DIL library */
            FreeLibrary(oldDllHandle);
        }
    }
    oldDllHandle = NULL;

    return hr;
}

HRESULT CDIL_CAN::selectHardwareInterfaces(const InterfaceHardwareList& sSelHwInterface, INT nCount)
{
	if (!driver)
			return S_FALSE;

	return driver->selectHardwareInterface(sSelHwInterface, nCount);
}

HRESULT CDIL_CAN::deselectHardwareInterfaces(void)
{
	if (!driver)
			return S_FALSE;

    return driver->deselectHardwareInterface();
}

HRESULT CDIL_CAN::displayConfigurationDialog(PSCONTROLLER_DETAILS InitData, int& Length)
{
	if (!driver)
			return S_FALSE;

    return driver->displayConfigurationDialog(InitData, Length);
}

HRESULT CDIL_CAN::setConfigurationData(PSCONTROLLER_DETAILS pInitData, int Length)
{
	if (!driver)
			return S_FALSE;

    return driver->setConfigurationData(pInitData, Length);
}

HRESULT CDIL_CAN::startHardware(void)
{
	if (!driver)
			return S_FALSE;

    return driver->startHardware();
}

HRESULT CDIL_CAN::stopHardware(void)
{
	if (!driver)
			return S_FALSE;

    return driver->stopHardware();
}

HRESULT CDIL_CAN::sendMessage(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
	if (!driver)
			return S_FALSE;

    return driver->sendMessage(dwClientID, sCanTxMsg);
}

HRESULT CDIL_CAN::getControllerParameters(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
	if (!driver)
			return S_FALSE;

    return driver->getControllerParameters(lParam, nChannel, eContrParam);
}

HRESULT CDIL_CAN::setControllerParameters(int nValue, ECONTR_PARAM eContrparam)
{
	if (!driver)
			return S_FALSE;

    return driver->setControllerParameters(nValue, eContrparam);
}

HRESULT  CDIL_CAN::getErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
	if (!driver)
			return S_FALSE;

    return driver->getErrorCount(sErrorCnt, nChannel, eContrParam);
}
