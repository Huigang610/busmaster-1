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
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

// FnsTreeView.h : header file
//
//#include <afxcview.h>   // Standard tree view header file
#include "FunctionEditorDoc.h"

class CFnsTreeView : public CTreeView
{
    DECLARE_DYNCREATE(CFnsTreeView)
    DECLARE_MESSAGE_MAP()

public:
    CFnsTreeView();           // protected constructor used by dynamic creation
    static BusType sm_eBus;
    void vSetItemName( CString );

    virtual void OnInitialUpdate();

protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    afx_msg void OnFunctionEditorSave();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnTreeItemSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemLeftClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnTreeViewRightclick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnDeleteHandle();
    afx_msg void OnAddHandle();
    afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnEditFunctionHeader();

private:
    BusType m_eBus;
    // Add new header
    void vOnNewIncludeHeader();
    // Edit selceted header
    void vEditIncludeHeader(HTREEITEM hItem);
    // Edit selceted utility function
    void vEditUtilityFunc(HTREEITEM hItem);
    // Edit selceted utility function
    void vEditTimerHandler(HTREEITEM hItem);
    // Edit the selected function
    void vEditSelectedFunction();
    // Add new global variable
    void vOnNewGlobalVariable();
    // New utility function
    void vOnNewUtilityFunction();
    // Add new key handler
    void vOnNewKeyHandler();
    // Add new timer handler
    void vOnNewTimerHandler();
    //  Add new message handler
    void vOnNewMessageHandler();
    //  Add new error handler
    void vOnNewErrorHandler();
    //  Add new event handler
    void vOnNewEventHandler();
    //  Add new DLL handler
    void vOnNewDLLHandler();
    //  Add new BUS Event handler
    void vOnNewBusEventHandler();
    // Delete a line from the code with the search string & header string
    BOOL bDeleteALineFromHeader(CString omStrHeader,CString);
    // Dlete selected handler
    BOOL bDeleteHandler(CString omStrFuncName,CFunctionEditorDoc* pDoc);

    BOOL bDeleteHandlerInArray(const CStringArray& omStrArray,CFunctionEditorDoc* pDoc);
    void vAddDeleteDllHandler(BOOL bDllLoad, BOOL bToAdd);

    // Right and left mouse click points
    CPoint m_omRightClickPoint;
    CPoint m_omLeftClickPoint;

    BOOL m_bFlag;
    // Image List object
    CImageList m_omImageList;
    // Populate tree with function prototypes
    // and global variables
    BOOL bPopulateTree();
};
