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
 * @brief     This header file contains the defintion of class
 * @author    Amarnath Shastry
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

#include "HashDefines.h"    // #define file
#include "GlobalObj.h"

class CExploreMsgSg : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    // Holds selected message name
    CString m_omStrMessageName;

    // Holds selected item, message or signal
    CString m_omStrSelectedItemText;

    // Overrided constructor
    CExploreMsgSg(CFunctionEditorDoc* pDoc,
                  CMsgNameMsgCodeListDataBase& odMsgNameCodeListDb,
                  BOOL bCheckRequired,
                  eMESSAGEFROM,
                  eSELTYPE,
                  CWnd* pParent = NULL);

    enum { IDD = IDD_MSG_SG_DLG };
    CListBox    m_omSignalListBox;
    CListCtrl   m_omMsgList;
    BOOL    m_bWantStructure;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    afx_msg void OnDblclkListMsgs(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblclkListSignals();
    afx_msg void OnSelect();
    virtual BOOL OnInitDialog();
    afx_msg void OnCbtnCancel();
    afx_msg void OnItemchangedLstcMsgs(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickLstcMsgs(NMHDR* pNMHDR, LRESULT* pResult);

private:
    CFunctionEditorDoc* m_pDoc;
    CMsgNameMsgCodeListDataBase& m_odMsgNameCodeListDb;         //CAPL_DB_NAME_CHANGE
    //CMsgNameMsgCodeList& m_odMsgNameCodeList;
    // Dialog class may be called by any window
    eMESSAGEFROM m_eWindow;
    // Flag to enable check box option
    BOOL m_bCheckRequired;
    // enum to indicate whether to
    // select message or signal
    eSELTYPE m_eSelectType;
    // Store last selected message item
    int m_nMsgIndex;
    //Get Signal Names from Message Id
    void vGetSigNamesFromMsgCode(DWORD dwMsgCode, CStringList& omSignalNames);
};
