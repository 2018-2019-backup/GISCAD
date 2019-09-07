// AssetNumberingUtilityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "time.h"
#include "windows.h"
#include "AssetNumberingUtilityDlg.h"
#include "EndDialog.h"
#include "..\..\Include\NSReportWriter.h"
#include "..\..\include\NSDWGmgr.h"
#include "..\..\Include\NSValidator.h"

#define WriteToLog(str,err) CNSReportWriter::getReportWriterInstance()->writeLog(str,err)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern IAssetMgr* gAssetMgr;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT\

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAssetNumberingUtilityDlg dialog

CAssetNumberingUtilityDlg::CAssetNumberingUtilityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAssetNumberingUtilityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAssetNumberingUtilityDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_szHost = new TCHAR[_MAX_PATH + 1];
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bFlag = false;
	m_pToolTip = NULL;
	fpDebugLog = fopen("AssetDebug1.log","w");
}

CAssetNumberingUtilityDlg::~CAssetNumberingUtilityDlg()
{
	delete m_pToolTip;
	m_pToolTip = NULL;
	delete[] m_szHost;
	m_szHost = NULL;
}

void CAssetNumberingUtilityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssetNumberingUtilityDlg)
	DDX_Control(pDX, IDC_LIST1, m_assetsListCtrl);
	DDX_Control(pDX, IDC_EDIT5, m_district);
	DDX_Control(pDX, IDC_EDIT4, m_password);
	DDX_Control(pDX, IDC_EDIT3, m_user);
	//DDX_Control(pDX, IDC_EDIT2, m_connectionInfo);
	//DDX_Control(pDX, IDC_EDIT1, m_hostNameID);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON2, m_btnGenerate);
	DDX_Control(pDX, IDC_COMBO1, m_hostID);
	DDX_Control(pDX, IDC_EDIT1, m_position);
}

BEGIN_MESSAGE_MAP(CAssetNumberingUtilityDlg, CDialog)
	//{{AFX_MSG_MAP(CAssetNumberingUtilityDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, OnHelp)
	ON_BN_CLICKED(IDC_BUTTON2, OnGenerateNumbers)
	ON_BN_CLICKED(IDC_BUTTON4, OnCancel)
	ON_BN_CLICKED(IDC_BUTTON1, OnScan)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAssetNumberingUtilityDlg message handlers

BOOL CAssetNumberingUtilityDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
		TRACE("Unable To create ToolTip\n");
		return TRUE;
	}
	
	// set default value to District
	m_district.SetWindowText(_T("INTG"));
	// Read Host IDs from ini file.

	TCHAR szAppPath[MAX_PATH+1];
	GetModuleFileName(_hdllInstance,szAppPath,MAX_PATH+1);
	CString strAppPath = szAppPath;
	int nIndex = strAppPath.ReverseFind('\\');
	if(nIndex != -1)
	{
		strAppPath = strAppPath.Left(nIndex);
	}
	strAppPath = strAppPath + "\\settings.ini";
	TCHAR *szSettingsFile = new TCHAR[1000];
	//strcpy(szSettingsFile, (TCHAR *)strAppPath.GetBuffer(0));
	NSSTRCPY(szSettingsFile,(int)NSSTRLEN(strAppPath.GetBuffer(0)) + 1 ,(TCHAR *)strAppPath.GetBuffer(0));
	m_pConfigFile = new ConfigFile(szSettingsFile);

	std::map<NSSTRING,NSSTRING> m_mapofURL;
	std::map<NSSTRING,NSSTRING>::iterator urlItr;
	m_pConfigFile->updateURLMap(m_mapofURL, _T("ELL"));
	for(urlItr = m_mapofURL.begin(); urlItr != m_mapofURL.end(); urlItr++)
	{
		m_hostID.AddString(((*urlItr).first).c_str());
		m_hostIDs.insert(std::map<NSSTRING, NSSTRING>::value_type((*urlItr).first, (*urlItr).second));
	}
	m_hostID.SetCurSel(0);
	m_assetsListCtrl.InsertColumn(0, _T("Description"), LVCFMT_LEFT, 135);
	m_assetsListCtrl.InsertColumn(1, _T("Value"), LVCFMT_LEFT, 132);
	/*if(NSSTRCMP(m_strCAPno.c_str(), _T("")) == 0 ||
		NSSTRCMP(m_strDwgNo.c_str(), _T("")) == 0 ||
		NSSTRCMP(m_strProjNo.c_str(), _T("")) == 0 ||
		NSSTRCMP(m_strProjectType.c_str(), _T("")) == 0 ||
		NSSTRCMP(m_strDepotCode.c_str(), _T("")) == 0)
	{
		m_btnGenerate.EnableWindow(false);
	}*/
	CString strItem;
	m_hostID.GetWindowText(strItem);
	m_pToolTip->AddTool(&m_hostID, strItem);
	gAssetMgr = createAssetMgr();
	CNSDWGMgr localDwgMgr;
	localDwgMgr.setDatabase(acdbHostApplicationServices()->workingDatabase());
	gAssetMgr->setDatabase(&localDwgMgr);
	CNSTitleBlkInfo blkInfo;
	gAssetMgr->scanTitleBlock(blkInfo);
	setCAPNumber(blkInfo.m_szCAP_NO);
	setDrawingNumber(blkInfo.m_szDrawingNumber);
	setProjectNumber(blkInfo.m_szProjectNumber);
	setProjectType(blkInfo.m_szProjectType);
	setDepotCode(blkInfo.m_szDepotCode);

	m_password.SetPasswordChar('*');
	//m_position.SetPasswordChar('*');
	m_btnGenerate.EnableWindow(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAssetNumberingUtilityDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

BOOL CAssetNumberingUtilityDlg::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != m_pToolTip)
	{
		m_pToolTip->RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);

}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAssetNumberingUtilityDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAssetNumberingUtilityDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAssetNumberingUtilityDlg::OnCancel() 
{
	if(fpDebugLog !=NULL)
		fclose(fpDebugLog);
	EndDialog(IDCANCEL);
	//this->DestroyWindow();
}

void CAssetNumberingUtilityDlg::OnHelp() 
{
	showHelp();	
}

