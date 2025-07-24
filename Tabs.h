#pragma once

// CTabTemplate.h : header file
//


// CTabTemplate default
class CTabTemplate : public CWnd
{

    // Construction
protected:
    class CFont      m_bigFont;
    class CFont      m_smallFont;

    CTabTemplate();


public:
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);


    // Implementation
protected:

    // Generated message map functions
    DECLARE_MESSAGE_MAP()
};
