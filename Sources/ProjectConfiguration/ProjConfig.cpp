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
 * @file      ProjConfig.cpp
 * @brief     This contains project configuration class implementation.
 * @authors   Anish Kumar, Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains project configuration class implementation.
 * Implementation of the ProjConfig class.
 */

/* Project includes */
#include "StdAfx_ProjectConfiguration.h"
#include "ProjectConfiguration_extern.h"
#include "ProjConfig.h"

CProjConfig::CProjConfig()
{
    clearProjectMap();
}

CProjConfig::~CProjConfig()
{
    clearProjectMap();
}

void CProjConfig::clearProjectMap(void)
{
    for (SectionMap::iterator i = sectionMap.begin(); i != sectionMap.end(); ++i)
    {
        delete i->second;
        i->second = NULL;
    }

    sectionMap.clear();
}

int CProjConfig::getSectionCount()
{
    return sectionMap.size();
}

void CProjConfig::getProjectData(ProjectData& ProjDATA)
{
    ProjDATA = projectData;
}

int CProjConfig::getSectionList(std::list<std::string>& SectionList)
{
    SectionList.empty();

    for (SectionMap::iterator i = sectionMap.begin(); i != sectionMap.end(); ++i)
    {
        SectionList.push_front(i->first);
    }

    return getSectionCount();
}

bool CProjConfig::getSectionData(std::string sectionName, SectionData& sectionData)
{
    bool bResult = false;

    SectionMap::const_iterator i;
    i = sectionMap.find(sectionName);
    if (i != sectionMap.end())
    {
        sectionData = *(i->second);
        bResult = true;
    }
    else
    {
        //MessageBox(NULL, "not found", "", MB_OK);
    }
    return bResult;
}

bool CProjConfig::setSectionData(const SectionData& sectionData)
{
    bool bAdded = false;

    SectionMap::iterator i;
    i = sectionMap.find(sectionData.sectionName);
    if (i != sectionMap.end())
    {
        *(i->second) = sectionData;
    }
    else
    {
        SectionData* pNewSection = new SectionData;
        *pNewSection = sectionData;
        sectionMap.insert(SectionMap::value_type(sectionData.sectionName,
                          pNewSection));
        bAdded = true;
    }

    return bAdded;
}

void CProjConfig::setProjectData(const ProjectData& projDATA)
{
    projectData = projDATA;
}
