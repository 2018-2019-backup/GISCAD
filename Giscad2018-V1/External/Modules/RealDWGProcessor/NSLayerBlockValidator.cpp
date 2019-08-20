#include "StdAfx.h"
#include "math.h"
#include "..\Include\NSValidationDataCollector.h"
#include "NSLayerBlockValidator.h"
#include "NSLocationValidator.h"
#include "NSErrorCircle.h"
#include <atlstr.h>

CNSErrorCircle gErrorCircle;
//std::map<enum Err_MSG, int> mapofError;

std::map<LONG_PTR, LONG_PTR> gMapOfValidIDs;
extern double gTolerance;

//constructor
CNSLayerBlockValidator::CNSLayerBlockValidator(void)
{
	m_dAreaFactor = 1.0;
}

//destructor
CNSLayerBlockValidator::~CNSLayerBlockValidator(void)
{
	STRING_ID_TYPE::iterator IDTypeItr;
	IDCollector *pCollector = NULL;
	for(IDTypeItr = m_mapOfNewFeautres.begin(); IDTypeItr != m_mapOfNewFeautres.end(); IDTypeItr++)
	{
		pCollector = (*IDTypeItr).second;
		pCollector->nObjectIds.clear();
		delete pCollector;
		pCollector = NULL;
	}
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator dataItr;
	CNSValidationDataCollector* pValidCollector = NULL;
	for(dataItr = m_OfDataCollector.begin(); dataItr != m_OfDataCollector.end(); dataItr++)
	{
		pValidCollector = (*dataItr).second;
		delete pValidCollector;
		pValidCollector = NULL;
	}
	
}

void CNSLayerBlockValidator::setStdDWGObject(CNSDWGMgr* pStdDWGMgr)
{
	m_pStdDWGMgr = pStdDWGMgr;
}

void CNSLayerBlockValidator::setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr)
{
	m_pLocalDWGMgr = pLocalDWGMgr;
}

void CNSLayerBlockValidator::setSearchAreaFactor(double dSearchAreaFactor)
{
	m_dAreaFactor = dSearchAreaFactor;
}

/*!
 @Brief			validation of all drawing features
 @Param [in]	none
 @Return		always success
 @Date			23 Aug 2007
*/
int CNSLayerBlockValidator::validate(int &NewFeatCnt, int &ModFeatCnt, int &DelFeatCnt)
{

	//Validate existing blocks for layers and cadastrial movement
	validateExisitingFeatures();
	//Validate new blocks for layers
	validateNewFeatures(NewFeatCnt);
	//Validate mod blocks for layers for EXT and Special symbols for modified
	validateReticulationBlks(RET_MOD,ModFeatCnt);
	validateReticulationBlks(RET_DEL,DelFeatCnt);
	if((NewFeatCnt+ModFeatCnt+DelFeatCnt)<=0)
	{
		int nErrNumber = WriteErrorRec(LOG_CAD,ERR_CAD_18);
		MessageBox(NULL, _T("There are no new assets to validate"), _T("No New Features"),MB_OK);
		return NS_FAIL;
	}
	return NS_SUCCESS;
}

/*!
 @Brief			valdates all features on _EXT Layer
 @Param [in]	none
 @Return		always success
 @Date			23 Aug 2007
*/
int CNSLayerBlockValidator::validateExisitingFeatures()
{
	VALIDATE_COL_MAP_TYPE mapOfExtBlocks, mapOfLinearFeatures;
	VALIDATE_COL_MAP_TYPE::iterator mapOfRetItr; 
	STRING_ID_TYPE::iterator LayerItr;
	if(NS_SUCCESS != m_pLocalDWGMgr->getFeaturesOnLayer(mapOfExtBlocks, mapOfLinearFeatures, 0, _T("_EXT")))
		return NS_FAIL;
	IDCollector* pIDCollector = NULL;
	NSSTRING strLayerName, strDBLayerName;
	CNSValidationDataCollector *pCollector = NULL;
	CNSLocationValidator locValidator;
	int nLocRet, nLayerRet;
	for(mapOfRetItr = mapOfExtBlocks.begin(); mapOfRetItr != mapOfExtBlocks.end(); mapOfRetItr++)
	{
		pCollector = (*mapOfRetItr).second;	

		/// subir
		NSSTRING strBlockNameSuffix = _T("");
		NSSTRING strTempBlkName = pCollector->getBlockName();
		if(strTempBlkName.size() > 4)
			strBlockNameSuffix = strTempBlkName.substr((strTempBlkName.size()-4), 4);
		if (NSSTRCMP(strBlockNameSuffix.c_str(),_T("_EXT")) != 0) 
		{
			continue;
		}
		///
		locValidator.setType(LOC_POINT);
		locValidator.setObject(pCollector);
		nLocRet = locValidator.validate();
		if(nLocRet == NS_SUCCESS)
		{
			nLayerRet = checkBlkLayer(pCollector, strLayerName, RET_EXT);//Error already reported no need to take return code
			if(nLayerRet == NS_SUCCESS)
			{	
				if(NS_SUCCESS == validateFIDforEXTBlock(pCollector, _T("BLOCK")))
				{
					gMapOfValidIDs.insert(std::map<LONG_PTR, LONG_PTR>::value_type(pCollector->getObjectId(),pCollector->getFID()));
				}
			}
		}
	}
	for(mapOfRetItr = mapOfLinearFeatures.begin(); mapOfRetItr != mapOfLinearFeatures.end(); mapOfRetItr++)
	{
		pCollector = (*mapOfRetItr).second;
		locValidator.setType(LOC_OTHER);
		locValidator.setObject(pCollector);
		nLocRet = locValidator.validate();
		//if(nLocRet == NS_SUCCESS)  // to allow moved polyline to connect // Sub - RE: bond Failure   Date - Tue 6/29/2010 2:25 AM
		{
			nLayerRet = checkNonBlkLayer(pCollector, strLayerName, RET_EXT);//Error already reported no need to take return code
			if(nLayerRet == NS_SUCCESS)
			{
				if(NS_SUCCESS == validateFIDforEXTBlock(pCollector, _T("LINEAR")))
				{
					gMapOfValidIDs.insert(std::map<LONG_PTR, LONG_PTR>::value_type(pCollector->getObjectId(),pCollector->getFID()));
				}
			}
		}
	}

	clearCollection(mapOfExtBlocks);
	clearCollection(mapOfLinearFeatures);
	return NS_SUCCESS;
}

