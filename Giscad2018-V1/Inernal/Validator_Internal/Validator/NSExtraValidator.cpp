#include "StdAfx.h"
#include "NSExtraValidator.h"
#include "NSErrorCircle.h"
extern CNSErrorCircle gErrorCircle;
CNSExtraValidator::CNSExtraValidator(void)
{	
	m_pStdDWGMgr = NULL;
	m_pLocalDWGMgr = NULL;
}

CNSExtraValidator::~CNSExtraValidator(void)
{
}

void CNSExtraValidator::setStdDWGObject(CNSDWGMgr* pStdDWGMgr)
{
	m_pStdDWGMgr = pStdDWGMgr;
}

void CNSExtraValidator::setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr)
{
	m_pLocalDWGMgr = pLocalDWGMgr;
}

int CNSExtraValidator::validate(bool m_bCadValidation, bool m_bGisValidation)
{
	
TCHAR szFormat[_MAX_PATH + 1];
if(m_bGisValidation == true)
{
	
	std::vector<CNSPoint3D> vectOfPts;
	std::vector<CNSPoint3D>::iterator PtsItr;

	//Check for the blocks and linear features that are present outside the export area;
	std::vector<CNSPoint3D>vectPointsofInterest;
	std::map<NSSTRING, NSSTRING>mapTitleAttributes;
	std::map<NSSTRING, NSSTRING> mapofPrompValues;
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ExportAreaOfInterest"),_T("")));
	m_pLocalDWGMgr->listTitleBlockHeaderAttributes(mapTitleAttributes, mapofPrompValues);
	NSSTRING strAttValues = mapTitleAttributes[_T("ExportAreaOfInterest")];


//#ifndef _INTERNAL_
	m_pLocalDWGMgr->convertString2Vertices(strAttValues, vectPointsofInterest);
	if(!vectPointsofInterest.empty())
		m_pLocalDWGMgr->withinarea(vectPointsofInterest, vectOfPts);	

	for(PtsItr = vectOfPts.begin(); PtsItr != vectOfPts.end(); PtsItr++)
	{
		gErrorCircle.logError(Err_ExportArea, _T("The Entity is out of Export Area of Interest"), LOG_GIS, (*PtsItr).m_dXX, (*PtsItr).m_dYY, ERR_GIS_26);
	}
	vectOfPts.clear();
//#endif

	//Check for closed polylines on "cadastre" layer. These are the layers for which
    // NumberOfNodes in NewLinearFeatures column is 0. We will first get all such layers
    // and then read/check polylines only on those layers.
    
    /* THIS PART HAS NOT BEEN INCLUDED IN THE VALIDATION NOW AS PER CLIENT REQUEST*
       THIS WILL BE INCLUDED IN THE NEXT PHASE WITH SOME CHANGES IN REQUIREMENT 
    */

    /*TCHAR szQuery[_MAX_PATH + 1];
    NSSPRINTF(szQuery, _T("Select NewLayer from NewLinearFeatures where  NumberOfNodes = 0"));
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
    std::vector<NSSTRING> szCadastreLayers;
	if(-1 != nDBRet)
	{
        std::vector<variant_t> vVectNewLayer;
        CNSDatabaseMgr::getInstance()->getValue(_T("NewLayer"), vVectNewLayer);
    
        std::vector<variant_t>::iterator varientVectItr;
        for(varientVectItr = vVectNewLayer.begin(); varientVectItr != vVectNewLayer.end(); varientVectItr++)
	    {
            szCadastreLayers.push_back((NSSTRING)(*varientVectItr).bstrVal);
        }
       
        if(szCadastreLayers.size()>0)
        {
	        m_pLocalDWGMgr->getOpenPLinesOnDeveloperPlan(vectOfPts,szCadastreLayers);	
	        for(PtsItr = vectOfPts.begin(); PtsItr != vectOfPts.end(); PtsItr++)
	        {
		        gErrorCircle.logError(Err_Developer, _T("Polylines on Cadastre data layers are not closed."), LOG_GIS, (*PtsItr).m_dXX, (*PtsItr).m_dYY);
	        }
	        vectOfPts.clear();
        }
    }*/
	
	//Check for geometries on "0" layer
	m_pLocalDWGMgr->getGeometryOnZeroLayer(vectOfPts);
	for(PtsItr = vectOfPts.begin(); PtsItr != vectOfPts.end(); PtsItr++)
	{
		gErrorCircle.logError(Err_Zero, _T("Geometry found on \'0\' layer"), LOG_GIS, (*PtsItr).m_dXX, (*PtsItr).m_dYY, ERR_GIS_27);
	}
	vectOfPts.clear();

	//Check the visibility of the blocks
	std::map<NSSTRING, std::map<NSSTRING,CNSAttributeRef*>*> mapOfStdBlkAttrs;
	m_pStdDWGMgr->checkBlockAttributeVisibility(mapOfStdBlkAttrs);

	std::map<NSSTRING, std::map<NSSTRING,CNSAttributeRef*>*> mapOfLocalBlkAttrs;
	m_pLocalDWGMgr->checkBlockAttributeVisibility(mapOfLocalBlkAttrs);

	NSSTRING strLocalBlkName;
	std::map<NSSTRING, std::map<NSSTRING,CNSAttributeRef*>*>::iterator StdBlkItr, LocalBlkItr;
	std::map<NSSTRING, CNSAttributeRef*>* pStdMap = NULL;
	std::map<NSSTRING, CNSAttributeRef*>* pLocalMap = NULL;
	std::map<NSSTRING, CNSAttributeRef*>::iterator StdMapItr, LocalMapItr;
	for(LocalBlkItr = mapOfLocalBlkAttrs.begin(); LocalBlkItr != mapOfLocalBlkAttrs.end(); LocalBlkItr++)
	{
		strLocalBlkName = (*LocalBlkItr).first;
		StdBlkItr = mapOfStdBlkAttrs.find(strLocalBlkName);
		if(StdBlkItr != mapOfStdBlkAttrs.end())
		{
			pStdMap = (*StdBlkItr).second;
			pLocalMap = (*LocalBlkItr).second;
			
			for(LocalMapItr = pLocalMap->begin(); LocalMapItr != pLocalMap->end(); LocalMapItr++)
			{
				StdMapItr = pStdMap->find((*LocalMapItr).first);
				if(StdMapItr == pStdMap->end())
				{
					    int nErrNumber ;
						//NSSPRINTF(szFormat,_T("Block tag name \'%s\' not found for block \'%s\'"), (*LocalMapItr).first.c_str(), (*LocalBlkItr). first.c_str());
						//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_2);
						
						nErrNumber = WriteErrorRecEx(LOG_GIS,ERR_GIS_2,_T("T$"),(*LocalMapItr).first.c_str(),_T("B$"),(*LocalBlkItr). first.c_str());

				}	
				else
				{
					if(NS_FAIL == compareAttribute((*StdMapItr).second , (*LocalMapItr).second))
					{
						int nErrNumber ;
						//NSSPRINTF(szFormat,_T("Block tag \'%s\' visibility values different for block \'%s\'"), (*LocalMapItr).first.c_str(), (*LocalBlkItr). first.c_str());
						//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_3);

						nErrNumber = WriteErrorRecEx(LOG_GIS,ERR_GIS_3,_T("T$"),(*LocalMapItr).first.c_str(),_T("B$"),(*LocalBlkItr). first.c_str());
					}
				}
			}
		}
		else
		{
			int nErrNumber;
			//NSSPRINTF(szFormat,_T("Non Standard block found \'%s\'"), strLocalBlkName.c_str());		    
			//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_4);			
			if(ERR_GIS_4!=204)
				WriteErrorRec(LOG_GIS,ERR_GIS_4,_T("B$"),strLocalBlkName.c_str() );

		}
	}
	releaseAttributeMap(mapOfLocalBlkAttrs);
	releaseAttributeMap(mapOfStdBlkAttrs);
/*	for(LocalBlkItr = mapOfLocalBlkAttrs.begin(); LocalBlkItr != mapOfLocalBlkAttrs.end(); LocalBlkItr++)
	{
		pLocalMap = (*LocalBlkItr).second;
		for(LocalMapItr = pLocalMap->begin(); LocalMapItr != pLocalMap->end(); LocalMapItr++)
		{
			delete (*LocalMapItr).second;
		}
		pLocalMap->clear();
		delete pLocalMap;
	}
	delete mapOfLocalBlkAttrs;
	mapOfLocalBlkAttrs.clear();

	for(StdBlkItr = mapOfStdBlkAttrs.begin(); StdBlkItr != mapOfStdBlkAttrs.end(); StdBlkItr++)
	{
		delete (*StdBlkItr).second;
	}
	delete mapOfStdBlkAttrs;
	mapOfStdBlkAttrs.clear();
*/
	//Check if the value of attribute 'NOTE' of non standard block is zero 
	CNSValidationDataCollector *pCollector = NULL;
	std::map<LONG_PTR, CNSValidationDataCollector*> mapOfNonStdBlks;
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator nonStdBlkItr;
	STRING_MAP::iterator TempItr;
	m_pLocalDWGMgr->selectNonStdBlocks(mapOfNonStdBlks);
	for(nonStdBlkItr = mapOfNonStdBlks.begin(); nonStdBlkItr != mapOfNonStdBlks.end(); nonStdBlkItr++)
	{
		pCollector = (*nonStdBlkItr).second;
		TempItr = pCollector->m_mapAttributes.find(_T("NOTES"));
		if(TempItr == pCollector->m_mapAttributes.end())
		{
			continue;
		}
		if(NSSTRLEN((*TempItr).second.c_str()) != 0)
		{
			int nErrNumber;
			//NSSPRINTF(szFormat,_T("For Non-Standard block \'%s\', Note:\'%s\'"), pCollector->getBlockName().c_str(), (*TempItr).second.c_str());
			//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_5);

			nErrNumber = WriteErrorRec(LOG_GIS,ERR_GIS_5,pCollector);
			
            CNSPoint3D pt = pCollector->getInsertionPoint();
			gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_5, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
		}
	}
	for(nonStdBlkItr = mapOfNonStdBlks.begin(); nonStdBlkItr != mapOfNonStdBlks.end(); nonStdBlkItr++)
	{
		delete (*nonStdBlkItr).second;
	}
}

	if(m_bCadValidation == true)
	{
		if(m_pLocalDWGMgr->isPaperSpaceValid()==NS_ERR_PAPERSPACE)
		{
			NSSPRINTF(szFormat,_T("Object found on paperspace"));
			//WriteErrorRec(LOG_CAD, szFormat);//WriteToLog(LOG_CAD, szFormat);
			WriteErrorRec(LOG_CAD,ERR_CAD_17,_T("Object found on paperspace"));
			
		}
	}
	
	return NS_SUCCESS;
}

int CNSExtraValidator::compareAttribute(CNSAttributeRef* pStdAtt, CNSAttributeRef* pLocalAtt)
{
	
	if(pStdAtt->getIsVisible() != pLocalAtt->getIsVisible())
		return NS_FAIL;
	return NS_SUCCESS;
}

void CNSExtraValidator::releaseAttributeMap(std::map<NSSTRING, std::map<NSSTRING,CNSAttributeRef*>*>& attrMap)
{
	std::map<NSSTRING, std::map<NSSTRING,CNSAttributeRef*>*>::iterator BlkItr;
	std::map<NSSTRING, CNSAttributeRef*>::iterator AttItr;
	std::map<NSSTRING, CNSAttributeRef*>* pAttMap = NULL;
	for(BlkItr = attrMap.begin(); BlkItr != attrMap.end(); BlkItr++)
	{
		pAttMap = (*BlkItr).second;
		for(AttItr = pAttMap->begin(); AttItr != pAttMap->end(); AttItr++)
		{
			delete (*AttItr).second;
		}
		pAttMap->clear();
		delete pAttMap;
	}
	attrMap.clear();
}
