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
 * @file      operations_file.cpp
 * @brief     This file contains the codes when configuration procedure
 * @authors   Anish Kumar, Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains the codes when configuration procedure
 */

/* Project includes */
#include "StdAfx_ProjectConfiguration.h"
#include "ProjectConfiguration_extern.h"
#include "ProjConfig.h"
#include "projConfigManager.h"

#define DATABASE_SIGNATURE    "/************* FrameFileDB **************/"

// Application version is added in the version 1.1
#define VERSIONLINE           "/************* Version 1.1 **************/"
#define APPLICATION_VERSION   "/************* BUSMASTER [1.7.3] ************************/"
#define OLDVERSIONLINE           "/************* Version 1.0 **************/"
#define PROJECT_TABLE_SIG     "  PROJECT_TABLE :: PROJECT_TABLE "
#define DB_END_SIGNATURE      "/*********** FrameFileDB_END ************/"
#define LEN_SIG_FILE_START                      sizeof(DATABASE_SIGNATURE)
#define LEN_SIG_FILE_END                        sizeof(DB_END_SIGNATURE)
#define LEN_SIG_PROJ_TABLE                      sizeof(PROJECT_TABLE_SIG)
#define LEN_VERSION                             sizeof(VERSIONLINE)
#define LEN_OLDER_VERSION                       sizeof(OLDVERSIONLINE)
#define LEN_APPLICATION_VERSION                 sizeof(APPLICATION_VERSION)
#define OPENING_FILE_ERROR                      1
#define FILE_NOT_PRESENT                       -1
#define PROBLEM_IN_WRITING_READING              2
#define INVALID_FORMAT_FILE                     3
#define UNKNOWN_ERROR                           4
#define FILE_INVALID_SECTIONID                  5

enum
{
    SECTION_SIGNATURE                   = 0x0,
    SECTION_VERSION,
    SECTION_PROJECT_TABLE_SIGNATURE,
    SECTION_PROJECT_TABLE_COUNT,
    SECTION_PROJECT_TABLE_ENTRY,
    SECTION_SECTION_TABLE_ENTRY,
    SECTION_SIGNATURE_FILE_END,
    SECTION_FILE_INVALID
};

static bool readString(FILE* pFile, std::string& ResultStr)
{
    bool bResult = false;
    int nLength;
    //fseek(pFile, sizeof("\n")-1, SEEK_CUR);
    if (fread(&nLength, sizeof(int), 1, pFile) == 1)
    {
        CHAR* Buffer = NULL;
        try
        {
            Buffer = new CHAR[nLength];
            if (Buffer != NULL)
            {
                if (fread(Buffer, sizeof(char) * nLength, 1, pFile) != NULL)
                {
                    ResultStr = Buffer;
                    bResult = true;
                }
                delete[] Buffer;
                Buffer = NULL;
            }
        }
        catch (...)
        {
            //throw 0;
            //MessageBox(str, MB_ICONERROR| MB_SYSTEMMODAL|MB_OK, 0 );
        }
    }
    return bResult;
}

int getErrorType(FILE* pFile)
{
    int nReturn = UNKNOWN_ERROR;
    if (feof(pFile))
    {
        nReturn = INVALID_FORMAT_FILE;
    }
    else if (ferror(pFile))
    {
        nReturn = PROBLEM_IN_WRITING_READING;
    }

    return nReturn;
}

static int readAndValidateString(FILE* pFile, char Buffer[], char* ValidationStr)
{
    int nResult = 0;
    int nLength;
    //fseek(pFile, sizeof("\n")-1, SEEK_CUR);
    if (fread(&nLength, sizeof(int), 1, pFile) == 1)
    {
        memset(Buffer, '\0', nLength);
        if (fread(Buffer, sizeof(char) * nLength, 1, pFile) == NULL)
        {
            nResult = getErrorType(pFile);
        }
    }

    if (strcmp(Buffer, ValidationStr) != 0)
    {
        nResult = INVALID_FORMAT_FILE;
    }

    return nResult;
}

static int writeString(FILE* pFile, char SrcString[])
{
    int nResult = PROBLEM_IN_WRITING_READING;

    size_t Length = strlen(SrcString);
    //fputs("\n", pFile);
    if (fwrite(&Length, sizeof(int), 1, pFile) == 1)
    {
        if (fwrite(SrcString, sizeof(char), Length, pFile) == Length)
        {
            nResult = 0;
        }
    }

    return nResult;
}

