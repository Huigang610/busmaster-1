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
 * @brief     Definition of CParamLoggerBUS class.
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CParamLoggerBUS class.
 */

#pragma once

class CParamLoggerBUS
{
public:
    /**
     * Version info of the application suite
     */
    char version[MAX_PATH];

    /**
     * Error logger module
     */
    Base_WrapperErrorLogger* m_pILog;

    /**
     * Client identifier
     */
    DWORD dwClientID;

    CParamLoggerBUS()
    {
        m_pILog = NULL;
        dwClientID = 0;
        for (int i = 0; i < MAX_PATH; i++)
        {
            version[i] = L'\0';
        }
    };

    CParamLoggerBUS& operator=(const CParamLoggerBUS& RefObj)
    {
        m_pILog     = RefObj.m_pILog;
        dwClientID  = RefObj.dwClientID;
        strcpy_s(version, MAX_PATH, RefObj.version);
        return *this;
    }
};
