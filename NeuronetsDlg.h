
// NeuronetsDlg.h : header file
//

#pragma once


// CNeuronetsDlg dialog
class CNeuronetsDlg : public CDialogEx
{
// Construction
public:
    CNeuronetsDlg(CWnd* pParent = nullptr);  // standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_LAB4_DIALOG };
#endif

private:

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    CTabCtrl     m_selection;

    class CTabTemplate*     m_CurrentTab;

    //
    // solver class
    //

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()
};
