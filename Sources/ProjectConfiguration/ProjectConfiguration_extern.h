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
 * @brief     This contains exported function declarations, that can be
 * @authors   Anish Kumar, Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains exported function declarations, that can be
 */

#pragma once

#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

#include "DataTypes/ProjConfig_DataTypes.h"

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

    /**
     * @{
     * Data storage functions
     */

    /**
     * @brief Set the present data storage configuration
     * @req RS_06_01 Set the present data storage configuration
     *
     * Set the present data storage configuration.
     * This is a data storage related function.
     */
    USAGEMODE int setDataStorageConfiguration(DataStorageInformation* storageInformation);

    /**
     * @brief Get the presently selected data storage configuration
     * @req RS_06_02 Get the presently selected data storage configuration
     *
     * Get the presently selected data storage configuration.
     * This is a data storage related function.
     */
    USAGEMODE int getDataStorageConfiguration(DataStorageInformation* storageInformation);

    /**
     * @brief Perform data storage operation after selection
     * @param[in] operation is OPEN=0x1, SAVE=0x2 or CLOSE=0x4
     * @req RS_06_03 Perform data storage operation after selection
     * @return
     *   -  0: NoError
     *   -  1: Problem in opening file
     *   - -1: File not present
     *   -  2: Problem in writing/reading file
     *   -  3: Invalid file
     *   -  4: Unknown error
     *
     * Perform data storage operation after selection.
     * This is a data storage related function.
     */
    USAGEMODE int doDataStorageOperation(DataStorageOperationMode operation);

    /** @} */

    /**
     * @{
     * Project functions
     */

    /**
     * @brief Adds a project entry in the project table or modifies an existing one
     * @req RS_06_06 Adds a project entry in the project table or modifies an existing one
     *
     * Adds a project entry in the project table or modifies an existing one.
     * This is a project table related function.
     */
    USAGEMODE int setProjectData(ProjectData& projectData);

    /**
     * @brief Deletes the project entry from the project table
     * @req RS_06_07 Deletes the project entry from the project table
     *
     * Deletes the project entry from the project table.
     * This is a project table related function.
     */
    USAGEMODE int deleteProject(std::string& projectName);

    /**
     * @brief Get total number of projects in the project table
     * @req RS_06_04 Get total number of projects in the project table
     *
     * Get total number of projects in the project table.
     * This is a project table related function.
     */
    USAGEMODE int getProjectCount(void);

    /**
     * @brief Get project name list from the project table
     * @req RS_06_05 Get project name list from the project table
     *
     * Get project name list from the project table.
     * This is a project table related function.
     */
    USAGEMODE int getProjectList(StringList* projectList);

    /**
     * @brief Retrieves project data from the project table
     * @req RS_06_08 Retrieves project data from the project table
     *
     * Retrieves project data from the project table.
     * This is a project table related function.
     */
    USAGEMODE int getProjectData(std::string& projectName, ProjectData& projectData);

    /** @} */

    /**
     * @{
     * Section functions
     */

    /**
     * @brief Gets information of that particular section from the section table of the project
     * @req RS_06_11 Gets information of that particular section from the section table of the project
     *
     * Gets information of that particular section from the section table of the project.
     * This is a section data related function.
     */
    USAGEMODE int setSectionData(std::string& projectName, SectionData& sectionData);

    /**
     * @brief Delete section data
     *
     * Deletes the section data.
     */
    USAGEMODE int deleteSectionData(std::string& projectName, SectionData& sectionData);

    /**
     * @brief Add a section or modify an existing one in the section table of the project
     * @req RS_06_09 Add a section or modify an existing one in the section table of the project
     *
     * Add a section or modify an existing one in the section table of the project.
     * This is a section data related function.
     */
    USAGEMODE BOOL getSectionData(std::string& projectName, std::string& sectionName, SectionData& sectionData);

    /**
     * @brief Receives total number of sections from the section table of the project
     * @req RS_06_12 Receives total number of sections from the section table of the project
     *
     * Receives total number of sections from the section table of the project.
     * This is a section data related function.
     */
    USAGEMODE int getSectionCount(std::string& projectName);

    /**
     * @brief Retrieves list of all the section names from the section table of the project
     * @req RS_06_13 Retrieves list of all the section names from the section table of the project
     *
     * Retrieves list of all the section names from the section table of the project.
     * This is a section data related function.
     */
    USAGEMODE int getSectionList(std::string& projectName, StringList* sectionList);

    /** @} */

#ifdef __cplusplus
}
#endif