/*!
 @Brief			valdates all features on _NEW or _NEW_XXX Layer
 @Param [in]	none
 @Return		always success
 @Date			23 Aug 2007
*/
int CNSLayerBlockValidator::validateNewFeatures(int &NewFeaturesCount)
{
	VALIDATE_COL_MAP_TYPE mapOfBlocks, mapOfLinearFeatures;
	VALIDATE_COL_MAP_TYPE::iterator mapOfRetItr; 
	STRING_ID_TYPE::iterator LayerItr;

	///////// To report error to all those blocks whose name end with "_NEW" but they don't lay on layer whose name ends with "_NEW" ( IF NOT CONFIGURED )
	// Date - 7.7.9 By Subir
	//std::vector<CNSFeatureContainer*> vectNewContainer;
	//m_pLocalDWGMgr->selectBlocks(vectNewContainer, _T(""), _T("_NEW"));
	VALIDATE_COL_MAP_TYPE mapBlocksOnWrongLayer;
	CNSValidationDataCollector *pCollector0 = NULL;
	m_pLocalDWGMgr->getBlocksOnNonMatchingLayer(mapBlocksOnWrongLayer, _T("_NEW"));
	for(mapOfRetItr = mapBlocksOnWrongLayer.begin(); mapOfRetItr != mapBlocksOnWrongLayer.end(); mapOfRetItr++)
	{
		pCollector0 = (*mapOfRetItr).second;
		NSSTRING strDBLayerName, strLayerName;

		RET_TYPE enType = RET_NEW;

		getLayerName(pCollector0, strDBLayerName, enType , enType);
		strLayerName = pCollector0->getLayerName();		

				//layer is mod or del then skip

		        NSSTRING strLayerNameSuffix = _T("");
				if(strLayerName.size() > 4)
				  strLayerNameSuffix = strLayerName.substr((strLayerName.size()-4), 4);
								
				if( strLayerNameSuffix == _T("_MOD") || strLayerNameSuffix == _T("_DEL") )
				{
					continue; // already handeled in other place 
				}

		if(NS_SUCCESS == checkBlkLayer(pCollector0, strLayerName, RET_NEW)) //Check layer of new feature with database layer
		{
			continue; // purposely configured 
		}
		//int nErrNumber = WriteErrorRecEx(LOG_GIS,ERR_GIS_7,pCollector0,_T("SL$"),strDBLayerName.c_str(),_T("L$"),strLayerName.c_str());

		//CNSPoint3D pt = pCollector0->getInsertionPoint();
		//gErrorCircle.drawCircle(nErrNumber, LOG_GIS, pt.m_dXX, pt.m_dYY, pt.m_dZZ);

	}
	///////// To report error to all those blocks whose name end with "_NEW" but they don't lay on layer whose name ends with "_NEW" ( IF NOT CONFIGURED )


	//Retrive new features
	if(NS_SUCCESS != m_pLocalDWGMgr->getFeaturesOnLayer(mapOfBlocks, mapOfLinearFeatures, 0, _T("_NEW")))
		return NS_FAIL;
	IDCollector* pIDCollector = NULL;
	NSSTRING strLayerName, strDBLayerName;
	CNSValidationDataCollector *pCollector = NULL;
	for(mapOfRetItr = mapOfBlocks.begin(); mapOfRetItr != mapOfBlocks.end(); mapOfRetItr++)
	{		
		pCollector = (*mapOfRetItr).second;

		if(NS_SUCCESS == checkBlkLayer(pCollector, strLayerName, RET_NEW)) //Check layer of new feature with database layer
		{
			#ifndef _INTERNAL_
				NSSTRUPR((TCHAR*)strLayerName.c_str(), strLayerName.size()+1);
			#else
			    NSSTRUPR((TCHAR*)strLayerName.c_str(), strLayerName.size()+1);
				/*NSSTRUPR((TCHAR*)strLayerName.c_str());*/
			#endif		
			validateArrtibutes(pCollector);
			LayerItr = m_mapOfNewFeautres.find(strLayerName);
			if(LayerItr != m_mapOfNewFeautres.end())
			{
				pIDCollector = (*LayerItr).second; 
			}
			else
			{
				pIDCollector = new IDCollector;
				m_mapOfNewFeautres.insert(STRING_ID_TYPE::value_type(strLayerName, pIDCollector));					
			}	
			CNSValidationDataCollector *pNewCollector = new CNSValidationDataCollector;
			pNewCollector = pCollector; 
			pIDCollector->nObjectIds.push_back((*mapOfRetItr).first);
			m_mapOfNewFeautres.insert(STRING_ID_TYPE::value_type(strLayerName, pIDCollector)); NewFeaturesCount++;	
			m_OfDataCollector.insert(std::map<LONG_PTR, CNSValidationDataCollector*>::value_type((*mapOfRetItr).first,pNewCollector));
			gMapOfValidIDs.insert(std::map<LONG_PTR, LONG_PTR>::value_type(pCollector->getObjectId(),pCollector->getFID()));
		}
		else
		{
			checkGISPrompt(pCollector);
		}
	}

	NSSTRING newLayerName;
	TCHAR szQuery[MAX_PATH+1];
	int nDBRet = -1;
	for(mapOfRetItr = mapOfLinearFeatures.begin(); mapOfRetItr != mapOfLinearFeatures.end(); mapOfRetItr++)
	{
		pCollector = (*mapOfRetItr).second;
		NSSPRINTF(szQuery, _T("Select FNO from NewLinearFeatures where NewLayer = \'%S\'"),
			pCollector->getLayerName().c_str());
		
		nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
		if(-1 != nDBRet)
		{
			variant_t vFNO;
			bool bFNO = CNSDatabaseMgr::getInstance()->getValue(_T("FNO"), vFNO);
			if(bFNO)
			{
				gMapOfValidIDs.insert(std::map<LONG_PTR, LONG_PTR>::value_type(pCollector->getObjectId(),pCollector->getFID()));
				NewFeaturesCount++;
			}
			else
			{
				int nErrNumber;
				TCHAR szFormat[_MAX_PATH + 1];
				CNSPoint3D pt =  pCollector->getInsertionPoint();
				//NSSPRINTF(szFormat,_T("Invalid geometry found on Layer =\'%s\'. Verify \'NewLinearFeatures\' table"),  pCollector->getLayerName().c_str());
				//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_6);

				nErrNumber = WriteErrorRec(LOG_GIS,ERR_GIS_6,pCollector);
				
				gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_6, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
			}
		}
	}

	return NS_SUCCESS;
}

/*!
 @Brief			release meory associated with collector object
 @Param [in]	none
 @Return		always success
 @Date			23 Aug 2007
*/
void CNSLayerBlockValidator::clearCollection(VALIDATE_COL_MAP_TYPE &mapOfCollection)
{
	VALIDATE_COL_MAP_TYPE::iterator mapOfRetItr; 
	CNSValidationDataCollector *pCollector = NULL;
	for(mapOfRetItr = mapOfCollection.begin(); mapOfRetItr != mapOfCollection.end(); mapOfRetItr++)
	{
		pCollector = (*mapOfRetItr).second;
		delete pCollector;
		pCollector = NULL;
	}
}

/*!
 @Brief			find paritcular entry in map of XData
 @Param [in]	none
 @Return		always success
 @Date			27 Aug 2007
*/
int CNSLayerBlockValidator::getValue(STRING_MAP* mapOfXData, NSSTRING strType, NSSTRING &strValue)
{
	STRING_MAP::iterator TempItr;
	TempItr = mapOfXData->find(strType);
	if(TempItr == mapOfXData->end())
	{
		return NS_FAIL;
	}
	strValue = (*TempItr).second;
	return NS_SUCCESS;
}

