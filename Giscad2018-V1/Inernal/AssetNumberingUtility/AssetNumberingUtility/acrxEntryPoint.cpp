// (C) Copyright 2002-2005 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
//#import "acax18ENU.tlb" no_implementation raw_interfaces_only named_guids
#import "acax22ENU.tlb" no_implementation raw_interfaces_only named_guids

#include <rxregsvc.h>
#include <aced.h>
#include <adslib.h>
#include "AssetNumberingUtilityDlg.h"
#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSReportWriter.h"
#include "..\..\Include\NSValidator.h"
#include "..\..\Include\NSAssetMgrIFace.h"
#include <afxdisp.h>        // MFC OLE automation classes

#include "adslib.h"
#include "acadi.h"
#include "rxmfcapi.h"


#define WriteToLog(str,err) CNSReportWriter::getReportWriterInstance()->writeLog(str,err)
//-----------------------------------------------------------------------------
#define szRDS _RXST("NS")
IAssetMgr* gAssetMgr = NULL;

// Adding Customized Menu to AutoCAD.
void addMenuThroughCom()
{
	AutoCAD::IAcadApplication *pAcad;
    AutoCAD::IAcadMenuBar *pMenuBar;
    AutoCAD::IAcadMenuGroups *pMenuGroups;
    AutoCAD::IAcadMenuGroup *pMenuGroup;
    AutoCAD::IAcadPopupMenus *pPopUpMenus;
    AutoCAD::IAcadPopupMenu *pPopUpMenu = NULL;
    AutoCAD::IAcadPopupMenuItem *pPopUpMenuItem1,*pPopUpMenuItem2;

    HRESULT hr = NOERROR;
    LPUNKNOWN pUnk = NULL;
	LPDISPATCH pAcadDisp = acedGetIDispatch(TRUE); 
	if(pAcadDisp==NULL)
		return;

	hr = pAcadDisp->QueryInterface(AutoCAD::IID_IAcadApplication,(void**)&pAcad);
    pAcadDisp->Release();
	if (FAILED(hr))
        return;

    pAcad->put_Visible(true);
	pAcad->get_MenuBar(&pMenuBar);
    pAcad->get_MenuGroups(&pMenuGroups);
    pAcad->Release();
    long numberOfMenus;
    pMenuBar->get_Count(&numberOfMenus);
    pMenuBar->Release();

    VARIANT index;
    VariantInit(&index);
    V_VT(&index) = VT_I4;
    V_I4(&index) = 0;

    pMenuGroups->Item(index, &pMenuGroup);
    pMenuGroups->Release();
    
    pMenuGroup->get_Menus(&pPopUpMenus);
    pMenuGroup->Release();

    WCHAR wstrMenuName[256];
    MultiByteToWideChar(CP_ACP, 0, "Integral Energy", -1, wstrMenuName, 256); 

 	VARIANT IEMenu;
    VariantInit(&IEMenu);
    V_VT(&IEMenu) = VT_BSTR;
    V_BSTR(&IEMenu) = (BSTR)"Integral Energy";

	pPopUpMenus->Item(IEMenu,&pPopUpMenu);

	int numberOfMenuItems = 0;
	if(pPopUpMenu == NULL)
		pPopUpMenus->Add(wstrMenuName, &pPopUpMenu);
	else
		numberOfMenuItems = 1;
	
	if (pPopUpMenu != NULL) {

        WCHAR wstrMenuItemName[256];
        WCHAR wstrMenuItemMacro[256];

		MultiByteToWideChar(CP_ACP, 0, "AssetNumbering", -1, wstrMenuItemName, 256); 
        MultiByteToWideChar(CP_ACP, 0, "IEASSETNUM\r", -1, wstrMenuItemMacro, 256); 

        VariantInit(&index);
        V_VT(&index) = VT_I4;
        V_I4(&index) = numberOfMenuItems;
        pPopUpMenu->AddMenuItem(index, wstrMenuItemName, wstrMenuItemMacro, &pPopUpMenuItem1);

        
        VariantInit(&index);
        V_VT(&index) = VT_I4;
        V_I4(&index) = numberOfMenus - 1;
        pPopUpMenu->InsertInMenuBar(index);

        pPopUpMenuItem1->Release();

	}
    pPopUpMenus->Release();
}

bool getApplication(LPDISPATCH * pVal)
{
	LPDISPATCH pDispatch = acedGetAcadWinApp()->GetIDispatch(TRUE);
	if (pDispatch == NULL)
		return false;
	*pVal = pDispatch;
	return true;
}

bool getAcadMenuGroup(IAcadMenuGroup  **pVal)
{

	IAcadApplication *acadApp = NULL;
	LPDISPATCH  pDisp = NULL;

	if (!getApplication(&pDisp))
		return false;
	
	HRESULT hr = S_OK;
	hr = pDisp->QueryInterface(IID_IAcadApplication, (LPVOID*)&acadApp);
	if (FAILED(hr))
		return false;


	LPDISPATCH  pTempDisp = NULL;
	IAcadMenuGroups *mnuGrps = NULL;
	long cnt = 0;
	
	//get the menu groups
	hr = acadApp->get_MenuGroups(&mnuGrps);
	if (FAILED(hr))
	{
		acadApp->Release();
		return false;
	}
	mnuGrps->get_Count(&cnt);
	

	//get AutoCAD menu group. say it is index 0.
	IAcadMenuGroup *mnuGrp = NULL;

	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  grpName;
	bool found = false ;
	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = mnuGrps->Item(vtName, &mnuGrp);
		if (FAILED(hr))
			return false;

		
		hr  = mnuGrp->get_Name(&grpName);
		CString cgrpName(grpName);
		if (cgrpName.CompareNoCase(_T("Acad"))==0) 
		{
			found = true;
			*pVal = mnuGrp;
			break;
		}
	}

	acadApp->Release();
	return found;
}

