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
 * @brief     Implementation file for main sub entry entities.
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for main sub entry entities.
 */

/* Project includes */
#include "DataTypes_stdafx.h"
#include "MainSubEntry.h"

GuiParameters::GuiParameters()
{
    memset(titleName, 0, sizeof(titleName));
    memset(mainListName, 0, sizeof(mainListName));
    memset(unselectedListName, 0, sizeof(unselectedListName));
    memset(selectedListName, 0, sizeof(selectedListName));

    imageList      = NULL;
    unselectedIconIndex  = 0;
    selectedIconIndex    = 0;
    combine          = TRUE;
}

BOOL SignalWatchListSubEntry::operator ==(const SignalWatchListSubEntry& RefObj) const
{
    BOOL bResult = FALSE;
    if (/*(m_unSubEntryID == RefObj.m_unSubEntryID) || */
        (subEntryName == RefObj.subEntryName))
    {
        bResult = TRUE;
    }
    return bResult;
}

SignalWatchListMainEntry::SignalWatchListMainEntry()
{
    vResetAll();
}

SignalWatchListMainEntry::~SignalWatchListMainEntry()
{
    vResetAll();
}

void SignalWatchListMainEntry::vResetAll(void)
{
    m_unMainEntryID = 0;
    mainEntryName = "";
    m_odUnSelEntryList.RemoveAll();
    m_odSelEntryList.RemoveAll();
}

SignalWatchListMainEntry& SignalWatchListMainEntry::operator=(SignalWatchListMainEntry& RefObj)
{
    vResetAll();

    m_unMainEntryID     = RefObj.m_unMainEntryID;
    mainEntryName   = RefObj.mainEntryName;
    m_odUnSelEntryList.AddTail(&(RefObj.m_odUnSelEntryList));
    m_odSelEntryList.AddTail(&(RefObj.m_odSelEntryList));

    return *this;
}

BOOL SignalWatchListMainEntry::operator==(const SignalWatchListMainEntry& RefObj) const
{
    BOOL bResult = FALSE;
    if ((m_unMainEntryID == RefObj.m_unMainEntryID) ||
            (mainEntryName == RefObj.mainEntryName))
    {
        bResult = TRUE;
    }
    return bResult;
}
