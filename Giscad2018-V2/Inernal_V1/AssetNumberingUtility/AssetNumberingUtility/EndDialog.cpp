// EndDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EndDialog.h"


// CEndDialog dialog

//IMPLEMENT_DYNCREATE(CEndDialog, CDialog)

CEndDialog::CEndDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEndDialog::IDD, pParent)
{
	m_ErrorShowFlag = false;
}

CEndDialog::~CEndDialog()
{
}

void CEndDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PICTURE, m_ErrorPicture);
}

BOOL CEndDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CStatic* pErrorLabel = (CStatic*) GetDlgItem(IDC_ERROR);
	if(pErrorLabel!=NULL)
	{
		/*HBITMAP hBmp = NULL;*/
		if(m_ErrorShowFlag)
		{
			pErrorLabel->SetWindowText(_T("Warnings or errors found !!"));
			/*hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
                                        MAKEINTRESOURCE(IDB_BITMAP3),
                                        IMAGE_BITMAP,
                                        0,0,
                                        LR_LOADMAP3DCOLORS);*/

		}
		/*else
			hBmp = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(OBM_CHECK));
			/*hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
                                        MAKEINTRESOURCE(IDB_BITMAP2),
                                        IMAGE_BITMAP,
                                        0,0,
                                        LR_LOADMAP3DCOLORS);
		if(hBmp!=NULL)
			m_ErrorPicture.SetBitmap(hBmp);*/
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CEndDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnButtonOK)
	ON_BN_CLICKED(IDCANCEL, OnButtonCancel)
END_MESSAGE_MAP()

// CEndDialog message handlers

void CEndDialog::OnButtonOK()
{
	OnOK();
}
void CEndDialog::OnButtonCancel()
{
	OnCancel();
}

void CEndDialog::setErrorFlag(bool bFlag)
{
	m_ErrorShowFlag = bFlag;
}