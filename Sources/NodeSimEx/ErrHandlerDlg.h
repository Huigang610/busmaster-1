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
 * @brief     This file contain the definition of CErrHandlerDlg class
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CErrHandlerDlg class
 */

#pragma once

/* Project includes */
#include "NodeSimEx_resource.h"

class CErrHandlerDlg : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    CStringArray m_omStrArrayErrorHandler;
    CErrHandlerDlg(CFunctionEditorDoc* pDoc, CWnd* pParent = NULL,BOOL bIsDelete = FALSE);

    enum { IDD = IDD_DLG_ERROR_HANDLER };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();
    afx_msg void OnCbtnErrorCancel();
    afx_msg void OnCbtnErrorOk();

private:
    CFunctionEditorDoc* m_pDoc;
    BOOL m_bIsDelete;
};
