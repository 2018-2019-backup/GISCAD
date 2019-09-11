#pragma once
# include "resource.h"
# include "NSEllipseDlg.h"
# include "NSReportDialog.h"
# include "NSTabCtrlSSL.h"


// CNSValidatorDlg dialog

class CNSValidatorDlg : public CDialog
{
	DECLARE_DYNAMIC(CNSValidatorDlg)

public:
	CNSValidatorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNSValidatorDlg();

// Dialog Data
	enum { IDD = IDD_VALIDATE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonHelp();
	CNSTabCtrlSSL m_TabControl;
	CNSEllipseDlg m_EllipseDlg;
	CNSReportDialog m_ReportDlg;
	virtual BOOL OnInitDialog();
	void SetCurrentDrawing(CString strCurrentDwg);
	CString GetHTMLReportPath();
	void SetHTMLReportPath(CString strHTMLPath);
	void showWindow(bool bShow);
private:
	void showHelp();
private:
	CString m_CurrentDwg;
	bool bExited;
	CString m_HTMLReportPath;
public:
	CButton m_Validator;
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
