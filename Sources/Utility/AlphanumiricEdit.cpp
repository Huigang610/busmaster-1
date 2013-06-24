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
 * @file      AlphanumiricEdit.cpp
 * @brief     Implementation file for CAlphanumiricEdit class
 * @author    Anish Kumar
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CAlphanumiricEdit class
 */

/* Project includes */
#include "Utils_stdafx.h"
#include "AlphanumiricEdit.h"
#include "RadixEdit.h" //for backspace definintion

IMPLEMENT_DYNAMIC(CAlphanumiricEdit, CEdit)

BEGIN_MESSAGE_MAP(CAlphanumiricEdit, CEdit)
    ON_WM_CHAR()
END_MESSAGE_MAP()

/**
 * \param[in] nChar character code value of the key
 * \param[in] nRepCnt repeat count
 * \param[in] nFlags flag
 *
 * This prevent an user to insert special characters in
 * message ar signal name.
 */
void CAlphanumiricEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if(nChar=='_' || isalnum(nChar)!= 0 || nChar == BACK_SPACE || (nChar>=0 && nChar <=32) )
    {
        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
}
