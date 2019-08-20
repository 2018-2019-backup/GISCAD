#include "StdAfx.h"
#include "nsassethandler.h"
#include "../../Include/NSCredentials.h"
#include "../../Include/NSAsset.h"
#include "../../Include/NSDatabaseMgr.h"
#include "../../Include/NSDataItem.h"

CNSAssetHandler::CNSAssetHandler(void)
{
	m_ErrorsOccured = false;
}

CNSAssetHandler::~CNSAssetHandler(void)
{
	std::map<NSSTRING, ASSET_VECTOR*>::iterator it;
	for(it = m_mapOfAssets.begin(); it != m_mapOfAssets.end(); it++)
	{
		ASSET_VECTOR* pVect = (*it).second;
		ASSET_VECTOR::iterator vectItr;
		for(vectItr = pVect->begin(); vectItr != pVect->end(); vectItr++)
		{
			CNSAsset* pAsset = (*vectItr);
			delete pAsset;
			pAsset = NULL;
		}
		delete pVect;
		pVect = NULL;
	}
	m_mapOfAssets.clear();
}
int CNSAssetHandler::scan()
{
	m_mapOfAssets.clear();
	ASSET_VECTOR vectOfAssets;
	m_LocalDWGObj.scanAssets(vectOfAssets);
	filterAssets(vectOfAssets);
	return NS_SUCCESS;
}

void CNSAssetHandler::setDatabase(CNSDWGMgr* pDWGMgr)
{
	m_LocalDWGObj = *pDWGMgr;
}

void CNSAssetHandler::setUserCredentials(CNSUserCredentials userCredentials)
{
	m_UserCredentials = userCredentials;
}

int CNSAssetHandler::scanTitleBlock(CNSTitleBlkInfo &titleBlk)
{
	STRING_MAP mapOfTitleBlkAtts, mapOfPromptValues;
	mapOfTitleBlkAtts.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CAP_NO"), _T("")));
	mapOfTitleBlkAtts.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("DrawingNumber"), _T("")));
	mapOfTitleBlkAtts.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ProjectNumber"), _T("")));
	mapOfTitleBlkAtts.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ProjectType"), _T("")));
	mapOfTitleBlkAtts.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("DEPOTCODE"), _T("")));
	mapOfTitleBlkAtts.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("EXPORTID"), _T("")));
	m_LocalDWGObj.listTitleBlockHeaderAttributes(mapOfTitleBlkAtts, mapOfPromptValues);
	STRING_MAP::iterator attributesItr;
	attributesItr = mapOfTitleBlkAtts.find(_T("CAP_NO"));
	NSSTRCPY(titleBlk.m_szCAP_NO, (*attributesItr).second.c_str());
	attributesItr = mapOfTitleBlkAtts.find(_T("DrawingNumber"));
	NSSTRCPY(titleBlk.m_szDrawingNumber, (*attributesItr).second.c_str());
	attributesItr = mapOfTitleBlkAtts.find(_T("ProjectNumber"));
	NSSTRCPY(titleBlk.m_szProjectNumber, (*attributesItr).second.c_str());
	attributesItr = mapOfTitleBlkAtts.find(_T("ProjectType"));
	NSSTRCPY(titleBlk.m_szProjectType, (*attributesItr).second.c_str());
	attributesItr = mapOfTitleBlkAtts.find(_T("DEPOTCODE"));
	NSSTRCPY(titleBlk.m_szDepotCode, (*attributesItr).second.c_str());
	attributesItr = mapOfTitleBlkAtts.find(_T("EXPORTID"));
	NSSTRCPY(titleBlk.m_szExportID, (*attributesItr).second.c_str());
	m_titleBlk = titleBlk;
	return NS_SUCCESS;
}

int CNSAssetHandler::filterAssets(ASSET_VECTOR &vectOfAssets)
{
	NSSTRING szAssets[10] = {_T("Pole"), _T("Column"), _T("LVPillar"), _T("Streetlight"), _T("SLCP"), _T("Cubicle"), 
			_T("LVServicePillar"), _T("Switch"), _T("Substation"), _T("SubSwitch")};
	std::map<NSSTRING, NSSTRING> mapofAssets;
	for(int nCntr = 0; nCntr < 10; nCntr++)
	{
		mapofAssets.insert(std::map<NSSTRING, NSSTRING>::value_type(_wcsupr((TCHAR*)szAssets[nCntr].c_str()), _T("")));
	}
	::CoInitialize(NULL);
	ASSET_VECTOR::iterator assetItr;
	CNSAsset *pAsset = NULL;
	if(CNSDatabaseMgr::getInstance()->openDatabase(_T(""), _T(""), _T("GISCAD")) == NS_FAIL) 
	{
		return NS_FAIL;
	}
	for(assetItr = vectOfAssets.begin(); assetItr != vectOfAssets.end(); assetItr++)
	{
		pAsset = (*assetItr);
		NSSTRING strAssetNumber = pAsset->getAssetNumber();
		//AK if(NSSTRCMP(strAssetNumber.c_str(), _T("")) == 0)
		{	

			/*NSSTRING strUsage, strOrientation;
			pAsset->getAttributeValue(_T("USAGE"), strUsage);
			strUsage = getQueryString(strUsage);
			pAsset->getAttributeValue(_T("ORIENTATION"), strOrientation);
			strOrientation = getQueryString(strOrientation);*/

			TCHAR szQuery[_MAX_PATH + 1];
			//NSSPRINTF(szQuery, _T("Select FeatureName, fno from BlockFeatures where NewBlock = \'%s\' and Usage %s and Orientation %s"),
				//pAsset->getBlockName().c_str(), strUsage.c_str(), strOrientation.c_str());
			NSSPRINTF(szQuery, _T("Select FeatureName, fno from BlockFeatures where NewBlock = \'%s\'"),
				pAsset->getBlockName().c_str());
			//NSSTRING strFinalQuery = getQueryString(pAsset, szQuery);
			//int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(strFinalQuery.c_str());
			int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
			_variant_t vFeatureName, vFNO;
			if(nDBRet != -1)
			{
				//bool bResult = CNSDatabaseMgr::getInstance()->getValue("FeatureName", vFeatureName);
				bool bResult = CNSDatabaseMgr::getInstance()->getValue(_T("FeatureName"), vFeatureName);
				if(bResult)
				{	
					NSSTRING strKey = (NSSTRING)((_bstr_t)(vFeatureName.bstrVal));
					std::map<NSSTRING, NSSTRING>::iterator assetsItr;
					assetsItr = mapofAssets.find(_wcsupr((TCHAR*)strKey.c_str()));
					if(assetsItr == mapofAssets.end())
					{
						delete pAsset;
						pAsset = NULL;
						continue;
					}	
					bResult = CNSDatabaseMgr::getInstance()->getValue(_T("fno"), vFNO);
					int nFNO;
					if(bResult)
					{
						nFNO = vFNO.intVal;
					}					
					_wcsupr((TCHAR*)strKey.c_str());
					std::map<NSSTRING, ASSET_VECTOR*>::iterator it = m_mapOfAssets.find(strKey);
					ASSET_VECTOR* pVector;
					pAsset->setFNO(nFNO);

					//AK START
					bool bAllowSubstnOrPillarAsset = false;
					if(NSSTRCMP(strAssetNumber.c_str(), _T("")) != 0)
					{
						if(NSSTRCMP(strKey.c_str(), _T("SUBSTATION")) == 0 || NSSTRCMP(strKey.c_str(), _T("LVPILLAR")) == 0)
							bAllowSubstnOrPillarAsset = true;
					}

					if(NSSTRCMP(strAssetNumber.c_str(), _T("")) == 0 || bAllowSubstnOrPillarAsset)
					{
						if(it == m_mapOfAssets.end())
						{
							pVector = new ASSET_VECTOR;						
							pVector->push_back(pAsset);
							m_mapOfAssets.insert(std::map<NSSTRING, ASSET_VECTOR*>::value_type(strKey, pVector));
						}
						else
						{
							pVector = (*it).second;
							pVector->push_back(pAsset);
						}
					}
					else
					{
						delete pAsset;
						pAsset = NULL;
					}
					//AK END
				}
				else
				{
					delete pAsset;
					pAsset = NULL;
				}
			}
			else
			{
				delete pAsset;
				pAsset = NULL;
			}

		}
		/* ak else
		{
			delete pAsset;
			pAsset = NULL;
		}*/
	}
	CNSDatabaseMgr::getInstance()->closeDatabase();
	::CoUninitialize();
	return NS_SUCCESS;
}

int CNSAssetHandler::getNumOfAssets(enum ASSET_TYPE enAssetType)
{
	NSSTRING strKey;
	int nSize = 0;
	switch(enAssetType)
	{
	case ASSET_POLE: strKey = _T("POLE"); break;
	case ASSET_COLUMN: strKey = _T("COLUMN"); break;
	case ASSET_PILLAR: strKey = _T("LVPILLAR"); break;
	case ASSET_SL: strKey = _T("STREETLIGHT"); break;
	case ASSET_SLCP: strKey = _T("SLCP"); break;
	case ASSET_CUBICLE: strKey = _T("CUBICLE"); break;
	case ASSET_LVSERVICEPILLAR: strKey = _T("LVSERVICEPILLAR"); break;
	case ASSET_SWITCH: strKey = _T("SWITCH"); break;
	case ASSET_SUBSTATION: strKey = _T("SUBSTATION"); break;
	case ASSET_SUBSWITCH: strKey = _T("SUBSWITCH"); break;
	}
	std::map<NSSTRING, ASSET_VECTOR*>::iterator it;// = m_mapOfAssets.find(strKey);
	for(it = m_mapOfAssets.begin(); it != m_mapOfAssets.end(); it++)
	{
		if(NSSTRCMP(((*it).first).c_str(), strKey.c_str()) == 0)
		{
			ASSET_VECTOR* pVector;
			pVector = (*it).second;
			if(pVector)
			{
				nSize = 0;
				ASSET_VECTOR::iterator vecItr;
				for(vecItr = pVector->begin();vecItr != pVector->end();vecItr++)
				{
					if(NSSTRCMP((*vecItr)->getAssetNumber().c_str(),_T(""))==0)
						nSize++;
				}
				//nSize = (int)pVector->size();
				return nSize;
			}

		}
	}
	/*ASSET_VECTOR* pVector;
	if(it != m_mapOfAssets.end())
	{
		pVector = (*it).second;
		if(pVector)
			nSize = (int)pVector->size();
	}*/
	return nSize;
}

