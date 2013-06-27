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
 * @brief     This file contain declaration of all function and data
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain declaration of all function and data
 */

#pragma once

#include "NodeSimEx_resource.h"
#include "HashDefines.h"
class CTransmitMsg;

typedef struct tagSCALCEXECTIMETHREAD
{
    bool            m_bThreadStop;
    CWinThread*     m_pomThreadPtr;
    CEvent          m_omExitThreadEvent;
    CTransmitMsg*   m_pTransmitMsg;
    BusType       m_eBus; //Added for issue #356

    tagSCALCEXECTIMETHREAD()
    {
        m_bThreadStop  = FALSE;
        m_pomThreadPtr = NULL;
        m_pTransmitMsg = NULL;
        m_eBus = CAN;
    }

} SCALCEXECTIMETHREAD,*PSCALCEXECTIMETHREAD;

class CSetResetTimer : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    CSetResetTimer(BusType eBus, CWnd* pParent = NULL); // standard constructor
    void vSetResetAllTimers(CString om_StrNode,BOOL bEnable);
    void vTimerStatusChanged();
    PSTIMERHANDLER GetPointerOfHandler(PSTIMERHANDLERLIST psTimerHandl,
                                       int nSelectedItem);
    void vStopTimer();
    void vStartTimer();
    void vAddNewNodeTimerList(CString om_NodeName,PSTIMERHANDLERLIST psTimerStrList);
    void vDeleteNodeTimerList(CString om_NodeName);
    void vSetSimSysNodeArray(const CStringArray&,BOOL);
    void vRestoreTimerStatus(CString omNodeName,
                             PSTIMERHANDLERLIST psTimerListPtr);

    enum { IDD = IDD_DLG_SET_RESET_TIMER };
    CComboBox   m_omComboNodeName;
    CListCtrl   m_omTimerList;
    CString m_omStrNodeName;

    static BusType sm_eBus;

    virtual BOOL DestroyWindow();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg void OnBtnApply();
    afx_msg void OnChckMonoshot();
    afx_msg void OnUpdateEditTimerValue();
    afx_msg void OnBtnOk();
    afx_msg void OnItemchangedLstcTimerHandler(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangeNodeList();
    virtual void OnCancel();

private:
    void vSaveUpdatedTimerInformation();
    void vUpdateTimerInformation(PSTIMERHANDLERLIST ps_CurrTimeHandler,
                                 int nSelectedItem);
    void vDeleteTimerListOfNode(PSTIMERHANDLERLIST psListOfTimer);

    BusType m_eBus;
    PSNODETIMERLIST m_psFirstNodeTimerList;
    PSNODETIMERLIST m_psLastNodeTimerList;
    BOOL m_bApplyButtonPressed;
    BOOL m_bDialogGettingInit;
    //For storing the ID of timer
    UINT m_unTimerID;
    CWinThread* ExecuteThread1 ;
    PSTIMERHANDLERLIST m_psSelectedNodeTimerListPtr;
    int m_nSelectedNode;
    CStringArray m_omSysNodeName;          //store node from simulated system
    BOOL m_bSimSysNodes;                   //if dialog box from simsys or mainframe
    BOOL m_bNodeNameChanged ;
    PSCALCEXECTIMETHREAD m_psCalTimerThreadStruct;
};
