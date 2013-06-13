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
 * @file      DLLHandlerDlg.h
 * @brief     This file contain declaration of CDLLHandlerDlg class.
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain declaration of CDLLHandlerDlg class.
 */

#pragma once

class CDLLHandlerDlg : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    CStringArray m_omStrArrayDLLHandler;

    CDLLHandlerDlg(BYTE byType, CFunctionEditorDoc* pDoc, CWnd* pParent = NULL, BOOL bIsDelete = FALSE);   // standard constructor

    enum { IDD = IDD_DLG_DLL_HANDLER };
    CButton m_omCheckDLLUnLoad;
    CButton m_omCheckDLLLoad;
    BOOL    m_bCheckDLLLoad;
    BOOL    m_bCheckDLLUnLoad;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnCbtnDllCancel();

private:
    CFunctionEditorDoc* m_pDoc;
    BOOL m_bIsDelete;
    BYTE m_byType;
};