void CAssetNumberingUtilityDlg::OnGenerateNumbers() 
{
	try
	{
		UpdateData();
		if(fpDebugLog!=NULL)
		{
			fprintf(fpDebugLog,"In OnGenerateNumbers function\n");
			CString strUsr;
			m_user.GetWindowText(strUsr);
			fprintf(fpDebugLog,"Username = %s\n",strUsr);
			CString strPwd;
			m_password.GetWindowText(strPwd);
			fprintf(fpDebugLog,"Password = %s\n",strPwd);
		}
		TCHAR* szProjectType = (TCHAR*)m_strProjectType.c_str();
		if(NSSTRCMP(m_strProjectType.c_str(), _T("")) == 0)
		{
			MessageBox(_T("Missing value for ProjectType in Title block"), _T("AssetNumbering Utility"));
			WriteToLog(LOG_OTHER, _T("Missing value for ProjectType in Title block"));
			CNSReportWriter::getReportWriterInstance()->release();
			return;
		}
		
		if(NSSTRCMP(m_strDwgNo.c_str(), _T("")) == 0)
		{
			MessageBox(_T("Missing value for DrawingNumber in Title block"), _T("AssetNumbering Utility"));
			WriteToLog(LOG_OTHER, _T("Missing value for DrawingNumber in Title block"));
			CNSReportWriter::getReportWriterInstance()->release();
			return;
		}
		if(NSSTRCMP(m_strDepotCode.c_str(), _T("")) == 0)
		{
			MessageBox(_T("Missing value for DepotCode in Title block"), _T("AssetNumbering Utility"));
			WriteToLog(LOG_OTHER, _T("Missing value for DepotCode in Title block"));
			CNSReportWriter::getReportWriterInstance()->release();
			return;
		}
		else if(!gAssetMgr->IsValidDepotCode(m_strDepotCode))
		{
			MessageBox(_T("Invalid value for DepotCode in Title block"), _T("AssetNumbering Utility"));
			WriteToLog(LOG_OTHER, _T("Invalid value for DepotCode in Title block"));
			CNSReportWriter::getReportWriterInstance()->release();
			return;
		}
		NSSTRUPR(szProjectType,(int)NSSTRLEN(szProjectType));
		if(NSSTRCMP(m_strProjectType.c_str(), _T("INTERNAL")) == 0)
		{
			if(NSSTRCMP(m_strProjNo.c_str(), _T("")) == 0)
			{
				MessageBox(_T("Missing value for ProjectNumber in Title block"), _T("AssetNumbering Utility"));
				WriteToLog(LOG_OTHER, _T("Missing value for ProjectNumber in Title block"));
				CNSReportWriter::getReportWriterInstance()->release();
				return;
			}
		}
		else if(NSSTRCMP(m_strProjectType.c_str(), _T("EXTERNAL")) == 0)
		{
			if(NSSTRCMP(m_strCAPno.c_str(), _T("")) == 0)
			{
				MessageBox(_T("Missing value for CAP_NO in Title block"), _T("AssetNumbering Utility"));
				WriteToLog(LOG_OTHER, _T("Missing value for CAP_NO in Title block"));
				CNSReportWriter::getReportWriterInstance()->release();
				return;
			}
		}
		NSSTRING szAssets[10] = {_T("Pole"), _T("Column"), _T("LVPillar"), _T("Streetlight"), _T("SLCP"), _T("Cubicle"), 
				_T("LVServicePillar"), _T("Switch"), _T("Substation"), _T("SubSwitch")};
		writeHeaderInformation();
		TCHAR szFormat[_MAX_PATH + 1];
		int nSize = 0;
		/*if(!m_bFlag)
		{*/
			WriteToLog(LOG_HEADER, _T("ASSET TYPE                                       NUMBER OF ASSETS FOUND"));
			WriteToLog(LOG_HEADER, _T("-----------------------------------------------------------------------------------------------"));
		/*}*/
		for(int nCntr = 0; nCntr < 10; nCntr++)
		{
			nSize = gAssetMgr->getNumOfAssets(ASSET_TYPE(nCntr));
			TCHAR szAssetsCnt[10];
			NSSPRINTF(szAssetsCnt, _T("%d"), nSize);
			//int nLen = strlen(szAssets[nCntr].c_str());
			int nLen = (int)NSSTRLEN(szAssets[nCntr].c_str());
			NSSPRINTF(szFormat, _T(" %s%50d"),  szAssets[nCntr].c_str(), nSize);
			/*if(!m_bFlag)*/
				WriteToLog(LOG_HEADER, szFormat);
		}

		/*if(!m_bFlag)*/
			WriteToLog(LOG_HEADER, _T("-----------------------------------------------------------------------------------------------"));
		

		CString strUserName;
		m_user.GetWindowText(strUserName);
		m_strUserName = strUserName;
		CString strPwd;
		m_password.GetWindowText(strPwd);
		CString strDistrict;
		m_district.GetWindowText(strDistrict);
		CString strPosition;
		m_position.GetWindowText(strPosition);

		if(_tcscmp(strUserName, _T("")) == 0)
		{
			MessageBox(_T("Please enter UserName"), _T("AssetNumbering Utility"));
			m_user.SetFocus();
			CNSReportWriter::getReportWriterInstance()->release();
			return;
		}
		
		else if(_tcscmp(strPwd, _T("")) == 0)
		{
			MessageBox(_T("Please enter Password"), _T("AssetNumbering Utility"));
			m_password.SetFocus();
			CNSReportWriter::getReportWriterInstance()->release();
			return;
		}
		
		else if(_tcscmp(strDistrict, _T("")) == 0)
		{
			MessageBox(_T("Please enter District"), _T("AssetNumbering Utility"));
			m_district.SetFocus();
			CNSReportWriter::getReportWriterInstance()->release();
			return;
		}
		CString strHostID;
		m_hostID.GetWindowText(strHostID);
		CNSUserCredentials userCredentials;
		NSSTRCPY(userCredentials.m_szUserName, strUserName.GetBuffer(0));
		NSSTRCPY(userCredentials.m_szPassword, strPwd.GetBuffer(0));
		NSSTRCPY(userCredentials.m_szDistrict, strDistrict.GetBuffer(0));
		NSSTRCPY(userCredentials.m_szPosition, strPosition.GetBuffer(0));
		NSSTRING strKey(strHostID.GetBuffer(0));
		std::map<NSSTRING, NSSTRING>::iterator it = m_hostIDs.find(strKey);
		if(it != m_hostIDs.end())
		{
			NSSTRCPY(userCredentials.m_szHostID, ((*it).second).c_str() );
		}
		m_btnGenerate.EnableWindow(false);
		gAssetMgr->setUserCredentials(userCredentials);

		if(fpDebugLog!=NULL)
			fprintf(fpDebugLog,"Before generateNumbers function\n");
		int iRet = gAssetMgr->generateNumbers();
		if(fpDebugLog!=NULL)
			fprintf(fpDebugLog,"After generateNumbers function\n");
		CEndDialog dlgEnd;
		dlgEnd.setErrorFlag(gAssetMgr->GetErrorsOccuredFlag());
		if(dlgEnd.DoModal()!=IDOK)
		{
		//EndDialog(IDCANCEL);
		/*if(iRet == NS_SUCCESS)
		{*/
			CNSReportWriter::getReportWriterInstance()->writeReport(REPORT_TXT, (TCHAR*)m_strLogFileLoc.c_str());
			CNSReportWriter::getReportWriterInstance()->release();
			ShellExecute(m_hWnd, _T("open"), (TCHAR*)m_strLogFileLoc.c_str(), _T(""), _T("C:\\"), SW_SHOWNORMAL);
		}
		/*}*/
		m_btnGenerate.EnableWindow(false);
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON1);
		if(pButton)
			pButton->SetFocus();
		
	}
	catch(...)
	{
		if(fpDebugLog !=NULL)
			fclose(fpDebugLog);
	}
}

