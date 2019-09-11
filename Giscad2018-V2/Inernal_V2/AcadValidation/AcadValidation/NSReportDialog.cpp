// NSReportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "NSReportDialog.h"
#include "NSValidatorDlg.h"
#include "shlobj.h"

// CNSReportDialog dialog

extern CString gStrHTMLReportPath;

IMPLEMENT_DYNAMIC(CNSReportDialog, CDialog)
CNSReportDialog::CNSReportDialog(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CNSReportDialog::IDD, pParent)
	, m_htmlPath()
{
	brush = new CBrush(RGB(49,49,49));
}

CNSReportDialog::~CNSReportDialog()
{
}

void CNSReportDialog::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_EDIT, m_edit);
}

BEGIN_MESSAGE_MAP(CNSReportDialog, CTabPageSSL)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()

END_MESSAGE_MAP()


// CNSReportDialog message handlers

BOOL CNSReportDialog::OnInitDialog()
{
	CTabPageSSL::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
/*!
 @Brief			Sets the HTML report name based on title block in dwg
 @Date			13 Aug 2007
*/
void CNSReportDialog::SetHTMLReportName(const char* strHtmlFile)
{
	//AfxMessageBox(strHtmlFile);
	//m_edit.SetWindowText(strHtmlFile);
//	m_edit.UpdateData();
}

HBRUSH CNSReportDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTabPageSSL::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	switch(nCtlColor)
	{
		case CTLCOLOR_STATIC:
 			pDC->SetTextColor(RGB(0,0, 255));
 			return (HBRUSH)(brush->GetSafeHandle());
	}
	return hbr;
}

void CNSReportDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_edit.SetWindowText(gStrHTMLReportPath);
	CNSValidatorDlg* validatorObj = (CNSValidatorDlg*)(this->GetParent());
	validatorObj->showWindow(false);
}

BOOL CNSReportDialog::PreTranslateMessage(MSG* pMsg)
{
    if(!(pMsg->wParam==VK_TAB) && (pMsg->message==WM_KEYDOWN))
    {
        return TRUE;
    } 

	if(pMsg->message==WM_RBUTTONDOWN)
    {
        return TRUE;
    } 
    return CDialog::PreTranslateMessage(pMsg);  
}

