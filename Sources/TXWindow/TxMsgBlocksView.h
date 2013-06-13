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
 * @file      TxMsgBlocksView.h
 * @brief     Interface file for CTxMsgBlocksView class
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxMsgBlocksView class
 */

#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "afxwin.h"
#include "TxWindow_resource.h"
#include "Utility/RadixEdit.h"          // For the RAdix Edit control definition
#include "Utility/FFListCtrl.h"

class CTxMsgBlocksView : public CFormView
{
    DECLARE_DYNCREATE(CTxMsgBlocksView)
    DECLARE_MESSAGE_MAP()

public:
    enum { IDD = IDD_DLG_TX_MSG_BLOCKS };
    bool        m_bNewBlock;    //used to check whether its new block is added or not
    bool        m_bDelayBtnBlocks;      //this will store the value of delay between blocks locally,
    //this is needed to keep a back up of the value in case when
    //user doesnt want to save the data, this should not be used anywhere
    //other than to restore the value to global variables on selection of close and not saving
    CFFListCtrl   m_omLctrMsgBlockName;
    CString m_omStrMsgBlockName;
    CString m_omStrTimeIntervalVal;
    CString m_omStrKeyVal;
    CString m_omDelayBtnBlocks;
    //BOOL    m_bIsMonoshot;
    //BOOL    m_bTriggerType;
    CEdit   m_omEditMsgBlockName;
    //CButton m_omButtonTriggerType;
    CRadixEdit m_omEditTrgTimeIntervalVal;
    CRadixEdit m_omEditTrgKeyVal;
    CButton m_omButtonKeyTrigger;
    CButton m_omButtonTimeTrigger;
    CButton m_omButtonAddMsgBlock;
    //CButton m_omButtonTxAllFrame;
    CButton m_omButtonDeleteMsgBlock;
    CButton m_omDelayBtwnBlocks;
    CRadixEdit   m_omTimeDelayBtwnBlocks;
    BOOL    m_bListItemChange;
    //BOOL    m_bTXAllFrame;

	// To denote modifications after connect
    BOOL m_bModified;
    // To store selected message block index
    int m_nSelectedMsgBlockIndex;
    // To Keep local copy of Tx Msg List
    PSMSGBLOCKLIST m_psMsgBlockList;
    // To denote programmed modification of block list so that onchange list item
    // handler will not be exccuted
    BOOL m_bInitDlg;
    // To keep track of message block count
    UINT m_unMsgBlockCount;

	// To get message block pointer denoted by the index from the given
    // list of blocks
    SMSGBLOCKLIST* psGetMsgBlockPointer(INT nIndexCurBlock,
                                        SMSGBLOCKLIST* psMsgBlockList);
    // To update given block information in the UI
    VOID vUpdateMsgBlockDetials(SMSGBLOCKLIST* psCurrentMsgBlock);
    // To enable or disable list of controls
    void vEnableControls(BOOL bEnable);
    // To display given message block information in UI
    VOID vDisplayMsgBlockDetails(SMSGBLOCKLIST* psMsgBlock);
    // To set control properties like limit text, base etc.
    void vSetControlProperties();
    //Updates the contents of listview to list
    void UpdateList(NM_LISTVIEW* pNMListView);

    CComboBox m_omComboAllMsgs;
    CTxMsgBlocksView();
    afx_msg void AutoUpdateChanges();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual ~CTxMsgBlocksView();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    virtual void OnInitialUpdate();

	afx_msg void OnBnClickedRadiomonoshot();
    afx_msg void OnBnClickedRadiocyclic();
    afx_msg void OnCbnSelchangeComboMsgs();
    afx_msg void OnBnClickedCheckMsgBlockDelay();
    afx_msg void OnEnUpdateEditBlockTrgTimerVal();
    afx_msg void OnAddMsgBlock();
    afx_msg void OnDeleteSelectedMsgBlock();
    afx_msg void OnItemchangedLstcMsgBlocksName(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeEditMsgBlockName();
    afx_msg void OnChkbTriggerType();
    afx_msg void OnChkbOnTimeTrigger();
    afx_msg void OnChkbOnKeyTrigger();
    afx_msg void OnUpdateEditTrgTimeVal();
    afx_msg void OnUpdateEditTrgKeyVal();
    afx_msg void OnChkbTxAllFrame();
    afx_msg void OnRclickLstcMsgBlocksName(NMHDR* pNMHDR, LRESULT* pResult);

private:
    // To keep the block deletion flag
    BOOL m_bMsgBlockDeleted;
    // To store trigger type of message block
    unsigned char m_nRBTNTriggerType;

    // To retain the previous value of time delay between messges
    int m_nTimeDelMsg;

    // To retain the previous value of time delay between blocks
    int m_nTimeDelBlocks;

    // Functions
    // To add a message block
    BOOL bAddBlock(SMSGBLOCKLIST*& psMsgCurrentBlock);
    // To initialise message block with default values
    void vInitMsgBlockInfo(SMSGBLOCKLIST* psMsgBlockList);
    // To delete selected message block
    BOOL bDeleteBlock();
    // To get parent window pointer
    CWnd* pomGetParentWindow() const;
    // To get CTxMsgListView pointer
    CWnd* pomGetListViewPointer() const;
    // To get CTxFunctionsView Pointer
    CWnd* pomGetFunctionsViewPointer() const;
    // To get CTxMsgDetailsView Pointer
    CWnd* pomGetDetailsViewPointer() const;
    // To show popup memu
    BOOL bDisplayPopMenu(CListCtrl& omList, UINT nIDResource );
    // Save the current msg block
    void vSaveCurrentBlockFirst(void);
};
