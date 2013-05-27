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
 * @file      ProjConfig_DataTypes.cpp
 * @brief     Source file for project configuration data types.
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for project configuration data types.
 */

/* Project includes */
#include "DataTypes_StdAfx.h"
#include "ProjConfig_DataTypes.h"

FileStorageInformation::FileStorageInformation() :
    filePath("")
{
}

void ProjectData::initialize(void)
{
    SYSTEMTIME currSysTime;
    GetSystemTime(&currSysTime);

    projectName   = "";
    language      = "eng";
    memcpy(&projectTime, &currSysTime, sizeof(SYSTEMTIME));
    applicationVersion = 0;
    applicationUniqueId = 0;
};

ProjectData::ProjectData()
{
    initialize();
}

ProjectData& ProjectData::operator=(const ProjectData& refObj)
{
    projectName = refObj.projectName;
    language = refObj.language;
    applicationVersion = refObj.applicationVersion;
    applicationUniqueId = refObj.applicationUniqueId;
    memcpy(&projectTime, &refObj.projectTime, sizeof(SYSTEMTIME));

    return *this;
}

bool ProjectData::write(FILE* file)
{
    bool result = false;

    /* Below is shown the order in which project data are saved:
    1. Length of the project name [int]
    2. Project name [character stream]
    3. Length of language name [int]
    4. Language name [character stream]
    5. System time and date [SYSTEMTIME, byte stream]
    6. Application version number [float]
    7. Application identifier [DWORD] */

    const char* buffer = projectName.c_str();
    int n = (int) strlen(buffer);
    if (fwrite(&n, sizeof(int), 1, file) == 1)
    {
        if ((int) fwrite(buffer, sizeof(char), n, file) == n)
        {
            buffer = language.c_str();
            n =  (int) strlen(buffer);
            if (fwrite(&n, sizeof(int), 1, file) == 1)
            {
                if ((int) fwrite(buffer, sizeof(char), n, file) == n)
                {
                    if (fwrite(&projectTime, sizeof(SYSTEMTIME), 1, file) == 1)
                    {
                        if (fwrite(&applicationVersion, sizeof(float), 1, file) == 1)
                        {
                            if (fwrite(&applicationUniqueId, sizeof(DWORD), 1, file) == 1)
                            {
                                result = true;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

bool ProjectData::read(FILE* file)
{
    initialize();

    bool result = false;
    char buffer[_MAX_PATH] = "";
    int n;

    // For obvious reason reading takes place in the same order as writing.
    if (fread(&n, sizeof(int), 1, file) == 1)
    {
        if (fread(buffer, sizeof(char) * n, 1, file) != NULL)
        {
            projectName = buffer;
            if (fread(&n, sizeof(int), 1, file) == 1)
            {
                if (fread(buffer, sizeof(char) * n, 1, file) != NULL)
                {
                    language = buffer;
                    if (fread(&projectTime, sizeof(SYSTEMTIME), 1, file) == 1)
                    {
                        if (fread(&applicationVersion, sizeof(float), 1, file) == 1)
                        {
                            if (fread(&applicationUniqueId, sizeof(DWORD), 1, file) == 1)
                            {
                                result = true;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

SectionData::SectionData()
{
    blob = NULL;
    blobLen = 0;
    sectionName = "";
}

SectionData::~SectionData()
{
    initialize();
}

void SectionData::initialize(void)
{
    if (NULL != blob)
    {
        delete[] blob;
        blob = NULL;
    }
    blobLen = 0;
    sectionName = "";
}

SectionData& SectionData::operator=(const SectionData& refObj)
{
    initialize();

    blob = new BYTE[refObj.blobLen];
    if (blob != NULL)
    {
        blobLen = refObj.blobLen;
        memcpy(blob, refObj.blob, refObj.blobLen);
        sectionName = refObj.sectionName;
    }
    return *this;
}

bool SectionData::write(FILE* file)
{
    bool result = false;

    /* Below is shown the order in which section data are saved:
    1. Length of the section name [int]
    2. Section name [character stream]
    3. Length of BLOB [int]
    4. BLOB (section data) [byte stream] */

    const char* buffer = sectionName.c_str();
    int n = (int) strlen(buffer);
    if (fwrite(&n, sizeof(int), 1, file) == 1)
    {
        if ((int) fwrite(buffer, sizeof(char), n, file) == n)
        {
            if (fwrite(&blobLen, sizeof(blobLen), 1, file) == 1)
            {
                if (blob != NULL)
                {
                    if ((int) fwrite(blob, 1, blobLen, file) == blobLen)
                    {
                        result = true;
                    }
                }
            }
        }
    }
    return result;
}

bool SectionData::read(FILE* file)
{
    initialize();

    bool result = false;
    char buffer[_MAX_PATH] = "";
    int n;

    if (fread(&n, sizeof(int), 1, file) == 1)
    {
        if (fread(buffer, sizeof(char) * n, 1, file) != NULL)
        {
            sectionName = buffer;
            if (fread(&blobLen, sizeof(blobLen), 1, file) == 1)
            {
                blob = new BYTE[blobLen];
                if (blob != NULL)
                {
                    if ((int) fread(blob, 1, blobLen, file) == blobLen)
                    {
                        result = true;
                    }
                }
            }
        }
    }
    return result;
}
