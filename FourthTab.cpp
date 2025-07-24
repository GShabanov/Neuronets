
// FourthTab.cpp : implementation file
//

#include "pch.h"
#include <afxdialogex.h>
#include "..\Shared\MFCPicture.h"
#include "FourthTab.h"
#include "resource.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFourthTab dialog

CFourthTab::CFourthTab()
    : CTabTemplate()
    , m_Picture(NULL)
    , m_bInPicture(FALSE)
    , m_Learned(0)
{
}


BEGIN_MESSAGE_MAP(CFourthTab, CTabTemplate)
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CFirstTab message handlers
BOOL
CFourthTab::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
    CRect rectDummy;
    rectDummy.SetRectEmpty();


    m_pNetwork = new CMlpNetwork<double>(64 * 64, 10);

    if (NULL == m_pNetwork)
        return FALSE;

    m_pNetwork->reset();


    if (__super::Create(dwStyle | WS_CHILD, rect, pParentWnd) != TRUE)
    {
        return FALSE;
    }

    m_Picture = new CArrayPicture(this);

    m_Picture->Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
        rectDummy, this);

    m_Picture->SetDrawCursor(LoadCursor((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1)));

    m_DoButton.Create(
        _T("Завчити"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, rectDummy, this, NULL);

    m_ResetButton.Create(
        _T("Скинути"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectDummy, this, NULL);

    m_classesStatic.Create(
        _T("Класи"), WS_CHILD | WS_VISIBLE, rectDummy, this, NULL);

    m_Clear.Create(
        _T("Очистити"), WS_CHILD | WS_VISIBLE, rectDummy, this, NULL);

    m_PictureList.Create(
        WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_OWNERDRAWFIXED,
        rectDummy,
        this, NULL);

    m_classes.SetSize(10);

    for (INT_PTR i = 0; i < m_classes.GetSize(); i++)
    {

        m_classes[i].checkBox.Create(
            _T(""), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, rectDummy, this, NULL);

        m_classes[i].m_InfoWindow.Create(
            WS_CHILD | WS_VISIBLE, rectDummy, this);

        m_classes[i].m_InfoWindow.SetNumber(0);
    }

    LoadTrainingData(CString(_T(".\\data\\")));


    AdjustLayout();

    return TRUE;  // return TRUE  unless you set the focus to a control

}

void
CFourthTab::AdjustLayout()
{
    CRect  clRect;
    CRect  workRect;

    if (m_Picture == NULL)
        return; // Return if window is not created yet.


    GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    workRect = clRect;

    workRect.top += 70;
    workRect.right -= 200;


    m_Picture->SetWindowPos(this, workRect.left, workRect.top, 5 * 64 + 2, 5 * 64 + 2, SWP_NOZORDER);

    workRect.top += (5 * 64 + 5);
    workRect.bottom = workRect.top + 30;
    workRect.left += (5 * 32 - 50);
    workRect.right = workRect.left + 100;

    m_Clear.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);


    workRect = clRect;

    workRect.top += 70;
    workRect.left = 5 * 64 + 2 + 10;
    workRect.right = workRect.left + 100;
    workRect.bottom = workRect.top + 30;

    m_DoButton.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);

    workRect.left = workRect.right + 5;
    workRect.right += 100;

    m_ResetButton.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);

    workRect.top = clRect.top + 75;
    workRect.bottom = workRect.top + 30;
    workRect.left = 5 * 64 + 265;
    workRect.right = workRect.left + 100;

    m_classesStatic.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);


    workRect.left = 5 * 64 + 2 + 10;
    workRect.right = workRect.left + 110;
    workRect.top = clRect.top + 110;
    workRect.bottom = clRect.bottom - 10;

    m_PictureList.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);


    workRect.top = clRect.top + 110;
    workRect.bottom = workRect.top + 56;
    workRect.left = 5 * 64 + 2 + 250;
    workRect.right = workRect.left + 20;

    for (INT_PTR i = 0; i < m_classes.GetCount(); i++)
    {
        m_classes[i].checkBox.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);

        CRect wr2 = workRect;

        wr2.left += 20;
        wr2.right += 100;

        m_classes[i].m_InfoWindow.SetWindowPos(this, wr2.left, wr2.top, wr2.Width(), wr2.Height(), SWP_NOZORDER);

        workRect.top += 60;
        workRect.bottom += 60;
    }
}


BOOL
CFourthTab::LoadImageFile(const CString &filename)
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
        drawContext->Number = 0;

        m_BitmapTable.Add(drawContext);

        m_PictureList.InsertItem(LVIF_IMAGE | LVIF_STATE | LVIF_PARAM, (int)(m_BitmapTable.GetSize() - 1), _T(""), 0, 0, 0, (LPARAM)drawContext);
    }

    pPicture->Release();

    return TRUE;
}

BOOL
CFourthTab::LoadTrainingData(const CString &path)
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
CFourthTab::OnDestroy()
{

    m_Picture->DestroyWindow();
    delete m_Picture;

    for (INT_PTR i = 0; i < m_classes.GetCount(); i++)
    {
        m_classes[i].checkBox.DestroyWindow();

        m_classes[i].m_InfoWindow.DestroyWindow();
    }


    m_DoButton.DestroyWindow();
    m_PictureList.DestroyWindow();
    //m_InfoWindow.DestroyWindow();

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
CFourthTab::OnPaint()
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


void
CFourthTab::OnSelectPicture(CListCtrlMy::DRAW_CONTEXT* pContext)
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
        m_DoButton.EnableWindow(FALSE);
    }
    else
    {
        m_DoButton.EnableWindow(TRUE);
    }

}

