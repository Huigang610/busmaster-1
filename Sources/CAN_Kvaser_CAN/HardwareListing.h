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
 * @brief     Interface file for CHardwareListing class
 * @author    RaTnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CHardwareListing class
 */

#pragma once

/* C++ includes */
#include <map>

/* Project includes */
#include "CAN_Kvaser_CAN_resource.h"    // For resource symbols
#include "DataTypes/DIL_Datatypes.h"    // For DIL datatypes
#include "Include/CanUsbDefs.h"

class CHardwareListing : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    // Array of channels
    int m_anSelectedChannels[ CHANNEL_ALLOWED ];
    // Array to hold driver handle
    //SHWNETLIST m_sHwNetList[ MAX_HCANNET ];
    // To update selected hardware detaisl
    void vUpdateHwDetails( int nIndex );
    // To set List of hardware handles
    void vSetHardwareList(InterfaceHardware*, int );
    // Constructor
    CHardwareListing(InterfaceHardware*, int , int*, CWnd* pParent = NULL);
    // standard constructor
    CHardwareListing();
    //Get selection list
    INT nGetSelectedList(int* pnList);
    //Set selection list
    void vSetSelectedList();

    // Dialog Data
    enum { IDD = IDD_DLG_CHANNEL_SELECTION };
    CListCtrl   m_omSelectedHwList;
    CEdit   m_omNetName;
    CEdit   m_omFirmware;
    CEdit   m_omDriverID;
    CListCtrl   m_omHardwareList;

    // Hardware CONTAINER
    typedef struct tagHardwareContainer
    {
        int                 m_omDriverId;
        CString             m_omHardwareName;
        CString             m_omFirmware;
        CString             hardwareDescription;

    } HARDWARE_CONTAINER, *PHARDWARE_CONTAINER;

    typedef std::pair<int, PHARDWARE_CONTAINER> Int_Pair;
    std::map<int, PHARDWARE_CONTAINER>::iterator m_pIter;
    std::map<int, PHARDWARE_CONTAINER> mHardwareListMap;

    //Hardware container object
    PHARDWARE_CONTAINER m_pouHardwareContainer;
    void vSortHardwareItems();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnItemchangedHWList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonSelect();
    afx_msg void OnButtonRemove();
    afx_msg void OnItemchangedLstcSelectedHwList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMClickLstcHwList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMClickLstcSelectedHwList(NMHDR* pNMHDR, LRESULT* pResult);

private:
    void vEnableDisableButtons();
    // Pointer to Hardware List
    InterfaceHardware* m_psHwInterface;
    // Size of the array
    int m_nSize;
    //Number of selected items
    int m_nNoOfHwSelected;
    // Image for CListCtrl
    CImageList m_omImageList;
    // Selected Item index
    int m_nSelectedItem;
    // Pointer to Selected List
    int* m_pnSelList;
};
