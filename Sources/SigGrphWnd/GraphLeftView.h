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
 * @brief     Interface file for CGraphLeftView class
 * @author    Raja N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CGraphLeftView class
 */

#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "GraphElement.h"	// For Graph Element class
#include "MessageList.h"	// For Color ListCtrl class definition
#include "Utility/ColorSelector.h"	// For Color Pocker class definition

class CGraphLeftView : public CFormView
{
    DECLARE_DYNCREATE(CGraphLeftView)
    DECLARE_MESSAGE_MAP()

public:
    enum { IDD = IDD_DLG_GRAPH_LEFT_TOP };
    CButton m_omBtnVisible;
    CButton m_omBtnEnable;
    CMessageList    m_omSignalList;
    CColorSelector   m_omPointColor;
    CColorSelector   m_omLineColor;
    int     m_nLineType;
    int     m_nSymbolType;
    IDMGraphCtrl* m_pDMGraphCtrl;

    // To handle tool connection change event
    void vHandleConnectionStatusChange(BOOL bConnect);
    // To populate list with graph element details
    void vPopulateElementList();
    // To Populate graph control with list of elements
    void vPopulateGraphControl();
    virtual void OnInitialUpdate();

	int m_nLineDisplay;
    HRESULT GetCurrentElement(IDMGraphCtrl* pGraphCtrl, IDMGraphElement** ppGraphElement);

protected:
    // protected constructor used by dynamic creation
    CGraphLeftView();

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual ~CGraphLeftView();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    afx_msg void OnBtnVisible();
    afx_msg void OnItemchangedListSignals(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangeComboType();
    afx_msg void OnSelchangeComboSymbol();
    afx_msg void OnBtnEnable();
    afx_msg LRESULT OnColorChange( WPARAM wparam,LPARAM lparam);
    afx_msg LRESULT vHandleConfigFileChange( WPARAM wParam,LPARAM lParam);

private:
    BOOL m_bConnectionStatus;
    void vUpdateGraphControl(int nFromControl, long lIndex);
    // To get selected item details
    //CGraphElement odGetSelectedElement();
    // To update UI controls with the element details
    void vSetElementDetails(CGraphElement odElement);
    // To Enable/ Disable controls.using the value bEnable
    void vEnableControls(BOOL bEnable);
    // To Enable/ Disable controls based on selection in UI element List
    void vEnableDisableControls();
    // To get parent window pointer
    CWnd* pomGetParentWindow() const;
};
