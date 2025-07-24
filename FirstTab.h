#pragma once

#include "Tabs.h"

#include "CListCtrl2.h"
#include "HopfieldNetwork.h"


// CFirstTab dialog
class CFirstTab : public CTabTemplate, protected CNetworkUpdateCallback<double>
{
private:

    class CArrayPicture* m_Picture;

    CButton              m_LearnButton;
    CButton              m_ResetButton;
    CButton              m_RestoreButton;
    CListCtrlMy          m_PictureList;

    HANDLE               m_hThread;

    CHopfieldNetwork<double>  *m_pNetwork;

    BOOL                 m_bInPicture;

    CArray<CListCtrlMy::DRAW_CONTEXT *>    m_BitmapTable;


    BOOL   LoadTrainingData(const CString &path);

    BOOL   LoadImageFile(const CString &filename);


    // Construction
public:
    CFirstTab(); // standard constructor
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);


    // Implementation
protected:

    static DWORD  WINAPI RecoveryThread(CFirstTab* lpThis) {
        return lpThis->RecoveryThreadRoutine();
    }

    DWORD  RecoveryThreadRoutine();

    virtual void NetworkUpdateCallback(const CArray<double>& output);

    void OnSelectPicture(CListCtrlMy::DRAW_CONTEXT* pContext);
    void OnLearnButton();
    void OnRecoveryButton();

    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    // Generated message map functions
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};
