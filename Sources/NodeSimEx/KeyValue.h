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
 * @brief     This file contain the definition all of CKeyValue class
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all of CKeyValue class
 */

#pragma once

#include "NodeSimEx_resource.h"

class CKeyValue : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    CKeyValue(CFnsTreeView* pFnsTreeView, CFunctionEditorDoc* pDoc,
              CWnd* pParent = NULL,char* pcKeyVal = (char*)NULL);

    enum { IDD = IDD_DLG_KEY_VALUE };

    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg void OnCbtnKeyApply();

private:
    CFnsTreeView* m_pFnsTreeView;
    CFunctionEditorDoc* m_pDoc;
    char* m_pcKeyVal;
    bool m_bDataSaved;
    BOOL bValidateDuplicateKeyHandler(CFunctionEditorDoc* pDoc);
};
