#pragma once
#include "resource.h"

// CEndDialog dialog

class CEndDialog : public CDialog
{
	//DECLARE_DYNAMIC(CEndDialog)

public:
	CEndDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEndDialog();

// Dialog Data
	enum { IDD = IDD_END_DIALOG };

	void setErrorFlag(bool bFlag);

protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEndDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CEndDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOK();
	afx_msg void OnButtonCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_ErrorShowFlag;
public:
	CStatic m_ErrorPicture;
};
