#include "StdAfx.h"
#include "NSLayerTbl.h"
#include <atlstr.h>

CNSLayerTbl::CNSLayerTbl(void)
{
	m_pStdDWGMgr = NULL;
	m_pLocalDWGMgr = NULL;
}

//CNSLayerTbl::~CNSLayerTbl(void)
//{
//}
void CNSLayerTbl::setStdDWGObject(CNSDWGMgr* pStdDWGMgr)
{
	m_pStdDWGMgr = pStdDWGMgr;
}
void CNSLayerTbl::setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr)
{
	m_pLocalDWGMgr = pLocalDWGMgr;
}
int CNSLayerTbl::validate(void)
{
	//getting all the properties of the std dwg file
	std::map<NSSTRING,CNSLayer*> mapOfStdLayers;
	m_pStdDWGMgr->getLayerProperties(mapOfStdLayers);

	//getting all the properties of the local dwg file
	std::map<NSSTRING,CNSLayer*> mapOfLocalLayers;
	m_pLocalDWGMgr->getLayerProperties(mapOfLocalLayers);

	CNSLayer *pLocalLayer, *pStdLayer ;
	std::map<NSSTRING,CNSLayer*>::iterator LayerIter, LayerStdIter;
	NSSTRING strLayerName;
	TCHAR szFormat[_MAX_PATH + 1];
	for(LayerIter = mapOfLocalLayers.begin(); LayerIter != mapOfLocalLayers.end(); LayerIter++)
	{
		//Get the Layer name and check it in the std dwg file
		strLayerName = (*LayerIter).first;
		//NSSTRING strtocmp = _T("_User M");
		CString strtocmp = _T("_USER");
		CString tempStrLayerName = strLayerName.substr(0,5).c_str();


		//if(strLayerName.substr(0,5) != strtocmp.substr(0,5))
		if(tempStrLayerName.CompareNoCase(strtocmp))
		{

			LayerStdIter = mapOfStdLayers.find(strLayerName);
			if(LayerStdIter == mapOfStdLayers.end())
			{
#ifdef _INTERNAL_
				//NSSTRUPR((TCHAR*)strLayerName.c_str());
				NSSTRUPR((TCHAR*)strLayerName.c_str(),strLayerName.length()+1);
#else
				NSSTRUPR((TCHAR*)strLayerName.c_str(),strLayerName.length()+1);
#endif

				if((strLayerName.find(_T("_NEW")) == strLayerName.length()-4)||
					(strLayerName.find(_T("_NEW_"))!=-1) || strLayerName.find(_T("_NEW")))
				{

				}
				else
				{
					if((NSSTRCMP(strLayerName.c_str(), _T("GIS_ERRORS")) != 0) 
						&& (NSSTRCMP(strLayerName.c_str(), _T("UNKNOWN_FEATURES"))) != 0
						&& (NSSTRCMP(strLayerName.c_str(), _T("VALIDATION_LOG"))) != 0)
					{
						//NSSPRINTF(szFormat,_T("Layer \'%s\', Name is non-standard"), strLayerName.c_str());
						//WriteIDToLog(LOG_CAD, szFormat, ERR_CAD_4);
				//if(ERR_CAD_13!=104 && ERR_CAD_13!=113)
						WriteErrorRec(LOG_CAD,ERR_CAD_4,_T("L$"),strLayerName.c_str());

					}
				}
			}
			else
			{
				//Compare all the properties of the Layer
				pStdLayer = (*LayerStdIter).second;
				pLocalLayer = (*LayerIter).second;
				//Check whether the color values are equal
				int nStdcolor = pStdLayer->getColor();
				int nLocalcolor = pLocalLayer->getColor();
				if(nStdcolor != nLocalcolor)
				{
					//NSSPRINTF(szFormat,_T("Layer property \'%s\' values are different for layer \'%s\'. Current Value = \'%i\' , Standard Value = \'%i\'"),
					//_T("Color"),strLayerName.c_str(), nLocalcolor , nStdcolor);
					//WriteIDToLog(LOG_CAD, szFormat, ERR_CAD_5);

					TCHAR currVAL[_MAX_PATH + 1];
					NSSPRINTF(currVAL,_T("%i"), nLocalcolor);
					TCHAR stdVAL[_MAX_PATH + 1];
					NSSPRINTF(stdVAL,_T("%i"), nStdcolor);
					WriteErrorRec(LOG_CAD,ERR_CAD_5,_T("Color"),strLayerName.c_str(), currVAL , stdVAL);
					//TCHAR color[_MAX_PATH + 1];
					//NSSPRINTF(color,_T("%s"),"Color_");
					//stringstream  Localcolor= "Hello" currVAL;
					/*	std::string strtemp;
					strtemp.append("Colour_");
					strtemp+=nLocalcolor;

					std::string strtemp1;
					strtemp1.append("Colour_");
					strtemp1+=nStdcolor;*/

					TCHAR currStyleName[_MAX_PATH + 1];
					//NSSPRINTF(currStyleName,_T("%s"), strtemp);

					NSSPRINTF(currStyleName,_T("Colour_%i"),nLocalcolor);

					//strtemp+=nLocalcolor;

					//strtemp.append(nLocalcolor.str());

					TCHAR stdStyleName[_MAX_PATH + 1];
					//NSSPRINTF(stdStyleName,_T("%s"), strtemp1);
					NSSPRINTF(stdStyleName,_T("Colour_%i"),nStdcolor);

					WriteErrorRec(LOG_CAD,ERR_CAD_12,_T("PlotStyleName"), strLayerName.c_str(), currStyleName,stdStyleName); //strLocalPlotStyleName.c_str(), strStdPlotStyleName.c_str()); 

				}
				//Check whether the IsFrozen values are equal
				bool bStdIsFrozen = pStdLayer->getIsFrozen();
				bool bLocalIsFrozen = pLocalLayer->getIsFrozen();
				if(bStdIsFrozen != bLocalIsFrozen)
				{
					//NSSPRINTF(szFormat,_T("Layer property \'%s\' value is different for layer \'%s\'. Current Value = \'%i\' ,Standard Value = \'%i\'"),
					//_T("Freeze"),strLayerName.c_str(), bLocalIsFrozen, bStdIsFrozen);
					//WriteIDToLog(LOG_CAD, szFormat, ERR_CAD_6);	

					TCHAR currVAL[_MAX_PATH + 1];
					NSSPRINTF(currVAL,_T("%i"), bLocalIsFrozen);
					TCHAR stdVAL[_MAX_PATH + 1];
					NSSPRINTF(stdVAL,_T("%i"), bStdIsFrozen);
					WriteErrorRec(LOG_CAD,ERR_CAD_6,_T("Freeze"),strLayerName.c_str(), currVAL, stdVAL); 

				}	

				//Check whether the IsLocked values are equal
				bool bstdIsLocked = pStdLayer->getIsLocked();
				bool bLocalIsLocked = pLocalLayer->getIsLocked();
				if(bstdIsLocked != bLocalIsLocked)
				{
					//NSSPRINTF(szFormat,_T("Layer property \'%s\' value is different for layer \'%s\'. Current Value = \'%i\' , Standard Value = \'%i\'"),
					//_T("Lock"),strLayerName.c_str(), bLocalIsLocked, bstdIsLocked);
					//WriteIDToLog(LOG_CAD, szFormat, ERR_CAD_7);

					TCHAR currVAL[_MAX_PATH + 1];
					NSSPRINTF(currVAL,_T("%i"), bLocalIsLocked);
					TCHAR stdVAL[_MAX_PATH + 1];
					NSSPRINTF(stdVAL,_T("%i"), bstdIsLocked);
					WriteErrorRec(LOG_CAD,ERR_CAD_7,_T("Lock"),strLayerName.c_str(), currVAL, stdVAL); 

				}	

				//Check whether the IsOff values are equal
				bool bStdIsOff = pStdLayer->getIsOff();
				bool bLocalIsOff = pLocalLayer->getIsOff();
				if(bStdIsOff != bLocalIsOff)
				{
					//NSSPRINTF(szFormat,_T("Layer property \'%s\' value is different for layer \'%s\'. Current Value = \'%i\' , Standard Value = \'%i\'"),
					//_T("On"),strLayerName.c_str(), bLocalIsOff, bStdIsOff);
					//WriteIDToLog(LOG_CAD, szFormat, ERR_CAD_8);

					TCHAR currVAL[_MAX_PATH + 1];
					NSSPRINTF(currVAL,_T("%i"), bLocalIsOff);
					TCHAR stdVAL[_MAX_PATH + 1];
					NSSPRINTF(stdVAL,_T("%i"), bStdIsOff);
					WriteErrorRec(LOG_CAD,ERR_CAD_8,_T("On"),strLayerName.c_str(), currVAL, stdVAL); 

				}	

				//Check whether the LineWeight values are equal
				int nStdLineWt = pStdLayer->getLineWt();
				NSSTRING strStdLineWt =  getLineWtString(nStdLineWt);
				int nLocalLineWt = pLocalLayer->getLineWt();
				NSSTRING strLocalLineWt = getLineWtString(nLocalLineWt);
				if(nStdLineWt != nLocalLineWt)
				{
					//NSSPRINTF(szFormat,_T("Layer property \'%s\' value is different for layer \'%s\'. Current Value = \'%s\' , Standard Value = \'%s\'"),
					//_T("LineWeight"),strLayerName.c_str(), strLocalLineWt.c_str(), strStdLineWt.c_str());
					//WriteIDToLog(LOG_CAD, szFormat, ERR_CAD_9);

					WriteErrorRec(LOG_CAD,ERR_CAD_9,_T("LineWeight"),strLayerName.c_str(), strLocalLineWt.c_str(), strStdLineWt.c_str()); 

				}

				//Check whether the PlotState values are equal
				int bStdPlotState = pStdLayer->getPlotState();
				int bLocalPlotState = pLocalLayer->getPlotState();
				if(bStdPlotState != bLocalPlotState)
				{
					//NSSPRINTF(szFormat,_T("Layer property \'%s\' value is different for layer \'%s\'. Current Value = \'%i\' , Standard Value = \'%i\'"),
					//_T("PlotState"),strLayerName.c_str(), bLocalPlotState, bStdPlotState);
					//WriteIDToLog(LOG_CAD, szFormat, ERR_CAD_10);

					TCHAR currVAL[_MAX_PATH + 1];
					NSSPRINTF(currVAL,_T("%i"), bLocalPlotState);
					TCHAR stdVAL[_MAX_PATH + 1];
					NSSPRINTF(stdVAL,_T("%i"), bStdPlotState);
					WriteErrorRec(LOG_CAD,ERR_CAD_10,_T("PlotState"),strLayerName.c_str(), currVAL, stdVAL); 

				}

				//Check whether the Linetype names are equal
				NSSTRING strStdLineTypeName = pStdLayer->getLineTypeName();
				NSSTRING strLocalLineTypeName = pLocalLayer->getLineTypeName();
				if(strStdLineTypeName != strLocalLineTypeName)
				{
					//NSSPRINTF(szFormat,_T("Layer property \'%s\' value is different for layer \'%s\'. Current Value = \'%s\' , Standard Value = \'%s\'"), 
					//_T("LineType"), strLayerName.c_str(), strLocalLineTypeName.c_str(), strStdLineTypeName.c_str());
					//WriteIDToLog(LOG_CAD, szFormat, ERR_CAD_11);

					WriteErrorRec(LOG_CAD,ERR_CAD_11,_T("LineType"), strLayerName.c_str(), strLocalLineTypeName.c_str(), strStdLineTypeName.c_str()); 

				}

				//Check whether the PlotStyleName values are equal
				NSSTRING strStdPlotStyleName = pStdLayer->getPlotStyleName();
				NSSTRING strLocalPlotStyleName = pLocalLayer->getPlotStyleName();
				//if(strStdPlotStyleName != strLocalPlotStyleName)
				//{
				//	//NSSPRINTF(szFormat,_T("Layer property \'%s\' value is different for layer \'%s\'. Current Value = \'%s\' , Standard Value = \'%s\'"), 
				//	//_T("PlotStyleName"), strLayerName.c_str(), strLocalPlotStyleName.c_str(), strStdPlotStyleName.c_str());
				//	//WriteIDToLog(LOG_CAD, szFormat, ERR_CAD_12);

				//	WriteErrorRec(LOG_CAD,ERR_CAD_12,_T("PlotStyleName"), strLayerName.c_str(), strLocalPlotStyleName.c_str(), strStdPlotStyleName.c_str()); 
				//	
				//}
			}
		}
	}

	//Delete the objects of the class LayerTbl from the map of both std and local.
	for(LayerIter = mapOfLocalLayers.begin(); LayerIter != mapOfLocalLayers.end(); LayerIter++)
	{
		delete (*LayerIter).second;
	}
	for(LayerIter = mapOfStdLayers.begin(); LayerIter != mapOfStdLayers.end(); LayerIter++)
	{
		delete (*LayerIter).second;
	}
	//Clear the maps
	mapOfLocalLayers.clear();
	mapOfStdLayers.clear();
	return NS_SUCCESS;
}

NSSTRING CNSLayerTbl::getLineWtString(int nNumber)
{
	NSSTRING strLineWt;
	switch(nNumber)
	{
	case -1: strLineWt = _T("ByLayer"); break;
	case -2: strLineWt = _T("ByBlock"); break;
	case -3: strLineWt = _T("Default"); break;
	default: 
		{
			TCHAR szFormat[_MAX_PATH + 1];
			NSSPRINTF(szFormat,_T("%lf mm"), (double)nNumber / 100.0);
			strLineWt = szFormat; break;
		}
	}
	return strLineWt;
}