void addToolBar()
{
	IAcadMenuGroup *mnuGrp = NULL;
	if (!getAcadMenuGroup(&mnuGrp))
		return ;
	//now get all the popup menus 
	IAcadToolbars  *tlbrs;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;
	hr = tlbrs->Add(L"AEC Toolbar", &tlbr);
	if FAILED(hr)
		return;
	tlbrs->Release();
	//now add toolbar buttons
	IAcadToolbarItem *button=NULL;
	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;

	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;

	TCHAR szFileName[MAX_PATH];
	GetModuleFileName(GetModuleHandle(_T("NSAssetNumberingUtility.arx")),szFileName,MAX_PATH);
	CString csPath(szFileName);
	csPath=csPath.Left(csPath.ReverseFind('\\')+1);
	
	hr = tlbr->AddToolbarButton(index, L"Asset Numbering", L"Asset Numbering Utility", L"IEASSETNUM\n", vtFalse, &button);
	hr=button->SetBitmaps(CComBSTR(csPath+"INFO.ICO"),CComBSTR(csPath+"INFO.ICO"));
	button->Release();

	tlbr->Dock(acToolbarDockLeft);
	tlbr->Release();
	return;
}

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CAssetNumberingUtilityApp : public AcRxArxApp {

public:
	CAssetNumberingUtilityApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
//		addMenuThroughCom();
		//addToolBar();
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}


	// - NSAssetNumberingUtility._IEASSETNUM command (do not rename)
	static void NSAssetNumberingUtility_IEASSETNUM(void)
	{
		// Add your code for command NSAssetNumberingUtility._IEASSETNUM here

		// Checking for Configuration file
		TCHAR szAppPath[MAX_PATH+1];
		GetModuleFileName(_hdllInstance,szAppPath,MAX_PATH+1);
		CString strAppPath = szAppPath;
		CString strLogFilePath;
		int nIndex = strAppPath.ReverseFind('\\');
		if(nIndex != -1)
		{
			strAppPath = strAppPath.Left(nIndex);
		}
		/////////
		Acad::ErrorStatus errStatus;
		NSSTRING strDWGPath, strDWGFinalName = _T("\\"),strDWGFullPath;
		//ACHAR* pOutput;
		//errStatus = acdbHostApplicationServices()->workingDatabase()->(ACHAR*)getFilename(pOutput);
	    const TCHAR* pOutput;
		errStatus = acdbHostApplicationServices()->workingDatabase()->getFilename(pOutput);
		if(errStatus == Acad::eOk)
		{
			strDWGPath = pOutput;	
			int nSlash = strDWGPath.find_last_of(_T("\\"));
			if(nSlash!=-1)
			{
				strDWGFullPath = strDWGPath.substr(0,nSlash);
				strDWGPath = strDWGPath.substr(nSlash+1,strDWGPath.length()-1);
				int nDot = strDWGPath.find_last_of(_T("."));
				if(nDot!=-1)
				{
					strDWGPath = strDWGPath.substr(0,nDot);
					strDWGFinalName +=strDWGPath;
					strDWGFinalName =strDWGFullPath + strDWGFinalName+_T("_Assets.log");
				}
			}
		}
		else
		{
			strDWGFinalName =_T("\\AssetNumbering.log");
		}
		//////

		//strLogFilePath = strAppPath + "\\AssetNumbering.log";
		//strLogFilePath = strAppPath + strDWGFinalName.c_str();
		strLogFilePath = strDWGFinalName.c_str();
		strAppPath = strAppPath + "\\Settings.ini";
		CNSValidator validator;
		int nRet = validator.isFilePresent(strAppPath.GetBuffer(0));
		if(nRet != NS_SUCCESS)
		{
			WriteToLog(LOG_OTHER, _T("Configuration File Missing."));
			AfxMessageBox(_T("Configuration File missing"), MB_ICONINFORMATION|MB_OK);
			return;
		}

		CoInitialize(NULL);
		CAssetNumberingUtilityDlg objDlg;
		objDlg.setConfigLocation(strAppPath.GetBuffer(0));
		objDlg.setLogFileLocation(strLogFilePath.GetBuffer(0));
		objDlg.DoModal();
		CoUninitialize();

	}
public:

	// - NSAssetNumberingUtility._IEVALIDATE command (do not rename)
	static void NSAssetNumberingUtility_IEVALIDATE(void)
	{
		CAssetNumberingUtilityDlg objDlg;
		objDlg.DoModal();
		// Add your code for command NSAssetNumberingUtility._IEVALIDATE here
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CAssetNumberingUtilityApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CAssetNumberingUtilityApp, NSAssetNumberingUtility, _IEASSETNUM, IEASSETNUM, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAssetNumberingUtilityApp, NSAssetNumberingUtility, _IEVALIDATE, IEVALIDATE, ACRX_CMD_TRANSPARENT, NULL)
