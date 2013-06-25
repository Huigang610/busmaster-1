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
 * @author    Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#pragma once

/* Project includes */
#include "Utility/ColorSelector.h"
#include "messagelist.h"
#include "MainFrm.h"    // For Main Frame Definition

class CSigGrphConfigDlg : public CDialog
{
    DECLARE_DYNAMIC(CSigGrphConfigDlg)
    DECLARE_MESSAGE_MAP()

public:
    CSigGrphConfigDlg(UINT nHardware = defNO_OF_CHANNELS,CWnd* pParent = NULL);   // standard constructor

    // Pointer to Main Frame
    CMainFrame* m_pMainFrame;

    // Dialog Data
    enum { IDD = IDD_DIALOG_CONFIG_SIG_GRPH_WND };

    CColorSelector m_omLineColor;
    CColorSelector m_omPointColor;
    CButton m_omBtnVisible;
    CButton m_omBtnEnable;
    INT m_nSelectedIndex;
    CMessageList m_omSignalList;
    CComboBox m_omCmbBusType;
    afx_msg void OnCbnSelchangeComboBusType();
    afx_msg void OnLvnItemchangedListSignals(NMHDR* pNMHDR, LRESULT* pResult);
    int m_nSymbolType;
    int m_nLineType;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtnVisible();
    afx_msg void OnBnClickedBtnEnable();
    afx_msg void OnBnClickedBtnConfigure();
    afx_msg void OnBnClickedBtnLineColor();
    afx_msg void OnBnClickedButtonGrpDispConfig();
    afx_msg void OnCbnSelchangeComboType();
    afx_msg void OnCbnSelchangeComboSymbol();
    afx_msg void OnCbnSelchangeComboDisplay();
    afx_msg LRESULT OnColorChange( WPARAM wparam,LPARAM lparam);

private:
    void vEnableControls(BOOL bEnable);
    void vPopulateElementList();
    void vEnableDisableControls();
    void vSetElementDetails( CGraphElement odElement );
    void vUpdateLineDisplayOfElements(eDISPLAY_TYPE eCurrDisplay);
    UINT m_nHardware;
};
