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
 * @brief     Interface file for CTxFunctionsView class
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxFunctionsView class
 */

#pragma once

// TxFunctionsView.h : header file
#include "TxWindow_resource.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTxFunctionsView : public CFormView
{
    DECLARE_DYNCREATE(CTxFunctionsView)
    DECLARE_MESSAGE_MAP()

public:
    CTxFunctionsView();

	enum { IDD = IDD_DLG_TX_FUNCTIONS };
    CButton m_omButtonApply;

	void vAccessButtonApply();
    CButton m_CheckBoxAutoUpdate;

    virtual void OnInitialUpdate();
    void OnInvokeClose();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual ~CTxFunctionsView();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnButtonApply();
    afx_msg void OnBtnClose();
    afx_msg void OnBnClickedCheckAutoUpdate();

private:
    // To Update Configuration module with the changes done
    void vApplyChanges();
    // To reload TxMsg window with the configuration module Tx data
    void vReloadData();
    // To Get Parent window pointer. That is pointer of CTxMsgChildFrame
    CWnd* pomGetParentWindow() const;
    // To Get CTxMsgBlocksView Pointer
    CWnd* pomGetBlocksViewPointer() const;
    // To Get CTxMsgListView Pointer
    CWnd* pomGetListViewPointer() const;
    // To Get CTxMsgDetailsView Pointer
    CWnd* pomGetDetailsViewPointer() const;
    void SetUpdateBtnChanges();
};
