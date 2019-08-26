////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project name     : IE Toolbox
// Client name      : AEC Systems
// File name        : OtherCommands.cpp
// Created          : 7th May 2007
// Description      : All commands that do not have a separate User Interface
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Resource.h"
#include "LightInfo.h"
#include "LegendInfo.h"

//////////////////////////////////////////////////////////////////////////
// Externally defined functions
//////////////////////////////////////////////////////////////////////////
// Defined in PoleDlg.cpp
extern CString GetPoleTag(CString csAttDesc); 
extern struct resbuf * ConvertToPole(ads_name enPole);

// Defined TableColumnDlg.cpp
extern void    setTextString(AcDbTable *pTable, int iRow, int iCol, AcDbObjectId objIDTxtStyle, AcDb::RotationAngle rotAngle, CString csText, CString csTxtHt); 
extern void    GetInsertNumbers(CString csRegappName, CUIntArray &uiaInsertNumbers);
extern int     getMaxInArray (CUIntArray &uiaInt);
extern CString GetLightTag(CString csAttDesc); 

// Defined in LightInfo.cpp
void sortLightInfo(std:: vector <CLightInfo> &lightInfo);
void consolidateLightInfo (std:: vector <CLightInfo> &lightInfo_Vector);

// Defined in AttachCableDlg.cpp
extern bool UnifyCablesAndDeleteSegments(AcGePoint3dArray &aryVertices, AcDbObjectIdArray &aryObjIDs, CString csLType);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : ModifyPoleTable
// Description  : Called by "Command_Edit_Pole" and "Command_RenumberPoles" when a pole information is modified.
//                Updates the values in tables generated with the latest information in poles.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ModifyPoleTable(AcDbObjectId objPole, CString csPoleNumber, CString csNewPoleNumber)
{
	// Get the handle
	AcDbHandle dbPoleHandle =  objPole.handle(); 
	ACHAR szPoleHandle[17]; dbPoleHandle.getIntoAsciiBuffer(szPoleHandle);
	
	// Get the pole number
	struct resbuf *rbpPoleXD = NULL;
	ads_name enPole; acdbGetAdsName(enPole, objPole);
	if ((rbpPoleXD = getXDataFromEntity(enPole, XDATA_POLE)) == NULL) return;
	rbpPoleXD = rbpPoleXD->rbnext->rbnext->rbnext->rbnext;
	CString csPoleStatus       = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csFieldNumberExist   = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
  CString csFieldNumberNew    = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csPoleSize          = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csPoleStay          = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csPoleFooting       = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csPoleDia           = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csHoleDepth         = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csHoleDia           = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csPoleHV            = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csPoleLV            = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csLanternStatus     = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csLanternBracket    = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csSpanLength        = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
	CString csLineDevDeg        = rbpPoleXD->resval.rstring;
  acutRelRb(rbpPoleXD);

	// Get the tables that has the records corresponding to this pole.
	ads_name ssTables;
	struct resbuf *rbpFilt = acutBuildList(67, 1, AcDb::kDxfRegAppName, _T("POLE_SCHEDULE"), AcDb::kDxfRegAppName, csPoleNumber, NULL);
	if (acedSSGet(_T("X"), NULL, NULL, rbpFilt, ssTables) != RTNORM) { acedSSFree(ssTables); acutRelRb(rbpFilt); return; }
	acutRelRb(rbpFilt);

	ads_name enTable;
	Adesk::Int32 lLength = 0L; acedSSLength(ssTables, &lLength);
	for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
	{
		acedSSName(ssTables, lCtr, enTable);

		// Open the table pointer and get the POLE_SCHEDULE xdata that has the list of numbers in the table
		AcDbObjectId objTable;
		AcDbTable *pTable = NULL;
		acdbGetObjectId(objTable, enTable);
		if (acdbOpenObject(pTable, objTable, AcDb::kForWrite) != Acad::eOk) { acedSSFree(ssTables); return; }

		// Get the Table Configuration
		struct resbuf *rbpTableCfg = pTable->xData(_T("TABLECFG"));	if (rbpTableCfg == NULL) continue;
		rbpTableCfg = rbpTableCfg->rbnext; bool bHasFooting   = rbpTableCfg->resval.rint; 
		rbpTableCfg = rbpTableCfg->rbnext; bool bHasStay      = rbpTableCfg->resval.rint; 
		rbpTableCfg = rbpTableCfg->rbnext; bool bHasType      = rbpTableCfg->resval.rint; 
		rbpTableCfg = rbpTableCfg->rbnext; bool bHasHoleData  = rbpTableCfg->resval.rint; 
		rbpTableCfg = rbpTableCfg->rbnext; bool bHasLantern   = rbpTableCfg->resval.rint; 
		rbpTableCfg = rbpTableCfg->rbnext; bool bHasFieldPole = rbpTableCfg->resval.rint; 
		rbpTableCfg = rbpTableCfg->rbnext; bool bHasConstData = rbpTableCfg->resval.rint; 
		acutRelRb(rbpTableCfg);
		
		for (int iRow = 0; iRow < pTable->numRows(); iRow++)
		{
			if (pTable->textString(iRow, pTable->numColumns() - 1) == csPoleNumber)
			{
				int iCol = 0;
				if (bHasFieldPole)
        {
          pTable->setTextString(iRow, iCol++, csFieldNumberNew);
          pTable->setTextString(iRow, iCol++, csFieldNumberExist);
        }
				if (bHasLantern)
				{ 
          if (!csLanternStatus.CompareNoCase(_T("New")))			pTable->setTextString(iRow, iCol + 0, _T("X")); else pTable->setTextString(iRow, iCol + 0, _T("")); 
					if (!csLanternStatus.CompareNoCase(_T("Remove")))   pTable->setTextString(iRow, iCol + 1, _T("X")); else pTable->setTextString(iRow, iCol + 0, _T(""));
					if (!csLanternStatus.CompareNoCase(_T("Existing"))) pTable->setTextString(iRow, iCol + 2, _T("X")); else pTable->setTextString(iRow, iCol + 0, _T(""));
					// if (csLanternStatus == _T("Replace"))  pTableLegend->setTextString(iRow, iCol + 3, _T("X")); iCol++;
					
					
					// iCol = iCol + 4; 

					//iCol = iCol + 3; //Commented on 24-06-2014
					iCol = iCol + 4; 

					pTable->setTextString(iRow, iCol++, csLanternBracket);
				}

				if (bHasConstData) 
				{
					pTable->setTextString(iRow, iCol++, csSpanLength);
					pTable->setTextString(iRow, iCol++, csLineDevDeg);
					pTable->setTextString(iRow, iCol++, csPoleHV);        
					pTable->setTextString(iRow, iCol++, csPoleLV);        
				}

				if (bHasHoleData)
				{
					pTable->setTextString(iRow, iCol++, csHoleDia);   
					pTable->setTextString(iRow, iCol++, csHoleDepth); 
				}

				// Set the row data
				if (bHasType)        pTable->setTextString(iRow, iCol++, csPoleSize);
				if (bHasStay)        pTable->setTextString(iRow, iCol++, csPoleStay);
				if (bHasFooting)     pTable->setTextString(iRow, iCol++, csPoleFooting);
        
				if (!csPoleStatus.CompareNoCase(_T("Relocate"))) pTable->setTextString(iRow, iCol + 0, _T("X"));  else pTable->setTextString(iRow, iCol + 0, _T(""));
				if (!csPoleStatus.CompareNoCase(_T("Replace")))  pTable->setTextString(iRow, iCol + 1, _T("X"));  else pTable->setTextString(iRow, iCol + 1, _T(""));
				if (!csPoleStatus.CompareNoCase(_T("New")))      pTable->setTextString(iRow, iCol + 2, _T("X"));  else pTable->setTextString(iRow, iCol + 2, _T(""));
				if (!csPoleStatus.CompareNoCase(_T("Existing"))) pTable->setTextString(iRow, iCol + 3, _T("X"));  else pTable->setTextString(iRow, iCol + 3, _T(""));
				if (!csPoleStatus.CompareNoCase(_T("Remove")))   pTable->setTextString(iRow, iCol + 4, _T("X"));  else pTable->setTextString(iRow, iCol + 4, _T(""));
				iCol = iCol + 5; 
				pTable->setTextString(iRow, iCol, csNewPoleNumber);

				break; 
			}
		}

		// Remove the previous pole number XData
		struct resbuf *rbpOldXD = acutBuildList(AcDb::kDxfRegAppName, csPoleNumber, NULL); 
		pTable->setXData(rbpOldXD); acutRelRb(rbpOldXD);

		// Add the new pole number XData
		struct resbuf *rbpNewXD = acutBuildList(AcDb::kDxfRegAppName, csNewPoleNumber, AcDb::kDxfXdInteger16, 1, NULL);
		acdbRegApp(csNewPoleNumber); pTable->setXData(rbpNewXD); acutRelRb(rbpNewXD);

		pTable->close();
	}

	acedSSFree(ssTables);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name: GetLightDetails()
// Description  : Gets the details of all the lights (from its XDATA) whose light numbers are given in the 
//                selection set filter.
// Called by    : ModifyLightTable()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetLightDetails(struct resbuf *rbpFilt, std::vector <CLightInfo> &lightInfo_Vector)
{
	// Get all the lights from model space for the given filter list.
	ads_name ssLights; 
	Adesk::Int32 lLength = 0L;
	if (acedSSGet(_T("X"), NULL, NULL, rbpFilt, ssLights) != RTNORM) { acedSSFree(ssLights); return; }
	if ((acedSSLength(ssLights, &lLength) != RTNORM) && (lLength < 1L)) return;

	CString csValue;
	ads_name enLight;
	struct resbuf *rbpLightXD = NULL;
	for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
	{
		// Get the light details
		acedSSName(ssLights, lCtr, enLight);
		if ((rbpLightXD = getXDataFromEntity(enLight, XDATA_LIGHT)) == NULL) continue;

		CLightInfo lightInfo;	
		rbpLightXD = rbpLightXD->rbnext->rbnext;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_iLightNumber	      = rbpLightXD->resval.rint; 
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLightNumber      = rbpLightXD->resval.rstring; 
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csIsStandard       = suppressZero(rbpLightXD->resval.rint);
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternSchedule  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternStatus    = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternType		  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternStockCode = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternLamp			= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLampStockCode    = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csRateCode				  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csKingbolt				  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternMast    	= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csMastCode       	= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternOutreach  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csOutreachCode		  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternBracket	  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csBracketCode   	  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternLocation  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternCategory  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csAssetColumn			= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csAssetLantern		  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternColumn    = rbpLightXD->resval.rstring;

		rbpLightXD = rbpLightXD->rbnext->rbnext; lightInfo.m_csRagBolt	= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csFoundationType	= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csMASTRATE			= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csOutreachRate		= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csBracketRate		= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csMountingHeight	= rbpLightXD->resval.rstring;
		
		// Charge To options
		rbpLightXD = rbpLightXD->rbnext->rbnext; int iSizeOfChargeTo = rbpLightXD->resval.rint;
		for (int iCtr = 0; iCtr < iSizeOfChargeTo; iCtr++) 
		{
			rbpLightXD = rbpLightXD->rbnext; csValue.Format(_T("%s"), rbpLightXD->resval.rstring);
			if (!lightInfo.m_csChargeTo.IsEmpty())
				lightInfo.m_csChargeTo.Format(_T("%s\\P%s(%s%%)"), lightInfo.m_csChargeTo, csValue.Mid(0, csValue.Find(_T("#"))), csValue.Mid(csValue.Find(_T("#")) + 1));
			else
				lightInfo.m_csChargeTo.Format(_T("%s(%s%%)"), csValue.Mid(0, csValue.Find(_T("#"))), csValue.Mid(csValue.Find(_T("#")) + 1));
		}

		//////////////////////////////////////////////////////////////////////////
		// If the light status is Replaced, add the replaced light information too
		//////////////////////////////////////////////////////////////////////////
		if (!lightInfo.m_csLanternStatus.CompareNoCase(_T("Replace")))
		{
			// Add this to the vector for sort
			lightInfo.m_csLanternStatus = _T("New");

			//////////////////////////////////////////////////////////////////////////
			// Replace light information
			//////////////////////////////////////////////////////////////////////////
			CLightInfo lightInfoReplace;
			lightInfoReplace.m_csLanternStatus = _T("Remove");
			lightInfoReplace.m_iLightNumber    = _ttoi(lightInfo.m_csLightNumber.Mid(1));
			lightInfoReplace.m_csLightNumber   = lightInfo.m_csLightNumber; 
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csIsStandard.Format(_T("%d"), rbpLightXD->resval.rint); 
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csLanternSchedule   = rbpLightXD->resval.rstring;
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csLanternType     	= rbpLightXD->resval.rstring;
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csLanternLamp     	= rbpLightXD->resval.rstring;
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csRateCode        	= rbpLightXD->resval.rstring;
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csLanternCategory  	= rbpLightXD->resval.rstring;
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csAssetColumn      	= rbpLightXD->resval.rstring;
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csAssetLantern     	= rbpLightXD->resval.rstring;

			//////////////////////////////////////////////////////////////////////////
			// Replace - Charge To
			//////////////////////////////////////////////////////////////////////////
			rbpLightXD = rbpLightXD->rbnext;
			iSizeOfChargeTo	= rbpLightXD->resval.rint; 
			for (int iCtr = 0; iCtr < iSizeOfChargeTo; iCtr++)
			{
				rbpLightXD = rbpLightXD->rbnext; 
				csValue.Format(_T("%s"), rbpLightXD->resval.rstring);
				if (!lightInfoReplace.m_csChargeTo.IsEmpty())
					lightInfoReplace.m_csChargeTo.Format(_T("%s\\P%s(%s%%)"), lightInfoReplace.m_csChargeTo, csValue.Mid(0, csValue.Find(_T("#"))), csValue.Mid(csValue.Find(_T("#")) + 1));
				else
					lightInfoReplace.m_csChargeTo.Format(_T("%s(%s%%)"), csValue.Mid(0, csValue.Find(_T("#"))), csValue.Mid(csValue.Find(_T("#")) + 1));
			}

			// Add this to the vector for sort
			lightInfo_Vector.push_back(lightInfoReplace);
		}

		lightInfo_Vector.push_back(lightInfo);
	}

	// Free the selection set 
	acedSSFree(ssLights);

	// Sort the light array
	sortLightInfo(lightInfo_Vector);
}

//////////////////////////////////////////////////////////////////////////
// Function name:
// Description  :
//////////////////////////////////////////////////////////////////////////
bool PopulateLightTable(AcDbTable *pTableLegend,
												CString csTxtHt,
												CStringArray &csaLightNumber,      CStringArray &csaQty,
									  		CStringArray &csaLanternStatus,    CStringArray &csaLanternType,   
												CStringArray &csaLanternStockCode, CStringArray &csaLaternLamp,
												CStringArray &csaLampStockCode, 	 CStringArray &csaLanternMast,
												CStringArray &csaMastCode, 				 CStringArray &csaLanternOutreach,
												CStringArray &csaOutreachCode, 		 CStringArray &csaRateCode, 
												CStringArray &csaLanternCategory,	 CStringArray &csaChargeTo,
												CStringArray &csaLanternBracket,   CStringArray &csaBracketCode,
												CStringArray &csaKBH,

												CStringArray &csAssetLantern, CStringArray &csaLuminaireRate,CStringArray &csaRagbolt,
												CStringArray &csaFoundationType,CStringArray &csaMASTRATE,CStringArray &csaOutreachRate
											 )
{
	// Get the objectID of the text style
	AcDbObjectId objIDTxtStyle;
	AcDbTextStyleTable *pTxtStyle = NULL;
	if (acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTxtStyle, AcDb::kForWrite) != Acad::eOk) return false;
	if (pTxtStyle->getAt(g_csSchTextStyle, objIDTxtStyle) != Acad::eOk) { pTxtStyle->close(); return false; }
	pTxtStyle->close();
  
	// Set text style in the table to "Standard"
	pTableLegend->setTextStyle(objIDTxtStyle, AcDb::kAllRows);
	pTableLegend->generateLayout(); // Very very important, else expect crashes later on

	// Get the Table Configuration
	struct resbuf *rbpTableCfg = pTableLegend->xData(_T("TABLECFG"));	if (rbpTableCfg == NULL) return false;
	rbpTableCfg = rbpTableCfg->rbnext; bool bHasLaternDetails = rbpTableCfg->resval.rint; 
	rbpTableCfg = rbpTableCfg->rbnext; bool bHasColumnDetails = rbpTableCfg->resval.rint; 
	rbpTableCfg = rbpTableCfg->rbnext; bool bHasPoleDetails   = rbpTableCfg->resval.rint; 
	//rbpTableCfg = rbpTableCfg->rbnext; bool bHasTypeCode      = rbpTableCfg->resval.rint; 
	//rbpTableCfg = rbpTableCfg->rbnext; bool bHasRateCode      = rbpTableCfg->resval.rint; 
	rbpTableCfg = rbpTableCfg->rbnext; bool bHasCategory      = rbpTableCfg->resval.rint; 
	rbpTableCfg = rbpTableCfg->rbnext; bool bHasCharge        = rbpTableCfg->resval.rint; 
	acutRelRb(rbpTableCfg);

	int iSize;
	int elements;
	int iStart = 0;
	CString csLightNumber; 
	int iCol;
	for (int iCtr = 0, iRow = 3; iCtr < csaLightNumber.GetSize(); iCtr++, iRow++)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Light numbers are to be consolidated like L1,L3-L5 etc. In the string right now it would be in the format. Say L1,L3,L5 etc.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Get all the light numbers (i.e the integer part) into an array
		int *array;	array = new int[_ttoi(csaQty.GetAt(iCtr))];
		csLightNumber = csaLightNumber.GetAt(iCtr); 
		iSize = 0;
		while (csLightNumber.Find(_T(",")) != -1)
		{
			array[iSize]  = _ttoi(csLightNumber.Mid(1, csLightNumber.Find(_T(",")) - 1)); 
			iSize++;
			csLightNumber = csLightNumber.Mid(csLightNumber.Find(_T(",")) + 1);
		}
		array[iSize] = _ttoi(csLightNumber.Mid(1)); iSize++;

		// Now consolidate them like L1,L3-L5 etc. (if any)
		csLightNumber.Empty();
		int iThis, iNext;
		bool bAdded = false;
		iStart = array[0];
		CString csLTemp;

		int iPrev;
		csLightNumber.Format(_T("L%d%s"), iStart, ((!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove"))) ? _T("r") : _T("")));
		for (int iCnt = 1; iCnt < iSize; iCnt++)
		{
			iThis = array[iCnt];
			iPrev = array[iCnt - 1];
			if (iThis > (iPrev + 1)) 
			{ 
				//////////////////////////////////////////////////////////////////////////
				// Not in sequence
				//////////////////////////////////////////////////////////////////////////
				if (iPrev == iStart) // 57 < 52
					csLTemp.Format(_T(", L%d%s"), iThis, ((!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove"))) ? _T("r") : _T(""))); 
				else 
					csLTemp.Format(_T(" - %d%s, L%d%s"), iPrev, ((!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove"))) ? _T("r") : _T("")), 
					iThis, ((!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove"))) ? _T("r") : _T("")) 
					); 

				iStart = iThis;
				csLightNumber += csLTemp;
			}
			else if (iCnt == iSize - 1)
			{
				// If we are are at the end of the loop add the last number
				if (iThis == iPrev + 1)
					csLTemp.Format(_T("-L%d%s"), iThis, ((!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove"))) ? _T("r") : _T(""))); 
				else
					csLTemp.Format(_T(", L%d%s"), iThis, ((!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove"))) ? _T("r") : _T(""))); 

				csLightNumber += csLTemp;
			}
		}
		delete[] array;


		
		//////////////////////////////////////////////////////////////////////////
		// Write the light number text finally
		//////////////////////////////////////////////////////////////////////////
		iCol = 0;
		//setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csLightNumber, csTxtHt); iCol++;
		csLTemp.Format(_T("%d"),iCtr+1) ;
		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csLTemp, csTxtHt); iCol++;// ITEM ID

		//STATE
		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternStatus.GetAt(iCtr), csTxtHt); iCol++;

		//SL NUMBER (NEW)		
		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLightNumber.GetAt(iCtr), csTxtHt); iCol++;//_T("SL NUMBER (NEW)")

		//Support Number
		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csAssetLantern.GetAt(iCtr), csTxtHt); iCol++;
		//////////////////////////////////////////////////////////////////////////////
		////// Write the light number text finally
		//////////////////////////////////////////////////////////////////////////////
		////iCol = 0;
		////setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csLightNumber, csTxtHt); iCol++;

		//////////////////////////////////////////////////////////////////////////

		////if (bHasLaternDetails)
		////{
		////	//////////////////////////////////////////////////////////////////////////
		////	// Added on 06-11-07
		////	// For : SEE Point (4) of Issues_Register on 06-11-2007
		////	//////////////////////////////////////////////////////////////////////////
		////	// Type
		////	if (csaLanternStatus.GetAt(iCtr) == _T("Existing"))
		////	{
		////		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); 
		////	}
		////	//////////////////////////////////////////////////////////////////////////
		////	// Old code
		////	//////////////////////////////////////////////////////////////////////////
		////	// Type
		////	// if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
		////	//////////////////////////////////////////////////////////////////////////
		////	else if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
		////		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternType.GetAt(iCtr), csTxtHt), csTxtHt; 
		////	else 
		////		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); 
		////	iCol++;
		////	
		////	// Stock code
		////	if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
		////		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternStockCode.GetAt(iCtr), csTxtHt); 
		////	else
		////		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); 
		////	iCol++;

		////	// Lamp
		////	if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
		////		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLaternLamp.GetAt(iCtr), csTxtHt);
		////	else
		////		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); 
		////	 iCol++;

		////	// Lamp stock code
		////	if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
		////		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLampStockCode.GetAt(iCtr), csTxtHt); 
		////	else
		////		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); 
		////	iCol++;

		////	// RMVD
		////	if (csaLanternStatus.GetAt(iCtr) == _T("Remove"))
		////		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternType.GetAt(iCtr), csTxtHt); 
		////	else
		////		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); 
		////	iCol++;
		////}

		////if (bHasColumnDetails)
		////{
		////	setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternMast.GetAt(iCtr), csTxtHt); iCol++;
		////	setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaMastCode.GetAt(iCtr), csTxtHt); iCol++;
		////	setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternOutreach.GetAt(iCtr), csTxtHt); iCol++;
		////	setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaOutreachCode.GetAt(iCtr), csTxtHt); iCol++;
		////}

		////if (bHasPoleDetails)
		////{
		////	// setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternMast.GetAt(iCtr)); iCol++;
		////	setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternBracket.GetAt(iCtr), csTxtHt); iCol++;
		////	setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaBracketCode.GetAt(iCtr), csTxtHt); iCol++;
		////	setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaKBH.GetAt(iCtr), csTxtHt); iCol++;
		////}

			if (bHasLaternDetails)
		{
			// Description
			if (csaLanternStatus.GetAt(iCtr) == _T("Existing"))
			{
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); 
			}			
			else if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternType.GetAt(iCtr), csTxtHt); 
			else 
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); 
			iCol++;

			// Part No
			if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternStockCode.GetAt(iCtr), csTxtHt); 
			else
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); 
			iCol++;

			// QTY
			/*if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLaternLamp.GetAt(iCtr), csTxtHt);
			else
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); */

			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); //Since no value QTY is left blank
			iCol++;

			// Rate code
			/*if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLampStockCode.GetAt(iCtr), csTxtHt); 
			else
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); */
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLuminaireRate.GetAt(iCtr), csTxtHt);

			iCol++;

			//iCol++;

			//// RMVD
			//if (csaLanternStatus.GetAt(iCtr) == _T("Remove"))
			//	setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternType.GetAt(iCtr), csTxtHt); 
			//else
			//	setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); 
			//iCol++;
		}

		if (bHasColumnDetails)
		{
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternMast.GetAt(iCtr), csTxtHt); iCol++;//DESCRIPTION
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaMastCode.GetAt(iCtr), csTxtHt); iCol++;//PART NO
			CString temp;
			temp.Format(csaRagbolt.GetAt(iCtr) + "/" + csaFoundationType.GetAt(iCtr));
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, temp, csTxtHt); iCol++;			
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), csTxtHt); iCol++;//QTY
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaMASTRATE.GetAt(iCtr), csTxtHt); iCol++;//RATE CODE
		}

		if (bHasPoleDetails)
		{		
			
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternOutreach.GetAt(iCtr), csTxtHt); iCol++;
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaOutreachCode.GetAt(iCtr), csTxtHt); iCol++;
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaOutreachRate.GetAt(iCtr) , csTxtHt); iCol++;
			//iCol++;
		}

		//UPCAST
		//Default value for upcast is 5°
		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("5°"), csTxtHt); 
		iCol++;

		//CATEGORY
		if (bHasCategory) 
		{
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternCategory.GetAt(iCtr), csTxtHt); 
			iCol++;
		}

		//// Quantity
		//setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, (!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove")) ? _T("-") : _T("")) + csaQty.GetAt(iCtr), csTxtHt); iCol++;


	//if (GetParameterValue(g_csaLightTblTitles, _T("UPCAST"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	//pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol); setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("UPCAST"), csTxtHt); iCol++;

	//if (bHasCategory)
	//{
	//	if (GetParameterValue(g_csaLightTblTitles, _T("CATEGORY"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	//	pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol); setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("CATEGORY"), csTxtHt); iCol++;
	//}
	if (bHasCharge)   { setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaChargeTo.GetAt(iCtr), csTxtHt); iCol++; }




		// Mail 16-07-08: The "Type" code must be the same "Rate code"
		// Old: if (bHasTypeCode) { setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternType.GetAt(iCtr)); iCol++; }

		//*******************Changed on 08-Feb-2014**************************
