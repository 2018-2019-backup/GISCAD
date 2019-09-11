#pragma once
#include "resource.h"
#include "NSTabPageSSL.h"
#include "..\..\Include\nsconfigfilemgr.h"



// CNSEllipseDlg dialog

class CNSEllipseDlg : public CTabPageSSL
{
	DECLARE_DYNAMIC(CNSEllipseDlg)

public:
	CNSEllipseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNSEllipseDlg();
	CEdit m_User;
	CEdit m_Password;
	CEdit m_District;

// Dialog Data
	enum { IDD = IDD_ELLIPSE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
public:
	CButton m_assetCheck;
	CButton m_ChkCadValid;
	CButton m_ChkGisValid;
	CButton m_ChkP1Errors;
	CButton m_ChkP2Errors;
	CButton m_ChkP3Errors;
	CButton m_ChkP4Errors;
	virtual BOOL OnInitDialog();
	void Validate(CString szDwgPath);
	CString GetHTMLReportPath();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CEdit m_Position;
	CComboBox m_Host;
	afx_msg void OnCbnSelchangeCombo1();
private:
	void EnableControls();
	bool checkErrorBoxChanged(NSSTRING& dlgErrorMsgs, NSSTRING& fileErrorMsgs);
	CToolTipCtrl* m_pToolTip;
	ConfigFile* m_pConfigFile;
	CString GetServerStdDwgPath(CString szVariableToFind);
	std::map<NSSTRING, NSSTRING> m_hostIDs;
};
