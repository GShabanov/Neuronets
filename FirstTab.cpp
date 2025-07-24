
// CFirstTab.cpp : implementation file
//

#include "pch.h"
#include <afxdialogex.h>
#include "..\..\Shared\MFCPicture.h"
#include "FirstTab.h"
#include "resource.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFirstTab dialog

CFirstTab::CFirstTab()
    : CTabTemplate()
    , m_Picture(NULL)
    , m_bInPicture(FALSE)
    , m_hThread(NULL)
{
}


BEGIN_MESSAGE_MAP(CFirstTab, CTabTemplate)
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CFirstTab message handlers
BOOL
CFirstTab::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
    CRect  clRect;
    CRect  workRect;

    m_pNetwork = new CHopfieldNetwork<double>(64 * 64);

    if (NULL == m_pNetwork)
        return FALSE;

    if (!m_pNetwork->Init())
    {
        delete m_pNetwork;
        m_pNetwork = NULL;

        return FALSE;
    }


    if (__super::Create(dwStyle | WS_CHILD, rect, pParentWnd) != TRUE)
    {
        return FALSE;
    }

    this->GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    workRect = clRect;

    workRect.top += 70;
    workRect.right = workRect.left + 5 * 64 + 2;
    workRect.bottom = workRect.top + 5 * 64 + 2;

    m_Picture = new CArrayPicture();

    m_Picture->Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
        workRect, this);

    m_Picture->SetDrawCursor(LoadCursor((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1)));

    workRect.left = 5 * 64 + 2 + 10;
    workRect.right += 100;
    workRect.bottom = workRect.top + 30;

    m_LearnButton.Create(
        _T("Завчити"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_DISABLED, workRect, this, NULL);


    workRect.left = workRect.right + 5;
    workRect.right += 100;

    m_ResetButton.Create(
        _T("Скинути"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, workRect, this, NULL);

    workRect.left = workRect.right + 5;
    workRect.right += 100;

    m_RestoreButton.Create(
        _T("Відновити"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_DISABLED, workRect, this, NULL);

    workRect.top = workRect.bottom + 10;
    workRect.bottom = clRect.bottom - 10;
    workRect.left = 5 * 64 + 2 + 10;
    workRect.right = workRect.left + 110;

    m_PictureList.Create(
        WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_OWNERDRAWFIXED,
        workRect,
        this, NULL);



    LoadTrainingData(CString(_T(".\\data\\")));


    return TRUE;  // return TRUE  unless you set the focus to a control

}

BOOL
CFirstTab::LoadImageFile(const CString &filename)
{
    IPicture* pPicture;
    IStream*  pStream;
    HRESULT   hr;

    //
    // Load any file (jpg, bmp, etc.)
    //
    hr = SHCreateStreamOnFile(
        filename.operator LPCWSTR(),
        STGM_READ,
        &pStream);


    if (FAILED(hr))
        return FALSE;


    hr = ::OleLoadPicture(
        pStream,
        0,
        FALSE,
        IID_IPicture,
        (LPVOID*)&pPicture);


    pStream->Release();

    if (FAILED(hr))
    {
        return FALSE;
    }

    LONG hmWidth = 0;
    LONG hmHeight = 0;
    HBITMAP hsrc;

    pPicture->get_Width(&hmWidth);
    pPicture->get_Height(&hmHeight);

    hr = pPicture->get_Handle((OLE_HANDLE*)&hsrc);

    if (FAILED(hr))
    {
        pPicture->Release();
        return FALSE;
    }

    //
    // get bitmap for this file
    //
    HBITMAP hbmp = (HBITMAP)CopyImage(hsrc, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);


    if (hbmp == NULL)
    {
        pPicture->Release();
        return FALSE;
    }

    CBitmap   *bitmap = CBitmap::FromHandle(hbmp);

    CListCtrlMy::DRAW_CONTEXT  *drawContext = new CListCtrlMy::DRAW_CONTEXT;

    if (drawContext != NULL)
    {
        drawContext->Type = CListCtrlMy::DRAW_ITEM_TYPE::OkType;
        drawContext->bitmap = bitmap;
        drawContext->Learned = FALSE;

        m_BitmapTable.Add(drawContext);

        m_PictureList.InsertItem(LVIF_IMAGE | LVIF_STATE | LVIF_PARAM, (int)(m_BitmapTable.GetSize() - 1), _T(""), 0, 0, 0, (LPARAM)drawContext);
    }

    pPicture->Release();

    return TRUE;
}

BOOL
CFirstTab::LoadTrainingData(const CString &path)
{
    CString  searchPath;
    WIN32_FIND_DATA  findData;

    HANDLE           hFindHandle;

    searchPath = path;
    searchPath += _T("*.jpg");

    hFindHandle = FindFirstFile(
        searchPath,
        &findData);

    while (hFindHandle != NULL)
    {

        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
        {
            CString  fileName = path;
            fileName += findData.cFileName;


            LoadImageFile(fileName);


        }


        if (!FindNextFile(hFindHandle, &findData))
            break;
    }


    FindClose(hFindHandle);

    return TRUE;
}

void
CFirstTab::OnDestroy()
{
    HANDLE  hThread = (HANDLE)InterlockedExchangePointer(&m_hThread, NULL);

    if (hThread != NULL)
    {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }


    m_Picture->DestroyWindow();
    delete m_Picture;

    m_LearnButton.DestroyWindow();
    m_PictureList.DestroyWindow();

    while (m_BitmapTable.GetSize() > 0)
    {
        m_BitmapTable[0]->bitmap->DeleteObject();
        delete m_BitmapTable[0];

        m_BitmapTable.RemoveAt(0, 1);
    }

    delete m_pNetwork;
    m_pNetwork = NULL;

    __super::OnDestroy();
}

void
CFirstTab::OnPaint()
{
    PAINTSTRUCT     paintStruct = { 0, };
    CDC             *dc;


    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {

        CRect  clRect;

        HGDIOBJ oldFont = dc->SelectObject(m_bigFont);

        GetClientRect(clRect);

        clRect.top = 10;
        clRect.bottom = 70;
        clRect.left = clRect.left = clRect.Width() / 2 - (16 * sizeof("Нейронні мережі, Шабанов Г.В. ІКМ-A1323") / 3);
        clRect.right = clRect.left + 500;

        dc->DrawText(CString(_T("Нейронні мережі, Шабанов Г.В. ІКМ-A1323")), clRect, 0);

        dc->SelectObject(oldFont);

        this->EndPaint(&paintStruct);
    }
}


afx_msg void 
CFirstTab::OnMouseMove(UINT nFlags, CPoint point)
{
    //::ShowCursor(TRUE);
    /*RECT inwindow;

    m_Picture->GetClientRect(&inwindow);

    this->MapWindowPoints(m_Picture, &inwindow);
    //this->ClientToScreen(&point);

    if (PtInRect(&inwindow, point))
    {
        if (m_bInPicture == FALSE)
        {
            m_bInPicture = TRUE;

            ::ShowCursor(FALSE);
        }
    }
    else
    {

        if (m_bInPicture == TRUE)
        {
            ::ShowCursor(TRUE);
        }
        
        m_bInPicture = FALSE;
    }*/
    
}

void
CFirstTab::OnSelectPicture(CListCtrlMy::DRAW_CONTEXT* pContext)
{
    CBitmap* Bitmap = pContext->bitmap;

    BITMAP  bitmap;

    if (!Bitmap->GetBitmap(&bitmap))
        return;


    CArrayEx<CArrayEx<COLORREF>>  newArray;

    BYTE* dwBits = (BYTE*)bitmap.bmBits;

    DWORD  dwMask = (DWORD)(bitmap.bmBitsPixel == 32 ? -1 : (1 << bitmap.bmBitsPixel) - 1);

    for (LONG y = 0; y < bitmap.bmHeight; y++)
    {
        newArray.Add(CArrayEx<COLORREF>());

        for (LONG x = 0; x < bitmap.bmWidth; x++)
        {
            COLORREF  color = RGB(0x80, 0x20, 0x20);

            newArray[y].Add(color);
        }
    }

    for (LONG y = 0; y < bitmap.bmHeight; y++)
    {

        for (LONG x = 0; x < bitmap.bmWidth; x++)
        {
            BYTE Blue = *(dwBits + 0);
            BYTE Green = *(dwBits + 1);
            BYTE Red = *(dwBits + 2);

            COLORREF  color = RGB(Red, Green, Blue);

            newArray[bitmap.bmHeight - 1 - y][x] = color;

            dwBits = dwBits + (bitmap.bmBitsPixel >> 3);
        }


    }

    m_Picture->SetInputArray(newArray);

    if (pContext->Learned == TRUE)
    {
        m_LearnButton.EnableWindow(FALSE);
    }
    else
    {
        m_LearnButton.EnableWindow(TRUE);
    }

}

BOOL
CFirstTab::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    NMHDR* header = (NMHDR*)lParam;

    if (header->hwndFrom == m_PictureList.m_hWnd &&
        header->code == LVN_ITEMCHANGING)
    {

        NMLISTVIEW * pNMListView = (LPNMLISTVIEW)lParam;

        if (pNMListView->uNewState & LVIS_SELECTED)
        {
            pNMListView = pNMListView;

            OnSelectPicture((CListCtrlMy::DRAW_CONTEXT*)pNMListView->lParam);

        }

    }

    return __super::OnNotify(wParam, lParam, pResult);
}

void
CFirstTab::OnLearnButton()
{
    POSITION pos = m_PictureList.GetFirstSelectedItemPosition();

    if (pos == NULL)
    {
        return;
    }

    int nItem = m_PictureList.GetNextSelectedItem(pos);

    CListCtrlMy::DRAW_CONTEXT* m_pItemData = (CListCtrlMy::DRAW_CONTEXT*)m_PictureList.GetItemData(nItem);

    m_pItemData->Learned = TRUE;

    m_LearnButton.EnableWindow(FALSE);
    m_PictureList.Invalidate(TRUE);

    CArrayEx<COLORREF> curPicture = m_Picture->Serialize();


    CArrayEx<double>  learnArray;


    for (INT_PTR i = 0; i < curPicture.GetCount(); i++)
    {
        COLORREF  color = curPicture[i];

        ULONG  grayScale = 
            (GetRValue(color) +
            GetGValue(color) +
            GetBValue(color)) / 3;


        if (grayScale > 0x20)
        {
            learnArray.Add(1.0);
        }
        else
        {
            learnArray.Add(-1.0);
        }

    }

    m_pNetwork->Teach(learnArray);


    m_RestoreButton.EnableWindow(TRUE);

}

void
CFirstTab::NetworkUpdateCallback(const CArray<double>& output)
{

    CArrayEx<CArrayEx<COLORREF>>  newArray;

    for (LONG y = 0; y < m_Picture->GetHeight(); y++)
    {
        newArray.Add(CArrayEx<COLORREF>());

        for (LONG x = 0; x < m_Picture->GetWidth(); x++)
        {
            COLORREF  color = output[y * m_Picture->GetWidth() + x] > 0 ? RGB(0xFF, 0xFF, 0xFF) : RGB(0x00, 0x00, 0x00);

            newArray[y].Add(color);
        }
    }


    m_Picture->SetInputArray(newArray);

    Sleep(100);
}

DWORD
CFirstTab::RecoveryThreadRoutine()
{
    CArrayEx<COLORREF> curPicture = m_Picture->Serialize();

    m_Picture->Clear();


    CArrayEx<double>  learnArray;


    for (INT_PTR i = 0; i < curPicture.GetCount(); i++)
    {
        COLORREF  color = curPicture[i];

        ULONG  grayScale =
            (GetRValue(color) +
                GetGValue(color) +
                GetBValue(color)) / 3;


        if (grayScale > 0x50)
        {
            learnArray.Add(1.0);  // більше до білого +1
        }
        else
        {
            learnArray.Add(-1.0);  // чорний -1
        }

    }

    CArrayEx<double>  outputArray;


    m_pNetwork->Recovery(learnArray, outputArray, this);



    CArrayEx<CArrayEx<COLORREF>>  newArray;

    for (LONG y = 0; y < m_Picture->GetHeight(); y++)
    {
        newArray.Add(CArrayEx<COLORREF>());

        for (LONG x = 0; x < m_Picture->GetWidth(); x++)
        {
            //
            // де  > 0  це білий, а  < 0 це чорний
            //
            COLORREF  color = outputArray[y * m_Picture->GetWidth() + x] > 0 ? RGB(0xFF, 0xFF, 0xFF) : RGB(0x00, 0x00, 0x00);

            newArray[y].Add(color);
        }
    }


    m_Picture->SetInputArray(newArray);

    m_RestoreButton.EnableWindow(TRUE);

    HANDLE  hThread = (HANDLE)InterlockedExchangePointer(&m_hThread, NULL);

    if (hThread != NULL)
        CloseHandle(hThread);


    return 0;
}

void
CFirstTab::OnRecoveryButton()
{


    HANDLE  hThread = CreateThread(
        NULL,
        0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(&CFirstTab::RecoveryThread),
        this,
        NULL,
        NULL);

    if (hThread != NULL)
    {

        m_RestoreButton.EnableWindow(FALSE);

        m_hThread = hThread;
    }
}

BOOL
CFirstTab::OnCommand(
    WPARAM wParam,
    LPARAM lParam
    )
{
    if ((HWND)lParam == m_PictureList.m_hWnd)
    {
        m_PictureList.m_hWnd = m_PictureList.m_hWnd;
    }

    if ((HWND)lParam == m_LearnButton.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {

        OnLearnButton();
    }

    if ((HWND)lParam == m_RestoreButton.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {

        OnRecoveryButton();
    }

    if ((HWND)lParam == m_ResetButton.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {
        m_pNetwork->Reset();

        for (INT_PTR i = 0, j = m_BitmapTable.GetSize(); i < j; i++)
        {
            m_BitmapTable[i]->Learned = FALSE;

            if (m_PictureList.GetFirstSelectedItemPosition() != NULL)
            {
                m_LearnButton.EnableWindow(TRUE);
            }
            else
            {
                m_LearnButton.EnableWindow(FALSE);
            }

            m_PictureList.Invalidate(TRUE);

        }


        m_RestoreButton.EnableWindow(FALSE);
    }

    return __super::OnCommand(wParam, lParam);


}


void
CFirstTab::OnSize(UINT nType, int cx, int cy)
{
    CRect  clRect;
    CRect  workRect;

    __super::OnSize(nType, cx, cy);

    if (m_Picture == NULL)
        return; // Return if window is not created yet.


    GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    workRect = clRect;

    workRect.top += 70;
    workRect.right -= 200;


    m_Picture->SetWindowPos(this, workRect.left, workRect.top, 5 * 64 + 2, 5 * 64 + 2, SWP_NOZORDER);

    workRect.left = 5 * 64 + 2 + 10;
    workRect.right = workRect.left + 100;
    workRect.bottom = workRect.top + 30;

    m_LearnButton.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);

    workRect.left = workRect.right + 5;
    workRect.right += 100;

    m_ResetButton.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);

    workRect.left = workRect.right + 5;
    workRect.right += 100;

    m_RestoreButton.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);

    workRect.top = workRect.bottom + 10;
    workRect.bottom = clRect.bottom - 10;
    workRect.left = 5 * 64 + 2 + 10;
    workRect.right = workRect.left + 110;

    m_PictureList.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);

}
