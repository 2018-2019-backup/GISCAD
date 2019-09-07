/*!
 @Class			CGISCADLoaderApp
 @Brief			Defines the class behaviour of the class GISCADLoader.cpp
 @Author		Neilsoft Ltd
 @Date			12-07-2007
 @History 		Change history
*/

#include "stdafx.h"
#include "GISCADLoader.h"
#include "GISCADLoaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGISCADLoaderApp

BEGIN_MESSAGE_MAP(CGISCADLoaderApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGISCADLoaderApp construction

CGISCADLoaderApp::CGISCADLoaderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CGISCADLoaderApp object

CGISCADLoaderApp theApp;


// CGISCADLoaderApp initialization

BOOL CGISCADLoaderApp::InitInstance()
{

//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.

	TCHAR szApp_path[_MAX_PATH + 1];
	GetModuleFileName((HMODULE)AfxGetApp()->m_hInstance, szApp_path, MAX_PATH);
	CString strPath = szApp_path;
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	strPath+=_T("\\settings.ini");
	NSSTRING strFilename = (NSSTRING)strPath;

 	if(NSACCESS(strFilename.c_str(), 0) == -1)
	{
		MessageBox(NULL,_T("Settings.ini not found."),_T("GISCADLoader"), MB_OK);
		return FALSE;
	}
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CGISCADLoaderDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
