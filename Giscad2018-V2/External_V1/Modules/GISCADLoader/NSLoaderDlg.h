/*!
 @Class			CNSLoaderDlg
 @Brief			This class provides the UI functionality to perform
				dwg to xml conversion.
 @Author		Neilsoft Ltd
 @Date			12-07-2007
 @History 		Change history
*/
#pragma once
#include "..\..\Include\nsconfigfilemgr.h"
#include <sys/stat.h>
#include "NSTabPageSSL.h"
#include "afxwin.h"
#include "afxcmn.h"

// CNSLoaderDlg dialog
class CNSLoaderDlg : public CTabPageSSL
{
public:
	CNSLoaderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNSLoaderDlg();
	HICON m_hIcon;
	//std::string CStringToSTDStr(const CString& theCStr);
	afx_msg void OnBnClickedBtnAddFiles();
	afx_msg void OnBnClickedBtnRemoveFiles();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnEnChangeEditXmlpath();
	BOOL GetFolderSelection(HWND hWnd, LPTSTR szBuf);
	afx_msg void OnLbnSelchangeLstDwgfiles();
	afx_msg BOOL OnInitDialog();
	BOOL EnhanceLISTBOX( HINSTANCE hInstance );
	afx_msg void OnBnClickedBtnConvert();
	afx_msg void OnBnClickedBtnHelp();
	//afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedBtnValidate();
	afx_msg void disableControls(int value, BOOL bShow);
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedChkAssetvalidation();
	CWnd *CWnd_GetNextDlgGroupItemEx(CWnd *pCtrlWnd);
	afx_msg void OnEnSetfocusEditXmlpath();
	afx_msg void showProgressBar();
	CString getSummaryPath();
	void unselectListCtrl();
	bool checkErrorBoxChanged(NSSTRING& dlgErrorMsgs, NSSTRING& fileErrorMsgs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	bool UpdateReportLocation(std::wstring m_strFilename);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:
	DWORD ExecuteConverterApp(TCHAR* strFunct);
	enum { IDD = IDD_LOADER };
	ConfigFile* m_pConfigFile;

	//Declaration of variables
	CButton m_ChkAssetNumber;
	//CEdit m_editHost;
	CEdit m_editUser;
	CEdit m_editPassword;
	CEdit m_editDistrict;
	CButton m_Remove;
	CButton m_Add;
	CButton m_Browse;
	CListCtrl m_ListCtrl;
	CStatic m_ellipse;
	CProgressCtrl m_ProgressBar;
	CStatic m_FileName;
	CEdit m_PathName;
	CEdit m_Position;
	CStatic m_StaticHost;
	CStatic m_StaticUser;
	CStatic m_StaticPassword;
	CStatic m_StaticPosition;
	CStatic m_StaticDistrict;
	CComboBox m_Host;
    std::map<NSSTRING,NSSTRING> m_hostIDs;
	CToolTipCtrl* m_pToolTip;
	int isDirectoryExists(TCHAR *szFileName);
	afx_msg void OnCbnSelchangeCombo1Host();

	CButton m_ChkCadValid;
	CButton m_ChkGisValid;
	CButton m_ChkP1Errors;
	CButton m_ChkP2Errors;
	CButton m_ChkP3Errors;
	CButton m_ChkP4Errors;
};
