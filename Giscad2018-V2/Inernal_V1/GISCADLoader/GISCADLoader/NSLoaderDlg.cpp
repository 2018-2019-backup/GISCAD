/*!
 @Class			CNSLoaderDlg
 @Brief			Implements the main functionality of converting from dwg to xml file and 
				validating the dwg file.
 @Author		Neilsoft Ltd
 @Date			16-07-2007
 @History 		Change history
*/
#include "string.h"
#include "stdafx.h"
#include "cderr.h" 
#ifdef VALIDATOR
#include "..\..\Include\NSValidatorIntface.h"
#endif
#include "GISCADLoader.h"
#include "NSLoaderDlg.h"
#include "GISCADLoaderDlg.h"
#include "shellapi.h"
#include "NSInputDlg.h"

#define SUCCESS 0
#define INVALID_DRAWING -1001
#define GIS_INVALID -1002
#define PARTIAL_SUCCESS -1003
#define FAIL 1


CString gstrReportPaths;
bool gValidate = false;

CNSLoaderDlg::CNSLoaderDlg(CWnd* pParent)
	: CTabPageSSL(CNSLoaderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CNSLoaderDlg::~CNSLoaderDlg()
{
}

void CNSLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_XMLPATH, m_PathName);
	DDX_Control(pDX, IDC_BTN_REMOVE_FILES, m_Remove);
	DDX_Control(pDX, IDC_CHK_ASSETVALIDATION, m_ChkAssetNumber);
	DDX_Control(pDX, IDC_EDIT_USER, m_editUser);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_EDIT_DISTRICT, m_editDistrict);
	DDX_Control(pDX, IDC_PROGRESS, m_ProgressBar);
	DDX_Control(pDX, IDC_FILE_NAME, m_FileName);
	DDX_Control(pDX, IDC_LIST_DWGFile, m_ListCtrl);
	DDX_Control(pDX, IDC_BTN_ADD_FILES, m_Add);
	DDX_Control(pDX, IDC_BTN_BROWSE, m_Browse);
	DDX_Control(pDX, IDC_ELLIPSEDETAIL, m_ellipse);
	DDX_Control(pDX, IDC_COMBO1_HOST, m_Host);
	DDX_Control(pDX, IDC_EDIT1_POSITION, m_Position);
	DDX_Control(pDX, IDC_LBL_HOST, m_StaticHost);
	DDX_Control(pDX, IDC_LBL_USER, m_StaticUser);
	DDX_Control(pDX, IDC_LBL_PASSWORD, m_StaticPassword);
	DDX_Control(pDX, IDC_STATIC_POSITION, m_StaticPosition);
	DDX_Control(pDX, IDC_LBL_DISTRICT, m_StaticDistrict);
	DDX_Control(pDX, IDC_CHK_CADVALIDATION, m_ChkCadValid);
	DDX_Control(pDX, IDC_CHK_GISVALIDATION, m_ChkGisValid);
	DDX_Control(pDX, IDC_CHK_P1_ERRORS, m_ChkP1Errors);
	DDX_Control(pDX, IDC_CHK_P2_ERRORS, m_ChkP2Errors);
	DDX_Control(pDX, IDC_CHK_P3_ERRORS, m_ChkP3Errors);
	DDX_Control(pDX, IDC_CHK_P4_ERRORS, m_ChkP4Errors);
}


BEGIN_MESSAGE_MAP(CNSLoaderDlg, CTabPageSSL)
	ON_BN_CLICKED(IDC_BTN_ADD_FILES, &CNSLoaderDlg::OnBnClickedBtnAddFiles)
	ON_BN_CLICKED(IDC_BTN_REMOVE_FILES, &CNSLoaderDlg::OnBnClickedBtnRemoveFiles)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CNSLoaderDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_CONVERT, &CNSLoaderDlg::OnBnClickedBtnConvert)
	//ON_BN_CLICKED(IDC_BTN_HELP, &CNSLoaderDlg::OnBnClickedBtnHelp)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_BTN_VALIDATE, &CNSLoaderDlg::OnBnClickedBtnValidate)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DWGFile, &CNSLoaderDlg::OnNMClickList2)
	ON_BN_CLICKED(IDC_CHK_ASSETVALIDATION, &CNSLoaderDlg::OnBnClickedChkAssetvalidation)
	ON_EN_SETFOCUS(IDC_EDIT_XMLPATH, &CNSLoaderDlg::OnEnSetfocusEditXmlpath)
	ON_CBN_SELCHANGE(IDC_COMBO1_HOST, &CNSLoaderDlg::OnCbnSelchangeCombo1Host)
