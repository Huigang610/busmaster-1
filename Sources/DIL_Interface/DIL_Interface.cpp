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
 * @brief     Source file for the exported function used to retrieve the
 * @author    Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for the exported function used to retrieve the
 */

/* Project includes */
#include "DIL_Interface_stdafx.h"
#include "Include/BaseDefs.h"
#define USAGE_EXPORT
#include "DIL_Interface_extern.h"
#include "DIL_CAN.h"
#include "DILI_J1939.h"
#include "DIL_Interface.h"

static CDIL_CAN* sg_pouDIL_CAN = NULL;
static CDILI_J1939* sg_pouDILI_J1939 = NULL;

/**
 * The one and only CDILApp object
 */
CDILApp theApp;

int CDILApp::ExitInstance()
{
    INT nResult = -1;
    if (NULL != sg_pouDIL_CAN)
    {
        delete sg_pouDIL_CAN;
        sg_pouDIL_CAN = NULL;
    }
    return nResult;
}

USAGEMODE HRESULT DIL_GetInterface(BusType eBusType, void** ppvInterface)
{
    HRESULT hResult = S_OK;

    switch (eBusType)
    {
        case CAN:
        {
            if (NULL == sg_pouDIL_CAN)
            {
                if ((sg_pouDIL_CAN = new CDIL_CAN) == NULL)
                {
                    ASSERT(FALSE);
                    hResult = S_FALSE;
                }
                else
                {
                    sg_pouDIL_CAN->InitInstance();
                }
            }
            // Else the object has been existing already
            *ppvInterface = (void*) sg_pouDIL_CAN; /* Doesn't matter even if sg_pouFP_CAN is null */
        }
        break;
        case MCNET:
            break;
        case J1939:
        {
            if (NULL == sg_pouDILI_J1939)
            {
                if ((sg_pouDILI_J1939 = new CDILI_J1939) == NULL)
                {
                    ASSERT(FALSE);
                    hResult = S_FALSE;
                }
            }
            // Else the object has been existing already
            *ppvInterface = (void*) sg_pouDILI_J1939; /* Doesn't matter even if sg_pouFP_CAN is null */
        }
        break;
        default:
        {
            hResult = S_FALSE;
        }
        break;
    }

    return hResult;
}
