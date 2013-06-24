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
 * @brief     This file contain the definition all of CDefineTimerHandler
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all of CDefineTimerHandler
 */

#pragma once

/* Project includes */
#include "Utility/RadixEdit.h"
#include "FunctionEditorDoc.h"
#include "NodeSimEx_resource.h"

class CDefineTimerHandler : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    CString m_omStrTimerFunctionName;
    CDefineTimerHandler(CFunctionEditorDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

    enum { IDD = IDD_DLG_DEFINE_TIMER_HANDLER };
    CRadixEdit  m_omEditTimerValue;
    CEdit   m_omEditTimerFuncName;
    CString m_omEditStrTimerFuncName;
    CString m_omStrCurrentTimerName;
    UINT    m_unEditTimerValue;

    // Check if timer is created new or existing timer is edited
    void vSetTimerEdited(BOOL bEdited);
    BOOL bIsTimerEdited();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    virtual void OnOK();
    virtual BOOL OnInitDialog();

private:
    CFunctionEditorDoc* m_pDoc;
    BOOL    m_bIsTimerEdited;
};