END_MESSAGE_MAP()
// CNSLoaderDlg message handlers
/*
@Brief		At the start of the implementation the UI reads the xml file path from the
			config file and stores it in the edit box in UI and inserts 2 column name
			in the list control box.
*/
BOOL CNSLoaderDlg::OnInitDialog() 
{
	CTabPageSSL::OnInitDialog();
	m_ChkCadValid.SetCheck(1);
	m_ChkGisValid.SetCheck(1);
	
	#ifdef _DEBUG
		//m_ChkAssetNumber.SetCheck(1);		

		m_editUser.SetWindowTextW(_T("37037"));
		m_editPassword.SetWindowTextW(_T("gol1ath2"));
		m_Position.SetWindowTextW(_T(""));
	#endif // _DEBUG

    m_editDistrict.SetWindowTextW(_T("INTG"));
	//Read the default path to store xml files and set it
	// to the edit control
	TCHAR szApp_path[_MAX_PATH + 1];
	GetModuleFileName((HMODULE)AfxGetApp()->m_hInstance, szApp_path, MAX_PATH);
	CString strPath = szApp_path;
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	strPath+=_T("\\settings.ini");
	NSSTRING strFilename = (NSSTRING)strPath;
	//m_pConfigFile = new ConfigFile(_T("E:\\settings.ini"))

	//Creating tool tip in the combo box
	m_pToolTip = new CToolTipCtrl;
      if(!m_pToolTip->Create(this))
      {
            TRACE("Unable To create ToolTip\n");
			return TRUE;
      }
	CString strItem;
	m_Host.GetWindowText(strItem);
    m_pToolTip->AddTool(&m_Host, strItem);

 	if(NSACCESS(strFilename.c_str(), 0) != -1)
	{
		m_pConfigFile = new ConfigFile((TCHAR*)strFilename.c_str());
		if(m_pConfigFile != NULL)
		{
			TCHAR szValue[_MAX_PATH + 1];
			if(m_pConfigFile->readString(szValue,_T("SAVE_XML_FILES_LOCATION")))
			{
				m_PathName.SetWindowTextW(szValue);			
			}

			//----------Reading CheckBox status from .INI File --
			if(m_pConfigFile->readString(szValue,_T("P1")))
			{
				if(NSATOI(szValue))
					m_ChkP1Errors.SetCheck(1);
			}
			if(m_pConfigFile->readString(szValue,_T("P2")))
			{
				if(NSATOI(szValue))
					m_ChkP2Errors.SetCheck(1);
			}
			if(m_pConfigFile->readString(szValue,_T("P3")))
			{
				if(NSATOI(szValue))
					m_ChkP3Errors.SetCheck(1);
			}
			if(m_pConfigFile->readString(szValue,_T("P4")))
			{
				if(NSATOI(szValue))
					m_ChkP4Errors.SetCheck(1);
			}
			//---------------------------------------------
		}
	}

	//Create multiple column list control and code for selecting the whole row
	m_ListCtrl.InsertColumn(0,_T("DWG Files"), LVCFMT_LEFT, 300);
	m_ListCtrl.InsertColumn(1,_T("Status"), LVCFMT_LEFT, 90);
	
	ListView_SetExtendedListViewStyleEx(m_ListCtrl.m_hWnd,
										LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	m_ListCtrl.SetScrollRange(SB_HORZ, 0, 0);

	std::map<NSSTRING,NSSTRING> m_mapofURL;
	std::map<NSSTRING,NSSTRING>::iterator urlItr;
	m_pConfigFile->updateURLMap(m_mapofURL, _T("ELL"));
	for(urlItr = m_mapofURL.begin(); urlItr != m_mapofURL.end(); urlItr++)
	{
		m_Host.AddString(((*urlItr).first).c_str());
		m_hostIDs.insert(std::map<NSSTRING, NSSTRING>::value_type((*urlItr).first, (*urlItr).second));
	}

    m_Host.SetCurSel(0);

    delete m_pConfigFile; 
    m_pConfigFile = NULL;

	return TRUE;
}
/*
@brief		This function adds the dwg files in the list control and checks whether only
			dwg files are inserted and same file is not inserted again
@Param(in)	NULL
@Param(out)	NULL
*/
void CNSLoaderDlg::OnBnClickedBtnAddFiles()
{
	unselectListCtrl();

	//Sets the progress bar and file name to its initial position.
	m_FileName.SetWindowTextW(_T(" "));
	m_ProgressBar.SetPos(0);

	for(int nCntr = 0; nCntr < m_ListCtrl.GetItemCount(); nCntr++)
    { 
        m_ListCtrl.SetItemText(nCntr, 1, _T(" "));
		m_ListCtrl.Update(nCntr);
    }
	TCHAR szFilter[MAX_PATH + 1] = {_T("DWG Files (*.dwg)|*.dwg|")};
	CFileDialog FileDlg(TRUE, _T(".dwg"), NULL, OFN_ALLOWMULTISELECT,
						szFilter);
    FileDlg.m_ofn.lpstrFile = new TCHAR[10000]; 
    FileDlg.m_ofn.nMaxFile = 10000;				
    memset(FileDlg.m_ofn.lpstrFile, 0, 100);	 //Can add large no of files.

	int nItem = 0;
	if( FileDlg.DoModal() == IDOK)
	{
		POSITION position = FileDlg.GetStartPosition();
		//To add single or multiple files in the list control 
		while(position != NULL)
		{
			CString strPath = FileDlg.GetNextPathName(position);
			strPath.Replace(_T("\\\\"), _T("\\"));
			int nDot = strPath.ReverseFind('.');
			CString strPath1;
			CString strExt = strPath.Mid(nDot+1,strPath.GetLength()-nDot);
			//Check whether the file is .dwg
			if(strExt.MakeUpper().Compare(_T("DWG")) == 0)    
			{
				int nLen = strPath.GetLength();
				LPWSTR lpszBuf = strPath.GetBuffer(nLen);
				strPath.ReleaseBuffer();
				int flag = 0;
				//Check whether the file is already present
				for(int nFilecount = 0; nFilecount < m_ListCtrl.GetItemCount(); nFilecount++)
				{
					strPath1 = m_ListCtrl.GetItemText(nFilecount, 0);
					if(strPath.Compare(strPath1) == 0)
					{
						flag =1;
                        m_ListCtrl.SetItemText(nFilecount, 1, _T(" "));
						m_ListCtrl.Update(nFilecount);
						break;
					}
				}
				//If not present then add it in the list control
				if(flag == 0)
				{
					LVITEM LvItem={0};
					LvItem.mask = LVIF_TEXT ;
					LvItem.iItem = m_ListCtrl.GetItemCount();
					LvItem.iSubItem = 0;
					LvItem.pszText = lpszBuf;
					m_ListCtrl.InsertItem( &LvItem);
					nItem++;
				}
			
			}
		}	
		//Enable the convert and validate buttons
		if(nItem > 0)
		{
			BOOL bShow = 0;
			CGISCADLoaderDlg* obj = (CGISCADLoaderDlg*)(this->GetParent());
			obj->enableControls(!bShow);
		}
	}
	if(CommDlgExtendedError() == FNERR_BUFFERTOOSMALL)
      MessageBox(_T("To many selection"));
	delete []FileDlg.m_ofn.lpstrFile;
}
/* 
@brief		This function is to remove the selected dwg files from the list control 
@Param(in)	NULL
@Param(out)	NULL
*/
void CNSLoaderDlg::OnBnClickedBtnRemoveFiles()
{
	//Sets the progress bar and file name to its initial position.
	m_FileName.SetWindowTextW(_T(" "));
	m_ProgressBar.SetPos(0);
	
	int nRows = 0;
	int nFileCount = m_ListCtrl.GetSelectedCount();
	//Delete selected items 
	do
	{
		if(m_ListCtrl.GetItemState(nRows, LVIS_SELECTED) == LVIS_SELECTED)
		{
			m_ListCtrl.DeleteItem(nRows);	
			nFileCount--;
		}
		else
			nRows++;
	}while(nFileCount != 0);
	
	//If there are no files in list control then disable the Convert and Validate button
	//and remove the contents in the report tab and disable the remove button.
	if(m_ListCtrl.GetItemCount() == 0)
	{
		m_ListCtrl.SetColumnWidth(0, 300);
		m_ListCtrl.SetColumnWidth(1, 90);
		m_ListCtrl.SetScrollRange(SB_HORZ,0,0);
		m_Remove.EnableWindow(false);
		BOOL bShow = 1;
		CGISCADLoaderDlg* GISCADObj = (CGISCADLoaderDlg*)(this->GetParent());
		GISCADObj->enableControls(!bShow);
		GISCADObj->deleteReport();        
	}
	m_Remove.EnableWindow(false);
}
/*
@brief			This function opens the file dialog on clicking the browse button
@Param(in)		Handle
@Param(out)		The path where file is to be stored
return			Result
*/
BOOL CNSLoaderDlg::GetFolderSelection(HWND hWnd, LPTSTR szBuf)
{
	LPITEMIDLIST pidl     = NULL;
	BROWSEINFO   bi       = { 0 };
	BOOL         bResult  = FALSE;

	bi.hwndOwner      = hWnd;
	bi.pszDisplayName = szBuf;
	bi.pidlRoot       = NULL;
	bi.ulFlags        = BIF_RETURNONLYFSDIRS;

	if ((pidl = SHBrowseForFolder(&bi)) != NULL)
	{
		bResult = SHGetPathFromIDList(pidl, szBuf);
		CoTaskMemFree(pidl);
	}
	
	return bResult;
}
/*
@brief			Calls a fuction to dispaly the output path to store the file
@Param(in)		NULL
@Param(out)		The path where file is to be stored
*/
void CNSLoaderDlg::OnBnClickedBtnBrowse()
{
	unselectListCtrl();
	m_FileName.SetWindowTextW(_T(" "));
	m_ProgressBar.SetPos(0);
	TCHAR szFolder[100];
	if (GetFolderSelection(m_hWnd, szFolder))
		m_PathName.SetWindowTextW(szFolder);
}
/*
@brief		This function enables or disables the controls accordingly when the
			convert button is clicked
@Param(in)	Two values 'nValue' and 'bShow' to enable and disable controls
*/
void CNSLoaderDlg::disableControls(BOOL bValue, BOOL bShow)
{
	CGISCADLoaderDlg* obj = (CGISCADLoaderDlg*)(this->GetParent());
	if(obj!=NULL)
	{
		obj->enableControls(bShow);		//For convert and validate button.
		//obj->enableHelpExit(bShow);		//For help button.
	}
    
    if(m_ListCtrl.GetSelectedCount() != 0)
            m_Remove.EnableWindow(bValue);

	m_Add.EnableWindow(bValue);
	m_Browse.EnableWindow(bValue);
	m_PathName.EnableWindow(bValue);
}
/*
@brief	    Executes the GISCADConverter through the system
@Param(in)	The dwg file path and the xml file output path
@Param(out)	The return value of the converter.exe
*/
DWORD CNSLoaderDlg::ExecuteConverterApp(TCHAR* strFunct)
{
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
	ULONG rc;
	
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_FORCEONFEEDBACK;
	StartupInfo.wShowWindow = SW_HIDE;

	if (!CreateProcess( NULL, strFunct, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, 
		NULL, 
		NULL,
		&StartupInfo,
		&ProcessInfo))
	{
		return GetLastError();		
	}

	DWORD temp = GetLastError();	
	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	if(!GetExitCodeProcess(ProcessInfo.hProcess, &rc))
		rc = 0;

	CloseHandle(ProcessInfo.hThread);
	CloseHandle(ProcessInfo.hProcess);
	return rc;
}
/*
@brief      After the button is clicked dwg gets converted into xml files
@Param(in)  The dwg file that is to be converted
@Param(out) The converted output xml file 
*/
void CNSLoaderDlg::OnBnClickedBtnConvert()
{
	unselectListCtrl();
	int nflag = 0;
	bool bFocusFlag = false;
	bool bValue = 0;
	bool bShow = 0;
	bool bConvert = false;
	std::map<CString, int> mapofXMLPath;
	std::map<CString, int>::iterator XMLPathIter;

	gValidate = false;

	//Empty the second column of the list control.
	for(int nCntr = 0; nCntr < m_ListCtrl.GetItemCount(); nCntr++)	
    {															
        m_ListCtrl.SetItemText(nCntr, 1, _T(" "));	
		m_ListCtrl.Update(nCntr);
    }				

	int  nCount= m_ListCtrl.GetItemCount();						
	CString strSummaryFile = getSummaryPath();					
	DeleteFile(strSummaryFile);		

	//Set Progress bar to its initial position					
	m_ProgressBar.SetPos(0);		
	m_ProgressBar.SetStep(1);	
	int nProgressCount = 1;

	for(int nFileno = 0; nFileno < nCount; nFileno++)
	{
		// Getting output file path
		CString strSummaryFile,strOutputFile1, strOutputFile2 = NULL;
		m_PathName.GetWindowTextW(strOutputFile1);
		m_PathName.GetWindowTextW(strOutputFile2);
		//Get the application path
		TCHAR szApp_Path[_MAX_PATH + 1];
		GetModuleFileName((HMODULE)AfxGetApp()->m_hInstance, szApp_Path, MAX_PATH);
		CString strApp = szApp_Path;
			
		//Check if valid file name is given in the edit box.
		//Return 0 if valid else -1 if not valid
		int nretValue = isDirectoryExists(strOutputFile1.GetBuffer(0));
		//int nretValue = NSACCESS(strOutputFile1.GetBuffer(0), 0); 
		if(nretValue == NS_FAIL)
		{
			CString strMsg = _T("Invalid XMLOutput File Path : ") + strOutputFile1;
			int nRet = MessageBox(strMsg);
			bFocusFlag = true;
			break;
		}
		//Open the config file and modify the XML Path location acc to path given by user
		CString strConfigFile = szApp_Path;
		strConfigFile = strConfigFile.Left(strConfigFile.ReverseFind('\\'));
		strConfigFile+=_T("\\Settings.ini");
		NSSTRING strFilename = (NSSTRING)strConfigFile;
		if(NSACCESS(strFilename.c_str(), 0) != -1)
		{
			bool val= UpdateReportLocation(strFilename);
			/*m_pConfigFile = new ConfigFile((TCHAR*)strFilename.c_str());
			if(m_pConfigFile != NULL)
			{
				TCHAR szValue[_MAX_PATH + 1];
				m_pConfigFile->add(_T("SAVE_XML_FILES_LOCATION"),  (NSSTRING)strOutputFile1);
			}

			NSFSTREAM os(strFilename.c_str(), std::ios::out);	
			m_pConfigFile->GetFromStream(os);			
			os.close();		
			delete m_pConfigFile;*/
		}
		  
		// Getting fileName and displays the name of file that is getting converted
		CString strDwgPath;
		strDwgPath = m_ListCtrl.GetItemText(nFileno, 0);		
		int nSlash = strDwgPath.ReverseFind('\\');
		CString strExt = _T("Converting ") + strDwgPath.Mid(nSlash + 1, (strDwgPath.GetLength() - nSlash))+_T("......");
		m_FileName.SetWindowTextW(strExt);			
		
		//Show the progress on the progress bar and disable the other controls buttons
		disableControls(bValue, bShow);
		m_ProgressBar.SetRange32(0, nCount*2);					
		nflag = 0;

		m_PathName.GetWindowTextW(strOutputFile1);
		int nIndex = strOutputFile1.ReverseFind(_T('\\'));
		if(nIndex != -1 && (nIndex == strOutputFile1.GetLength() - 1))
			strOutputFile1.Append(_T("\\"));
		
		//Insert the xml filename along with its path in the map 
		CNSInputDLG inputDlg;
		int nPath = 0;
 		CString strName;
		//Get the dwg file name.
		int nStrno = strDwgPath.ReverseFind('\\');
		CString strDWGPath1 = strDwgPath.Mid(nStrno+1, strDwgPath.GetLength() - nStrno);
		strDWGPath1 = strDWGPath1.Left(strDWGPath1.ReverseFind('.'));
		strOutputFile2.Append(_T("\\"));
		//Append the dwg filename to the xml file path and then remove the '.dwg' from it.
		strOutputFile2.Append(strDWGPath1);

		//Check if same dwg file name with different path is there in list ctrl then
		//user should be asked to enter a different xml file name and then go further.
		CString strXMLFileName = strDWGPath1 + _T(".xml");
		XMLPathIter = mapofXMLPath.find(strXMLFileName);
		if(XMLPathIter != mapofXMLPath.end())
		{
			strOutputFile2.Append(_T(".xml"));
			//If the filenames of new xml created from different dwg's are same 
			//then give a message to the user and change the xml file name.
			CString strMsgFileName = strOutputFile2.Right(strOutputFile2.Find('\\'));
			CString strMessage;
			strMessage.Format(_T("Output XML '%s'already exist. Do you want to create XML with different name?"), strOutputFile2);
			MessageBox(strMessage);
			DlgCtrl:INT_PTR iret = inputDlg.DoModal();

			//Get the new xml file name.
			strName = inputDlg.m_strXMLName;
			//CString strDWGPathTmp = strDWGPath1.Left(strDWGPath1.ReverseFind('.'));
			if(strName.ReverseFind('.') == -1)
				strName.Append(_T(".xml"));
			strXMLFileName = strName;
			XMLPathIter = mapofXMLPath.find(strName);
			if(XMLPathIter != mapofXMLPath.end())
			{
				MessageBox(_T("The file already exists"));
				goto DlgCtrl;
			}
			//Check the new xml is empty or the file extension is other than .xml.
			CString strNameTmp1 = strName.Right(strName.ReverseFind('.'));
			CString strNameTmp2 = strName.Left(strName.ReverseFind('.'));
			if(NSSTRCMP(strNameTmp2,_T(""))== 0  || NSSTRCMP(strNameTmp1, _T("xml")) == 1)
			{
				MessageBox(_T("Enter valid file name."));
				goto DlgCtrl;
			}
			//Store the new file name along with its path in the output string.
			strOutputFile1.Replace(_T("/"), _T("\\"));
			strOutputFile1.Append(_T("\\"));
			strOutputFile1.Append(strName);
		}

		//Create the command line argument by adding the app_path, DWG file path and the
		//the path where the xml file is going to be stored.
		CString strPath = szApp_Path;
		strPath = strPath.Left(strPath.ReverseFind('\\'));

		#ifndef _DEBUG
			strPath+=_T("\\GISCADConverter.exe");  //release
		#else
			//strPath+=_T("\\GISCADConverterD.exe");
			strPath+=_T("\\GISCADConverter.exe");
		#endif
		
		//Get the command used for converion in a string
		strPath = strPath + _T(" ") + _T("\"")+ strDwgPath + _T("\"") + _T(" ") + _T("\"") + strOutputFile1 + _T("\"");
		
		//Change the status of progress bar
        m_ProgressBar.SetPos(nProgressCount++);
		
		//Execute the converter utility
		TCHAR* pszCmd = strPath.GetBuffer(0);
		BeginWaitCursor();
		int nRetVal = (int)ExecuteConverterApp(pszCmd);
        Invalidate();
        EndWaitCursor();
        m_ProgressBar.SetPos(nProgressCount++);
		
		//If the conversion is successfull then show a 'Done' word in the label ctrl that 
		//displays file name
        strExt = strDwgPath.Mid(nSlash + 1, (strDwgPath.GetLength() - nSlash))+_T("....Done.");
		m_FileName.SetWindowTextW(strExt);

		if(nRetVal == SUCCESS || nRetVal == PARTIAL_SUCCESS)
		{
			bConvert = true;
			mapofXMLPath.insert(std::map<CString,int >::value_type(strXMLFileName,nFileno));
		}
	
		
		//Display the result in the second column acc to the value of the nRetVal
		CString strDisplayResult = _T(" ");
		switch(nRetVal)
		{
		case SUCCESS: strDisplayResult = _T("Success"); break;
		case INVALID_DRAWING: strDisplayResult = _T("Invalid Drawing"); break;
		case PARTIAL_SUCCESS: strDisplayResult = _T("Partial Success");break;
		case FAIL: strDisplayResult = _T("Failed");break;
		}
		m_ListCtrl.SetItemText(nFileno, 1, strDisplayResult);
		m_ListCtrl.Update(nFileno);
		
        m_FileName.SetWindowTextW(_T(" "));
		if(nRetVal == -1001 || nRetVal == -1002)
		{
			continue;
		}
	}
	//Display message that the conversion is completed and then set the progress bar to zero.
	if(bConvert)
		MessageBox(_T("Conversion completed!"), _T("GISCADLoader"), MB_ICONASTERISK | MB_OK);
	m_ProgressBar.SetPos(0);
	if(nflag == 0)
	{
		if(bFocusFlag)
		{
			m_PathName.SetFocus();
			m_PathName.SetSel(0, m_PathName.GetWindowTextLengthW());
		}
		bValue = 1;
		bShow = 1;
		disableControls(bValue, bShow);
	}
}
/*
@brief		Displays the help contents as .chm file
@Param(in)	NULL
@Param(out)	NULL
*/
void CNSLoaderDlg::OnBnClickedBtnHelp()
{
	unselectListCtrl();
	m_FileName.SetWindowTextW(_T(" "));
	m_ProgressBar.SetPos(0);
	::HtmlHelp(this->m_hWnd, _T("..\\..\\..\\..\\Release Control\\Help\\GCIHELP.chm"),HH_DISPLAY_TOPIC,NULL); 
}
/*
@brief  Displays the help contents as .chm file on pressing F1
@Param(in)	NULL
@Param(out)	NULL
*/
/*BOOL CNSLoaderDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{	
	//::HtmlHelp(this->m_hWnd,_T("readarx.chm"),HH_DISPLAY_TOPIC,NULL);
    //AfxMessageBox(_T("Online help is unavailable"));
	return TRUE;
}*/
/*
@brief		Validates the dwg file 
@Param(in)	Input is the dwg file that is to be validated
@Param(out)	Output is the result that is either valid or invalid dwg
*/
void CNSLoaderDlg::OnBnClickedBtnValidate()
{
	unselectListCtrl();
	 gValidate = true;
	 //Make the second column of list ctrl blank before starting validation.
	for(int nCntr = 0; nCntr < m_ListCtrl.GetItemCount(); nCntr++)
    { 
        m_ListCtrl.SetItemText(nCntr, 1, _T(" "));
		m_ListCtrl.Update(nCntr);
    }
	CString strDisplayResult = _T(" ");
	bool bValidate = false;
	//Set Progress bar to its initial position
	m_ProgressBar.SetPos(0);
	m_ProgressBar.SetStep(1);
	int nProgressCount = 1;

	#ifdef VALIDATOR														
	//Bring the progress bar to its initial position						
	//and delete the label ctrl blank in which file name gets displayed.	
	//CNSLoaderDlg::showProgressBar();										 
 	TCHAR app_path[_MAX_PATH + 1];
	GetModuleFileName((HMODULE)AfxGetApp()->m_hInstance, app_path, MAX_PATH);
	CString strPath = app_path;
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	strPath+=_T("\\settings.ini");
	NSSTRING strFilename = (NSSTRING)strPath;


	if(NSACCESS(strFilename.c_str(), 0) != -1)
	{
		m_pConfigFile = new ConfigFile((TCHAR*)strFilename.c_str());
		TCHAR szValue[_MAX_PATH + 1];
		IValidator* pValidator = createValidator();
		
		//Get the path of std dwg file from the settings.ini file
		if(m_pConfigFile->readString(szValue,_T("SRVR_STD_DWG")))
		{
			pValidator->setStdDWGFilePath(szValue);
		}
		if(m_pConfigFile->readString(szValue,_T("REPORT_LOCATION")))
		{
			pValidator->setReportFilePath(szValue);
		}
		if(m_pConfigFile->readString(szValue,_T("RELATION_FLAG")))
		{
			int nRelFlag = NSATOI(szValue);
			pValidator->setRelationshipFlag(nRelFlag);
		}
		//-- Read Connectivity Flag from .INI File. --
		if(m_pConfigFile->readString(szValue,_T("GIS_FLAG")))
		{
			int nRelFlag = NSATOI(szValue);
			pValidator->setConnctivityFlag(nRelFlag); //Set GIS_Flag to CNSBaseValidator class
		}
		//---------------------------------------------
		if(m_pConfigFile->readString(szValue,_T("GRAPHIC_INDICATION")))
		{
			pValidator->setGraphicIndication(NSATOI(szValue));
		}
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

		CString strOutputFile1;
		gstrReportPaths = _T("");
		//Start validating the dwg file one by one
		int nCount = m_ListCtrl.GetItemCount();
		for(int nItemCntr = 0; nItemCntr < nCount; nItemCntr++)
		{
			NSSTRING strReportPath;
			// Getting fileName on the list ctrl
			CString szDwgPath;
			BeginWaitCursor();
			szDwgPath = m_ListCtrl.GetItemText(nItemCntr, 0);	
			
			int nSlash = szDwgPath.ReverseFind('\\');
			CString strExt = _T("Validating ") + szDwgPath.Mid(nSlash + 1, (szDwgPath.GetLength() - nSlash))+_T("......");
			m_FileName.SetWindowTextW(strExt);	
			//Increase the status of the progress bar
			pValidator->setDWGFilePath(szDwgPath.GetBuffer(0));
			m_ProgressBar.SetRange32(0, nCount);
			m_ProgressBar.SetPos(nProgressCount++);

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

            if(m_ChkAssetNumber.GetCheck()==1)
	        {
		        pValidator->setAssetValidationFlag(true);
                
                CString strHostID;
		        m_Host.GetWindowText(strHostID);

		        CString strUserName;
		        m_editUser.GetWindowText(strUserName);

		        //m_strUserName = strUserName;
		        CString strPwd;
                m_editPassword.GetWindowText(strPwd);

		        CString strDistrict;
                m_editDistrict.GetWindowText(strDistrict);

		        CString strPosition;
                m_Position.GetWindowText(strPosition);

                if(NSSTRCMP(strUserName, _T("")) == 0)
		        {
			        MessageBox(_T("Please enter UserName"));
			        m_editUser.SetFocus();
			        return;
		        }
        		
		        else if(NSSTRCMP(strPwd, _T("")) == 0)
		        {
			        MessageBox(_T("Please enter Password"));
			        m_editPassword.SetFocus();
			        return;
		        }
        		
                else if(NSSTRCMP(strDistrict, _T("")) == 0)
		        {
			        MessageBox(_T("Please enter District"));
			        m_editDistrict.SetFocus();
			        return;
		        }
        		
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

		        //NSSTRCPY(userCredentials.m_szHostID, strHostID.GetBuffer(0));
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
			int nRetVal = pValidator->validate();
			EndWaitCursor();
			if(nRetVal == 0)
				bValidate = true;	
			switch(nRetVal)
			{
			case SUCCESS: strDisplayResult = _T("COMPLETED"); break;
			case FAIL: strDisplayResult = _T("FAILED");break;
			}
			m_ListCtrl.SetItemText(nItemCntr, 1, strDisplayResult);
			m_ListCtrl.Update(nItemCntr);
			pValidator->getReportFilePath(strReportPath);
			gstrReportPaths.Append(strReportPath.c_str());
		}
			if(bValidate)
				MessageBox(_T("Validation completed!"), _T("GISCADLoader"), MB_ICONASTERISK | MB_OK);
			m_ProgressBar.SetPos(0);
			m_FileName.SetWindowTextW(_T(""));
			delete pValidator;
	}
	else
		MessageBox(_T("Config File not present."));
#endif
}
/*
@brief			Disable the remove button if not selected properly on the file name
@Param(in)		NULL
@Param(out)		NULL
*/
void CNSLoaderDlg::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_FileName.SetWindowTextW(_T(" "));
	m_ProgressBar.SetPos(0);

	
	//Disable the remove button if nothing is selected
	int nSelcount = m_ListCtrl.GetSelectedCount();
	if(nSelcount != 0)
	{
		m_Remove.EnableWindow(true);
	}
	else
		m_Remove.EnableWindow(false);
	
	
	*pResult = 0;
}
/*
@brief		Initially the convert and validate button should be disabled.
*/
void CNSLoaderDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CTabPageSSL::OnShowWindow(bShow, nStatus);
	CGISCADLoaderDlg* obj = (CGISCADLoaderDlg*)(this->GetParent());
	obj->showControls(!bShow);
}
/*
@brief			This fuction enables and disables the ellipse group according to the 
				status of Asset Validation check box.
@Param(in)		NULL
*/
CWnd* CNSLoaderDlg::CWnd_GetNextDlgGroupItemEx(CWnd *pCtrlWnd)
{
	CWnd	*pWnd;
	if (pCtrlWnd == NULL
	|| pCtrlWnd->GetDlgCtrlID() == IDC_EDIT_DISTRICT
	|| (pWnd = pCtrlWnd->GetWindow(GW_HWNDNEXT)) == NULL
	|| (pWnd->GetStyle() & WS_GROUP))
	{
		return NULL;
	}
	return pWnd;
}

