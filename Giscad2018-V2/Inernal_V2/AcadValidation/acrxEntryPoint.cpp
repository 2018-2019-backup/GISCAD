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
#include "NSValidatorDlg.h"

//#import "D:\\ObjectARX 2006\\inc\\acax16ENU.tlb" no_implementation raw_interfaces_only named_guids
//#import "E:\ObjectARX 2010\inc-win32\\acax18ENU.tlb" no_implementation raw_interfaces_only named_guids
#import "C:\\ObjectARX 2013\\inc-x64\\acax19ENU.tlb" no_implementation raw_interfaces_only named_guids
#include <rxregsvc.h>
#include <aced.h>
#include <adslib.h>

#include <afxdisp.h>        // MFC OLE automation classes

#include "adslib.h"
#include "acadi.h"
#include "rxmfcapi.h"


//-----------------------------------------------------------------------------
#define szRDS _RXST("NS")

//-----------------------------------------------------------------------------

void addToolBar();
void addMenuThroughCom();
void removeIEMenu();
void ValidateDrawing();

//----- ObjectARX EntryPoint
class CAcadValidationApp : public AcRxArxApp {

public:
	CAcadValidationApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		//addMenuThroughCom();
		//addToolBar();
		acedRegCmds->addCommand(_T("VALIDATION_COMMANDS"), 
                            _T("validate"), 
                            _T("validate"),
							ACRX_CMD_MODAL, 
                            ValidateDrawing);
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		//removeIEMenu();
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

void removeIEMenu()
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

	VariantInit(&index);
    V_VT(&index) = VT_BSTR;
    V_BSTR(&index) = wstrMenuName;
    pPopUpMenus->RemoveMenuFromMenuBar(index);
    VariantClear(&index);
}



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

		MultiByteToWideChar(CP_ACP, 0, "Design Validation", -1, wstrMenuItemName, 256); 
        MultiByteToWideChar(CP_ACP, 0, "validate\r", -1, wstrMenuItemMacro, 256); 

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

void ValidateDrawing()
{
	CAcModuleResourceOverride myResources;

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
	ConfigFile *pConfigFile = NULL;
	try
	{
		pConfigFile = new ConfigFile((TCHAR*)szSettingsFile.c_str());
	}
	catch(...)
	{
		MessageBox(acedGetAcadFrame()->m_hWnd,_T("Missing settings.ini file"), _T("Design Validation"), MB_ICONEXCLAMATION|MB_OK);
		return;
	}
	delete pConfigFile;
	pConfigFile = NULL;
	CNSValidatorDlg* objDlg = new CNSValidatorDlg();
	if(objDlg!=NULL)
	{
		const ACHAR* szDwgName;
		acdbHostApplicationServices()->workingDatabase()->getFilename(szDwgName);
		objDlg->SetCurrentDrawing(szDwgName);
		objDlg->DoModal();
		delete objDlg;
	}
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
	hr = tlbrs->Add(L"Integral Energy tools", &tlbr);
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
	GetModuleFileName(GetModuleHandle(_T("NSAcadValidation.arx")),szFileName,MAX_PATH);
	CString csPath(szFileName);
	csPath=csPath.Left(csPath.ReverseFind('\\')+1);
	
	hr = tlbr->AddToolbarButton(index, L"Design Validation", L"Design validation tool", L"validate\n", vtFalse, &button);
	hr=button->SetBitmaps(CComBSTR(csPath+"validation.ico"),CComBSTR(csPath+"validation.ico"));
	button->Release();

	tlbr->Dock(acToolbarDockLeft);
	tlbr->Release();
	return;
}

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CAcadValidationApp)

