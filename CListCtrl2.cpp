/***************************************************************************************\
*   File:                                                                               *
*       CListCtrl2.cpp                                                                  *
*                                                                                       *
*   Abstract:                                                                           *
*       Component for drawing graphs                                                    *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Dec-2022                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#include "pch.h"
#include "Neuronets.h"
#include "CListCtrl2.h"


BEGIN_MESSAGE_MAP(CListCtrlMy, CListCtrl)
    ON_WM_MEASUREITEM_REFLECT()
    ON_WM_DRAWITEM_REFLECT()
    ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL
CListCtrlMy::Create(_In_ DWORD dwStyle, _In_ const RECT& rect, _In_ CWnd* pParentWnd, _In_ UINT nID)
{

    if (LoadResource() != TRUE)
        return FALSE;

    dwStyle |= LVS_OWNERDRAWFIXED;

    BOOL _return = __super::Create(dwStyle, rect, pParentWnd, nID);

    if (_return == 0)
    {
        m_pPicture->Release();
        m_pPicture = NULL;
        return FALSE;
    }


    m_BigFont.CreateFontW(
        40,                       // nHeight
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

    LVCOLUMN  column;

    column.mask = LVCF_WIDTH | LVCF_FMT;
    column.fmt = LVCFMT_LEFT;
    column.cx = 66;


    this->InsertColumn(0, &column);

    return TRUE;
}

CListCtrlMy::~CListCtrlMy()
{
    CListCtrl::~CListCtrl();

    if (m_pPicture != NULL)
        m_pPicture->Release();
}

BOOL
CListCtrlMy::LoadResource()
{
    BOOL      _return = FALSE;
    HRSRC     hDIBRes;
    HGLOBAL   hDIBMem;
    HGLOBAL   hDIBCopy;
    DWORD     nBmpImageSize = 0;
    IStream* pStream = NULL;

    if ((hDIBRes = ::FindResource(theApp.m_hInstance, MAKEINTRESOURCE(IDR_TEACHED), _T("RT_GIF"))) > NULL)
    {

        if ((hDIBMem = ::LoadResource(theApp.m_hInstance, hDIBRes)) > 0)
        {
            if (::LockResource(hDIBMem))
            {

                nBmpImageSize = ::SizeofResource(theApp.m_hInstance, hDIBRes);

                hDIBCopy = ::GlobalAlloc(GMEM_MOVEABLE, nBmpImageSize);

                if (hDIBCopy != NULL)
                {

                    PVOID hgInMem = ::GlobalLock(hDIBCopy);

                    memcpy(hgInMem, hDIBMem, nBmpImageSize);

                    if (nBmpImageSize > 0 && ::CreateStreamOnHGlobal(hDIBCopy, TRUE, &pStream) == S_OK)
                    {

                        HRESULT result = ::OleLoadPicture(
                            pStream,
                            nBmpImageSize,
                            FALSE,
                            IID_IPicture,
                            (LPVOID*)&this->m_pPicture);

                        if (result == S_OK)
                        {

                            _return = TRUE;
                        }

                        pStream->Release();
                    }

                    ::GlobalUnlock(hDIBCopy);

                    ::GlobalFree(hDIBCopy);
                }


                ::FreeResource(hDIBMem);
            }
        }
    }

    return _return;
}

void CListCtrlMy::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

    lpMeasureItemStruct->itemHeight = 66; // BitmapData.bmHeight + 2;
    lpMeasureItemStruct->itemWidth = 66;  // BitmapData.bmWidth + 2;
}


void
CListCtrlMy::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    DRAW_CONTEXT *drawContext = (DRAW_CONTEXT *)lpDrawItemStruct->itemData;

    BITMAP  BitmapData;

    memset(&BitmapData, 0, sizeof(BitmapData));

    if (drawContext->bitmap == NULL ||
        drawContext->bitmap->GetBitmap(&BitmapData) == 0) {

        return;
    }

    BITMAPINFO  bitmapInfo;

    memset(&bitmapInfo, 0, sizeof(bitmapInfo));

    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    GetDIBits(lpDrawItemStruct->hDC, (HBITMAP)drawContext->bitmap->GetSafeHandle(), 0, 0, NULL, &bitmapInfo, DIB_RGB_COLORS);

    CDC* pDC;
    pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

    CRect rect = lpDrawItemStruct->rcItem;

    if (lpDrawItemStruct->itemState & ODS_SELECTED)
    {
        pDC->FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT));
    }
    else
    {
        pDC->FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_WINDOW));
    }

    rect.DeflateRect(1, 1);

    CDC dcMemory;
    dcMemory.CreateCompatibleDC(pDC);

    dcMemory.SetStretchBltMode(HALFTONE);
    dcMemory.SelectObject(drawContext->bitmap);


    /*SetDIBitsToDevice(
        dcNew,                  // handle to DC
        rect.left,              // x-coord of destination upper-left corner
        rect.top,               // y-coord of destination upper-left corner 
        rect.Width(),           // source rectangle width
        rect.Height(),          // source rectangle height
        0,                      // x-coord of source lower-left corner
        0,                      // y-coord of source lower-left corner
        0,                      // first scan line in array
        BitmapData.bmHeight,         // number of scan lines
        ((DWORD*)BitmapData.bmBits),// array of DIB bits
        &bitmapInfo,            // bitmap information
        DIB_RGB_COLORS);*/        // RGB or palette indexes


    StretchBlt(
        lpDrawItemStruct->hDC,
        rect.left, rect.top,
        rect.Width(), rect.Height(),
        dcMemory.GetSafeHdc(), 
        0, 0, 
        BitmapData.bmWidth, BitmapData.bmHeight,
        SRCCOPY);

    if (drawContext->Type == OkType)
    {
        if (drawContext->Learned == TRUE)
        {

            long hmHeight;
            long hmWidth;
            this->m_pPicture->get_Height(&hmHeight);
            this->m_pPicture->get_Width(&hmWidth);

            m_pPicture->Render(
                lpDrawItemStruct->hDC,
                rect.left, rect.top,
                rect.Width(), rect.Height(),
                0, hmHeight,
                hmWidth, -hmHeight,
                NULL);

        }
    }
    else
    if (drawContext->Type == NumberType)
    {
        if (drawContext->Learned == TRUE)
        {

            HGDIOBJ oldFont = pDC->SelectObject(m_BigFont.GetSafeHandle());

            CBrush  brush;

            TCHAR   string[10];
            string[0] = 0;

            _itot(drawContext->Number, string, 10);
            SIZE_T len = _tcslen(string);

            pDC->SetTextColor(RGB(0x00, 0xFF, 0x00));

            pDC->TextOut(
                rect.left + rect.Width() / 2,
                rect.top + rect.Height() / 2,
                string, (int)len);

            pDC->SelectObject(oldFont);
        }

    }
    else
    {
        //assert(false);
    }

}
