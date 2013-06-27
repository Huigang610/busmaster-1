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
 * @brief     This file contains the definition of CMsgIDAttr class
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains the definition of CMsgIDAttr class
 */

#pragma once

class CRadixEdit;   // forward declaration

class CMsgIDAttr : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    bool m_bForEdit;
    BOOL m_bDBMessage;
    UINT m_nID;
    UINT m_nPreviousID;
    COLORREF m_sColour;
    CMsgIDAttr(BusType eBusType, CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    enum { IDD = IDD_DLG_MSGID_ATTRIB };
    CStatic m_omColourBox;
    CRadixEdit  m_odCanIDVal;
    CString m_omStrMsg;

    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnPbtnColour();
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
    HBRUSH m_hClrBtn;
    BusType m_eBusType;
};
