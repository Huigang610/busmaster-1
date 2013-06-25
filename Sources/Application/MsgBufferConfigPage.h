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
 * @brief     This file contain the definition of CMsgBufferConfigPage
 * @author    Raja N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CMsgBufferConfigPage
 */

#pragma once

class CMsgBufferConfigPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CMsgBufferConfigPage)
    DECLARE_MESSAGE_MAP()

public:
    CMsgBufferConfigPage();
    void vSetBufferSize(INT* pnBufferSize);
    virtual ~CMsgBufferConfigPage();

    enum { IDD = IDD_DLG_MSG_BUFFER_CONGIF };

    virtual void OnOK();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();
    afx_msg void OnCbtnSetDefault();

private:
    UINT    m_unAppendSize;
    UINT    m_unOverWriteSize;
    UINT    m_unDisplayUpdateRate;
    INT*    m_pnBufferSize;
};