/*!
 @Brief			validate _MOD and _DEL feaures
 @Param [in]	block type
 @Return		return success in case of successful validation else false
 @Date			23 Aug 2007
*/
int CNSLayerBlockValidator::validateReticulationBlks(RET_TYPE enType, int &FeatCnt)
{
	VALIDATE_COL_MAP_TYPE mapOfBlocks, mapOfLinearFeatures;
	//Get extension from the type
	NSSTRING strExt = getExtFromType(enType);
	//Retrive feautes from corresponding layer
	if(NS_SUCCESS != m_pLocalDWGMgr->getFeaturesOnLayer(mapOfBlocks, mapOfLinearFeatures, 0, (TCHAR*)strExt.c_str()))
		return NS_FAIL;

	VALIDATE_COL_MAP_TYPE::iterator mapOfRetItr;
	STRING_ID_TYPE mapOfNameIDs; 
	STRING_ID_TYPE::iterator mapOfNameIDItr;
	CNSValidationDataCollector* pCollector = NULL;
	IDCollector* pIDCollector = NULL;
	NSSTRING strLayerName;
	//Create a map of Layername Vs ObjectID collection so thatd layers will begrouped under layername
	for(mapOfRetItr = mapOfBlocks.begin(); mapOfRetItr != mapOfBlocks.end(); mapOfRetItr++)
	{
		pCollector = (*mapOfRetItr).second;
		strLayerName = pCollector->getLayerName();
		#ifndef _INTERNAL_
			NSSTRUPR((TCHAR*)strLayerName.c_str(), strLayerName.size()+1);
		#else
		    NSSTRUPR((TCHAR*)strLayerName.c_str(), strLayerName.size()+1);
			//NSSTRUPR((TCHAR*)strLayerName.c_str());
		#endif	
	
			CString strtocmp = _T("_USER");
		CString tempStrLayerName = strLayerName.substr(0,5).c_str();


		//if(strLayerName.substr(0,5) != strtocmp.substr(0,5))
		if(!tempStrLayerName.CompareNoCase(strtocmp))
		{
			continue;
		}
//NSSTRING strtocmp = _T("_User");
////if(NSSTRCMP((TCHAR*)strLayerName.substr(0,5).c_str(),(TCHAR*)strtocmp.substr(0,5).c_str()) ==0)
//if(strLayerName.substr(0,5) == strtocmp.substr(0,5))
//{
//continue;
//}

		mapOfNameIDItr = mapOfNameIDs.find(strLayerName);
		if(mapOfNameIDItr != mapOfNameIDs.end()) //Layer entry in map is already there
		{ 
			pIDCollector = (*mapOfNameIDItr).second; //Add to found entry
		}
		else //Need to create Layer entry
		{
			pIDCollector = new IDCollector; //Add to new entry
			mapOfNameIDs.insert(STRING_ID_TYPE::value_type(strLayerName, pIDCollector));
			FeatCnt++;
		}	
		pIDCollector->nObjectIds.push_back((*mapOfRetItr).first);
	}

	//Validates these segregated entries
	for(mapOfNameIDItr = mapOfNameIDs.begin(); mapOfNameIDItr != mapOfNameIDs.end(); mapOfNameIDItr++)
	{
		validateBlkLayers((*mapOfNameIDItr).first, (*mapOfNameIDItr).second, mapOfBlocks, enType); 
	}
	
//	if(enType == RET_DEL)
	{
		for(mapOfRetItr = mapOfLinearFeatures.begin(); mapOfRetItr != mapOfLinearFeatures.end(); mapOfRetItr++)
		{
			pCollector = (*mapOfRetItr).second;
			if(NS_SUCCESS == validateFIDforEXTBlock(pCollector, _T("LINEAR")))
			{
				FeatCnt++;
				if(enType == RET_DEL)
					checkNonBlkLayer(pCollector, strLayerName, RET_DEL);//Error already reported no need to take return code
			}
		}
	}

	clearCollection(mapOfBlocks);
	clearCollection(mapOfLinearFeatures);

	return NS_SUCCESS;
}

/*!
 @Brief			validate block against layers
 @Param [in]	LayerName, type of blocks
 @Return		return success in case of successful validation else false
 @Date			23 Aug 2007
*/
int CNSLayerBlockValidator::validateBlkLayers(NSSTRING strLayerName, IDCollector *pIDCollector, VALIDATE_COL_MAP_TYPE &mapOfBlocks, RET_TYPE enType)
{
	VALIDATE_COL_MAP_TYPE::iterator mapOfRetItr;
	IDCollector extCollector, nonExtCollector;
	std::vector<LONG_PTR>::iterator CollectItr, NonExtItr;
	NSSTRING strBlockName;
	//Separate the Objects based on whether it is existing or nonexisting
	for(CollectItr = pIDCollector->nObjectIds.begin(); CollectItr != pIDCollector->nObjectIds.end(); CollectItr++)
	{
		mapOfRetItr = mapOfBlocks.find(*CollectItr);
		if(mapOfRetItr != mapOfBlocks.end())
		{
			strBlockName = ((*mapOfRetItr).second)->getBlockName();
			if(RET_EXT == getTypeFromBlkName(strBlockName)) // Of type "_EXT"
			{
				extCollector.nObjectIds.push_back(*CollectItr);
			}
			else
			{
				nonExtCollector.nObjectIds.push_back(*CollectItr);
			}
		}
	}

	CNSValidationDataCollector* pCollector = NULL;
	NSSTRING strExtLayer,strDelLayer, strBlk, strLayer;
	STRING_MAP mapOfLayers;
	std::map<LONG_PTR, LONG_PTR> mapOfRelIDs;
	std::vector<CNSValidationDataCollector*> vectValidExt;
	LONG_PTR nObjID = -1;
	//itrate the exisitng Objects collection to check they are on the proper layer
	for(CollectItr = extCollector.nObjectIds.begin(); CollectItr != extCollector.nObjectIds.end(); CollectItr++)
	{
		mapOfRetItr = mapOfBlocks.find(*CollectItr); //Retrive the information of this existing object
		if(mapOfRetItr != mapOfBlocks.end())  
		{
			pCollector = (*mapOfRetItr).second; 
			CNSPoint3D pt = pCollector->getInsertionPoint();
			if(NS_SUCCESS == getLayerName(pCollector, strLayer, enType, RET_EXT)) //For this existing object get the layer name of type enType
			{
				//Layer name not matched
				if(NSSTRCMP(strLayer.c_str(), pCollector->getLayerName().c_str()) != 0)
				{
					int nErrNumber;
					TCHAR szFormat[_MAX_PATH + 1];
					//NSSPRINTF(szFormat,_T("Block \'%s\' found on invalid layer, please verify attributes. IE Standard Layer =\'%s\', Current Layer = \'%s\'"),  pCollector->getBlockName().c_str(), strLayer.c_str(), pCollector->getLayerName().c_str());
                    //WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_7);

					nErrNumber = WriteErrorRecEx(LOG_GIS,ERR_GIS_7,pCollector,_T("SL$"),strLayer.c_str()/*,_T("L$"),strLayerName.c_str()*/);
					
					gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_7, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
					checkGISPrompt(pCollector);
				}
				else
				{
					//This ID is valid add as avalid entry
				//	if(NS_SUCCESS == validateArrtibutes(pCollector))
				//	{
						validateArrtibutes(pCollector);
						vectValidExt.push_back(pCollector);
						if(NS_SUCCESS == validateFIDforEXTBlock(pCollector, _T("BLOCK")))
						{
							gMapOfValidIDs.insert(ID_TYPE::value_type(pCollector->getObjectId(),pCollector->getObjectId()));
						}
				//	}
				}				
			}
			else //Layername not found
			{
				    int nErrNumber;
					TCHAR szFormat[_MAX_PATH + 1];
					//NSSPRINTF(szFormat,_T("Block \'%s\' found on invalid layer, please verify attributes. IE Standard Layer =\'%s\', Current Layer = \'%s\'"), pCollector->getBlockName().c_str() , strLayer.c_str(), pCollector->getLayerName().c_str());
                    //WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_7);

					nErrNumber = WriteErrorRecEx(LOG_GIS,ERR_GIS_7,pCollector,_T("SL$"),strLayer.c_str()/*,_T("L$"),strLayerName.c_str()*/);
					
					gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_7, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
					checkGISPrompt(pCollector);
			}
		}
	}

	//Validate this existing collection with NonExisitng for match
	checkRelation(vectValidExt, nonExtCollector, mapOfBlocks, nObjID, enType);
	if(enType == RET_DEL)
	{
		checkRelation(vectValidExt, nonExtCollector, mapOfBlocks, nObjID, RET_REP);
	}

	std::map<LONG_PTR, LONG_PTR>::iterator RelIDsItr;
	for(CollectItr = nonExtCollector.nObjectIds.begin(); CollectItr != nonExtCollector.nObjectIds.end(); CollectItr++)
	{
		RelIDsItr = gMapOfValidIDs.find(*CollectItr);
		if(RelIDsItr == gMapOfValidIDs.end())
		{
			mapOfRetItr = mapOfBlocks.find(*CollectItr);
			if(mapOfRetItr != mapOfBlocks.end())
			{
				int nErrNumber;
				pCollector = (*mapOfRetItr).second; 
				CNSPoint3D pt = pCollector->getInsertionPoint();
				RET_TYPE enBlkType = getTypeFromBlkName(pCollector->getBlockName());
				TCHAR szFormat[_MAX_PATH + 1];
				if(enBlkType == RET_DEL || enBlkType == RET_MOD)
				{
					//NSSPRINTF(szFormat,_T("Existing block not found for block \'%s\'"), pCollector->getBlockName().c_str());
					//WriteIDToLog(LOG_GIS, (TCHAR*)strNotes.c_str(), ERR_GIS_9);
				    
					nErrNumber = WriteErrorRec(LOG_GIS,ERR_GIS_9, pCollector);
					gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_9, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
				}
				else if(enBlkType == RET_REP)
				{
					NSSTRING strNotes = pCollector->getNotesAppended(szFormat);

					//NSSPRINTF(szFormat,_T("Replaced block \'%s\'should matching new and deleted block"), pCollector->getBlockName().c_str());
					//WriteIDToLog(LOG_GIS, (TCHAR*)strNotes.c_str(), ERR_GIS_10);
				    
					nErrNumber = WriteErrorRec(LOG_GIS,ERR_GIS_10,pCollector );
					gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_10, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
				}
				else
				{
					NSSTRING strNotes = pCollector->getNotesAppended(szFormat);

					//NSSPRINTF(szFormat,_T("Block \'%s\' found on invalid layer \'%s\'. Verify \'BlockFeatures\' table from database"), pCollector->getBlockName().c_str(), pCollector->getLayerName().c_str());
					//WriteIDToLog(LOG_GIS, (TCHAR*)strNotes.c_str(), ERR_GIS_8);
				    
					nErrNumber = WriteErrorRec(LOG_GIS,ERR_GIS_8, pCollector);
					gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_8, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
				}

				

				
				
			}		
		}
	}

	return NS_SUCCESS;
}

