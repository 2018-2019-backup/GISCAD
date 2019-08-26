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
//----- DocData.h : include file for document specific data. An instance of this
//----- class is automatically created and managed by the AsdkDataManager class.
//----- See the AsdkDmgr.h / DocData.cpp for more datails
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//----- Here you can store the document / database related data.
class CDocData {
public:
	CDocData () ;
	CDocData (const CDocData &data) ;
	~CDocData () ;

	//{{AFX_ARX_DATA(CDocData)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_ARX_DATA

	//----- TODO: here you can add your variables
	
	CString g_csBaseFolder;
	CString g_csPoleTextStyle, g_csPoleTextHeight, g_csPoleDiameter;
	CString g_csSchTextStyle,  g_csSchTextHeight;

	CStringArray g_csaPoleHVCat, g_csaPoleHVText, g_csaPoleLVCat, g_csaPoleLVText;
	CStringArray g_csaPoleStays, g_csaPoleFootings, g_csaPoleBrackets;
	CStringArray g_csaPoleSizes, g_csaPoleDepths, g_csaPoleDiameters;
	CStringArray g_csaPoleTypes, g_csaPoleBlocks, g_csaPoleLayers, g_csaPoleLayerColors;
	CStringArray g_csaPoleAttDescs, g_csaPoleAttTags;

	CStringArray g_csaLanternCharges, g_csaLanternCategories;
	CStringArray g_csaLanternBrackets, g_csaLanternBracketCodes;
	CStringArray g_csaLanternMasts, g_csaLanternMastCodes;
	CStringArray g_csaLanternOutreaches, g_csaLanternOutreachCodes;
	CStringArray g_csaLanternTypes, g_csaLanternBlocks, g_csaLanternLayers, g_csaLanternLayerColors;
	CStringArray g_csaColumnTypes, g_csaColumnBlocks, g_csaColumnLayers, g_csaColumnLayerColors;
	CStringArray g_csaLanternSch1Types, g_csaLanternSch1Lamps, g_csaLanternSch1LampCodes, g_csaLanternSch1LanternCodes, g_csaLanternSch1RateCodes;
	CStringArray g_csaLanternSch2Types, g_csaLanternSch2Lamps, g_csaLanternSch2LampCodes, g_csaLanternSch2LanternCodes, g_csaLanternSch2RateCodes;
	CStringArray g_csaLightAttDescs, g_csaLightAttTags;
	CStringArray g_csaColumnAttDescs, g_csaColumnAttTags;

	CStringArray g_csaConductorNames, g_csaConductorLTypes, g_csaConductorLayers, g_csaConductorLayerColors;
	CStringArray g_csaLightTblTitles, g_csaPoleTblTitles;

	CString g_csMDBLocation;
	CString g_csDWGLocation;
} ;
