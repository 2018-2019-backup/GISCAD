#pragma once

#include "EditListCtrl.h"
#include "IEComboBox.h"

// CReplaceLightDlg dialog

class CReplaceLightDlg : public CDialog
{
	DECLARE_DYNAMIC(CReplaceLightDlg)

public:
	CReplaceLightDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReplaceLightDlg();

// Dialog Data
	enum { IDD = IDD_SP_LIGHT_REPLACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  afx_msg void OnClickNonStandard();
  afx_msg void OnSelectSchedule();
  afx_msg void OnSelectType();
	afx_msg void OnHelp();
  virtual void OnOK();

	DECLARE_MESSAGE_MAP()

public:
  // Controls
  CStatic m_stChargeBox;
  CStatic m_stAssetBox;
  CButton m_btnNonStandard;
  CComboBox m_cbLanternSchedule;
  CIEComboBox m_cbLanternType;
  CIEComboBox m_cbLanternLamp;
  CIEComboBox m_cbLanternCategory;
  CEdit m_edRateCode;
  CEdit m_edAssetColumn;
  CEdit m_edAssetLantern;
  CEditListCtrl m_lcCharge;

  // Control variables
  BOOL m_bNonStandard;
  CString m_csLanternSchedule;
  CString m_csLanternType;
  CString m_csLanternLamp;
  CString m_csRateCode;
  CString m_csLanternCategory;
  CString m_csAssetColumn;
  CString m_csAssetLantern;

  // Other variables
  int m_iSchedule;
  CString m_csChargeList;
  CStringArray m_csaChargeTo;
};