/*!
 @Brief			get extension from block type 
 @Param [in]	block type
 @Return		extension string
 @Date			23 Aug 2007
*/
NSSTRING CNSLayerBlockValidator::getExtFromType(RET_TYPE enType)
{
	NSSTRING strNameType;
	switch(enType)
	{
	case RET_EXT: strNameType = _T("_EXT"); break;
	case RET_DEL: strNameType = _T("_DEL"); break;
	case RET_MOD: strNameType = _T("_MOD"); break;
	case RET_REP: strNameType = _T("_REP"); break;
	}
	return strNameType;
}

/*!
 @Brief			get block type from extension string
 @Param [in]	extension string
 @Return		block type
 @Date			23 Aug 2007
*/
int CNSLayerBlockValidator::getBlkNameFromType(NSSTRING strExtName, RET_TYPE enType, NSSTRING &strNewName)
{
	NSSTRING strNameType = getExtFromType(enType);
	if(strExtName.length() < strNameType.length()) return NS_FAIL;
	strNewName = strExtName.substr(0, strExtName.size()- strNameType.length());
	strNewName += strNameType;
	return NS_SUCCESS;
}

/*!
 @Brief			get block type from extension string
 @Param [in]	extension string
 @Return		block type
 @Date			23 Aug 2007
*/
RET_TYPE CNSLayerBlockValidator::getTypeFromBlkName(NSSTRING strExtName)
{
	RET_TYPE enType = RET_INVALID;
	if(strExtName.length() < 4) return RET_INVALID;
	NSSTRING strTemp  = strExtName.substr(strExtName.size()- 4, strExtName.size());
	if(NSSTRCMP(strTemp.c_str(), _T("_EXT")) == 0)
	{
		enType = RET_EXT;
	}
	else if(NSSTRCMP(strTemp.c_str(), _T("_NEW")) == 0)
	{
		enType = RET_NEW;
	}
	else if(NSSTRCMP(strTemp.c_str(), _T("_DEL")) == 0)
	{
		enType = RET_DEL;
	}
	else if(NSSTRCMP(strTemp.c_str(), _T("_MOD")) == 0)
	{
		enType = RET_MOD;
	}
	else if(NSSTRCMP(strTemp.c_str(), _T("_REP")) == 0)
	{
		enType = RET_REP;
	}

	return enType;
}

/*!
 @Brief			get layername for existing features 
 @Param [in]	Collector data, Layertype and blocktype
 @Param [out]	layername
 @Return		block type
 @Date			21 Aug 2007
*/
int CNSLayerBlockValidator::getLayerName(CNSValidationDataCollector* pCollector , NSSTRING &strLayerName, RET_TYPE enLayerType, RET_TYPE enBlockType)
{
	NSSTRING strUsage, strOrientation, strState, usage, orientation, strFNO;
	if(enBlockType == RET_EXT)
	{
		getValue(&pCollector->m_mapOfRelationsAndConnectivity, _T("Usage"), strUsage);
		getValue(&pCollector->m_mapOfRelationsAndConnectivity, _T("Orientation"), strOrientation);
		if(NS_SUCCESS == getValue(&pCollector->m_mapOfRelationsAndConnectivity, _T("FNO"), strFNO))
		{
			pCollector->setFNO(NSATOI(strFNO.c_str()));
		}
	}
	else 
	{
		getValue(&pCollector->m_mapAttributes, _T("USAGE"), strUsage);
		getValue(&pCollector->m_mapAttributes, _T("ORIENTATION"), strOrientation);
	}
	NSSTRING strBlkName = pCollector->getBlockName();
	NSSTRING strBlkLayerName = pCollector->getLayerName();

	usage = getQueryString(strUsage);
	orientation = getQueryString(strOrientation);

	NSSTRING strLayerType, strBlkType;
	switch(enLayerType)
	{
	case RET_EXT: strLayerType = _T("ExtLayer"); break;
	case RET_DEL: strLayerType = _T("DelLayer"); break;
	case RET_MOD: strLayerType = _T("ModLayer"); break;
	case RET_NEW: strLayerType = _T("NewLayer"); break;
	}
	TCHAR szQuery[_MAX_PATH + 1];
	int nDBRet = 0;
	if(enBlockType == RET_EXT)
	{
		NSSPRINTF(szQuery, _T("Select %S from BlockFeatures where FNO = %d and USAGE %S and ORIENTATION %S"),
			strLayerType.c_str(), pCollector->getFNO(), usage.c_str(), orientation.c_str());
		NSSTRING szFinalQuery = getQueryString(pCollector->m_mapAttributes, szQuery);
		nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szFinalQuery.c_str());
	}
	else
	{
		NSSPRINTF(szQuery, _T("Select FNO , %S from BlockFeatures where NewBlock = \'%S\' and USAGE %S and ORIENTATION %S"),
		strLayerType.c_str(), strBlkName.c_str(), usage.c_str(), orientation.c_str());	
		NSSTRING szFinalQuery = getQueryString(pCollector->m_mapAttributes, szQuery);
		nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szFinalQuery.c_str());
	}
	//MessageBox(NULL, szQuery, _T("Test"), MB_OK );
	if(-1 != nDBRet)
	{
		variant_t vBlkName;
		bool bFNO = CNSDatabaseMgr::getInstance()->getValue(strLayerType.c_str(), vBlkName);
		if(bFNO)
		{
			#ifndef _INTERNAL_
				strLayerName = vBlkName.bstrVal;
			#else
				strLayerName = vBlkName.bstrVal;
				/*char* strLayer = _com_util::ConvertBSTRToString(vBlkName.bstrVal);
				strLayerName = strLayer;*/
			#endif
		}
		if(enBlockType != RET_EXT)
		{
			variant_t vFNO;
			bFNO = CNSDatabaseMgr::getInstance()->getValue(_T("FNO"), vFNO);
			if(bFNO)
			{
				pCollector->setFNO(vFNO);
				return NS_SUCCESS;
			}
		}
		else
			return NS_SUCCESS;
	}
	return NS_FAIL;

	
}

