#include "StdAfx.h"
#include "nsassetvalidation.h"
#include "..\Include\NSAsset.h"
#include "NSErrorCircle.h"
#include "..\Include\NSDatabaseMgr.h"

extern CNSErrorCircle gErrorCircle;

CNSAssetValidation::CNSAssetValidation(void)
{
}

CNSAssetValidation::~CNSAssetValidation(void)
{
}

//*******************************Commented on 4-Feb-2014, 2013 conversion, since no asset validation is performed************************************
//int getNumberOfDigits(int nNumber)
//{
//	int nNumDigits = 0;
//	if (nNumber < 0)
//		nNumber = -nNumber; 
//	while(nNumber > 0)
//	{
//		nNumDigits++;    
//		nNumber /= 10;
//	}
//	return nNumDigits;
//}
//
//void CNSAssetValidation::setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr)
//{
//	m_pLocalDWGMgr = pLocalDWGMgr;
//}
//
//void CNSAssetValidation::setUserCredentials(CNSUserCredentials userCredentials)
//{
//	m_UserCredentials = userCredentials;
//}
//
//void CNSAssetValidation::leftTrim(NSSTRING &src, TCHAR ch)
//{
//	NSSTRING::size_type pos = src.find_first_not_of(ch, 0);
//	if(pos > 0)
//		src.erase(0, pos);
//}
//*******************************Commented on 4-Feb-2014, 2013 conversion, since no asset validation is performed************************************