int CNSAssetHandler::generateNumbers()
{
	FILE*fp;
	mapOfNumAssets.clear();
	m_mapOfEquipNoIDs.clear();
	m_mapOfSwitchType.clear();
	std::map<LONG_PTR, SWITCH_MAP*>::iterator switchItr;
	for(switchItr = mapofSwitches.begin(); switchItr != mapofSwitches.end(); switchItr++)
	{
		SWITCH_MAP* pMap = (*switchItr).second;
		SWITCH_MAP::iterator it;
		for(it = pMap->begin(); it != pMap->end(); it++)
		{
			OBJECTID_VECTOR* pVect = (*it).second;
			delete pVect;
			pVect = NULL;
		}
		delete pMap;
		pMap = NULL;
	}
	mapofSwitches.clear();
	std::map<LONG_PTR, ASSET_VECTOR*>::iterator pillarsIter;
	for(pillarsIter = mapOfPillars.begin(); pillarsIter != mapOfPillars.end(); pillarsIter++)
	{
		ASSET_VECTOR* pVect = (*pillarsIter).second;
		ASSET_VECTOR::iterator it;
		/*for(it = pVect->begin(); it != pVect->end(); it++)
		{
			CNSAsset *pAsset = (*it);
			delete pAsset;
			pAsset = NULL;
		}*/
		pVect->clear();
		delete pVect;
		pVect = NULL;
	}
	mapOfPillars.clear();
	try
	{
		fp = fopen("AssetDebug2.log","w");
		if(fp!=NULL)
			fprintf(fp,"In CNSAssetHandler::generateNumbers function\n");

		std::map<NSSTRING, ASSET_VECTOR*>::iterator itr;
		std::map<TCHAR*, TCHAR*> mapofCredentials;
		mapofCredentials.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("Username"), m_UserCredentials.m_szUserName));
		mapofCredentials.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("Password"), m_UserCredentials.m_szPassword));
		mapofCredentials.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("Position"), m_UserCredentials.m_szPosition));
		mapofCredentials.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("District"), m_UserCredentials.m_szDistrict));
		WriteToLog(LOG_HEADER, _T("ASSET TYPE            FNO            ASSET_NUMBER"));

		if(fp!=NULL)
			fprintf(fp,"In CNSAssetHandler::generateNumbers step 1\n");

		for(itr = m_mapOfAssets.begin(); itr != m_mapOfAssets.end(); itr++)
		{
			NSSTRING strAssetType = (*itr).first;
			ASSET_VECTOR* vectAssets = (*itr).second;
			if(/*(NSSTRCMP((TCHAR*)strAssetType.c_str(), "LVPillar") == 0) ||*/
					(NSSTRCMP((TCHAR*)strAssetType.c_str(), _T("LVServicePillar")) == 0) ||
					/*(NSSTRCMP((TCHAR*)strAssetType.c_str(), "Substation") == 0) ||*/
					(NSSTRCMP((TCHAR*)strAssetType.c_str(), _T("SubSwitch")) == 0))
					continue;

			if(fp!=NULL)
				fprintf(fp,"In CNSAssetHandler::generateNumbers step 2\n");

			int size = (int)vectAssets->size();
			ASSET_VECTOR::iterator vectItr;
			for(vectItr = vectAssets->begin(); vectItr != vectAssets->end(); vectItr++)
			{
				CNSAsset *pAsset = (*vectItr);
				int nFNO = pAsset->getFNO();
				NSSTRING strAssetNumber = pAsset->getAssetNumber();
				if(NSSTRCMP(strAssetNumber.c_str(), _T("")) != 0)
				{
					mapOfNumAssets.insert(std::map<LONG_PTR,NSSTRING>::value_type(pAsset->getObjectID(), strAssetNumber));
					continue;
				}
				int nResult = validateAsset(pAsset, strAssetType);
				if(nResult == NS_FAIL)
				{
					//TCHAR szInfo[_MAX_PATH + 1];
					//NSSPRINTF(szInfo, "Mandatory attributes for asset number generation not found in Block %s, FNO = %d ", (TCHAR*)(pAsset->getBlockName().c_str()), pAsset->getFNO());
					//WriteToLog(LOG_WARNINGS, szInfo);
				}
				else
				{
					std::vector<TCHAR*> replyVect;
					setReplyParameters(replyVect);
					std::map<TCHAR*, TCHAR*> mapOfRequest;
					std::map<TCHAR*, TCHAR*>::iterator itr;
					setRequestParameters(mapOfRequest, strAssetType, pAsset);
					if(fp!=NULL)
						fprintf(fp,"In CNSAssetHandler::generateNumbers step 3\n");
					int iRet = prepareSOAPMessage(mapofCredentials, replyVect, mapOfRequest);
					if(fp!=NULL)
						fprintf(fp,"In CNSAssetHandler::generateNumbers step 4\n");
					if(iRet == NS_FAIL)
						continue;
					for(itr = mapOfRequest.begin(); itr != mapOfRequest.end(); itr++)
					{
						TCHAR* szValue = (*itr).second;
						delete[] szValue;
						szValue = NULL;
					}
					std::vector<CNSDataItem*> dataItemsVect;
					std::vector<CNSDataItem*>::iterator dataItemItr;
					std::vector<TCHAR*> vecErrors;
					TCHAR* szPlatNumber = m_ellipseConnector.getResult(dataItemsVect,vecErrors);
				/*	if(vecErrors.size()>0)
					{
						for(int i=0;i<vecErrors.size();i++)
						{
							WriteToLog(LOG_OTHER, vecErrors.at(i));
						}
						SetErrorsOccuredFlag(true);
						return NS_FAIL;
					}*/
					if(NS_FAIL == processErrMessages(vecErrors))
						return NS_FAIL;
					if(fp!=NULL)
						fprintf(fp,"In CNSAssetHandler::generateNumbers step 5\n");
					TCHAR* szEquipNumber = new TCHAR[_MAX_PATH + 1];
					NSSTRCPY (szEquipNumber,_MAX_PATH+1, _T(""));
				//	NSSTRCPY (szEquipNumber, _T(""));
					for(dataItemItr = dataItemsVect.begin(); dataItemItr != dataItemsVect.end(); dataItemItr++)
					{
						int size = (int)dataItemsVect.size();
						CNSDataItem* pDataItem = (*dataItemItr);
						if(pDataItem == NULL)
						{
							return NS_FAIL;
						}
					/*	TCHAR* szPlantNo = pDataItem->getValueFromMap("PlantNo");
						NSSTRING strPlantNumber = getPlantNo(szPlantNo);
						if(fp!=NULL)
							fprintf(fp,"In CNSAssetHandler::generateNumbers step 6\n");
						int iRet = m_LocalDWGObj.setAssetNumber(pAsset->getObjectID(), (TCHAR*)strPlantNumber.c_str());
						if(fp!=NULL)
							fprintf(fp,"In CNSAssetHandler::generateNumbers after setting asset number step 7\n");
						if(iRet != -1)
						{
							mapOfNumAssets.insert(std::map<long,int>::value_type(pAsset->getObjectID(), 1));
							TCHAR szMsg[1000];												
							NSSPRINTF(szMsg, "%s           %d           %s", (TCHAR*)strAssetType.c_str(), nFNO, (TCHAR*)strPlantNumber.c_str());
							WriteToLog(LOG_HEADER, szMsg);
						}
						else
						{
							TCHAR szMsg[_MAX_PATH + 1];
							NSSPRINTF(szMsg, "Failed to add asset number for Asset type = %s FNO = %d ", (TCHAR*)strAssetType.c_str(), pAsset->getFNO());
							WriteToLog(LOG_HEADER, szMsg);
							SetErrorsOccuredFlag(true);
						}

						TCHAR* szEquip = pDataItem->getValueFromMap("EquipNo");
						strcpy(szEquipNumber, szEquip);*/
						///OP_ID implementation
						TCHAR* szEquipNo = pDataItem->getValueFromMap(_T("EquipNo"));
                        NSSTRCPY(szEquipNumber,NSSTRLEN(szEquipNo)+1 ,szEquipNo);
						//NSSTRCPY(szEquipNumber, szEquipNo);
						m_mapOfEquipNoIDs.insert(std::map<LONG_PTR, NSSTRING>::value_type(pAsset->getObjectID(), szEquipNumber));
						delete []szEquipNo;
						///OP_ID implementation
					}
					
					// Link to project
					if(fp!=NULL)
						fprintf(fp,"In CNSAssetHandler::generateNumbers step 8\n");
					setLinkToProject(mapofCredentials, szEquipNumber);
					if(fp!=NULL)
							fprintf(fp,"In CNSAssetHandler::generateNumbers step 9\n");
				//	delete[] szEquipNumber;
				//	szEquipNumber = NULL;
				}
			}

		}
		if(fp!=NULL)
			fprintf(fp,"In CNSAssetHandler::generateNumbers step 10\n");
		// Creating assets for MainsPillars and Service Pillars
	/*	generateAssetsForPillars(mapofCredentials);
		if(fp!=NULL)
			fprintf(fp,"In CNSAssetHandler::generateNumbers step 11\n");*/

		// Creating assets for Substations ans switches
		//generateAssetsForSwitches(mapofCredentials);

		if(fp!=NULL)
			fprintf(fp,"In CNSAssetHandler::generateNumbers step 12\n");

		if(fp!=NULL)
			fclose(fp);

	/*	int nIDCntr = 0;
		std::map<long, NSSTRING>::iterator EquipNoIDsItr;
		for(EquipNoIDsItr = m_mapOfEquipNoIDs.begin(); EquipNoIDsItr != m_mapOfEquipNoIDs.end(); EquipNoIDsItr++)
		{
			TCHAR *pNum = new TCHAR[MAX_PATH+1];
			if(nIDCntr == 0)
			{
				NSSTRCPY(pNum,_T("000001401160"));
			}
			else if(nIDCntr == 1)
			{
				NSSTRCPY(pNum, _T("000001401161"));
			}
			else if(nIDCntr == 2)
			{
				NSSTRCPY(pNum, _T("000001401159"));
			}
			nIDCntr++;
			(*EquipNoIDsItr).second = pNum;
		}*/
		if(m_mapOfEquipNoIDs.size() > 0)
		{
			STRING_MAP mapOfValues;
			assignOPIds(mapofCredentials, mapOfValues);
			setAssetNumbers(mapOfValues);
		}
		m_mapOfEquipNoIDs.clear();
		generateAssetForSubSwitches(mapofCredentials);
		m_mapOfEquipNoIDs.clear();
        generateAssetForServicePillars(mapofCredentials);

		std::map<LONG_PTR, NSSTRING>::iterator theNumIterator;
		std::map<NSSTRING, ASSET_VECTOR*>::iterator assetsItr;
		ASSET_VECTOR::iterator assetVectItr;
		ASSET_VECTOR* pAssetVect = NULL;
		CNSAsset* pAsset = NULL;
        for(assetsItr = m_mapOfAssets.begin(); assetsItr != m_mapOfAssets.end(); assetsItr++)
		{
			pAssetVect = (*assetsItr).second;
			for(assetVectItr = pAssetVect->begin(); assetVectItr != pAssetVect->end(); assetVectItr++)
			{
				pAsset = *assetVectItr;
				theNumIterator = mapOfNumAssets.find(pAsset->getObjectID());
				CNSPoint3D pt;
				 m_LocalDWGObj.GetInsertionPoint(pAsset->getObjectID(), pt);
				if(theNumIterator == mapOfNumAssets.end())
				{
					TCHAR szMsg[_MAX_PATH + 1];												
					NSSPRINTF(szMsg, _T("Failed to add asset number for Asset type = \'%s\' on layer = \'%s\' at location(X=%lf, Y=%lf)"), pAsset->getBlockName().c_str(), pAsset->getLayerName().c_str(), pt.m_dXX, pt.m_dYY);
					WriteToLog(LOG_OTHER, szMsg);
				}
			}
		}
	}
	catch(...)
	{
		if(fp!=NULL)
			fclose(fp);
	}
	return NS_SUCCESS;
}

int CNSAssetHandler::validateAsset(CNSAsset* pAsset, NSSTRING strAssetType, bool bDisplayErrorMsg)
{
	int nResult = NS_SUCCESS;
	std::map<NSSTRING, NSSTRING> mapofBlkAttbs;
	bool bErrorTitle1 = false,bErrorTitle2 = false;  
	TCHAR szInfo1[_MAX_PATH + 1];
	TCHAR szInfo2[_MAX_PATH + 1];
	
	// These are the mandatory block attributes to validate asset.
	std::vector<NSSTRING> requiredAttributesVect;
	requiredAttributesVect.push_back(_T("STREETNAME"));
	requiredAttributesVect.push_back(_T("SUBURB"));
	if((NSSTRCMP((TCHAR*)strAssetType.c_str(), _T("Substation")) == 0) ||
		(NSSTRCMP((TCHAR*)strAssetType.c_str(), _T("SubSwitch")) == 0)||
		(NSSTRCMP((TCHAR*)strAssetType.c_str(), _T("HVSwitch")) == 0))
	{
		requiredAttributesVect.push_back(_T("EQUIPGRPID"));
	}
	if((NSSTRCMP((TCHAR*)strAssetType.c_str(), _T("SubSwitch")) == 0))
	{
		//requiredAttributesVect.push_back(_T("SWITCH_TYPE"));
		requiredAttributesVect.push_back(_T("EQUIP_TYPE"));
		requiredAttributesVect.push_back(_T("SUBSTN_NUMBER"));
	}
	if((NSSTRCMP((TCHAR*)strAssetType.c_str(), _T("LVServicePillar")) == 0))
	{
		requiredAttributesVect.push_back(_T("SUFFIX"));
		requiredAttributesVect.push_back(_T("PARENT"));
	}
	for(int nCntr = 0; nCntr < (int)requiredAttributesVect.size(); nCntr++)
	{
		int nErrResult = pAsset->checkAttribute(requiredAttributesVect.at(nCntr));
		if(NS_SUCCESS != nErrResult)
		{
			if(nErrResult == -101)
			{
				if(!bErrorTitle1)
				{
					bErrorTitle1 = true;
					//NSSPRINTF(szInfo, "Following mandatory attributes information not found in Block %s, FNO = %d:", (TCHAR*)(pAsset->getBlockName().c_str()), pAsset->getFNO());
					NSSPRINTF(szInfo1, _T("Following mandatory attributes are not present for block type %s: "), strAssetType.c_str());
					SetErrorsOccuredFlag(true);
					///WriteToLog(LOG_WARNINGS, szInfo);
				}
				NSSTRING szFailedAtt = requiredAttributesVect.at(nCntr);
				szFailedAtt.append(_T(","));
				lstrcat(szInfo1,szFailedAtt.c_str());
			}
			else if(nErrResult == -102)
			{
				if(!bErrorTitle2)
				{
					bErrorTitle2 = true;
					//NSSPRINTF(szInfo, "Following mandatory attributes information not found in Block %s, FNO = %d:", (TCHAR*)(pAsset->getBlockName().c_str()), pAsset->getFNO());
					NSSPRINTF(szInfo2, _T("Values of following mandatory attributes are empty for block type %s: "), strAssetType.c_str());
					SetErrorsOccuredFlag(true);
					///WriteToLog(LOG_WARNINGS, szInfo);
				}
				NSSTRING szFailedAtt = requiredAttributesVect.at(nCntr);
				szFailedAtt.append(_T(","));
				lstrcat(szInfo2,szFailedAtt.c_str());
			}
			
			//WriteToLog(LOG_OTHER,(TCHAR*)szFailedAtt.c_str());

			//szVecFailed.push_back(std::vector<NSSTRING>::value_type(szFailedAtt.c_str()));
			nResult = NS_FAIL;
		}
	}
	if(bErrorTitle1 && bDisplayErrorMsg)
		WriteToLog(LOG_OTHER, szInfo1);
	if(bErrorTitle2 && bDisplayErrorMsg)
		WriteToLog(LOG_WARNINGS, szInfo2);
	return nResult;
}

