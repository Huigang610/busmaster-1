#pragma once


// CBusEventHandlerDlg dialog
#include "NodeSimEx_resource.h"
class CBusEventHandlerDlg : public CDialog
{
    DECLARE_DYNAMIC(CBusEventHandlerDlg)
    DECLARE_MESSAGE_MAP()

public:
    CBusEventHandlerDlg(CFunctionEditorDoc* pDoc, CWnd* pParent =NULL, BOOL bIsDelete = FALSE);   // standard constructor
    CStringArray m_omStrArrayBusEventHandler;
    virtual ~CBusEventHandlerDlg();

    // Dialog Data
    enum { IDD = IDD_DLG_BUSEV_HANDLER };

    BOOL m_bChkBusEventConnect;
    BOOL m_bChkBusEventDisConnect;
    virtual BOOL OnInitDialog();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnBnClickedOk();

private:
    CFunctionEditorDoc* m_pDoc;
    BOOL m_bIsDelete;
};
