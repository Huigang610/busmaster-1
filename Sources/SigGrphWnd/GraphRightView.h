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
 * @brief     Interface file for CGraphRightView class
 * @author    Raja N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CGraphRightView class
 */

#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "GraphChildFrame.h"
#include "GraphSink.h"

class CGraphRightView : public CFormView
{
    DECLARE_DYNCREATE(CGraphRightView)
    DECLARE_EVENTSINK_MAP()
    DECLARE_MESSAGE_MAP()

public:
    enum { IDD = IDD_DLG_GRAPH_RIGHT_TOP };

    void CursorPositionGraphCtrl(double X, double Y, short shCursorID);
    IDMGraphCtrl* m_pDMGraphCtrl;

    void vHandleConnectionStatusChange(BOOL bConnectStatus);

    virtual void OnInitialUpdate();

protected:
    // protected constructor used by dynamic creation
    CGraphRightView();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual ~CGraphRightView();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
	afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
    // To get parent window pointer
    CWnd* pomGetParentWindow() const;
    CGraphChildFrame* m_pParentWnd;

    //Pointer to hold CGraphSink class pointer
    CGraphSink* m_pGraphSink;
    //Get hold of the Sink cookie ID
    DWORD m_dwCookie;
    CWnd* m_pWndGraphCtrl;
};
