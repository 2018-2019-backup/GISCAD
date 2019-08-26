#include "stdafx.h"
#include "lightinfo.h"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightInfo::CLightInfo
// Description  : Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLightInfo::CLightInfo(void)
{
	m_iLightNumber = 0;
	m_csLightNumber = _T(""); 
	m_csIsStandard = _T("");
	m_csLanternSchedule = _T("");
	m_csLanternStatus = _T("");
	m_csLanternType = _T("");
	m_csLanternStockCode = _T("");
	m_csLanternLamp = _T("");
	m_csLampStockCode = _T("");
	m_csRateCode = _T("");
	m_csKingbolt = _T("");
	m_csLanternMast = _T("");
	m_csMastCode = _T("");
	m_csLanternOutreach = _T("");
	m_csOutreachCode = _T("");
	m_csLanternBracket = _T("");
	m_csBracketCode = _T("");
	m_csLanternLocation = _T("");
	m_csLanternCategory = _T("");
	m_csAssetColumn = _T("");
	m_csAssetLantern = _T("");
	m_csLanternColumn = _T("");
	m_csChargeTo = _T("");

//	m_csMASTRATE= _T("");
//m_csOutreachRate= _T("");
//m_csBracketRate= _T("");

	
	m_csReplaceIsStandard = _T("");    
  m_csReplaceLanternSchedule = _T("");
  m_csReplaceLanternType = _T("");
  m_csReplaceLanternLamp = _T("");
  m_csReplaceRateCode = _T("");
  m_csReplaceLanternCategory = _T("");
	m_csReplaceAssetColumn = _T("");
  m_csReplaceAssetLantern = _T("");
	m_csReplaceChargeTo = _T("");

	m_iQty = 1;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightInfo::~CLightInfo
// Description  : Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLightInfo::~CLightInfo(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : sortLightInfo
// Description  : Sorts the vector array of light information instances in ascending order of light numbers.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void sortLightInfo (std:: vector <CLightInfo> &lightInfo)
{
	CLightInfo tmpLightInfo;
	for (int i = 0; i < lightInfo.size() - 1; i++)
	{
		for (int j = i + 1; j < lightInfo.size(); j++)
		{
			if (lightInfo[i].m_iLightNumber >= lightInfo[j].m_iLightNumber)
			{
				tmpLightInfo = lightInfo[i]; // Swapping entire class
				lightInfo[i] = lightInfo[j];
				lightInfo[j] = tmpLightInfo;
			}
		}
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : consolidateLightInfo
// Description  : Consolidate the light numbers based on its specification
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void consolidateLightInfo (std:: vector <CLightInfo> &lightInfo_Vector)
{
	for (int i = 0; i < lightInfo_Vector.size() - 1; i++)
	{
		for (int j = i + 1; j < lightInfo_Vector.size(); j++)
		{
			// These are already matched and consolidated, so skip them
			if (!lightInfo_Vector.at(i).m_iQty) continue;

			if ((lightInfo_Vector.at(i).m_csLanternStatus	   == lightInfo_Vector.at(j).m_csLanternStatus)		  &&
					(lightInfo_Vector.at(i).m_csLanternType	 	   == lightInfo_Vector.at(j).m_csLanternType)	 		  &&
					(lightInfo_Vector.at(i).m_csLanternStockCode == lightInfo_Vector.at(j).m_csLanternStockCode)  &&
					(lightInfo_Vector.at(i).m_csLanternLamp			 == lightInfo_Vector.at(j).m_csLanternLamp) 		  &&
					(lightInfo_Vector.at(i).m_csLampStockCode		 == lightInfo_Vector.at(j).m_csLampStockCode)		  &&
					(lightInfo_Vector.at(i).m_csLanternMast			 == lightInfo_Vector.at(j).m_csLanternMast)			  &&
					(lightInfo_Vector.at(i).m_csMastCode       	 == lightInfo_Vector.at(j).m_csMastCode)          &&
					(lightInfo_Vector.at(i).m_csLanternOutreach	 == lightInfo_Vector.at(j).m_csLanternOutreach)   &&
					(lightInfo_Vector.at(i).m_csOutreachCode	   == lightInfo_Vector.at(j).m_csOutreachCode)		  &&
					(lightInfo_Vector.at(i).m_csRateCode				 == lightInfo_Vector.at(j).m_csRateCode)			 	  &&
					(lightInfo_Vector.at(i).m_csLanternCategory	 == lightInfo_Vector.at(j).m_csLanternCategory)   &&
					(lightInfo_Vector.at(i).m_csChargeTo				 == lightInfo_Vector.at(j).m_csChargeTo)					&&	
					(lightInfo_Vector.at(i).m_csLanternBracket 	 == lightInfo_Vector.at(j).m_csLanternBracket)    &&
					(lightInfo_Vector.at(i).m_csBracketCode			 == lightInfo_Vector.at(j).m_csBracketCode)			  &&
				  (lightInfo_Vector.at(i).m_csKingbolt			 	 == lightInfo_Vector.at(j).m_csKingbolt)
				)
			{
				// Add the light number matched and increment the quantity
				lightInfo_Vector.at(i).m_csLightNumber += _T(",") + lightInfo_Vector.at(j).m_csLightNumber;
				lightInfo_Vector.at(i).m_iQty          += 1;
				/*if(lightInfo_Vector.at(i).m_iColumnQty<1)
				lightInfo_Vector.at(i).m_iColumnQty=1;*/
				if(lightInfo_Vector.at(i).m_csLanternColumn != _T(""))
				{
					lightInfo_Vector.at(i).m_iColumnQty+=1;
				}
				// Edit the quantity of the matched light info to "0", so that we can remove it after this loop
				lightInfo_Vector.at(j).m_iQty = 0;
			}

			////Add Qty if only one Column present in drawing
			//if(lightInfo_Vector.at(i).m_iColumnQty<1)
			//{
			//	if(lightInfo_Vector.at(i).m_csLanternColumn != _T(""))
			//		lightInfo_Vector.at(i).m_iColumnQty+=1;
			//}
		}
	}

	// Remove all the unused ones
	int iRemove = 0;
	std::vector <CLightInfo> tmpLightInfo_Vector = lightInfo_Vector; lightInfo_Vector.clear();
	for (int i = 0; i < tmpLightInfo_Vector.size(); i++)
	{
		if (tmpLightInfo_Vector.at(i).m_iQty) lightInfo_Vector.push_back(tmpLightInfo_Vector.at(i));
	}

	return;
}
