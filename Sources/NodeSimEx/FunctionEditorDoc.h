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

#include "NodeSimEx_Struct.h"
#include "FunctionEditorStruct.h"

class CFunctionEditorDoc : public CDocument
{
    DECLARE_DYNCREATE(CFunctionEditorDoc)

public:
    CFunctionEditorDoc();
    CStringList m_omSourceCodeTextList;

    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void OnCloseDocument();

    // Function to get initialised string for database messages
    CString omStrGetInitialisedMessage( UINT unMsgID,
                                        const CString& omStrMsgName,
                                        const CString& omStrVarName,
                                        BOOL bInitData,
                                        UCHAR ucChannelId);
    CStringArray* pomStrGetErrorHandlerPrototypes();
    CStringArray* pomStrGetDLLHandlerPrototypes();
    CStringArray* pomStrGetBusEventHandlerPrototypes();
    CStringArray* omStrGetEventIndPrototypes();
    CStringArray* omStrGetIncudeHeaderFiles();
    int nGetMaxLineLength();
    CString omStrExtractFunctionNameFromPrototype ( CString omStrPrototype);
    CStringArray* omStrGetFunctionNamesForDEF();
    CStringArray* omStrGetGlobalVariablePrototypes();
    CStringArray* omStrGetUtilityFunctionPrototypes();
    CStringArray* omStrGetKeyHandlerPrototypes();
    CStringArray* omStrGetTimerHandlerPrototypes();
    CStringArray* omStrGetMessageHandlerPrototypes();
    int ReadString(const LPSTR lpBuffer, UINT nSize, LPSTR lpsz, UINT& nIndex);
    long m_lCurrentWarningLineNum;
    POSITION SetPosToFirstLine();
    int nGetDocumentSize();
    const char* pcGetLine(POSITION& rPosition);
    DWORD dwGetLineCount();
    BOOL bInitBusSpecificInfo(SBUS_SPECIFIC_INFO& sBusSpecInfo);
    BOOL bGetBusSpecificInfo(SBUS_SPECIFIC_INFO& sBusSpecInfo);
    BOOL bAddFunctionPrototype(CString omStrFunton, BOOL bGCCExport);
    BOOL bAddGCCExportPrototype(CString& omStrFunction);
    virtual ~CFunctionEditorDoc();
    BOOL bCreateNewDocument(CString& omPath);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

private:
    CStringArray m_omErrorHandlerArray;
    CStringArray m_omDLLHandlerArray;
    CStringArray m_omBusEventsHandlerArray;
    CStringArray m_omEventIndArray;
    int m_nMaxLineLength;
    void vUpdateDEFFile(CString omStrFileName);
    DWORD m_dwSourceCodeLineNo;
    CStringArray m_omMessageHandlerArray;
    CStringArray m_omTimerHandlerArray;
    CStringArray m_omKeyHandlerArray ;
    CStringArray m_omUtilityFuncArray ;
    CStringArray m_omGlobalVariableArray ;
    CStringArray m_omFunctionNameArray ;
    CStringArray m_omIncludeFileArray ;
    SBUS_SPECIFIC_INFO m_sBusSpecInfo;

    // To get the application version
    CString GetApplicationVersion();
    void vInitialiseBusSpecStructure(CString& omStrTemp, UCHAR unChannel,
                                     SMSG_NAME_CODE& sMsgNameCode, BOOL bInitData);
};
