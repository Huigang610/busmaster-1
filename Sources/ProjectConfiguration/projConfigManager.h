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
 * @file      projConfigManager.h
 * @brief     Contains project configuration manager class definition.
 * @authors   Anish Kumar, Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains project configuration manager class definition.
 */

#pragma once

/* C++ includes */
#include <list>
#include <map>
#include <string>

/* Project includes */
#include "ProjConfig.h"

typedef std::map<std::string, CProjConfig> ProjectMap;

class CProjConfigManager
{
public:
    // Constructor and destructor
    CProjConfigManager();
    virtual ~CProjConfigManager();

    /* Project functions */
    int getProjectCount();
    int getProjectList(std::list<std::string>& projectList);
    bool getProjectData(std::string projectName, ProjectData& projectData);
    void setProjectData(const ProjectData& projectData);
    void deleteProject(std::string projectName);
    void deleteAllProjects(void);

    /* Section functions */
    int getSectionCount(std::string projectName);
    int getSectionList(std::string projectName, std::list<std::string>& sectionList);
    bool getSectionData(std::string projectName, std::string sectionName, SectionData& sectionData);
    bool setSectionData(std::string projectName, const SectionData& sectionData);

private:
    /**
     * Project map
     */
    ProjectMap projectMap;

    /**
     * Get project configuration.
     */
    bool getProjectConfiguration(std::string projectName, CProjConfig*& projectConfiguration);
};

/**
 * Project manager object
 */
extern CProjConfigManager g_ProjCfgManager;
