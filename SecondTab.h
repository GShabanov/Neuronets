#pragma once

#include "Tabs.h"

#include "CListCtrl2.h"
#include "InfoWindow.h"
#include "HammingNetwork.h"


// CSecondTab dialog
class CSecondTab : 
    public CTabTemplate, 
    protected CNetworkUpdateCallback<double>,
    public CMFCNotify
{
private:

    typedef struct _CLASSTYPE_PAIR {
        CButton         checkBox;
        CInfoWindow     m_InfoWindow;
    } CLASSTYPE_PAIR;

    class CArrayPicture* m_Picture;

    CStatic              m_classesStatic;
    CButton              m_Clear;
    CButton              m_DoButton;
    CButton              m_ResetButton;
    CListCtrlMy          m_PictureList;
    //CInfoWindow          m_InfoWindow;

    CArray<CLASSTYPE_PAIR>  m_classes;

    CHammingNetwork<double>  *m_pNetwork;

    ULONG                m_Learned;

    BOOL                 m_bInPicture;

    ULONG                m_granularity;
    ULONG                m_sizeX;
    ULONG                m_sizeY;

    CArray<CListCtrlMy::DRAW_CONTEXT *>    m_BitmapTable;


    void   AdjustLayout();

    BOOL   LoadTrainingData(const CString &path);
    BOOL   LoadImageFile(const CString &filename);
    void   UpdateState();

    // Construction
public:
    CSecondTab(); // standard constructor
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);


    // Implementation
protected:

    virtual void NetworkUpdateCallback(const CArray<double>& output);

    void OnSelectPicture(CListCtrlMy::DRAW_CONTEXT* pContext);
    void OnLearnButton();
    void OnClearButton();

    virtual void ControlCallback(CWnd* from, void* data);

    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    // Generated message map functions
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};