//*******************************Commented on 4-Feb-2014, 2013 conversion, since no asset validation is performed************************************
//int CNSAssetValidation::validate()
//{
//	std::map<TCHAR*, TCHAR*> mapOfCredentials;
//	mapOfCredentials.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("Username"), m_UserCredentials.m_szUserName));
//	mapOfCredentials.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("Password"), m_UserCredentials.m_szPassword));
//	mapOfCredentials.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("Position"), m_UserCredentials.m_szPosition));
//	mapOfCredentials.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("District"), m_UserCredentials.m_szDistrict));
//
//	if(NS_SUCCESS != m_EllipseConnector.connect(m_UserCredentials.m_szHostID))
//	{
//		return NS_FAIL; 
//	}
//	if(NS_SUCCESS != m_EllipseConnector.beginSOAPMessage(_T("process")))
//	{
//		return NS_FAIL; 
//	}
//	if(NS_SUCCESS != m_EllipseConnector.startRequest(_T("a_message_id"), _T("1.0"), _T("http://www.w3.org/2001/XMLSchema-instance"), 
//		_T("H:\\Scada-ELL\\XSD\\EllipseRequest.xsd")))
//	{
//		return NS_FAIL; 
//	}
//	if(NS_SUCCESS != m_EllipseConnector.buildCredentials(mapOfCredentials))
//	{
//		return NS_FAIL; 
//	}
//	if(NS_SUCCESS != m_EllipseConnector.startService(_T("EQUIPMENT.Retrieve")))
//	{
//		return NS_FAIL; 
//	}
//	std::vector<TCHAR*> replyVect;
//	replyVect.push_back(_T("EquipNo"));
//	replyVect.push_back(_T("EquipClass"));
//	replyVect.push_back(_T("EquipGrpId"));
//	replyVect.push_back(_T("PlantNo"));
//	replyVect.push_back(_T("EquipStatus"));
//	replyVect.push_back(_T("OriginalDoc"));
//	if(NS_SUCCESS != m_EllipseConnector.buildReply(replyVect))
//	{
//		return NS_FAIL;
//	}
//	std::map<TCHAR*, TCHAR*> mapOfRequest;
//	std::map<TCHAR*, TCHAR*>::iterator itr;
//	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipRefSearchMethod"), (TCHAR*)_T("E")));
//
//	
//	STRING_MAP mapOfAttribs, mapOfPrompt;
//	mapOfAttribs.insert(STRING_MAP::value_type((TCHAR*)_T("CAP_NO"),_T("")));
//	mapOfAttribs.insert(STRING_MAP::value_type((TCHAR*)_T("DrawingNumber"), _T("")));
//	mapOfAttribs.insert(STRING_MAP::value_type((TCHAR*)_T("ProjectNumber"), _T("")));
//	mapOfAttribs.insert(STRING_MAP::value_type((TCHAR*)_T("USERSTATUS"), _T("")));
//	mapOfAttribs.insert(STRING_MAP::value_type((TCHAR*)_T("DEPOTCODE"), _T("")));
//	mapOfAttribs.insert(STRING_MAP::value_type((TCHAR*)_T("EXPORTID"), _T("")));
//	m_pLocalDWGMgr->listTitleBlockHeaderAttributes(mapOfAttribs, mapOfPrompt);
//	STRING_MAP::iterator attributesItr;
//	attributesItr = mapOfAttribs.find(_T("DrawingNumber"));
//	NSSTRING strDrawingNumber = (*attributesItr).second;
//	NSSTRING strDRGNumber = _T("DRG");
//	strDRGNumber.append(strDrawingNumber);
//
//	mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipRef"), (TCHAR*)strDRGNumber.c_str()));
//
//	if(NS_SUCCESS != m_EllipseConnector.buildRequest(mapOfRequest, _T("ID00001"),true))
//	{
//		return NS_FAIL; 
//	}
//	if(NS_SUCCESS != m_EllipseConnector.endElement())
//	{
//		return NS_FAIL; 
//	}
//	if(NS_SUCCESS != m_EllipseConnector.endElement())
//	{
//		return NS_FAIL; 
//	}
//
//	#ifndef _DEBUG
//		if(NS_SUCCESS != m_EllipseConnector.endSOAPMessage())
//		{
//			return NS_FAIL; 
//		}
//	#endif // _DEBUG
//
//	std::vector<CNSDataItem*> dataItemsVect;
//	std::vector<CNSDataItem*>::iterator dataItemItr;
//	std::vector<TCHAR*> vecErrors;
//
//	#ifdef _DEBUG
//		m_EllipseConnector.debugxml = _T("E:\\Projects done\\Project AEC\\asset validation simulation\\loginerror.xml");
//	#endif // _DEBUG
//
//	m_EllipseConnector.getResult(dataItemsVect,vecErrors);
//	if(NS_FAIL == processErrMessages(vecErrors))
//			return NS_FAIL;
//
//	int nLen = (int)dataItemsVect.size();
//	std::map<NSSTRING, int> mapAssetNum;
//	std::map<NSSTRING,int>::iterator mapItr;
//	
//	for(dataItemItr = dataItemsVect.begin(); dataItemItr != dataItemsVect.end(); dataItemItr++)
//	{
//		CNSDataItem* pData = (*dataItemItr);
//
//        TCHAR* szPlantNo = pData->getValueFromMap(_T("EquipNo"));
//		m_mapOfEquipNoIDs.insert(std::map<NSSTRING,long>::value_type((TCHAR*)szPlantNo,1));
//
//		TCHAR* szOriginalDoc = pData->getValueFromMap(_T("OriginalDoc"));
//		m_mapOfEquipNoAndOriginalDocs.insert(std::map<NSSTRING,NSSTRING>::value_type((TCHAR*)szPlantNo,(TCHAR*)szOriginalDoc));
//
//		//NSSTRING strPlantNo = getPlantNo(szPlantNo);
//		//leftTrim(strPlantNo, '0');
//		//mapAssetNum.insert(std::map<NSSTRING, int>::value_type(strPlantNo, 0));
//	}
//
//	std::map<NSSTRING,int> mapOfValues;
//	assignOPIds(mapOfCredentials,mapOfValues,true);
//
//	std::vector<CNSAsset*> assetNumVect;
//	std::vector<CNSAsset*>::iterator drawItr;
//	m_pLocalDWGMgr->getAssetNumbers(assetNumVect);
//
//	for(drawItr = assetNumVect.begin(); drawItr != assetNumVect.end();drawItr++)
//	{
//		CNSAsset* pAsset = (*drawItr);
//		NSSTRING strAssetNo = pAsset->getAssetNumber();		
//		leftTrim(strAssetNo, '0');
//		mapItr = mapOfValues.find(strAssetNo);
//		if(mapItr != mapOfValues.end())
//		{
//			(*mapItr).second = 1;
//
//			std::map<NSSTRING,NSSTRING>::iterator mapItr1;
//			mapItr1 = m_mapOfEquipNoAndOriginalDocs.find(strAssetNo);
//			if(mapItr1 != m_mapOfEquipNoAndOriginalDocs.end())	//End checking added by prafulla on 2 Sept 2010
//			{
//				NSSTRING szOriginalDoc = (*mapItr1).second;
//
//				if(NSSTRCMP(szOriginalDoc.c_str(), _T("")) != 0)
//				{
//					int nErrorNumber = 0;
//					CNSPoint3D pt; 
//					m_pLocalDWGMgr->GetInsertionPoint(pAsset->getObjectID(), pt);
//					nErrorNumber = WriteErrorRec(LOG_GIS,ERR_GIS_36,pAsset);
//					gErrorCircle.drawCircle(nErrorNumber, LOG_GIS, ERR_GIS_36, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
//				}
//			}
//
//		}
//		else
//		{
//			TCHAR szMsg[_MAX_PATH + 1];
//            CNSPoint3D pt; 
//			m_pLocalDWGMgr->GetInsertionPoint(pAsset->getObjectID(), pt);
//			int nErrorNumber = 0;
//			if(pAsset->getAssetNumber().empty())
//			{
//
//           	    //NSSPRINTF(szMsg, _T("Asset Number value is empty for Block \'%s\' on layer \'%s\'"), 
//				//			pAsset->getBlockName().c_str(), pAsset->getLayerName().c_str());
//				//WriteIDToLog(LOG_GIS, szMsg, ERR_GIS_0);
//
//				nErrorNumber = WriteErrorRec(LOG_GIS,ERR_GIS_0,pAsset);
//				
//			}
//			else
//			{
//				//NSSPRINTF(szMsg, _T("Asset Number value of \'%s\' for Block \'%s\' on layer \'%s\' not found in Ellipse"), 
//				//pAsset->getAssetNumber().c_str(), pAsset->getBlockName().c_str(), pAsset->getLayerName().c_str());
//				//WriteIDToLog(LOG_GIS, szMsg, ERR_GIS_301);
//
//				nErrorNumber = WriteErrorRec(LOG_GIS,ERR_GIS_301,pAsset);
//				
//			}
//   	        gErrorCircle.drawCircle(nErrorNumber, LOG_GIS, ERR_GIS_301, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
//		}
//	}
//
//	for(mapItr = mapOfValues.begin(); mapItr != mapOfValues.end(); mapItr++)
//	{
//		int nVal = (*mapItr).second;
//		NSSTRING szKey = (*mapItr).first;
//		if(nVal == 0)
//		{
//			TCHAR szMsg[_MAX_PATH + 1];
//			//NSSPRINTF(szMsg, _T("Asset Number value \'%s\' returned by Ellipse not found in the drawing"), szKey.c_str());
//			//WriteIDToLog(LOG_GIS, szMsg, ERR_GIS_1);
//
//			WriteErrorRec(LOG_GIS,ERR_GIS_1,_T("AN$"), szKey.c_str());
//			
//			
//		}
//
//	}
//
//	// Run a loop to report duplicate asset numbers found in drawing
//	std::vector<CNSAsset*>::iterator compItr;
//
//	for(drawItr = assetNumVect.begin(); drawItr != assetNumVect.end();drawItr++)
//	{
//		CNSAsset* pAsset = (*drawItr);
//		NSSTRING strAssetNo = pAsset->getAssetNumber();
//		NSSTRING strFeatureType;
//		getFeatureTypeFromBlockLayer(pAsset,strFeatureType);
//		for(compItr = assetNumVect.begin(); compItr != assetNumVect.end(); compItr++)
//		{
//			//Don't check with self
//			if(drawItr == compItr)
//				continue;
//				CNSAsset* pCompAsset = (*compItr);
//				NSSTRING strCompAssetNo = pCompAsset->getAssetNumber();
//				NSSTRING strCompFeatureType;
//				getFeatureTypeFromBlockLayer(pCompAsset, strCompFeatureType);
//				//Matching asset number should not have same feature type
//				if(!strCompAssetNo.empty())
//				{
//					if(NSSTRCMP(strCompAssetNo.c_str(), strAssetNo.c_str()) == 0 && //Asset number is matched
//					   NSSTRCMP(strFeatureType.c_str(), strCompFeatureType.c_str()) == 0) //Feature type matched
//					{
//						//Report error
//						TCHAR pszMsg[_MAX_PATH + 1];
//						CNSPoint3D pt; 
//						m_pLocalDWGMgr->GetInsertionPoint(pAsset->getObjectID(), pt);
//						int nErrorNumber;
//
//						//NSSPRINTF(pszMsg, _T("Duplicate Asset Number \'%s\' found for more than one assets of type \'%s\'"),pAsset->getAssetNumber().c_str(),strFeatureType.c_str());
//						//WriteIDToLog(LOG_GIS, pszMsg,ERR_GIS_313);
//
//						nErrorNumber = WriteErrorRec(LOG_GIS,ERR_GIS_313,pAsset,strFeatureType);						
//						
//						gErrorCircle.drawCircle(nErrorNumber, LOG_GIS, ERR_GIS_313, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
//						break; //stop comparing for this asset
//					}
//				}
//		}
//	}
//	for(drawItr = assetNumVect.begin(); drawItr != assetNumVect.end();drawItr++)
//	{
//		CNSAsset* pAsset = (*drawItr);
//		delete pAsset;
//		pAsset = NULL;
//	}
//	return NS_SUCCESS;
//}
//*******************************Commented on 4-Feb-2014, 2013 conversion, since no asset validation is performed************************************

