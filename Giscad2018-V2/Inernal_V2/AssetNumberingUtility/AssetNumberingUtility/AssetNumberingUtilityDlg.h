// AssetNumberingUtilityDlg.h : header file
//

#if !defined(AFX_ASSETNUMBERINGUTILITYDLG_H__2963E016_E2A1_43E4_A33D_92053101597C__INCLUDED_)
#define AFX_ASSETNUMBERINGUTILITYDLG_H__2963E016_E2A1_43E4_A33D_92053101597C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\nsconfigfilemgr.h"
#include "..\..\Include\NSAssetMgrIFace.h"
#include "..\..\Include\NSAsset.h"
#include "..\..\Include\NSEllipseConnector.h"
/////////////////////////////////////////////////////////////////////////////
// CAssetNumberingUtilityDlg dialog

class CAssetNumberingUtilityDlg : public CDialog
{
// Construction
public:
	CAssetNumberingUtilityDlg(CWnd* pParent = NULL);	// standard constructor
	~CAssetNumberingUtilityDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// Dialog Data
	//{{AFX_DATA(CAssetNumberingUtilityDlg)
	enum { IDD = IDD_ASSETNUMBERINGUTILITY_DIALOG };
	CListCtrl	m_assetsListCtrl;
	CEdit	m_district;
	CEdit	m_password;
	CEdit	m_user;
	/*CEdit	m_connectionInfo;
	CEdit	m_hostNameID;*/
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAssetNumberingUtilityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void setCAPNumber(const NSSTRING strCAPno);
	void setDrawingNumber(const NSSTRING strDwgNo);
	void setProjectNumber(const NSSTRING strProjNo);
	void setProjectType(const NSSTRING strUserStatus);
	void setDepotCode(const NSSTRING strDepotCode);
	void setConfigLocation(const NSSTRING strLoc);
	void setLogFileLocation(const NSSTRING strLoc);

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAssetNumberingUtilityDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCancel();
	afx_msg void OnHelp();
	afx_msg void OnGenerateNumbers();
	afx_msg void OnScan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void setRequestParameters(std::map<TCHAR*, TCHAR*> & mapOfRequest, NSSTRING strAssetType, CNSAsset* pAsset);
	void setReplyParameters(std::vector<TCHAR*> & replyVect);
	void writeHeaderInformation();
	void setProjectLink(NSSTRING strService, NSSTRING strReplyParam, std::map<TCHAR*, TCHAR*> mapOfRequest);
	void showHelp();
private:
	NSSTRING m_strCAPno;
	NSSTRING m_strDwgNo;
	NSSTRING m_strProjNo;
	NSSTRING m_strProjectType;
	NSSTRING m_strUserName;
	NSSTRING m_strDepotCode;
	CNSEllipseConnector ellipseConnector;
	ConfigFile* m_pConfigFile;
	CToolTipCtrl* m_pToolTip;
	bool m_bFlag;
	std::map<NSSTRING, NSSTRING> m_hostIDs;
	TCHAR* m_szHost;
	IAssetMgr* gAssetMgr;
	NSSTRING m_strConfigLoc;
	NSSTRING m_strLogFileLoc;
	FILE* fpDebugLog;
public:
	CButton m_btnGenerate;
	CComboBox m_hostID;
	afx_msg void OnCbnSelchangeCombo1();
	CEdit m_position;
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASSETNUMBERINGUTILITYDLG_H__2963E016_E2A1_43E4_A33D_92053101597C__INCLUDED_)
