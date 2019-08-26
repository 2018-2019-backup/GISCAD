#pragma once

#include "EditListCtrl.h"

// CPoleDlg dialog

class CPoleDlg : public CDialog
{
	DECLARE_DYNAMIC(CPoleDlg)

public:
	CPoleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPoleDlg();

// Dialog Data
	enum { IDD = IDD_SP_POLE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  afx_msg void OnSelectPoleStatus();
  afx_msg void OnSelectPoleSize();
  afx_msg void OnClickedStayRequired();
  afx_msg void OnSelectHVABC();
  afx_msg void OnSelectHVBare();
  afx_msg void OnSelectHVOther();
  afx_msg void OnSelectLVABC();
  afx_msg void OnSelectLVBare();
  afx_msg void OnSelectLVOther();
  afx_msg void OnCheckHVList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnCheckLVList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnClearAll();
	afx_msg void OnHelp();
  virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
  void AddRemoveListItems(CString csType, CListCtrl& lcList, CButton& btnCheck, CString csCheck, CStringArray& csaCat, CStringArray& csaText);

public:
  // Controls
  CStatic m_stInfoBox;
  CStatic m_stConsBox;
  CStatic m_stHVBox;
  CStatic m_stLVBox;
  CStatic m_stLanternBox;
  CStatic m_stHVSel;
  CStatic m_stLVSel;
  CComboBox m_cbPoleStatus;
  CComboBox m_cbPoleStay;
  CComboBox m_cbPoleSize;
  CComboBox m_cbPoleFooting;
  CComboBox m_cbLanternBracket;
  CComboBox m_cbLanternStatus;
  CButton m_btnStay;
  CEdit m_edPoleNumber;
  CEdit m_edFieldNumberNew;
	CEdit m_edFieldNumberExist;
  CEdit m_edPoleDia;
  CEdit m_edHoleDia;
  CEdit m_edHoleDepth;
  CEdit m_edSpanLength;
  CEdit m_edLineDevDeg;
  CEdit m_edPoleDesc;
  CButton m_btnDisplay;
  CEditListCtrl m_lcHV;
  CEditListCtrl m_lcLV;
  CButton m_btnHVABC;
  CButton m_btnHVBare;
  CButton m_btnHVOther;
  CButton m_btnLVABC;
  CButton m_btnLVBare;
  CButton m_btnLVOther;
  CButton m_btnAction;

	// New
	CStringArray m_csaLanternTypes;


  // Control variables
  CString m_csPoleStatus;
  CString m_csPoleStay;
  CString m_csPoleNumber;
  CString m_csFieldNumberNew;
	CString m_csFieldNumberExist;
  CString m_csPoleSize;
  CString m_csPoleFooting;
  CString m_csPoleDia;
  CString m_csHoleDia;
  CString m_csHoleDepth;
  CString m_csSpanLength;
  CString m_csLineDevDeg;
  CString m_csLanternBracket;
  CString m_csLanternStatus;
  CString m_csPoleDesc;
  CString m_csPoleHV;
  CString m_csPoleLV;
  BOOL m_bStayReq;
  BOOL m_bDisplayPoleNumber;
  BOOL m_bHVABC, m_bHVBare, m_bHVOther;
  BOOL m_bLVABC, m_bLVBare, m_bLVOther;
  unsigned int m_iActiveEditListCtrlID;

	CStringArray m_csaHVBare, m_csaLVBare;

  // Other variables
  BOOL m_bFirstRun;
	BOOL m_bEditing;

	CString m_csPoleSymbol;
	CString m_csPoleLayer;
	CString m_csPoleLayerColor;
	CUIntArray m_uiaPoleNumbers;

	CString m_csExistingPoleNumber;
	CString m_csExistingPoleSymbol;
	
	void UpdateSelectedValues(CListCtrl& lcList, UINT nID);
  afx_msg void OnLvnEndlabeleditPoleHvList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnLvnEndlabeleditPoleLvList(NMHDR *pNMHDR, LRESULT *pResult);
};