void CAssetNumberingUtilityDlg::OnScan() 
{
	if(fpDebugLog!=NULL)
		fprintf(fpDebugLog,"In scan function\n");
	if(!m_bFlag)
	{
		if(fpDebugLog!=NULL)
			fprintf(fpDebugLog,"m_bFlag is true\n");

		ConfigFile* m_pConfigFile = new ConfigFile((TCHAR*)m_strConfigLoc.c_str());
		TCHAR szValue[_MAX_PATH + 1];
		if(m_pConfigFile->readString(szValue,_T("LOCAL_MDB_LOCATION")))
		{
			CNSValidator validator;
			bool RetValue = validator.createDSN(szValue);
			if(RetValue)
			{
				//WriteToLog(LOG_WARNINGS, "DSN added successfully");
				m_bFlag = true;
			}
		
		}
		else
		{
			MessageBox(_T("Invalid database path in settings.ini. Please check LOCAL_MDB_LOCATION."));
			WriteToLog(LOG_OTHER, _T("Database Location not found in the Configuration File"));
			return;
		}
	}

	/*gAssetMgr = createAssetMgr();
	CNSDWGMgr localDwgMgr;
	localDwgMgr.setDatabase(acdbHostApplicationServices()->workingDatabase());
	gAssetMgr->setDatabase(&localDwgMgr);
	CNSTitleBlkInfo blkInfo;
	gAssetMgr->scanTitleBlock(blkInfo);*/
	/*setCAPNumber(blkInfo.m_szCAP_NO);
	setDrawingNumber(blkInfo.m_szDrawingNumber);
	setProjectNumber(blkInfo.m_szProjectNumber);
	setProjectType(blkInfo.m_szProjectType);
	setDepotCode(blkInfo.m_szDepotCode);*/
	if(fpDebugLog!=NULL)
		fprintf(fpDebugLog,"B4 scan function\n");
	gAssetMgr->scan();
	if(fpDebugLog!=NULL)
		fprintf(fpDebugLog,"After scan function\n");
	m_assetsListCtrl.DeleteAllItems();
	int nItem = m_assetsListCtrl.InsertItem(0, _T("CAP Number"));
	m_assetsListCtrl.SetItemText(nItem, 1, m_strCAPno.c_str());

	nItem = m_assetsListCtrl.InsertItem(1, _T("Drawing Number"));
	m_assetsListCtrl.SetItemText(nItem, 1, m_strDwgNo.c_str());

	nItem = m_assetsListCtrl.InsertItem(2, _T("Project Number"));
	m_assetsListCtrl.SetItemText(nItem, 1, m_strProjNo.c_str());

	TCHAR szFormat[_MAX_PATH + 1];
	/*if(!m_bFlag)
	{
		WriteToLog(LOG_HEADER, _T("ASSET TYPE                                       NUMBER OF ASSETS FOUND"));
		WriteToLog(LOG_HEADER, _T("-----------------------------------------------------------------------------------------------"));
	}*/
	
	int nIndex = 2;
	int nSize = 0;
	NSSTRING szAssets[10] = {_T("Poles found"), _T("Columns found"), _T("LVPillars found"),
		_T("Streetlights found"), _T("SLCPs found"), _T("Cubicles found"), 
			_T("LVServicePillars found"), _T("Switches found"), _T("Substations found"), _T("SubSwitches found")};

	for(int nCntr = 0; nCntr < 10; nCntr++)
	{
		nSize = gAssetMgr->getNumOfAssets(ASSET_TYPE(nCntr));
		TCHAR szAssetsCnt[10];
		NSSPRINTF(szAssetsCnt, _T("%d"), nSize);
		NSSPRINTF(szFormat, _T(" %s                                                   %d"),  szAssets[nCntr].c_str(), nSize);
		/*if(!m_bFlag)
			WriteToLog(LOG_HEADER, szFormat);*/
		if(nSize!= 0)
		{
			nIndex = nIndex + 1;
			nItem = m_assetsListCtrl.InsertItem(nIndex, szAssets[nCntr].c_str());
			m_assetsListCtrl.SetItemText(nItem, 1, szAssetsCnt);
		}
		
	}

	if(fpDebugLog!=NULL)
		fprintf(fpDebugLog,"End of scanning function\n");
	m_btnGenerate.EnableWindow(true);
	
	/*if(!m_bFlag)
		WriteToLog(LOG_HEADER, _T("-----------------------------------------------------------------------------------------------"));*/
	/*if(NSSTRCMP(m_strCAPno.c_str(), _T("")) == 0 ||
		NSSTRCMP(m_strDwgNo.c_str(), _T("")) == 0 ||
		NSSTRCMP(m_strProjNo.c_str(), _T("")) == 0 ||
		NSSTRCMP(m_strProjectType.c_str(), _T("")) == 0||
		NSSTRCMP(m_strDepotCode.c_str(), _T("")) == 0)
	{
		m_btnGenerate.EnableWindow(false);
	}
	else
	{
		m_btnGenerate.EnableWindow(true);
	}*/
	/*m_bFlag = true;*/
}