void CNSAssetHandler::setRequestParameters(std::map<TCHAR*, TCHAR*> & mapOfRequest, NSSTRING strAssetType, CNSAsset* pAsset)
{
	
	TCHAR *szEquipClass = new TCHAR[_MAX_PATH + 1];
	TCHAR *szPlantCode0 = new TCHAR[_MAX_PATH + 1];
	TCHAR *szEquipGrpID = new TCHAR[_MAX_PATH + 1];
	TCHAR *szActiveFlag = new TCHAR[_MAX_PATH + 1];
	TCHAR *szEquipStatus = new TCHAR[_MAX_PATH + 1];
	TCHAR *szEquipClassif18 = new TCHAR[_MAX_PATH + 1];
	TCHAR *szUserName = new TCHAR[_MAX_PATH + 1];
	TCHAR *szEquipClassif6 = new TCHAR[_MAX_PATH + 1];
	TCHAR *szEquipNoD2 = new TCHAR[_MAX_PATH + 1];

	std::map<NSSTRING, NSSTRING> mapOfAttributes;
	m_LocalDWGObj.readBlockAttributes(pAsset->getObjectID(), mapOfAttributes);

	////NSSTRCPY(pMessage, szLogMessage);
	//NSSTRING strMsg(szLogMessage);
	//NSSTRCPY(pMessage, , szLogMessage);

	NSSTRCPY(szEquipNoD2,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("LOCATION")))+1 ,getValueFromMap(mapOfAttributes, _T("LOCATION")));
	
	if(NSSTRCMP(strAssetType.c_str(), _T("Pole")) == 0)
	{
		//strcpy(szEquipClass, getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
        NSSTRCPY(szEquipClass,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")))+1 ,getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		if(NSSTRCMP(szEquipClass, _T("")) == 0)
			//strcpy(szEquipClass, _T("PL"));
            NSSTRCPY(szEquipClass,(int)NSSTRLEN( _T("PL"))+1,_T("PL"));
		NSSTRCPY(szPlantCode0,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("PLANTCODE0")))+1,getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		if(NSSTRCMP(szPlantCode0, _T("")) == 0)
			//strcpy(szPlantCode0, _T("PL"));
			NSSTRCPY(szPlantCode0,(int)NSSTRLEN( _T("PL"))+1,_T("PL"));
		//strcpy(szEquipGrpID, getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
        NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		if(NSSTRCMP(szEquipGrpID, _T("")) == 0)
			//strcpy(szEquipGrpID, _T("PLUNK"));
			NSSTRCPY(szEquipGrpID,(int)NSSTRLEN( _T("PLUNK"))+1,_T("PLUNK"));
		//strcpy(szEquipClassif18, getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		if(NSSTRCMP(szEquipClassif18, _T("")) == 0)
			//strcpy(szEquipClassif18, _T("DI"));
			NSSTRCPY(szEquipClassif18,(int)NSSTRLEN( _T("DI"))+1,_T("DI"));
		
	}
	else if(NSSTRCMP(strAssetType.c_str(), _T("Column")) == 0)
	{
		//strcpy(szEquipClass, getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		NSSTRCPY(szEquipClass,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		if(NSSTRCMP(szEquipClass, _T("")) == 0)
		//	strcpy(szEquipClass, _T("CL"));
		NSSTRCPY(szEquipClass,(int)NSSTRLEN( _T("CL"))+1,_T("CL"));
		//strcpy(szPlantCode0, getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
        NSSTRCPY(szPlantCode0,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("PLANTCODE0")))+1,getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		if(NSSTRCMP(szPlantCode0, _T("")) == 0)
			//strcpy(szPlantCode0, _T("PL"));
			NSSTRCPY(szPlantCode0,(int)NSSTRLEN(_T("PL"))+1,_T("PL"));
		//strcpy(szEquipGrpID, getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		 NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		if(NSSTRCMP(szEquipGrpID, _T("")) == 0)
			//strcpy(szEquipGrpID, _T("CLUNK"));
			 NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(_T("CLUNK"))+1,_T("CLUNK"));
		//strcpy(szEquipClassif18, getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		 NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		if(NSSTRCMP(szEquipClassif18, _T("")) == 0)
			//strcpy(szEquipClassif18, _T("DI"));
			NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(_T("DI"))+1,_T("DI"));

	}
	else if(NSSTRCMP(strAssetType.c_str(), _T("LVPillar")) == 0)
	{
		//strcpy(szEquipClass, getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		if(NSSTRCMP(szEquipClass, _T("")) == 0)
			//strcpy(szEquipClass, _T("LP"));
			NSSTRCPY(szEquipClass,(int)NSSTRLEN(_T("LP"))+1,_T("LP"));
		//strcpy(szPlantCode0, getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		NSSTRCPY(szPlantCode0,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("PLANTCODE0")))+1,getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		if(NSSTRCMP(szPlantCode0, _T("")) == 0)
			//strcpy(szPlantCode0, _T("LV"));
			NSSTRCPY(szPlantCode0,(int)NSSTRLEN(_T("LV"))+1,_T("LV"));
		//strcpy(szEquipGrpID, getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		if(NSSTRCMP(szEquipGrpID, _T("")) == 0)
			//strcpy(szEquipGrpID, _T("LVPILL"));
			NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(_T("LVPILL"))+1,_T("LVPILL"));
		//strcpy(szEquipClassif18, getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		if(NSSTRCMP(szEquipClassif18, _T("")) == 0)
			//strcpy(szEquipClassif18,_T("DI"));		
			NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(_T("DI"))+1,_T("DI"));
	}
	else if(NSSTRCMP(strAssetType.c_str(), _T("Streetlight")) == 0)
	{
		//strcpy(szEquipClass, getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		NSSTRCPY(szEquipClass,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		if(NSSTRCMP(szEquipClass, _T("")) == 0)
			//strcpy(szEquipClass, _T("SL"));
			NSSTRCPY(szEquipClass,(int)NSSTRLEN(_T("SL"))+1,_T("SL"));
		//strcpy(szPlantCode0, getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		NSSTRCPY(szPlantCode0,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("PLANTCODE0")))+1,getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		if(NSSTRCMP(szPlantCode0, _T("")) == 0)
			//strcpy(szPlantCode0, _T("SL"));
			NSSTRCPY(szPlantCode0,(int)NSSTRLEN(_T("SL"))+1,_T("SL"));
		//strcpy(szEquipGrpID, getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		if(NSSTRCMP(szEquipGrpID, _T("")) == 0)
			//strcpy(szEquipGrpID, _T("SLUNK"));
			NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(_T("SLUNK"))+1,_T("SLUNK"));
	//	strcpy(szEquipClassif18, getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		if(NSSTRCMP(szEquipClassif18, _T("")) == 0)
			//strcpy(szEquipClassif18, _T("SL"));
			NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(_T("SL"))+1,_T("SL"));
		
	}
	else if(NSSTRCMP(strAssetType.c_str(), _T("SLCP")) == 0)
	{
		//strcpy(szEquipClass, getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		NSSTRCPY(szEquipClass,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		if(NSSTRCMP(szEquipClass, _T("")) == 0)
			//strcpy(szEquipClass, _T("szEquipClass"));
			NSSTRCPY(szEquipClass,(int)NSSTRLEN(_T("szEquipClass"))+1,_T("szEquipClass"));
		//strcpy(szPlantCode0, getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		NSSTRCPY(szPlantCode0,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("PLANTCODE0")))+1,getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		if(NSSTRCMP(szPlantCode0, _T("")) == 0)
			//strcpy(szPlantCode0, _T("CP"));
			NSSTRCPY(szPlantCode0,(int)NSSTRLEN(_T("CP"))+1,_T("CP"));
		//strcpy(szEquipGrpID, getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		if(NSSTRCMP(szEquipGrpID, _T("")) == 0)
			//strcpy(szEquipGrpID, _T("CPZELL"));
			NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(_T("CPZELL"))+1,_T("CPZELL"));
		//strcpy(szEquipClassif18, getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		if(NSSTRCMP(szEquipClassif18, _T("")) == 0)
			//strcpy(szEquipClassif18, _T("SL"));
			NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(_T("SL"))+1,_T("SL"));
		
	}
	else if(NSSTRCMP(strAssetType.c_str(), _T("Cubicle")) == 0)
	{
		//strcpy(szEquipClass, getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		NSSTRCPY(szEquipClass,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		if(NSSTRCMP(szEquipClass, _T("")) == 0)
			//strcpy(szEquipClass, _T("LC"));
			NSSTRCPY(szEquipClass,(int)NSSTRLEN(_T("LC"))+1,_T("LC"));
		//strcpy(szPlantCode0, getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		NSSTRCPY(szPlantCode0,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("PLANTCODE0")))+1,getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		if(NSSTRCMP(szPlantCode0, _T("")) == 0)
			//strcpy(szPlantCode0, _T("LV"));
			NSSTRCPY(szPlantCode0,(int)NSSTRLEN(_T("LV"))+1,_T("LV"));
		//strcpy(szEquipGrpID, getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		if(NSSTRCMP(szEquipGrpID, _T("")) == 0)
			//strcpy(szEquipGrpID, _T("LVCUB"));
			NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(_T("LVCUB"))+1,_T("LVCUB"));
		//strcpy(szEquipClassif18, getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		if(NSSTRCMP(szEquipClassif18, _T("")) == 0)
			//strcpy(szEquipClassif18, _T("DI"));
			NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(_T("DI"))+1,_T("DI"));
	}
	else if(NSSTRCMP(strAssetType.c_str(), _T("LVServicePillar")) == 0)
	{
		//strcpy(szEquipClass, getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		NSSTRCPY(szEquipClass,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		if(NSSTRCMP(szEquipClass, _T("")) == 0)
			//strcpy(szEquipClass, _T("LR");
			NSSTRCPY(szEquipClass,(int)NSSTRLEN(_T("LR"))+1,_T("LR"));
		//strcpy(szPlantCode0, getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		NSSTRCPY(szPlantCode0,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("PLANTCODE0")))+1,getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		if(NSSTRCMP(szPlantCode0, _T("")) == 0)
			//strcpy(szPlantCode0, _T("LV"));
			NSSTRCPY(szPlantCode0,(int)NSSTRLEN(_T("LV"))+1,_T("LV"));
		//strcpy(szEquipGrpID, getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		if(NSSTRCMP(szEquipGrpID, _T("")) == 0)
		//	strcpy(szEquipGrpID, _T("LVPILL"));
			NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(_T("LVPILL"))+1,_T("LVPILL"));
		//strcpy(szEquipClassif18, getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		if(NSSTRCMP(szEquipClassif18, _T("")) == 0)
			//strcpy(szEquipClassif18, _T("DI"));
			NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(_T("DI"))+1,_T("DI"));

	}
	else if(NSSTRCMP(strAssetType.c_str(), _T("Substation")) == 0)
	{
		//strcpy(szEquipClass, getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		NSSTRCPY(szEquipClass,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		if(NSSTRCMP(szEquipClass, _T("")) == 0)
			//strcpy(szEquipClass, _T("DS"));
			NSSTRCPY(szEquipClass,(int)NSSTRLEN(_T("DS"))+1,_T("DS"));
		//strcpy(szPlantCode0, getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		NSSTRCPY(szPlantCode0,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("PLANTCODE0")))+1,getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		if(NSSTRCMP(szPlantCode0, _T("")) == 0)
			//strcpy(szPlantCode0, _T("DS"));
			NSSTRCPY(szPlantCode0,(int)NSSTRLEN(_T("DS"))+1,_T("DS"));
		//strcpy(szEquipGrpID, getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		if(NSSTRCMP(szEquipGrpID, _T("")) == 0)
			//strcpy(szEquipGrpID, _T(""));
			NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(_T(""))+1,_T(""));
		//strcpy(szEquipClassif18, getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		if(NSSTRCMP(szEquipClassif18, _T("")) == 0)
			//strcpy(szEquipClassif18, _T("DI"));
			NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(_T("DI"))+1,_T("DI"));

	}
	else if(NSSTRCMP(strAssetType.c_str(), _T("HVSwitch")) == 0)
	{
		//strcpy(szEquipClass, getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		NSSTRCPY(szEquipClass,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		if(NSSTRCMP(szEquipClass, _T("")) == 0)
			//strcpy(szEquipClass, _T("HF"));
			NSSTRCPY(szEquipClass,(int)NSSTRLEN(_T("HF"))+1,_T("HF"));
		//strcpy(szPlantCode0, getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		NSSTRCPY(szPlantCode0,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("PLANTCODE0")))+1,getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		if(NSSTRCMP(szPlantCode0, _T("")) == 0)
			//strcpy(szPlantCode0, _T("SW"));
			NSSTRCPY(szPlantCode0,(int)NSSTRLEN(_T("SW"))+1,_T("SW"));
		//strcpy(szEquipGrpID, getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPGRPID")));
		if(NSSTRCMP(szEquipGrpID, _T("")) == 0)
			//strcpy(szEquipGrpID, _T(""));
			NSSTRCPY(szEquipGrpID,(int)NSSTRLEN(_T(""))+1,_T(""));
		//strcpy(szEquipClassif18, getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		if(NSSTRCMP(szEquipClassif18, _T("")) == 0)
			//strcpy(szEquipClassif18, _T("DI"));
			NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(_T("DI"))+1,_T("DI"));
	}
	else if(NSSTRCMP(strAssetType.c_str(), _T("SubSwitch")) == 0)
	{
		//strcpy(szEquipClass, getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		NSSTRCPY(szEquipClass,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASS")));
		if(NSSTRCMP(szEquipClass, _T("")) == 0)
			//strcpy(szEquipClass, _T("HS"));
			NSSTRCPY(szEquipClass,(int)NSSTRLEN(_T("HS"))+1,_T("HS"));
		//strcpy(szPlantCode0, getValueFromMap(mapOfAttributes, "PLANTCODE0"));
		NSSTRCPY(szPlantCode0,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("PLANTCODE0")))+1,getValueFromMap(mapOfAttributes, _T("PLANTCODE0")));
		if(NSSTRCMP(szPlantCode0, _T("")) == 0)
			//strcpy(szPlantCode0, _T("DS"));
			NSSTRCPY(szPlantCode0,(int)NSSTRLEN(_T("DS"))+1,_T("DS"));
		/*strcpy(szEquipGrpID, getValueFromMap(mapOfAttributes, "EQUIPGRPID"));
		if(NSSTRCMP(szEquipGrpID, "") == 0)
			strcpy(szEquipGrpID, "");*/
		//strcpy(szEquipClassif18, getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")))+1,getValueFromMap(mapOfAttributes, _T("EQUIPCLASSIF18")));
		if(NSSTRCMP(szEquipClassif18, _T("")) == 0)
			//strcpy(szEquipClassif18, _T("DI"));
			NSSTRCPY(szEquipClassif18,(int)NSSTRLEN(_T("DI"))+1,_T("DI"));
	}
	//strcpy(szActiveFlag, _T("Y"));
	NSSTRCPY(szActiveFlag,(int)NSSTRLEN(_T("Y"))+1,_T("Y"));
	//strcpy(szEquipStatus, _T("PR"));
	NSSTRCPY(szEquipStatus,(int)NSSTRLEN(_T("PR"))+1,_T("PR"));
	

	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipClass"), szEquipClass));
	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("PlantCode0"), szPlantCode0));
	if(NSSTRCMP(strAssetType.c_str(), _T("LVServicePillar")) == 0)
	{
		//Assign parent asset number. This should be an attribute on the block.
		TCHAR* szPlatCode2 = new TCHAR[_MAX_PATH + 1];
		//strcpy(szPlatCode2, getValueFromMap(mapOfAttributes, _T("SUFFIX")));
		NSSTRCPY(szPlatCode2,(int)NSSTRLEN(getValueFromMap(mapOfAttributes, _T("SUFFIX")))+1,getValueFromMap(mapOfAttributes, _T("SUFFIX")));
		//service pillar suffix. This should be an attribute on the block.
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("PlantCode2"), szPlatCode2));
	}
	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipGrpId"), szEquipGrpID));
	NSSTRING strAccountCode = getAccountCode(m_titleBlk.m_szDepotCode, szEquipClass);
	TCHAR *szAccCode = new TCHAR[_MAX_PATH];
	//strcpy(szAccCode, strAccountCode.c_str());
	NSSTRCPY(szAccCode,strAccountCode.size()+1,strAccountCode.c_str());
	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("AccountCode"), szAccCode));
	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("ActiveFlag"), szActiveFlag));
	//strcpy(szUserName, m_UserCredentials.m_szUserName);
	NSSTRCPY(szUserName,(int)NSSTRLEN(m_UserCredentials.m_szUserName)+1,m_UserCredentials.m_szUserName);
	//mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type("InputBy", m_UserCredentials.m_szUserName));
	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("InputBy"), szUserName));
	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipStatus"), szEquipStatus));
	NSSTRING strD1 = pAsset->getEquipNumberD1();
	TCHAR *szEquipNoD1 = new TCHAR[_MAX_PATH];
	//strcpy(szEquipNoD1, strD1.c_str());
	NSSTRCPY(szEquipNoD1,strD1.size()+1,strD1.c_str());
	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipNoD1"), szEquipNoD1));
	//strcpy(szEquipClassif6, m_titleBlk.m_szDepotCode);
	NSSTRCPY(szEquipClassif6,(int)NSSTRLEN(m_titleBlk.m_szDepotCode)+1,m_titleBlk.m_szDepotCode);
	//mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type("EquipClassif6", m_titleBlk.m_szDepotCode));
	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipClassif6"), szEquipClassif6));
	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipClassif18"), szEquipClassif18));
	if(NSSTRCMP(szEquipNoD2, _T("")) != 0)
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipNoD2"), szEquipNoD2));
}

void CNSAssetHandler::setReplyParameters(std::vector<TCHAR*> & replyVect)
{
	replyVect.push_back((TCHAR*)_T("EquipNo"));
	replyVect.push_back((TCHAR*)_T("EquipClass"));
	replyVect.push_back((TCHAR*)_T("EquipGrpId"));
	replyVect.push_back((TCHAR*)_T("PlantNo"));
	replyVect.push_back((TCHAR*)_T("EquipStatus"));
}

void CNSAssetHandler::setLinkToProject(std::map<TCHAR*, TCHAR*> & mapofCredentials, TCHAR* szEquipNumber)
{

	std::vector<TCHAR*> replyVect;
	replyVect.push_back((TCHAR*)_T("EquipNo"));
	std::map<TCHAR*, TCHAR*> mapOfRequest;
	int iRet = setProjectLink(_T("NAMEPLATE.Modify"), replyVect, mapOfRequest, mapofCredentials, szEquipNumber, true);
	if(iRet != NS_SUCCESS)
	{
		WriteToLog(LOG_OTHER, _T("NamePlate not modified"));
		SetErrorsOccuredFlag(true);
	}

	TCHAR* szProjectType = m_titleBlk.m_szProjectType;
	NSSTRING strTemp = szProjectType;
	NSSTRUPR(szProjectType,strTemp.size()+1);
	if(NSSTRCMP(szProjectType, _T("INTERNAL")) == 0)
	{
		mapOfRequest.clear();
		NSSTRING strProjectNumber = _T("PROJECT");
		strProjectNumber.append(_T("                          "));
		strProjectNumber.append(m_titleBlk.m_szProjectNumber);
		int nSpaces = 33 - (int)NSSTRLEN(m_titleBlk.m_szProjectNumber);
		for(int nCntr = 0; nCntr < nSpaces; nCntr++)
		{
			strProjectNumber.append(_T(" "));
		}
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipNo"), szEquipNumber));
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("AlternateRef"), (TCHAR*)strProjectNumber.c_str()));
		replyVect.clear();
		replyVect.push_back((TCHAR*)_T("AlternateRef"));
		iRet = setProjectLink(_T("EQUIPMENT.CreateAlternateref"), replyVect, mapOfRequest, mapofCredentials, szEquipNumber, false);
		if(iRet != NS_SUCCESS)
		{
			WriteToLog(LOG_OTHER, _T("Failed in CreateAlternateRef"));
			SetErrorsOccuredFlag(true);
		}
	}

	mapOfRequest.clear();						
	NSSTRING strDrawingNumber = _T("DRG");
	strDrawingNumber.append(m_titleBlk.m_szDrawingNumber);
	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipNo"), szEquipNumber));
	if(NSSTRCMP(szProjectType, _T("EXTERNAL")) == 0)
	{
		int nLen = (int)strDrawingNumber.length();
		int nSpaces = 24 - nLen;
		for(int nCntr = 0; nCntr < nSpaces; nCntr++)
		{
			strDrawingNumber = strDrawingNumber.append(_T(" "));
		}
		strDrawingNumber = strDrawingNumber.append(m_titleBlk.m_szCAP_NO);
	}
	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("ColloqEq"), (TCHAR*)strDrawingNumber.c_str()));
	replyVect.clear();
	replyVect.push_back(_T("EquipNo"));
	iRet = setProjectLink(_T("EQUIPMENT.UpdateColloquial"), replyVect, mapOfRequest, mapofCredentials, szEquipNumber, false);
	if(iRet != NS_SUCCESS)
	{
		WriteToLog(LOG_OTHER, _T("Failed in UpdateColloquial"));
		SetErrorsOccuredFlag(true);
	}
}

