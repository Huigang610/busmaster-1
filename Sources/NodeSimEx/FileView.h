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
 * @brief     This header file contains the defintion of class ileView. All
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class ileView. All
 */

#pragma once

#include "HashDefines.h"        // #defines defintion file
#include "FunctionEditorDoc.h"  // Document defintion file

class CFileView : public CScrollView
{
    DECLARE_DYNCREATE(CFileView)
    DECLARE_MESSAGE_MAP()

public:
    CFileView();

    static ETYPE_BUS sm_eBus;
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

    // Takes the cursor to the  specified line number
    void vGoToLine( UINT unLineNo );

    // Highlights the line number specified
    void vDisplayWarningLineNumber(eMESSAGEFROM eMsgFrom,UINT unLineNo);

    // Gets valid document pointer
    CFunctionEditorDoc* omGetDocument();

    virtual ~CFileView();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected: // create from serialization only
    virtual void OnInitialUpdate(); // called first time after construct

    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

private:
    // character height information
    int     m_nCharHeight;
    // character width information
    int     m_nCharWidth;

    ETYPE_BUS m_eBus;
};
