// GISCADConvertorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GISCADLoader.h"
#include "GISCADConvertorDlg.h"
#include "afxdialogex.h"


// GISCADConvertorDlg dialog

IMPLEMENT_DYNAMIC(GISCADConvertorDlg, CDialogEx)

GISCADConvertorDlg::GISCADConvertorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(GISCADConvertorDlg::IDD, pParent)
{

}

GISCADConvertorDlg::~GISCADConvertorDlg()
{
}

void GISCADConvertorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GISCADConvertorDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &GISCADConvertorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &GISCADConvertorDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// GISCADConvertorDlg message handlers


void GISCADConvertorDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void GISCADConvertorDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