BOOL
CFourthTab::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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

            UpdateState();

        }

    }

    return __super::OnNotify(wParam, lParam, pResult);
}

void
CFourthTab::ControlCallback(CWnd* from, void* data)
{
    if (from != m_Picture)
        return;

    UpdateState();


    //m_pNetwork->Teach(learnArray);

}

void
CFourthTab::UpdateState()
{
    CArrayEx<COLORREF> curPicture = m_Picture->Serialize();

    std::vector<double>  learnArray;


    for (INT_PTR i = 0; i < curPicture.GetCount(); i++)
    {
        COLORREF  color = curPicture[i];

        /*ULONG  grayScale =
            (GetRValue(color) +
                GetGValue(color) +
                GetBValue(color)) / 3;*/

        ULONG Y = (30 * (ULONG)GetRValue(color) + 59 * (ULONG)GetGValue(color) + 11 * (ULONG)GetBValue(color)) / 100;

        if (Y > 90)
        {
            learnArray.push_back(-1.0f);
        }
        else
        {
            learnArray.push_back(+1.0f);
        }

    }


    std::vector<double> result = m_pNetwork->classify(learnArray);

    std::vector<double> conf = m_pNetwork->softmax(result);

    int best = 0;
    for (int i = 1; i < result.size(); ++i)
        if (conf[i] > conf[best]) best = i;

    for (INT_PTR i = 0; i < m_classes.GetCount(); i++)
    {
        m_classes[i].m_InfoWindow.SetNumber((float)(conf[i] * 100.0f));
        m_classes[i].m_InfoWindow.SetAlerted(i == best);

        m_classes[i].m_InfoWindow.Invalidate(TRUE);
    }
}


void
CFourthTab::OnLearnButton()
{
    POSITION pos = m_PictureList.GetFirstSelectedItemPosition();

    if (pos == NULL)
    {
        return;
    }

    //m_pItemData->Number = m_Learned;

    m_Learned++;

    int checked = -1;

    for (INT_PTR i = 0; i < m_classes.GetCount(); i++)
    {
        if (m_classes[i].checkBox.GetCheck() == BST_CHECKED)
        {
            checked = (int)i;
            break;
        }
    }

    if (checked == -1)
    {
        AfxMessageBox(_T("Оберіть клас"), MB_OK);
        return;
    }


    int nItem = m_PictureList.GetNextSelectedItem(pos);

    CListCtrlMy::DRAW_CONTEXT* m_pItemData = (CListCtrlMy::DRAW_CONTEXT*)m_PictureList.GetItemData(nItem);

    m_pItemData->Learned = TRUE;


    CArrayEx<COLORREF> curPicture = m_Picture->Serialize();


    std::vector<double>  learnArray;


    for (INT_PTR i = 0; i < curPicture.GetCount(); i++)
    {
        COLORREF  color = curPicture[i];

        /*ULONG  grayScale = 
            (GetRValue(color) +
            GetGValue(color) +
            GetBValue(color)) / 3;*/

        //double Y = 0.3 * GetRValue(color) + 0.59 * GetGValue(color) + 0.11 * GetBValue(color);
        ULONG Y = (30 * (ULONG)GetRValue(color) + 59 * (ULONG)GetGValue(color) + 11 * (ULONG)GetBValue(color)) / 100;

        if (Y > 90)
        {
            learnArray.push_back(-1.0f);
        }
        else
        {
            learnArray.push_back(+1.0f);
        }

    }

    m_pNetwork->trainOneSample(learnArray, checked);

    UpdateState();

    m_DoButton.EnableWindow(FALSE);
    m_PictureList.Invalidate(TRUE);

}

void
CFourthTab::OnClearButton()
{
    m_Picture->Clear();
    UpdateState();
}

void
CFourthTab::NetworkUpdateCallback(const CArray<double>& output)
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


BOOL
CFourthTab::OnCommand(
    WPARAM wParam,
    LPARAM lParam
    )
{
    if ((HWND)lParam == m_PictureList.m_hWnd)
    {
        m_PictureList.m_hWnd = m_PictureList.m_hWnd;
    }

    if ((HWND)lParam == m_DoButton.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {

        OnLearnButton();
    }

    if ((HWND)lParam == m_Clear.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {

        OnClearButton();
    }

    if ((HWND)lParam == m_ResetButton.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {
        m_pNetwork->reset();
        m_Learned = 0;

        for (INT_PTR i = 0, j = m_BitmapTable.GetSize(); i < j; i++)
        {
            m_BitmapTable[i]->Learned = FALSE;

            if (m_PictureList.GetFirstSelectedItemPosition() != NULL)
            {
                m_DoButton.EnableWindow(TRUE);
            }
            else
            {
                m_DoButton.EnableWindow(FALSE);
            }

            m_PictureList.Invalidate(TRUE);

        }

        UpdateState();

    }

    for (INT_PTR i = 0; i < m_classes.GetCount(); i++)
    {
        if ((HWND)lParam == m_classes[i].checkBox.m_hWnd &&
            HIWORD(wParam) == BN_CLICKED)
        {

            if (m_classes[i].checkBox.GetCheck() == BST_UNCHECKED)
            {
                for (INT_PTR j = 0; j < m_classes.GetCount(); j++)
                {
                    m_classes[j].checkBox.SetCheck(BST_UNCHECKED);
                }

                m_classes[i].checkBox.SetCheck(BST_CHECKED);
            }

        }

    }


    return __super::OnCommand(wParam, lParam);


}


void
CFourthTab::OnSize(UINT nType, int cx, int cy)
{
    CRect  clRect;
    CRect  workRect;

    __super::OnSize(nType, cx, cy);

    if (m_Picture == NULL)
        return; // Return if window is not created yet.

    AdjustLayout();

}
