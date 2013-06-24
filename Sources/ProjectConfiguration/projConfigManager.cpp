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
 * @brief     Contains project configuration manager class implementation.
 * @authors   Anish Kumar, Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains project configuration manager class implementation.
 */

/* Project includes */
#include "StdAfx_ProjectConfiguration.h"
#include "ProjectConfiguration_extern.h"
#include "ProjConfig.h"
#include "projConfigManager.h"
#include "Application/StdAfx.h"

CProjConfigManager g_ProjCfgManager;

CProjConfigManager::CProjConfigManager()
{
    projectMap.clear();
}

CProjConfigManager::~CProjConfigManager()
{
    // ENSURE DESTRUCTOR OF CProjConfig IS CALLED
    projectMap.clear();
}

bool CProjConfigManager::getProjectConfiguration(std::string ProjectName, CProjConfig*& ProjConfig)
{
    bool bResult = false;
    ProjectMap::iterator i = projectMap.begin();
    if (i != projectMap.end())
    {
        bResult = true;
        ProjConfig = &(i->second);
    }
    return bResult;
}

int CProjConfigManager::getProjectCount()
{
    return projectMap.size();
}

int CProjConfigManager::getProjectList(std::list<std::string>& ProjectList)
{
    ProjectList.clear();

    for (ProjectMap::iterator i = projectMap.begin(); i != projectMap.end(); ++i)
    {
        ProjectList.push_front(i->first);
    }

    return getProjectCount();
}

bool CProjConfigManager::getProjectData(std::string ProjName, ProjectData& ProjData)
{
    bool bResult = false;

    ProjectMap::iterator i = projectMap.find(ProjName);
    bResult = (i != projectMap.end());

    if (bResult)
    {
        i->second.getProjectData(ProjData);
    }

    return bResult;
}

int CProjConfigManager::getSectionCount(std::string ProjectName)
{
    int nResult = 0;

    CProjConfig* ProjConfig = NULL;
    if (getProjectConfiguration(ProjectName, ProjConfig))
    {
        nResult = ProjConfig->getSectionCount();
    }

    return nResult;
}

int CProjConfigManager::getSectionList(std::string ProjectName, std::list<std::string>& SectionList)
{
    int nResult = 0;

    CProjConfig* ProjConfig = NULL;
    if (getProjectConfiguration(ProjectName, ProjConfig))
    {
        nResult = ProjConfig->getSectionList(SectionList);
    }

    return nResult;
}

bool CProjConfigManager::getSectionData(std::string ProjectName, std::string SectionName,
                                        SectionData& Sectiondata)
{
    bool bResult = false;

    CProjConfig* ProjConfig = NULL;
    if (getProjectConfiguration(ProjectName, ProjConfig))
    {
        bResult = ProjConfig->getSectionData(SectionName, Sectiondata);
    }
    return bResult;
}

void CProjConfigManager::setProjectData(const ProjectData& NewProjData)
{
    CProjConfig* pProjConfig = NULL;
    if (getProjectConfiguration(NewProjData.projectName, pProjConfig))
    {
        pProjConfig->setProjectData(NewProjData);
    }
    else
    {
        CProjConfig NewProjConfig;
        NewProjConfig.setProjectData(NewProjData);
        projectMap.insert(ProjectMap::value_type(NewProjData.projectName,
                          NewProjConfig));
    }
}

bool CProjConfigManager::setSectionData(std::string ProjectName,
                                        const SectionData& SectionData)
{
    bool bResult = false;

    CProjConfig* ProjConfig = NULL;
    if (getProjectConfiguration(ProjectName, ProjConfig))
    {
        bResult = ProjConfig->setSectionData(SectionData);
    }

    return bResult;
}

void CProjConfigManager::deleteProject(std::string ProjectName)
{
    projectMap.erase(ProjectName);
}

void CProjConfigManager::deleteAllProjects(void)
{
    projectMap.clear();
}
