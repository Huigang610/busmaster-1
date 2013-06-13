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
 * @file      WaitIndicator.h
 * @brief     The File contain the definition of WaitIndicator
 * @author    Venkatanarayana Makam
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * The File contain the definition of WaitIndicator
 */

#pragma once

// Window class name
#define INDICATOR_CLASSNAME    "IndicatorCtrl"

class CWaitIndicator : public CWnd
{
    DECLARE_DYNAMIC(CWaitIndicator)
    DECLARE_MESSAGE_MAP()

public:
    CWaitIndicator();
    virtual ~CWaitIndicator();

    //To Activater and to Show the Window
    BOOL DisplayWindow(CString strTitle, CWnd* pParentWnd, UINT nID = 0);
    void SetWindowText(char* acText);
    void PaintWindow(CDC& dc);

    //To Close the window
    void CloseWindow();

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint(void);

private:
    CString m_strTitle;
    CFont* m_TitleFont;

	//To Move the Window to the center of Parent Window
    void MoveToCenter(void);
    //To Register the custom window name
    BOOL RegisterWindowClass(void);
    void Redraw();
};
