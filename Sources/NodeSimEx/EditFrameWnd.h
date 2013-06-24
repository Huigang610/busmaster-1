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

/* Project includes */
#include "HashDefines.h"    // #define file
#include "NodeSimEx_resource.h"

class CEditFrameWnd : public CMDIChildWnd
{
    DECLARE_DYNCREATE(CEditFrameWnd)
    DECLARE_MESSAGE_MAP()

public:
    CEditFrameWnd();

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void ActivateFrame(int nCmdShow);

    static ETYPE_BUS sm_eBus;

protected:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    void vCalculateSplitterPos(CSize&);
    virtual ~CEditFrameWnd();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

private:
    ETYPE_BUS m_eBus;

    // Horizontal splitter
    CSplitterWnd m_omSplitterWndTwo;
    // Vertical splitter
    CSplitterWnd m_omSplitterWndOne;
    // Flag for successfull spliter creation
    BOOL m_bIsSplitterCreated;
};
