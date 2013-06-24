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
 * @brief     This file contain the definition of CDisplayIntMsg class
 * @author    Amitesh Bharti
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CDisplayIntMsg class
 */

#pragma once

class CDisplayIntMsg : public CDialog
{
public:
    CDisplayIntMsg(CONST CStringArray& omStrStringArray);

    enum { IDD = IDD_DLG_INTERPRETED_MSG };
    CEdit   m_omEditControl;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();

private:
    CStringArray m_omInterpretedMsg;
};
