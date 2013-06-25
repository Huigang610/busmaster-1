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
 * @brief     Interface file for CMsgFilterConfigPage class
 * @author    Raja N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CMsgFilterConfigPage class
 */

#pragma once

class CMsgFilterConfigPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CMsgFilterConfigPage)
    DECLARE_MESSAGE_MAP()

public:
    // Standard constructor
    CMsgFilterConfigPage();
    // Construction with filter details
    CMsgFilterConfigPage(const SFILTERAPPLIED_CAN* psFilterConfigured,
                         HWND hMsgWnd);
    // Standard destructor
    ~CMsgFilterConfigPage();

    // Dialog Data
    enum { IDD = IDD_DLG_MSG_DISPLAY_FILTER };
    CListCtrl   m_omLstcFilterList;

    virtual void OnOK();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnBtnConfigure();

private:
    // To create image list
    BOOL bCreateImageList();
    // To create filter list UI
    VOID vCreateFilterUIList();
    // To init filter list UI
    VOID vInitFilterUIList();
    // To craete filter data structures
    VOID vInitFilterDataList();
    // To update the filter list with UI changes
    VOID vUpdateDataFromUI();

    // Image list for the filter items
    CImageList m_omImageList;
    HWND m_hMsgWnd; //Msg window handle
    const SFILTERAPPLIED_CAN* m_psFilterConfigured;
    SFILTERAPPLIED_CAN m_sFilterAppliedCan;
};
