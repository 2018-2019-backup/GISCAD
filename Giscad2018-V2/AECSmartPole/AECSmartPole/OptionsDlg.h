#pragma once


// COptionsDlg dialog

class COptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsDlg)

public:
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionsDlg();

// Dialog Data
	enum { IDD = IDD_SP_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  virtual void OnOK();
	afx_msg void OnHelp();

	DECLARE_MESSAGE_MAP()

public:
  CComboBox m_cbPoleStyle;
  CEdit m_edPoleTextHeight;
  CEdit m_edPoleDia;
  CComboBox m_cbSchStyle;
  CEdit m_edSchTextHeight;

  CString m_csPoleStyle;
  CString m_csPoleTextHeight;
  CString m_csPoleDia;
  CString m_csSchStyle;
  CString m_csSchTextHeight;
};
