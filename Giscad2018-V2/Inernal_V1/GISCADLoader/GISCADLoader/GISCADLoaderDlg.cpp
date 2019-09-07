/*!
 @Class			CGISCADLoaderApp
 @Brief			Implements the different tabs in the UI.
 @Author		Neilsoft Ltd
 @Date			12-07-2007
 @History 		Change history
*/
#include "..\..\Include\nsconfigfilemgr.h"
#include "stdafx.h"
#include "GISCADLoader.h"
#include "GISCADLoaderDlg.h"
#include "NSTabCtrlSSL.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About
//HINSTANCE m_hLibRichEdit;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnKillfocusOk();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// Implementation
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

CGISCADLoaderDlg::CGISCADLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGISCADLoaderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CGISCADLoaderDlg::~CGISCADLoaderDlg()
{
//	FreeLibrary(m_hLibRichEdit);	//Free the library of the RichEdit ctrl
}
void CGISCADLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CTRL, m_TabCtrl);
	DDX_Control(pDX, IDC_BTN_EXIT, m_exit);
	//DDX_Control(pDX, IDC_BTN_HELP, m_help);
	DDX_Control(pDX, IDC_BTN_VALIDATE, m_validate);
	DDX_Control(pDX, IDC_BTN_CONVERT, m_convert);
}

BEGIN_MESSAGE_MAP(CGISCADLoaderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONVERT, &CGISCADLoaderDlg::OnBnClickedBtnConvert)
	ON_BN_CLICKED(IDC_BTN_VALIDATE, &CGISCADLoaderDlg::OnBnClickedBtnValidate)
	//ON_BN_CLICKED(IDC_BTN_HELP, &CGISCADLoaderDlg::OnBnClickedBtnHelp)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CGISCADLoaderDlg::OnBnClickedBtnExit)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CTRL, &CGISCADLoaderDlg::OnTcnSelchangeTabCtrl)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
    ON_WM_HELPINFO()
END_MESSAGE_MAP()
// CGISCADLoaderDlg message handlers

BOOL CGISCADLoaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	 
	//if(LoadLibrary(_T("RICHED32.DLL")) == NULL)
	//	return FALSE;
	//InitCommonControls();
#ifndef VALIDATOR
	m_validate.EnableWindow(false);
#endif
	int nPageID = 0;
	m_LoaderDlg.Create(IDD_LOADER, this);
	m_TabCtrl.AddSSLPage(_T("Validation"), nPageID++, &m_LoaderDlg);
	m_ReportDlg.Create(IDD_REPORT, this);
	m_TabCtrl.AddSSLPage(_T("Report"), nPageID++, &m_ReportDlg);
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CGISCADLoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CGISCADLoaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGISCADLoaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*
@brief		The function calls the convert function in NSLoader.cpp and 
			the display function in NSReport.cpp
@Param(in)	NULL
*/
void CGISCADLoaderDlg::OnBnClickedBtnConvert()
{
	m_LoaderDlg.OnBnClickedBtnConvert();
	m_ReportDlg.display();
}
/*
@brief		The function calls the validate function in NSLoader.cpp and 
			the display function in NSReport.cpp
@Param(in)	NULL
*/
void CGISCADLoaderDlg::OnBnClickedBtnValidate()
{
	m_LoaderDlg.OnBnClickedBtnValidate();
    m_ReportDlg.display();
}
/*
@brief		The function calls the help function in NSLoader.cpp
@Param(in)	NULL
*/

void CGISCADLoaderDlg::showHelp()
{
    /*TCHAR szAppPath[MAX_PATH+1];
    GetCurrentDirectory(MAX_PATH+1,szAppPath);
    CString strAppPath = szAppPath;*/
    TCHAR szAppPath[MAX_PATH+1];
	GetModuleFileName(NULL,szAppPath,MAX_PATH+1);
	CString strAppPath = szAppPath;
	int nIndex = strAppPath.ReverseFind('\\');
	if(nIndex != -1)
	{
		strAppPath = strAppPath.Left(nIndex);
	}
    strAppPath.Append(_T("\\Help\\DWG2XML.chm"));
   	::HtmlHelp(NULL,strAppPath,HH_DISPLAY_TOC,0);
}
//void CGISCADLoaderDlg::OnBnClickedBtnHelp()
//{
//	//m_LoaderDlg.unselectListCtrl();
//    showHelp();
//}

/*
@brief		The function saves the summary path in the path as mentioned in the UI
			to store xml file
@Param(in)	NULL
*/
void CGISCADLoaderDlg::saveOutputPathToInI()
{
	CString strSummaryPath = m_LoaderDlg.getSummaryPath();
	strSummaryPath = strSummaryPath.Left(strSummaryPath.ReverseFind('\\'));

	ConfigFile* configFile = new ConfigFile(_T("settings.ini"));
	//if(configFile!=NULL)
	//{
	//	{
	//		//m_PathName.SetWindowTextW(szValue);			
	//	}
	//}
}
/*
@brief		Closes the UI and deletes the summary file
@Param(in)  NULL
@Param(out) NULL
*/
void CGISCADLoaderDlg::OnBnClickedBtnExit()
{
	CString strOutputFile;	
	strOutputFile = GetSummaryFilePath();
	DeleteFile(strOutputFile);
	this->DestroyWindow();
}
/*
@brief		Calls  the function in NSLoader.cpp to display summary file
@Param(in)  NULL
@Param(out) NULL
*/
CString CGISCADLoaderDlg::GetSummaryFilePath()
{
	return m_LoaderDlg.getSummaryPath();
}
void CGISCADLoaderDlg::OnTcnSelchangeTabCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}
void CGISCADLoaderDlg::showControls(BOOL bValue)
{
		m_convert.ShowWindow(bValue);
		m_validate.ShowWindow(bValue);
		m_LoaderDlg.showProgressBar();
}
/*
@brief		Disables and enables the Convert and Validate button accordingly
@Param(in)  NULL
@Param(out) NULL
*/
void CGISCADLoaderDlg::enableControls(BOOL bValue)
{
#ifdef VALIDATOR
	m_validate.EnableWindow(bValue);
#endif
	m_convert.EnableWindow(bValue);
}
void CGISCADLoaderDlg::enableHelpExit(BOOL bValue)
{
	m_help.EnableWindow(bValue);
	m_exit.EnableWindow(bValue);
}
/*
@brief		Calls  the function in NSReport.cpp to remove the contents in report tab
@Param(in)  NULL
@Param(out) NULL
*/
void CGISCADLoaderDlg::deleteReport()
{
	m_ReportDlg.deleteReport();
}
/*
@brief		Closes the UI and deletes the summary file
@Param(in)  NULL
@Param(out) NULL
*/
void CGISCADLoaderDlg::OnClose()
{
	CString strOutputFile;	
	strOutputFile = GetSummaryFilePath();
	DeleteFile(strOutputFile);
	CDialog::OnClose();
}


BOOL CGISCADLoaderDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
    // TODO: Add your message handler code here and/or call default

    showHelp();
    return TRUE;
    //return CDialog::OnHelpInfo(pHelpInfo);
}