void CNSLoaderDlg::OnBnClickedChkAssetvalidation()
{
	unselectListCtrl();
	int nStatus = m_ChkAssetNumber.GetCheck();
    bool bStatus;
    (nStatus ==1)?bStatus = true:bStatus = false;
	m_Host.EnableWindow(bStatus);
	m_editUser.EnableWindow(bStatus);
	m_editPassword.EnableWindow(bStatus);
	m_editDistrict.EnableWindow(bStatus);
	m_Position.EnableWindow(bStatus);

	m_StaticHost.EnableWindow(bStatus);
	m_StaticUser.EnableWindow(bStatus);
	m_StaticPassword.EnableWindow(bStatus);
	m_StaticPosition.EnableWindow(bStatus);
	m_StaticDistrict.EnableWindow(bStatus);

		/*	CWnd *pCtrlWnd, *pCtrlWnd1;
	pCtrlWnd = this->GetDlgItem(IDC_ELLIPSEDETAIL);
	int nID = pCtrlWnd->GetDlgCtrlID();  
	if(pCtrlWnd != NULL)
	{
		do
		{
			pCtrlWnd->EnableWindow(m_ChkAssetNumber.GetCheck());
		} while ((pCtrlWnd = CWnd_GetNextDlgGroupItemEx(pCtrlWnd)) != NULL && (nID != IDC_XML2DWG));
	}*/
}
void CNSLoaderDlg::OnEnSetfocusEditXmlpath()
{
	unselectListCtrl();
	m_FileName.SetWindowTextW(_T(" "));
	m_ProgressBar.SetPos(0);
}
/*
@brief			Gets the path of the report and stores it in the summary.txt
@Param(in)		NULL
@Param(out)		Returns the summary path
*/
CString CNSLoaderDlg::getSummaryPath()
{
	CString strOutputFile;
	m_PathName.GetWindowTextW(strOutputFile);
	CString strReport = _T("\\summary.txt");
	strOutputFile.Append(strReport);
	return (strOutputFile);
}
/*
@breif			Sets the progress bar and the label ctrl to its initial position
@Param(in)		NULL
@Param(out)		NULL
*/
void CNSLoaderDlg::showProgressBar()
{
	m_FileName.SetWindowTextW(_T(" "));
	m_ProgressBar.SetPos(0);
}
/*
@breif			Unselects if any dwg file is selected and disables the remove button.
@Param(in)		NULL
@Param(out)		NULL
*/
void CNSLoaderDlg::unselectListCtrl()
{
	int nFileCount = m_ListCtrl.GetItemCount();
	for(int i=0;i<nFileCount;i++)
	{
		if(m_ListCtrl.GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED)
			m_ListCtrl.SetItemState(i, 0, LVIS_SELECTED);
	}
	m_Remove.EnableWindow(false);
}
int CNSLoaderDlg::isDirectoryExists(TCHAR *szFileName)
{
  TCHAR* szReport = szFileName;
  struct NSSTATSTRUCT stFileInfo;
  if(NSACCESS(szReport, 0) != -1)
  {
        int value = NSSTATFUNC(szReport, &stFileInfo);  
        if(value != -1)
        {
              if(stFileInfo.st_mode & _S_IFDIR )
                    return NS_SUCCESS;
        }
  }
  return NS_FAIL;

} 
BOOL CNSLoaderDlg::PreTranslateMessage(MSG* pMsg)
{
      if (NULL != m_pToolTip)
      {
            m_pToolTip->RelayEvent(pMsg);
      }
      return CDialog::PreTranslateMessage(pMsg);
}

