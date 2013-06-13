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
 * @file      SignalWatchListBox.h
 * @brief     Interface file for CSignalWatchListBox class
 * @authors   Ratnadip Choudhury, Amarnath Shastri
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CSignalWatchListBox class
 */

#pragma once

/* Project includes */
#include "Utility/FFListCtrl.h"	// For Flicker Free List Control Definiton

class CSignalWatchListBox : public CFFListCtrl
{
	DECLARE_MESSAGE_MAP()

public:
    CSignalWatchListBox();

	void vUpdateParentWnd(HWND hParentWnd);

    void OnChar(UINT nChar, UINT nRepeatCount, UINT nflags);
    void OnKeyDown(UINT nChar, UINT nRepeatCount, UINT nflags);
	
protected:
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnSgWatchClear();

private:
	HWND m_hParentWnd;
};