static int readWriteSection(bool bToRead, short SectionID,
                            FILE* pFile, void* pData = NULL)
{
    int nResult = 0;

    switch (SectionID)
    {
        case SECTION_SIGNATURE:
        {
            if (bToRead)
            {
                char acBuffer[LEN_SIG_FILE_START] = "";
                nResult = readAndValidateString(pFile, acBuffer, DATABASE_SIGNATURE);
                if (nResult != 0)
                {
                    readWriteSection(bToRead, SECTION_FILE_INVALID, NULL);
                }
            }
            else
            {
                nResult = writeString(pFile, DATABASE_SIGNATURE);
            }
            if (nResult == 0)
            {
                nResult = readWriteSection(bToRead, SECTION_VERSION, pFile);
            }
        }
        break;
        case SECTION_VERSION:
        {
            if (bToRead)
            {
                char acBuffer[LEN_VERSION] = "";
                nResult = readAndValidateString(pFile, acBuffer, VERSIONLINE);
                if(nResult != 0)
                {
                    CString strVer = (const char*)acBuffer;
                    strVer.Remove('.');
                    strVer.Remove('*');
                    strVer.Remove('/');
                    strVer.Replace("Version", "");
                    strVer.Remove(' ');

                    // Get the cfx version info
                    INT nVer = 0;

                    nVer = atoi(strVer);

                    switch (nVer)
                    {
                        case 10:
                            // Successfull
                            nResult = 0;
                            break;
                        default:
                            // Invalid
                            nResult = INVALID_FORMAT_FILE;
                            break;
                    }
                }

                // If the newer version[1.1] file is opened
                else
                {
                    char acBuffer[LEN_APPLICATION_VERSION] = "";

                    // Reading Application version
                    readAndValidateString(pFile, acBuffer, APPLICATION_VERSION);
                }

                if (nResult != 0)
                {
                    readWriteSection(bToRead, SECTION_FILE_INVALID, NULL);
                }
            }
            else
            {
                nResult = writeString(pFile, VERSIONLINE);
                // Writing Application version is added in the Version 1.1
                nResult = writeString(pFile, APPLICATION_VERSION);
            }
            if (nResult == 0)
            {
                nResult = readWriteSection(bToRead, SECTION_PROJECT_TABLE_SIGNATURE, pFile);
            }
        }
        break;
        case SECTION_PROJECT_TABLE_SIGNATURE:
        {
            if (bToRead)
            {
                char acBuffer[LEN_SIG_PROJ_TABLE] = "";
                nResult = readAndValidateString(pFile, acBuffer, PROJECT_TABLE_SIG);
                if (nResult != 0)
                {
                    readWriteSection(bToRead, SECTION_FILE_INVALID, NULL);
                }
            }
            else
            {
                nResult = writeString(pFile, PROJECT_TABLE_SIG);
            }
            if (nResult == 0)
            {
                nResult = readWriteSection(bToRead, SECTION_PROJECT_TABLE_COUNT, pFile);
            }
        }
        break;
        case SECTION_PROJECT_TABLE_COUNT:
        {
            UCHAR Entries = 0;
            if (bToRead)
            {
                //fseek(pFile, sizeof("\n")-1, SEEK_CUR);
                if (fread(&Entries, sizeof(Entries), 1, pFile) == 1)
                {
                    nResult = readWriteSection(bToRead, SECTION_PROJECT_TABLE_ENTRY,
                                               pFile, &Entries);
                }
                else
                {
                    nResult = getErrorType(pFile);
                }
            }
            else
            {
                Entries = (UCHAR) g_ProjCfgManager.getProjectCount();
                //fputs("\n", pFile);
                if (fwrite(&Entries, sizeof(UCHAR), 1, pFile) == 1)
                {
                    nResult = readWriteSection(bToRead, SECTION_PROJECT_TABLE_ENTRY,
                                               pFile, &Entries);
                }
                else
                {
                    nResult = PROBLEM_IN_WRITING_READING;
                }
            }
        }
        break;
        case SECTION_PROJECT_TABLE_ENTRY:
        {
            UCHAR Entries = *((UCHAR*) pData);
            if (bToRead)
            {
                for (UCHAR i = 0; (i < Entries) && (nResult == 0); i++)
                {
                    ProjectData sProjectDataTmp;
                    if (sProjectDataTmp.read(pFile))
                    {
                        g_ProjCfgManager.setProjectData(sProjectDataTmp);
                    }
                    else
                    {
                        nResult = getErrorType(pFile);
                    }
                }
            }
            else
            {
                bool bAllWell = true;
                StringList ProjectList;
                g_ProjCfgManager.getProjectList(ProjectList);

                for (StringList::iterator i = ProjectList.begin(); (i != ProjectList.end()) && bAllWell; ++i)
                {
                    ProjectData ProjData;
                    bAllWell = g_ProjCfgManager.getProjectData(*i, ProjData);
                    if (bAllWell)
                    {
                        bAllWell = ProjData.write(pFile);
                    }
                }
                if (!bAllWell)
                {
                    nResult = PROBLEM_IN_WRITING_READING;
                }
            }
            if (nResult == 0)
            {
                nResult = readWriteSection(bToRead, SECTION_SECTION_TABLE_ENTRY,
                                           pFile, &Entries);
            }
        }
        break;
        case SECTION_SECTION_TABLE_ENTRY:
        {
            bool bAllWell = true;
            if (bToRead)
            {
                UCHAR ProjectEntries = *((UCHAR*) pData);
                for (UCHAR i = 0; (i < ProjectEntries) && bAllWell; i++)
                {
                    std::string ProjectName;
                    bAllWell = readString(pFile, ProjectName);
                    //ProjectName[ProjectName.length()-1] = '\0';
                    UCHAR SectionEntries = 0;
                    if (bAllWell)
                    {
                        //fseek(pFile, sizeof("\n")-1, SEEK_CUR);
                        bAllWell = (fread(&SectionEntries, sizeof(SectionEntries),
                                          1, pFile) == 1);
                    }
                    if (bAllWell)
                    {
                        for (UCHAR j = 0; (j < SectionEntries) && bAllWell; j++)
                        {
                            SectionData CurrSection;
                            bAllWell = CurrSection.read(pFile);
                            if (bAllWell)
                            {
                                g_ProjCfgManager.setSectionData(
                                    ProjectName, CurrSection);
                            }
                            else
                            {
                                nResult = getErrorType(pFile);
                            }
                        }
                    }
                }
                if (!bAllWell)
                {
                    nResult = PROBLEM_IN_WRITING_READING;
                }
            }
            else
            {
                StringList ProjectList;
                g_ProjCfgManager.getProjectList(ProjectList);

                for (StringList::iterator i = ProjectList.begin(); (i != ProjectList.end()) && bAllWell; ++i)
                {
                    const char* str = i->c_str();
                    if ((nResult = writeString(pFile, (char*) str)) == 0)
                    {
                        StringList SectionList;
                        UCHAR Sections = (UCHAR) g_ProjCfgManager.getSectionList(*i, SectionList);
                        //fputs("\n", pFile);
                        if (fwrite(&Sections, sizeof(UCHAR), 1, pFile) != 1)
                        {
                            bAllWell = false;
                        }

                        for (StringList::iterator j = SectionList.begin(); (j != SectionList.end()) && bAllWell; ++j)
                        {
                            SectionData CurrSecData;
                            bAllWell = g_ProjCfgManager.getSectionData(*i, *j, CurrSecData);
                            if (bAllWell)
                            {
                                bAllWell = CurrSecData.write(pFile);
                            }
                        }
                    }
                }
                if (!bAllWell)
                {
                    nResult = PROBLEM_IN_WRITING_READING;
                }
            }
            if (bAllWell)
            {
                nResult = readWriteSection(bToRead, SECTION_SIGNATURE_FILE_END, pFile);
            }
        }
        break;
        case SECTION_SIGNATURE_FILE_END:
        {
            if (bToRead)
            {
                char acBuffer[LEN_SIG_FILE_END] = "";
                nResult = readAndValidateString(pFile, acBuffer, DB_END_SIGNATURE);
                if (nResult != 0)
                {
                    readWriteSection(bToRead, SECTION_FILE_INVALID, NULL);
                }
            }
            else
            {
                nResult = writeString(pFile, DB_END_SIGNATURE);
            }
        }
        break;


        default:
        {
            nResult = FILE_INVALID_SECTIONID;
        }
        break;
    }

    return nResult;
}

void closeDataFile()
{
    g_ProjCfgManager.deleteAllProjects();
}

int loadDataFile(std::string fileName)
{
    g_ProjCfgManager.deleteAllProjects();

    int nResult = 0;
    FILE* pFile = NULL;

    //Check if file exists
    if (fopen_s(&pFile, fileName.c_str(), "rb") == 0)
    {
        nResult = readWriteSection(true, SECTION_SIGNATURE, pFile);
        fclose(pFile);
    }
    else
    {
        nResult = OPENING_FILE_ERROR;
    }

    return nResult;
}

int saveDataFile(std::string fileName)
{
    int nResult = 0;
    FILE* pFile = NULL;

    //open again with write permission
    if (fopen_s(&pFile, fileName.c_str(), "wb") == 0)
    {
        nResult = readWriteSection(false, SECTION_SIGNATURE, pFile);
        fclose(pFile);
    }
    else
    {
        nResult = OPENING_FILE_ERROR;
    }

    return nResult;
}
