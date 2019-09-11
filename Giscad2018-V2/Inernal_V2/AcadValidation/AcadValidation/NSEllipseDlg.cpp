// NSEllipseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NSEllipseDlg.h"
#include "..\..\Include\NSValidatorIntface.h"
#include "..\..\Include\nsconfigfilemgr.h"
#include "..\..\include\NSDWGmgr.h"
#include "..\..\Include\NSCredentials.h"
#include "NSValidatorDlg.h"

// CNSEllipseDlg dialog 

CString gStrHTMLReportPath = "";

IMPLEMENT_DYNAMIC(CNSEllipseDlg, CDialog)
CNSEllipseDlg::CNSEllipseDlg(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CNSEllipseDlg::IDD, pParent)
{
	m_pConfigFile = NULL;
}

CNSEllipseDlg::~CNSEllipseDlg()
{
	delete m_pConfigFile; 
	m_pConfigFile = NULL;
}

void CNSEllipseDlg::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USER, m_User);
	DDX_Control(pDX, IDC_EDIT_PWD, m_Password);
	DDX_Control(pDX, IDC_EDIT_DIST, m_District);
	DDX_Control(pDX, IDC_CHECK1, m_assetCheck);
	DDX_Control(pDX, IDC_CHK_CADVALIDATION, m_ChkCadValid);
	DDX_Control(pDX, IDC_CHK_GISVALIDATION, m_ChkGisValid);
	DDX_Control(pDX, IDC_EDIT1_POSITION, m_Position);
	DDX_Control(pDX, IDC_COMBO1, m_Host);
    DDX_Control(pDX, IDC_CHK_P1_ERRORS, m_ChkP1Errors);
	DDX_Control(pDX, IDC_CHK_P2_ERRORS, m_ChkP2Errors);
	DDX_Control(pDX, IDC_CHK_P3_ERRORS, m_ChkP3Errors);
	DDX_Control(pDX, IDC_CHK_P4_ERRORS, m_ChkP4Errors);
}


BEGIN_MESSAGE_MAP(CNSEllipseDlg, CTabPageSSL)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CNSEllipseDlg message handlers

BOOL CNSEllipseDlg::OnInitDialog()
{
	CTabPageSSL::OnInitDialog();
	gStrHTMLReportPath = "";
	m_assetCheck.SetCheck(0);
	m_ChkCadValid.SetCheck(1);
	m_ChkGisValid.SetCheck(1);
	TCHAR szAppPath[MAX_PATH+1];
	GetModuleFileName(_hdllInstance,szAppPath,MAX_PATH+1);
	CString strAppPath = szAppPath;
	int nIndex = strAppPath.ReverseFind('\\');
	if(nIndex != -1)
	{
		strAppPath = strAppPath.Left(nIndex);
	}
	strAppPath = strAppPath + "\\settings.ini";
	NSSTRING szSettingsFile = strAppPath;
	m_pConfigFile = new ConfigFile((TCHAR*)szSettingsFile.c_str());
	//-------------- Reading checkBox status from .INI File.----
	if(m_pConfigFile != NULL)
	{
		TCHAR szValue[_MAX_PATH + 1];
		if(m_pConfigFile->readString(szValue, _T("P1")))
		{
			if(NSATOI(szValue))
			{
				m_ChkP1Errors.SetCheck(1);
			}
		}
		if(m_pConfigFile->readString(szValue, _T("P2")))
		{
			if(NSATOI(szValue))
			{
				m_ChkP2Errors.SetCheck(1);
			}
		}
		if(m_pConfigFile->readString(szValue, _T("P3")))
		{
			if(NSATOI(szValue))
			{
				m_ChkP3Errors.SetCheck(1);
			}
		}
		if(m_pConfigFile->readString(szValue, _T("P4")))
		{
			if(NSATOI(szValue))
			{
				m_ChkP4Errors.SetCheck(1);
			}
		}
	}
	//------------------------------------------------
	std::map<NSSTRING,NSSTRING> m_mapofURL;
	std::map<NSSTRING,NSSTRING>::iterator urlItr;
	m_pConfigFile->updateURLMap(m_mapofURL, _T("ELL"));
	for(urlItr = m_mapofURL.begin(); urlItr != m_mapofURL.end(); urlItr++)
	{
		m_Host.AddString(((*urlItr).first).c_str());
		m_hostIDs.insert(std::map<NSSTRING, NSSTRING>::value_type((*urlItr).first, (*urlItr).second));
	}
	m_Host.SetCurSel(0);
	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
		TRACE("Unable To create ToolTip\n");
		return TRUE;
	}

	CString strItem;
	m_Host.GetWindowText(strItem);
	m_pToolTip->AddTool(&m_Host, strItem);

	CString strDist = "INTG";
	m_District.SetWindowText(strDist);

	delete m_pConfigFile; 
    m_pConfigFile = NULL;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*!
 @Brief			Enables or disbales controls for asset numbering 
 @Date			13 Aug 2007
