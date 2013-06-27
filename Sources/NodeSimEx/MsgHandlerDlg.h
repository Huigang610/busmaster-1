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
 * @brief     This file contain the definition of CMsgHandlerDlg class
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CMsgHandlerDlg class
 */

#pragma once

// MsgHandlerDlg.h : header file
//

#include "NodeSimEx_resource.h"
#include "Utility/RadixEdit.h"
#include "FunctionEditorDoc.h"
#include "SimSysManager.h"

class CMsgHandlerDlg : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    CString m_omStrSelectedItemText;
    CString m_omStrFunctionAdded;
    CMsgHandlerDlg(BusType eBus, CWnd* pParent = NULL);   // standard constructor

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    afx_msg void OnCbtnMsgCancel();
    afx_msg void OnCbtnMsgOk();
    afx_msg void OnCbtnMsgHandlerApply();
    afx_msg void OnRbtnMsgAll();
    afx_msg void OnRbtnMsgId();
    afx_msg void OnRbtnMsgName();
    afx_msg void OnRbtnMsgRange();
    afx_msg void OnSelchangeLstbMsgHandlerList();
    afx_msg void OnUpdateEditMsgHandlerId();
    afx_msg void OnUpdateEditMsgHandlerIdFrom();
    afx_msg void OnUpdateEditMsgHandlerIdTo();
    virtual BOOL OnInitDialog();

private:
    void vInitDlgWithBusSpecNames(void);
    void vTokenize(CString strInput, CString strToken, CString& strOutput, int& nStart);
    enum { IDD = IDD_DLG_MSG_HANDLERS };
    CListBox    m_omListMsgName;
    CRadixEdit  m_odEditMsgIDTo;
    CRadixEdit  m_odEditMsgIDFrom;
    CEdit m_odEditMsgID;
    CButton m_omButtonApply;
    CButton m_omButtonOK;
    CString m_omStrMsgID;
    CString m_omStrMsgIDFrom;
    CString m_omStrMsgIDTo;

    BusType m_eBus;

    BOOL bAddMessageNameInListBox(const CStringArray* pomStrArray,
                                  const CString& omStrMessageName);
    BOOL bValidateUserSelection(CFunctionEditorDoc* pDoc);
    BOOL bValidateMessageNameAndID(const CStringArray* pomStrArray,
                                   const CString& omStrHandlerType,
                                   CString omStrMessageNameOrID);
};
