
// Neuronests.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "Neuronets.h"
#include "NeuronetsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNeuronetsApp

BEGIN_MESSAGE_MAP(CNeuronetsApp, CWinApp)
   ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNeuronestsApp construction

CNeuronetsApp::CNeuronetsApp()
{
}


// The one and only CNeuronetsApp object

CNeuronetsApp theApp;


// CNeuronetsApp initialization

BOOL CNeuronetsApp::InitInstance()
{
    CWinApp::InitInstance();


    // Create the shell manager, in case the dialog contains
    // any shell tree view or shell list view controls.
    CShellManager *pShellManager = new CShellManager;

    // Activate "Windows Native" visual manager for enabling themes in MFC controls
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));


    CNeuronetsDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();


    // Delete the shell manager created above.
    if (pShellManager != nullptr)
    {
        delete pShellManager;
    }

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
    ControlBarCleanUp();
#endif

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}

