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
 * @file      SelectFunctions.h
 * @brief     This header file contains the defintion of class
 * @author    Amarnath Shastry
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

/* Project includes */
#include "NodeSimEx_resource.h"
#include "Include/BaseDefs.h"

class CSelectFunctions : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    CString m_omSelectedFunctionPrototype;
    CString m_omStrSelectedText;
    CSelectFunctions(ETYPE_BUS eBus, CWnd* pParent = NULL);   // standard constructor

    enum { IDD = IDD_DLG_SELECT_FNS };
    CListBox    m_omFnListBox;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListFunctions();
    afx_msg void OnSelect();
    afx_msg void OnCancel();

private:
    ETYPE_BUS m_eBus;
    BOOL bFormatFunctionPrototype(CString& omStrFunctionPrototype);
};
