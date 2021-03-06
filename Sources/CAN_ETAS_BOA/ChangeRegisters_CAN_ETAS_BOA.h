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
 * @brief     CChangeRegisters_CAN_ETAS_BOA dialog
 * @author    Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CChangeRegisters_CAN_ETAS_BOA dialog
 */

#pragma once

/* Project includes */
#include "Utility/RadixEdit.h"
#include "Math.h"
#include "Include/Struct_Can.h"
#include "Include/CanUsbDefs.h"
#include "ChangeRegDefines.h"

#define defMAXPropDelay 8

class CChangeRegisters_CAN_ETAS_BOA : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    CChangeRegisters_CAN_ETAS_BOA(CWnd* pParent = NULL, PSCONTROLLER_DETAILS psControllerDetails = NULL, UINT nHardwareCount = 0);

    /**
     * To Fill controller information taken from configuration module
     */
    BOOL   bFillControllerConfig();
    BOOL bSetBaudRateFromCom(int nChannel,BYTE bBTR0,BYTE bBTR1);
    BOOL bGetBaudRateFromCom(int nChannel,BYTE& bBTR0,BYTE& bBTR1);
    BOOL bSetFilterFromCom(BOOL  bExtended, DWORD  dBeginMsgId,
                           DWORD dEndMsgId);
    BOOL bGetFilterFromCom(BOOL& bExtended, double& dBeginMsgId, double& dEndMsgId);
    INT nGetInitStatus();

protected:
    enum { IDD = IDD_DLG_CHANNEL_CONFIGURATION };
    CListCtrl   m_omChannelList;
    CRadixEdit  m_omEditWarningLimit;
    CComboBox   m_omCombSampling;
    CListCtrl   m_omListCtrlBitTime;
    CRadixEdit   m_omEditBaudRate;
    CString m_omStrEditBTR0;
    CString m_omStrEditBTR1;
    CString m_omStrEditCNF1;
    CString m_omStrEditCNF2;
    CString m_omStrEditCNF3;
    CString m_omStrComboSampling;
    CString m_omStrEditBaudRate;
    CString m_omStrEditWarningLimit;
    CString m_omStrSamplePoint;
    CString m_omStrSJW;
    DOUBLE  m_dEditBaudRate;
    UINT    m_unCombClock;
    CComboBox m_omCtrlSamplePoint;
    CComboBox m_omCtrlSJW;
    CImageList m_omChannelImageList;

    /* CAN FD Parameters */
    CString m_omstrDataBitRate;
    CString m_omstrDataSamplePoint;
    CString m_omstrDataBTL_Cycles;
    CString m_omstrDataSJW;
    CString m_omstrTxDelayCompensationON;
    CString m_omstrTxDelayCompensationQuanta;
    CString m_omstrRxCompatibility;
    CString m_omstrTxCompatibility;

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    afx_msg void OnKillfocusEditBaudRate();
    afx_msg void OnSelchangeCombSampling();
    afx_msg void OnSetfocusEditBaudRate();
    afx_msg void OnClickedOK();
    afx_msg void OnSetfocusCombSampling();
    afx_msg void OnClickListChannels(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedListChannels(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblclkListChannels(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnCbnSelchangeCombSjw();
    afx_msg void OnCbnSelchangeCombDelayCompensation();
    afx_msg void OnCbnSelchangeCombPropdelay();

private:
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();

    /**
     * Pointer to hold controller information
     */
    SCONTROLLER_DETAILS m_pControllerDetails[defNO_OF_CHANNELS];
    PSCONTROLLER_DETAILS psMainContrDets;
    int m_nLastSelection;
    SACC_FILTER_INFO m_sAccFilterInfo;
    USHORT  m_usBTR0BTR1;
    UCHAR   m_ucWarningLimit;
    UCHAR   m_ucControllerMode;
    BOOL    m_bDialogCancel;
    int     m_nPropDelay;
    int     m_nSJWCurr;
    BYTE    m_bOption;
    UINT    m_nNoHardware;
    INT     m_nDataConfirmStatus;

    /**
     * To populate list box with possible vlaues for the given baudrate
     * and to set the focus to the selected item
     */
    void    vValidateBaudRate();

    /**
     * To save user given values in to the backend data
     */
    void vUpdateControllerDetails();

    /**
     * To set the backend data to UI
     */
    void vFillControllerConfigDetails();

    BYTE bGetNBT(double fBaudRate);
    int nGetValueFromComboBox(CComboBox& omComboBox);
    CString omGetFormattedRegVal(UCHAR ucRegVal);
    int GetSelectedEntryIndex(void);
    BOOL bUpdateControllerDataMembers(void);
    void vEnableFDParameters(BOOL bEnable);
};