void CAssetNumberingUtilityDlg::writeHeaderInformation()
{
	const ACHAR* szDwgName;
	TCHAR szUserName[MAX_COMPUTERNAME_LENGTH + 1];  
	DWORD dwBufferSize = MAX_COMPUTERNAME_LENGTH + 1;  
	if( !GetUserName(szUserName, &dwBufferSize))
	{
		NSSTRCPY(szUserName, _T(""));
	}
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getFilename(szDwgName);
	TCHAR szFormat[_MAX_PATH + 1];
	NSSPRINTF(szFormat, _T("User Name                             : %s "), szUserName);
	WriteToLog(LOG_HEADER, szFormat);
	TCHAR szDate[_MAX_PATH + 1];
	TCHAR szTime[_MAX_PATH + 1];
	_strdate((char *)szDate);
	_strtime((char *)szTime);
	NSSPRINTF(szFormat, _T("Report Creation date and time         : %s %s "), szDate, szTime);
	WriteToLog(LOG_HEADER, szFormat);
	NSSPRINTF(szFormat, _T("Drawing Name                          : %s "), szDwgName);
	WriteToLog(LOG_HEADER, szFormat);
	NSSPRINTF(szFormat, _T("CAP Number                            : %s "), m_strCAPno.c_str());
	WriteToLog(LOG_HEADER, szFormat);
	NSSPRINTF(szFormat, _T("Drawing Number                        : %s "), m_strDwgNo.c_str());
	WriteToLog(LOG_HEADER, szFormat);
	NSSPRINTF(szFormat, _T("Project Number                        : %s "), m_strProjNo.c_str());
	WriteToLog(LOG_HEADER, szFormat);
	NSSPRINTF(szFormat, _T("Project Type                          : %s "), m_strProjectType.c_str());
	WriteToLog(LOG_HEADER, szFormat);
	WriteToLog(LOG_HEADER, _T("-----------------------------------------------------------------------------------------------"));
}
void CAssetNumberingUtilityDlg::setCAPNumber(const NSSTRING strCAPno)
{
	m_strCAPno = strCAPno;
}

