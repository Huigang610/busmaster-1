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
 * @file      MsgReplayWnd.h
 * @brief     This file contain the definition of CMsgReplayWnd class
 * @authors   Amitesh Bharti, Krishnaswamy B.N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CMsgReplayWnd class
 */

#pragma once

/* Project includes */
#include "ReplayFile.h"	// For Replay File Definition
#include "OwnerDataListCtrl.h"
#include "ReplayProcess.h"	// For replay process class

class CMsgReplayWnd : public CMDIChildWnd
{
    DECLARE_MESSAGE_MAP()

public:
    // constructor used by dynamic creation
    CMsgReplayWnd(CReplayFile ouReplayFile, WINDOWPLACEMENT& sWndPlacement);

    // Replay Operations
    VOID vCmdSkip();
    VOID vCmdStop();
    VOID vCmdStep();
    VOID vCmdGo();
    // UI Update Functions
    BOOL bGetUIStateCmdStep();
    BOOL bGetUIStateCmdSkip();
    BOOL bGetUIStateCmdGo();
    BOOL bGetUIStateCmdStop();
    // To open and parse the repaly file
    BOOL bOpenReplayFile();
    // To create repaly window with list control
    BOOL bCreateReplayWindow();
    // To set running thread stop signal
    BOOL bSetThreadStopSignal();
    // To kill the thread after wait
    BOOL bStopReplayThread();
    // Friend classes of CMsgReplayWnd class
    friend class CReplayProcess;
    friend class CLogFileCSVExport;
    // Replay process information
    CReplayProcess  m_ouReplayDetails;

protected:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnClose();
    afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg LRESULT vHandleListControlDataReq(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vListDoubleClick(WPARAM wParam, LPARAM lParam);

private:
    // To set list control font
    VOID SetWindowFont();
    // To get selected messages indeces
    VOID vGetSelectedIndeces( CArray<int,int>& omIndexList );
    // Font for list control
    CFont m_omNewFont;
    // Lixt control with owner data support
    COwnerDataListCtrl m_omMessageList;
    // List control Image List
    CImageList m_omImageList;
    // Reply flag
    eReplayStates m_eReplayState;

    //Window placement
    WINDOWPLACEMENT m_sWndPlacement;
};