int CNSAssetHandler::setProjectLink(NSSTRING strService, std::vector<TCHAR*> & replyVect, std::map<TCHAR*, TCHAR*> & mapOfRequest, std::map<TCHAR*, TCHAR*> & mapofCredentials, TCHAR* szEquipNumber, bool bFlag)
{
	if(NS_SUCCESS != m_ellipseConnector.connect(m_UserCredentials.m_szHostID))
	{
		WriteToLog(LOG_OTHER, _T("Failed to connect to ellipse"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL; 
	}
	if(NS_SUCCESS != m_ellipseConnector.beginSOAPMessage(_T("process")))
	{
		WriteToLog(LOG_OTHER, _T("Failed to begin SOAP message"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.startRequest(_T("a_message_id"), _T("1.0"), _T("http://www.w3.org/2001/XMLSchema-instance"), 
		_T("H:\\Scada-ELL\\XSD\\EllipseRequest.xsd")))
	{
		WriteToLog(LOG_OTHER, _T("Failed in preparing Start Request"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.buildCredentials(mapofCredentials))
	{
		WriteToLog(LOG_OTHER, _T("Failed in build credentials"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.startService(strService.c_str()))
	{
		WriteToLog(LOG_OTHER, _T("Failed in start service"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.buildReply(replyVect))
	{
		WriteToLog(LOG_OTHER, _T("Failed in build reply"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}

	if(bFlag)
	{
		mapOfRequest.clear();
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipNo"), szEquipNumber));
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("AttributeName"), (TCHAR*)_T("PROJECT")));
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("AttributeValue"), m_titleBlk.m_szProjectNumber));
		if(NS_SUCCESS != m_ellipseConnector.buildRequest(mapOfRequest, (TCHAR*)_T("ID00001")))
		{
			WriteToLog(LOG_OTHER, _T("Failed in build RequestParameters"));
			SetErrorsOccuredFlag(true);
			return NS_FAIL;
		}
		mapOfRequest.clear();
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipNo"), szEquipNumber));
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("AttributeName"), (TCHAR*)_T("DRAWING")));
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("AttributeValue"), m_titleBlk.m_szDrawingNumber));
		if(NS_SUCCESS != m_ellipseConnector.buildRequest(mapOfRequest, (TCHAR*)_T("ID00002")))
		{
			WriteToLog(LOG_OTHER, _T("Failed in build RequestParameters"));
			SetErrorsOccuredFlag(true);
			return NS_FAIL;
		}
		mapOfRequest.clear();
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipNo"), szEquipNumber));
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("AttributeName"), (TCHAR*)_T("CAP")));
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("AttributeValue"), m_titleBlk.m_szCAP_NO));
		if(NS_SUCCESS != m_ellipseConnector.buildRequest(mapOfRequest, (TCHAR*)_T("ID00003")))
		{
			WriteToLog(LOG_OTHER, _T("Failed in build RequestParameters"));
			SetErrorsOccuredFlag(true);
			return NS_FAIL;
		}
	}
	else
	{
		if(NS_SUCCESS != m_ellipseConnector.buildRequest(mapOfRequest, (TCHAR*)_T("ID00001")))
		{
			WriteToLog(LOG_OTHER, _T("Failed in build RequestParameters"));
			SetErrorsOccuredFlag(true);
			return NS_FAIL;
		}
	}
	if(NS_SUCCESS != m_ellipseConnector.endElement())
	{
		WriteToLog(LOG_OTHER, _T("Failed in build End element in SetProejctLink"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.endElement())
	{
		WriteToLog(LOG_OTHER, _T("Failed in build End element in SetProejctLink"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.endSOAPMessage())
	{
		WriteToLog(LOG_OTHER, _T("Failed in end SOAP message in SetProejctLink"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}

	std::vector<CNSDataItem*> dataItemsVect;
	std::vector<CNSDataItem*>::iterator dataItemItr;
	std::vector<TCHAR*> vecErrors;

	TCHAR* szPlatNumber = m_ellipseConnector.getResult(dataItemsVect,vecErrors);
	if(NS_FAIL == processErrMessages(vecErrors))
			return NS_FAIL;
	for(dataItemItr = dataItemsVect.begin(); dataItemItr != dataItemsVect.end(); dataItemItr++)
	{
		CNSDataItem* pDataItem = (*dataItemItr);
		TCHAR* szEquip = pDataItem->getValueFromMap((TCHAR*)_T("EquipNo"));
	}
	
	return NS_SUCCESS;
}

NSSTRING CNSAssetHandler::getAccountCode(NSSTRING strDepot, NSSTRING strEquipClass)
{
	CNSDatabaseMgr* pDatabase = CNSDatabaseMgr::getInstance();
	int iRet = pDatabase->openDatabase(_T(""), _T(""), _T("GISCAD"));
	if(iRet == NS_FAIL)
		return _T("");
	TCHAR szQuery[_MAX_PATH + 1];
	NSSTRING strActivity, strOrgUnit;
	NSSPRINTF(szQuery, _T("select Activity from Activity where EquipClass = \'%s\'"), strEquipClass.c_str());
	if(-1 != pDatabase->executeQuery(szQuery))
	{
		variant_t vActivity;
		bool bRet = pDatabase->getValue((TCHAR*)_T("Activity"), vActivity);
		if(bRet != false)
		{
			strActivity = (NSSTRING)((_bstr_t)(vActivity.bstrVal));
		}
	}

	NSSPRINTF(szQuery, _T("select OrgUnit from OrgUnit where Depot = \'%s\'"), strDepot.c_str());
	if(-1 != pDatabase->executeQuery(szQuery))
	{
		variant_t vOrgUnit;
		bool bRet = pDatabase->getValue(_T("OrgUnit"), vOrgUnit);
		if(bRet != false)
		{
			strOrgUnit = (NSSTRING)((_bstr_t)(vOrgUnit.bstrVal));
		}
	}

	pDatabase->closeDatabase();
	return strOrgUnit.append(_T("0")).append(strActivity);
}

int CNSAssetHandler::validate()
{
	return NS_FAIL;
}

int CNSAssetHandler::prepareSOAPMessage(std::map<TCHAR*, TCHAR*> & mapofCredentials, std::vector<TCHAR*> & replyVect, std::map<TCHAR*, TCHAR*> & mapOfRequest)
{
	if(NS_SUCCESS != m_ellipseConnector.connect(m_UserCredentials.m_szHostID))
	{
		WriteToLog(LOG_OTHER, _T("Failed to Connect to Ellipse Webservice"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL; 
	}
	if(NS_SUCCESS != m_ellipseConnector.beginSOAPMessage(_T("process")))
	{
		WriteToLog(LOG_OTHER, _T("Failed to build SOAP message"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.startRequest(_T("a_message_id"), _T("1.0"), _T("http://www.w3.org/2001/XMLSchema-instance"), 
		_T("H:\\Scada-ELL\\XSD\\EllipseRequest.xsd")))
	{
		WriteToLog(LOG_OTHER, _T("Failed to Start Request to Ellipse"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.buildCredentials(mapofCredentials))
	{
		WriteToLog(LOG_OTHER, _T("Failed to build Credentials"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.startService(_T("EQUIPMENT.Create")))
	{
		WriteToLog(LOG_OTHER, _T("Failed to start service"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.buildReply(replyVect))
	{
		WriteToLog(LOG_OTHER, _T("Failed to build reply parameters"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	
	if(NS_SUCCESS != m_ellipseConnector.buildRequest(mapOfRequest, _T("ID00001")))
	{
		WriteToLog(LOG_OTHER, _T("Failed to build request parameters"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.endElement())
	{
		WriteToLog(LOG_OTHER, _T("Failed to End Element"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.endElement())
	{
		WriteToLog(LOG_OTHER, _T("Failed to End Element"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.endSOAPMessage())
	{
		WriteToLog(LOG_OTHER, _T("Ellipse Connection Failure : Failed to send SOAP message to web service"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	return NS_SUCCESS;
}

int CNSAssetHandler::scanSubStationAssets(std::map<LONG_PTR, SWITCH_MAP*> & mapofSwitches)
{	
	std::map<LONG_PTR, ASSET_VECTOR*> mapOfSubStns;
	std::map<NSSTRING, ASSET_VECTOR*>::iterator assetsItr;
	std::map<NSSTRING, ASSET_VECTOR*>::iterator assetsSubSwicthesItr;
	std::map<LONG_PTR, int> mapOfMatchingSwitches;
	std::map<LONG_PTR, int> mapOfInvalidSubSwitches;
	std::map<LONG_PTR, int>::iterator InvalidSubSwitchItr;

	// validate all subswitches first for mandatory attributes
	assetsSubSwicthesItr = m_mapOfAssets.find(_T("SUBSWITCH"));
	if(assetsSubSwicthesItr != m_mapOfAssets.end())
	{
		ASSET_VECTOR* subSwitchesAssets = (*assetsSubSwicthesItr).second;
		ASSET_VECTOR::iterator subSwitchesItr;
		for(subSwitchesItr = subSwitchesAssets->begin(); subSwitchesItr != subSwitchesAssets->end(); subSwitchesItr++)
		{
			CNSAsset* pSwitchAsset = (*subSwitchesItr);
			int nResult = validateAsset(pSwitchAsset, _T("SubSwitch"));
			if(nResult == NS_FAIL)
			{
				mapOfInvalidSubSwitches.insert(std::map<LONG_PTR, int>::value_type(pSwitchAsset->getObjectID(), 1));
			}
		}
	}

	assetsItr = m_mapOfAssets.find(_T("SUBSTATION"));
	if(assetsItr != m_mapOfAssets.end())
	{
		ASSET_VECTOR* subStnAssets = (*assetsItr).second;
		ASSET_VECTOR::iterator subStnItr;
		for(subStnItr = subStnAssets->begin(); subStnItr != subStnAssets->end(); subStnItr++)
		{
			CNSAsset* pAsset = (*subStnItr);
			std::map<NSSTRING, NSSTRING> mapofattributes;
			std::map<NSSTRING, NSSTRING>::iterator attributesItr;
			m_LocalDWGObj.readBlockAttributes(pAsset->getObjectID(), mapofattributes);
			attributesItr = mapofattributes.find(_T("SUBSTN_NUMBER"));
			if(attributesItr != mapofattributes.end())
			{
				NSSTRING strSubStnNo = (*attributesItr).second;
				assetsItr = m_mapOfAssets.find(_T("SUBSWITCH"));
				if(assetsItr != m_mapOfAssets.end())
				{
					ASSET_VECTOR* pSubSwitchVect = (*assetsItr).second;
					ASSET_VECTOR::iterator subSwitchItr;
					for(subSwitchItr = pSubSwitchVect->begin(); subSwitchItr != pSubSwitchVect->end(); subSwitchItr++)
					{
						CNSAsset* pSubSwitch = (*subSwitchItr);

						//If it invalid subswitch don't add send to ellipse
						InvalidSubSwitchItr =  mapOfInvalidSubSwitches.find(pSubSwitch->getObjectID());
						if(InvalidSubSwitchItr != mapOfInvalidSubSwitches.end())
							continue;

						std::map<NSSTRING, NSSTRING> mapOfSwitchAtts;
						std::map<NSSTRING, NSSTRING>::iterator switchAttItr;
						m_LocalDWGObj.readBlockAttributes(pSubSwitch->getObjectID(), mapOfSwitchAtts);
						switchAttItr = mapOfSwitchAtts.find(_T("SUBSTN_NUMBER"));
						if(switchAttItr != mapOfSwitchAtts.end())
						{
							NSSTRING strSwitchSubStnNo = (*switchAttItr).second;
							if(NSSTRCMP((TCHAR*)strSwitchSubStnNo.c_str(), (TCHAR*)strSubStnNo.c_str()) == 0)
							{
								std::map<LONG_PTR, ASSET_VECTOR*>::iterator subStnItr;
								subStnItr = mapOfSubStns.find(pAsset->getObjectID());
								if(subStnItr != mapOfSubStns.end())
								{
									ASSET_VECTOR* pSwitchVect = (*subStnItr).second;
									pSwitchVect->push_back(pSubSwitch);
								}
								else
								{
									ASSET_VECTOR* switchVect = new ASSET_VECTOR;
									switchVect->push_back(pSubSwitch);
									mapOfSubStns.insert(std::map<LONG_PTR, ASSET_VECTOR*>::value_type(pAsset->getObjectID(), switchVect));
								}
								mapOfMatchingSwitches.insert(std::map<LONG_PTR, int>::value_type(pSubSwitch->getObjectID(), 0));
							}
						}
						//else
						//{
						//	//For Switch the SUBSTN_NUMBER attribute is mandatory, report this error
						//	TCHAR szMsg[_MAX_PATH + 1];
						//	NSSPRINTF(szMsg, "The mandatory attribute \'SUBSTN_NUMBER\' not found for asset = \'%s\' on layer = \'%s\'", pAsset->getBlockName().c_str(),pAsset->getLayerName().c_str());
						//	WriteToLog(LOG_HEADER, szMsg);
						//}
					}
				}
			}
			//This substation is not added, must send substation info to Ellipse
			
			std::map<LONG_PTR, ASSET_VECTOR*>::iterator tempIter;
			tempIter = mapOfSubStns.find(pAsset->getObjectID());
			if(tempIter == mapOfSubStns.end())
			{
				ASSET_VECTOR* switchVect = new ASSET_VECTOR;
				mapOfSubStns.insert(std::map<LONG_PTR, ASSET_VECTOR*>::value_type(pAsset->getObjectID(), switchVect));
			}
		}
	}
	std::map<LONG_PTR, ASSET_VECTOR*>::iterator subStnItr;
	for(subStnItr = mapOfSubStns.begin(); subStnItr != mapOfSubStns.end(); subStnItr++)
	{
		SWITCH_MAP* pSwitchMap = new SWITCH_MAP;
		SWITCH_MAP::iterator switchMapItr;
		LONG_PTR lSubStnID = (*subStnItr).first;
		ASSET_VECTOR* switchVect = (*subStnItr).second;
		ASSET_VECTOR::iterator switchItr;
		for(switchItr = switchVect->begin(); switchItr != switchVect->end(); switchItr++)
		{
			CNSAsset *pSwitch = (*switchItr);
			std::map<NSSTRING, NSSTRING> mapofblkAtts;
			std::map<NSSTRING, NSSTRING>::iterator attrItr;
		
			//	NSSTRING strSwitchType = (*attrItr).second;
				NSSTRING strSwitchType;
				if(NS_FAIL ==  m_LocalDWGObj.getBlockName(pSwitch->getObjectID(), strSwitchType))
					continue;
				m_LocalDWGObj.readBlockAttributes(pSwitch->getObjectID(), mapofblkAtts);
				////attrItr = mapofblkAtts.find("SWITCH_TYPE");
				attrItr = mapofblkAtts.find(_T("EQUIP_TYPE"));			
				if(attrItr != mapofblkAtts.end())
				{
					m_mapOfSwitchType.insert(std::map<LONG_PTR, NSSTRING>::value_type(pSwitch->getObjectID(),  (*attrItr).second));
				}
				switchMapItr = pSwitchMap->find(strSwitchType);
				if(switchMapItr != pSwitchMap->end())
				{
					OBJECTID_VECTOR* pObjectIDVect = (*switchMapItr).second;
					pObjectIDVect->push_back(pSwitch->getObjectID());
				}
				else
				{
					OBJECTID_VECTOR* pObjVect = new OBJECTID_VECTOR;
					pObjVect->push_back(pSwitch->getObjectID());
					pSwitchMap->insert(SWITCH_MAP::value_type(strSwitchType, pObjVect));
				}

			/*}*/
		}
		mapofSwitches.insert(std::map<LONG_PTR, SWITCH_MAP*>::value_type(lSubStnID, pSwitchMap));
	}

	// Check if switches have matching substation and if not report error.
	assetsItr = m_mapOfAssets.find(_T("SUBSWITCH"));
	if(assetsItr != m_mapOfAssets.end())
	{
		ASSET_VECTOR* pSubSwitchVect = (*assetsItr).second;
		ASSET_VECTOR::iterator subSwitchItr;
		for(subSwitchItr = pSubSwitchVect->begin(); subSwitchItr != pSubSwitchVect->end(); subSwitchItr++)
		{
			std::map<LONG_PTR, int>::iterator subMatchingSwitchItr;

			CNSAsset* pSubSwitch = (*subSwitchItr);
			//If it invalid subswitch don't add send to ellipse
			InvalidSubSwitchItr =  mapOfInvalidSubSwitches.find(pSubSwitch->getObjectID());
			if(InvalidSubSwitchItr != mapOfInvalidSubSwitches.end())
					continue;

			subMatchingSwitchItr = mapOfMatchingSwitches.find(pSubSwitch->getObjectID());
			if(subMatchingSwitchItr == mapOfMatchingSwitches.end())
			{
				TCHAR szMsg[_MAX_PATH + 1];
				NSSPRINTF(szMsg, _T("No matching \'SUBSTN_NUMBER\' found for subswitch = \'%s\' on layer = \'%s\'"), pSubSwitch->getBlockName().c_str(),pSubSwitch->getLayerName().c_str());
				WriteToLog(LOG_OTHER, szMsg);
			}
			
		}
	}

	return NS_SUCCESS;
}

int CNSAssetHandler::scanServicePillars(std::map<LONG_PTR, ASSET_VECTOR*> & mapOfPillars)
{
	std::map<NSSTRING, ASSET_VECTOR*>::iterator assetsItr;
	std::map<LONG_PTR, int> mapOfMatchingServicePillars;
	std::map<NSSTRING, ASSET_VECTOR*>::iterator assetsServicePillarsItr;
	std::map<LONG_PTR, int> mapOfInvalidServicePillars;
	std::map<LONG_PTR, int>::iterator InvalidServicePillarsItr;

	// validate all service pillars first for mandatory attributes
	assetsServicePillarsItr = m_mapOfAssets.find(_T("LVSERVICEPILLAR"));
	if(assetsServicePillarsItr != m_mapOfAssets.end())
	{
		ASSET_VECTOR* subServicePillarAssets = (*assetsServicePillarsItr).second;
		ASSET_VECTOR::iterator servicePillarItr;
		for(servicePillarItr = subServicePillarAssets->begin(); servicePillarItr != subServicePillarAssets->end(); servicePillarItr++)
		{
			CNSAsset* pServicePillarAsset = (*servicePillarItr);
			int nResult = validateAsset(pServicePillarAsset, _T("LVServicePillar"));
			if(nResult == NS_FAIL)
			{
				mapOfInvalidServicePillars.insert(std::map<LONG_PTR, int>::value_type(pServicePillarAsset->getObjectID(), 1));
			}
		}
	}

	assetsItr = m_mapOfAssets.find(_T("LVPILLAR"));
	if(assetsItr != m_mapOfAssets.end())
	{
		ASSET_VECTOR* lvPillarAssets = (*assetsItr).second;
		ASSET_VECTOR::iterator lvPillarAssetsItr;
		for(lvPillarAssetsItr = lvPillarAssets->begin(); lvPillarAssetsItr != lvPillarAssets->end(); lvPillarAssetsItr++)
		{
			CNSAsset* pAsset = (*lvPillarAssetsItr);
			std::map<NSSTRING, NSSTRING> mapofattributes;
			std::map<NSSTRING, NSSTRING>::iterator attributesItr;
			m_LocalDWGObj.readBlockAttributes(pAsset->getObjectID(), mapofattributes);
			attributesItr = mapofattributes.find(_T("PILLAR_NUMBER"));
			if(attributesItr != mapofattributes.end())
			{
				NSSTRING strPillarNo = (*attributesItr).second;
				assetsItr = m_mapOfAssets.find(_T("LVSERVICEPILLAR"));
				if(assetsItr != m_mapOfAssets.end())
				{
					ASSET_VECTOR* pServicePillarVect = (*assetsItr).second;
					ASSET_VECTOR::iterator servicePillarVectItr;
					for(servicePillarVectItr = pServicePillarVect->begin(); servicePillarVectItr != pServicePillarVect->end(); servicePillarVectItr++)
					{
						CNSAsset* pServicePillar = (*servicePillarVectItr);
						//If it invalid ServicePillar don't add send to ellipse
						InvalidServicePillarsItr =  mapOfInvalidServicePillars.find(pServicePillar->getObjectID());
						if(InvalidServicePillarsItr != mapOfInvalidServicePillars.end())
							continue;
						std::map<NSSTRING, NSSTRING> mapOfServicePillarAtts;
						std::map<NSSTRING, NSSTRING>::iterator servicePillarItr;
						m_LocalDWGObj.readBlockAttributes(pServicePillar->getObjectID(), mapOfServicePillarAtts);
						servicePillarItr = mapOfServicePillarAtts.find(_T("PARENT"));
						if(servicePillarItr != mapOfServicePillarAtts.end())
						{
							NSSTRING strParentNo = (*servicePillarItr).second;
							if(NSSTRCMP((TCHAR*)strParentNo.c_str(), (TCHAR*)strPillarNo.c_str()) == 0)
							{
								std::map<LONG_PTR, ASSET_VECTOR*>::iterator pillarsItr;
								pillarsItr = mapOfPillars.find(pAsset->getObjectID());
								if(pillarsItr != mapOfPillars.end())
								{
									ASSET_VECTOR* pServiceVect = (*pillarsItr).second;
									pServiceVect->push_back(pServicePillar);
								}
								else
								{
									ASSET_VECTOR* pServiceVect = new ASSET_VECTOR;
									pServiceVect->push_back(pServicePillar);
									mapOfPillars.insert(std::map<LONG_PTR, ASSET_VECTOR*>::value_type(pAsset->getObjectID(), pServiceVect));
								}
								mapOfMatchingServicePillars.insert(std::map<LONG_PTR, int>::value_type(pServicePillar->getObjectID(), 0));
							}
						}
					}
				}
				else
				{
					ASSET_VECTOR* pVect = new ASSET_VECTOR;
					mapOfPillars.insert(std::map<LONG_PTR, ASSET_VECTOR*>::value_type(pAsset->getObjectID(), pVect));
				}
			}
		}

	}

	// Check if service pillars have matching pillars and if not report error.
	assetsItr = m_mapOfAssets.find(_T("LVSERVICEPILLAR"));
	if(assetsItr != m_mapOfAssets.end())
	{
		ASSET_VECTOR* pServicePillarVect = (*assetsItr).second;
		ASSET_VECTOR::iterator servicePillarItr;
		for(servicePillarItr = pServicePillarVect->begin(); servicePillarItr != pServicePillarVect->end(); servicePillarItr++)
		{
			std::map<LONG_PTR, int>::iterator subMatchingPillarItr;

			CNSAsset* pServicePillar = (*servicePillarItr);
			subMatchingPillarItr = mapOfMatchingServicePillars.find(pServicePillar->getObjectID());
			if(subMatchingPillarItr == mapOfMatchingServicePillars.end())
			{
				TCHAR szMsg[_MAX_PATH + 1];
				NSSPRINTF(szMsg, _T("No matching \'PARENT\' found for service pillar = \'%s\' on layer = \'%s\'"), pServicePillar->getBlockName().c_str(),pServicePillar->getLayerName().c_str());
				WriteToLog(LOG_OTHER, szMsg);
			}
			
		}
	}
	return NS_SUCCESS;
}

TCHAR* CNSAssetHandler::getValueFromMap(std::map<NSSTRING, NSSTRING> & mapOfAttributes, TCHAR* szKey)
{
	TCHAR* szValue = new TCHAR[_MAX_PATH + 1];
	//strcpy(szValue, _T(""));
	NSSTRCPY(szValue,(int)NSSTRLEN(_T(""))+1,_T(""));
	std::map<NSSTRING, NSSTRING>::iterator attributesItr;
	attributesItr = mapOfAttributes.find(szKey);
	if(attributesItr != mapOfAttributes.end())
		//strcpy(szValue, (TCHAR*)((*attributesItr).second).c_str());
		NSSTRCPY(szValue,(*attributesItr).second.size()+1,(TCHAR*)((*attributesItr).second).c_str());
	return szValue;
}

void CNSAssetHandler::generateAssetsForPillars(std::map<TCHAR*, TCHAR*> & mapofCredentials)
{
	std::map<LONG_PTR, ASSET_VECTOR*>::iterator pillarsItr;
	mapOfPillars.clear();
	scanServicePillars(mapOfPillars);
	int Size = (int)mapOfPillars.size();
	for(pillarsItr = mapOfPillars.begin(); pillarsItr != mapOfPillars.end(); pillarsItr++)
	{
		std::map<NSSTRING, NSSTRING> mapofAttributes;
		NSSTRING strMainPillarAssetNo = _T("");
		LONG_PTR pillarObjID = (*pillarsItr).first;
		CNSAsset *pAsset = new CNSAsset;
		pAsset->setObjectID(pillarObjID);
		m_LocalDWGObj.readBlockAttributes(pillarObjID, mapofAttributes);
		pAsset->m_mapOfAttributes = mapofAttributes;
		int nResult = validateAsset(pAsset, _T("LVPillar"));
		if(nResult == NS_FAIL)
		{
			/*TCHAR szInfo[_MAX_PATH + 1];
			NSSPRINTF(szInfo, "Invalid Asset found. Block Name = %s, FNO = %d ", (TCHAR*)pAsset->getBlockName().c_str(), pAsset->getFNO());
			WriteToLog(LOG_WARNINGS, "Invalid Asset found of type LVPillar");*/
			SetErrorsOccuredFlag(true);
			delete pAsset;
			pAsset = NULL;
			continue;
		}
		//AK START -- Check if this substn already has asset number
		NSSTRING strAssetNumber =_T("");
		getAttributeValue(_T("ASSET_NUM"),strAssetNumber,pAsset->m_mapOfAttributes);

		if(NSSTRCMP(strAssetNumber.c_str(), _T("")) == 0)
		{
			ASSET_VECTOR* pSPillarsVect = (*pillarsItr).second;
			std::vector<TCHAR*> replyVect;
			setReplyParameters(replyVect);
			std::map<TCHAR*, TCHAR*> mapOfRequest;
			setRequestParameters(mapOfRequest, _T("LVPillar"), pAsset);
			int iRet = prepareSOAPMessage(mapofCredentials, replyVect, mapOfRequest);
			if(iRet == NS_FAIL)
			{
				delete pAsset;
				pAsset = NULL;
				continue;
			}
			std::vector<CNSDataItem*> dataItemsVect;
			std::vector<CNSDataItem*>::iterator dataItemItr;
			std::vector<TCHAR*> vecErrors;

			TCHAR* szPlatNumber = m_ellipseConnector.getResult(dataItemsVect,vecErrors);
			if(NS_FAIL == processErrMessages(vecErrors))
				 return;
			TCHAR* szMainPillarEquip = new TCHAR[_MAX_PATH + 1];
			//strcpy(szMainPillarEquip, _T(""));
            NSSTRCPY(szMainPillarEquip,_MAX_PATH + 1,_T(""));
			for(dataItemItr = dataItemsVect.begin(); dataItemItr != dataItemsVect.end(); dataItemItr++)
			{
				CNSDataItem* pDataItem = (*dataItemItr);
				if(pDataItem == NULL)
				{
					return;
				}
			/*	TCHAR* szPlantNo = pDataItem->getValueFromMap("PlantNo");
				strMainPillarAssetNo = getPlantNo(szPlantNo);
				int iRet = m_LocalDWGObj.setAssetNumber(pAsset->getObjectID(), (TCHAR*)strMainPillarAssetNo.c_str());
				if(iRet != -1)
				{
					TCHAR szMsg[_MAX_PATH + 1];
					NSSPRINTF(szMsg, "  %s                          %s", "LV Pillar", (TCHAR*)strMainPillarAssetNo.c_str());
					WriteToLog(LOG_HEADER, szMsg);
					mapOfNumAssets.insert(std::map<long,int>::value_type(pAsset->getObjectID(), 1));
				}
				else
				{
					TCHAR szMsg[_MAX_PATH + 1];
					NSSPRINTF(szMsg, "Failed to add asset number for Asset type = %s" , "LV Pillar");
					WriteToLog(LOG_HEADER, szMsg);
					SetErrorsOccuredFlag(true);
				}
				TCHAR* szEquip = pDataItem->getValueFromMap("EquipNo");
				strcpy(szMainPillarEquip, szEquip);*/
				TCHAR* szEquip = pDataItem->getValueFromMap((TCHAR*)_T("EquipNo"));
				NSSTRING strTemp = szEquip;
				//NSSTRCPY(szMainPillarEquip, szEquip);
				NSSTRCPY(szMainPillarEquip,strTemp.size()+1 ,szEquip);
				m_mapOfEquipNoIDs.insert(std::map<LONG_PTR, NSSTRING>::value_type(pAsset->getObjectID(), szMainPillarEquip));
				delete []szEquip;

			}
			// Link Project if AssetType is MainPillar
			setLinkToProject(mapofCredentials, szMainPillarEquip);
		/*	delete[] szMainPillarEquip;
			szMainPillarEquip = NULL;*/
		}
		else
		{
			mapOfNumAssets.insert(std::map<LONG_PTR,NSSTRING>::value_type(pAsset->getObjectID(), strAssetNumber));
			strMainPillarAssetNo = strAssetNumber;
		}
		//AK END
	//		ASSET_VECTOR* pServicePillarVect = (*pillarsItr).second;
	//		ASSET_VECTOR::iterator serviceItr;
	//		for(serviceItr = pServicePillarVect->begin(); serviceItr != pServicePillarVect->end(); serviceItr++)
	//		{

	//			CNSAsset* pServicePillar = (*serviceItr);
	//			int nResult = validateAsset(pServicePillar, "LVServicePillar", false);
	//			if(nResult == NS_FAIL)
	//			{
	//				TCHAR szInfo[_MAX_PATH + 1];
	//				NSSPRINTF(szInfo, "Invalid Asset found. Block Name = %s, FNO = %d ", (TCHAR*)(pServicePillar->getBlockName().c_str()), pServicePillar->getFNO());
	//				WriteToLog(LOG_WARNINGS, szInfo);
	//				SetErrorsOccuredFlag(true);
	//				continue;
	//			}	
	//			std::vector<TCHAR*> replyVect;
	//			setReplyParameters(replyVect);
	//			std::map<TCHAR*, TCHAR*> mapOfRequest;
	//			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type("PlantCode1", (TCHAR*)strMainPillarAssetNo.c_str()));
	//			setRequestParameters(mapOfRequest, "LVServicePillar", pServicePillar);
	//			int iResult = prepareSOAPMessage(mapofCredentials, replyVect, mapOfRequest);
	//			if(iResult == NS_FAIL)
	//				continue;
	//			std::vector<CNSDataItem*> dataItemsVect;
	//			std::vector<CNSDataItem*>::iterator dataItemItr;
	//			TCHAR* szPlatNumber = m_ellipseConnector.getResult(dataItemsVect);
	//			TCHAR* szSPillarEquipNo = new TCHAR[_MAX_PATH + 1];
	//			strcpy(szSPillarEquipNo, "");
	//			for(dataItemItr = dataItemsVect.begin(); dataItemItr != dataItemsVect.end(); dataItemItr++)
	//			{
	//				CNSDataItem* pDataItem = (*dataItemItr);
	//				if(pDataItem == NULL)
	//				{
	//					delete pAsset;
	//					pAsset = NULL;
	//					return;
	//				}
	//				TCHAR* szPlantNo = pDataItem->getValueFromMap("PlantNo");
	//				NSSTRING strServiceAssetNo = getPlantNo(szPlantNo);
	//				int iRet = m_LocalDWGObj.setAssetNumber(pServicePillar->getObjectID(), (TCHAR*)strServiceAssetNo.c_str());
	//				if(iRet != -1)
	//				{
	//					TCHAR szMsg[_MAX_PATH + 1];
	//					NSSPRINTF(szMsg, "  %s             %d             %s", "LV Service Pillar", pServicePillar->getFNO(), (TCHAR*)strServiceAssetNo.c_str());
	//					WriteToLog(LOG_HEADER, szMsg);
	//				}
	//				else
	//				{
	//					TCHAR szMsg[_MAX_PATH + 1];
	//					NSSPRINTF(szMsg, "Failed to add asset number for Asset type = %s FNO = %d ", "LV Service Pillar", pServicePillar->getFNO());
	//					WriteToLog(LOG_HEADER, szMsg);
	//					SetErrorsOccuredFlag(true);
	//				}
	//				TCHAR* szEquip = pDataItem->getValueFromMap("EquipNo");
	//				strcpy(szSPillarEquipNo, szEquip);
	//			}

	//			// Link Project if AssetType is ServicePillar
	//			setLinkToProject(mapofCredentials, szSPillarEquipNo);
	//			delete[] szSPillarEquipNo;
	//			szSPillarEquipNo = NULL;
	//		}
			delete pAsset;
			pAsset = NULL;
	//	//}
	}
	//std::map<long, ASSET_VECTOR*>::iterator pillarsIter;
	//for(pillarsIter = mapOfPillars.begin(); pillarsIter != mapOfPillars.end(); pillarsIter++)
	//{
	//	ASSET_VECTOR* pVect = (*pillarsIter).second;
	//	ASSET_VECTOR::iterator it;
	//	/*for(it = pVect->begin(); it != pVect->end(); it++)
	//	{
	//		CNSAsset *pAsset = (*it);
	//		delete pAsset;
	//		pAsset = NULL;
	//	}*/
	//	pVect->clear();
	//	delete pVect;
	//	pVect = NULL;
	//}
	//mapOfPillars.clear();
}

NSSTRING CNSAssetHandler::getPlantNo(TCHAR* szPlantNo)
{
	NSSTRING strPlantNo = _T("");
	NSSTRING strAssetNo = szPlantNo;
	if(strAssetNo.length() > 2)
		strPlantNo = strAssetNo.substr(2, strAssetNo.length() - 1);
	return strPlantNo;
}

int CNSAssetHandler::getAttributeValue(NSSTRING pszKey, NSSTRING& pszValue, std::map<NSSTRING, NSSTRING> &mapofAttributes)
{
	std::map<NSSTRING, NSSTRING>::iterator itrMap;

	for(itrMap = mapofAttributes.begin();itrMap != mapofAttributes.end();itrMap++)
	{
		if(NSSTRCMP((*itrMap).first.c_str(),pszKey.c_str())==0)
		{
			pszValue = (*itrMap).second;
			return NS_SUCCESS;
		}
	}
	return NS_FAIL;
}

void CNSAssetHandler::generateAssetsForSwitches(std::map<TCHAR*, TCHAR*> & mapofCredentials)
{
	//std::map<long, SWITCH_MAP*> mapofSwitches;
	//mapofSwitches.clear();
	std::map<LONG_PTR, SWITCH_MAP*>::iterator switchItr;
	scanSubStationAssets(mapofSwitches);
	int noOfSubstns = (int)mapofSwitches.size();
	if(noOfSubstns == 0)
	{
		//WriteToLog(LOG_WARNINGS, "No substation found for switch");
		//SetErrorsOccuredFlag(true);
	}
	for(switchItr = mapofSwitches.begin(); switchItr != mapofSwitches.end(); switchItr++)
	{
		std::map<NSSTRING, NSSTRING> mapofAttributes;
		// Creating asset for Substation
		NSSTRING strSubStnAssNo = _T("");
		LONG_PTR lSubStnID = (*switchItr).first;
		CNSAsset* pSubStnAsset = new CNSAsset;
		m_LocalDWGObj.readBlockAttributes(lSubStnID, mapofAttributes);
		pSubStnAsset->setObjectID(lSubStnID);
		pSubStnAsset->m_mapOfAttributes = mapofAttributes;
		//int nResult = validateAsset(pSubStnAsset, "LVServicePillar");
		int nResult = validateAsset(pSubStnAsset, _T("Substation"));
		if(nResult == NS_FAIL)
		{
			//TCHAR szInfo[_MAX_PATH + 1];
			//NSSPRINTF(szInfo, "Invalid Asset found. Block Name = %s, FNO = %d ", (TCHAR*)pServicePillar->getBlockName().c_str(), pServicePillar->getFNO());
			//WriteToLog(LOG_WARNINGS, "Invalid Asset found of type Substation");
			SetErrorsOccuredFlag(true);
			continue;
		}
		//AK START -- Check if this substn already has asset number
		NSSTRING strAssetNumber =_T("");
		getAttributeValue(_T("ASSET_NUM"),strAssetNumber,pSubStnAsset->m_mapOfAttributes);
		std::vector<TCHAR*> replyVect;

		if(NSSTRCMP(strAssetNumber.c_str(), _T("")) == 0)
		{
			setReplyParameters(replyVect);
			std::map<TCHAR*, TCHAR*> mapOfRequest;
			setRequestParameters(mapOfRequest, _T("Substation"), pSubStnAsset);
			int iRet = prepareSOAPMessage(mapofCredentials, replyVect, mapOfRequest);
			if(iRet == NS_FAIL)
				continue;
			std::vector<CNSDataItem*> dataItemsVect;
			std::vector<CNSDataItem*>::iterator dataItemItr;
			std::vector<TCHAR*> vecErrors;
			TCHAR* szPlatNumber = m_ellipseConnector.getResult(dataItemsVect,vecErrors);
			if(NS_FAIL == processErrMessages(vecErrors))
				 return;
			TCHAR* szSubEquipNo = new TCHAR[_MAX_PATH + 1];
			//strcpy(szSubEquipNo, "");
			NSSTRCPY(szSubEquipNo,(int)NSSTRLEN(_T("")),_T(""));
			for(dataItemItr = dataItemsVect.begin(); dataItemItr != dataItemsVect.end(); dataItemItr++)
			{
				CNSDataItem* pDataItem = (*dataItemItr);
				if(pDataItem == NULL)
				{
					return;
				}
			/*	TCHAR* szPlantNo = pDataItem->getValueFromMap("PlantNo");
				strSubStnAssNo = getPlantNo(szPlantNo);
				int iRet = m_LocalDWGObj.setAssetNumber(pSubStnAsset->getObjectID(), (TCHAR*)strSubStnAssNo.c_str());
				if(iRet != -1)
				{
					TCHAR szMsg[_MAX_PATH + 1];
					NSSPRINTF(szMsg, "  %s                          %s", "SubStation", (TCHAR*)strSubStnAssNo.c_str());
					WriteToLog(LOG_HEADER, szMsg);
					mapOfNumAssets.insert(std::map<long,int>::value_type(pSubStnAsset->getObjectID(), 1));
				}
				else
				{
					TCHAR szMsg[_MAX_PATH + 1];
					NSSPRINTF(szMsg, "Failed to add asset number for Asset type = %s", "Sub Station");
					WriteToLog(LOG_HEADER, szMsg);
					SetErrorsOccuredFlag(true);
				}
				TCHAR* szEquip = pDataItem->getValueFromMap("EquipNo");
				strcpy(szSubEquipNo, szEquip);*/
				TCHAR* szEquipNo = pDataItem->getValueFromMap((TCHAR*)_T("EquipNo"));
				//NSSTRCPY(szSubEquipNo, ,szEquipNo);
				NSSTRCPY(szSubEquipNo,(int)NSSTRLEN(szEquipNo) ,szEquipNo);
				m_mapOfEquipNoIDs.insert(std::map<LONG_PTR, NSSTRING>::value_type(pSubStnAsset->getObjectID(), szSubEquipNo));
				delete []szEquipNo;

			}

			// Link Project if AssetType is ServicePillar
			setLinkToProject(mapofCredentials, szSubEquipNo);
			//delete[] szSubEquipNo;
			//szSubEquipNo = NULL;
		}
		else
		{
			//mapOfNumAssets.insert(std::map<long,int>::value_type(pSubStnAsset->getObjectID(), 1));
			mapOfNumAssets.insert(std::map<LONG_PTR,NSSTRING>::value_type(pSubStnAsset->getObjectID(), strAssetNumber));
            strSubStnAssNo = strAssetNumber;
		}
		//AK END

		/*SWITCH_MAP* switchMap = (*switchItr).second;
		SWITCH_MAP::iterator itA;
		for(itA = switchMap->begin(); itA != switchMap->end(); itA++)
		{
			// Create Assets for SubSwitches
			NSSTRING strSwitchType = (*itA).first;
			
			OBJECTID_VECTOR* pObjIdVect = (*itA).second;
			int nSize = (int)pObjIdVect->size();
			TCHAR szPlantCode1[_MAX_PATH + 1];
			strcpy(szPlantCode1, (TCHAR*)strSubStnAssNo.c_str());
			TCHAR szPlantCode2[_MAX_PATH + 1];
			strcpy(szPlantCode2, (TCHAR*)strSwitchType.c_str());
			TCHAR szPlantCode3[_MAX_PATH + 1];
			strcpy(szPlantCode3, "-01");
			TCHAR szEquipGrpID[_MAX_PATH + 1];
			NSSTRING strEquipGrpID = getEquipGrpID((TCHAR*)strSwitchType.c_str());
			strcpy(szEquipGrpID, (TCHAR*)strEquipGrpID.c_str());
			TCHAR szEquipRef[_MAX_PATH + 1];
			strcpy(szEquipRef, "DS");
			strcat(szEquipRef, szPlantCode1);
			TCHAR szOpIdInstance[_MAX_PATH + 1];
			TCHAR szSize[_MAX_PATH + 1];
			strcpy(szOpIdInstance, itoa(nSize, szSize, 10));
			std::map<TCHAR*, TCHAR*> mapOfRequest;
			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type("PlantCode1", szPlantCode1));
			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type("PlantCode2", szPlantCode2));
			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type("PlantCode3", szPlantCode3));
			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type("EquipGrpId", szEquipGrpID));
			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type("EquipRef", szEquipRef));
			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type("OpIdInstance", szOpIdInstance));
			setRequestParameters(mapOfRequest, "SubSwitch", pSubStnAsset);
			int iRet = prepareSOAPMessage(mapofCredentials, replyVect, mapOfRequest);
			if(iRet == NS_FAIL)
				continue;
			std::vector<CNSDataItem*> dataItemsVect;
			std::vector<CNSDataItem*>::iterator dataItemItr;
			TCHAR* szPlatNumber = m_ellipseConnector.getResult(dataItemsVect);
			//TCHAR* szSwitchAsset = new TCHAR[_MAX_PATH + 1];
			TCHAR* szSubSwitchEquipNo = new TCHAR[_MAX_PATH + 1];
			strcpy(szSubSwitchEquipNo, "");
			for(dataItemItr = dataItemsVect.begin(); dataItemItr != dataItemsVect.end(); dataItemItr++)
			{
				CNSDataItem* pDataItem = (*dataItemItr);
				if(pDataItem == NULL)
				{
					return;
				}
				TCHAR* szPlantNo = pDataItem->getValueFromMap("PlantNo");
				NSSTRING strsubSwitchAssetNo = getPlantNo(szPlantNo);
				//strcpy(szSwitchAsset, szPlantNo);
				
				OBJECTID_VECTOR::iterator it10;
				TCHAR* szEquip = pDataItem->getValueFromMap("EquipNo");
				strcpy(szSubSwitchEquipNo, szEquip);
				for(it10 = pObjIdVect->begin(); it10 != pObjIdVect->end(); it10++)
				{
					std::map<NSSTRING, NSSTRING> mapofAttributes;
					long objID = (*it10);
					CNSAsset *pAsset = new CNSAsset;
					pAsset->setObjectID(objID);
					m_LocalDWGObj.readBlockAttributes(lSubStnID, mapofAttributes);
					pAsset->m_mapOfAttributes = mapofAttributes;
					int nResult = validateAsset(pAsset, "SubSwitch", false);
					if(nResult == NS_FAIL)
					{
						//TCHAR szInfo[_MAX_PATH + 1];
						//NSSPRINTF(szInfo, "Invalid Asset found. Block Name = %s, FNO = %d ", (TCHAR*)pServicePillar->getBlockName().c_str(), pServicePillar->getFNO());
						//WriteToLog(LOG_WARNINGS, "Invalid Asset found of type SubSwitch");
						SetErrorsOccuredFlag(true);
						continue;
					}	
					int iRet = m_LocalDWGObj.setAssetNumber(objID, (TCHAR*)strsubSwitchAssetNo.c_str());
					if(iRet != -1)
					{
						TCHAR szMsg[_MAX_PATH + 1];
						NSSPRINTF(szMsg, "  %s                          %s", "Sub Switch", (TCHAR*)strsubSwitchAssetNo.c_str());
						WriteToLog(LOG_HEADER, szMsg);
						mapOfNumAssets.insert(std::map<long,int>::value_type(objID, 1));
					}
					else
					{
						TCHAR szMsg[_MAX_PATH + 1];
						NSSPRINTF(szMsg, "Failed to add asset number for Asset type = %s ", "Sub Switch");
						WriteToLog(LOG_HEADER, szMsg);
						SetErrorsOccuredFlag(true);
					}
					delete pAsset;
					pAsset = NULL;

				}
			}

			// Link Project if AssetType is ServicePillar
			setLinkToProject(mapofCredentials, szSubSwitchEquipNo);
			delete[] szSubSwitchEquipNo;
			szSubSwitchEquipNo = NULL;
		}*/
		delete pSubStnAsset;
		pSubStnAsset = NULL;
	}

	/*for(switchItr = mapofSwitches.begin(); switchItr != mapofSwitches.end(); switchItr++)
	{
		SWITCH_MAP* pMap = (*switchItr).second;
		SWITCH_MAP::iterator it;
		for(it = pMap->begin(); it != pMap->end(); it++)
		{
			OBJECTID_VECTOR* pVect = (*it).second;
			delete pVect;
			pVect = NULL;
		}
		delete pMap;
		pMap = NULL;
	}*/

}

NSSTRING CNSAssetHandler::getEquipGrpID(TCHAR* szSwitchType)
{
	CNSDatabaseMgr* pDatabase = CNSDatabaseMgr::getInstance();
	int iRet = pDatabase->openDatabase(_T(""), _T(""), _T("GISCAD"));
	if(iRet == NS_FAIL)
		return _T("");
	TCHAR szQuery[_MAX_PATH + 1];
	NSSTRING strEquipGrpID;
	NSSPRINTF(szQuery, _T("select EquipGrpId from SwitchType where SwitchType = \'%s\'"), szSwitchType);
	if(-1 != pDatabase->executeQuery(szQuery))
	{
		variant_t vEquipGrpID;
		bool bRet = pDatabase->getValue((TCHAR*)_T("EquipGrpId"), vEquipGrpID);
		if(bRet != false)
		{
			strEquipGrpID = (NSSTRING)((_bstr_t)(vEquipGrpID.bstrVal));
		}
	}
	pDatabase->closeDatabase();
	return strEquipGrpID;
}

NSSTRING CNSAssetHandler::getQueryString(NSSTRING strQueryString)
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

NSSTRING CNSAssetHandler::getQueryString(CNSAsset* pAsset, TCHAR szQuery[_MAX_PATH])
{
	NSSTRING strQuery(szQuery);
	NSSTRING strSubType, strSwitchType, strCCPType, strLanthrnType, strActualStatus;
	int iRetCode;
	iRetCode = pAsset->getAttributeValue(_T("SUBSTATION_TYPE"), strSubType);
	if(iRetCode != NS_FAIL)
	{
		strSubType = getQueryString(strSubType);
		TCHAR szTemp[_MAX_PATH + 1];
		NSSPRINTF(szTemp, _T(" and SUBSTATION_TYPE %s"), strSubType.c_str());
		strQuery.append(szTemp);
	}
	iRetCode = pAsset->getAttributeValue(_T("SWITCH_TYPE"), strSwitchType);
	if(iRetCode != NS_FAIL)
	{
		strSwitchType = getQueryString(strSwitchType);
		TCHAR szTemp[_MAX_PATH + 1];
		NSSPRINTF(szTemp, _T(" and SWITCH_TYPE %s"), strSwitchType.c_str());
		strQuery.append(szTemp);
	}
	iRetCode = pAsset->getAttributeValue(_T("CCP_TYPE"), strCCPType);
	if(iRetCode != NS_FAIL)
	{
		strCCPType = getQueryString(strCCPType);
		TCHAR szTemp[_MAX_PATH + 1];
		NSSPRINTF(szTemp, _T(" and CCP_TYPE %s"), strCCPType.c_str());
		strQuery.append(szTemp);
	}
	iRetCode = pAsset->getAttributeValue(_T("LANTERN_TYPE"), strLanthrnType);
	if(iRetCode != NS_FAIL)
	{
		strLanthrnType = getQueryString(strLanthrnType);
		TCHAR szTemp[_MAX_PATH + 1];
		NSSPRINTF(szTemp, _T(" and LANTERN_TYPE %s"), strLanthrnType.c_str());
		strQuery.append(szTemp);
	}
	iRetCode = pAsset->getAttributeValue(_T("ACTUAL_STATUS"), strActualStatus);
	if(iRetCode != NS_FAIL)
	{
		strActualStatus = getQueryString(strActualStatus);
		TCHAR szTemp[_MAX_PATH + 1];
		NSSPRINTF(szTemp, _T(" and ACTUAL_STATUS %s"), strActualStatus.c_str());
		strQuery.append(szTemp);
	}
	return strQuery;
}

bool CNSAssetHandler::IsValidDepotCode(NSSTRING szDepotCode)
{
	bool bFlag = true;
	if(!NSSTRCMP(szDepotCode.c_str(),_T("")))
		return false;
	CNSDatabaseMgr* pDatabase = CNSDatabaseMgr::getInstance();
	int iRet = pDatabase->openDatabase(_T(""), _T(""), _T("GISCAD"));
	if(iRet == NS_FAIL)
		return false;
	TCHAR szQuery[_MAX_PATH + 1];
	NSSTRING strOrgUnit;
	NSSPRINTF(szQuery, _T("select OrgUnit from OrgUnit where Depot = \'%s\'"), szDepotCode.c_str());
	if(-1 != pDatabase->executeQuery(szQuery))
	{
		variant_t vOrgUnit;
		bool bRet = pDatabase->getValue(_T("OrgUnit"), vOrgUnit);
		if(bRet != false)
		{
			strOrgUnit = (NSSTRING)((_bstr_t)(vOrgUnit.bstrVal));
		}
		else
			bFlag = false;
	}
	else
		bFlag = false;
    pDatabase->closeDatabase();

	return bFlag;
}

bool CNSAssetHandler::GetErrorsOccuredFlag()
{
	return m_ErrorsOccured;
}
void CNSAssetHandler::SetErrorsOccuredFlag(bool bFlag)
{
	m_ErrorsOccured = bFlag;
}

/*
Step 4 – Retrieving the Asset number from Ellipse

When equipment is created, Ellipse automatically populates a field referred to as the OP_ID. Generally this matches the PlantNo, but not always. Thus to correctly read the asset number the application needs to retrieve the OP_ID for each equipment created in ellipse.
To obtain the asset number, for each equipment created the following XML must be send to Ellipse:

  <Service name="EQUIPMENT.RetrieveAlternateRefs">
    <ReplyParameters>
      <Parameter name="EquipNo"/>
      <Parameter name="AlternateRef"/>
    </ReplyParameters>
    <Request id="ID00001">
			<Parameter name="EquipNo">EquipNo1</Parameter>
    </Request>
    <Request id="ID00002">
			<Parameter name="EquipNo"> EquipNo2</Parameter>
    </Request>
…
    <Request id="ID00099">
			<Parameter name="EquipNo"> EquipNo?</Parameter>
    </Request>
  </Service>
</EllipseRequest>
</SOAP-ENV:Body></SOAP-ENV:Envelope>

Where EquipNo1-? is the value returned when the equipment was created in Step 1 above.

This above query will return the following XML:

<?xml version="1.0" encoding="utf-8"?><soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:xsd="http://www.w3.org/1999/XMLSchema" xmlns:xsi="http://www.w3.org/1999/XMLSchema-instance">
<soapenv:Body>
<EllipseReply messageId="a_message_id" version="1.0">
<ServiceResponses>
<Response id="ID00003">
<DataItem>
<Parameter name="EquipNo">000001401160</Parameter>
<Parameter name="AlternateRef">OP_ID   Operating Number         SW069045                      001OP_ID   Operating Number         SW069046                      002</Parameter>
</DataItem>
</Response>
<Response id="ID00002">
<DataItem>
<Parameter name="EquipNo">000001401161</Parameter>
<Parameter name="AlternateRef">OP_ID   Operating Number         SW069047                      001</Parameter>
</DataItem>
</Response>
<Response id="ID00001">
<DataItem>
<Parameter name="EquipNo">000001401159</Parameter>
<Parameter name="AlternateRef">OP_ID   Operating Number         DS27122                       001</Parameter>
</DataItem>
</Response>
</ServiceResponses>
</EllipseReply>
</soapenv:Body>
</soapenv:Envelope>


The OP_ID is returned in the format:

OP_ID   Operating Number         SW069046                      001

In this example the value to be written back to the block as the asset number is 69046.
*/
int getNumberOfDigits(int nNumber)
{
	int nNumDigits = 0;
	if (nNumber < 0)
		nNumber = -nNumber; 
	while(nNumber > 0)
	{
		nNumDigits++;    
		nNumber /= 10;
	}
	return nNumDigits;
}

int CNSAssetHandler::assignOPIds(std::map<TCHAR*, TCHAR*> &mapofCredentials, STRING_MAP &mapOfValues, bool bGroupFlag)
{
	if(NS_SUCCESS != m_ellipseConnector.connect(m_UserCredentials.m_szHostID))
	{
		WriteToLog(LOG_OTHER, _T("Failed to connect to ellipse"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL; 
	}
	if(NS_SUCCESS != m_ellipseConnector.beginSOAPMessage(_T("process")))
	{
		WriteToLog(LOG_OTHER, _T("Failed to begin SOAP message"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.startRequest(_T("a_message_id"), _T("1.0"), _T("http://www.w3.org/2001/XMLSchema-instance"), 
		_T("H:\\Scada-ELL\\XSD\\EllipseRequest.xsd")))
	{
		WriteToLog(LOG_OTHER, _T("Failed in preparing Start Request"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.buildCredentials(mapofCredentials))
	{
		WriteToLog(LOG_OTHER, _T("Failed in build credentials"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.startService(_T("EQUIPMENT.RetrieveAlternateRefs")))
	{
		WriteToLog(LOG_OTHER, _T("Failed in start service"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}

	std::vector<TCHAR*> replyVect;
	replyVect.push_back(_T("EquipNo"));
	replyVect.push_back(_T("AlternateRef"));
	
	if(NS_SUCCESS != m_ellipseConnector.buildReply(replyVect))
	{
		WriteToLog(LOG_OTHER, _T("Failed in build reply"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}

	std::map< LONG_PTR, NSSTRING>::iterator EquipNoIDsItr;
	
	int nIDCntr = 0;
	for(EquipNoIDsItr = m_mapOfEquipNoIDs.begin(); EquipNoIDsItr != m_mapOfEquipNoIDs.end(); EquipNoIDsItr++)
	{
		NSSTRING strID = _T("ID");
		nIDCntr++;
		std::map<TCHAR*, TCHAR*> mapOfRequest;
		TCHAR strMessage[MAX_PATH + 1];
		int nDigits = getNumberOfDigits(nIDCntr);
		nDigits = 5 - nDigits;
		for(int nCntr = 0; nCntr < nDigits; nCntr++) strID.append(_T("0"));
		NSSPRINTF(strMessage, _T("%s%i"), strID.c_str(), nIDCntr);
		
		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipNo") , (TCHAR*)((*EquipNoIDsItr).second).c_str()));
		if(NS_SUCCESS != m_ellipseConnector.buildRequest(mapOfRequest, strMessage))
		{
			WriteToLog(LOG_OTHER, _T("Failed in build RequestParameters"));
			SetErrorsOccuredFlag(true);
		}
	}
	if(NS_SUCCESS != m_ellipseConnector.endElement())
	{
		WriteToLog(LOG_OTHER, _T("Failed in build End element in assignOPIds"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.endElement())
	{
		WriteToLog(LOG_OTHER, _T("Failed in build End element in assignOPIds"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}
	if(NS_SUCCESS != m_ellipseConnector.endSOAPMessage())
	{
		WriteToLog(LOG_OTHER, _T("Failed in end SOAP message in assignOPIds"));
		SetErrorsOccuredFlag(true);
		return NS_FAIL;
	}

	std::vector<CNSDataItem*> dataItemsVect;
	std::vector<CNSDataItem*>::iterator dataItemItr;
	std::vector<TCHAR*> vecErrors;

	TCHAR* szPlatNumber = m_ellipseConnector.getResult(dataItemsVect,vecErrors);
	if(NS_FAIL == processErrMessages(vecErrors))
			return NS_FAIL;
//	STRING_MAP mapOfValues;
	for(dataItemItr = dataItemsVect.begin(); dataItemItr != dataItemsVect.end(); dataItemItr++)
	{
		CNSDataItem* pDataItem = (*dataItemItr);
		TCHAR* szOPId = pDataItem->getValueFromMap((TCHAR*)_T("AlternateRef"));
		TCHAR* szEquipNo = pDataItem->getValueFromMap((TCHAR*)_T("EquipNo"));
		tokenize(szOPId, szEquipNo, mapOfValues, bGroupFlag);
	}
	return NS_SUCCESS;
}


int CNSAssetHandler::tokenize(NSSTRING strInputString, NSSTRING strEquipNo, STRING_MAP &mapOfValues, bool bGroupFlag)
{
	NSSTRING pszTemp = (TCHAR*)strInputString.c_str();
	
	// While there are tokens in "string"
	NSSTRING strOPId, strAssetID, strAssetNum;
	TCHAR const *chDelims = _T(" \t\r\n");
	while( true )
	{
		int indexCh2b = pszTemp.find(_T("Operating Number"), 0 );
	    if(indexCh2b == string::npos ) break;
		strOPId = pszTemp.substr(indexCh2b + 16, pszTemp.length() - (indexCh2b + 16));
		//trim leading whhjite space
		NSSTRING::size_type notWhite = strOPId.find_first_not_of(chDelims);
		strOPId.erase(0, notWhite);

		//find next white space
		NSSTRING::size_type isWhite = strOPId.find_first_of(chDelims);
		strAssetID = strOPId.substr(0, isWhite);
		strAssetNum = getPlantNo((TCHAR*)strAssetID.c_str());
		leftTrim(strAssetNum,'0');
		if(bGroupFlag == true)
			mapOfValues.insert(STRING_MAP::value_type(strAssetNum, strEquipNo));
		else	
			mapOfValues.insert(STRING_MAP::value_type(strEquipNo,strAssetNum));
		pszTemp = strOPId;
	
	}
	return NS_SUCCESS;
}

int	CNSAssetHandler::setAssetNumbers(STRING_MAP &mapOfValues)
{
	std::map<LONG_PTR, NSSTRING>::iterator EquipNoIDsItr;
	std::map<NSSTRING, NSSTRING>::iterator EquipNoAssetsItr;
	for(EquipNoIDsItr = m_mapOfEquipNoIDs.begin(); EquipNoIDsItr != m_mapOfEquipNoIDs.end(); EquipNoIDsItr++)
	{	
		EquipNoAssetsItr = mapOfValues.find((*EquipNoIDsItr).second);
		if(EquipNoAssetsItr != mapOfValues.end())
		{
				int nResult = m_LocalDWGObj.setAssetNumber((*EquipNoIDsItr).first, (*EquipNoAssetsItr).second);
				if(nResult == NS_SUCCESS)
				{
					CNSAsset * pAsset = getAsset((*EquipNoIDsItr).first);
					if(pAsset)
					{
						NSSTRING strBlkName = pAsset->getBlockName();
						int nFNO = pAsset->getFNO();
						mapOfNumAssets.insert(std::map<LONG_PTR,NSSTRING>::value_type((*EquipNoIDsItr).first, (*EquipNoAssetsItr).second));
						TCHAR szMsg[1000];												
						NSSPRINTF(szMsg, _T("%s           %d           %s"), (TCHAR*)strBlkName.c_str(), nFNO, (TCHAR*)((*EquipNoAssetsItr).second).c_str());
						WriteToLog(LOG_HEADER, szMsg);
					}
				}
		}
	}
	return NS_SUCCESS;
}


int CNSAssetHandler::generateAssetForSubSwitches(std::map<TCHAR*, TCHAR*> &mapofCredentials)
{
	std::map<LONG_PTR, SWITCH_MAP*>::iterator switchItr;
	std::map<LONG_PTR, NSSTRING>::iterator AssetNumItr;
	
	scanSubStationAssets(mapofSwitches);

	for(switchItr = mapofSwitches.begin(); switchItr != mapofSwitches.end(); switchItr++)
	{
		SWITCH_MAP* switchMap = (*switchItr).second;
		AssetNumItr = mapOfNumAssets.find((*switchItr).first);
		if(AssetNumItr == mapOfNumAssets.end()) continue;
		LONG_PTR lSubStnID = (*AssetNumItr).first;
		CNSAsset *pAsset = getAsset(lSubStnID);
		NSSTRING strSubStnAssNo = (*AssetNumItr).second;
		SWITCH_MAP::iterator itA;

		
		std::map<LONG_PTR, NSSTRING>::iterator theSwitchTypeItr;
		for(itA = switchMap->begin(); itA != switchMap->end(); itA++)
		{
			bool bDiffEquipType = false;
			std::vector<TCHAR*> replyVect;
			setReplyParameters(replyVect);
			// Create Assets for SubSwitches
		//	NSSTRING strSwitchType = (*itA).first;
			NSSTRING strSwitchType = _T("");
			OBJECTID_VECTOR* pObjIdVect = (*itA).second;
			int nSize = (int)pObjIdVect->size();
			if(nSize > 0)
			{
				OBJECTID_VECTOR::iterator theObjVectItr;
				theObjVectItr = pObjIdVect->begin();
				theSwitchTypeItr = m_mapOfSwitchType.find(*theObjVectItr);
				if(theSwitchTypeItr != m_mapOfSwitchType.end())
				{
					strSwitchType = (*theSwitchTypeItr).second;
				}
				theObjVectItr++;
				NSSTRING strCurrentSwitchType;
				for(; theObjVectItr != pObjIdVect->end(); theObjVectItr++)
				{
					theSwitchTypeItr = m_mapOfSwitchType.find((*theObjVectItr));
					if(theSwitchTypeItr != m_mapOfSwitchType.end())
					{
						strCurrentSwitchType = (*theSwitchTypeItr).second;
						if(NSSTRCMP(strCurrentSwitchType.c_str(), strSwitchType.c_str()) != 0)
						{
							TCHAR szMsg[_MAX_PATH + 1];
							NSSPRINTF(szMsg, _T("\'EQUIP_TYPE\' is different for asset \'%s\'"), ((*itA).first).c_str());
							WriteToLog(LOG_OTHER, szMsg);
							bDiffEquipType = true;
							break;
						}
					}
				}
			}
			if(bDiffEquipType == true) continue;

			TCHAR szPlantCode1[_MAX_PATH + 1];
			//strcpy(szPlantCode1, (TCHAR*)strSubStnAssNo.c_str());
			NSSTRCPY(szPlantCode1,strSubStnAssNo.size()+1,(TCHAR*)strSubStnAssNo.c_str());
			TCHAR szPlantCode2[_MAX_PATH + 1];
			//strcpy(szPlantCode2, (TCHAR*)strSwitchType.c_str());
			NSSTRCPY(szPlantCode2,strSwitchType.size()+1,(TCHAR*)strSwitchType.c_str());
			TCHAR szPlantCode3[_MAX_PATH + 1];
			//strcpy(szPlantCode3, _T("-01"));
			NSSTRCPY(szPlantCode3,(int)NSSTRLEN(_T("-01"))+1,_T("-01"));
			TCHAR szEquipGrpID[_MAX_PATH + 1];
			NSSTRING strEquipGrpID = getEquipGrpID((TCHAR*)strSwitchType.c_str());
			//strcpy(szEquipGrpID, (TCHAR*)strEquipGrpID.c_str());
			NSSTRCPY(szEquipGrpID,strEquipGrpID.size()+1,(TCHAR*)strEquipGrpID.c_str());
			TCHAR szEquipRef[_MAX_PATH + 1];
			//strcpy(szEquipRef, "DS");
			NSSTRCPY(szEquipRef,(int)NSSTRLEN(_T("DS"))+1,_T("DS"));
			lstrcat(szEquipRef, szPlantCode1);
			TCHAR szOpIdInstance[_MAX_PATH + 1];
			TCHAR szSize[_MAX_PATH + 1];
			//strcpy(szOpIdInstance, itoa(nSize, (char *) szSize, 10));
       		NSSTRCPY(szOpIdInstance,(int)NSSTRLEN((TCHAR*)itoa(nSize, (char *) szSize, 10))+1,(TCHAR*)itoa(nSize, (char *) szSize, 10));
			std::map<TCHAR*, TCHAR*> mapOfRequest;
			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("PlantCode1"), szPlantCode1));
			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("PlantCode2"), szPlantCode2));
			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("PlantCode3"), szPlantCode3));
			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipGrpId"), szEquipGrpID));
			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipRef"), szEquipRef));
			mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("OpIdInstance"), szOpIdInstance));
			setRequestParameters(mapOfRequest, (TCHAR*)_T("SubSwitch"), pAsset);
			int iRet = prepareSOAPMessage(mapofCredentials, replyVect, mapOfRequest);
			if(iRet == NS_FAIL)
				continue;
			std::vector<CNSDataItem*> dataItemsVect;
			std::vector<CNSDataItem*>::iterator dataItemItr;
			std::vector<TCHAR*> vecErrors;

			// Need to clear the equip id map for next group of switches
			m_mapOfEquipNoIDs.clear();

			TCHAR* szPlatNumber = m_ellipseConnector.getResult(dataItemsVect,vecErrors);
			if(NS_FAIL == processErrMessages(vecErrors))
				return NS_FAIL;
			//TCHAR* szSwitchAsset = new TCHAR[_MAX_PATH + 1];
			TCHAR* szSubSwitchEquipNo = new TCHAR[_MAX_PATH + 1];
			//strcpy(szSubSwitchEquipNo, _T(""));
			NSSTRCPY(szSubSwitchEquipNo,(int)NSSTRLEN(_T(""))+1,_T(""));
			for(dataItemItr = dataItemsVect.begin(); dataItemItr != dataItemsVect.end(); dataItemItr++)
			{
				CNSDataItem* pDataItem = (*dataItemItr);
				if(pDataItem == NULL)
				{
					continue;
				}
			/*	TCHAR* szPlantNo = pDataItem->getValueFromMap("PlantNo");
				NSSTRING strsubSwitchAssetNo = getPlantNo(szPlantNo);*/
				//strcpy(szSwitchAsset, szPlantNo);
				
				OBJECTID_VECTOR::iterator it10;
				TCHAR* szEquip = pDataItem->getValueFromMap((TCHAR*)_T("EquipNo"));
				//strcpy(szSubSwitchEquipNo, szEquip);
				NSSTRCPY(szSubSwitchEquipNo,(int)NSSTRLEN(szEquip)+1,szEquip);
				m_mapOfEquipNoIDs.insert(std::map<LONG_PTR, NSSTRING>::value_type(pAsset->getObjectID(), szSubSwitchEquipNo));
			}

			// Link Project if AssetType is ServicePillar
			setLinkToProject(mapofCredentials, szSubSwitchEquipNo);
			STRING_MAP mapOfValues;
			STRING_MAP::iterator ValueIter;
			assignOPIds(mapofCredentials, mapOfValues, true);
			ValueIter = mapOfValues.begin();
			OBJECTID_VECTOR::iterator it10;
			for(it10 = pObjIdVect->begin(); it10 != pObjIdVect->end(); it10++)
			{
				
				std::map<NSSTRING, NSSTRING> mapofAttributes;
				LONG_PTR objID = (*it10);
				CNSAsset *pAsset = new CNSAsset;
				pAsset->setObjectID(objID);
				/*m_LocalDWGObj.readBlockAttributes(lSubStnID, mapofAttributes);
				pAsset->m_mapOfAttributes = mapofAttributes;*/
				//int nResult = validateAsset(pAsset, "SubSwitch", false);
				//if(nResult == NS_FAIL)
				//{
				//	//TCHAR szInfo[_MAX_PATH + 1];
				//	//NSSPRINTF(szInfo, "Invalid Asset found. Block Name = %s, FNO = %d ", (TCHAR*)pServicePillar->getBlockName().c_str(), pServicePillar->getFNO());
				//	//WriteToLog(LOG_WARNINGS, "Invalid Asset found of type SubSwitch");
				//	SetErrorsOccuredFlag(true);
				//	ValueIter++;
				//	continue;
				//}	
				if(ValueIter == mapOfValues.end()) continue;
				NSSTRING strsubSwitchAssetNo = (*ValueIter).first;
				int iRet = m_LocalDWGObj.setAssetNumber(objID, strsubSwitchAssetNo);
				if(iRet != -1)
				{
					CNSAsset *pOrgAsset = getAsset(objID);
                    TCHAR szMsg[_MAX_PATH + 1];
					if(pOrgAsset)
					{
						NSSPRINTF(szMsg, _T("  %s            %d              %s"), pOrgAsset->getBlockName().c_str(), pOrgAsset->getFNO(), (TCHAR*)strsubSwitchAssetNo.c_str());
					}
					else
					{
						NSSPRINTF(szMsg, _T("  %s                          %s"), _T("Sub Switch"), (TCHAR*)strsubSwitchAssetNo.c_str());
					}
					WriteToLog(LOG_HEADER, szMsg);
					//mapOfNumAssets.insert(std::map<long,int>::value_type(objID, 1));
					mapOfNumAssets.insert(std::map<LONG_PTR,NSSTRING>::value_type(objID, strsubSwitchAssetNo));

				}
				else
				{
					TCHAR szMsg[_MAX_PATH + 1];
					NSSPRINTF(szMsg, _T("Failed to add asset number for Asset type = %s "), _T("Sub Switch"));
					WriteToLog(LOG_HEADER, szMsg);
					SetErrorsOccuredFlag(true);
				}
				delete pAsset;
				pAsset = NULL;
				ValueIter++;
			}

			delete[] szSubSwitchEquipNo;
			szSubSwitchEquipNo = NULL;
		}
}
	for(switchItr = mapofSwitches.begin(); switchItr != mapofSwitches.end(); switchItr++)
	{
		SWITCH_MAP* pMap = (*switchItr).second;
		SWITCH_MAP::iterator it;
		for(it = pMap->begin(); it != pMap->end(); it++)
		{
			OBJECTID_VECTOR* pVect = (*it).second;
			delete pVect;
			pVect = NULL;
		}
		delete pMap;
		pMap = NULL;
	}
	mapofSwitches.clear();
return NS_SUCCESS;
}

CNSAsset* CNSAssetHandler::getAsset(LONG_PTR nObjectID)
{
	std::map<NSSTRING, ASSET_VECTOR*>::iterator it;
	CNSAsset *pAsset = NULL;
	for(it = m_mapOfAssets.begin(); it != m_mapOfAssets.end(); it++)
	{
		ASSET_VECTOR* pVect = (*it).second;
		ASSET_VECTOR::iterator vectItr;
		for(vectItr = pVect->begin(); vectItr != pVect->end(); vectItr++)
		{
			pAsset = *vectItr;
			if(pAsset->getObjectID() == nObjectID)
				return pAsset;
		}
	}
	return NULL;
}

int CNSAssetHandler::generateAssetForServicePillars(std::map<TCHAR*, TCHAR*> &mapofCredentials)
{
	std::map<LONG_PTR, ASSET_VECTOR*>::iterator pillarsItr;
	std::map<LONG_PTR, NSSTRING>::iterator AssetNumItr;
	mapOfPillars.clear();
	scanServicePillars(mapOfPillars);
	int Size = (int)mapOfPillars.size();
	for(pillarsItr = mapOfPillars.begin(); pillarsItr != mapOfPillars.end(); pillarsItr++)
	{
		//AK END
			ASSET_VECTOR* pServicePillarVect = (*pillarsItr).second;
			ASSET_VECTOR::iterator serviceItr;
			for(serviceItr = pServicePillarVect->begin(); serviceItr != pServicePillarVect->end(); serviceItr++)
			{
				AssetNumItr = mapOfNumAssets.find((*pillarsItr).first);
				if(AssetNumItr == mapOfNumAssets.end()) continue;
				LONG_PTR lPillarID = (*AssetNumItr).first;
			//	CNSAsset *pAsset = getAsset(lPillarID);
				NSSTRING strMainPillarAssetNo = (*AssetNumItr).second;

				CNSAsset* pServicePillar = (*serviceItr);
			
				std::vector<TCHAR*> replyVect;
				setReplyParameters(replyVect);
				std::map<TCHAR*, TCHAR*> mapOfRequest;
				mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("PlantCode1"), (TCHAR*)strMainPillarAssetNo.c_str()));
				setRequestParameters(mapOfRequest,(TCHAR*) _T("LVServicePillar"), pServicePillar);
				int iResult = prepareSOAPMessage(mapofCredentials, replyVect, mapOfRequest);
				if(iResult == NS_FAIL)
					continue;
				std::vector<CNSDataItem*> dataItemsVect;
				std::vector<CNSDataItem*>::iterator dataItemItr;
				std::vector<TCHAR*> vecErrors;

				// Need to clear the equip id map for next group of switches
				m_mapOfEquipNoIDs.clear();

				TCHAR* szPlatNumber = m_ellipseConnector.getResult(dataItemsVect,vecErrors);
				if(NS_FAIL == processErrMessages(vecErrors))
					return NS_FAIL;
				TCHAR* szSPillarEquipNo = new TCHAR[_MAX_PATH + 1];
				//strcpy(szSPillarEquipNo, _T(""));
				NSSTRCPY(szSPillarEquipNo,(int)NSSTRLEN(_T("")),_T(""));
				for(dataItemItr = dataItemsVect.begin(); dataItemItr != dataItemsVect.end(); dataItemItr++)
				{
					CNSDataItem* pDataItem = (*dataItemItr);
					if(pDataItem == NULL)
					{
						continue;
					}
					/*TCHAR* szPlantNo = pDataItem->getValueFromMap("PlantNo");
					NSSTRING strServiceAssetNo = getPlantNo(szPlantNo);*/
				
					TCHAR* szEquip = pDataItem->getValueFromMap((TCHAR*)_T("EquipNo"));
					//strcpy(szSPillarEquipNo, szEquip);
					NSSTRCPY(szSPillarEquipNo,(int)NSSTRLEN(szEquip)+1,szEquip);
					m_mapOfEquipNoIDs.insert(std::map<LONG_PTR, NSSTRING>::value_type(pServicePillar->getObjectID(), szSPillarEquipNo));
				}

				
				// Link Project if AssetType is ServicePillar
				setLinkToProject(mapofCredentials, szSPillarEquipNo);
				STRING_MAP mapOfValues;
				STRING_MAP::iterator ValueIter;
				assignOPIds(mapofCredentials, mapOfValues, true);
				ValueIter = mapOfValues.begin();
				LONG_PTR objID = pServicePillar->getObjectID();
				CNSAsset *pAsset = new CNSAsset;
				pAsset->setObjectID(objID);
				if(ValueIter != mapOfValues.end()) 
				{
					NSSTRING strServicePillarAssetNo = (*ValueIter).first;
					int iRet = m_LocalDWGObj.setAssetNumber(objID, strServicePillarAssetNo);
					if(iRet != -1)
					{
						CNSAsset *pOrgAsset = getAsset(objID);
						TCHAR szMsg[_MAX_PATH + 1];
						if(pOrgAsset)
						{
							NSSPRINTF(szMsg, _T("  %s            %d              %s"), pOrgAsset->getBlockName().c_str(), pOrgAsset->getFNO(), (TCHAR*)strServicePillarAssetNo.c_str());
						}
						else
						{
							NSSPRINTF(szMsg, _T("  %s                          %s"), _T("Service Pilar"), (TCHAR*)strServicePillarAssetNo.c_str());
						}
						WriteToLog(LOG_HEADER, szMsg);
						//mapOfNumAssets.insert(std::map<long,int>::value_type(objID, 1));
						mapOfNumAssets.insert(std::map<LONG_PTR,NSSTRING>::value_type(objID, strServicePillarAssetNo));

					}
					else
					{
						TCHAR szMsg[_MAX_PATH + 1];
						NSSPRINTF(szMsg, _T("Failed to add asset number for Asset type = %s "), _T("Service Pillar"));
						WriteToLog(LOG_HEADER, szMsg);
						SetErrorsOccuredFlag(true);
					}
				}
				delete pAsset;
				pAsset = NULL;
			
			}
	
		//}
	}
	std::map<LONG_PTR, ASSET_VECTOR*>::iterator pillarsIter;
	for(pillarsIter = mapOfPillars.begin(); pillarsIter != mapOfPillars.end(); pillarsIter++)
	{
		ASSET_VECTOR* pVect = (*pillarsIter).second;
		ASSET_VECTOR::iterator it;
		/*for(it = pVect->begin(); it != pVect->end(); it++)
		{
			CNSAsset *pAsset = (*it);
			delete pAsset;
			pAsset = NULL;
		}*/
		pVect->clear();
		delete pVect;
		pVect = NULL;
	}
	mapOfPillars.clear();
	return NS_SUCCESS;
}

int CNSAssetHandler::processErrMessages(std::vector<TCHAR*> &vectMsgs)
{
	TCHAR* pszMsg = NULL;
	if(vectMsgs.size()>0)
	{
		for(int i=0;i<vectMsgs.size();i++)
		{
			pszMsg = vectMsgs.at(i);
			WriteToLog(LOG_OTHER, pszMsg);
		}
		SetErrorsOccuredFlag(true);
		for(int i=0;i<vectMsgs.size();i++)
		{
			pszMsg = vectMsgs.at(i);
			delete[] pszMsg; 
		}
		vectMsgs.clear();
		return NS_FAIL;
	}
	return NS_SUCCESS;
}

void CNSAssetHandler::leftTrim(NSSTRING &src, TCHAR ch)
{
	NSSTRING::size_type pos = src.find_first_not_of(ch, 0);
	if(pos > 0)
		src.erase(0, pos);
}