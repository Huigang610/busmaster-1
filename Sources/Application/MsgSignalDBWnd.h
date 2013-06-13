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
 * @file      MsgSignalDBWnd.h
 * @brief     This header file contains the defintion of class
 * @author    Amarnath Shastry
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

/* Project includes */
#include "CFilesDefs_CAN.h"
#include "MsgSgDetView.h"       // Forms the right pane

class CMsgSignalDBWnd : public CMDIChildWnd
{
    DECLARE_DYNCREATE(CMsgSignalDBWnd)
    DECLARE_MESSAGE_MAP()

public:
    static BOOL sm_bValidJ1939Wnd;

    SDBPARAMS m_sDbParams;

    CMsgSignalDBWnd();
    CMsgSignalDBWnd(const SDBPARAMS& sDbParams);
    void vSetDBName(CString& omDBName);
    void vSaveModifiedDBs(CMsgSignal**& ppTempMsgSg);
    CWnd* GetWorkingView();

protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

    afx_msg LRESULT OnSaveDBJ1939(WPARAM wParam, LPARAM lParam);
    afx_msg void OnClose();

private:
    void vCalculateSplitterPosition(CSize& cSize);
    BOOL m_bSplitWndCreated;
    CSplitterWnd m_omSplitterWnd;
};
