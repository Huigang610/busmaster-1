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
 * @file      CGCtrl.h
 * @brief     Interface file for CCGCtrl class
 * @author    Raja N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CCGCtrl class
 */

#pragma once

class CCGCtrl : public CWnd
{
protected:
    DECLARE_DYNCREATE(CCGCtrl)
public:
    CLSID const& GetClsid()
    {
        static CLSID const clsid
            = { 0xc9fe01c2, 0x2746, 0x479b, { 0x96, 0xab, 0xe0, 0xbe, 0x99, 0x31, 0xb0, 0x18 } };
        return clsid;
    }
    virtual BOOL Create(LPCTSTR /*lpszClassName*/,
                        LPCTSTR /*lpszWindowName*/, DWORD /*dwStyle*/,
                        const RECT& /*rect*/,
                        CWnd* /*pParentWnd*/, UINT /*nID*/,
                        CCreateContext* /*pContext = NULL*/)
    {
        return NULL;/*CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);*/
    }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
                const RECT& rect, CWnd* pParentWnd, UINT nID,
                CFile* pPersist = NULL, BOOL bStorage = FALSE,
                BSTR bstrLicKey = NULL)
    {
        return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
                             pPersist, bStorage, bstrLicKey);
    }

    // Attributes
public:
    short GetAppearance();
    void SetAppearance(short);
    CString GetCaption();
    void SetCaption(LPCTSTR);
    BOOL GetEnabled();
    void SetEnabled(BOOL);
    unsigned long GetAxisColor();
    void SetAxisColor(unsigned long);
    unsigned long GetGridColor();
    void SetGridColor(unsigned long);
    unsigned long GetLabelColor();
    void SetLabelColor(unsigned long);
    unsigned long GetCursorColor();
    void SetCursorColor(unsigned long);
    LPDISPATCH GetLabelFont();
    void SetLabelFont(LPDISPATCH);
    LPDISPATCH GetTickFont();
    void SetTickFont(LPDISPATCH);
    LPDISPATCH GetTitleFont();
    void SetTitleFont(LPDISPATCH);
    LPDISPATCH GetIdentFont();
    void SetIdentFont(LPDISPATCH);
    short GetXGridNumber();
    void SetXGridNumber(short);
    short GetYGridNumber();
    void SetYGridNumber(short);
    BOOL GetShowGrid();
    void SetShowGrid(BOOL);
    CString GetXLabel();
    void SetXLabel(LPCTSTR);
    CString GetYLabel();
    void SetYLabel(LPCTSTR);
    unsigned long GetElementLineColor();
    void SetElementLineColor(unsigned long);
    unsigned long GetElementPointColor();
    void SetElementPointColor(unsigned long);
    long GetElementLinetype();
    void SetElementLinetype(long);
    long GetElementWidth();
    void SetElementWidth(long);
    long GetElementPointSymbol();
    void SetElementPointSymbol(long);
    BOOL GetElementSolidPoint();
    void SetElementSolidPoint(BOOL);
    BOOL GetElementShow();
    void SetElementShow(BOOL);
    long GetTrackMode();
    void SetTrackMode(long);
    CString GetElementName();
    void SetElementName(LPCTSTR);
    BOOL GetElementIdentify();
    void SetElementIdentify(BOOL);
    BOOL GetXLog();
    void SetXLog(BOOL);
    BOOL GetYLog();
    void SetYLog(BOOL);
    LPDISPATCH GetControlFramePicture();
    void SetControlFramePicture(LPDISPATCH);
    LPDISPATCH GetPlotAreaPicture();
    void SetPlotAreaPicture(LPDISPATCH);
    unsigned long GetControlFrameColor();
    void SetControlFrameColor(unsigned long);
    unsigned long GetPlotAreaColor();
    void SetPlotAreaColor(unsigned long);
    long GetFrameStyle();
    void SetFrameStyle(long);
    CString GetAnnoLabelCaption();
    void SetAnnoLabelCaption(LPCTSTR);
    double GetAnnoLabelX();
    void SetAnnoLabelX(double);
    double GetAnnoLabelY();
    void SetAnnoLabelY(double);
    unsigned long GetAnnoLabelColor();
    void SetAnnoLabelColor(unsigned long);
    BOOL GetAnnoLabelHorizontal();
    void SetAnnoLabelHorizontal(BOOL);
    short GetAnnotation();
    void SetAnnotation(short);
    BOOL GetAnnoVisible();
    void SetAnnoVisible(BOOL);
    short GetAnnoCount();
    void SetAnnoCount(short);
    short GetElement();
    void SetElement(short);
    short GetElementCount();
    void SetElementCount(short);
    unsigned long GetAnnoLabelBkColor();
    void SetAnnoLabelBkColor(unsigned long);
    short GetCursorCount();
    void SetCursorCount(short);
    short GetCursor();
    void SetCursor(short);
    double GetCursorX();
    void SetCursorX(double);
    double GetCursorY();
    void SetCursorY(double);
    short GetCursorStyle();
    void SetCursorStyle(short);
    BOOL GetCursorVisible();
    void SetCursorVisible(BOOL);
    short GetCursorMode();
    void SetCursorMode(short);
    CString GetFormatAxisBottom();
    void SetFormatAxisBottom(LPCTSTR);
    CString GetFormatAxisLeft();
    void SetFormatAxisLeft(LPCTSTR);
    BOOL GetYTime();
    void SetYTime(BOOL);
    BOOL GetXTime();
    void SetXTime(BOOL);
    short GetLineDisplay();
    void  SetLineDisplay(short);

    // Operations
public:
    void SetRange(double xmin, double xmax, double ymin, double ymax);
    void AutoRange();
    void CopyToClipboard();
    void PrintGraph();
    void AddElement(long index);
    void DeleteElement(short ElementID);
    void ClearGraph();
    double GetElementXValue(long index, short ElementID);
    void SetElementXValue(long index, short ElementID, double newValue);
    double GetElementYValue(long index, short ElementID);
    void SetElementYValue(long index, short ElementID, double newValue);
    void PlotXY(double X, double Y, short ElementID);
    void PlotY(double Y, short ElementID);
    void ShowProperties();
    void SaveAs(LPCTSTR szFilename);
    void AddAnnotation();
    void DeleteAnnotation(short annoID);
    void AddCursor();
    void DeleteCursor(short cursorID);
    void UpdateDisplay();
    void GoToRunMode();
    void GoToNormalMode();
    short GetZoomMode();
    void SetZoomMode(short nNewValue);
    void SetXAxisRange(double xmin, double xmax);
    void FitAll();
    short GetElementRange(short ElementID, double* xmin, double* xmax, double* ymin, double* ymax);
    void GenerateCSVReport(LPCTSTR szFilename);
    short GenerateHTMLReport(LPCTSTR szFilename, LPUNKNOWN* pParams);
    void ShiftDisplay(short xShift, short yShift);
    void SetGridLines(short xGrids, short yGrids);
    void SetElementUnit(LPCTSTR lpszUnit);
    void GetXAxisRange(double FAR* pXMin, double FAR* pXMax);
    void GetElementValueAtCursor(short ElementID, double dblCursorVal, double* dblElemVal);
    void AboutBox();
};
