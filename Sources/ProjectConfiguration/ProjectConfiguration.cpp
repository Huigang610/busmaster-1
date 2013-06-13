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
 * @file      ProjectConfiguration.cpp
 * @brief     This contains the dll entry point function and defines the entry point for the DLL application.
 * @authors   Anish Kumar, Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains the dll entry point function and defines the entry point for the DLL application.
 */

/* Project includes */
#include "StdAfx_ProjectConfiguration.h"
#include "ProjectConfiguration.h"
#include "include/utils_macro.h"
#define USAGE_EXPORT
#include "ProjectConfiguration_extern.h"
#include "operations_file.h"
#include "projConfigManager.h"

// {410BD867-1AD9-4492-9DA4-8175CCB7147B}
static const GUID guidProjectConfig =
{
    0x410bd867, 0x1ad9, 0x4492, { 0x9d, 0xa4, 0x81, 0x75, 0xcc, 0xb7, 0x14, 0x7b }
};

static DataStorageInformation sdataStorageInfo;

/**
 * The one and only CProjectConfigurationApp object
 */
CProjectConfigurationApp theApp;

BOOL CProjectConfigurationApp::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}

USAGEMODE int setProjectData(ProjectData& ProjData)
{
    int nReturn = 0;
    if(sdataStorageInfo.mode == FILEMODE)
    {
        g_ProjCfgManager.setProjectData(ProjData);
    }
    return nReturn;
}

USAGEMODE int deleteProject(std::string& ProjectName)
{
    int nReturn = 0;
    if(sdataStorageInfo.mode == FILEMODE)
    {
        g_ProjCfgManager.deleteProject(ProjectName);
    }
    return nReturn;
}

USAGEMODE int getProjectCount(void)
{
    int nReturn = 0;
    if(sdataStorageInfo.mode == FILEMODE)
    {
        nReturn = g_ProjCfgManager.getProjectCount();
    }
    return nReturn;
}

USAGEMODE int getProjectList(StringList* ProjList)
{
    int nReturn = 0;
    if(sdataStorageInfo.mode == FILEMODE)
    {
        nReturn = g_ProjCfgManager.getProjectList(*ProjList);
    }
    return nReturn;
}

USAGEMODE int getProjectData(std::string& ProjName, ProjectData& ProjData)
{
    int nReturn = 0;
    if(sdataStorageInfo.mode == FILEMODE)
    {
        nReturn = g_ProjCfgManager.getProjectData(ProjName, ProjData);
    }
    return nReturn;
}

USAGEMODE int setSectionData(std::string& ProjectName, SectionData& SectionData)
{
    int nReturn = 0;
    if(sdataStorageInfo.mode == FILEMODE)
    {
        nReturn = g_ProjCfgManager.setSectionData(ProjectName, SectionData);
    }
    return nReturn;
}

USAGEMODE BOOL getSectionData(std::string& ProjectName, std::string& SectionName, SectionData& SectionData)
{
    BOOL bReturn = 0;
    if(sdataStorageInfo.mode == FILEMODE)
    {
        bReturn = g_ProjCfgManager.getSectionData(ProjectName, SectionName, SectionData);
    }
    return bReturn;
}

USAGEMODE int getSectionCount(std::string& ProjectName)
{
    int nReturn = 0;
    if(sdataStorageInfo.mode == FILEMODE)
    {
        nReturn = g_ProjCfgManager.getSectionCount(ProjectName);
    }
    return nReturn;
}

USAGEMODE int getSectionList(std::string& ProjectName, StringList* SectionList)
{
    int nReturn = 0;
    if(sdataStorageInfo.mode == FILEMODE)
    {
        nReturn = g_ProjCfgManager.getSectionList(ProjectName, *SectionList);
    }
    return nReturn;
}

USAGEMODE int setDataStorageConfiguration(DataStorageInformation* StorageInfo)
{
    sdataStorageInfo.mode = StorageInfo->mode ;
    if (sdataStorageInfo.mode == FILEMODE)
    {
        if (sdataStorageInfo.FSInfo == NULL)
        {
            sdataStorageInfo.FSInfo = new FileStorageInformation; //Change
        }
        sdataStorageInfo.FSInfo->filePath = StorageInfo->FSInfo->filePath;
    }
    return 0;
}

USAGEMODE int getDataStorageConfiguration(DataStorageInformation* StorageInfo)
{
    StorageInfo->mode = sdataStorageInfo.mode;
    if (sdataStorageInfo.mode == FILEMODE)
    {
        if (sdataStorageInfo.FSInfo != NULL)
        {
            StorageInfo->FSInfo = new FileStorageInformation;
            StorageInfo->FSInfo->filePath = sdataStorageInfo.FSInfo->filePath;
        }
        else
        {
            StorageInfo->FSInfo = NULL;
        }
    }
    return 0;
}

USAGEMODE int doDataStorageOperation(DataStorageOperationMode operation)
{
    int nReturn = 0;
    if (sdataStorageInfo.mode == FILEMODE)
    {
        if (operation & SAVE)
        {
            nReturn = saveDataFile(sdataStorageInfo.FSInfo->filePath);
        }
        else if (operation & OPEN)
        {
            nReturn = loadDataFile(sdataStorageInfo.FSInfo->filePath);
        }
        else if (operation & CLOSE)
        {
            closeDataFile();
            if (sdataStorageInfo.FSInfo != NULL)
            {
                delete sdataStorageInfo.FSInfo;
                sdataStorageInfo.FSInfo = NULL;
            }
        }
    }
    return nReturn;
}
