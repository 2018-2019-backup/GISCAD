/*!
 @Class			CGISCADLoaderDlg
 @Brief			This class is used to insert different tabs in the UI
 @Author		Neilsoft Ltd
 @Date			12-07-2007
 @History 		Change history
*/
#pragma once
#include "NSTabCtrlSSL.h"
#include "NSLoaderDlg.h"
#include "NSReportDlg.h"
#include "afxwin.h"
#include "afxcmn.h"

// CGISCADLoaderDlg dialog
class CGISCADLoaderDlg : public CDialog
{
// Construction
public:
	CGISCADLoaderDlg(CWnd* pParent = NULL);				// standard constructor
	~CGISCADLoaderDlg();
	// Dialog Data
	enum { IDD = IDD_GISCADLOADER_DIALOG };

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CButton getValidate();
	CButton getConvert();
	void showControls(BOOL bValue);
	void enableControls(BOOL bValue);
	void enableHelpExit(BOOL bValue);
	void deleteReport();
	CString GetSummaryFilePath();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Implementation
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


private:
	afx_msg void OnBnClickedBtnConvert();
	afx_msg void OnBnClickedBtnValidate();
	//afx_msg void OnBnClickedBtnHelp();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnTcnSelchangeTabCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	void saveOutputPathToInI();
    
private:
	CNSTabCtrlSSL m_TabCtrl;
	CNSLoaderDlg m_LoaderDlg;
	CNSReportDlg m_ReportDlg;
	CButton m_exit;
	CButton m_help;
	CButton m_validate;
	CButton m_convert;
    void showHelp();
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