//*******************************Commented on 4-Feb-2014, 2013 conversion, since no asset validation is performed************************************
//NSSTRING CNSAssetValidation::getPlantNo(TCHAR* szPlantNo)
//{
//	NSSTRING strPlantNo = _T("");
//	NSSTRING strAssetNo = szPlantNo;
//	if(strAssetNo.length() > 2)
//		strPlantNo = strAssetNo.substr(2, strAssetNo.length() - 1);
//	return strPlantNo;
//}
//
//
//int CNSAssetValidation::assignOPIds(std::map<TCHAR*, TCHAR*> &mapofCredentials, std::map<NSSTRING,int> &mapOfValues, bool bGroupFlag)
//{
//	if(NS_SUCCESS != m_EllipseConnector.connect(m_UserCredentials.m_szHostID))
//	{
//		//WriteIDToLog(LOG_GIS,  _T("Failed to connect to ellipse"), ERR_GIS_303);
//
//		WriteErrorRec(LOG_GIS, ERR_GIS_303,_T("Failed to connect to ellipse"));
//		
//		return NS_FAIL; 
//	}
//	if(NS_SUCCESS != m_EllipseConnector.beginSOAPMessage(_T("process")))
//	{
//		//WriteIDToLog(LOG_GIS,  _T("Failed to begin SOAP message"), ERR_GIS_304);
//
//		WriteErrorRec(LOG_GIS , ERR_GIS_304,_T("Failed to begin SOAP message"));
//
//        return NS_FAIL;
//	}
//	if(NS_SUCCESS != m_EllipseConnector.startRequest(_T("a_message_id"), _T("1.0"), _T("http://www.w3.org/2001/XMLSchema-instance"), 
//		_T("H:\\Scada-ELL\\XSD\\EllipseRequest.xsd")))
//	{
//		//WriteIDToLog(LOG_GIS,  _T("Failed in preparing Start Request"), ERR_GIS_305);
//
//		WriteErrorRec(LOG_GIS, ERR_GIS_305,_T("Failed in preparing Start Request"));
//
//        return NS_FAIL;
//	}
//	if(NS_SUCCESS != m_EllipseConnector.buildCredentials(mapofCredentials))
//	{
//		//WriteIDToLog(LOG_GIS,  _T("Failed in build credentials"), ERR_GIS_306);
//
//		WriteErrorRec(LOG_GIS, ERR_GIS_306, _T("Failed in build credentials"));
//
//        return NS_FAIL;
//	}
//	if(NS_SUCCESS != m_EllipseConnector.startService(_T("EQUIPMENT.RetrieveAlternateRefs")))
//	{
//
//		//WriteIDToLog(LOG_GIS,  _T("Failed in start service"), ERR_GIS_307);
//
//		WriteErrorRec(LOG_GIS, ERR_GIS_307, _T("Failed in start service")); 
//
//        return NS_FAIL;
//	}
//
//	std::vector<TCHAR*> replyVect;
//	replyVect.push_back(_T("EquipNo"));
//	replyVect.push_back(_T("AlternateRef"));
//	
//	if(NS_SUCCESS != m_EllipseConnector.buildReply(replyVect))
//	{
//		WriteErrorRec(LOG_OTHER, _T("Failed in build reply"));
//		return NS_FAIL;
//	}
//
//	std::map<NSSTRING,long>::iterator EquipNoIDsItr;
//	
//	int nIDCntr = 0;
//	for(EquipNoIDsItr = m_mapOfEquipNoIDs.begin(); EquipNoIDsItr != m_mapOfEquipNoIDs.end(); EquipNoIDsItr++)
//	{
//		NSSTRING strID = _T("ID");
//		nIDCntr++;
//		std::map<TCHAR*, TCHAR*> mapOfRequest;
//		TCHAR strMessage[MAX_PATH + 1];
//		int nDigits = getNumberOfDigits(nIDCntr);
//		nDigits = 5 - nDigits;
//		for(int nCntr = 0; nCntr < nDigits; nCntr++) strID.append(_T("0"));
//		NSSPRINTF(strMessage, _T("%s%i"), strID.c_str(), nIDCntr);
//		
//		mapOfRequest.insert(std::map<TCHAR*, TCHAR*>::value_type((TCHAR*)_T("EquipNo") , (TCHAR*)((*EquipNoIDsItr).first).c_str()));
//		if(NS_SUCCESS != m_EllipseConnector.buildRequest(mapOfRequest, strMessage))
//		{
//			//WriteIDToLog(LOG_GIS,  _T("Failed in build RequestParameters"), ERR_GIS_308);
//
//			WriteErrorRec(LOG_GIS, ERR_GIS_308, _T("Failed in build RequestParameters")); 
//
//		}
//	}
//	if(NS_SUCCESS != m_EllipseConnector.endElement())
//	{
//
//		//WriteIDToLog(LOG_GIS, _T("Failed in build End element in assignOPIds"), ERR_GIS_309);
//
//		WriteErrorRec(LOG_GIS, ERR_GIS_309, _T("Failed in build End element in assignOPIds")); 
//
//        return NS_FAIL;
//	}
//	if(NS_SUCCESS != m_EllipseConnector.endElement())
//	{
//		//WriteIDToLog(LOG_GIS,  _T("Failed in build End element in assignOPIds"), ERR_GIS_310);
//
//		WriteErrorRec(LOG_GIS, ERR_GIS_310,_T("Failed in build End element in assignOPIds")); 
//
//       	return NS_FAIL;
//	}
//
//	#ifndef _DEBUG
//	if(NS_SUCCESS != m_EllipseConnector.endSOAPMessage())
//	{
//		//WriteIDToLog(LOG_GIS,  _T("Failed in end SOAP message in assignOPIds"), ERR_GIS_311);
//
//		WriteErrorRec(LOG_GIS, ERR_GIS_311, _T("Failed in end SOAP message in assignOPIds")); 
//
//        return NS_FAIL;
//	}
//	#endif // _DEBUG	
//
//	std::vector<CNSDataItem*> dataItemsVect;
//	std::vector<CNSDataItem*>::iterator dataItemItr;
//	std::vector<TCHAR*> vecErrors;
//
//	#ifdef _DEBUG
//		//m_EllipseConnector.debugxml = _T("E:\\Projects done\\Project AEC\\asset validation simulation\\altref.xml");
//		 m_EllipseConnector.debugxml = _T("C:\\AEC Asset validation xml\\altref.xml");
//	#endif // _DEBUG
//
//	TCHAR* szPlatNumber = m_EllipseConnector.getResult(dataItemsVect,vecErrors);
//	if(NS_FAIL == processErrMessages(vecErrors))
//			return NS_FAIL;
//
////	STRING_MAP mapOfValues;
//	for(dataItemItr = dataItemsVect.begin(); dataItemItr != dataItemsVect.end(); dataItemItr++)
//	{
//		CNSDataItem* pDataItem = (*dataItemItr);
//		TCHAR* szOPId = pDataItem->getValueFromMap(_T("AlternateRef"));
//		TCHAR* szEquipNo = pDataItem->getValueFromMap(_T("EquipNo"));
//		tokenize(szOPId, szEquipNo, mapOfValues, bGroupFlag);
//	}
//	
//	return NS_SUCCESS;
//}
//
//int CNSAssetValidation::tokenize(NSSTRING strInputString, NSSTRING strEquipNo, std::map<NSSTRING,int> &mapOfValues, bool bGroupFlag)
//{
//	NSSTRING pszTemp = (TCHAR*)strInputString.c_str();
//	
//	// While there are tokens in "string"
//	NSSTRING strOPId, strAssetID, strAssetNum;
//	TCHAR const *chDelims = _T(" \t\r\n");
//	while( true )
//	{
//		int indexCh2b = pszTemp.find(_T("Operating Number"), 0 );
//	    if(indexCh2b == string::npos ) break;
//		strOPId = pszTemp.substr(indexCh2b + 16, pszTemp.length() - (indexCh2b + 16));
//		//trim leading whhjite space
//		NSSTRING::size_type notWhite = strOPId.find_first_not_of(chDelims);
//		strOPId.erase(0, notWhite);
//
//		//find next white space
//		NSSTRING::size_type isWhite = strOPId.find_first_of(chDelims);
//		strAssetID = strOPId.substr(0, isWhite);
//		strAssetNum = getPlantNo((TCHAR*)strAssetID.c_str());
//		leftTrim(strAssetNum, '0');
//		if(bGroupFlag == true)
//			mapOfValues.insert(std::map<NSSTRING,int>::value_type(strAssetNum,0));
//		else	
//			mapOfValues.insert(std::map<NSSTRING,int>::value_type(strEquipNo,0));
//		pszTemp = strOPId;
//	
//	}
//	return NS_SUCCESS;
//}
//
//int CNSAssetValidation::processErrMessages(std::vector<TCHAR*> &vectMsgs)
//{
//	TCHAR* pszMsg = NULL;
//	if(vectMsgs.size()>0)
//	{
//		for(int i=0;i<vectMsgs.size();i++)
//		{
//			pszMsg = vectMsgs.at(i);
//			WriteErrorRec(LOG_OTHER, pszMsg);
//		}
//		for(int j=0;j<vectMsgs.size();j++)
//		{
//			pszMsg = vectMsgs.at(j);
//			delete[] pszMsg; 
//		}
//		vectMsgs.clear();
//		return NS_FAIL;
//	}
//	return NS_SUCCESS;
//}
//
//void CNSAssetValidation::getFeatureTypeFromBlockLayer(CNSAsset* pAsset,NSSTRING &strFeatureType)
//{
//	if(pAsset)
//	{
//    	
//		NSSTRING szBlockName = pAsset->getBlockName();
//		NSSTRING szLayerName = pAsset->getLayerName();
//
//		int nDBRet = 0;
//
//		TCHAR szQuery[_MAX_PATH + 1];
//		NSSPRINTF(szQuery, _T("Select FeatureName from BlockFeatures where NewBlock = \'%s\' and NewLayer = \'%s\'"),
//				szBlockName.c_str(), szLayerName.c_str());
//		nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
//
//		if(-1 != nDBRet)
//		{
//			variant_t vFeatureType;
//			bool bFNO = CNSDatabaseMgr::getInstance()->getValue(_T("FeatureName"), vFeatureType);
//			if(bFNO)
//			{
//				#ifndef _INTERNAL_
//					strFeatureType = (const char*)_bstr_t(vFeatureType.bstrVal);
//				#else
//					strFeatureType = vFeatureType.bstrVal;
//					/*char* strLayer = _com_util::ConvertBSTRToString(vFeatureType.bstrVal);
//					strFeatureType = strLayer;*/
//				#endif
//			}
//		}
//	}
//}

//*******************************Commented on 4-Feb-2014, 2013 conversion, since no asset validation is performed************************************