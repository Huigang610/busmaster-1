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

#include "Utility/RadixEdit.h"
#include "Include/BaseDefs.h"
#include "SimSysNodeInfo.h"
#include "NodeSimEx_resource.h"

class CSimSysDetView : public CFormView
{
    DECLARE_DYNCREATE(CSimSysDetView)
    DECLARE_MESSAGE_MAP()

public:
    CSimSysDetView();           // protected constructor used by dynamic creation
    enum { IDD = IDD_DLG_SIMSYSDETVIEW };
    CListCtrl   m_omListCtrlHanDet;
    CListCtrl   m_omListCtrlHanVal;
    CString m_omStrNodeName;
    CString m_omStrCFile;

    void vEnableButtons (BOOL bEnable );
    BOOL bGetControlStatus();
    void vHideControls(UINT unEnableHide);
    void vChangeLUButtonText(BOOL bIsLoaded);

    void vChangeEDAllHanButtonText(BOOL bIsEnabled);

    void vDisplayNodeInformation(PSNODEINFO pNodeInfo);
    void vEnableHandlerButtons( BOOL bIsEnabled );
    void vGetNodeInfo(PSNODEINFO psNodeInfo);
    void vUpdateHandlerList();
    void vSetNodeAddress(BYTE byAddress);
    void vUpdateHandlerEnableDisableStatus(int nItem, BOOL bEnableHandler);

    static ETYPE_BUS CSimSysDetView::sm_eBus;
    CRadixEdit m_omPrefAdres;
    CRadixEdit m_omECU_NAME;

    virtual void OnInitialUpdate();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void PostNcDestroy();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    afx_msg void OnButtonBuild();
    afx_msg void OnButtonBuildandload();
    afx_msg void OnButtonEditfile();
    afx_msg void OnButtonEnableDisableallhandlers();
    afx_msg void OnButtonEnableDisablehandler();
    afx_msg void OnButtonLoadUnload();
    afx_msg void OnButtonOpenfile();
    afx_msg void OnClickLstcHandlerDetails(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedLstcHandlerDetails(NMHDR* pNMHDR, LRESULT* pResult);

private:
    ETYPE_BUS m_eBus;
    CStringList m_omSourceCodeTextList;
    CStringArray m_omIncludeFileArray ;
    DWORD m_dwSourceCodeLineNo;
    PSNODEINFO m_psNodeInfo;
    BOOL bConnected;
    BOOL m_bIsTimerHanEnabled;
    BOOL m_bIsKeyHanEnabled;
    BOOL m_bIsMsgHanEnabled;
    BOOL m_bIsErrorHanEnabled;
    BOOL m_bIsEventHanEnabled;

    BOOL m_bAreControlsHidden;
    BOOL m_bIsDllLoadSuccess;
    void vAddItemToHanDetList( sNODEINFO* pNode );
    void vEnableHandlerDetails( BOOL bIsEnabled );
    void vUpDownArrowKeySelection(int nSel);
    void vSetNodeInfoEDHanStatus( int nSelItem , BOOL bIsEnabled );
    BOOL bUpdateNodeInfoFile(CString omStrCFile);

    void vChangeEDHanButtonText(BOOL bIsEnabled);
    void vUpdateNodeDetailsAndView();
    void vSetHandlerDetailRowText(int nRow, int nNoOfHandler, BOOL bEnabled);
};
