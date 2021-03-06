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
 * @brief     Interface file for CMessageInterpretation class
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CMessageInterpretation class
 */

#pragma once

typedef struct tagSMESSAGE_ENTRY_INFO
{
    UINT m_unMessageID;
    CString m_omMessageName;
    UCHAR m_ucDataLen;
    UCHAR m_ucData[8];
    // Constructor to initialise the data members
    tagSMESSAGE_ENTRY_INFO()
    {
        m_unMessageID     = 0;
        m_omMessageName   = "";
        m_ucDataLen = 0;
        memset(m_ucData,0,8);
    }
} SMESSAGE_ENTRY_INFO;

class CMessageInterpretation : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    void vCreateMsgIntrprtnDlg(CWnd* pomParent, BOOL bShow);
    void vUpdateMessageData(const SMESSAGE_ENTRY_INFO&, BOOL);
    void vUpdateMessageData(UINT unMsgID, const CString& omStrMsgName,
                            const CStringArray& omSASignals,
                            const CStringArray& omSARaw,
                            const CStringArray& omSAPhy,
                            const CStringArray& omSAUnits, BOOL bHexON);
    // Window Placement routines
    // Save window status
    void vUpdateWndCo_Ords();
    // Load window status
    void vUpdateWinStatus();
    void vSetCaption(CString strCaption);

    void vClearWindowContent();
    CMessageInterpretation(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    enum { IDD = IDD_DLG_INTERPRET_MSG };
    CFFListCtrl   m_ctrlSignal;
    CString m_omMsgName;
    CString m_omMsgID;

    virtual BOOL DestroyWindow();
    CString m_strCaption;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnClose();
    afx_msg void OnCancel();
    afx_msg void OnOK();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
    // To keep window co ordinates
    WINDOWPLACEMENT m_sWinCurrStatus;
    // Save Window status - private member
    void vSaveWinStatus();
    // Get the status from the config
    void vGetWinStatus();

    HBRUSH m_hBrushStatic;
    HWND m_hWndParent;

    void vResizeControls(void);
};
