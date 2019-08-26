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
//----- StdAfx.h : include file for standard system include files,
//-----      or project specific include files that are used frequently,
//-----      but are changed infrequently
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#ifdef _FULLDEBUG_
#pragma message ("Warning! You should not run this application in a Standard AutoCAD release.")
#pragma message ("         Due to the memory managment constraints.")
#endif

#pragma warning(disable: 4786)
//#pragma warning(disable: 4098)

//-----------------------------------------------------------------------------
#define STRICT

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN			//----- Exclude rarely-used stuff from Windows headers
#endif

//----- Modify the following defines if you have to target a platform prior to the ones specified below.
//----- Refer to MSDN for the latest info on corresponding values for different platforms.
/*#ifndef WINVER					//----- Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501			//----- Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT			//----- Allow use of features specific to Windows NT 4 or later.
//#define _WIN32_WINNT 0x0400		//----- Change this to the appropriate value to target Windows 2000 or later.
#define _WIN32_WINNT 0x0500
#endif						

#ifndef _WIN32_WINDOWS			//----- Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0500	//----- Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE				//----- Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0500		//----- Change this to the appropriate value to target IE 5.0 or later.
#endif*/

//-----------------------------------------------------------------------------
//----- 'DEBUG workaround' below prevents the MFC or ATL #include-s from pulling 
//----- in "Afx.h" that would force the debug CRT through #pragma-s.
#if defined(_DEBUG) && !defined(_FULLDEBUG_)
#define _DEBUG_WAS_DEFINED
#undef _DEBUG
#pragma message ("     Compiling MFC / STL / ATL header files in release mode.")
#endif

//----- ObjectARX and OMF headers needs this
#include <map>

//-----------------------------------------------------------------------------
#include <afxwin.h>				//----- MFC core and standard components
#include <afxext.h>				//----- MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>				//----- MFC OLE classes
#include <afxodlgs.h>			//----- MFC OLE dialog classes
#include <afxdisp.h>			//----- MFC Automation classes
#endif //----- _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>				//----- MFC ODBC database classes
#endif //----- _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>				//----- MFC DAO database classes
#endif //----- _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>			//----- MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>				//----- MFC support for Windows Common Controls
#endif //----- _AFX_NO_AFXCMN_SUPPORT

#include <io.h>          //------ For file system functions support
#include <ODBCInst.h>    //------ For ODBC support

//-----------------------------------------------------------------------------
#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG
#undef _DEBUG_WAS_DEFINED
#endif

//-----------------------------------------------------------------------------
//----- Include ObjectDBX/ObjectARX headers
//----- Uncomment one of the following lines to bring a given library in your project.
//#define _BREP_SUPPORT_			//- Support for the BRep API
//#define _HLR_SUPPORT_				//- Support for the Hidden Line Removal API
//#define _IE_SDK_SUPPORT_			//- Support for the Autodesk Image Engine API
//#define _AMODELER_SUPPORT_		//- Support for the AModeler API
//#define _ASE_SUPPORT_				//- Support for the ASI/ASE API
//#define _RENDER_SUPPORT_			//- Support for the AutoCAD Render API
//#define _ARX_CUSTOM_DRAG_N_DROP_	//- Support for the ObjectARX Drag'n Drop API
//#define _INC_LEAGACY_HEADERS_		//- Include legacy headers in this project
#include "arxHeaders.h"
#include "acedCmdNF.h"

//-----------------------------------------------------------------------------
#include "DocData.h" //----- Your document specific data class holder

//-----------------------------------------------------------------------------
#include <vector>
#include "SPUtils.h"
#include "QueryTbl.h"

//-----------------------------------------------------------------------------
#define DSN_IETB              _T("IE_ToolBox")
#define DICT_IETB							_T("IE_TB_DICTIONARY")
#define XREC_IETB							_T("IE_TB_XREC")
#define XDATA_POLE            _T("IE_TB_POLE")
#define XDATA_LIGHT           _T("IE_TB_LIGHT")
#define XDATA_LIGHT_NUM       _T("IE_TB_LIGHT_NUM")
#define XDATA_COLUMN          _T("IE_TB_COLUMN")
#define XDATA_COLUMN_NUM      _T("IE_TB_COLUMN_NUM")
#define XDATA_CABLE           _T("IE_TB_CABLE")
#define XDATA_POLE_SCH        _T("IE_TB_POLE_SCHEDULE")
#define XDATA_LIGHT_SCH       _T("IE_TB_LIGHT_SCHEDULE")
#define XDATA_CABLE_IN_POLE   _T("IE_TB_POLE_CABLE")

