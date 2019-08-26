#pragma once
#include "ACEdit.h"

// CAttachCableDlg dialog

class CAttachCableDlg : public CDialog
{
	DECLARE_DYNAMIC(CAttachCableDlg)

public:
	CAttachCableDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAttachCableDlg();

// Dialog Data
	enum { IDD = IDD_SP_CABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  virtual void OnOK();
	afx_msg void OnHelp();
	// afx_msg LONG OnUpdateConductorType(UINT lParam, LONG wParam);
	afx_msg LRESULT OnUpdateConductorType(WPARAM lParam, LPARAM wParam);
	DECLARE_MESSAGE_MAP()

public:
	CACEdit m_ectrlConductor;
	CAcUiLineTypeComboBox m_cbLineType;

  BOOL m_bStart;
  CString m_csConductor, m_csLayer, m_csLayerColor, m_csLType;
	
  afx_msg void OnSelectConductorType();
};
