#pragma once
# include "resource.h"
# include "NSTabCtrlSSL.h"
# include "NSHyperEdit.h"


// CNSReportDialog dialog

class CNSReportDialog : public CTabPageSSL
{
	DECLARE_DYNAMIC(CNSReportDialog)

public:
	CNSReportDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNSReportDialog();
	

// Dialog Data
	enum { IDD = IDD_REPORT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CNSHyperEdit m_edit;

	afx_msg void OnStnClickedHtmlLink();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CBrush* brush;
	void SetHTMLReportName(const char* strHtmlFile);
	CString m_htmlPath;
};