/*!
 @Brief			format querystring for database search for new features
 @Param [in]	atribute map
 @Return		query string
 @Date			21 Aug 2007
*/
NSSTRING CNSLayerBlockValidator::getQueryString(STRING_MAP & mapAttributes, TCHAR szQuery[_MAX_PATH])
{
	NSSTRING strQuery(szQuery);
	std::map<NSSTRING, NSSTRING> mapofColumns;
	std::map<NSSTRING, NSSTRING>::iterator mapItr;
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("SUBSTATION_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("SWITCH_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CCP_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("LANTERN_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ACTUAL_STATUS"), _T("")));

	STRING_MAP::iterator itr;
	for(itr = mapAttributes.begin(); itr != mapAttributes.end(); itr++)
	{
		NSSTRING strAttName = (*itr).first;
		#ifndef _INTERNAL_
			NSSTRUPR((TCHAR*)strAttName.c_str(), strAttName.size()+1);
		#else
		    NSSTRUPR((TCHAR*)strAttName.c_str(), strAttName.size()+1);
			//NSSTRUPR((TCHAR*)strAttName.c_str());
		#endif	
	
		mapItr = mapofColumns.find(strAttName);
		if(mapItr != mapofColumns.end())
		{
			NSSTRING strAttValue = getQueryString((*itr).second);
			TCHAR szTemp[_MAX_PATH + 1];
			NSSPRINTF(szTemp, _T(" and %s %s"), strAttName.c_str(), strAttValue.c_str());
			strQuery.append(szTemp);
		}

	}
	return strQuery;
}

/*!
 @Brief			get layername for new linear features
 @Param [in]	atribute map
 @Return		query string
 @Date			21 Aug 2007
*/
int CNSLayerBlockValidator::getLayerNameFromLineType(CNSValidationDataCollector* pCollector , NSSTRING &strLayerName, RET_TYPE enLayerType, RET_TYPE enBlockType)
{
	NSSTRING strUsage, strOrientation, strState, usage, orientation, strFNO;
	if(enBlockType == RET_EXT)
	{
		getValue(&pCollector->m_mapOfRelationsAndConnectivity, _T("Usage"), strUsage);
		getValue(&pCollector->m_mapOfRelationsAndConnectivity, _T("Orientation"), strOrientation);
		if(NS_SUCCESS == getValue(&pCollector->m_mapOfRelationsAndConnectivity, _T("FNO"), strFNO))
		{
			pCollector->setFNO(NSATOI(strFNO.c_str()));
		}
	}

	NSSTRING strLineType = pCollector->getLineType();
	NSSTRING strLineLayerName = pCollector->getLayerName();

	usage = getQueryString(strUsage);
	orientation = getQueryString(strOrientation);

	NSSTRING strLayerType, strLnType;
	switch(enLayerType)
	{
	case RET_EXT: strLayerType = _T("ExtLayer"); break;
	case RET_DEL: strLayerType = _T("DelLayer"); break;
	case RET_MOD: strLayerType = _T("ModLayer"); break;
	case RET_NEW: strLayerType = _T("NewLayer"); break;
	}
//	if(enBlockType == RET_EXT) strLnType = _T("ExtLineType");
//	else strLnType = _T("NewLineType");

	TCHAR szQuery[MAX_PATH+1];
	NSSPRINTF(szQuery, _T("Select %S from LinearFeatures where FNO = %d and USAGE %S and ORIENTATION %S"),
		strLayerType.c_str(), pCollector->getFNO(), usage.c_str(), orientation.c_str());
	
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	if(-1 != nDBRet)
	{
		variant_t vFNO;
		bool bFNO = CNSDatabaseMgr::getInstance()->getValue(strLayerType.c_str(), vFNO);
		if(bFNO)
		{
			#ifndef _INTERNAL_
				strLayerName = vFNO.bstrVal;
			#else
				strLayerName = vFNO.bstrVal;
				/*char* strLayer = _com_util::ConvertBSTRToString(vFNO.bstrVal);
				strLayerName = strLayer;*/
			#endif
			return NS_SUCCESS;
		}
	}
	return NS_FAIL;
}

/*!
 @Brief			feature checking/overlapping for presence of new and delete entry
 @Param [in]	collection object, non ext items, block ids, layer type
 @Param [out]	objectID
 @Return		success/fail
 @Date			21 Aug 2007
*/
int CNSLayerBlockValidator::checkRelation(std::vector<CNSValidationDataCollector*> &vectExtCollector , IDCollector &nonExtCollector, VALIDATE_COL_MAP_TYPE &mapOfBlocks, LONG_PTR &nObjID, RET_TYPE enLayerType)
{
	VALIDATE_COL_MAP_TYPE::iterator mapOfRetItr;
	ID_TYPE::iterator IDItr;
	std::vector<CNSValidationDataCollector*>::iterator VaildExtItr;
	std::vector<LONG_PTR>::iterator NonExtItr;
	CNSPoint3D pt, temp;
	CNSValidationDataCollector* pCollector, *pExtCollector = NULL;
	NSSTRING strBlkName;

	//itrate nonexisting collection
	for(NonExtItr = nonExtCollector.nObjectIds.begin(); NonExtItr != nonExtCollector.nObjectIds.end(); NonExtItr++)
	{
		mapOfRetItr = mapOfBlocks.find(*NonExtItr);
		if(mapOfRetItr != mapOfBlocks.end())
		{
			pCollector = (*mapOfRetItr).second; 
			//iterate through all the exsiting entries for this non-ext entry
			for(VaildExtItr = vectExtCollector.begin(); VaildExtItr != vectExtCollector.end(); VaildExtItr++)
			{
				pExtCollector = *VaildExtItr;
				IDItr = m_mapOfPairs.find(pExtCollector->getObjectId());
				if(IDItr != m_mapOfPairs.end()) //This ID is already paired, skip this
					continue;
				if(NS_SUCCESS == getBlkNameFromType(pExtCollector->getBlockName(), enLayerType, strBlkName))
				{
					//check if the non-exting block name match with the existing type
					if(NSSTRCMP(strBlkName.c_str(), pCollector->getBlockName().c_str()) == 0)
					{
						//check for the insertion points match
						if(NS_SUCCESS == isPointOverlapped(pExtCollector->getInsertionPoint(), pCollector->getInsertionPoint()))
						{
							//Relaced block needs special handling for NEW as well as deleted entry
							if(enLayerType == RET_REP)
							{
								if(NS_SUCCESS == checkReplacedWithNew(pExtCollector, mapOfBlocks))//Check for validity
								{
									nObjID = *NonExtItr;
									m_mapOfPairs.insert(ID_TYPE::value_type(pExtCollector->getObjectId(),nObjID));
									gMapOfValidIDs.insert(ID_TYPE::value_type(nObjID,nObjID));
								}
							}
							else
							{
								nObjID = *NonExtItr;
								m_mapOfPairs.insert(ID_TYPE::value_type(pExtCollector->getObjectId(),nObjID));
								gMapOfValidIDs.insert(ID_TYPE::value_type(nObjID,nObjID));
							}
						}
					}
				}
			}
		}
	}
	return NS_FAIL;
}

