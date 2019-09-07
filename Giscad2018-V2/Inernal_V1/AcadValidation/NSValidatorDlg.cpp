// NSValidatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NSValidatorDlg.h"
#include "NSEllipseDlg.h"
#include "NSReportDialog.h"


// CNSValidatorDlg dialog

IMPLEMENT_DYNAMIC(CNSValidatorDlg, CDialog)
CNSValidatorDlg::CNSValidatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNSValidatorDlg::IDD, pParent)
{
}

CNSValidatorDlg::~CNSValidatorDlg()
{
}

void CNSValidatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabControl);
	DDX_Control(pDX, IDOK, m_Validator);
}


BEGIN_MESSAGE_MAP(CNSValidatorDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnBnClickedButtonHelp)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


/*!
 @Brief			Event handler for Validate button
 @Date			13 Aug 2007
*/

void CNSValidatorDlg::OnBnClickedOk()
{
	/*CProgressCtrl * pProgressCtrl = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS1);
	if(pProgressCtrl!=NULL)
	{
		pProgressCtrl->SetRange(0,100);
		pProgressCtrl->SetStep(50);
		pProgressCtrl->SetPos(0);
		pProgressCtrl->StepIt();
	}*/
	m_EllipseDlg.Validate(m_CurrentDwg);
	//pProgressCtrl->SetPos(100);
}

/*!
 @Brief			Event handler for cancel button. Closed the dialog.
 @Date			13 Aug 2007
*/
void CNSValidatorDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	EndDialog(IDCANCEL);

	//OnCancel();
}

/*!
 @Brief			Event handler for Help button. Opens associated help file
 @Date			13 Aug 2007
*/

void CNSValidatorDlg::OnBnClickedButtonHelp()
{
	// TODO: Add your control notification handler code here
	showHelp();
}

BOOL CNSValidatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int nPageID = 0;
	m_EllipseDlg.Create(IDD_ELLIPSE_DIALOG, this);
	m_TabControl.AddSSLPage(_T("Validation"), nPageID++, &m_EllipseDlg);
	m_ReportDlg.Create(IDD_REPORT_DIALOG, this);
	m_TabControl.AddSSLPage(_T("Report"), nPageID++, &m_ReportDlg);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->SetIcon(m_hIcon,false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*!
 @Brief			Sets the current drawing file member varaible
 @Date			13 Aug 2007
*/
void CNSValidatorDlg::SetCurrentDrawing(CString strDwgName)
{
	m_CurrentDwg = strDwgName;
}

/*!
 @Brief			Gets the HTML report path member variable
 @Date			13 Aug 2007
*/

CString CNSValidatorDlg::GetHTMLReportPath()
{
	return m_HTMLReportPath;
}

/*!
 @Brief			Sets the HTML report path member variable
 @Date			13 Aug 2007
*/
void CNSValidatorDlg::SetHTMLReportPath(CString strHTMLPath)
{
	m_HTMLReportPath= strHTMLPath;
}
void CNSValidatorDlg::showWindow(bool bShow)
{
	m_Validator.ShowWindow(bShow);
}
BOOL CNSValidatorDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	
	showHelp();
	return CDialog::OnHelpInfo(pHelpInfo);
}

void CNSValidatorDlg::showHelp()
{
	TCHAR szAppPath[MAX_PATH+1];
	GetModuleFileName(_hdllInstance,szAppPath,MAX_PATH+1);
	CString strAppPath = szAppPath;
	int nIndex = strAppPath.ReverseFind('\\');
	if(nIndex != -1)
	{
		strAppPath = strAppPath.Left(nIndex);
	}
    strAppPath.Append(_T("\\Help\\DesignValCad.chm"));
	::HtmlHelp(this->m_hWnd,strAppPath,HH_DISPLAY_TOC,0);
}
