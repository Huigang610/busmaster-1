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
 * @file      ProjConfig_DataTypes.h
 * @brief     Definition file for project configuration data types.
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for project configuration data types.
 */

#pragma once

/* C++ includes */
#include <list>
#include <string>

typedef enum DataStorageOperationMode {
    OPEN = 0x1,
    SAVE = 0x2,
    CLOSE = 0x4
} DataStorageOperationMode;

typedef enum DataStorageMode {
    FILEMODE = 0,
    DATABASEMODE
} DataStorageMode;

typedef std::list<std::string> StringList;

class FileStorageInformation
{
public:
    std::string filePath;
    FileStorageInformation();
};

typedef struct DataStorageInformation
{
    DataStorageMode mode;
    FileStorageInformation* FSInfo;
} DataStorageInformation;

/**
 * This class is defined to represent a project entry completely
 * without the data (or section). This is used as a structure and hence all the
 * members are declared public.
 *
 * The necessary and sufficient information for a
 * project entry is name (signature and hence it
 * has to be unique), the language (expressed in
 * ISO 639-3 code), time and date of last saving,
 * application version and unique identifier.
 */
class ProjectData
{
public:
    std::string projectName;
    std::string language;
    SYSTEMTIME projectTime;
    float applicationVersion;
    DWORD applicationUniqueId;

    ProjectData();

    /**
     * @brief Does initialisation.
     *
     * This clears the project entry wiping out all the data.
     */
    void initialize(void);

    /**
     * Equal operator overloaded
     *
     * @param[in] refObj The soure object
     * @return The current object, the modified one.
     */
    ProjectData& operator=(const ProjectData& refObj);

    /**
     * Writes the current object data into the target file.
     * Advances the file pointer afterwards.
     *
     * @param[in] file The target file pointer.
     * @return true if all the operations are successful, else false.
     */
    bool write(FILE* file);

    /**
     * Assuming the current file pointer points to a project
     * data entry, this function retrieves project data entry
     * and initialises the current object with the information.
     * Advances the file pointer afterwards.
     *
     * @param[in] file The source file pointer.
     * @return true if all the operations are successful, else false.
     */
    bool read(FILE* file);
};

/**
 * This class is defined to represent a section completely.
 *
 * The necessary and sufficient information for a
 * section are the section name and the data in
 * byte stream that it should contain.
 */
class SectionData
{
public:
    std::string sectionName;
    int blobLen;
    BYTE* blob;

    SectionData();
    ~SectionData();

    /**
     * @brief Does initialisation
     *
     * This clears the section wiping out all the data.
     */
    void initialize(void);

    /**
     * Equal operator overloaded
     *
     * @param[in] refObj The soure object
     * @return The current object, the modified one.
     */
    SectionData& operator=(const SectionData& refObj);

    /**
     * Writes the current object data into the target file.
     * Advances the file pointer afterwards.
     *
     * @param[in] file The target file pointer.
     * @return true if all the operations are successful, else false.
     */
    bool write(FILE* file);

    /**
     * Assuming the current file pointer points to a section
     * entry, this function retrieves section entry and
     * initialises the current object with the information.
     * Advances the file pointer afterwards.
     *
     * @param[in] file The source file pointer.
     * @return true if all the operations are successful, else false.
     */
    bool read(FILE* file);
};
