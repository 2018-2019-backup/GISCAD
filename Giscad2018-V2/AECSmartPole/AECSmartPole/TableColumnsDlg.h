#pragma once


// CTableColumnsDlg dialog

class CTableColumnsDlg : public CDialog
{
	DECLARE_DYNAMIC(CTableColumnsDlg)

public:
	CTableColumnsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTableColumnsDlg();

// Dialog Data
	enum { IDD = IDD_TABLE_COLUMNS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedSelectAll();
  afx_msg void OnBnClickedRemoveAll();
	afx_msg void OnHelp();
	
  virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
  void ShowLayoutsInDrawing();
  void ShowTextStylesInDrawing();

public:
  CListCtrl m_lcColumns;
  CComboBox m_cbLayouts;
  CComboBox m_cbStyles;
  CEdit m_edHeight;

  int m_iCalledFor;
  CString m_csLayout;
  CString m_csStyle;
  CString m_csHeight;
  CStringArray m_csaColumns;
  afx_msg void OnLvnItemchangedTableList(NMHDR *pNMHDR, LRESULT *pResult);
};
