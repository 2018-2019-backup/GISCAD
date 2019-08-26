#pragma once
#include "afxwin.h"


#pragma once


// CNSInputDLG dialog

class CNSInputDLG : public CDialog
{
	DECLARE_DYNAMIC(CNSInputDLG)

public:
	CNSInputDLG(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNSInputDLG();

// Dialog Data
	enum { IDD = IDD_INPUTDLG };
public:
	CString getXMLName(CString &strXMLName1);

	CEdit m_XMLFileName;
	CString m_strXMLName;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

};
