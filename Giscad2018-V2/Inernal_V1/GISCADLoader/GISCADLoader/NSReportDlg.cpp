/*!
 @Class			CNSReportDlg
 @Brief			Displays the report of the conversion in the report tab
 @Author		Neilsoft Ltd
 @Date			21-07-2007
 @History 		Change history
*/
#include "stdafx.h"
#include "..\..\Include\NSDirectives.h"
#include "GISCADLoader.h"
#include "NSReportDlg.h"
#include "GISCADLoaderDlg.h"

extern CString gstrReportPaths;;
extern bool gValidate;

// CNSReportDlg dialog
IMPLEMENT_DYNAMIC(CNSReportDlg, CTabPageSSL)

CNSReportDlg::CNSReportDlg(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CNSReportDlg::IDD, pParent)
{

}
CNSReportDlg::~CNSReportDlg()
{
}
BOOL CNSReportDlg::OnInitDialog() 
{
	CTabPageSSL::OnInitDialog();
	(CGISCADLoaderDlg)this->GetParent();
	return TRUE;
}
void CNSReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_REPORT, m_Edit);
}

BEGIN_MESSAGE_MAP(CNSReportDlg, CTabPageSSL)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()
// CNSReportDlg message handlers

void CNSReportDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;
}
/*
@brief			This function is used to display the report in the report dialog
@Param (in)		The summary file in which links of the report is present 
@Param (out)	The contents of the report file is displayed
*/
int CNSReportDlg::display()
{
	CString strOutputFile;
    CString strFinal, strfilename;
	CStdioFile sourceFile, SummaryFile;
	CFileException ex;
    CGISCADLoaderDlg* pParent = (CGISCADLoaderDlg*)(this->GetParent());

	//**//SUBIR  - 10.02.2010
	m_Edit.SetWindowTextW(_T(""));
	//**//SUBIR  - 10.02.2010

    if(gValidate == false)
    {
	    CString strSummaryReport = pParent->GetSummaryFilePath();
      // open the source file for reading
       if (SummaryFile.Open(strSummaryReport,CFile::modeRead | CFile::shareDenyWrite, &ex))
       {
		    while(SummaryFile.ReadString(strfilename))
		    {
		        // we'll use a CFileException object to get error information	  
		       // open the source file for reading
			    CString strTemp;
		        if (sourceFile.Open(strfilename,CFile::modeRead | CFile::shareDenyWrite, &ex))
		        {
				    strFinal.Append(_T("============================================================\r\n"));
				    CString strFile = strfilename.Right(strfilename.GetLength() - strfilename.ReverseFind('\\') - 1);
				    strFinal.Append(_T("                               ")+strFile+_T("                        \r\n"));
				    strFinal.Append(_T("============================================================\r\n"));
				    while(sourceFile.ReadString(strTemp))
			        {
					    strFinal.Append(strTemp);
				        strFinal.Append(_T("\r\n"));
			        }
		        }
		        else
			        return NS_FAIL;
		        sourceFile.Close();
	        }
	        SummaryFile.Close();		   
        }
	    else
		    return NS_FAIL;
    }
    else
    {
        strFinal = gstrReportPaths;
    }

   //Display the report in rich edit ctrl
	m_Edit.SetWindowTextW(strFinal); 
    m_Edit.HideCaret();
    //m_Edit.SetWindowText(_T("file:c:\\test.html\r\n\r\nfile:c:\\a.html"));
    //m_Edit.SetWindowText(strReportPaths);
  
	return NS_SUCCESS;
}
/*
@brief			This function is used to delete the contents of report from rich edit ctrl
@Param (in)		NULL
@Param (out)	NULL
*/
void CNSReportDlg::deleteReport()
{
	m_Edit.SetWindowTextW(_T(" "));
}
/*
@brief			On Show Window of the report tab the Hide and Validate button
				should be hidden
@Param (in)		NULL
@Param (out)	NULL
*/
void CNSReportDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CTabPageSSL::OnShowWindow(bShow, nStatus);
	CGISCADLoaderDlg* obj = (CGISCADLoaderDlg*)(this->GetParent());
	obj->showControls(!bShow);
}