*/
void CNSEllipseDlg::EnableControls()
{
	bool bStatus = false;
	if(m_assetCheck.GetCheck()==1)
	{
		bStatus = true;
	}
	m_Host.EnableWindow(bStatus);
	m_User.EnableWindow(bStatus);
	m_Password.EnableWindow(bStatus);
	m_Position.EnableWindow(bStatus);
	m_District.EnableWindow(bStatus);
}

/*!
 @Brief			Asset numbering check box event handler
 @Date			13 Aug 2007
*/
void CNSEllipseDlg::OnBnClickedCheck1()
{
	EnableControls();
}

/*!
 @Brief			Event handler for "validate" button clicked
 @Date			13 Aug 2007
*/
void CNSEllipseDlg::Validate(CString szDwgPath)
{
	gStrHTMLReportPath = "";
	IValidator* pValidator = createValidator();
	pValidator->setGraphicIndication(1);
	TCHAR strStandardDrawing[_MAX_PATH + 1];

	/*TCHAR szAppPath[MAX_PATH+1];
	GetModuleFileName(_hdllInstance,szAppPath,MAX_PATH+1);
	CString strAppPath = szAppPath;
	int nIndex = strAppPath.ReverseFind('\\');
	if(nIndex != -1)
	{
		strAppPath = strAppPath.Left(nIndex);
	}
	strAppPath = strAppPath + "\\settings.ini";
	NSSTRING szSettingsFile = strAppPath;
	ConfigFile* pConfig = new ConfigFile((TCHAR*)szSettingsFile.c_str());
	if(pConfig == NULL)
	{
		AfxMessageBox(_T("Missing settings.ini file"));
		delete pValidator;
		return;
	}*/

	// Read .ini file path 
    TCHAR szAppPath[MAX_PATH+1];
	GetModuleFileName(_hdllInstance,szAppPath,MAX_PATH+1);
	CString strAppPath = szAppPath;
	int nIndex = strAppPath.ReverseFind('\\');
	if(nIndex != -1)
	{
		strAppPath = strAppPath.Left(nIndex);
	}
	strAppPath = strAppPath + "\\settings.ini";
	NSSTRING szSettingsFile = strAppPath;
	if(NSACCESS(szSettingsFile.c_str(), 0) != -1)  //if .ini file is present
	{
        m_pConfigFile = new ConfigFile((TCHAR*)szSettingsFile.c_str());

		m_pConfigFile->readString(strStandardDrawing,_T("SRVR_STD_DWG"));

		pValidator->setStdDWGFilePath(strStandardDrawing);

		NSSTRING strCurrentDwg = szDwgPath;
		pValidator->setDWGFilePath((TCHAR*)strCurrentDwg.c_str());
		CNSDWGMgr localDwgMgr;
		localDwgMgr.setDatabase(acdbHostApplicationServices()->workingDatabase());
		pValidator->setDatabase(&localDwgMgr);

		m_pConfigFile->readString(strStandardDrawing,_T("REPORT_LOCATION"));
		pValidator->setReportFilePath(strStandardDrawing);

		TCHAR szValue[_MAX_PATH + 1];
		if(m_pConfigFile->readString(szValue,_T("RELATION_FLAG")))
		{
			int nRelFlag = NSATOI(szValue);
			pValidator->setRelationshipFlag(nRelFlag);
		}
		//-- Read Connectivity Flag from .INI File. --
		if(m_pConfigFile->readString(szValue,_T("GIS_FLAG")))
		{
			int nGisFlag = NSATOI(szValue);
			pValidator->setConnctivityFlag(nGisFlag); //Set GIS_Flag to CNSBaseValidator class
		}	
		//-----------------------------------------

		pValidator->setGraphicIndication(1);
	
		double dSymbolFactor = 5.0;
		if(m_pConfigFile->readString(szValue,_T("SEARCH_SYMBOL_FACTOR")))
		{
			dSymbolFactor = NSATOF(szValue); 
		}

		if(m_pConfigFile->readString(szValue,_T("SEARCH_AREA_FACTOR")))
		{
			double dSearchAreaFactor = NSATOF(szValue);
			pValidator->setSearchAreaFactor(dSearchAreaFactor, dSymbolFactor);
		}
		if(m_pConfigFile->readString(szValue,_T("BOND_SEARCH_RADIUS")))
		{
			double dSearchAreaRadius = NSATOF(szValue);
			pValidator->setSearchAreaRadius(dSearchAreaRadius);
		}
		if(m_pConfigFile->readString(szValue,_T("TOLERANCE")))
		{
			double dTolerance = NSATOF(szValue);
			pValidator->setTolerance(dTolerance);
		}
		if(m_pConfigFile->readString(szValue,_T("PRECISION")))
		{
			pValidator->setPrecision(NSATOI(szValue));
		}
	
		if(m_pConfigFile->readString(szValue,_T("LOCAL_MDB_LOCATION")))
		{
			pValidator->setMDBLocation(szValue);
		}

		if(m_pConfigFile->readString(szValue,_T("SRVR_MDB_LOCATION")))
		{
			pValidator->setMDBLocation(szValue, true);
		}
		if(m_pConfigFile->readString(szValue,_T("LINEAR_FEATURE_ON_DIFFERENT_LAYER")))
		{
			int nFlag = NSATOI(szValue);
			pValidator->setLinearConnOnLayerFlag(nFlag);
		}

		if(m_pConfigFile->readString(szValue,_T("TEMPLATE_VERSION")))
		{
			double version = NSATOF(szValue);
			pValidator->setTemplateVersion(version);
		}

		if(m_ChkCadValid.GetCheck()==1)
		{
			pValidator->setCadValidationFlag(true);
		}
		else
		{
			pValidator->setCadValidationFlag(false);
		}

		if(m_ChkGisValid.GetCheck()==1)
		{
			pValidator->setGisValidationFlag(true);
		}
		else
		{
			pValidator->setGisValidationFlag(false);
		}

		if(m_assetCheck.GetCheck()==1)
		{
			pValidator->setAssetValidationFlag(true);

			CString strHostID;
			m_Host.GetWindowText(strHostID);

			CString strUserName;
			m_User.GetWindowText(strUserName);

			//m_strUserName = strUserName;
			CString strPwd;
			m_Password.GetWindowText(strPwd);

			CString strDistrict;
			m_District.GetWindowText(strDistrict);

			CString strPosition;
			m_Position.GetWindowText(strPosition);

			//if(strcmp(strUserName, "") == 0)
			if(strUserName.Compare(_T(""))==0)
			{
				MessageBox(_T("Please enter UserName"), _T("Design Validation"));
				m_User.SetFocus();
				return;
			}
		
			else if(_tcscmp(strPwd, _T("")) == 0)
			{
				MessageBox(_T("Please enter Password"), _T("Design Validation"));
				m_Password.SetFocus();
				return;
			}
		
			else if(_tcscmp(strDistrict, _T("")) == 0)
			{
				MessageBox(_T("Please enter District"), _T("Design Validation"));
				m_District.SetFocus();
				return;
			}
		
			CNSUserCredentials userCredentials;
			NSSTRCPY(userCredentials.m_szUserName, strUserName.GetBuffer(0));
			NSSTRCPY(userCredentials.m_szPassword, strPwd.GetBuffer(0));
			NSSTRCPY(userCredentials.m_szDistrict, strDistrict.GetBuffer(0));
			NSSTRCPY(userCredentials.m_szPosition, strPosition.GetBuffer(0));
			//NSSTRCPY(userCredentials.m_szHostID, strHostID.GetBuffer(0));
			NSSTRING strKey(strHostID.GetBuffer(0));
			std::map<NSSTRING, NSSTRING>::iterator it = m_hostIDs.find(strKey);
			if(it != m_hostIDs.end())
			{
				NSSTRCPY(userCredentials.m_szHostID, ((*it).second).c_str() );
			}
			pValidator->setUserCredentials(userCredentials);
		}
		else
		{
			pValidator->setAssetValidationFlag(false);
		}

		//---Set Check box status to CNSBaseValidator class --
		pValidator->setValidationRulesErrorFlag(m_ChkP1Errors.GetCheck(), m_ChkP2Errors.GetCheck(),
				                            m_ChkP3Errors.GetCheck(), m_ChkP4Errors.GetCheck());

		NSSTRING dlgErrorMsgs,fileErrorMsgs;
		//check error status of dialog and .ini file
		if(checkErrorBoxChanged(dlgErrorMsgs, fileErrorMsgs))  
		    m_pConfigFile->UpdateErrorMsgToFile(dlgErrorMsgs, fileErrorMsgs);
		//MessageBox(_T("Validation completed"), _T("CAD Validation before"), MB_ICONASTERISK);
		int nRetVal=-1;

		nRetVal = pValidator->validate();
		if(nRetVal == 0)
		{
			MessageBox(_T("Validation completed"), _T("CAD Validation"), MB_ICONASTERISK);
			//AfxMessageBox("Validation completed",MB_ICONASTERISK);
		}
		else
		{
			AfxMessageBox(_T("Validation failed"));
		}
		NSSTRING strHTMLPath;
		pValidator->getReportFilePath(strHTMLPath);
		gStrHTMLReportPath = strHTMLPath.c_str();
		delete pValidator;
	}
	else 
		MessageBox(_T("Config File not present."), _T("Design Validation"));
		//delete m_pConfigFile; 
}

