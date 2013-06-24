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
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

struct GuiParameters
{
    GuiParameters();

	char       titleName[MAX_PATH];
    char       mainListName[MAX_PATH];
    char       unselectedListName[MAX_PATH];
    char       selectedListName[MAX_PATH];

	/**
	 * Pointer to loaded imagelist
	 */
    CImageList* imageList;

	/**
	 * Icon will appear in left list control
	 */
    INT unselectedIconIndex;

	/**
	 * Icon will appear in right list control
	 */
    INT selectedIconIndex;

	/**
	 * Whether Main Entry should be combined with subentry name while displaying in the list
	 */
    BOOL combine;
};

typedef struct SignalWatchListSubEntry
{
	/**
	 * ID
	 */
    unsigned int m_unSubEntryID;

	/**
	 * Name
	 */
    CString subEntryName;

    BOOL operator ==(const SignalWatchListSubEntry& RefObj) const;
} SignalWatchListSubEntry;

typedef CList<SignalWatchListSubEntry, SignalWatchListSubEntry&> SignalWatchListSubEntries;

/**
 * Structure for Signal Watch Implementation
 */
typedef struct SignalWatchListMainEntry
{
	/**
	 * ID
	 */
    UINT m_unMainEntryID;

	/**
	 * Name
	 */
    CString mainEntryName;

	/**
	 * Sub entries which are not selected
	 */
    SignalWatchListSubEntries m_odUnSelEntryList;

	/**
	 * Sub entries which are selected
	 */
    SignalWatchListSubEntries m_odSelEntryList;

    SignalWatchListMainEntry();
    ~SignalWatchListMainEntry();
    void vResetAll(void);
    SignalWatchListMainEntry& operator=(SignalWatchListMainEntry& RefObj);
    BOOL operator==(const SignalWatchListMainEntry& RefObj) const;
} SignalWatchListMainEntry;

typedef CList<SignalWatchListMainEntry, SignalWatchListMainEntry&> SignalWatchListMainEntries;
