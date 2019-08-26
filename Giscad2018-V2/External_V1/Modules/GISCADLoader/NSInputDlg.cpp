
#include "stdafx.h"
#include "GISCADLoader.h"
#include "NSInputDLG.h"


// CNSInputDLG dialog

IMPLEMENT_DYNAMIC(CNSInputDLG, CDialog)

CNSInputDLG::CNSInputDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CNSInputDLG::IDD, pParent)
{

}

CNSInputDLG::~CNSInputDLG()
{
}

void CNSInputDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_XMLFilename, m_XMLFileName);
}


BEGIN_MESSAGE_MAP(CNSInputDLG, CDialog)
	ON_BN_CLICKED(IDOK, &CNSInputDLG::OnBnClickedOk)
END_MESSAGE_MAP()


// CNSInputDLG message handlers


void CNSInputDLG::OnBnClickedOk()
{
	m_XMLFileName.GetWindowTextW(m_strXMLName);
	this->OnOK(); 
}
CString CNSInputDLG::getXMLName(CString &strXMLName1)
{
	return m_strXMLName;
}