/*!
 @Brief			replaced feature checking for presence of new and delete entry
 @Param [in]	collection object, block ids
 @Param [out]	objectID
 @Return		success/fail
 @Date			21 Aug 2007
*/
int CNSLayerBlockValidator::checkReplacedWithNew(CNSValidationDataCollector* pExtCollector,  VALIDATE_COL_MAP_TYPE &mapOfBlocks)
{
	VALIDATE_COL_MAP_TYPE::iterator mapOfRetItr; 
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator DataItr;
	STRING_ID_TYPE::iterator TypeItr;
	IDCollector *pIDCollector = NULL;
	std::vector<LONG_PTR>::iterator NewItr;
	LONG_PTR nObjID;
	NSSTRING strLayerName;
	CNSPoint3D pt, temp;
	CNSValidationDataCollector* pCollector = NULL;
	getLayerName(pExtCollector , strLayerName, RET_NEW, RET_EXT);
	#ifndef _INTERNAL_
		NSSTRUPR((TCHAR*)strLayerName.c_str(), strLayerName.size()+1);
	#else
	    NSSTRUPR((TCHAR*)strLayerName.c_str(), strLayerName.size()+1);
		//NSSTRUPR((TCHAR*)strLayerName.c_str());
	#endif		
	TypeItr = m_mapOfNewFeautres.find(strLayerName);
	if(TypeItr != m_mapOfNewFeautres.end())
	{
		pIDCollector = (*TypeItr).second; 
		
		NSSTRING strBlkName;
		for(NewItr = pIDCollector->nObjectIds.begin(); NewItr != pIDCollector->nObjectIds.end(); NewItr++)
		{
			DataItr = m_OfDataCollector.find(*NewItr);
			if(DataItr != m_OfDataCollector.end())
			{
				pCollector = (*DataItr).second; 
				//check for the insertion points match
				if(NS_SUCCESS == isPointOverlapped(pExtCollector->getInsertionPoint(), pCollector->getInsertionPoint()))
				{
					nObjID = *NewItr;
					m_mapOfPairs.insert(ID_TYPE::value_type(pExtCollector->getObjectId(),nObjID));
					gMapOfValidIDs.insert(ID_TYPE::value_type(nObjID,nObjID));
					return NS_SUCCESS;
				}
				else
					return NS_FAIL;
			}
		}	
	}
	else
	{
		return NS_FAIL;		
	}	
	return NS_SUCCESS;
}

/*!
 @Brief			check if the block is on proper layer as per database
 @Param [in]	collection object, block type
 @Param [out]	layername
 @Return		success/fail
 @Date			21 Aug 2007
*/
int CNSLayerBlockValidator::checkBlkLayer(CNSValidationDataCollector *pCollector, NSSTRING &strLayerName, RET_TYPE enType)
{
	if(pCollector == NULL) return NS_FAIL;
	
	NSSTRING strDBLayerName, strBlkName;
	strLayerName = pCollector->getLayerName();
	strBlkName = pCollector->getBlockName();
	//Retrive the database layername for the type specified using properties of block
	getLayerName(pCollector, strDBLayerName, enType , enType);

	if(NSSTRCMP(strDBLayerName.c_str(), strLayerName.c_str()) != 0)//DBLayer name and current layername conflicts
	{
		if(!CheckForAdditionalAllowedLayers(pCollector,enType,strLayerName))
		{
			int nErrNumber;
			TCHAR szFormat[_MAX_PATH + 1];

			NSSTRING strNotes = pCollector->getNotesAppended(szFormat);
			CNSPoint3D pt = pCollector->getInsertionPoint();
				
			CString strtocmp = _T("_USER");
		CString tempStrLayerName = strLayerName.substr(0,5).c_str();

		
		//if(strLayerName.substr(0,5) != strtocmp.substr(0,5))
	
	/*		NSSTRING strtocmp = _T("_User");*/
				/*if(strLayerName.substr(0,5) != strtocmp.substr(0,5))*/
		if(tempStrLayerName.CompareNoCase(strtocmp))
			{
				if(isExtPole(enType,pCollector))
				{
					nErrNumber = WriteErrorRecEx(LOG_GIS,ERR_GIS_37,pCollector,_T("SL$"),strDBLayerName.c_str(),_T("L$"),strLayerName.c_str());
					gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_37, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
				}
				else
				{
					nErrNumber = WriteErrorRecEx(LOG_GIS,ERR_GIS_7,pCollector,_T("SL$"),strDBLayerName.c_str(),_T("L$"),strLayerName.c_str());
					gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_7, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
				}
			return NS_FAIL;
			}
		}
	}
	return NS_SUCCESS;
}

/*!
 @Brief			check if the block is on any of the allowed layers as per database
 @Date			15 June 2009
*/
bool CNSLayerBlockValidator::CheckForAdditionalAllowedLayers(CNSValidationDataCollector *pCollector, RET_TYPE enType,NSSTRING &strLayerName)
{
	bool retval = false;

	NSSTRING strLayerType, strBlkType;
	switch(enType)
	{
	case RET_EXT: strLayerType = _T("ExtLayers"); break;
	case RET_DEL: strLayerType = _T("DelLayers"); break;
	case RET_MOD: strLayerType = _T("ModLayers"); break;
	case RET_NEW: strLayerType = _T("NewLayers"); break;
	}
	TCHAR szQuery[_MAX_PATH + 1];
	int nDBRet = 0;

		NSSPRINTF(szQuery, _T("Select %S from AdditionalAllowedLayers where FNO = \'%d\'"),strLayerType.c_str(), pCollector->getFNO());		
		nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);	
		

	if(-1 != nDBRet)
	{

		std::vector<_variant_t> vValues;
		CNSDatabaseMgr::getInstance()->getValue(strLayerType.c_str(), vValues);

		std::map<NSSTRING, NSSTRING>::iterator mapIter;
		NSSTRING strAttDwgName;
		NSSTRING strAttName;
		std::vector<_variant_t>::iterator resultIter;
		for(resultIter = vValues.begin(); resultIter != vValues.end(); resultIter++)
		{
			NSSTRING LayerName = (NSSTRING)((_bstr_t)(resultIter->bstrVal));	

			if(NSSTRCMP(LayerName.c_str(), strLayerName.c_str()) == 0)			
																		return true;			
		}


	}

	return false;	

}

