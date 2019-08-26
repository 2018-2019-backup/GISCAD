#pragma once

#include "EditListCtrl.h"
#include "ReplaceLightDlg.h"
#include "IEComboBox.h"

// CLightDlg dialog

class CLightDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightDlg)

public:
	CLightDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightDlg();

// Dialog Data
	enum { IDD = IDD_SP_LIGHT_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  afx_msg void OnClickNonStandard();
  afx_msg void OnSelectStatus();
  afx_msg void OnClickReplace();
  afx_msg void OnClickColumn();
  afx_msg void OnSelectSchedule();
  afx_msg void OnSelectType();
  afx_msg void OnSelectMast();
  afx_msg void OnSelectOutreach();
  afx_msg void OnSelectBracket();
	afx_msg void OnHelp();
  virtual void OnOK();
	BOOL RecreateComboBox(CComboBox* pCombo, LPVOID lpParam/*=0*/);

	DECLARE_MESSAGE_MAP()

public:
  // Controls
  CStatic m_stInfoBox;
  CStatic m_stChargeBox;
  CStatic m_stAssetBox;
  CButton m_btnNonStandard;
  CButton m_btnReplace;
  CButton m_btnInsertColumn;
  CButton m_btnAction;
  CComboBox m_cbLanternStatus;
  CComboBox m_cbLanternSchedule;
  CComboBox m_cbLanternColumn;
  CIEComboBox m_cbLanternType;
  CIEComboBox m_cbLanternMast;
  CIEComboBox m_cbLanternLamp;
  CIEComboBox m_cbLanternOutreach;
  CIEComboBox m_cbLanternBracket;

CIEComboBox m_cbRagBolt;
CIEComboBox m_cbFoundationType;

  CIEComboBox m_cbLanternCategory;
  CEdit m_edLanternStockCode;
  CEdit m_edLampStockCode;
  CEdit m_edRateCode;
  CEdit m_edKingbolt;
  CEdit m_edMastCode;
  CEdit m_edOutreachCode;
  CEdit m_edBracketCode;
  CEdit m_edPoleNumber;
  CEdit m_edLanternNumber;
  CEdit m_edAssetColumn;
	CEdit m_edLightNumber;
  CEdit m_edLanternLocation;

  CEdit m_edMASTRATE;
  CEdit m_edOutreachRate;
  CEdit m_edBracketRate;
	CEdit m_edMountingHeight;

  CEditListCtrl m_lcCharge;
  CUIntArray m_uiaLightNumbers;
	
  // Control variables
  BOOL m_bColumnReq;
  BOOL m_bNonStandard;
	CString m_csLightNumber;
  CString m_csLanternStatus;
  CString m_csLanternSchedule;
  CString m_csLanternColumn;
  CString m_csLanternType;
  CString m_csLanternLamp;
  CString m_csLanternMast;
  CString m_csLanternOutreach;
  CString m_csLanternBracket;
  CString m_csLanternCategory;
  CString m_csLanternStockCode;
  CString m_csLampStockCode;
  CString m_csRateCode;
  CString m_csKingbolt;
  CString m_csMastCode;
  CString m_csOutreachCode;
  CString m_csBracketCode;
  CString m_csLanternLocation;
  CString m_csAssetColumn;
  CString m_csLanternNumber;
	CString m_csExistingLightNumber;
	
	CString m_csRagBolt;
	CString m_csFoundationType;

	CString m_csMASTRATE;
	CString m_csOutreachRate;
	CString m_csBracketRate;
CString m_csMountingHeight;
	// New
	CStringArray m_csaLanternTypes;
	CStringArray m_csaLanternLayers;
	CStringArray m_csaLanternBlocks;
	CStringArray m_csaLanternLayerColors;
		
  // Other variables
  int m_iSchedule;
  BOOL m_bFirstRun, m_bEditing, m_bReplace, m_bReplaceData;
  
	CString m_csLanternSymbol;
	CString m_csLanternLayer;
	CString m_csLanternLayerColor;

	CString m_csExistingLanternSymbol;
	
	CString m_csColumnSymbol;
	CString m_csColumnLayer;
	CString m_csColumnLayerColor;
	
  CString m_csChargeList;
  CStringArray m_csaChargeTo;
	TCHAR m_szColumnHandle[17];

  // Replace light dialog
  CReplaceLightDlg m_dlgRL;
	afx_msg void OnBnClickedClearall();
	afx_msg void OnLvnItemchangedLightChargeList(NMHDR *pNMHDR, LRESULT *pResult);
};
