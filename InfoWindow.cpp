/***************************************************************************************\
*   File:                                                                               *
*       InfoWindow.cpp                                                                  *
*                                                                                       *
*   Abstract:                                                                           *
*       Informational window                                                            *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 18-Apr-2023                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#include "pch.h"
#include "InfoWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  CHART_BACKGROUND_COLOR     RGB(0x20, 0x20, 0x20)
#define  CHART_AXIS_COLOR           RGB(0xFF, 0xFF, 0x20)
#define  CHART_GRID_COLOR           RGB(0x50, 0x50, 0x50)
#define  CHART_NAMES_COLOR          RGB(0x50, 0xFF, 0x50)
#define  CHART_GRAPH_COLOR          RGB(0x20, 0x20, 0xFF)
#define  CHART_CROSSPOINT_OFFSET    16
#define  CHART_ARROW_LENGTH         12


//---------------------------------------------------------------------------
//
// CHART CONTROL IMPLEMENTAION
//
//---------------------------------------------------------------------------

// CChartControlApp
BEGIN_MESSAGE_MAP(CInfoWindow, CWnd)
    //{{AFX_MSG_MAP(CInfoWindow)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CGraphControl construction

CInfoWindow::CInfoWindow()
    : CWnd()
{
    m_alerted = false;
    m_Number = -1;
}

CInfoWindow::~CInfoWindow()
{

}

void
CInfoWindow::OnDestroy()
{
    CWnd::OnDestroy();

}

BOOL
CInfoWindow::Create(
    DWORD dwStyle, const CRect& rect, CWnd* pParentWnd)
{


    if (CWnd::Create(NULL, NULL, dwStyle | WS_CHILD, rect, pParentWnd, NULL, NULL) != TRUE)
    {
        return FALSE;
    }

    m_BigFont.CreateFontW(
        25,                       // nHeight
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
        _T("Arial"));            // lpszFacename


    m_MedFont.CreateFontW(
        10,                       // nHeight
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
        _T("Arial"));            // lpszFacename

    return TRUE;
}


void
CInfoWindow::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

}



void
CInfoWindow::OnPaint()
/*
    WM_PAINT dispatcher
*/
{
    CWnd::OnPaint();

    CRect clRect;
    this->GetClientRect(&clRect);

    int  width = clRect.Width();
    int  height = clRect.Height();

    if ((width < 10) || (height < 10))
    {
        return;
    }

    CClientDC dc(this);
    HGDIOBJ oldFont;

    if (m_alerted)
    {
        dc.FillSolidRect(clRect, RGB(0xFF, 0x55, 0x55));
    }
    else
    {
        dc.FillSolidRect(clRect, RGB(0x55, 0x55, 0x55));
    }

    /*if (m_Number < 0)
    {
        oldFont = dc.SelectObject(m_MedFont.GetSafeHandle());

        dc.SetTextColor(RGB(0x00, 0xFF, 0x00));

        CString   string = TEXT("Некласи");

        dc.TextOut(
            clRect.left + 5,
            clRect.top + 10,
            string);

        string = TEXT("фіковано");

        dc.TextOut(
            clRect.left + 5,
            clRect.top + 30,
            string);

    }
    else
    {*/
        oldFont = dc.SelectObject(m_BigFont.GetSafeHandle());

        CString   string;

        string.Format(L"%2.1f%%", m_Number);

        dc.SetTextColor(RGB(0x00, 0xFF, 0x00));

        dc.TextOut(
            clRect.left + 10,
            clRect.top + 10,
            string);

    //}

    dc.SelectObject(oldFont);

}