bool CNSLayerBlockValidator::isExtPole(RET_TYPE enType , CNSValidationDataCollector *pCollector  )
{

	if(enType == RET_EXT)
	{
		////////////////////////////////////////////////

	    NSSTRING strUsage, strOrientation, strState, usage, orientation, strFNO;

		getValue(&pCollector->m_mapOfRelationsAndConnectivity, _T("Usage"), strUsage);
		getValue(&pCollector->m_mapOfRelationsAndConnectivity, _T("Orientation"), strOrientation);
		if(NS_SUCCESS == getValue(&pCollector->m_mapOfRelationsAndConnectivity, _T("FNO"), strFNO))
		{
			pCollector->setFNO(NSATOI(strFNO.c_str()));
		}
	
		
		NSSTRING strBlkName = pCollector->getBlockName();
		NSSTRING strBlkLayerName = pCollector->getLayerName();

		usage = getQueryString(strUsage);
		orientation = getQueryString(strOrientation);

		//NSSTRING strLayerType, strBlkType;	

		TCHAR szQuery[_MAX_PATH + 1];
		int nDBRet = 0;

		NSSPRINTF(szQuery, _T("Select FeatureName from BlockFeatures where FNO = %d and USAGE %S and ORIENTATION %Ss"), pCollector->getFNO(), usage.c_str(), orientation.c_str());
		NSSTRING szFinalQuery = getQueryString(pCollector->m_mapAttributes, szQuery);
		nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szFinalQuery.c_str());
	
		NSSTRING strFeatureName;

		//strLayerType = _T("ExtLayer");

		if(-1 != nDBRet)
		{
			variant_t varFieldValue;

					NSSTRING strFieldName = _T("FeatureName");
					bool bResult = CNSDatabaseMgr::getInstance()->getValue((TCHAR*)strFieldName.c_str(), varFieldValue);

					if(bResult)
					{
						if(varFieldValue.vt == VT_BSTR)
						{
							#ifndef _INTERNAL_
								strFeatureName = varFieldValue.bstrVal;
							#else
                                strFeatureName = (NSSTRING)varFieldValue.bstrVal ;
								/*strFeatureName = (NSSTRING)(_com_util::ConvertBSTRToString(varFieldValue.bstrVal));*/
							#endif
						}
					}

			
		}
		
		#ifndef _INTERNAL_
				NSSTRUPR((TCHAR*)strFeatureName.c_str(), strFeatureName.size()+1);
		#else
		        NSSTRUPR((TCHAR*)strFeatureName.c_str(), strFeatureName.size()+1);
				//NSSTRUPR((TCHAR*)strFeatureName.c_str());
		#endif

		if(strFeatureName == _T("POLE") || strFeatureName == _T("POLES") )
		{
			return true;
		}
		
	}

	return false;
}
/*!
 @Brief			check if the non block feature is on proper layer as per database
 @Param [in]	collection object, block type
 @Param [out]	layername
 @Return		success/fail
 @Date			21 Aug 2007
*/
int CNSLayerBlockValidator::checkNonBlkLayer(CNSValidationDataCollector *pCollector, NSSTRING &strLayerName, RET_TYPE enType)
{
	if(pCollector == NULL) return NS_FAIL;
	
	NSSTRING strDBLineType, strLineType;
	strLayerName = pCollector->getLayerName();
	strLineType = pCollector->getLineType();
	//Retrive the database layername for the type specified using properties of block
	getLayerNameFromLineType(pCollector, strDBLineType, enType , RET_EXT);

	if(NSSTRCMP(strDBLineType.c_str(), strLayerName.c_str()) != 0)//DBLayer name and current layername conflicts
	{
		//NSSTRING strtocmp = _T("_User");
		////if(NSSTRUPR(strLayerName.substr(0,5).c_str(),6) == strtocmp.substr(0,5))
		//if(strLayerName.substr(0,5) != strtocmp.substr(0,5))
			CString strtocmp = _T("_USER");
		CString tempStrLayerName = strLayerName.substr(0,5).c_str();
		if(tempStrLayerName.CompareNoCase(strtocmp))
		{
			TCHAR szFormat[_MAX_PATH + 1];
			if(strDBLineType.size() > 0)
			{
				int nErrNumber;
				CNSPoint3D pt = pCollector->getInsertionPoint();
				//NSSPRINTF(szFormat,_T("Feature with linetype '%s' found on invalid layer, please verify attributes. IE Standard Layer =\'%s\', Current Layer = \'%s\'"), 
				//strLineType.c_str(), strDBLineType.c_str(), strLayerName.c_str());
				//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_11);

				nErrNumber = WriteErrorRecEx(LOG_GIS,ERR_GIS_11,pCollector,_T("LT$") ,strLineType.c_str(), _T("SL$"),strDBLineType.c_str(), _T("L$"),strLayerName.c_str() ); 
				

				
				gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_11, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
			}
		
		//else
		//	NSSPRINTF(szFormat,_T("Feature '%s' is not on proper layer '%s'."), strLineType.c_str(), strLayerName.c_str());
		//int nErrNumber = WriteToLog(LOG_GIS, szFormat);
		return NS_FAIL;
		}
	}
	return NS_SUCCESS;
}

int CNSLayerBlockValidator::isPointOverlapped(CNSPoint3D firstPt, CNSPoint3D secondPt)
{
	if(fabs(firstPt.m_dXX - secondPt.m_dXX) < m_dAreaFactor && fabs(firstPt.m_dYY - secondPt.m_dYY) < m_dAreaFactor)
	{
		return NS_SUCCESS;
	}
	return NS_FAIL;
}

NSSTRING CNSLayerBlockValidator::getQueryString(NSSTRING strQueryString)
{
	NSSTRING strOutput;
	if(NSSTRCMP(strQueryString.c_str(), _T("")) == 0)
	{
		strOutput = _T(" is null");
	}
	else
	{
		strOutput = _T(" = \'") + strQueryString + _T("\'");
	}
	return strOutput;
}

/*!
 @Brief			checking the valuies of attribute against the allowed values 
				coulumn in BlockAttributes table if present
 @Param [in]	collection object
 @Return		success/fail
 @Date			21 Aug 2007
*/
int CNSLayerBlockValidator::validateArrtibutes(CNSValidationDataCollector *pCollector)
{
	checkGISPrompt(pCollector);
    STRING_MAP mapOfAllowedValues;
	STRING_MAP::iterator AllowedValuesItr, AttrItr;
	std::map<int, NSSTRING> mapOfAttNoName;
	TCHAR szQuery[_MAX_PATH + 1];
	NSSPRINTF(szQuery, _T("Select ANO from FeatureAttributes where FNO = %d"), pCollector->getFNO());
	int nRetVal = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	if(nRetVal == -1)
	{
		return NS_SUCCESS;
	}
	std::vector<variant_t> vectANO;
	CNSDatabaseMgr::getInstance()->getValue(_T("ANO"), vectANO);
	variant_t varVal, varAttrName;

	for(int nANOCntr = 0; nANOCntr < (int)vectANO.size(); nANOCntr++)
	{
		NSSPRINTF(szQuery, _T("Select AllowedValues from FeatureAttributes where FNO = %d and ANO = %i and Required <> 2 and Required <> 3"), pCollector->getFNO(), vectANO.at(nANOCntr).intVal );
		nRetVal = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
		if(nRetVal != -1)
		{	
			if(true != CNSDatabaseMgr::getInstance()->getValue(_T("AllowedValues"), varVal))
			{
				continue;
			}

			NSSPRINTF(szQuery, _T("Select AttNameInDWG from Attributes where ANO = %i"), vectANO.at(nANOCntr).intVal);
			int nRetVal = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
			if(nRetVal != -1)
			{
				bool bRet = CNSDatabaseMgr::getInstance()->getValue(_T("AttNameInDWG"), varAttrName);
				if(bRet	!= true)
					continue;
			}
			
			//NSSTRING strAttName = (NSSTRING)varAttrName.bstrVal;
			//NSSTRUPR((TCHAR*)strAttName.c_str(), strAttName.size()+1);
			#ifndef _INTERNAL_
				NSSTRING strAttName;
				strAttName = varAttrName.bstrVal;
				NSSTRUPR((TCHAR*)strAttName.c_str(), strAttName.size()+1);				
				//////mapOfAllowedValues.insert(STRING_MAP::value_type(strAttName, varVal.bstrVal));


				//*******************Line written on 2013/14 Conversion - 03Feb2014*****************************************
			/*	int nErrNumber;
				TCHAR szFormat[_MAX_PATH + 1];

				nErrNumber = WriteErrorRecEx(LOG_GIS,ERR_GIS_12,pCollector , _T("A$"),strAttName.c_str() ,_T("CV$"),strAttName ,_T("AV$"),strAttName);
				
				CNSPoint3D pt = pCollector->getInsertionPoint();
				gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_12, pt.m_dXX, pt.m_dYY, pt.m_dZZ);*/
				//************************************************************************************************************

			#else
                NSSTRING strAttName;
				strAttName = (NSSTRING)varAttrName.bstrVal;
				NSSTRUPR((TCHAR*)strAttName.c_str(), strAttName.size()+1);
				mapOfAllowedValues.insert(STRING_MAP::value_type(strAttName, varVal.bstrVal));
				/*TCHAR* strAtt = _com_util::ConvertBSTRToString(varVal.bstrVal);
				TCHAR* strVal = _com_util::ConvertBSTRToString(varAttrName.bstrVal);
				NSSTRUPR(strVal);
				mapOfAllowedValues.insert(STRING_MAP::value_type(strVal,strAtt));*/
			//	mapOfAllowedValues.insert(STRING_MAP::value_type((TCHAR*)strAttName.c_str(),strAtt));
			#endif
	
		}
	}

	//*************************************Commented on 03-Feb-2014***********************************************
	for(AllowedValuesItr = mapOfAllowedValues.begin(); AllowedValuesItr != mapOfAllowedValues.end(); AllowedValuesItr++)
	{
		AttrItr = pCollector->m_mapAttributes.find((*AllowedValuesItr).first);
		if(AttrItr != pCollector->m_mapAttributes.end())
		{
			//Check if the attributes values are valid
			if(NS_FAIL == compareAttributes((*AttrItr).second, (*AllowedValuesItr).second))
			{
				int nErrNumber;
				TCHAR szFormat[_MAX_PATH + 1];
                //NSSPRINTF(szFormat,_T("Mismatched values found for attribute \'%s\' in block \'%s\'. Current Value =\'%s\' Allowed Values = \'%s\'"), 
				//(*AttrItr).first.c_str(),pCollector->getBlockName().c_str(), (*AttrItr).second.c_str(),(*AllowedValuesItr).second.c_str());
				//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_12);

				nErrNumber = WriteErrorRecEx(LOG_GIS,ERR_GIS_12,pCollector , _T("A$"),(*AttrItr).first.c_str() ,_T("CV$"),(*AttrItr).second.c_str() ,_T("AV$"),(*AllowedValuesItr).second.c_str());
				
				CNSPoint3D pt = pCollector->getInsertionPoint();
				gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_12, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
			}
		}
	}

	//************************************************************************************************************

return NS_SUCCESS;
}

