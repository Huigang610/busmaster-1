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
 * @brief     Interface file for CMessageList class
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CMessageList class
 */

#pragma once

/* Project includes */
#include "Utility/FFListCtrl.h"	// Definition of Flicker Free List Control

class CMessageList : public CFFListCtrl
{
    DECLARE_MESSAGE_MAP()

public:
    CMessageList();

protected:
    // Overwride to implement custom draw
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

    afx_msg void OnDestroy();

private:
    // Row String
    CString m_omStrInARow;
    // GDI Objects for background and selection brush
    HBRUSH m_hWhiteBrush, m_hBlueBrush;
};
