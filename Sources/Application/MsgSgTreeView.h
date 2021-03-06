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

#include <afxcview.h>
#include "CFilesDefs_CAN.h"

class CMsgSgTreeView : public CTreeView
{
    DECLARE_DYNCREATE(CMsgSgTreeView)
    DECLARE_MESSAGE_MAP()

public:
    static SDBPARAMS sm_sDbParams;

    CString m_omSelectedItemText;
    void vSetAllItemsNormal();
    void vSetTextBold();
    void vSetRootItemText(CString);
    BOOL m_bIsNewMessage;
    void vSetMessageName(CString omStrMsgName);

    virtual void OnInitialUpdate();

protected:
    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRightClickTreeItem(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnDeleteMessage();
    afx_msg void OnNewMessage();
    afx_msg void OnInfoToolTip(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnEditMsg();
    afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnTvnKeydown(NMHDR* pNMHDR, LRESULT* pResult);

    CMsgSgTreeView();           // protected constructor used by dynamic creation

private:
    SDBPARAMS m_sDbParams;

    CString m_omCurrDbName;

    void vAddEditMessage(BOOL bMode);
    //  CToolTipCtrl* m_pomToolTip;
    CPoint m_omRightClickPoint;
    CPoint m_omLeftCLickPoint;
    CImageList m_omImageList;
    HTREEITEM m_hTreeItem;
    BOOL bPopulateTree();
};