void CAssetNumberingUtilityDlg::setDrawingNumber(const NSSTRING strDwgNo)
{
	m_strDwgNo = strDwgNo;
}

void CAssetNumberingUtilityDlg::setProjectNumber(const NSSTRING strProjNo)
{
	m_strProjNo = strProjNo;
}

void CAssetNumberingUtilityDlg::setProjectType(const NSSTRING strUserStatus)
{
	m_strProjectType = strUserStatus;
}

void CAssetNumberingUtilityDlg::setDepotCode(const NSSTRING strDepotCode)
{
	m_strDepotCode = strDepotCode;
}
void CAssetNumberingUtilityDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	CString strItem;
	m_hostID.GetWindowText(strItem);
	m_pToolTip->AddTool(&m_hostID, strItem);
}

void CAssetNumberingUtilityDlg::setConfigLocation(const NSSTRING strLoc)
{
	m_strConfigLoc = strLoc;
}

void CAssetNumberingUtilityDlg::setLogFileLocation(const NSSTRING strLoc)
{
	m_strLogFileLoc = strLoc;
}
BOOL CAssetNumberingUtilityDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: Add your message handler code here and/or call default
	showHelp();
	return CDialog::OnHelpInfo(pHelpInfo);
}

void CAssetNumberingUtilityDlg::showHelp()
{
	TCHAR szAppPath[MAX_PATH+1];
	GetModuleFileName(_hdllInstance,szAppPath,MAX_PATH+1);
	CString strAppPath = szAppPath;
	int nIndex = strAppPath.ReverseFind('\\');
	if(nIndex != -1)
	{
		strAppPath = strAppPath.Left(nIndex);
	}
    strAppPath.Append(_T("\\Help\\AssetNum.chm"));
	::HtmlHelp(this->m_hWnd,strAppPath,HH_DISPLAY_TOC,0);
}