void CNSLoaderDlg::OnCbnSelchangeCombo1Host()
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

bool CNSLoaderDlg::checkErrorBoxChanged(NSSTRING& dlgErrorMsgs, NSSTRING& fileErrorMsgs)
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
/*bool CNSLoaderDlg::UpdateReportLocation(std::wstring m_strFilename)
{
	
	NSIFSTREAM inputFile(m_strFilename.c_str() ,std::ios::in | std::ios::binary );
	inputFile.seekg(0,std::ios::end);
	long size = inputFile.tellg();
	wchar_t *buffer=new wchar_t[size];
	inputFile.seekg(0,std::ios::beg);
	inputFile.read(buffer,size);
	inputFile.close();
	NSSTRING txtStr(buffer,size);
	delete[] buffer;
	size_t off=0;
	
	CString strReportLocn;
	m_PathName.GetWindowTextW(strReportLocn);
	const TCHAR* strReportLoc=LPCTSTR(strReportLocn);

	if(strReportLoc != NULL)
	{				
		off=txtStr.find(_T("SAVE_XML_FILES_LOCATION = "),off);
		if(off)
		{
			CString tempStr("SAVE_XML_FILES_LOCATION = ");
			tempStr.Append(strReportLoc);
			tempStr.Append(_T("\r"));
			TCHAR* tempstrReportLoc = tempStr.GetBuffer(MAX_PATH);
			txtStr.replace(off ,tempstrReportLoc,tempstrReportLoc);
			
			tempStr.ReleaseBuffer();		    
		}
	}

	std::wofstream outFile(m_strFilename.c_str(),std::ios::out | std::ios::binary);
	outFile.write(txtStr.c_str(),size);
	txtStr.clear();
	outFile.close();
	return 0;
}*/

