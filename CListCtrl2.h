#pragma once
#ifndef __CLISTCTRL2_H__
#define __CLISTCTRL2_H__



class CListCtrlMy : public CListCtrl
{
public:
    enum DRAW_ITEM_TYPE {
        OkType = 0,
        NumberType = 1,
    };

    typedef struct _DRAW_CONTEXT {

        DRAW_ITEM_TYPE  Type;

        CBitmap* bitmap;
        BOOL     Learned;
        ULONG    Number;

    } DRAW_CONTEXT, * PDRAW_CONTEXT;

private:

    IPicture* m_pPicture;

    CFont     m_BigFont;

public:

    CListCtrlMy()
        : CListCtrl()
        , m_pPicture(NULL)
    {
    }

    virtual ~CListCtrlMy();

    BOOL Create(_In_ DWORD dwStyle, _In_ const RECT& rect, _In_ CWnd* pParentWnd, _In_ UINT nID);

    BOOL LoadResource();


protected:

    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    DECLARE_MESSAGE_MAP()

};

#endif // __CLISTCTRL2_H__
