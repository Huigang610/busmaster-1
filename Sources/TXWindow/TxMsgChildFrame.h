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
 * @brief     Interface file for CTxMsgChildFrame class
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxMsgChildFrame class
 */

#pragma once

/* Project includes */
#include "Tx_MDIChildBase.h"       // For Common base class defintion
#include "TxFunctionsView.h"    // For CTxFunctionsView class defintions
#include "TxMsgBlocksView.h"    // For CTxFunctionsView class defintions
#include "TxMsgDetailsView.h"   // For CTxMsgDetailsView class defintions
#include "TxMsgListView.h"      // For CTxMsgListView class defintions

class CTxMsgChildFrame : public CMDIChildBase
{
    DECLARE_DYNCREATE(CTxMsgChildFrame)
    DECLARE_MESSAGE_MAP()

public:
    // To update window size and splitter after loading a conf file
    void vUpdateWinStatus();
    // To save window size and splitter postion before unloading a conf file
    void vUpdateWndCo_Ords();
    // Default constructor
    CTxMsgChildFrame();

    // To Set View Pointers
    void vSetTxMsgViewPointers(ETXMSGVIEWTYPE eViewIdentity, CWnd* pomWnd);
    // To get View pointers
    CWnd* pomGetTxMsgViewPointers(ETXMSGVIEWTYPE eViewIdentity) const;

    void vSetMsgDBPtrInDetailsView(void* pMsgDB);

    void vChangeConnectionStatus(BOOL bConnect);

    void vCallAutoUpdate();

protected:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
    afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnDestroy();
    afx_msg LRESULT vUserCommand(WPARAM wParam, LPARAM lParam);

    CSplitterWnd m_omRootSplitter;
    CSplitterWnd m_omLeftViewSplitter;
    CSplitterWnd m_omRightViewSplitter;

private:
    // Structure to hold splitter window postion
    STXMSGSPLITTERDATA m_sSplitterPosition;
    // Pointers to all views
    CTxMsgBlocksView* m_pomTxMsgBlocksView;
    // Message List view Pointer
    CTxMsgListView* m_pomTxMsgListView;
    // For functions view poniter
    CTxFunctionsView* m_pomFunctionsView;
    // Message Details view pointer
    CTxMsgDetailsView* m_pomTxMsgDetailsView;
    BOOL m_bInit;

    // Function to set splitter postion.
    void vSetSplitterPostion();
    // Function to get splitter postion from configuration file
    void vGetSplitterStatus();
    // To save splitter position from configuration file
    void vSaveSplitterPostion();
};