/*bool CNSLoaderDlg::UpdateReportLocation(std::wstring m_strFilename)
{
	NSFSTREAM inputFile(m_strFilename.c_str() ,std::ios::in |std::ios::out | std::ios::binary);
	inputFile.seekg(0,std::ios::end);
	
	long size = inputFile.tellg();
	wchar_t *buffer=new wchar_t[size];

	wchar_t *linebuffer=new wchar_t[70];
	int sizeLineBuffer;

	inputFile.seekg(0,std::ios::beg);
	
	
	NSSTRING txtStr(linebuffer,70);
	
	CString before;
	CString after;
	long writePosg;
	long writePosp;

	writePosg = inputFile.tellg();
	writePosp = inputFile.tellp();

	while(!inputFile.eof())       // Read the file 
	{
		inputFile.clear();
		
		inputFile.seekg(writePosg);
		inputFile.getline(linebuffer,70);
		writePosg = inputFile.tellg();

		CString strEqual(linebuffer);
		int lenBuffer= strEqual.GetLength();
		int pos = strEqual.Find(_T("="));

		 before = strEqual.Mid(0, pos - 1);
		

		before.Append(_T(" ="));
		TCHAR* tempstrReportLoc = before.GetBuffer(MAX_PATH);		
		before.ReleaseBuffer();

		//Update the New Value of "SAVE_XML_FILES_LOCATION"
		if(NSSTRCMP(tempstrReportLoc,_T("SAVE_XML_FILES_LOCATION ="))==0)
		{
			CString strReportLocn;
			after = strEqual.Mid(pos+1, (strEqual.GetLength() -1));

			m_PathName.GetWindowTextW(strReportLocn);
			//int sizeNewLoc = strReportLocn.GetLength();
			//const TCHAR* strReportLoc=LPCTSTR(strReportLocn);

			CString rptLine;
			rptLine.Append(before);
			rptLine.Append(strReportLocn);
			//rptLine.Append(endl);
			const TCHAR* strReportLoc=LPCTSTR(rptLine);
			
			//inputFile.seekp((writePosg - lenBuffer),writePosg);
			inputFile.seekp((writePosg - lenBuffer));
			//writePosp = inputFile.tellp();
			inputFile.write(strReportLoc,(writePosp -1));
				writePosp = inputFile.tellp();
			inputFile.seekg(writePosg);
			
			
		}
		else
		{
			continue;
		}
		
	}

	
	inputFile.close();
	//NSSTRING txtStr(buffer,size);
	delete[] buffer;

	///std::wofstream outFile(m_strFilename.c_str(),std::ios::out | std::ios::binary);
	//outFile.write(txtStr.c_str(),txtStr.size());
	//txtStr.clear();
	//outFile.close();
	
	return 0;
}*/