/*!
 @Brief			Returns the HTML report path based on the title block 
 @Date			13 Aug 2007
*/
CString CNSEllipseDlg::GetHTMLReportPath()
{
	//return m_strHTMLReportPath;
	return "";
}

void CNSEllipseDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CNSValidatorDlg* validatorObj = (CNSValidatorDlg*)(this->GetParent());
	validatorObj->showWindow(true);
}

BOOL CNSEllipseDlg::PreTranslateMessage(MSG* pMsg)
{
      if (NULL != m_pToolTip)
      {
            m_pToolTip->RelayEvent(pMsg);
      }
      return CDialog::PreTranslateMessage(pMsg);

}

void CNSEllipseDlg::OnCbnSelchangeCombo1()
{
	CString strItem;
	m_Host.GetWindowText(strItem);
	m_pToolTip->AddTool(&m_Host, strItem);
}

/*!
 @Brief			Function checks error whether Error checkBox status is changged.
 @Param [in]	NSSTRING dlgErrorMsgs - status of error messages from GISCADLoader Dialog
                NSSTRING fileErrorMsgs - statue of error messages from .ini file
 @Param [in]	
 @Date			
*/

bool CNSEllipseDlg::checkErrorBoxChanged(NSSTRING& dlgErrorMsgs, NSSTRING& fileErrorMsgs)
{
	TCHAR id[20];
	NSSTRING tempString, tempString1;
	bool errorFlag = false;
	TCHAR szValue[_MAX_PATH + 1];
	NSITOW(m_ChkP1Errors.GetCheck(),id,20,10); 
	if(m_pConfigFile->readString(szValue,_T("P1")))
	{
		if(NSSTRCMP(szValue,id) != 0)
		   errorFlag = true;
		dlgErrorMsgs.append( _T("P1 = "));
		dlgErrorMsgs.append(id);
		dlgErrorMsgs = dlgErrorMsgs + _T(",");
		fileErrorMsgs.append( _T("P1 = "));
		fileErrorMsgs.append(szValue);
		fileErrorMsgs = fileErrorMsgs + _T(",");
		
	}
	NSITOW(m_ChkP2Errors.GetCheck(),id,20,10); 
	if(m_pConfigFile->readString(szValue,_T("P2")))
	{
		if(NSSTRCMP(szValue,id) != 0)
		   errorFlag = true;
		dlgErrorMsgs.append( _T("P2 = "));
		dlgErrorMsgs.append(id);
		dlgErrorMsgs = dlgErrorMsgs + _T(",");
		fileErrorMsgs.append( _T("P2 = "));
		fileErrorMsgs.append(szValue);
		fileErrorMsgs = fileErrorMsgs + _T(",");
	}
	NSITOW(m_ChkP3Errors.GetCheck(),id,20,10); 
	if(m_pConfigFile->readString(szValue,_T("P3")))
	{
		if(NSSTRCMP(szValue,id) != 0)
		   errorFlag = true;
		dlgErrorMsgs.append( _T("P3 = "));
		dlgErrorMsgs.append(id);
		dlgErrorMsgs = dlgErrorMsgs + _T(",");
		fileErrorMsgs.append( _T("P3 = "));
		fileErrorMsgs.append(szValue);
		fileErrorMsgs = fileErrorMsgs + _T(",");
	}
	NSITOW(m_ChkP4Errors.GetCheck(),id,20,10); 
	if(m_pConfigFile->readString(szValue,_T("P4")))
	{
		if(NSSTRCMP(szValue,id) != 0)
		   errorFlag = true;
		dlgErrorMsgs.append( _T("P4 = "));
		dlgErrorMsgs.append(id);
		dlgErrorMsgs = dlgErrorMsgs;
		fileErrorMsgs.append( _T("P4 = "));
		fileErrorMsgs.append(szValue);
		fileErrorMsgs = fileErrorMsgs;
	}
	return errorFlag;
}