int CNSLayerBlockValidator::checkGISPrompt(CNSValidationDataCollector *pCollector)
{
	if(pCollector == NULL) return NS_FAIL;
	std::map<NSSTRING, NSSTRING> mapOfPromptValues;
	std::map<NSSTRING, NSSTRING>::iterator mapPromptItr;
	TCHAR szFormat[_MAX_PATH + 1];
	m_pLocalDWGMgr->getPromptValues(pCollector->getObjectId(), mapOfPromptValues);

	return NS_SUCCESS;//RC changed this to match internal validator comparison

	for(mapPromptItr = mapOfPromptValues.begin(); mapPromptItr != mapOfPromptValues.end(); mapPromptItr++)
	{
		NSSTRING strAttrName = (*mapPromptItr).second;
		//if(strAttrName == _T(""))
		{
			int nErr;
			//NSSPRINTF(szFormat,_T("The GIS attribute \'%s\' value of block \'%s\' is empty"), strAttrName.c_str(), pCollector->getBlockName().c_str());
            //WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_13);

			nErr = WriteErrorRecEx(LOG_GIS,ERR_GIS_13, pCollector,_T("A$"),strAttrName.c_str() );
			
            CNSPoint3D pt = pCollector->getInsertionPoint();
			gErrorCircle.drawCircle(nErr, LOG_GIS, ERR_GIS_13, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
		}
	}

	return NS_SUCCESS;
}

int CNSLayerBlockValidator::compareAttributes(NSSTRING strActValue, NSSTRING strAllowedValue)
{
	STRING_MAP::iterator ActValueItr, AllowedValueItr;
	STRING_MAP mapOfActValues, mapOfAllowedValues;
	if(strActValue.length() == 0)
	{
		mapOfActValues.insert(STRING_MAP::value_type(_T(""), _T("")));
	}
	tokenize(strActValue, mapOfActValues);
	tokenize(strAllowedValue, mapOfAllowedValues);
	for(ActValueItr = mapOfActValues.begin(); ActValueItr != mapOfActValues.end(); ActValueItr++)
	{
		AllowedValueItr = mapOfAllowedValues.find((*ActValueItr).first);
		if(AllowedValueItr == mapOfAllowedValues.end())
		{
			return NS_FAIL;
		}
	}
	return NS_SUCCESS;
}

int CNSLayerBlockValidator::tokenize(NSSTRING strInputString, STRING_MAP &mapOfValues)
{
	TCHAR* pszTemp = NULL;
	TCHAR* pszSep = _T(";");
	CNSPoint3D ptTemp;
	int nPointCounter = 0;
	int nCordinateCounter = 1;

	// Establish string and get the first token
	TCHAR* pszNextToken;   //dummy 
	# ifndef _INTERNAL_
		pszTemp = NSSTRTOK((TCHAR*)strInputString.c_str(), pszSep, &pszNextToken);
	#else
		pszTemp = NSSTRTOK((TCHAR*)strInputString.c_str(), pszSep, &pszNextToken);
		//pszTemp = NSSTRTOK((TCHAR*)strInputString.c_str(), pszSep);
	#endif
	
	// While there are tokens in "string"
	while( pszTemp != NULL )
	{
		NSSTRING strTemp = pszTemp;
		#ifndef _INTERNAL_
			NSSTRUPR(pszTemp, strTemp.size()+1);
		#else
		    NSSTRUPR(pszTemp, strTemp.size()+1);
			//NSSTRUPR(pszTemp);
		#endif		
		
		mapOfValues.insert(STRING_MAP::value_type(pszTemp, _T("")));
		// Get next token
		# ifndef _INTERNAL_
			pszTemp = NSSTRTOK( NULL, pszSep, &pszNextToken);
		#else
			pszTemp = NSSTRTOK( NULL, pszSep, &pszNextToken);
			//pszTemp = NSSTRTOK( NULL, pszSep);
		#endif

	}
	return NS_SUCCESS;
}

/*!
 @Brief			Validates if the exsiitng feature is having FID as XData and is filled in
 @Param [in]	Feature Type LINEAR/BLOCK
 @Return		success/fail
 @Date			21 Aug 2007
*/
int CNSLayerBlockValidator::validateFIDforEXTBlock(CNSValidationDataCollector *pCollector, NSSTRING strFeatureType)
{
	VALIDATE_COL_MAP_TYPE::iterator mapOfRetItr; 

	if(NSSTRCMP(strFeatureType.c_str(), _T("BLOCK")) == 0)
	{
		CNSPoint3D ptObj = pCollector->getInsertionPoint();
		NSSTRING strFID = pCollector->readXDATA(_T("FID"));

		if(NSSTRCMP(strFID.c_str(), _T("")) == 0)
		{
            TCHAR szFormat[_MAX_PATH + 1];
            NSSPRINTF(szFormat,_T("FID Value is empty for block \'%s\' on layer \'%s\'."),  pCollector->getBlockName().c_str(), pCollector->getLayerName().c_str());
			//gErrorCircle.logError(ERR_FID, szFormat, LOG_GIS, ptObj.m_dXX, ptObj.m_dYY, ERR_GIS_28);
			//WriteErrorRec(LOG_GIS,ERR_GIS_28,pCollector);
			gErrorCircle.logErrorEx(ERR_FID, szFormat, LOG_GIS, ptObj.m_dXX, ptObj.m_dYY, pCollector,ERR_GIS_28);
			return NS_FAIL;
		}
	}
	if(NSSTRCMP(strFeatureType.c_str(), _T("LINEAR")) == 0)
	{
		CNSPoint3D ptObj = pCollector->getInsertionPoint();
		NSSTRING strFID = pCollector->readXDATA(_T("FID"));

		if(NSSTRCMP(strFID.c_str(), _T("")) == 0)
		{
            TCHAR szFormat[_MAX_PATH + 1];
            NSSPRINTF(szFormat,_T("FID Value is empty for feature on layer \'%s\'."), pCollector->getLayerName().c_str());
			//gErrorCircle.logError(ERR_FID,szFormat, LOG_GIS, ptObj.m_dXX, ptObj.m_dYY, ERR_GIS_31);
			//WriteErrorRec(LOG_GIS,ERR_GIS_31,pCollector);
			gErrorCircle.logErrorEx(ERR_FID,szFormat, LOG_GIS, ptObj.m_dXX, ptObj.m_dYY, pCollector,ERR_GIS_31);
			return NS_FAIL;
		}
	}
	return NS_SUCCESS;
}
