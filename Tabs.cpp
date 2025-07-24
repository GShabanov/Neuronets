
// Lab3Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include <afxdialogex.h>
#include "Tabs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTabTemplate dialog

CTabTemplate::CTabTemplate()
    : CWnd()
{

}


BEGIN_MESSAGE_MAP(CTabTemplate, CWnd)
END_MESSAGE_MAP()


// CTabTemplate message handlers
BOOL
CTabTemplate::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{

    if (CWnd::Create(NULL, NULL, dwStyle | WS_CHILD, rect, pParentWnd, NULL, NULL) != TRUE)
    {
        return FALSE;
    }

    SetOwner(pParentWnd);

    m_bigFont.CreateFont(
        24,                       // nHeight
        0,                        // nWidth
        0,                        // nEscapement
        0,                        // nOrientation
        FW_NORMAL,                // nWeight
        FALSE,                    // bItalic
        FALSE,                    // bUnderline
        0,                        // cStrikeOut
        ANSI_CHARSET,             // nCharSet
        OUT_DEFAULT_PRECIS,       // nOutPrecision
        CLIP_DEFAULT_PRECIS,      // nClipPrecision
        DEFAULT_QUALITY,          // nQuality
        DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
        _T("Arial"));

    m_smallFont.CreateFont(
        14,                       // nHeight
        0,                        // nWidth
        0,                        // nEscapement
        0,                        // nOrientation
        FW_SEMIBOLD,                // nWeight
        FALSE,                    // bItalic
        FALSE,                    // bUnderline
        0,                        // cStrikeOut
        ANSI_CHARSET,             // nCharSet
        OUT_DEFAULT_PRECIS,       // nOutPrecision
        CLIP_DEFAULT_PRECIS,      // nClipPrecision
        DEFAULT_QUALITY,          // nQuality
        DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
        _T("Arial"));


    return TRUE;  // return TRUE  unless you set the focus to a control

}