bool CNSLoaderDlg::UpdateReportLocation(std::wstring m_strFilename)
{
	std::wstring m_strFilenameNew = m_strFilename;
	int sizeNewFile = m_strFilenameNew.length();
	m_strFilenameNew = m_strFilenameNew.substr(0,(sizeNewFile -4));
	m_strFilenameNew = m_strFilenameNew.append(_T("_NEW.ini"));
	fstream inputFile((m_strFilename.c_str()),std::ios::in | std::ios::binary);
	std::ofstream outFile(m_strFilenameNew.c_str(),std::ios::out | std::ios::binary);

	inputFile.seekg(0,std::ios::end);
	
	long size = inputFile.tellg();
	wchar_t *buffer=new wchar_t[size];

	//wchar_t *linebuffer=new wchar_t[70];
	int sizeLineBuffer;

	inputFile.seekg(0,std::ios::beg);
	
	CString before;
	CString after;

	string linebuffer;
		
	
	while(!inputFile.eof())       // Read the file 
	{
		
		
		getline(inputFile,linebuffer);
		CString strEqual(linebuffer.c_str());
		int lenBuffer= strEqual.GetLength();
		int pos = strEqual.Find(_T("="));

		 before = strEqual.Mid(0, pos - 1);
		

		before.Append(_T(" ="));
		TCHAR* tempstrReportLoc = before.GetBuffer(MAX_PATH);		
		before.ReleaseBuffer();

		//Update the New Value of "SAVE_XML_FILES_LOCATION"
		if(NSSTRCMP(tempstrReportLoc,_T("SAVE_XML_FILES_LOCATION ="))==0)
		{
			CString strReportLocn;
			after = strEqual.Mid(pos+1, (strEqual.GetLength() -1));

			m_PathName.GetWindowTextW(strReportLocn);
			

			CString rptLine;
			rptLine.Append(before); 
			rptLine.Append(_T(" "));
			rptLine.Append(strReportLocn);

			int count = rptLine.GetLength();
			char* cRpt = new char[count + 1];
			for(int j = 0; j < count; j++){
			 cRpt[j] = static_cast<char> (* rptLine.Mid(j, 1));
				}
		   cRpt[count] = '\0';
				
		   std::string str(cRpt);	
		
		 
		
			outFile << str ; 
			outFile << "\r\n";
			
			
			
		}
		else
		{			
			outFile << linebuffer << "\r\n"; 				
		}
		
	}

	
	inputFile.close();
	outFile.close();


	DeleteFile(m_strFilename.c_str());
	MoveFile(m_strFilenameNew.c_str(),m_strFilename.c_str());
	
	
	
	delete[] buffer;

	return 0;

}