//		if (bHasTypeCode) { setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaRateCode.GetAt(iCtr), csTxtHt); iCol++; }
		//if (bHasRateCode) { setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaRateCode.GetAt(iCtr), csTxtHt); iCol++; }
	/*	if (bHasCategory) { setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternCategory.GetAt(iCtr), csTxtHt); iCol++; }
		if (bHasCharge)   { setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaChargeTo.GetAt(iCtr), csTxtHt); iCol++; }*/
	}

	return true;
}

/*
//////////////////////////////////////////////////////////////////////////
// Function name: ConsolidateLightNumbers()
// Description  : 
//////////////////////////////////////////////////////////////////////////
void ConsolidateLightNumbers(std:: vector <CLightInfo> &lightInfo_Vector, 
														 CStringArray &csaLightNumber, 
														 CStringArray &csaLanternStatus, 
														 CStringArray &csaLanternType, 
														 CStringArray &csaLanternStockCode, 
														 CStringArray &csaLaternLamp, 
														 CStringArray &csaLightNumberXD,
                             CStringArray &csaLampStockCode, 
														 CStringArray &csaLanternMast, 
														 CStringArray &csaMastCode,
														 CStringArray &csaLanternOutreach, 
														 CStringArray &csaOutreachCode,
														 CStringArray &csaRateCode, 
														 CStringArray &csaLanternCategory;
														 CStringArray &csaChargeTo,
														 CStringArray &csaQty,
														 CStringArray &csaLanternBracket, 
														 CStringArray &csaBracketCode, 
														 CStringArray &csaKBH
														)
{
	//////////////////////////////////////////////////////////////////////////
	// Consolidate the information of lights by combining the identical lights
	//////////////////////////////////////////////////////////////////////////
	BOOL bMatched;
	for (int i = 0; i < lightInfo_Vector.size(); i++)
	{
		bMatched = FALSE;
		csaLightNumberXD.Add(suppressZero(lightInfo_Vector.at(i).m_iLightNumber));
		for (int iCtr = 0; iCtr < csaLightNumber.GetSize(); iCtr++)
		{
			// if ((csaLanternStatus.GetAt(iCtr)  == lightInfo_Vector.at(i).m_csLanternStatus)     &&
			if ((csaLanternType.GetAt(iCtr)      == lightInfo_Vector.at(i).m_csLanternType)	 		   &&
					(csaLanternStockCode.GetAt(iCtr) == lightInfo_Vector.at(i).m_csLanternStockCode)   &&
					(csaLaternLamp.GetAt(iCtr)			 == lightInfo_Vector.at(i).m_csLanternLamp)			   &&
					(csaLampStockCode.GetAt(iCtr)		 == lightInfo_Vector.at(i).m_csLampStockCode)		   &&
					(csaLanternMast.GetAt(iCtr)			 == lightInfo_Vector.at(i).m_csLanternMast)			   &&
					(csaMastCode.GetAt(iCtr)				 == lightInfo_Vector.at(i).m_csMastCode)           &&
					(csaLanternOutreach.GetAt(iCtr)	 == lightInfo_Vector.at(i).m_csLanternOutreach)    &&
					(csaOutreachCode.GetAt(iCtr)		 == lightInfo_Vector.at(i).m_csOutreachCode)		   &&
					(csaRateCode.GetAt(iCtr)				 == lightInfo_Vector.at(i).m_csRateCode)			 	   &&
					(csaLanternCategory.GetAt(iCtr)  == lightInfo_Vector.at(i).m_csLanternCategory)    &&
					(csaChargeTo.GetAt(iCtr)				 == lightInfo_Vector.at(i).m_csChargeTo)					 &&	
					(csaLanternBracket.GetAt(iCtr)	 == lightInfo_Vector.at(i).m_csLanternBracket)     &&
					(csaBracketCode.GetAt(iCtr)			 == lightInfo_Vector.at(i).m_csBracketCode)			   &&
					(csaKBH.GetAt(iCtr)							 == lightInfo_Vector.at(i).m_csKingbolt)			     // &&
				)
			{
				csaLightNumber.SetAt(iCtr, csaLightNumber.GetAt(iCtr) + "," + lightInfo_Vector.at(i).m_csLightNumber);
				csaQty.SetAt(iCtr, suppressZero(atoi(csaQty.GetAt(iCtr)) + 1));
				bMatched = TRUE;
				break;
			}
		}

		if (!bMatched)
		{
			csaLightNumber.     Add(lightInfo_Vector.at(i).m_csLightNumber);
			csaLanternStatus.   Add(lightInfo_Vector.at(i).m_csLanternStatus);
			csaLanternType.     Add(lightInfo_Vector.at(i).m_csLanternType);
			csaLanternStockCode.Add(lightInfo_Vector.at(i).m_csLanternStockCode);
			csaLaternLamp.      Add(lightInfo_Vector.at(i).m_csLanternLamp);
			csaLampStockCode.   Add(lightInfo_Vector.at(i).m_csLampStockCode);
			csaLanternMast.     Add(lightInfo_Vector.at(i).m_csLanternMast);
			csaMastCode.        Add(lightInfo_Vector.at(i).m_csMastCode);
			csaLanternOutreach. Add(lightInfo_Vector.at(i).m_csLanternOutreach);
			csaOutreachCode.    Add(lightInfo_Vector.at(i).m_csOutreachCode);
			csaRateCode.        Add(lightInfo_Vector.at(i).m_csRateCode);
			csaLanternCategory. Add(lightInfo_Vector.at(i).m_csLanternCategory);
			csaChargeTo.        Add(lightInfo_Vector.at(i).m_csChargeTo);
			csaQty.             Add("1");
			csaLanternBracket.  Add(lightInfo_Vector.at(i).m_csLanternBracket);
			csaBracketCode.	  	Add(lightInfo_Vector.at(i).m_csBracketCode);
			csaKBH.						 	Add(lightInfo_Vector.at(i).m_csKingbolt);

			/*
			// If the status of the light is REPLACE, we must add an additional line to show the details of the replaced light
			if (!lightInfo_Vector.at(i).m_csLanternStatus.CompareNoCase("Replace"))
			{
				csaLightNumber.     Add(lightInfo_Vector.at(i).m_csLightNumber);
				csaLanternStatus.   Add(L"Remove");
				csaLanternType.     Add(lightInfo_Vector.at(i).m_csReplaceLanternType);
				csaLaternLamp.      Add(lightInfo_Vector.at(i).m_csReplaceLanternLamp);
				csaRateCode.        Add(lightInfo_Vector.at(i).m_csReplaceRateCode);
				csaLanternCategory. Add(lightInfo_Vector.at(i).m_csReplaceLanternCategory);
				csaChargeTo.        Add(lightInfo_Vector.at(i).m_csReplaceChargeTo);
				csaQty.             Add("1");

				//////////////////////////////////////////////////////////////////////////
				// Parameters not present for replaced light
				//////////////////////////////////////////////////////////////////////////
				csaLanternStockCode.Add(""); csaLampStockCode.Add(""); csaLanternMast.Add("");    csaMastCode.Add(""); 
				csaLanternOutreach.Add("");  csaOutreachCode.Add("");  csaLanternBracket.Add(""); csaBracketCode.Add("");	
				csaKBH.Add("");

				csaReplaceLanternType.Add(""); csaReplaceLanternLamp.Add("");  csaReplaceRateCode.Add(""); csaReplaceLanternCategory.Add("");
				csaReplaceChargeTo.Add("");
			}
		}
	}
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : ModifyLightTable
// Description  : Called by "Command_Edit_Light" when a light information is modified.
//                Updates the values in tables generated with the latest information in lights.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ModifyLightTable(AcDbObjectId objLight, CString csLightNumber)
{
	// Get the handle
	AcDbHandle dbLightHandle =  objLight.handle(); 
	ACHAR szLightHandle[17]; dbLightHandle.getIntoAsciiBuffer(szLightHandle);

	// Get the tables that has the records corresponding to this light.
	ads_name ssTables;
	struct resbuf *rbpFilt = acutBuildList(67, 1, -4, _T("<AND"), AcDb::kDxfRegAppName, _T("LIGHT_SCHEDULE"), AcDb::kDxfRegAppName, csLightNumber, -4, _T("AND>"), NULL);
	if (acedSSGet(_T("X"), NULL, NULL, rbpFilt, ssTables) != RTNORM) { acedSSFree(ssTables); acutRelRb(rbpFilt); return; }
	acutRelRb(rbpFilt);

	// No tables
	Adesk::Int32 lLength = 0L; acedSSLength(ssTables, &lLength);
	if (lLength == 0L) { return; }

	ads_name enTable;
	for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
	{
		acedSSName(ssTables, lCtr, enTable);

		// Open the table pointer and get the LIGHT_SCHEDULE XDATA that has the list of numbers in the table
		AcDbObjectId objTable;
		AcDbTable *pTable = NULL;
		acdbGetObjectId(objTable, enTable);
		if (acdbOpenObject(pTable, objTable, AcDb::kForRead) != Acad::eOk) { acedSSFree(ssTables); return; }

		// Get all the XDATAs attached to be table
		CString csLNo;
		struct resbuf *rbpXD = pTable->xData(NULL);
		if (rbpXD == NULL) { pTable->close(); continue; }
		
		// Get all the light numbers assigned to the table and add it to the filter list to select these lights in model space.
		rbpFilt = acutBuildList(67, 0, AcDb::kDxfRegAppName, XDATA_LIGHT, -4, _T("<OR"), NULL);
		struct resbuf *rbpTempXD;
		for (rbpTempXD = rbpXD; rbpTempXD->rbnext; rbpTempXD = rbpTempXD->rbnext)
		{
			if (rbpTempXD->restype != 1001) continue; // Not a RegAppName
			if (!_ttoi(rbpTempXD->resval.rstring)) continue; // Not a Light Number

			struct resbuf *rbpTemp;
			for (rbpTemp = rbpFilt; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
			csLNo.Format(_T("L%s"), rbpTempXD->resval.rstring);
			rbpTemp->rbnext = acutBuildList(AcDb::kDxfRegAppName, csLNo, NULL);
		}

		// Close the filter list definition.
		struct resbuf *rbpTemp;
		for (rbpTemp = rbpFilt; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
		rbpTemp->rbnext = acutBuildList(-4, _T("OR>"), NULL);

		// Get the light details sorted in the light info array.
		std::vector <CLightInfo> lightInfo_Vector;
		GetLightDetails(rbpFilt, lightInfo_Vector);

		// Free the pointers
		acutRelRb(rbpTempXD);
		acutRelRb(rbpFilt);

		CStringArray csaLightNumber, csaLanternStatus, csaLanternType, csaLanternStockCode, csaLaternLamp, csaLightNumberXD;
		CStringArray csaLampStockCode, csaLanternMast, csaMastCode, csaLanternOutreach, csaOutreachCode, csaRateCode, csaLanternCategory;
		CStringArray csaChargeTo, csaQty;
		CStringArray csaLanternBracket, csaBracketCode, csaKBH;
		CStringArray csaReplaceLanternType, csaReplaceLanternLamp, csaReplaceRateCode, csaReplaceLanternCategory, csaReplaceChargeTo;


		CStringArray csAssetLantern, csaLuminaireRate,csaRagbolt;
		CStringArray csaFoundationType,csaMASTRATE,csaOutreachRate;
		// Consolidate the information of lights by combining the identical lights
		consolidateLightInfo(lightInfo_Vector);

		// Put the consolidated information into individual arrays for tabulating them
		CString csQty;
		for (int i = 0; i < lightInfo_Vector.size(); i++)
		{
			csaLightNumber.     Add(lightInfo_Vector.at(i).m_csLightNumber); 
			csaLanternStatus.   Add(lightInfo_Vector.at(i).m_csLanternStatus);
			csaLanternType.     Add(lightInfo_Vector.at(i).m_csLanternType);
			csaLanternStockCode.Add(lightInfo_Vector.at(i).m_csLanternStockCode);
			csaLaternLamp.      Add(lightInfo_Vector.at(i).m_csLanternLamp);
			csaLampStockCode.   Add(lightInfo_Vector.at(i).m_csLampStockCode);
			csaLanternMast.     Add(lightInfo_Vector.at(i).m_csLanternMast);
			csaMastCode.        Add(lightInfo_Vector.at(i).m_csMastCode);
			csaLanternOutreach. Add(lightInfo_Vector.at(i).m_csLanternOutreach);
			csaOutreachCode.    Add(lightInfo_Vector.at(i).m_csOutreachCode);
			csaRateCode.        Add(lightInfo_Vector.at(i).m_csRateCode);
			csaLanternCategory. Add(lightInfo_Vector.at(i).m_csLanternCategory);
			csaChargeTo.        Add(lightInfo_Vector.at(i).m_csChargeTo);
			csQty.Format(_T("%d"), lightInfo_Vector.at(i).m_iQty); csaQty.Add(csQty);
			csaLanternBracket.  Add(lightInfo_Vector.at(i).m_csLanternBracket);
			csaBracketCode.	  	Add(lightInfo_Vector.at(i).m_csBracketCode);
			csaKBH.						 	Add(lightInfo_Vector.at(i).m_csKingbolt);
			csAssetLantern.		Add(lightInfo_Vector.at(i).m_csAssetLantern);
			csaLuminaireRate.	Add(lightInfo_Vector.at(i).m_csLuminaireRate);
			csaRagbolt.			Add(lightInfo_Vector.at(i).m_csRagBolt);
			csaFoundationType.	Add(lightInfo_Vector.at(i).m_csFoundationType);
			csaMASTRATE.		Add(lightInfo_Vector.at(i).m_csMASTRATE);
			csaOutreachRate.	Add(lightInfo_Vector.at(i).m_csOutreachRate);
		}
		
		// Remove all row entries except for the headers in the table
		if (pTable->upgradeOpen() != Acad::eOk) { pTable->close(); acedSSFree(ssTables); return; }
		double dRowHeight = pTable->rowHeight(3);
		pTable->deleteRows(3, pTable->numRows() - 3);
		int iRowIndex = 3;
		pTable->insertRows(iRowIndex, dRowHeight, csaLightNumber.GetSize());
		
		pTable->setNumRows(csaLightNumber.GetSize() + 3); 

		// Call the function to populate the table
		PopulateLightTable(pTable, suppressZero(pTable->textHeight()),
															 csaLightNumber,      csaQty,             csaLanternStatus,  csaLanternType,
															 csaLanternStockCode, csaLaternLamp,      csaLampStockCode,  csaLanternMast,
															 csaMastCode, 				csaLanternOutreach, csaOutreachCode,   csaRateCode,
															 csaLanternCategory,  csaChargeTo,        csaLanternBracket, csaBracketCode,
															 csaKBH,
															 csAssetLantern, csaLuminaireRate,csaRagbolt,
															 csaFoundationType,csaMASTRATE,csaOutreachRate
										  );	

		pTable->close();
	}

	acedSSFree(ssTables);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_RenumberPoles
// Description  : Called when the user enters the "AEC_Renumber" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_RenumberPoles()
{
	// Get existing pole number array
	int iMaxNumber;
	CUIntArray uiaPoleNumbers; GetInsertNumbers(XDATA_POLE, uiaPoleNumbers);

	// Format the prompt with the default new pole number
	int iPoleNumber = getMaxInArray(uiaPoleNumbers) + 1;
	CString csPrompt; csPrompt.Format(_T("\nStarting pole number <%d>: "), iPoleNumber);

	// Get the new starting pole number
	while (T)
	{
		acedInitGet(RSG_NOZERO + RSG_NONEG, _T(""));

		int iRet = acedGetInt(csPrompt, &iPoleNumber);
		if (iRet == RTCAN) return;
		else if (iRet == RTNONE) { break; }
		else if (iRet == RTNORM)
		{
			/*
			if (CheckForDuplication(uiaPoleNumbers, iPoleNumber))
			{
				acutPrintf("\nPole number \"%d\" already exists in the drawing.", iPoleNumber);
			}
			else	break;
			*/
			break;
		}
	}

	while (T)
	{
		// Ask user to select all poles to be renumbered
		Adesk::Int32 lLength = 0L;
		ads_name ssPoles;

		ads_name enPole;
		ads_point ptPick;
		acedInitGet(RSG_NONULL, _T(""));
		int iResult = acedEntSel(_T("\nSelect a pole or ENTER to select multiple poles: "), enPole, ptPick);
		struct resbuf *rbpPole = NULL;

		if (iResult == RTNORM)
		{
			// Check if XDATA_POLE is attached. Else convert it to pole
			if (getXDataFromEntity(enPole, XDATA_POLE) == NULL)
			{
				//////////////////////////////////////////////////////////////////////////
				// This is not a pole added by "Insert_Pole" of IE_Toolbox, so check if
				// this if the block name matches any of the pole blocks in the standard
				// drawing. If it matches, add the relevant XDATA to it recognize the
				// same as an insert from IE_Toolbox.
				//////////////////////////////////////////////////////////////////////////
				rbpPole = ConvertToPole(enPole);
				if (rbpPole == NULL) { acutPrintf(_T("\n*Invalid selection*. Expects a pole to be selected.")); continue;	}
				acutRelRb(rbpPole);
			}
		}
		else if (iResult == RTCAN) return;
		else 
		{
			TCHAR* prompts[2] = { _T("Select poles: "), _T("Remove poles: ") };
			// struct resbuf *rbpFilter = acutBuildList(RTDXF0, "INSERT", AcDb::kDxfRegAppName, XDATA_POLE, NULL);
			struct resbuf *rbpFilter = BuildFilterListForPole();
			if (acedSSGet(_T(":$"), prompts, NULL, rbpFilter, ssPoles) != RTNORM) { acutRelRb(rbpFilter); acedSSFree(ssPoles); return; }
			acutRelRb(rbpFilter);
			if ((acedSSLength(ssPoles, &lLength) != RTNORM) || (lLength == 0L)) {	acedSSFree(ssPoles); return; }
			iResult = RTKWORD;
		}

		CString csTag, csNumber;
		AcDbObjectId objPole;
		AcDbBlockReference *pPole = NULL;
		AcDbObjectIterator *pIter = NULL;
		AcDbAttribute *pAtt = NULL;
		struct resbuf *rbpPoleXD = NULL;

		// For each pole in the selection set (reverse order, since acedSSGet with give last entity first)
		CString csExistingPoleNumber;
		CString csNewPoleNumber;
		long lCtr = ((iResult == RTKWORD) ? lLength - 1L : 0L);
		bool bDuplicatePoleNumber = false;
		for (lCtr; lCtr > -1; lCtr--)
		{
			// Open the pole for writing
			if (iResult == RTKWORD)	acedSSName(ssPoles, lCtr, enPole);

			// If the pole XDATA is not attached do the conversion to pole now
			if (getXDataFromEntity(enPole, XDATA_POLE) == NULL)	ConvertToPole(enPole);

			// If the pole does not have the XDATA
			if (acdbGetObjectId(objPole, enPole) != Acad::eOk) continue;
			if (acdbOpenObject(pPole, objPole, AcDb::kForWrite) != Acad::eOk) continue;

			// Store the xdata attached to the pole
			rbpPoleXD = pPole->xData(XDATA_POLE);

			// Get the attribute iterator
			pIter = pPole->attributeIterator();

			// Loop through the iterator
			for (pIter->start(); !pIter->done(); pIter->step())
			{
				// Open the attribute for writing
				if (acdbOpenObject(pAtt, pIter->objectId(), AcDb::kForWrite) != Acad::eOk) continue;

				// If the tag is what we want we shall proceed
				csTag = pAtt->tag();
				if (csTag.CompareNoCase(GetPoleTag(_T("Pole number")))) { pAtt->close(); continue; }
				
				// Format the new pole number
				csNumber.Format(_T("%d"), iPoleNumber);

				// Change the text to the new value
				csExistingPoleNumber = pAtt->textString();
				csNewPoleNumber      = suppressZero(iPoleNumber);
				pAtt->setTextString(csNumber);

				// Update the xdata in the pole
				rbpPoleXD->rbnext->resval.rint = iPoleNumber;
				_tcscpy(rbpPoleXD->rbnext->rbnext->resval.rstring, csNumber);
				pPole->setXData(rbpPoleXD);

				// Increment the pole number
				uiaPoleNumbers.Add(iPoleNumber);
				iPoleNumber++;

				////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// Duplication check removed because the client says it defeats the purpose of this command itself
				////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// bDuplicatePoleNumber = CheckForDuplication(uiaPoleNumbers, iPoleNumber);
				bDuplicatePoleNumber = false;
				//////////////////////////////////////////////////////////////////////////
				if (bDuplicatePoleNumber) { acutPrintf(_T("\nWarning: Subsequent pole number \"%d\" already exists in the drawing."), iPoleNumber);	}

				// Close the attribute
				pAtt->close();
				break;
			}

			// Delete the iterator
			delete pIter;

			// Close the pole
			pPole->close();

			if (bDuplicatePoleNumber) break;

			///////////////////////////////////////////////////////////////////////////
			// Modify the pole table if it is already generated.
			//////////////////////////////////////////////////////////////////////////
			ModifyPoleTable(objPole, csExistingPoleNumber, csNewPoleNumber);
		}

		// Free the selection set
		if (iResult == RTKWORD)	{ acedSSFree(ssPoles); return; }

		if (bDuplicatePoleNumber) break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_ResizePoleText
// Description  : Called when the user enters the "AEC_ResizeText" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_ResizePoleText()
{
  // Format the prompt with the default pole diameter
  CString csPrompt; csPrompt.Format(_T("\nNew pole number height <%s>: "), g_csPoleTextHeight);

  // Get the new pole number text height
  double dPoleTextHt = 0.0;
  acedInitGet(RSG_NOZERO + RSG_NONEG, _T(""));
  int iRet = acedGetReal(csPrompt, &dPoleTextHt);
  if (iRet == RTCAN) return;
  if (iRet == RTNONE) dPoleTextHt = _tstof(g_csPoleTextHeight);

  // Ask user to select all poles to be resized
  acutPrintf(_T("\nSelect pole/light asset numbers to be resized..."));
  struct resbuf *rbpFilter = acutBuildList(RTDXF0, _T("INSERT"), -4, L"<OR", AcDb::kDxfRegAppName, XDATA_POLE, AcDb::kDxfRegAppName, XDATA_LIGHT, -4, L"OR>", NULL);
  ads_name ssPoles; if (acedSSGet(NULL, NULL, NULL, rbpFilter, ssPoles) != RTNORM) return; acutRelRb(rbpFilter);
  Adesk::Int32 lLength = 0L; if ((acedSSLength(ssPoles, &lLength) != RTNORM) || (lLength == 0L)) { acedSSFree(ssPoles); return; }
 
  ads_name enPole;
  CString csTag;
  AcDbObjectId objPole;
  AcDbBlockReference *pPole = NULL;
  AcDbAttribute *pAtt = NULL;

  // For each pole in the selection set
  for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
  {
    // Open the pole for reading
    acedSSName(ssPoles, lCtr, enPole);
    if (acdbGetObjectId(objPole, enPole) != Acad::eOk) continue;
    if (acdbOpenObject(pPole, objPole, AcDb::kForRead) != Acad::eOk) continue;

    // Get the attribute iterator and close the pole
		AcDbObjectIterator *pIter = pPole->attributeIterator();
		pPole->close();
 
		bool bIsPole  = false;
		bool bIsLight = false;
		bool bIsColmn = false;

		CString csDBTag = L""; 
		struct resbuf *rbpXD = NULL; 
		/**/ if (rbpXD = getXDataFromEntity(enPole, XDATA_POLE)) 
		{ 
			bIsPole  = true;
			csDBTag = GetPoleTag(_T("Pole number")); 
		}
		else if (rbpXD = getXDataFromEntity(enPole, XDATA_LIGHT)) 
		{
			bIsLight  = true;
			csDBTag = GetLightTag(_T("Column/pole number")); 
		}
		acutRelRb(rbpXD);
    
    // Loop through the iterator 
		CString csThisTag;
    for (pIter->start(); !pIter->done(); pIter->step()) 
    {
      // Open the attribute for writing
      if (acdbOpenObject(pAtt, pIter->objectId(), AcDb::kForWrite) != Acad::eOk) continue;

      // If the tag is what we want
      csTag = pAtt->tag();
			if ((bIsPole && !csTag.CompareNoCase(csDBTag)) || (bIsLight && !csTag.CompareNoCase(csDBTag)))
      {
        // Change the text height to the new value
        pAtt->setHeight(dPoleTextHt);
				pAtt->close();
				break;
      }

      // Close the attribute
      pAtt->close();
    }

    // Delete the iterator
    delete pIter;
  }

  // Free the selection set
  acedSSFree(ssPoles);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_ResizePoleDia
// Description  : Called when the user enters the "AEC_ResizeDia" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_ResizePoleDia()
{
  // Format the prompt with the default pole diameter
  CString csPrompt; csPrompt.Format(_T("\nNew pole diameter <%s>: "), g_csPoleDiameter);

  // Get the new pole diameter
  double dPoleDia = 0.0;
  acedInitGet(RSG_NOZERO + RSG_NONEG, _T(""));
  int iRet = acedGetReal(csPrompt, &dPoleDia);
  if (iRet == RTCAN) return;
  if (iRet == RTNONE) dPoleDia = _tstof(g_csPoleDiameter);

  // Ask user to select all poles to be resized
  acutPrintf(_T("\nSelect poles to be resized..."));
  struct resbuf *rbpFilter = acutBuildList(RTDXF0, _T("INSERT"), AcDb::kDxfRegAppName, XDATA_POLE, NULL);
  ads_name ssPoles; if (acedSSGet(NULL, NULL, NULL, rbpFilter, ssPoles) != RTNORM) return; acutRelRb(rbpFilter);
  Adesk::Int32 lLength = 0L; if ((acedSSLength(ssPoles, &lLength) != RTNORM) || (lLength == 0L))  { acedSSFree(ssPoles); return; }

  ads_name enPole;
  AcDbObjectId objPole;
  AcDbBlockReference *pPole = NULL;

  // For each pole in the selection set
  for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
  {
    // Open the pole for writing
    acedSSName(ssPoles, lCtr, enPole);
    if (acdbGetObjectId(objPole, enPole) != Acad::eOk) continue;
    if (acdbOpenObject(pPole, objPole, AcDb::kForWrite) != Acad::eOk) continue;

    // Set its new scale factor
    pPole->setScaleFactors(AcGeScale3d(dPoleDia, dPoleDia, dPoleDia));

    // Close it
    pPole->close();
  }

  // Free the selection set 
  acedSSFree(ssPoles);
}

//////////////////////////////////////////////////////////////////////////
// ****** Functions related to ADD BOND / TERMINATION command *********
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// Function			: SetDrawOrder()
// Description	: Sets the draw order of all POLE and END shackle inserts to FRONT.
/////////////////////////////////////////////////////////////////////////////////////
void SetDrawOrder()
{
	//////////////////////////////////////////////////////////////////////////
	// First get all the END SHACKLE ON TOP of the cables
	//////////////////////////////////////////////////////////////////////////
	ads_name ssGet;
	struct resbuf *rbpFilt = acutBuildList(RTDXF0, _T("INSERT"), 2, _T("End Shackle_NEW"),  /*, 2, _T("End Shackle_EXT"),*/ NULL);
	if (acedSSGet(_T("X"), NULL, NULL, rbpFilt, ssGet) == RTNORM)
	{
		// Set the draw order
		acedCommandS(RTSTR, _T(".DRAWORDER"), RTPICKS, ssGet, RTSTR, _T(""), RTSTR, _T("F"), NULL);
	}

	// Clean up garbage
	acutRelRb(rbpFilt); 
	acedSSFree(ssGet);



	//////////////////////////////////////////////////////////////////////////
	// Now get the poles on top of both cables and end shackles
	//////////////////////////////////////////////////////////////////////////
	if (g_csaPoleBlocks.GetSize() == 0) return;

	// Build the selection set filter to select all pole blocks and end shackle blocks
	rbpFilt = acutBuildList(RTDXF0, _T("INSERT"), -4, _T("<OR"), NULL);
	for (int iCtr = 0; iCtr < g_csaPoleBlocks.GetSize(); iCtr++)
	{
		struct resbuf *rbpTemp;
		for (rbpTemp = rbpFilt; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
		rbpTemp->rbnext = acutBuildList(2, g_csaPoleBlocks.GetAt(iCtr), NULL);
	}

	struct resbuf *rbpTemp;
	for (rbpTemp = rbpFilt; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
	rbpTemp->rbnext = acutBuildList(-4, _T("OR>"), NULL);

	if (acedSSGet(_T("X"), NULL, NULL, rbpFilt, ssGet) == RTNORM)
	{
		// Set the draw order
		acedCommandS(RTSTR, _T(".DRAWORDER"), RTPICKS, ssGet, RTSTR, _T(""), RTSTR, _T("F"), NULL);
	}

	// Clean up
	acutRelRb(rbpFilt); 
	acedSSFree(ssGet);
} 

//////////////////////////////////////////////////////////////////////////
// Function name: ShiftBondLayer
// Description  : Puts the end shackle insert to the appropriate layer.
// Arguments    : 1. [IN] Selection set of entities comprising a BOND
//                2. [IN] Layer name of the first cable 
//                3. [IN] Connector type
//////////////////////////////////////////////////////////////////////////
bool ShiftBondLayer(ads_name ssBond, CString csLayerName, CString csConnectorType)
{
	// The layer for a bond should be "Facet1_Facet2_Facet3_Facet4" where Facet1 = 1st facet layer of first cable, Facet2 = 2nd facet 
	// layer of first cable, facet3 will be NEW and facet4 will be the connector type
	CString csFacet1 = _T("");
	CString csFacet2 = _T("");

	if (csLayerName.Find(_T("_")) != -1)
	{
		csFacet1    = csLayerName.Mid(0, csLayerName.Find(_T('_'))); 
		csLayerName = csLayerName.Mid(csLayerName.Find(_T('_')) + 1);

		if (csLayerName.Find(_T("_")) != -1)
		{
			csFacet2    = csLayerName.Mid(0, csLayerName.Find(_T('_'))); 
			csLayerName = csLayerName.Mid(csLayerName.Find(_T('_')) + 1);
		}
	}

	// Form the layer name
	csLayerName.Format(_T("%s_%s_NEW_%s"), csFacet1, csFacet2, csConnectorType);
	
	// Create the layer if it is not there (If NOT there set the COLOR to YELLOW!)
	AcDbLayerTable *pLayerTbl;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
	
	// If the layer doesn't exist return quietly
	bool bLayerExists = true;
	if (!pLayerTbl->has(csLayerName)) bLayerExists = false; 
	pLayerTbl->close();
	
	createLayer(csLayerName, Adesk::kFalse, Adesk::kFalse);
	if (!bLayerExists) setLayerColor(csLayerName, 2);

	// Change the layer of entities in the selection set the new layer
	ChangeProperties(FALSE, ssBond, NULL, csLayerName, _T("CONTINUOUS"), NULL);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Function name: ShiftEndShackleLayer
// Description  : Puts the end shackle insert to the appropriate layer.
// Arguments    : 1. [IN] Entity name of the end shackle block
//                2. [IN] Layer name of the first cable 
//////////////////////////////////////////////////////////////////////////
bool ShiftEndShackleLayer(ads_name enShackle, CString csLayerName)
{
	// The layer for a bond should be "Facet1_Facet2_Facet3" where Facet1 = 1st facet layer of first cable, Facet2 = 2nd facet 
	// layer of first cable, facet3 will be NEW
	CString csFacet1 = _T("");
	CString csFacet2 = _T("");

	if (csLayerName.Find(_T("_")) != -1)
	{
		csFacet1    = csLayerName.Mid(0, csLayerName.Find(_T('_'))); 
		csLayerName = csLayerName.Mid(csLayerName.Find(_T('_')) + 1);

		if (csLayerName.Find(_T("_")) != -1)
		{
			csFacet2    = csLayerName.Mid(0, csLayerName.Find(_T('_'))); 
			csLayerName = csLayerName.Mid(csLayerName.Find(_T('_')) + 1);
		}
	}

	// Form the layer name
	csLayerName.Format(_T("%s_%s_NEW"), csFacet1, csFacet2);

	// Create the layer if it is not there (If NOT there set the COLOR to YELLOW!)
	AcDbLayerTable *pLayerTbl;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);

	// If the layer doesn't exist return quietly
	bool bLayerExists = true;
	if (!pLayerTbl->has(csLayerName)) bLayerExists = false;  
	pLayerTbl->close();

	// Create the layer if it is not there
	createLayer(csLayerName, Adesk::kFalse, Adesk::kFalse);
	if (!bLayerExists) setLayerColor(csLayerName, 2);

	// Change the layer of entities in the selection set the new layer
	ChangeProperties(FALSE, enShackle, NULL, csLayerName, NULL, NULL);

	// Edit USAGE attributes to HV
	// Darren: 08.11.12: When you add a HV Bond the usage on the End Shackles is incorrect it should be HV not LV. The same applies to the HV UGOH.  
	// When you add a LV Bond the End Shackles Constructed Voltage should be LV not 22kV and the Operating Voltage should be LV not 11kV. The same applies to the LV UGOH.
	AcDbObjectId objShackle; acdbGetObjectId(objShackle, enShackle);
	if (!csFacet2.IsEmpty() && !csFacet2.CompareNoCase(L"HV")) 
		ChangeAttributeValue(objShackle, L"USAGE", L"HV");
	else
	{
		ChangeAttributeValue(objShackle, L"CONSTRUCTED_VOLTAGE", L"LV");
		ChangeAttributeValue(objShackle, L"OPERATING_VOLTAGE", L"LV");
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CreateCableTermination
// Description  : Called to cut the cable line at the end nearest the given insertion point and inserts a termination line at 90° to the cable
// Arguments    : 1. [IN]  Object Id of the cable to place termination
//                2. [IN]  Insertion point of the pole block
//                3. [IN]  Scale factor of the pole block
//                4. [OUT] Start point of the cable after termination
//								5. [OUT] End point of the cable after termination
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CreateCableTermination(AcDbObjectId objCable, AcGePoint3d gePoleIns, double dRadius, ads_point ptStart, ads_point ptEnd)
{
  CString csLayer, csLineType;
  ads_point ptStub1, ptStub2;
  AcGePoint3d geCableStart, geCableEnd;
  AcDbLine *pCable = NULL, *pStub = NULL;

  // Open the cable for writing
  if (acdbOpenObject(pCable, objCable, AcDb::kForWrite, true) != Acad::eOk) return 0;

  // Get the details of the line
  geCableStart = pCable->startPoint();
  geCableEnd   = pCable->endPoint();
  csLayer      = pCable->layer();
  csLineType   = pCable->linetype();
	
  // Convert the start and end points for easier manipulation
  ptStart[X] = geCableStart.x; ptStart[Y] = geCableStart.y; ptStart[Z] = geCableStart.z; 
  ptEnd[X]   = geCableEnd.x;   ptEnd[Y]   = geCableEnd.y;   ptEnd[Z]   = geCableEnd.z;

  // If the start point of the line is nearest to the pole insertion point
	AcDbObjectId objEndShackle;
	CString csEndShackleBlock = _T("End Shackle_NEW");
	int iNearest;
	bool bEndShackleInserted = false;
  if (geCableStart.distanceTo(gePoleIns) < geCableEnd.distanceTo(gePoleIns))
  {
    // Store the flag
    iNearest = 1;

    // Calculate the new start point
    // acutPolar(asDblArray(gePoleIns), acutAngle(ptStart, ptEnd), dRadius, ptStart);
		acutPolar(asDblArray(gePoleIns), acutAngle(ptStart, ptEnd), 0.0, ptStart);
				
    // Set the new start point for the cable
    pCable->setStartPoint(AcGePoint3d(ptStart[X], ptStart[Y], ptStart[Z]));
		
		// Modified on 17.10.12 
		// Change: As the End Shackle block will be oriented _T("-|") and not inverted _T("T")
		insertBlock(csEndShackleBlock, csLayer, asDblArray(gePoleIns), dRadius, 0.0, acutAngle(ptStart, ptEnd) - PIby2, _T(""), objEndShackle, TRUE);
		// insertBlock(csEndShackleBlock, csLayer, asDblArray(gePoleIns), dRadius, 0.0, acutAngle(ptStart, ptEnd), _T(""), objEndShackle, TRUE);
  }
  else
  {
		// Otherwise, the end point of the line is nearest to the pole insertion point
    // Store the flag
    iNearest = 2;

    // Calculate the new end point
    // acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd, ptStart), dRadius, ptEnd);
		acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd, ptStart), 0.0, ptEnd);
				
    // Set the new end point for the cable
    pCable->setEndPoint(AcGePoint3d(ptEnd[X], ptEnd[Y], ptEnd[Z]));

		// Change: As the End Shackle block will be oriented _T("-|") and not inverted _T("T")
		insertBlock(csEndShackleBlock, csLayer, asDblArray(gePoleIns), dRadius, 0.0, acutAngle(ptEnd, ptStart) - PIby2, _T(""), objEndShackle, TRUE);
		// insertBlock(csEndShackleBlock, csLayer, asDblArray(gePoleIns), dRadius, 0.0, acutAngle(ptEnd, ptStart), _T(""), objEndShackle, TRUE);
  }

	// Shift the end shackle layer
	if (objEndShackle.isValid())
	{
		ads_name enShackle;
		acdbGetAdsName(enShackle, objEndShackle); 

		// Call the function that decides the layer name for the end shackle and places them appropriately.
		ShiftEndShackleLayer(enShackle, csLayer); 
	}
	
  // Close the cable
  pCable->close();
	
  // Return the nearest end
  return iNearest;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : GetPoleExtents
// Description      : Retrieves the pole extents considering only the drawn objects and not attributes.
// Arguments        : 1. [IN] Object id of the pole
//                    2. [IN] Geometric extents of the pole
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetPoleExtents(AcDbObjectId objPole, AcDbExtents &exBounds)
{
	// Open the pole for reading
	AcDbBlockReference *pPole; if (acdbOpenObject(pPole, objPole, AcDb::kForRead) != Acad::eOk) return false;

	// Get the block definition pointer
	AcDbObjectId objId = pPole->blockTableRecord();

	// Get the insertion point of the Workstation
	AcGePoint3d geIns = pPole->position();

	// Explode the workstation to get the entities in it
	AcDbVoidPtrArray objArray;
	pPole->explode(objArray);

	// Close the workstation
	pPole->close();

	// For each entity in the array, get the geomExtents if it is not a CIRCLE
	AcDbEntity *pEntity = NULL;
	AcDbExtents exItem;
	AcGePoint3d geMin, geMax;
	CString csItemExtents, csMinX, csMinY, csMinZ, csMaxX, csMaxY, csMaxZ;
	double dMinX = 100000000.0, dMinY = 100000000.0, dMinZ = 100000000.0, dMaxX = -100000000.0, dMaxY = -100000000.0, dMaxZ = -100000000.0;
	AcDbObjectId objID;

	for (int iCtr = 0; iCtr < objArray.length(); iCtr++)
	{
		pEntity = (AcDbEntity *)objArray.at(iCtr);

		if (pEntity == NULL) continue;
		if (AcDbText::cast(pEntity))      { pEntity->close(); continue; }
		else if (AcDbAttribute::cast(pEntity)) { pEntity->close(); continue; }

		pEntity->getGeomExtents(exItem);
		pEntity->close();

		// Store the formatted extents in the array
		geMin = exItem.minPoint();
		geMax = exItem.maxPoint();

		// Update the minimum extents, if required
		if (geMin.x < dMinX) dMinX = geMin.x;
		if (geMin.y < dMinY) dMinY = geMin.y;
		if (geMin.z < dMinZ) dMinZ = geMin.z;

		// Update the maximum extents, if required
		if (geMax.x > dMaxX) dMaxX = geMax.x;
		if (geMax.y > dMaxY) dMaxY = geMax.y;
		if (geMax.z > dMaxZ) dMaxZ = geMax.z;
	}

	// Set the minimum and maximum extents
	geMin.x = dMinX; geMin.y = dMinY; geMin.z = dMinZ;
	geMax.x = dMaxX; geMax.y = dMaxY; geMax.z = dMaxZ;

	// Set the extents
	exBounds.set(geMin, geMax);

	// Success
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Function name: printHandle
// Description  : Helper function to print handle of an object
//////////////////////////////////////////////////////////////////////////
void printHandle(AcDbObjectId objId, int iLine) 
{
	AcDbHandle dbHandle =  objId.handle(); ACHAR szHandle[17]; dbHandle.getIntoAsciiBuffer(szHandle);
	acutPrintf(_T("\nHandle %s at %d"), szHandle, iLine);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CreateCableChainTerminationInBondNew
// Description  : Adds a termination symbol at the end of a cable chain.
// Arguments    : 1. [IN]  Object Id of the cable chain
//                2. [IN]  Object Id of the pole
//								3. [OUT] Start point of the terminated cable
//                4. [OUT] End point of the terminated cable
// Returns      : Flag indicating where the termination symbol is added.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CreateCableChainTerminationInBondNew(AcDbObjectId objCable, AcDbObjectId objPole, ads_point ptStart, ads_point ptEnd, struct resbuf *rbpXDExist)
{
	int iVertexIndicator = 0;

	// Get the poles insertion point, scale factor
	AcDbBlockReference *pPole = NULL;
	if (acdbOpenObject(pPole, objPole, AcDb::kForRead) != Acad::eOk) return iVertexIndicator;

	AcGePoint3d gePoleIns   = pPole->position();
	AcGeScale3d gePoleScale = pPole->scaleFactors();
	pPole->close();

	// Get the start point & end point, line type and layer of this segment
	AcDbLine *pCableSeg = NULL; 
	Acad::ErrorStatus es = acdbOpenObject(pCableSeg, objCable, AcDb::kForRead);
	if (es != Acad::eOk) 
	{ 
		acutPrintf(_T("\nError at %d: %s"), __LINE__, acadErrorStatusText(es)); 
		printHandle(objCable, __LINE__);
		return iVertexIndicator; 
	}

	AcGePoint2d geSegStart(pCableSeg->startPoint()[X], pCableSeg->startPoint()[Y]);
	AcGePoint2d geSegEnd(pCableSeg->endPoint()[X], pCableSeg->endPoint()[Y]);
	CString csLType; csLType.Format(_T("%s"), pCableSeg->linetype()); 
	CString csLayer; csLayer.Format(_T("%s"), pCableSeg->layer());

	// Get the handle of the parent chain to which this segment belongs
	struct resbuf *rbpChain = NULL; rbpChain = pCableSeg->xData(_T("IE_TB_CHAIN_HNDL"));
	if (!rbpChain) { return iVertexIndicator; }
	CString csChainHndl = rbpChain->rbnext->resval.rstring;

	pCableSeg->close(); 
	acutRelRb(rbpChain); 

	// Open the source polyline to process and iterate through the vertex in it.
	ads_name enChain; 
	if (acdbHandEnt(csChainHndl, enChain) != RTNORM) { return iVertexIndicator; }
	
	AcDbObjectId objChain;	es = acdbGetObjectId(objChain, enChain);
	if (es != Acad::eOk) { acutPrintf(_T("\nError at %d: %s"), __LINE__, acadErrorStatusText(es)); return iVertexIndicator; }

	AcDbPolyline *pChain = NULL;
	es = acdbOpenObject(pChain, objChain, AcDb::kForWrite, true);
	if (es != Acad::eOk) { acutPrintf(_T("\nError at %d: %s"), __LINE__, acadErrorStatusText(es)); return iVertexIndicator; }

	// Create a new polyline entity by appending the vertex one after the other until it matches with the end points of this segment
	AcGePoint2d geVertex1, geVertex2;
	AcDbPolyline *pNewChain = new AcDbPolyline(); pNewChain->setLayer(csLayer); pNewChain->setLinetype(csLType);

	int iCtr;
	for (iCtr = 0; iCtr < pChain->numVerts(); iCtr++)
	{
		// Get the vertex at the current index. For index = 0, get the start point and continue because we need two vertice's to make a segment.
		pChain->getPointAt(iCtr, (!iCtr ? geVertex1 : geVertex2));
		if (!iCtr) continue;

		// With index > 0, we will have two vertice's to compare if they match with the segment to add termination.
		if ((geVertex1 == geSegStart) && (geVertex2 == geSegEnd))
		{
			//////////////////////////////////////////////////////////////////////////
			// Segment matched
			//////////////////////////////////////////////////////////////////////////
			// Create a termination for this cable and get the end point indicator that suggests whether the "start" or "End" point was modified
			// "1" = "Start" and "2" = "End" are the indicators
			iVertexIndicator = CreateCableTermination(objCable, gePoleIns, gePoleScale.sx, ptStart, ptEnd);

			// If the segment is the starting segment of the chain && if its start point is modified. If it was not the first segment then we would have
			// already added the vertex during the previous iteration. See "Segment unmatched" section.
			if (iCtr == 1) pNewChain->addVertexAt(0, AcGePoint2d(ptStart[X], ptStart[Y]));	

			// Add the end point only if...
			// A. End point was terminated (OR)
			// B. Start point of the first segment was terminated
			if ((iVertexIndicator == 2) || ((iVertexIndicator == 1) && (iCtr == 1))) 
			{
				pNewChain->addVertexAt(iCtr, AcGePoint2d(ptEnd[X], ptEnd[Y]));	
			}

			// Code added so that the start vertex of the remaining polyline is modified to location of "T"
			if (iVertexIndicator == 1) { pChain->setPointAt(iCtr - 1, AcGePoint2d(ptStart[X], ptStart[Y]));	}

			// We would like to continue with the iteration of other vertex's if this was the first segment of the chain.
      if ((iCtr == 1) && (iVertexIndicator == 1)) continue;
  		
			// Add the new polyline to database
			appendEntityToDatabase(pNewChain);
			if (rbpXDExist != NULL)	
			{
				pNewChain->setXData(rbpXDExist);

				// For "AddBond" command temporary XDATA "IE_TB_CHAIN_HNDL" added must be removed before attaching the new cable with existing XDATA
				struct resbuf *rbpRemoveXD = acutBuildList(AcDb::kDxfRegAppName, _T("IE_TB_CHAIN_HNDL"), NULL);
				pNewChain->setXData(rbpRemoveXD);
			}
			pNewChain->close();
			break;
		}
		else 
		{
			//////////////////////////////////////////////////////////////////////////
			// Segment unmatched
			//////////////////////////////////////////////////////////////////////////
			// If the segment is the starting segment of the chain, then its start point has to be added first before adding the end point.
			if (iCtr == 1) pNewChain->addVertexAt(0, geVertex1);
			geVertex1 = geVertex2;
			pNewChain->addVertexAt(iCtr, geVertex2);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Modify the invisible cable chain by removing the vertice's already processed
	////////////////////////////////////////////////////////////////////////////////////
	// Attach the XDATA to this so that this polyline will not be brought back to life again.
	acdbRegApp(_T("IE_TB_CHAIN_REMOVED"));
	struct resbuf *rbpChainCopy = acutBuildList(AcDb::kDxfRegAppName, _T("IE_TB_CHAIN_REMOVED"), AcDb::kDxfXdInteger16, 0, NULL);

	if (iCtr == pChain->numVerts())
	{
		// If this is the last vertex, then remove the invisible cable chain
		appendEntityToDatabase(pNewChain);
		if (rbpXDExist != NULL)	
		{
			pNewChain->setXData(rbpXDExist);

			// For "AddBond" command temporary XDATA "IE_TB_CHAIN_HNDL" added must be removed before attaching the new cable with existing XDATA
			struct resbuf *rbpRemoveXD = acutBuildList(AcDb::kDxfRegAppName, _T("IE_TB_CHAIN_HNDL"), NULL);
			pNewChain->setXData(rbpRemoveXD);
		}

		pNewChain->close();

		pChain->erase(Adesk::kTrue);
		pChain->setXData(rbpChainCopy); acutRelRb(rbpChainCopy);
		pChain->close();
		
		return iVertexIndicator;
	}

	////////////////////////////////////////////////////////////////////////// 
	// Remove the vertex processed
	//////////////////////////////////////////////////////////////////////////

	// Old: for (int iRemove = 0;  iRemove < iCtr - (iVertexIndicator == 1) ? 1 : 0; iRemove++)
	// Code added so that the start vertex of the remaining polyline is modified to location of "T"
	for (int iRemove = 0;  iRemove < iCtr - (iVertexIndicator == 1) ? 1 : 0; iRemove++)
	{
		// Here the index is always zero, because if a vertex is removed the next vertex will take its place
		es = pChain->removeVertexAt(0);
		if (es == Acad::eInvalidIndex) { pChain->erase(Adesk::kTrue);	pChain->setXData(rbpChainCopy); acutRelRb(rbpChainCopy); break; }
	}

	pChain->close();

	// Remove the parent polyline
	// Attach the XDATA to this so that this polyline will not be brought back to life again.
	//acdbRegApp(_T("IE_TB_CHAIN_REMOVED"));
	//struct resbuf *rbpChainCopy = acutBuildList(AcDb::kDxfRegAppName, _T("IE_TB_CHAIN_REMOVED"), AcDb::kDxfXdInteger16, 0, NULL);
	//pChain->erase(Adesk::kTrue);
	//pChain->setXData(rbpChainCopy); acutRelRb(rbpChainCopy);
	//pChain->close();
	
	return iVertexIndicator;
} // objCable

////////////////////////////////////////////////////////////////////////////////////////
// Function name: GetCablesAttachedToPole()
// Description  : Retrieves the selection set of cables attached to the given pole.
// Arguments    : 1. [IN]  Object Id of the pole INSERT
//								2. [OUT] Array of cable handles attached to the given pole
////////////////////////////////////////////////////////////////////////////////////////
int GetCablesAttachedToPole(AcDbObjectId objPole, CStringArray &csaCableHandles)
{
	csaCableHandles.RemoveAll();

	// Get its insertion point, scale factor and the handles of the attached poles
	AcDbBlockReference *pPole; if (acdbOpenObject(pPole, objPole, AcDb::kForRead) != Acad::eOk) return csaCableHandles.GetSize();
	AcGePoint3d gePoleIns; gePoleIns = pPole->position();
	pPole->close();

	// Get the extents of the pole without considering the Attributes in it.
	AcDbExtents extents; GetPoleExtents(objPole, extents);

	// Get the cables connected to the selected pole by determining the lines connected to it.
	ads_name ssCables;
	struct resbuf *rbpFilt = acutBuildList(RTDXF0, _T("LINE"), NULL);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// UAT - 76 Cables connected at a small distance away from the exact center must also be considered.
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// if (acedSSGet("C", asDblArray(gePoleIns), asDblArray(gePoleIns), rbpFilt, ssCables) != RTNORM)
	if (acedSSGet(_T("C"), asDblArray(extents.minPoint()), asDblArray(extents.maxPoint()), rbpFilt, ssCables) != RTNORM)
	{ 
		acedSSFree(ssCables); acutRelRb(rbpFilt); return csaCableHandles.GetSize();
	}

	acutRelRb(rbpFilt);

	// Get the handles of the lines and assign it to the array of cable handles
	ads_name enCable;
	AcDbObjectId objCable;
	AcDbHandle dbCableHandle;
	ACHAR szCableHandle[17];
	CString csCableHandle;

	Adesk::Int32 lLength = 0L; acedSSLength(ssCables, &lLength);
	for (Adesk::Int32 lCtr = 0; lCtr < lLength; lCtr++)
	{
		// Get the object ID of the cables
		acedSSName(ssCables, lCtr, enCable);
		acdbGetObjectId(objCable, enCable);

		dbCableHandle =  objCable.handle(); 
		dbCableHandle.getIntoAsciiBuffer(szCableHandle); csCableHandle.Format(_T("%s"), szCableHandle);
		csaCableHandles.Add(csCableHandle);
	}

	acedSSFree(ssCables); 
	return csaCableHandles.GetSize();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	  : GetEndPoint
// Description	    : Determines the start/end point of a line/point/arc/ellipse
// Argument         : 1. [IN]  Entity name of an entity
//                    2. [OUT] End point of the entity
//                    3. [IN]  Flag to say START/END point to retrieve
// Returns    		  : Success state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL GetEndPoint(ads_name enEntity, ads_point ptEnd, BOOL bIsEnd)
{
	struct resbuf *rbpEnd;
	if (!(rbpEnd = acdbEntGet(enEntity)))
	{
		appError(__FILE__, _T("GetEndPoint()"), __LINE__, _T("Unable to retrieve entity data."));
		return FALSE;
	}

	if (!_tcsicmp(Assoc(rbpEnd, 0)->resval.rstring, _T("LINE")) || !_tcsicmp(Assoc(rbpEnd, 0)->resval.rstring, _T("POINT")))
	{
		if (bIsEnd)
		{
			ptEnd[X] = Assoc(rbpEnd, 11)->resval.rpoint[X];
			ptEnd[Y] = Assoc(rbpEnd, 11)->resval.rpoint[Y]; 
			ptEnd[Z] = 0.0;
		}
		else
		{
			ptEnd[X] = Assoc(rbpEnd, 10)->resval.rpoint[X];
			ptEnd[Y] = Assoc(rbpEnd, 10)->resval.rpoint[Y]; 
			ptEnd[Z] = 0.0;
		}
	}
	else if (!_tcsicmp(Assoc(rbpEnd, 0)->resval.rstring, _T("ARC")))
	{
		// Get the center point
		ads_point ptCenter = { Assoc(rbpEnd, 10)->resval.rpoint[X], Assoc(rbpEnd, 10)->resval.rpoint[Y], 0.0 };
		double dRadius     = Assoc(rbpEnd, 40)->resval.rreal;
		double dStartAng   = Assoc(rbpEnd, 50)->resval.rreal;
		double dEndAng     = Assoc(rbpEnd, 51)->resval.rreal;

		if (bIsEnd)
			acutPolar(ptCenter, dEndAng, dRadius, ptEnd);
		else
			acutPolar(ptCenter, dStartAng, dRadius, ptEnd);
	}
	else
	{
		// If the end point required is at end of the ellipse, then get the perimeter of the ellipse
		acedCommandS(RTSTR, _T(".LENGTHEN"), RTLB, RTENAME, enEntity, RTPOINT, ptEnd, RTLE, RTSTR, _T(""), NULL);
		struct resbuf rbLength; acedGetVar(_T("PERIMETER"), &rbLength);

		AcDbObjectId objID;
		if (acdbGetObjectId(objID, enEntity) != Acad::eOk) return FALSE;

		AcDbEllipse *pElli = NULL;
		if (acdbOpenObject(pElli, objID, AcDb::kForRead) != Acad::eOk) return FALSE;

		AcGePoint3d ptrxPoint;
		if (!bIsEnd) 
			pElli->getPointAtDist(0, ptrxPoint);
		else
			pElli->getPointAtDist(rbLength.resval.rreal, ptrxPoint);
		pElli->close();

		ptEnd[X] = ptrxPoint[X];
		ptEnd[Y] = ptrxPoint[Y];
		ptEnd[Z] = 0.0;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Function     : SetBackgroundColorToEntity
// Description  : Sets the given entity color to match the background color.
//                This way the given entity is made invisible.
// Arguments    : [IN] Entity name of the cable to make it invisible.
//////////////////////////////////////////////////////////////////////////
void SetEditorColorToEntity(ads_name enEntity)
{
	// Get the current AutoCAD background color
	AcColorSettings colSet; acedGetCurrentColors(&colSet);
	COLORREF crBack = (COLORREF)colSet.dwGfxModelBkColor;
	AcCmColor colBack; colBack.setRed(GetRValue(crBack)); colBack.setGreen(GetGValue(crBack));colBack.setBlue(GetBValue(crBack));

	// Open the object and assign the editor color to it
	AcDbObjectId objId; acdbGetObjectId(objId, enEntity);
	AcDbEntity *pEntity;
	if (acdbOpenObject(pEntity, objId, AcDb::kForWrite) != Acad::eOk) return;
	pEntity->setColor(colBack);
	pEntity->close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_TerminateCable
// Description  : Called when the user enters the "AEC_AddTerm" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_TerminateCable()
{ 
	switchOff();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Added on 27.06.11
	// Feedback: Adding a terminal removes some other cable elsewhere in the drawing.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Collect all the cables with "IE_TB_CHAIN_HNDL" XDATA and remove this XDATA that is attached. These XDATA are used by the command during the life of this function and
	// the entities with this XDATA are removed. In case if these are not removed due to some reason, these are to be removed here.
	ads_name ssClean; 
	struct resbuf *rbpClean = acutBuildList(AcDb::kDxfRegAppName, _T("IE_TB_CHAIN_HNDL"), NULL);

	if (acedSSGet(_T("X"), NULL, NULL, rbpClean, ssClean) == RTNORM)
	{
		Adesk::Int32 lClean = 0L;	acedSSLength(ssClean, &lClean);

		ads_name enClean;
		AcDbObjectId objClean;
		AcDbPolyline *pPline;
		double dLength = 0.0;
							
		for (Adesk::Int32 lCtr = 0; lCtr < lClean; lCtr++)
		{
			// Open the entity to remove the XDATA attached to the entity
			acedSSName(ssClean, lCtr, enClean);
			acdbGetObjectId(objClean, enClean);
			
			if (acdbOpenObject(pPline, objClean, AcDb::kForWrite) != Acad::eOk) continue;
			
			// If the PLINE is of ZERO Length, remove the entity or else remove the XDATA attached to it
			double dTotalLength = 0L;
			AcGePoint2d ptrx1;
			AcGePoint2d ptrx2;

			for (int iV = 0; iV < pPline->numVerts() - 1; iV++)
			{
				pPline->getPointAt(iV, ptrx1);
				pPline->getPointAt(iV + 1, ptrx2);
				
				dTotalLength += acutDistance(asDblArray(ptrx1), asDblArray(ptrx2));
			}

			if (dTotalLength <= 0.0)
				pPline->erase();
			else
				pPline->setXData(rbpClean);

			// Close the PLINE
			pPline->close();
		}

		// Release buffer
		acutRelRb(rbpClean);
		acedSSFree(ssClean);
	}

	// Ask the user to select the poles whose attached cables have to be terminated
	TCHAR* prompts[2] = { _T("Select poles: "), _T("Remove poles: ") };
	struct resbuf *rbpFilter = BuildFilterListForPole();

	ads_name ssPoles;
	if (acedSSGet(_T(":$"), prompts, NULL, rbpFilter, ssPoles) != RTNORM) { acutRelRb(rbpFilter); acedSSFree(ssPoles); return; }
	acutRelRb(rbpFilter);

	Adesk::Int32 lLength = 0L;
	if ((acedSSLength(ssPoles, &lLength) != RTNORM) || (lLength == 0L)) { acedSSFree(ssPoles); return; }

	ads_name enPole;
	ads_point ptStart, ptEnd;
	AcGeScale3d gePoleScale;
	AcGePoint3d gePoleIns;
	AcDbObjectId objPole, objCable;
	AcDbBlockReference *pPole = NULL;

	// For each pole in the selection set
	struct resbuf *rbpFilt = acutBuildList(67, 0, RTDXF0, _T("LWPOLYLINE"), NULL); 
	for (long lCtr = 0L; lCtr < lLength; lCtr++)
	{
		// Get the entity name of the pole from the selection set 
		acedSSName(ssPoles, lCtr, enPole);

		// If the pole XDATA is not attached convert it to a pole now
		if (getXDataFromEntity(enPole, XDATA_POLE) == NULL) 
		{
			if (!ConvertToPole(enPole)) { acutPrintf(_T("\nThat is not a valid pole.\n")); continue; }
		}

		// Get its insertion point, scale factor and the handles of the attached poles
		if (acdbGetObjectId(objPole, enPole) != Acad::eOk) continue;
		if (acdbOpenObject(pPole, objPole, AcDb::kForRead) != Acad::eOk) continue;

		gePoleIns   = pPole->position();
		gePoleScale = pPole->scaleFactors();
		pPole->close();

		// Get the extents of the pole without considering the Attributes in it.
		AcDbExtents extents; if (!GetPoleExtents(objPole, extents)) continue;

		// Get the cables connected to the selected pole by determining the polylines connected to it.
		ads_name ssLWCables;
		// if (acedSSGet("C", asDblArray(gePoleIns), asDblArray(gePoleIns), rbpFilt, ssLWCables) != RTNORM)
		if (acedSSGet(_T("C"), asDblArray(extents.minPoint()), asDblArray(extents.maxPoint()), rbpFilt, ssLWCables) != RTNORM)
		{ 
			acutPrintf(_T(" There are no cables attached to this pole.")); 
			acedSSFree(ssLWCables); 
			continue;
		}

		// 1. Duplicate each LWPOLYLINE (cable chain) connected and make the duplicate invisible.
		// 2. Explode the source LWPOLYLINE connected to the pole. 
		// 3. To each resultant LINE (cable segments) add the invisible LWPOLYLINE handle to it.
		// 4. Get the cable segment 

		ads_name enLWCable, ssCables;
		Adesk::Int32 lLWLength = 0L; acedSSLength(ssLWCables, &lLWLength);

		ads_name ssCablesAttached;
		CStringArray csaCablesAttached;
		TCHAR pszCableHandle[17];
		ads_point ptStart, ptEnd;
		ads_name enCable, enCopyChain;
		AcDbObjectId objCable;
		struct resbuf *rbpChainHndl = NULL;

		struct resbuf *rbpChainCopy = acutBuildList(AcDb::kDxfRegAppName, _T("IE_TB_CHAIN"), AcDb::kDxfXdInteger16, 0, NULL);
		acdbRegApp(_T("IE_TB_CHAIN"));

		struct resbuf *rbpXDExist = NULL;
		for (long lCnt = 0L; lCnt < lLWLength; lCnt++)
		{
			acedSSName(ssLWCables, lCnt, enLWCable);

			// Get the existing data attached to the 
			AcDbObjectId objWire; acdbGetObjectId(objWire, enLWCable);
			AcDbEntity *pWire;
			rbpXDExist = NULL;
			if (acdbOpenObject(pWire, objWire, AcDb::kForRead) == Acad::eOk)
			{
				// Get all the XDATA that is prevalent in the pole edited.
				rbpXDExist = pWire->xData(NULL);
				pWire->close();
			}
			
			// Before exploding copy this polyline. So that if a segment of the polyline is not selected, we can retrieve it back.
			// Here the new polyline must be made invisible else selection of segments cannot be possible.
			acedCommandS(RTSTR, _T(".COPY"), RTENAME, enLWCable, RTSTR, _T(""), RTSTR, _T("0,0"), RTSTR, _T(""), NULL);
			acdbEntLast(enCopyChain); AcDbObjectId objIdChain; acdbGetObjectId(objIdChain, enCopyChain);
			AcDbHandle dbChainHandle =  objIdChain.handle(); 
			ACHAR szChainHandle[17]; dbChainHandle.getIntoAsciiBuffer(szChainHandle); 

			// Make it invisible
			AcDbPolyline *pObjChainHndl = NULL;
			if (acdbOpenObject(pObjChainHndl, objIdChain, AcDb::kForWrite) != Acad::eOk) continue;
			pObjChainHndl->setVisibility(AcDb::kInvisible);
			pObjChainHndl->erase(Adesk::kTrue);
			pObjChainHndl->setXData(rbpChainCopy);
			pObjChainHndl->close();

			acedCommandS(RTSTR, _T(".EXPLODE"), RTENAME, enLWCable, NULL);

			// Get the exploded information
			if (acedSSGet(_T("P"), NULL, NULL, NULL, ssCables) != RTNORM)	{	acutPrintf(_T("\nUnable to retrieve segment information of cables.")); continue; }
			rbpChainHndl = acutBuildList(AcDb::kDxfRegAppName, _T("IE_TB_CHAIN_HNDL"), AcDb::kDxfXdAsciiString,  szChainHandle, NULL);

			ads_name enExSeg;
			Adesk::Int32 lExSegLen = 0L; acedSSLength(ssCables, &lExSegLen);
			for (Adesk::Int32 lExSeg = 0L; lExSeg < lExSegLen; lExSeg++)
			{
				acedSSName(ssCables, lExSeg, enExSeg);
				addXDataToEntity(enExSeg, rbpChainHndl);
			}

			acutRelRb(rbpChainHndl); 
			acedSSFree(ssCables);

			// Get the handles of the cable segments attached to this pole
			csaCablesAttached.RemoveAll();
			int iCablesAttached = GetCablesAttachedToPole(objPole, csaCablesAttached);

			for (int iCableIndex = csaCablesAttached.GetSize() - 1; iCableIndex >= 0; iCableIndex--)
			{
				// Get the entity name of each segment
				_tcscpy(pszCableHandle, csaCablesAttached.GetAt(iCableIndex)); acdbHandEnt(pszCableHandle, enCable); acdbGetObjectId(objCable, enCable);

				// Call the function that adds a cable termination if the pole is selected for bond
				CreateCableChainTerminationInBondNew(objCable, objPole, ptStart, ptEnd, rbpXDExist);
			}

			// Free the linked list
			if (rbpXDExist != NULL) acutRelRb(rbpXDExist);

			// Erase all the exploded polyline segments
			struct resbuf *rbpChainSegFilt = acutBuildList(67, 0, AcDb::kDxfRegAppName, _T("IE_TB_CHAIN_HNDL"), NULL);
			ads_name ssChainSegs;
			if (acedSSGet(_T("X"), NULL, NULL, rbpChainSegFilt, ssChainSegs) == RTNORM)
			{
				acedCommandS(RTSTR, _T(".ERASE"), RTPICKS, ssChainSegs, RTSTR, _T(""), NULL);
				acedSSFree(ssChainSegs);
			}
			acutRelRb(rbpChainSegFilt);
		}

		// Release the buffer
		acedSSFree(ssLWCables);
	}

	// Free the selection set
	acutRelRb(rbpFilt);
	acedSSFree(ssPoles);

	// Set the draw order of all pole ans end shackle blocks to front
	SetDrawOrder();
}

/////////////////////////////////////////////////////
// Function name: 
// Description  :
// Arguments    :
// Called by    :
/////////////////////////////////////////////////////
void InsertVertexAtPole(ads_name enChain, AcGePoint3d gePoleIns)
{
	// Get the closest point on the polyline (this is required to insert a new vertex on lines that doesn't have a vertex at pole insertion!)
	AcGePoint3d geClosest;
	AcDbPolyline *pObjChain;
	AcDbObjectId objChain; acdbGetObjectId(objChain, enChain);
	if (acdbOpenObject(pObjChain, objChain, AcDb::kForWrite) != Acad::eOk) return;

	Acad::ErrorStatus es = pObjChain->getClosestPointTo(gePoleIns, geClosest, Adesk::kFalse);
	if (pObjChain->getClosestPointTo(gePoleIns, geClosest, Adesk::kFalse) != Acad::eOk) return;
	
	bool bVertexAtPole = false;
	AcGePoint3d geVert;
	int iVertexAt = -1;
	AcGePoint2d geVertexAfter;

	int iV;
	for (iV = 0; iV < pObjChain->numVerts(); iV++) 
	{
		// Get the vertex at the current index. For index = 0, get the start point and continue because we need two vertice's to make a segment.
		pObjChain->getPointAt(iV, geVert);
		double dDist = floor(acutDistance(asDblArray(geVert), asDblArray(gePoleIns)));
		if (!dDist) { bVertexAtPole = true; break; }

		AcGeLineSeg2d line2d;
		if (pObjChain->getLineSegAt(iV, line2d) == Acad::eOk)
		{
			if (line2d.isOn(AcGePoint2d(geClosest.x, geClosest.y), AcGeContext::gTol) == Adesk::kTrue) iVertexAt = iV;
		}
	}

	if (!bVertexAtPole && (iVertexAt != -1)) pObjChain->addVertexAt(iVertexAt + 1, AcGePoint2d(gePoleIns.x, gePoleIns.y));
	pObjChain->close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_AddBond to Chain
// Description  : Called when the user enters the "AEC_AddBond" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_AddBond()
{
	int iRet = 0;
	BOOL bOkay = FALSE;
	ads_name enPole;
	ads_point ptDummy;
	AcGePoint3d gePoleIns;
	AcGeScale3d gePoleScale;
	CStringArray csaCableHandles;
	CStringArray csaBondHandles;
	AcDbObjectId objPole;
	CStringArray csaCablesAttached;
	AcDbObjectIdArray objIdArray;
	
	// Switch off
	switchOff();

	// Begin a transaction, so that we can abort it if user cancels
	acTransactionManagerPtr()->startTransaction();
	
	// Loop until the user selects a pole or cancels
	struct resbuf *rbpChainCopy = acutBuildList(AcDb::kDxfRegAppName, _T("IE_TB_CHAIN"), AcDb::kDxfXdInteger16, 0, NULL);
	acdbRegApp(_T("IE_TB_CHAIN"));

	while (bOkay == FALSE)
	{
		// Ask user to select a pole
		acutPrintf(_T("\nSelect a pole with cables attached to it..."));
		iRet = acedEntSel(_T("\nSelect pole: "), enPole, ptDummy);
		if (iRet == RTCAN) { acTransactionManagerPtr()->abortTransaction(); return; }
		if (iRet != RTNORM) continue;

		// If this is a valid pole
		if (getXDataFromEntity(enPole, XDATA_POLE)) 
		{
			// Get its insertion point, scale factor and the handles of the attached poles
			if (acdbGetObjectId(objPole, enPole) != Acad::eOk) continue;
			AcDbBlockReference *pPole; if (acdbOpenObject(pPole, objPole, AcDb::kForRead) != Acad::eOk) continue;
			gePoleIns = pPole->position();
			gePoleScale = pPole->scaleFactors();
			pPole->close();

			// Get the cables connected to the selected pole by determining the polyline's connected to its center.
			ads_name ssLWCables; 
			struct resbuf *rbpFilt = acutBuildList(67, 0, RTDXF0, _T("LWPOLYLINE"), NULL); 

			////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// UAT - 76 Cables connected at a small distance away from the exact center must also be considered.
			////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Get the geometric extents of the pole
			AcDbExtents extents; if (!GetPoleExtents(objPole, extents)) continue;

			// if (acedSSGet("C", asDblArray(gePoleIns), asDblArray(gePoleIns), rbpFilt, ssLWCables) != RTNORM)
			if (acedSSGet(_T("C"), asDblArray(extents.minPoint()), asDblArray(extents.maxPoint()), rbpFilt, ssLWCables) != RTNORM)
			{ 
				acutPrintf(_T(" There are no cables attached to this pole."));
				acedSSFree(ssLWCables); 
				continue;
			}

			acutRelRb(rbpFilt);

			// Length of LWPolylines connected to the pole
			Adesk::Int32 lLWLength = 0L; acedSSLength(ssLWCables, &lLWLength);
			ads_name enLWCable, ssCables;
			struct resbuf *rbpChain = NULL, *rbpChainID = NULL, *rbpChainHndl = NULL;
			ads_name enCopyChain;
			Acad::ErrorStatus es;

			for (Adesk::Int32 lCnt = 0L; lCnt < lLWLength; lCnt++)
			{
				// Get the entity name of the polyline connected to the pole
				acedSSName(ssLWCables, lCnt, enLWCable);

				// KMK: 09-07-12 To address cables without a vertex at the pole inserts
				// Insert vertex at pole center
				InsertVertexAtPole(enLWCable, gePoleIns);

				// Before exploding copy this polyline. So that if a segment of the polyline is not selected, we can retrieve it back.
				// Here the new polyline must be made invisible else selection of segments cannot be possible.
				acedCommandS(RTSTR, _T(".COPY"), RTENAME, enLWCable, RTSTR, _T(""), RTSTR, _T("0,0"), RTSTR, _T(""), NULL);

				// Make the copy invisible
				acdbEntLast(enCopyChain); 
				AcDbObjectId objIdChain; acdbGetObjectId(objIdChain, enCopyChain);

				AcDbHandle dbChainHandle =  objIdChain.handle(); 
				ACHAR szChainHandle[17]; dbChainHandle.getIntoAsciiBuffer(szChainHandle); 

				acDocManager->lockDocument(acDocManager->curDocument());  
				
				AcDbPolyline *pObjChainHndl = NULL;
				if (acdbOpenObject(pObjChainHndl, objIdChain, AcDb::kForWrite) != Acad::eOk) continue;

				pObjChainHndl->setVisibility(AcDb::kInvisible);
				pObjChainHndl->erase(Adesk::kTrue);
				pObjChainHndl->setXData(rbpChainCopy);
				pObjChainHndl->close();

				acDocManager->unlockDocument(acDocManager->curDocument());  

				// This is to restore the polylines back (in case they are not selected for bond)
				objIdArray.append(objIdChain);

				// Explode the polyline to get its segments
				acedCommandS(RTSTR, _T(".EXPLODE"), RTENAME, enLWCable, NULL);
								
				// Get the exploded information
				if (acedSSGet(_T("P"), NULL, NULL, NULL, ssCables) != RTNORM)	{	acutPrintf(_T("\nUnable to retrieve segment information of cables.")); continue; }
				rbpChainHndl = acutBuildList(AcDb::kDxfRegAppName, _T("IE_TB_CHAIN_HNDL"), AcDb::kDxfXdAsciiString,  szChainHandle, NULL);

				ads_name enExSeg;
				Adesk::Int32 lExSegLen = 0L; acedSSLength(ssCables, &lExSegLen);
				for (Adesk::Int32 lExSeg = 0L; lExSeg < lExSegLen; lExSeg++)
				{
					acedSSName(ssCables, lExSeg, enExSeg);
					addXDataToEntity(enExSeg, rbpChainHndl);
				}

				acutRelRb(rbpChainHndl); 
			}

			acedSSFree(ssLWCables);

			// Get the cable segments attached to this pole
			int iCablesAttached = GetCablesAttachedToPole(objPole, csaCablesAttached);
			if (iCablesAttached <= 1) 
			{
				acTransactionManagerPtr()->abortTransaction();
				acutPrintf(_T(" There is only one cable attached to this pole.")); 
				acTransactionManagerPtr()->startTransaction();
			}
			else bOkay = TRUE;
		} 
		// Otherwise, inform the user
		else acutPrintf(_T(" That is not a valid pole."));
	}
	
	// Now ask the user to select cables to be bonded
	long lLength = 0L;
	ads_point ptPick, ptArc;
	ads_name enCable;
	CString csCableLayerName, csPoleLayerName;
	ads_name ssCables;

	acutPrintf(_T("\nSelect cables to be bonded...\n"));
	saveOSMode();

	long aryHandles[5];
		
	int ilongHndlInd = 0;
	struct resbuf *rbpXDExist = NULL;
	while (T)
	{
		// Ask user to select a pole
		int iResult = acedEntSel(_T("\rSelect a cable: "), enCable, ptPick);

		if (iResult == RTNORM)
		{
			// Check if the cable has "IE_TB_CHAIN" attached to it
			if (getXDataFromEntity(enCable, _T("IE_TB_CHAIN_HNDL")) == NULL) 
			{
				acutPrintf(_T("The selected cable is not a polyline entity.\n")); continue; 
				acedCommandS(RTSTR, _T(".CHPROP"), RTENAME, enCable, RTSTR, _T(""), RTSTR, _T("C"), RTSTR, _T("2"), RTSTR, _T(""), NULL);
			}
			
			// Get the handle of the selected entity.
			AcDbObjectId objCable; acdbGetObjectId(objCable, enCable);
			AcDbHandle dbCableHandle =  objCable.handle(); 
			ACHAR szCableHandle[17]; dbCableHandle.getIntoAsciiBuffer(szCableHandle); 
			CString csCableHandle; csCableHandle.Format(_T("%s"), szCableHandle);
			GetEntityLayerName(objCable, csCableLayerName);

			// Get the existing XDATA attached to this cable
			//AcDbEntity *pWire;
			//if (acdbOpenObject(pWire, objCable, AcDb::kForRead) == Acad::eOk)
			//{
				//rbpXDExist = pWire->xData(); 
				//pWire->close(); 
			//}
									
			int iIndex;
			if (CheckForDuplication(csaCablesAttached, csCableHandle, iIndex))
			{	
				struct resbuf *rbpSelect = acutBuildList(AcDb::kDxfRegAppName, _T("IE_TB_SELECT"), AcDb::kDxfXdInteger16, 0, NULL);
				addXDataToEntity(enCable, rbpSelect);
				acutRelRb(rbpSelect);

				ads_name enCopy; acedCommandS(RTSTR, _T(".COPY"), RTENAME, enCable, RTSTR, _T(""), RTSTR, _T("0,0"), RTSTR, _T(""), NULL);	acdbEntLast(enCopy);
				SetEditorColorToEntity(enCopy);
				
				struct resbuf *rbpCopy = acutBuildList(AcDb::kDxfRegAppName, _T("IE_TB_EXTEND"), AcDb::kDxfXdInteger16, 0, NULL);
				addXDataToEntity(enCopy, rbpCopy);
				acutRelRb(rbpCopy);
				
				// Get the hexadecimal value of the cable handle selected and add it to the handle array
				long i;	_stscanf(szCableHandle, _T("%x"), &i); aryHandles[ilongHndlInd] = i;
				ilongHndlInd++;

				// Display how many cables were selected.
				acutPrintf(_T(" %d found.\n"), ilongHndlInd);
				
				// Pick point on the first cable will be the start point of the BOND arc
				if (ilongHndlInd == 1) { acutPolar(ptPick, 0.0, 0.0, ptArc); }
				if (ilongHndlInd == 5) break;
			}
			else { acutPrintf(_T("The selected cable is not connected to this pole.\n")); continue; }
		}
		else if (iResult == RTCAN) { acTransactionManagerPtr()->abortTransaction(); restoreOSMode(); return; }
		else if (iResult == RTERROR) 
		{
			if (ilongHndlInd < 2) { acutPrintf(_T("At least two cables must be selected for bonding.\n")); continue; }
			break; 
		}
	}

	// Sort the handles based on the order in which the entity was inserted
	selectionSort(aryHandles, ilongHndlInd);

	// Entities in BOND must be collected so that they can be moved to a separate layer at the end of the command
	ads_name enDonut1, enDonut2, enDonut3, enDonut4, enDonut5, enArc;

	// If there are only two cables, life is simple
	TCHAR pszHandle[17];
	AcDbEntity *pWire;
	if (ilongHndlInd == 2)
	{
		// Delete the unseen entities
		struct resbuf *rbpF = acutBuildList(67, 0, AcDb::kDxfRegAppName, _T("IE_TB_EXTEND"), NULL);
		if (acedSSGet(_T("X"), NULL, NULL, rbpF, ssCables) != RTNORM) { acedSSFree(ssCables); acutRelRb(rbpF); }
		Adesk::Int32 lCables = 0L; acedSSLength(ssCables, &lCables);

		acedCommandS(RTSTR, _T(".ERASE"), RTPICKS, ssCables, RTSTR, _T(""), NULL);
		acutRelRb(rbpF); 
		acedSSFree(ssCables);

		// Get the object IDs of the selected cables
		ads_name enCable1; _itot( aryHandles[0], pszHandle, 16); acdbHandEnt(pszHandle, enCable1);
		ads_name enCable2; _itot( aryHandles[1], pszHandle, 16); acdbHandEnt(pszHandle, enCable2);

		AcDbObjectId objCable1; acdbGetObjectId(objCable1, enCable1);
		AcDbObjectId objCable2; acdbGetObjectId(objCable2, enCable2);

		// Terminate the first and second cables and store which end was terminated
		ads_point ptStart1, ptEnd1, ptStart2, ptEnd2, ptDonut1, ptDonut2, ptArc1, ptArc2;

		// Get the existing XDATA attached to this cable
		if (acdbOpenObject(pWire, objCable1, AcDb::kForRead) == Acad::eOk)	{ rbpXDExist = pWire->xData(); pWire->close(); }

		int iCable1 = CreateCableChainTerminationInBondNew(objCable1, objPole, ptStart1, ptEnd1, rbpXDExist);

		// Get the existing XDATA attached to this cable
		if (acdbOpenObject(pWire, objCable2, AcDb::kForRead) == Acad::eOk)	{ rbpXDExist = pWire->xData(); pWire->close(); }
		int iCable2 = CreateCableChainTerminationInBondNew(objCable2, objPole, ptStart2, ptEnd2, rbpXDExist);

		// Calculate the point to place the donut on the first cable
		if (iCable1 == 1) 
		{
			// Darren: the add bond command to remove the 2m offset; just make it on the insertion point instead.
			// acutPolar(asDblArray(gePoleIns), acutAngle(ptStart1, ptEnd1), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut1);
			acutPolar(asDblArray(gePoleIns), acutAngle(ptStart1, ptEnd1), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut1);
		}
		else 
		{
			// acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd1, ptStart1), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut1);
			acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd1, ptStart1), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut1);
		}

		// Calculate the point to place the donut on the second cable
		if (iCable2 == 1) 
		{
			// acutPolar(asDblArray(gePoleIns), acutAngle(ptStart2, ptEnd2), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut2);
			acutPolar(asDblArray(gePoleIns), acutAngle(ptStart2, ptEnd2), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut2);
		}
		else 
		{
			// acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd2, ptStart2), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut2);
			acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd2, ptStart2), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut2);
		}

		//////////////////////////////////////////////////////////////////////////
		// Draw the two donuts
		//////////////////////////////////////////////////////////////////////////

		// Issues Register : 172 (DONUT must be replaced by BLOCK)
		// acedCommandS(RTSTR, _T(".DONUT"), RTREAL, 0.0, RTREAL, 0.15 * gePoleScale.sx, RTPOINT, ptDonut1, RTSTR, "", NULL);	acdbEntLast(enDonut1);
		// acedCommandS(RTSTR, _T(".DONUT"), RTREAL, 0.0, RTREAL, 0.15 * gePoleScale.sx, RTPOINT, ptDonut2, RTSTR, "", NULL);	acdbEntLast(enDonut2);

		AcDbObjectId objDonut;
		insertBlock(_T("BOND POINT"), _T("0"), ptDonut1, 0.15 * gePoleScale.sx, 0.0, 0.0, _T(""), objDonut, TRUE); acdbGetAdsName(enDonut1, objDonut);
		insertBlock(_T("BOND POINT"), _T("0"), ptDonut2, 0.15 * gePoleScale.sx, 0.0, 0.0, _T(""), objDonut, TRUE); acdbGetAdsName(enDonut2, objDonut);

		// Loop until user accepts
		bOkay = FALSE;
		BOOL bRev = FALSE;
		TCHAR szResult[21];

		while (bOkay == FALSE)
		{
			// Delete the arc
			acdbEntDel(enArc);

			// Draw the default arc or the reverse arc
			acedCommandS(RTSTR, _T(".ARC"), RTSTR, _T("C"), RTPOINT, asDblArray(gePoleIns), RTPOINT, (bRev ? ptDonut1 : ptDonut2), RTPOINT, (bRev ? ptDonut2 : ptDonut1), NULL);
			acdbEntLast(enArc);

			// Ask the user whether it is to be reversed
			acedInitGet(0, _T("Accept Reverse"));
			iRet = acedGetKword(_T("\nBond status [Accept/Reverse] <Accept>: "), szResult);
			if (iRet == RTNONE) _tcscpy(szResult, _T("Accept"));

			// If the user canceled at this stage, abort the transaction. This will restore the cables to their 
			// pre-bonding status.
			if (iRet == RTCAN) { acTransactionManagerPtr()->abortTransaction(); restoreOSMode(); return; }

			if (szResult[0] == _T('A')) bOkay = TRUE;
			// If it is to be reversed, set the flag
			else if (szResult[0] == _T('R')) bRev = !bRev;
			// Otherwise, set the flag to terminate
			else bOkay = TRUE;
		}
	}
	else 
	{
		// Get the object IDs of the selected cables
		ads_name enCable1; _itot( aryHandles[0], pszHandle, 16); acdbHandEnt(pszHandle, enCable1);
		ads_name enCable2; _itot( aryHandles[1], pszHandle, 16); acdbHandEnt(pszHandle, enCable2);
		ads_name enCable3; _itot( aryHandles[2], pszHandle, 16); acdbHandEnt(pszHandle, enCable3);
		ads_name enCable4; if (ilongHndlInd > 3)  { _itot( aryHandles[3], pszHandle, 16); acdbHandEnt(pszHandle, enCable4); }
		ads_name enCable5; if (ilongHndlInd > 4)  { _itot( aryHandles[4], pszHandle, 16); acdbHandEnt(pszHandle, enCable5); }

		AcDbObjectId objCable1; acdbGetObjectId(objCable1, enCable1);
		AcDbObjectId objCable2; acdbGetObjectId(objCable2, enCable2);
		AcDbObjectId objCable3; acdbGetObjectId(objCable3, enCable3);
		AcDbObjectId objCable4;	if (ilongHndlInd > 3) acdbGetObjectId(objCable4, enCable4);
		AcDbObjectId objCable5;	if (ilongHndlInd > 4) acdbGetObjectId(objCable5, enCable5);

		// Terminate the first and second cables and store which end was terminated
		ads_point ptStart1, ptStart2, ptStart3, ptStart4, ptStart5;
		ads_point ptEnd1, ptEnd2, ptEnd3, ptEnd4, ptEnd5;
		ads_point ptDonut1, ptDonut2, ptDonut3, ptDonut4, ptDonut5;

		// Get the existing XDATA attached to this cable 1
		if (acdbOpenObject(pWire, objCable1, AcDb::kForRead) == Acad::eOk)	{ rbpXDExist = pWire->xData(); pWire->close(); }
		int iCable1 = CreateCableChainTerminationInBondNew(objCable1, objPole, ptStart1, ptEnd1, rbpXDExist);

		// Get the existing XDATA attached to this cable 2
		if (acdbOpenObject(pWire, objCable2, AcDb::kForRead) == Acad::eOk)	{ rbpXDExist = pWire->xData(); pWire->close(); }
		int iCable2 = CreateCableChainTerminationInBondNew(objCable2, objPole, ptStart2, ptEnd2, rbpXDExist);

		// Get the existing XDATA attached to this cable 3
		if (acdbOpenObject(pWire, objCable3, AcDb::kForRead) == Acad::eOk)	{ rbpXDExist = pWire->xData(); pWire->close(); }
		int iCable3 = CreateCableChainTerminationInBondNew(objCable3, objPole, ptStart3, ptEnd3, rbpXDExist);
		
		int iCable4, iCable5; 
		if (ilongHndlInd > 3)
		{
			// Get the existing XDATA attached to this cable 4
			if (acdbOpenObject(pWire, objCable4, AcDb::kForRead) == Acad::eOk)	{ rbpXDExist = pWire->xData(); pWire->close(); }

			iCable4 = CreateCableChainTerminationInBondNew(objCable4, objPole, ptStart4, ptEnd4, rbpXDExist); 
		}
		if (ilongHndlInd > 4) 
		{
			// Get the existing XDATA attached to this cable
			if (acdbOpenObject(pWire, objCable5, AcDb::kForRead) == Acad::eOk)	{ rbpXDExist = pWire->xData(); pWire->close(); }

			iCable5 = CreateCableChainTerminationInBondNew(objCable5, objPole, ptStart5, ptEnd5, rbpXDExist); 
		}

		//////////////////////////////////////////////////////////////////////////
		// Calculate the point to place the donut on the first cable
		//////////////////////////////////////////////////////////////////////////
		if (iCable1 == 1) 
		{
			// acutPolar(asDblArray(gePoleIns), acutAngle(ptStart1, ptEnd1), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut1);
			acutPolar(asDblArray(gePoleIns), acutAngle(ptStart1, ptEnd1), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut1);
		}
		else 
		{
			// acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd1, ptStart1), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut1);
			acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd1, ptStart1), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut1);
		}

		//////////////////////////////////////////////////////////////////////////
		// Calculate the point to place the donut on the second cable
		//////////////////////////////////////////////////////////////////////////
		if (iCable2 == 1) 
		{
			// acutPolar(asDblArray(gePoleIns), acutAngle(ptStart2, ptEnd2), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut2);
			acutPolar(asDblArray(gePoleIns), acutAngle(ptStart2, ptEnd2), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut2);
		}
		else 
		{
			// acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd2, ptStart2), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut2);
			acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd2, ptStart2), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut2);
		}

		//////////////////////////////////////////////////////////////////////////
		// Calculate the point to place the donut on the third cable
		//////////////////////////////////////////////////////////////////////////
		if (iCable3 == 1) 
		{
			// acutPolar(asDblArray(gePoleIns), acutAngle(ptStart3, ptEnd3), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut3);
			acutPolar(asDblArray(gePoleIns), acutAngle(ptStart3, ptEnd3), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut3);
		}
		else 
		{
			// acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd3, ptStart3), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut3);
			acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd3, ptStart3), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut3);
		}

		//////////////////////////////////////////////////////////////////////////
		// Calculate the point to place the donut on the fourth cable
		//////////////////////////////////////////////////////////////////////////
		if (ilongHndlInd > 3)
		{
			if (iCable4 == 1)
			{
				// acutPolar(asDblArray(gePoleIns), acutAngle(ptStart4, ptEnd4), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut4);
				acutPolar(asDblArray(gePoleIns), acutAngle(ptStart4, ptEnd4), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut4);
			}
			else
			{
				// acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd4, ptStart4), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut4);
				acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd4, ptStart4), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut4);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Calculate the point to place the donut on the fifth cable
		//////////////////////////////////////////////////////////////////////////
		if (ilongHndlInd > 4)
		{
			if (iCable5 == 1)
			{
				// acutPolar(asDblArray(gePoleIns), acutAngle(ptStart5, ptEnd5), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut5);
				acutPolar(asDblArray(gePoleIns), acutAngle(ptStart5, ptEnd5), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut5);
			}
			else 
			{
				// acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd5, ptStart5), 2.0 /*acutDistance(asDblArray(gePoleIns), ptArc)*/, ptDonut5);
				acutPolar(asDblArray(gePoleIns), acutAngle(ptEnd5, ptStart5), acutDistance(asDblArray(gePoleIns), ptArc), ptDonut5);
			}
		}

		// Draw the three+ donuts
		// acedCommandS(RTSTR, _T(".DONUT"), RTREAL, 0.0, RTREAL, 0.15 * gePoleScale.sx, RTPOINT, ptDonut1, RTSTR, "", NULL);	acdbEntLast(enDonut1);
		// acedCommandS(RTSTR, _T(".DONUT"), RTREAL, 0.0, RTREAL, 0.15 * gePoleScale.sx, RTPOINT, ptDonut2, RTSTR, "", NULL);	acdbEntLast(enDonut2);
		// acedCommandS(RTSTR, _T(".DONUT"), RTREAL, 0.0, RTREAL, 0.15 * gePoleScale.sx, RTPOINT, ptDonut3, RTSTR, "", NULL);	acdbEntLast(enDonut3);
		// if (ilongHndlInd > 3) {	acedCommandS(RTSTR, _T(".DONUT"), RTREAL, 0.0, RTREAL, 0.15 * gePoleScale.sx, RTPOINT, ptDonut4, RTSTR, "", NULL);	acdbEntLast(enDonut4); }
		// if (ilongHndlInd > 4) {	acedCommandS(RTSTR, _T(".DONUT"), RTREAL, 0.0, RTREAL, 0.15 * gePoleScale.sx, RTPOINT, ptDonut5, RTSTR, "", NULL);	acdbEntLast(enDonut5); }

		AcDbObjectId objDonut;
		insertBlock(_T("BOND POINT"), _T("0"), ptDonut1, 0.15 * gePoleScale.sx, 0.0, 0.0, _T(""), objDonut, TRUE); acdbGetAdsName(enDonut1, objDonut);
		insertBlock(_T("BOND POINT"), _T("0"), ptDonut2, 0.15 * gePoleScale.sx, 0.0, 0.0, _T(""), objDonut, TRUE); acdbGetAdsName(enDonut2, objDonut);
		insertBlock(_T("BOND POINT"), _T("0"), ptDonut3, 0.15 * gePoleScale.sx, 0.0, 0.0, _T(""), objDonut, TRUE); acdbGetAdsName(enDonut3, objDonut);

		if (ilongHndlInd > 3) insertBlock(_T("BONDPOINT"), _T("0"), ptDonut4, 0.15 * gePoleScale.sx, 0.0, 0.0, _T(""), objDonut, TRUE); acdbGetAdsName(enDonut4, objDonut);
		if (ilongHndlInd > 4) insertBlock(_T("BONDPOINT"), _T("0"), ptDonut5, 0.15 * gePoleScale.sx, 0.0, 0.0, _T(""), objDonut, TRUE); acdbGetAdsName(enDonut5, objDonut);
				
		// Loop until user accepts
		bOkay = FALSE;
		BOOL bRev = FALSE;
		TCHAR szResult[21];
		ads_point ptArc[10]; acutPolar(ptDonut1, 0.0, 0.0, ptArc[0]);

		// Determine the start point to draw the arc
		ads_point ptStart; acutPolar(ptDonut1,	0.0, 0.0, ptStart); 
		double dAng			 = acutAngle(asDblArray(gePoleIns), ptStart) + DTR(5);
		ads_point ptEnd;   acutPolar(asDblArray(gePoleIns), dAng, gePoleScale.sx, ptEnd);

		struct resbuf *rbpF = acutBuildList(67, 0, AcDb::kDxfRegAppName, _T("IE_TB_EXTEND"), NULL);
		if (acedSSGet(_T("X"), NULL, NULL, rbpF, ssCables) != RTNORM) { acedSSFree(ssCables); acutRelRb(rbpF); }
		Adesk::Int32 lCables = 0L; acedSSLength(ssCables, &lCables);

		acedCommandS(RTSTR, _T(".ARC"), RTSTR, _T("C"), RTPOINT, asDblArray(gePoleIns), RTPOINT, ptStart, RTPOINT, ptEnd, NULL);
		acdbEntLast(enArc);
		
		int iCtr;
		for (iCtr = 0; iCtr < (int) lCables - 1; iCtr++)
		{
			acutPolar(ptStart, 0.0, 0.0, ptArc[iCtr]);
			acedCommandS(RTSTR, _T(".EXTEND"), RTPICKS, ssCables, RTSTR, _T(""), RTLB, RTENAME, enArc, RTPOINT, ptStart, RTLE, RTSTR, _T(""), NULL);
			GetEndPoint(enArc, ptStart, FALSE);
		}

		// Delete the unseen entities
		acedCommandS(RTSTR, _T(".ERASE"), RTPICKS, ssCables, RTSTR, _T(""), NULL);
		
		acutRelRb(rbpF); 
		acedSSFree(ssCables);
						
		acutPolar(ptStart, 0.0, 0.0, ptArc[iCtr]);
		int iSize = iCtr + 1;
		ads_point ptMid;
		int iArcInd = 0;
		int iEndInd = iSize - 1;
		int iMidInd = 1;

		while (bOkay == FALSE)
		{
			acdbEntDel(enArc);

			acutPolar(ptArc[iArcInd],	0.0,  0.0, ptStart);
			acutPolar(ptArc[iMidInd],	0.0,  0.0, ptMid);
			acutPolar(ptArc[iEndInd], 0.0,  0.0, ptEnd);

			acedCommandS(RTSTR, _T(".ARC"), RTPOINT, ptStart, RTPOINT, ptMid, RTPOINT, ptEnd, NULL);
			acdbEntLast(enArc);

			iArcInd++; if (iArcInd == iSize)  iArcInd = 0;
			iMidInd++; if (iMidInd == iSize)  iMidInd = 0;
			iEndInd++; if (iEndInd == iSize)  iEndInd = 0;

			// Ask the user whether it is to be reversed
			acedInitGet(0, _T("Accept Reverse"));
			iRet = acedGetKword(_T("\nBond status [Accept/Reverse] <Reverse>: "), szResult);
			if (iRet == RTNONE) _tcscpy(szResult, _T("Reverse"));

			// If the user canceled at this stage, abort the transaction. This will restore the cables to their pre-bonding status
			if (iRet == RTCAN) { acTransactionManagerPtr()->abortTransaction(); return; }

			// If it is accepted, we need to get the name of the block
			if (szResult[0] == _T('A')) bOkay = true;

			// If it is to be reversed, set the flag
			else if (szResult[0] == _T('R')) { bRev = !bRev; }

			// Otherwise, set the flag to terminate
			else bOkay = TRUE;
		}
	}

	// Erase all the exploded polyline segments
	struct resbuf *rbpChainSegFilt = acutBuildList(67, 0, AcDb::kDxfRegAppName, _T("IE_TB_CHAIN_HNDL"), NULL);
	ads_name ssChainSegs;
	if (acedSSGet(_T("X"), NULL, NULL, rbpChainSegFilt, ssChainSegs) == RTNORM)
	{
		acedCommandS(RTSTR, _T(".ERASE"), RTPICKS, ssChainSegs, RTSTR, _T(""), NULL);
		acedSSFree(ssChainSegs);
	}
	acutRelRb(rbpChainSegFilt);
		
	//////////////////////////////////////////////////////////////////////////
	// Modified as per Modification in Point (7) in Issued_Register_06.11.2007
	//////////////////////////////////////////////////////////////////////////
	TCHAR szConnectorType[15];
	acedInitGet(RSG_NONULL, _T("Hardwired UGOH"));
	int iRetConnectorType = acedGetKword(_T("\nConnector type [Hardwired/UGOH]: "), szConnectorType);
  
	if (acdbEntGet(enDonut1)) ShiftBondLayer(enDonut1, csCableLayerName, szConnectorType);
	if (acdbEntGet(enDonut2)) ShiftBondLayer(enDonut2, csCableLayerName, szConnectorType);
	if (acdbEntGet(enDonut3)) ShiftBondLayer(enDonut3, csCableLayerName, szConnectorType);
	if (acdbEntGet(enDonut4)) ShiftBondLayer(enDonut4, csCableLayerName, szConnectorType);
	if (acdbEntGet(enDonut5)) ShiftBondLayer(enDonut5, csCableLayerName, szConnectorType);
	if (acdbEntGet(enArc))    ShiftBondLayer(enArc,    csCableLayerName, szConnectorType);

	if (iRetConnectorType == RTCAN) { acTransactionManagerPtr()->abortTransaction(); restoreOSMode(); return; }

	// End the transaction, so that the changes will be committed 
	restoreOSMode();

	//////////////////////////////////////////////////////////////////////////
	// Bring back all the polylines that were not selected for bond
	//////////////////////////////////////////////////////////////////////////
	AcDbPolyline *pChain = NULL;
	CString csLType; 
	CString csLayer; 

	for (int iCtr = 0; iCtr < objIdArray.length(); iCtr++)
	{
		// Get the chain reference pointer
		Acad::ErrorStatus es = acdbOpenObject(pChain, objIdArray[iCtr], AcDb::kForRead, true);
		if (es != Acad::eOk) { acutPrintf(_T("Error at %d: %s"), __LINE__, acadErrorStatusText(es)); acTransactionManagerPtr()->abortTransaction();  return; }

		// If this chain is already processed, then it need not be given life again
		if (pChain->xData(_T("IE_TB_CHAIN_REMOVED")) != NULL) continue;

		// Get the existing XDATA attached to this cable
		rbpXDExist = pWire->xData();

		// Get the chains layer and line type
		csLType.Format(_T("%s"), pChain->linetype()); 
		csLayer.Format(_T("%s"), pChain->layer());

		// Create a new polyline entity by appending the vertex one after the other until it matches with the end points of this segment
		AcDbPolyline *pNewChain = new AcDbPolyline(); pNewChain->setLayer(csLayer); pNewChain->setLinetype(csLType);

		AcGePoint2d geVertex;
		for (int iCtr = 0; iCtr < pChain->numVerts(); iCtr++)
		{
			// Get the vertex at the current index. For index = 0, get the start point and continue because we need two vertice's to make a segment.
			pChain->getPointAt(iCtr, geVertex);
			pNewChain->addVertexAt(iCtr, geVertex);
		}

		// Add the new polyline to database
		appendEntityToDatabase(pNewChain);

		AcDbObjectId objNewID = pNewChain->objectId();
		pNewChain->close();
		pChain->close();

		// Add the existing XDATA to the new chain
		if (objNewID.isValid() && rbpXDExist)
		{
			AcDbEntity *pNewEnt;
			if (acdbOpenObject(pNewEnt, objNewID, AcDb::kForWrite) == Acad::eOk)
			{
				pNewEnt->setXData(rbpXDExist);
				pNewEnt->close();

				acutRelRb(rbpXDExist);
			}
		}
	}

	acTransactionManagerPtr()->endTransaction();

	// Set the draw order of all pole and end shackle blocks to front
	SetDrawOrder();
} // layer

//////////////////////////////////////////////////////////////////////////
// *********** Functions related to LEGEND command *******************
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Function name: CheckForTagInInsert
// Description  : Checks if the given tag name exists in the given INSERT
// Arguments    : 1. [IN] Object ID of the INSERT
//                2. [IN] Tag name to search
// Returns      : Success/Fail
//////////////////////////////////////////////////////////////////////////
bool CheckForTagInInsert(AcDbObjectId objInsertId, CString csTag)
{
	// Get the attribute iterator for the block reference
	AcDbBlockReference *pInsert;	
	Acad::ErrorStatus es = acdbOpenObject(pInsert, objInsertId, AcDb::kForRead);
	if (es != Acad::eOk) { acutPrintf(_T("\nERROR @%d: %s"), __LINE__, acadErrorStatusText(es)); return false; }
	AcDbObjectIterator *pIter = pInsert->attributeIterator();
	pInsert->close();

	// Check if the given tag is present in it
	AcDbAttribute *pAtt;
	AcDbObjectId objAttId;  
	for (pIter->start(); !pIter->done(); pIter->step())
	{
		objAttId = pIter->objectId();
		es = acdbOpenObject(pAtt, objAttId, AcDb::kForRead);
		if (es != Acad::eOk) { acutPrintf(_T("\nERROR @%d: %s"), __LINE__, acadErrorStatusText(es)); delete pIter; return false; }

		// This attribute tag matches with the given attribute tag
		if (!csTag.CompareNoCase(pAtt->tag())) { pAtt->close(); delete pIter; return true; }
		pAtt->close();
	}

	delete pIter;
	return false;
}

//////////////////////////////////////////////////////////////////////////
// Function name : GetYShift
// Description   : Returns half the maximum Y dimension of a block
// Arguments     : [IN] Block name of the INSERT
// Returns       : Factor of max. Y dimension of the block
//////////////////////////////////////////////////////////////////////////
double GetYShift(CString csName)
{
	ads_point ptLegend = { 10000.00, 10000.00, 0.0 };
	
	// Insert the block
	AcDbObjectId objLegend;
	if (insertBlock((LPCTSTR)csName, _T("NOTES"), ptLegend, 2.5, 2.5, 0.0, g_csPoleTextStyle, objLegend, FALSE) == FALSE) return 0.0;;
	
	// Get the geometric extents of the insert & erase the temporary insert
	AcDbBlockReference *pInsert;
	if (acdbOpenObject(pInsert, objLegend, AcDb::kForWrite) != Acad::eOk) { return 0.0; }
	AcDbExtents extents; pInsert->getGeomExtents(extents);
	pInsert->erase();
	pInsert->close();

	return (fabs(extents.maxPoint().y - extents.minPoint().y) / 2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : GetSSExtents
// Arguments        : 1. Selection set of entities whose extents have to be determined, as ads_name
//					          2. Resultant lower left extent, as ads_point
//                    3. Resultant upper right extent, as ads_point
//                    4. Resultant center point, as ads_point
//
// Called from      : Anywhere
// Returns          : TRUE if successful, FALSE if not
// Description      : Calculates the co-ordinate extents of the selection set
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL GetSSExtents(ads_name ssSet, ads_point ptLL, ads_point ptUR, ads_point ptCenter) // *** NOT USED ****
{
	// Reset the positions of the extent points
	ptLL[X] = ptLL[Y] = +999999.00;
	ptUR[X] = ptUR[Y] = -999999.00;
	ptLL[Z] = ptUR[Z] = 0.0;

	// Get the length of the selection set
	Adesk::Int32 lLength; if (acedSSLength(ssSet, &lLength) != RTNORM || (lLength < 1L)) return FALSE;

	// Calculate the geometric extents of each entity in the selection set
	ads_name enSet;
	AcDbObjectId objId;
	AcDbEntity *pEnt = NULL;
	AcDbExtents exLimits;
	AcGePoint3d geMax, geMin;
	for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
	{
		// Get the next entity name in the selection set and get its object id
		acedSSName(ssSet, lCtr, enSet);
		if (acdbGetObjectId(objId, enSet) != Acad::eOk) return FALSE;

		// Open it for reading and get is geometric extents
		if (acdbOpenObject(pEnt, objId, AcDb::kForWrite) != Acad::eOk) return FALSE;
		if (pEnt->getGeomExtents(exLimits) != Acad::eOk) { pEnt->close(); continue; }
		pEnt->close();

		// Check if either the lower left limit or the upper right limit lie outside the current values
		geMin = exLimits.minPoint(); geMax = exLimits.maxPoint();
		if (geMax[X] > ptUR[X]) ptUR[X] = geMax[X]; if (geMax[Y] > ptUR[Y]) ptUR[Y] = geMax[Y];
		if (geMin[X] < ptLL[X]) ptLL[X] = geMin[X]; if (geMin[Y] < ptLL[Y]) ptLL[Y] = geMin[Y];
	}

	// Calculate the center point of the extents box
	ptCenter[X] = (ptLL[X] + ptUR[X]) / 2.0;
	ptCenter[Y] = (ptLL[Y] + ptUR[Y]) / 2.0;
	ptCenter[Z] = 0.0;

	// Everything is fine
	return TRUE;
}

/////////////////////////////////////////////////////
// Function name: 
// Description  :
/////////////////////////////////////////////////////
bool GetExtents(CString csBlkName, AcDbExtents extents) // *** NOT USED ****
{
	// Insert the block
	ads_point ptOrigin = { -10000.0, -10000.0, 0.0 };
	AcDbObjectId objInsert;
	if (insertBlock((LPCTSTR)csBlkName, _T("NOTES"), ptOrigin, 2.5, 2.5, 0.0, g_csPoleTextStyle, objInsert, FALSE) == FALSE) return false;

	// Explode the insert
	ads_name enInsert;
	acdbGetAdsName(enInsert, objInsert);
	acedCommandS(RTSTR, L".EXPLODE", RTENAME, enInsert, NULL);

	ads_name ssExplode; 
	if (acedSSGet(L"P", NULL, NULL, NULL, ssExplode) != RTNORM) return false;

	ads_point ptLL, ptUR, ptCenter;
	if (!GetSSExtents(ssExplode, ptLL, ptUR, ptCenter)) 
	{
		acedCommandS(RTSTR, L".ERASE", RTPICKS, ssExplode, RTSTR, L"", NULL);
		return false;
	}

	extents.set(AcGePoint3d(ptLL[X], ptLL[Y], 0.0), AcGePoint3d(ptUR[X], ptUR[Y], 0.0));
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Function name: PlaceThisLegendInTable()
// Description  : Writes the TEXTS for a given ROW in the LEGEND table.
//////////////////////////////////////////////////////////////////////////
bool PlaceThisLegendInTable(AcDbTable *pTable, int iRow, CLegendInfo legendInfo, AcDbObjectId objIDTxtStyle)
{
	// If the information placed is for a LTYPE, get the line type assigned to the layer on which the line is.
	Adesk::Int16 iColor = 254;
	CString csName; // For a LINE, the LTYPE and for an INSERT its BLOCK NAME are used for a LEGEND
	csName = legendInfo.m_csName;
		
	// If the name is not EMPTY
	if (csName.IsEmpty()) return false;
	
	// Open the symbol table record for this name and get the object of this blocks definition
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);

	AcDbObjectId objId; 
	Acad::ErrorStatus es = pBlkTbl->getAt(csName, objId);  
	pBlkTbl->close();  
	if (es != Acad::eOk) { return false; }

	// Set the block table record id to the given row and column
	pTable->setBlockTableRecordId(iRow, 0, (objId.isValid() ? objId : NULL), false);

	pTable->setCellType(iRow, 0, AcDb::kBlockCell);
	pTable->setAlignment(AcDb::kMiddleCenter);

	pTable->setCellType(iRow, 1, AcDb::kTextCell);
	pTable->setTextString(iRow, 1, legendInfo.m_csDescription);
	pTable->setAlignment(iRow, 1, AcDb::kMiddleLeft);
					
	// Set the cells top margin
	pTable->setMargin(iRow + 2, -1, AcDb::kCellMarginTop, 0.5);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Function name: setTextSizeInStyle()
// Description  : Sets the values for style parameters for a TEXT.
//////////////////////////////////////////////////////////////////////////
bool setTextSizeInStyle(CString csStyle, double dNewTxtSize, double &dOldTxtSize, AcDbObjectId &objIDTxtStyle)
{
	AcDbTextStyleTable *pTxtStyle = NULL;
	if (acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTxtStyle, AcDb::kForRead) != Acad::eOk) return false;

	// Get the object id of the given text style. Set its text height as 0.0, in case they are fixed at a non-zero value.
	AcDbTextStyleTableRecord *pTxtStyleRcd = new AcDbTextStyleTableRecord();
	if (!pTxtStyle->has(csStyle))
	{
		// Create the style as specified in the tables
		double dHeight;
		pTxtStyle->close();
		// CreateEATextStyle(dHeight);
	}

	if (pTxtStyle->getAt(csStyle, pTxtStyleRcd, AcDb::kForWrite) != Acad::eOk) 
	{
		pTxtStyle->close(); 
		return false; 
	}

	dOldTxtSize = pTxtStyleRcd->textSize();
	objIDTxtStyle = pTxtStyleRcd->objectId();
	pTxtStyleRcd->setTextSize(0.0);
	pTxtStyleRcd->close();
	pTxtStyle->close();

	return true;
}

//////////////////////////////////////////////////////////////////
// Function name: SetTableStyle
// Description  : Sets the TABLE style for the LEGEND output.
//////////////////////////////////////////////////////////////////
void SetTableStyle(AcDbTable *pTableLegend, int iRows)
{
	pTableLegend->setLayer(_T("Notes"));
	pTableLegend->setSize(iRows + 0, 2); // The methods setNumColumns() and setNumRows() have been deprecated beyond AutoCAD 2007.
	pTableLegend->setColumnWidth(20);
	pTableLegend->setRowHeight(4.0);
	pTableLegend->setLinetype(L"CONTINUOUS");
	
	// Before setting the "Standard" text style for the table, set the text size in it to 0.0. Just in case someone sets it to nonzero value
	double dTxtSizeData;
	AcDbObjectId objIDTxtStyleData;	if (setTextSizeInStyle(L"Standard", 0.0, dTxtSizeData, objIDTxtStyleData)) { pTableLegend->setTextStyle(objIDTxtStyleData); }
	// AcDbObjectId objIDTxtStyleHead; if (setTextSizeInStyle(L"Standard", 0.0, dTxtSizeData, objIDTxtStyleHead)) { pTableLegend->setTextStyle(objIDTxtStyleHead); }
	AcDbObjectId objIDTxtStyleTitl; if (setTextSizeInStyle(L"Standard", 0.0, dTxtSizeData, objIDTxtStyleTitl)) { pTableLegend->setTextStyle(objIDTxtStyleTitl); }

	//////////////////////////////////////////////////////////////////////////
	// Modify the "Standard" table style
	//////////////////////////////////////////////////////////////////////////

	AcDbDictionary *pDict = NULL;
	AcDbObjectId styleId;      
	acdbHostApplicationServices()->workingDatabase()->getTableStyleDictionary(pDict, AcDb::kForRead);  
	Acad::ErrorStatus es;
	if (pDict->getAt(_T("LEGEND"), styleId) != Acad::eOk) 
	{
		appMessage(L"Unable to find \"LEGEND\" table style. Using \"STANDARD\" instead.", MB_ICONINFORMATION);
		pDict->getAt(_T("STANDARD"), styleId);
	}
	pDict->close();

	// Modify the table details for our requirements
	AcDbTableStyle *pTableStyleTbl;
	if (acdbOpenObject(pTableStyleTbl, styleId, AcDb::kForWrite) == Acad::eOk)
	{
		// pTableStyleTbl->setTextStyle(objIDTxtStyleData, AcDb::kDataRow);
		// pTableStyleTbl->setTextStyle(objIDTxtStyleData, AcDb::kHeaderRow);
		// pTableStyleTbl->setTextStyle(objIDTxtStyleData, AcDb::kTitleRow);

		pTableStyleTbl->setTextHeight(2.0, AcDb::kTitleRow);
		pTableStyleTbl->setTextHeight(2.0, AcDb::kHeaderRow);
		pTableStyleTbl->setTextHeight(2.0, AcDb::kDataRow);
		
		// pTableStyleTbl->suppressHeaderRow(true);

		pTableStyleTbl->setFlowDirection(AcDb::kTtoB);
		pTableStyleTbl->setAlignment(AcDb::kMiddleCenter);
		
		// Setting INVISIBLE to the grid to hide it
		//pTableStyleTbl->setGridVisibility(AcDb::kInvisible);
		pTableStyleTbl->close();
	}

	pTableLegend->setTableStyle(styleId);
	pTableLegend->setAlignment(AcDb::kMiddleCenter, AcDb::kAllRows);
	pTableLegend->generateLayout(); // Very very important, else expect crashes later on

	// Headers
	pTableLegend->setTextString(0, 0, _T("LEGEND"));
	pTableLegend->mergeCells(0, 0, 0, 2);
	pTableLegend->setColumnWidth(1, 50.0);
}

/////////////////////////////////////////////////////
// Function name: 
// Description  :
/////////////////////////////////////////////////////
bool PlaceLegend(std::vector <CLegendInfo> &legendInfo_Vector)
{
	AcDbTable *pTableLegend = new AcDbTable();  
	
	SetTableStyle(pTableLegend, legendInfo_Vector.size());

	// Add the table to the drawing
	AcDbBlockTable *pBlkTbl = NULL;
	if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead) != Acad::eOk) { pTableLegend->close(); return false; }

	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	if (pBlkTbl->getAt( ACDB_PAPER_SPACE, pBlkTblRcd, AcDb::kForWrite) != Acad::eOk) { pBlkTbl->close(); pTableLegend->close(); return false; }
	pBlkTbl->close();

	double dTableWidth  = pTableLegend->width();
	double dTableHeight = pTableLegend->height();
	pBlkTblRcd->appendAcDbEntity(pTableLegend);

	AcDbObjectId objLegend = pTableLegend->objectId();
	pBlkTblRcd->close();

	// Get on with the placement of items
	int iRow = 1;
	int iCtr = 0;
	for (; iCtr < legendInfo_Vector.size(); iCtr++)
	{
		if (!PlaceThisLegendInTable(pTableLegend, iRow, legendInfo_Vector[iCtr], NULL)) continue;
		iRow++;
	}

	// Check if the table has appropriate records in it. If NO, do not place it and delete it
	if (pTableLegend->numRows() <= 1)	{ pTableLegend->erase(); pTableLegend->close();	return true; }

	// Get the geometric extents of the table already placed
	int iRet;
	ads_point ptLegend;
	acedInitGet(RSG_NONULL, _T(""));
	while (T)
	{
		iRet = acedGetPoint(NULL, _T("\nSpecify insertion point: "), ptLegend);
		if (iRet == RTCAN) { pTableLegend->erase(); pTableLegend->close(); return false; }
		else if (iRet == RTNORM) break;
	}

	pTableLegend->setPosition(AcGePoint3d(ptLegend[X], ptLegend[Y], 0.0));
	pTableLegend->setAlignment(AcDb::kMiddleCenter, AcDb::kAllRows);
	
	// Add the LEGEND type XDATA to identify whether the LEGEND is for INSERT or LINES
	pTableLegend->close(); 
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_Legend
// Description  : Called when the user enters the "AEC_Legend" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_Legend()
{
	switchOff();

	// Ensure that the layer for this block is created and current
	createLayer(_T("NOTES"), Adesk::kFalse);

	// If in Paper space, switch to Model Space
	struct resbuf rbTileMode; rbTileMode.restype = RTSHORT; rbTileMode.resval.rint = 0; acedSetVar(_T("TILEMODE"), &rbTileMode);

	// Set the current VPORT to 1
	struct resbuf rbCVPort; acedGetVar(_T("CVPORT"), &rbCVPort); if (rbCVPort.resval.rint != 1) { acedCommandS(RTSTR, _T("PSPACE"), NULL); }

	ads_name enLegend, ssLegend;
	AcDbObjectId objInsert;
	AcDbObjectId objTblRcd; 
	AcDbBlockReference *pInsert = NULL;
	AcDbSymbolTableRecord *pBlkTblRcd = NULL;

	// Open the model space block table record
	CStringArray csaBlockNames;
	ads_point ptLegend;

	std::vector <CLegendInfo> LegendInfo_Vector;

	// Get the current layer name. This is required to set the layer back after the command exits.
	struct resbuf rbCLayer; acedGetVar(_T("CLAYER"), &rbCLayer);
	struct resbuf *rbpFilt = acutBuildList(RTDXF0, _T("INSERT"), 67, 0, NULL);
	if (acedSSGet(_T("X"), NULL, NULL, rbpFilt, ssLegend) == RTNORM)
	{
		// Get the location to place the legend
		//acedInitGet(RSG_NONULL, _T(""));
		//if (acedGetPoint(NULL, _T("\nPick lower left corner to insert legend: "), ptLegend) != RTNORM) { acutRelRb(rbpFilt); acedSSFree(ssLegend); return; }
		acutRelRb(rbpFilt);

		AcDbExtents exInsert; 
		AcGePoint3d geMin;
		AcGePoint3d geMax; 
		AcDbObjectId objLegend;
		AcDbObjectIterator *pIter = NULL;
		AcDbObjectId objAtt;
		AcDbAttribute *pAtt;
		AcString acsName; 
		CString csName;
		Acad::ErrorStatus es;

		CString csTag;
		Adesk::Int32 lLength = 0L; acedSSLength(ssLegend, &lLength);
		for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
		{
			// Get the object Id
			acedSSName(ssLegend, lCtr, enLegend);
			acdbGetObjectId(objInsert, enLegend);

			// Get the block reference pointer
			es = acdbOpenObject(pInsert, objInsert, AcDb::kForRead);
			if (es != Acad::eOk) { acutPrintf(_T("Error at %d: %s"), __LINE__, acadErrorStatusText(es)); acedSSFree(ssLegend); return; }
			objTblRcd = pInsert->blockTableRecord();
			pInsert->close();

			// Get the block name
			if (acdbOpenObject(pBlkTblRcd, objTblRcd, AcDb::kForRead) != Acad::eOk) { acedSSFree(ssLegend); return; }
			pBlkTblRcd->getName(acsName);
			pBlkTblRcd->close();

			// Insert the block in legend only if this is not already inserted
			csName.Format(_T("%s"), acsName.kTCharPtr());
			if (CheckForDuplication(csaBlockNames, csName)) continue;

			// Add this new block name to the distinct block name array 
			csaBlockNames.Add(csName);

			// Check if the block has "Legend" block name
			if (!CheckForTagInInsert(objInsert, _T("LEGEND"))) continue;

			// Valid block found so put it in Legend vector
			CLegendInfo LegendInfo;
			LegendInfo.m_csName = csName;
			
			// Get the blocks attribute iterator
			if (acdbOpenObject(pInsert, objInsert, AcDb::kForRead) != Acad::eOk) { continue; }
			pIter = pInsert->attributeIterator();
			// AcDbExtents extents; pInsert->getGeomExtents(extents);
			pInsert->close();

			// Loop through the attributes in the block (if any)
			CString csTag;
			for (pIter->start(); !pIter->done(); pIter->step())
			{
				// Get the attribute's object ID and open it for writing
				objAtt = pIter->objectId();
				if (acdbOpenObject(pAtt, objAtt, AcDb::kForWrite) != Acad::eOk) continue;

				// Get the attribute's tag
				csTag = pAtt->tag();

				// Set the attribute's value based on the tag
				if (!csTag.CompareNoCase(_T("Legend"))) LegendInfo.m_csDescription = pAtt->textString();
				pAtt->close();
			}
			delete pIter;

			LegendInfo_Vector.push_back(LegendInfo);

			/*
			// Get the geometric extents of this block to calculate its actual insert
			if (lCtr) { acutPolar(ptLegend, PIby2, GetYShift(csName), ptLegend); }

			// Insert the block
			if (insertBlock((LPCTSTR)csName, _T("NOTES"), ptLegend, 2.5, 2.5, 0.0, g_csPoleTextStyle, objLegend, FALSE) == FALSE) continue;
			
			ChangeAttributeLocation(objLegend, _T("Legend"), _T("E"));
						
			// Get the blocks attribute iterator
			if (acdbOpenObject(pInsert, objLegend, AcDb::kForRead) != Acad::eOk) { continue; }
			pIter = pInsert->attributeIterator();
			AcDbExtents extents; pInsert->getGeomExtents(extents);
			pInsert->close();

			// Loop through the attributes in the block (if any)
			CString csTag;
			for (pIter->start(); !pIter->done(); pIter->step())
			{
				// Get the attribute's object ID and open it for writing
				objAtt = pIter->objectId();
				if (acdbOpenObject(pAtt, objAtt, AcDb::kForWrite) != Acad::eOk) continue;

				// Get the attribute's tag
				csTag = pAtt->tag();

				// Set the attribute's value based on the tag
				if (csTag.CompareNoCase(_T("Legend"))) pAtt->setTextString(_T("")); else pAtt->setInvisible(Adesk::kFalse);
				pAtt->close();
			}
			delete pIter;

			//////////////////////////////////////////////////////////////////////////
			// Location of next legend item
			//////////////////////////////////////////////////////////////////////////
			// acutPolar(ptLegend, PIby2, fabs(extents.maxPoint().y - extents.minPoint().y) / 2, ptLegend);
			ptLegend[Y] = extents.maxPoint().y;
			acutPolar(ptLegend, PIby2, fabs(extents.maxPoint().y - extents.minPoint().y) / 2, ptLegend);
			*/
		}

		// Finally write the title "LEGEND"
		// insertBlock(_T("SM_LEGEND"), _T("NOTES"), ptLegend, 1.0, 1.0, 0.0, g_csPoleTextStyle, objLegend, FALSE);
	}

	acedSSFree(ssLegend);

	// Place the legend now
	if (LegendInfo_Vector.size()) PlaceLegend(LegendInfo_Vector);
}

//////////////////////////////////////////////////////////////////////////
// *********** Functions related to EE_CIRCUIT command ***************
//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_Circuit
// Description  : Called when the user enters the "EE_Circuit" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_Circuit()
{
	switchOff();

	// Get the string to process
	TCHAR szInputStr[15];
	acedInitGet(RSG_NONULL, NULL);
	int iRet = acedGetString(FALSE, _T("\nOption string: "), szInputStr);
	if (iRet == RTCAN) return;
	if (szInputStr == _T("")) return;

	// UAT - 144. If no view ports are active EXIT the command.
	// If in tile mode check if any view port is activated
	if (!acdbHostApplicationServices()->workingDatabase()->tilemode())
	{
		// Get the current CVPORT value. If the current viewport number is "1" simply return the function
		struct resbuf rbCVPort; acedGetVar(_T("CVPORT"), &rbCVPort);
		if (rbCVPort.resval.rint == 1)
		{
			acutPrintf(_T("\nNo viewports are active. Activate a viewport before invoking this command."));
			return;
		}
	}
		
	// Get the layers to be switched on for the option selected.
	CStringArray csaLayers; 
	CQueryTbl tblLayerCtrl;
	CString csSQL;
	csSQL.Format(_T("SELECT [Layer] FROM tblLayerControl WHERE [Option] = '%s'"), szInputStr);
	if (!tblLayerCtrl.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, __FUNCTION__)) return;
	if (tblLayerCtrl.GetRows() > 0) { tblLayerCtrl.GetColumnAt(0, csaLayers); }
	
	// Call the function to turn off the layers and the rest is turned on.
	struct resbuf rbLayer; acedGetVar(_T("CLAYER"), &rbLayer);

	CString csSubStr; csSubStr.Format(_T("%s"), szInputStr);
	// New requirement on 03-07-12 reverted: CString csSubStr; csSubStr.Format(_T("_%s_NEW"), szInputStr);
	turnOnLayers(rbLayer.resval.rstring, csaLayers, csSubStr.MakeUpper());
	acedCommandS(RTSTR, _T(".REGEN"), NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_Help
// Description  : Called when the user enters the "IE_HELP" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_Help() 
{
	CTime tmStart(CTime::GetCurrentTime());
	while (T)
	{
		if (DisplayHelp((DWORD) _T("Introduction.htm"))) break;

		CTime tmCur(CTime::GetCurrentTime());
		CTimeSpan tmSpan; tmSpan = tmCur - tmStart;
		if (tmSpan.GetTotalSeconds() > 5) break;
	}
}


