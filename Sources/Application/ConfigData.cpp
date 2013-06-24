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
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "StdAfx.h"
#include "ProjectConfiguration/ProjectConfiguration_extern.h"
#include "ConfigData.h"
#include "Utility/MultiLanguageSupport.h"

#define DEFAULT_PROJECT_NAME                         "BUSMASTER"
#define BUSMASTER_UNIQUE_ID                              133146

CConfigData CConfigData::m_ouConfigDetails;
CConfigData::CConfigData(void)
{
    m_strCurrProjName = DEFAULT_PROJECT_NAME;
    m_omStrCurrFileName = "";
}

CConfigData::~CConfigData(void)
{
}

CConfigData& CConfigData::ouGetConfigDetailsObject()
{
    return m_ouConfigDetails;
}
void CConfigData::vReadConfigFile()
{
    UCHAR pCheckSum,fChecksum;
    if (bGetCheckSum(m_omStrCurrFileName ,& pCheckSum, &fChecksum))
    {
        if (pCheckSum == fChecksum)
        {
            if (doDataStorageOperation(OPEN) == 0)
            {
                ProjectData sTempData;
                getProjectData(m_strCurrProjName, sTempData);
                if (sTempData.applicationUniqueId != BUSMASTER_UNIQUE_ID)
                {
                    AfxMessageBox (_("The configuration file is not compatible"),
                                   MB_OK | MB_ICONERROR);
                    doDataStorageOperation(CLOSE);
                }
            }
            else
            {
                AfxMessageBox (_("The configuration file is not valid"),
                               MB_OK | MB_ICONERROR);
                doDataStorageOperation(CLOSE);
            }
        }
        else
        {
            AfxMessageBox (_("Configuration file changed outside the application"),
                           MB_OK | MB_ICONERROR);
        }
    }
}

void CConfigData::vSaveConfigFile()
{
    UCHAR pCheckSum;
    doDataStorageOperation(SAVE);
    bSetCheckSum(m_omStrCurrFileName , &pCheckSum);
}

void CConfigData::vCloseConfigFile()
{
    doDataStorageOperation(CLOSE);
    m_omStrCurrFileName.Empty();
}

int CConfigData::SetConfigDatastorage(DataStorageInformation* StorageInfo)
{
    m_omStrCurrFileName = StorageInfo->FSInfo->filePath.c_str();
    return setDataStorageConfiguration(StorageInfo);
}

int CConfigData::GetConfigDatastorage(DataStorageInformation* StorageInfo)
{
    return getDataStorageConfiguration(StorageInfo);
}

void CConfigData::vSetCurrProjName(std::string strCurrProjName)
{
    m_strCurrProjName = strCurrProjName;
}

std::string CConfigData::GetCurrProjName()
{
    return m_strCurrProjName;
}

BOOL CConfigData::bSetCurrProjInfo(ProjectData& ProjData)
{
    ProjData.projectName = m_strCurrProjName;
    return setProjectData(ProjData);
}

BOOL CConfigData::bGetCurrProjInfo(ProjectData& ProjData)
{
    return getProjectData(m_strCurrProjName, ProjData);
}

BOOL CConfigData::bSetData(LPVOID lpVoid, int nStreamLength, std::string strSectionName)
{
    BOOL bRetVal = TRUE;

    // check input param
    if(lpVoid != NULL)
    {
        //if(m_bConfigInfoLoaded == TRUE)
        //{

        SectionData tempSecData;
        tempSecData.sectionName = strSectionName;
        tempSecData.blobLen = nStreamLength;
        tempSecData.blob = new BYTE[nStreamLength];
        memcpy(tempSecData.blob, lpVoid, nStreamLength);
        setSectionData(m_strCurrProjName, tempSecData);
        //delete []tempSecData.m_bBLOB;

        //}
    }
    return bRetVal;
}

BOOL CConfigData::bGetData(void*& lpData, int& nStreamLength, std::string strSectionName)
{
    BOOL bRetVal = FALSE;

    // is the configuration loaded?
    // if((m_bConfigInfoLoaded == TRUE) && (bRetVal == TRUE))
    {
        SectionData tempSecData;
        tempSecData.sectionName = strSectionName;
        bRetVal = getSectionData(m_strCurrProjName, tempSecData.sectionName, tempSecData);
        if (bRetVal != FALSE)
        {
            bRetVal = FALSE;
            BYTE* pbNewDat = new BYTE[tempSecData.blobLen];
            if (pbNewDat != NULL)
            {
                memcpy(pbNewDat, tempSecData.blob, tempSecData.blobLen);
                nStreamLength = tempSecData.blobLen;
                lpData = (void*) pbNewDat;
                bRetVal = TRUE;
            }
            else
            {
            }
        }

    }
    return bRetVal;
}
