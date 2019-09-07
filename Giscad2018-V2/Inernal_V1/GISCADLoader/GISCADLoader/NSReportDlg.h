/*!
 @Class			CNSReportDlg
 @Brief			This class provides the UI functionality to display report
 @Author		Neilsoft Ltd
 @Date			20-07-2007
 @History 		Change history
*/
#pragma once
#include "NSTabPageSSL.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "NSHyperEdit.h"
// CNSReportDlg dialog

class CNSReportDlg : public CTabPageSSL
{
	DECLARE_DYNAMIC(CNSReportDlg)
public:
	CNSReportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNSReportDlg();
	// Dialog Data
	enum { IDD = IDD_REPORT };

	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox m_ListBox;
	CProgressCtrl m_ProgressBar1;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnInitDialog();
	afx_msg int display();
	afx_msg void deleteReport();
	CNSHyperEdit m_Edit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
