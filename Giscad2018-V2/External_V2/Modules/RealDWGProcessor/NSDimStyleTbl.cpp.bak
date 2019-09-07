#include "stdafx.h"
#include "NSDimStyleTbl.h"

CNSDimStyleTbl::CNSDimStyleTbl(void)
{
	m_pStdDWGMgr = NULL;
	m_pLocalDWGMgr = NULL;
}

//CNSDimStyleTbl::~CNSDimStyleTbl(void)
//{
//}

void CNSDimStyleTbl::setStdDWGObject(CNSDWGMgr* pStdDWGMgr)
{
	m_pStdDWGMgr = pStdDWGMgr;
}

void CNSDimStyleTbl::setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr)
{
	m_pLocalDWGMgr = pLocalDWGMgr;
}
/*
@brief		This function validates by comparing the different DimStyle properties 
			of std dwg file and local dwg file.
*/
int CNSDimStyleTbl::validate(void)
{
	//getting all the dimstyle properties of std dwg files 
	std::map<NSSTRING,CNSDimStyle*> mapOfStdDimStyle;
	m_pStdDWGMgr->getDimStyleList(mapOfStdDimStyle);
	
	//getting all the dimstyle properties of local dwg files 
	std::map<NSSTRING,CNSDimStyle*> mapOfLocalDimStyle;
	m_pLocalDWGMgr->getDimStyleList(mapOfLocalDimStyle);
	
	CNSDimStyle *pLocalDimStyle, *pStdDimStyle ;
	std::map<NSSTRING,CNSDimStyle*>::iterator DimStyleIter, DimStyleStdIter;
	NSSTRING strDimStyleName;
	TCHAR szFormat[_MAX_PATH + 1];
	for(DimStyleIter = mapOfLocalDimStyle.begin(); DimStyleIter != mapOfLocalDimStyle.end(); DimStyleIter++)
	{
		//Get the DimStyle name and check it in the std dwg file.
		strDimStyleName = (*DimStyleIter).first;
		DimStyleStdIter = mapOfStdDimStyle.find(strDimStyleName);
		if(DimStyleStdIter == mapOfStdDimStyle.end())
		{
			//NSSPRINTF(szFormat,_T("Dimstyle \'%s\', Name is non-standard"), strDimStyleName.c_str());
			
			//WriteIDToLog(LOG_CAD, szFormat, ERR_CAD_0);

			WriteErrorRec(LOG_CAD,ERR_CAD_0,_T("DS$"),strDimStyleName.c_str() ); 
		}
		else
		{
			//Compare all the properties of the DimStyle
			pStdDimStyle = (*DimStyleStdIter).second;
			pLocalDimStyle = (*DimStyleIter).second;
		
			NSSTRING strStdFont =  pStdDimStyle->getFont();
			NSSTRING strLocalFont = pLocalDimStyle->getFont();
			if(strStdFont != strLocalFont)
			{
				//NSSPRINTF(szFormat,_T("Dimstyle property \'%s\' value is different for Dimstyle \'%s\'. Current Value = \'%s\' , Standard Value = \'%s\'"), 
				//_T("Font Name"), strDimStyleName.c_str(), strLocalFont.c_str(), strStdFont.c_str());
				//WriteIDToLog(LOG_CAD,szFormat,ERR_CAD_1);

				WriteErrorRec(LOG_CAD,ERR_CAD_1,_T("Font Name"), strDimStyleName.c_str(), strLocalFont.c_str(), strStdFont.c_str()); 
			}
		
			NSSTRING strStdLineTypeName = pStdDimStyle->getLineTypeName();
			NSSTRING strLocalLineTypeName = pLocalDimStyle->getLineTypeName();
			if(strStdLineTypeName != strLocalLineTypeName)
			{
				//NSSPRINTF(szFormat,_T("Dimstyle property \'%s\' value is different for Dimstyle \'%s\'. Current Value = \'%s\' , Standard Value = \'%s\'"), 
				//_T("Linetype"), strDimStyleName.c_str(), strLocalLineTypeName.c_str(), strStdLineTypeName.c_str());
                //WriteIDToLog(LOG_CAD,szFormat,ERR_CAD_2);

				WriteErrorRec(LOG_CAD,ERR_CAD_2,_T("Linetype"), strDimStyleName.c_str(), strLocalLineTypeName.c_str(), strStdLineTypeName.c_str() ); 

			}
		
			double dStdTxtHt = pStdDimStyle->getTxtHt();
			double dLocalTxtHt = pLocalDimStyle->getTxtHt();
			if(dStdTxtHt != dLocalTxtHt)
			{
				//NSSPRINTF(szFormat,_T("Dimstyle property \'%s\' value is different for Dimstyle \'%s\'. Current Value = \'%lf\' , Standard Value = \'%lf\'"), 
				//_T("Text height"), strDimStyleName.c_str(), dLocalTxtHt, dStdTxtHt);
				//WriteIDToLog(LOG_CAD,szFormat,ERR_CAD_3);

				TCHAR currval[_MAX_PATH + 1];
				NSSPRINTF(currval,_T("%lf"), dLocalTxtHt);
				TCHAR stdval[_MAX_PATH + 1];
				NSSPRINTF(stdval,_T("%lf"), dStdTxtHt);
				WriteErrorRec(LOG_CAD,ERR_CAD_3,_T("Text height"), strDimStyleName.c_str(), currval, stdval) ; 

			}
		}
	}
	//Delete the objects of the class Dimstyle from the map of both std and local.
	for(DimStyleIter = mapOfLocalDimStyle.begin(); DimStyleIter != mapOfLocalDimStyle.end(); DimStyleIter++)
	{
		delete (*DimStyleIter).second;
	}
	for(DimStyleIter = mapOfStdDimStyle.begin(); DimStyleIter != mapOfStdDimStyle.end(); DimStyleIter++)
	{
		delete (*DimStyleIter).second;
	}
	//Clear the maps
	mapOfLocalDimStyle.clear();
	mapOfStdDimStyle.clear();
	return NS_SUCCESS;
}