#define PI            3.1415926535897932384626433832795
#define PIby2         (PI / 2.0)
#define MinusPIby2    (PI / -2.0)

#define DTR(x) ((x) * (PI / 180.0))
#define RTD(x) ((x) * (180.0 / PI))

//-----------------------------------------------------------------------------
extern void ShowBalloon(CString csMessage, CWnd *pParent, UINT nCtrlID, CString csTitle = _T("Invalid")); // BalloonHelp.cpp

extern CString g_csBaseFolder;
extern CString g_csPoleTextStyle, g_csPoleTextHeight, g_csPoleDiameter;
extern CString g_csSchTextStyle,  g_csSchTextHeight;
extern CStringArray g_csaPoleHVCat, g_csaPoleHVText, g_csaPoleLVCat, g_csaPoleLVText;
extern CStringArray g_csaPoleStays, g_csaPoleFootings, g_csaPoleBrackets;
extern CStringArray g_csaPoleSizes, g_csaPoleDepths, g_csaPoleDiameters;
extern CStringArray g_csaPoleTypes, g_csaPoleBlocks, g_csaPoleLayers, g_csaPoleLayerColors;
extern CStringArray g_csaPoleAttDescs, g_csaPoleAttTags;

extern CStringArray g_csaLanternCharges, g_csaLanternCategories;
extern CStringArray g_csaLanternBrackets, g_csaLanternBracketCodes;
extern CStringArray g_csaLanternMasts, g_csaLanternMastCodes;
extern CStringArray g_csaLanternOutreaches, g_csaLanternOutreachCodes;
extern CStringArray g_csaLanternTypes, g_csaLanternBlocks, g_csaLanternLayers, g_csaLanternLayerColors;
extern CStringArray g_csaColumnTypes, g_csaColumnBlocks, g_csaColumnLayers, g_csaColumnLayerColors;

extern CStringArray g_csaRagboltType,g_csaRagboltTypeCodes;
extern CStringArray g_csaFoundationType,g_csaFoundationTypeCodes;

extern CStringArray g_csaLanternSch1Types, g_csaLanternSch1Lamps, g_csaLanternSch1LampCodes, g_csaLanternSch1LanternCodes, g_csaLanternSch1RateCodes;
extern CStringArray g_csaLanternSch2Types, g_csaLanternSch2Lamps, g_csaLanternSch2LampCodes, g_csaLanternSch2LanternCodes, g_csaLanternSch2RateCodes;
extern CStringArray g_csaLanternSch3Types, g_csaLanternSch3Lamps, g_csaLanternSch3LampCodes, g_csaLanternSch3LanternCodes, g_csaLanternSch3RateCodes;
extern CStringArray g_csaLanternSch4Types, g_csaLanternSch4Lamps, g_csaLanternSch4LampCodes, g_csaLanternSch4LanternCodes, g_csaLanternSch4RateCodes;
extern CStringArray g_csaLanternSch5Types, g_csaLanternSch5Lamps, g_csaLanternSch5LampCodes, g_csaLanternSch5LanternCodes, g_csaLanternSch5RateCodes;
extern CStringArray g_csaLanternSchUnRegulatedTypes, g_csaLanternSchUnRegulatedLamps, g_csaLanternSchUnRegulatedLampCodes, g_csaLanternSchUnRegulatedLanternCodes, g_csaLanternSchUnRegulatedRateCodes;

extern CStringArray g_csaReplaceLanternSch1Types, g_csaReplaceLanternSch1Lamps, g_csaReplaceLanternSch1LampCodes, g_csaReplaceLanternSch1LanternCodes, g_csaReplaceLanternSch1RateCodes;
extern CStringArray g_csaReplaceLanternSch2Types, g_csaReplaceLanternSch2Lamps, g_csaReplaceLanternSch2LampCodes, g_csaReplaceLanternSch2LanternCodes, g_csaReplaceLanternSch2RateCodes;

extern CStringArray g_csaLightAttDescs, g_csaLightAttTags;
extern CStringArray g_csaColumnAttDescs, g_csaColumnAttTags;

extern CStringArray g_csaConductorNames, g_csaConductorLTypes, g_csaConductorLayers, g_csaConductorLayerColors;
extern CStringArray g_csaLightTblTitles, g_csaPoleTblTitles;

extern CString g_csMDBLocation;
extern CString g_csDWGLocation;


//----- Declare it as an extern here so that it becomes available in all modules
extern AcApDataManager<CDocData> DocVars ;
void ads_regen();