#include "StdAfx.h"
#include "NSFeature.h"
#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSCompositeGeometry.h"
#include "NSComponentContainer.h"
#include "NSPoint.h"
#include "..\..\Include\NSReportWriter.h"
#include "..\..\Include\NSBlock.h"
#include "algorithm"
extern int gMaxFID;
#define WriteToLog(str,err) CNSReportWriter::getReportWriterInstance()->writeLog(str,err)
using namespace std;

// Constructor
CNSFeature::CNSFeature(void)
{
	m_pDwgMgr = NULL;
	m_pDbMgr = NULL;
	m_pContainer = NULL;
	m_pElement = NULL;
	m_strFeatureType = _T("");
	m_enCategory = CAT_INVALID;
	m_strState = _T("");
	m_strUsage = _T("");
	m_strOrientation = _T("");
	m_nFID = 0;
	m_nFNO = 0;
	m_bNestedFeature = false;
}

// Destructor
CNSFeature::~CNSFeature(void)
{

}

/*!
@Brief			This function reads Feature information from XML.
@Param [in]		element			:	Feature Node
@Return			NS_SUCCESS : If reading process successful.
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSFeature::readXML(CNSTElement &element)
{
	m_nFID = _wtoi(element.subval(_T("FID")).c_str());
	m_nFNO = _wtoi(element.subval(_T("FNO")).c_str());
	m_strState = element.subval(_T("State"));
	m_strUsage = element.subval(_T("Usage"));
	m_strOrientation = element.subval(_T("Orientation"));


	// Read Components data
	CNSTElement subelem = element.subnode(_T("Components"));
	m_compContainer.setObject(m_pDbMgr);
	if(NS_SUCCESS != m_compContainer.readXML(subelem))
		return NS_FAIL;
	// Read Attribute Data
	CNSTElement oAttributesNode = element.subnode(_T("Attributes"));
	if(NS_SUCCESS != m_attributeContainer.readXML(oAttributesNode))
		return NS_FAIL;
	// Read Realtionships data
	CNSTElement oRelationshipNode = element.subnode(_T("Relationships"));
	m_relationship.setElement(oRelationshipNode);
	if(NS_SUCCESS != m_relationship.readXML())
		return NS_FAIL;
	return NS_SUCCESS;
}

//int CNSFeature::createLinearAttributes(NSSTRING strLayerName, int nFNO)
//{
//	TCHAR szQuery[_MAX_PATH + 1];
//	NSSPRINTF(szQuery, _T("Select * from FeatureAttributes where FNO = %d"), nFNO);
//	int nRetVal = m_pDbMgr->executeQuery(szQuery);
//	if(nRetVal == -1)
//	{
//		WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'FeatureAttributes\'"));
//		return NS_FAIL;
//	}
//	std::vector<_variant_t> vValues;
//	m_pDbMgr->getValue(_T("ANO"), vValues);
//	std::map<NSSTRING, NSSTRING>::iterator mapIter;
//	NSSTRING strAttDwgName;
//	NSSTRING strAttName;
//	std::vector<_variant_t>::iterator resultIter;
//	for(resultIter = vValues.begin(); resultIter != vValues.end(); resultIter++)
//	{
//		int ANO = (*resultIter);
//		NSSPRINTF(szQuery, _T("Select * from FeatureAttributes where FNO = %d and ANO = %d"), nFNO, ANO);
//		int nRetVal = m_pDbMgr->executeQuery(szQuery);
//		if(nRetVal == -1)
//		{
//			WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'FeatureAttributes\'"));
//			return NS_FAIL;
//		}
//		_variant_t varAName;
//		_variant_t varRequired;
//		_variant_t varAValue;
//		NSSTRING strColumnName, strAName, strValue;
//		bool bResult1 = m_pDbMgr->getValue(_T("AName"), varAName);
//		bool bResult2 = m_pDbMgr->getValue(_T("Required"), varRequired);
//		if(bResult1 && bResult2)
//		{
//			if(varRequired.iVal == 2)
//			{
//				strColumnName = (NSSTRING)((_bstr_t)(varAName.bstrVal));
//				NSSPRINTF(szQuery, _T("Select %s from NewLinearFeatures where FNO = %d and NewLayer = \'%s\'"), strColumnName.c_str(), nFNO, strLayerName.c_str());
//				int nRetVal = m_pDbMgr->executeQuery(szQuery);
//				if(nRetVal == -1)
//				{
//					continue;		
//				}
//				
//				bool bResult1 = m_pDbMgr->getValue((TCHAR*)strColumnName.c_str(), varAValue);
//				if(bResult1)
//				{
//                    if(varAValue.vt == VT_BSTR)
//                    {
//					    strValue = (NSSTRING)((_bstr_t)(varAValue.bstrVal));
//					    m_attributeContainer.addAttribute(1, ANO, strColumnName, strValue);
//                    }
//                    else
//					{
//						TCHAR szQuery[_MAX_PATH + 1];
//						NSSPRINTF(szQuery, _T("Attribute cann't be added. The attribute column \'%s\' in NewLinearFeatures table should have \'Text\' datatype."), strColumnName.c_str());
//						WriteToLog(LOG_OTHER, szQuery );
//					}
//				}
//			}
//			else if(varRequired.iVal == 3)
//			{
//				int nIndex = (int)strLayerName.find_last_of(_T("_"));
//				if(nIndex != -1)
//				{
//					NSSTRING strCode = strLayerName.substr(nIndex+1, strLayerName.length());
//					strAName = (NSSTRING)((_bstr_t)(varAName.bstrVal));
//					if(NSSTRCMP(strCode.c_str(), _T("NEW")) != 0)
//						m_attributeContainer.addAttribute(1, ANO, strAName.c_str(), strCode);
//				}
//			}
//            else if(varRequired.iVal == 4)
//			{
//                strAName = (NSSTRING)((_bstr_t)(varAName.bstrVal));
//                m_attributeContainer.addAttribute(1, ANO, strAName.c_str(), m_strTextLabel);
//            }
//		}
//	}
//	
//	return NS_SUCCESS;
//
//}
//
/*!
@Brief			This function reads Feature information from drawing.
@Return			NS_SUCCESS : If reading process successful.
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSFeature::readDWG()
{
	std::map<NSSTRING, NSSTRING> mapXData ;
	std::map<NSSTRING, NSSTRING>::iterator xDataIter;
	NSSTRING strBlkName;
	NSSTRING strUsage;
	NSSTRING strOrien;
	TCHAR szQuery[_MAX_PATH + 1];
	CNSCompositeGeometry *pCompGeo = m_pContainer->getAt(0);
	NSSTRING strLayerName = pCompGeo->getLayername();
	strBlkName = pCompGeo->getName();
	ATTRIBUTE_MAP::iterator attItr;
	int nDBRet = -1;
	bool bIsLinear = false;
	NSSTRING strMessage = _T("");
	if(m_strFeatureType == _T("NEW"))
	{
		// If category is label then set the feature number to 369
		if(m_enCategory == CAT_LABEL)
		{
			m_nFNO = 369;
		}
		else
		{
			if(!isNestedFeature())
			{
				// For Block reading block name and layer name from Blockfeatures table.
				if(m_enCategory == CAT_BLOCK)
				{
					NSSTRING strTempUsage, strTemoOri;
					strTempUsage = getQueryString(m_strUsage);
					strTemoOri = getQueryString(m_strOrientation);
					NSSPRINTF(szQuery, _T("Select FNO from BlockFeatures where NewBlock = \'%S\' and Usage %S and Orientation %S"),
						strBlkName.c_str(), strTempUsage.c_str(), strTemoOri.c_str());
					NSSTRING strFinalQuery = getQueryString(m_mapAttribute,szQuery);
					nDBRet = m_pDbMgr->executeQuery(strFinalQuery.c_str());
					strMessage = _T("DataBase Error : Failed to access table \'BlockFeatures\'");
				}
				else
				{
					//if(NSSTRCMP(strLayerName.c_str(), _T("DEVELOPER_PLAN")) != 0 )
					{
						NSSPRINTF(szQuery, _T("Select FNO from NewLinearFeatures where NewLayer = \'%S\'"),strLayerName.c_str());
						nDBRet = m_pDbMgr->executeQuery(szQuery);
						if(nDBRet != -1)
						{
							variant_t vFNO;
							bool bFNO = m_pDbMgr->getValue(_T("FNO"), vFNO);	
							if(bFNO)
							{
								//createLinearAttributes(strLayerName, vFNO.iVal);
								createAttributes(strLayerName, vFNO.iVal);//strFeatureName = strLayerName
								bIsLinear = true;
								m_nFNO = vFNO;
		/*						int nIndex = (int)strLayerName.find_last_of(_T("_"));
								if(nIndex != -1)
								{
									NSSTRING strCode = strLayerName.substr(nIndex+1, strLayerName.length());
									if(NSSTRCMP(strCode.c_str(), _T("NEW")) != 0)
										m_attributeContainer.addAttribute(1, 30061, _T("CONDUCTOR_CODE"), strCode);
								}*/
							}
						}
					}
					/*else
					{
						NSSPRINTF(szQuery, _T("Select FNO from NewLinearFeatures where NewLayer = \'%s\'"),
							strLayerName.c_str());
						nDBRet = m_pDbMgr->executeQuery(szQuery);
					}
					strMessage = _T("DataBase Error : Failed to access table \'NewLinearFeatures\'");*/
				}

				if(-1 != nDBRet)
				{
					variant_t vFNO, vNumRep;
					if(m_enCategory == CAT_BLOCK)
					{
					/*	bool bNumRep = m_pDbMgr->getValue(_T("IsDualRepresent"), vNumRep);
						if(bNumRep)
						{
							int nDualRep = vNumRep;
							m_compContainer.setDualRepresent(nDualRep);
						}
						else
						{
							int nDualRep = 0;
							m_compContainer.setDualRepresent(nDualRep);
						}*/
					}
					if(bIsLinear != true)
					{
						bool bFNO = m_pDbMgr->getValue(_T("FNO"), vFNO);	
						if(bFNO)
							m_nFNO = vFNO;
						else
							return NS_FAIL;
					}

				}
				else
				{
					WriteToLog(LOG_OTHER, (TCHAR*)strMessage.c_str());
					return NS_FAIL;
				}
			}
		}
		m_compContainer.setFNO(m_nFNO);
	}
	else if(m_strFeatureType == _T("MOD"))
	{
		m_compContainer.setFNO(m_nFNO);
		int nDBRet = -1;
		NSSTRING strMessage = _T("");
		if(CAT_BLOCK == m_enCategory)
		{			
		/*	NSSPRINTF(szQuery, _T("Select IsDualRepresent from BlockFeatures where FNO = %d "),m_nFNO);
			strMessage = _T("DataBase Error : Failed to access table \'BlockFeatures\'");
		
			nDBRet = m_pDbMgr->executeQuery(szQuery);
			if(-1 != nDBRet)
			{
				variant_t vFNO, vNumRep;
				if(m_enCategory == CAT_BLOCK)
				{
					bool bNumRep = m_pDbMgr->getValue(_T("IsDualRepresent"), vNumRep);
					if(bNumRep)
					{
						int nDualRep = vNumRep;
						m_compContainer.setDualRepresent(nDualRep);
					}
					else
					{
						int nDualRep = 0;
						m_compContainer.setDualRepresent(nDualRep);
					}
				}
			}
			else
			{
				WriteToLog(LOG_OTHER, (TCHAR*)strMessage.c_str());
				return NS_FAIL;
			}*/
		}
	}
	m_compContainer.setObject(m_pContainer);
	m_compContainer.setObject(m_pDbMgr);
	m_compContainer.setObject(m_pDwgMgr);
	if(NS_SUCCESS != m_compContainer.readDWG())
		return NS_FAIL;
	return NS_SUCCESS;
}

/*!
@Brief			This function writes Feature information into XML.
@Return			NS_SUCCESS : If writing process successful.
				NS_FAIL    : If writing process not successful.
@Date			12-06-2007
*/
int CNSFeature::writeXML()
{
	TCHAR strFID[20] = _T("");
	if(m_nFID == 0)
	{
		m_nFID = --gMaxFID;
	}
	NSSPRINTF(strFID,_T("%d"),m_nFID);
	m_pElement->createNode(_T("FID"), strFID, DEFAULT_NAMESPACE);
	TCHAR strFNO[20] = _T("");
	NSSPRINTF(strFNO,_T("%d"),m_nFNO);
	m_pElement->createNode(_T("FNO"), strFNO, DEFAULT_NAMESPACE);

	m_compContainer.setElement(m_pElement);
	// Write components into XML
	m_compContainer.m_FNOReference = m_nFNO;
	if(NS_SUCCESS != m_compContainer.writeXML())
		return NS_FAIL;
	m_attributeContainer.setElement(m_pElement);
	// Write attributes into XML
	if(NS_SUCCESS != m_attributeContainer.writeXML())
		return NS_FAIL;
	m_relationship.setElement(*m_pElement);
	// Write Relation ship into XML
	if(NS_SUCCESS != m_relationship.writeXML())
		return NS_FAIL;
	return NS_SUCCESS;
}

/*!
@Brief			This formats the SQL query in specila cases like SUBSTATION_TYPE etc.
@Param [in]		mapAttributes			:	map of attributes
				szQuery					:   SQL query.
@Return			formatted SQL query.
@Date			12-06-2007
*/
NSSTRING CNSFeature::getQueryString(std::map<int, CNSAttribute*> & mapAttributes, TCHAR szQuery[_MAX_PATH])
{
	NSSTRING strQuery(szQuery);
	std::map<NSSTRING, NSSTRING> mapofColumns;
	std::map<NSSTRING, NSSTRING>::iterator mapItr;
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("SUBSTATION_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("SWITCH_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CCP_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("LANTERN_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ACTUAL_STATUS"), _T("")));

	std::map<int, CNSAttribute*>::iterator itr;
	for(itr = mapAttributes.begin(); itr != mapAttributes.end(); itr++)
	{
		CNSAttribute* pAttribute = (*itr).second;
		if(pAttribute != NULL)
		{
			NSSTRING strAttName = pAttribute->getAttributeName();
			NSSTRING strValue;
			NSSTRING strAtt;
			pAttribute->getDataValue(1, strValue);
			NSSTRUPR((TCHAR*)strAttName.c_str(), strAttName.size()+1);
			mapItr = mapofColumns.find(strAttName);
			if(mapItr != mapofColumns.end())
			{
				strValue = getQueryString(strValue);
				TCHAR szTemp[_MAX_PATH + 1];
				NSSPRINTF(szTemp, _T(" and %s %s"), strAttName.c_str(), strValue.c_str());
				strQuery.append(szTemp);
			}
		}
	}
	return strQuery;
}

/*!
@Brief			This formats the SQL query in specila cases like SUBSTATION_TYPE etc.
@Param [in]		mapAttributes			:	map of attributes
				szQuery					:   SQL query.
@Return			formatted SQL query.
@Date			12-06-2007
*/
NSSTRING CNSFeature::getQueryString(ATTRIBUTE_MAP & mapAttributes, TCHAR szQuery[_MAX_PATH])
{
	NSSTRING strQuery(szQuery);
	std::map<NSSTRING, NSSTRING> mapofColumns;
	std::map<NSSTRING, NSSTRING>::iterator mapItr;
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("SUBSTATION_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("SWITCH_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CCP_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("LANTERN_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ACTUAL_STATUS"), _T("")));

	ATTRIBUTE_MAP::iterator itr;
	for(itr = mapAttributes.begin(); itr != mapAttributes.end(); itr++)
	{
		NSSTRING strAttName = (*itr).first;
		NSSTRUPR((TCHAR*)strAttName.c_str(), strAttName.size()+1);
		mapItr = mapofColumns.find(strAttName);
		if(mapItr != mapofColumns.end())
		{
			NSSTRING strAttValue = (*itr).second;
			strAttValue = getQueryString(strAttValue);
			TCHAR szTemp[_MAX_PATH + 1];
			NSSPRINTF(szTemp, _T(" and %s %s"), strAttName.c_str(), strAttValue.c_str());
			strQuery.append(szTemp);
		}

	}
	return strQuery;
}

/*!
@Brief			This function writes feature information into Drawing
@Return			NS_SUCCESS : If writing process successful.
				NS_FAIL    : If writing process not successful.
@Date			12-06-2007
*/
int CNSFeature::writeDWG()
{
	m_compContainer.setObject(m_pDbMgr);
	m_compContainer.setObject(m_pDwgMgr);
	STRING_MAP *mapofXData = new STRING_MAP;
	std::map<int, CNSAttribute*> mapAttributes;
	std::map<NSSTRING, NSSTRING> mapBlkAttributes;
	CNSComponent *pComp = NULL;
	TCHAR szQuery[_MAX_PATH + 1];
	int nRetVal = -1;
	std::vector<CNSComponent*> &vComponents = m_compContainer.getComponents();
	getXData(*mapofXData);
	std::vector<CNSComponent*>::iterator compIt;
	CNSCompositeContainer *compContainer = new CNSCompositeContainer;
	CNSCompositeGeometry *pCompGeo = NULL; 
	CNSFeatureContainer featureCont;
	COMPONENT_TYPE enCompType = COMP_INVALID;
	NSSTRING strLayerName, strBlkName;
	m_attributeContainer.getAttributes(/*1,*/ mapAttributes);
	ATT_MAP *mapAttrRef =  new ATT_MAP;
	CNSAttributeRef *pAttrRef = NULL;
	CNSAttributeRefMap *pAttrRefMap = NULL;
	ATT_MAP::iterator AttrRefIter;
	getBlockAttributes(*mapAttrRef, mapAttributes);
	NSSTRING strUsage, strOri;
	strUsage = getQueryString(m_strUsage);
	strOri = getQueryString(m_strOrientation);
	bool bIsCompBlk = false;
	CNSCompositeGeometry *pNewComp = NULL;
	for(compIt = vComponents.begin(); compIt < vComponents.end(); compIt++)
	{
		pComp = (*compIt);
		assignRepresentation(pComp);
		CNSGeometry *pGeometry = pComp->getGeometry();
		pCompGeo = pGeometry->getCompositeGeo();
        pCompGeo->setCompRepresentation(pComp->getCompRepresentation());
        pNewComp = new CNSCompositeGeometry;
		*pNewComp = *pCompGeo;
		enCompType = pComp->getCompType();
		// code to compare criteria field  from FeatureComponents table and orientation
		//from XML file
		
		variant_t criteria;
		NSSTRING strCriteria;
		NSSPRINTF(szQuery, _T("Select Criteria from FeatureComponents where FNO = %d and CNO = %d"),
		m_nFNO, pComp->getComponentNumber());
		nRetVal = m_pDbMgr->executeQuery(szQuery);
		if(nRetVal == -1)
		{
		  	WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'FeatureComponents\'"));
			delete compContainer;
			delete mapofXData;
			featureCont.release();
			delete mapAttrRef;
			return NS_FAIL;
		}
		bool bExtCriteria = m_pDbMgr->getValue(_T("Criteria"), criteria);
		if(bExtCriteria)
			strCriteria = (NSSTRING)((_bstr_t)(criteria.bstrVal));
        if(true != readCriteria(strCriteria))
			continue;
		//---------------------------
		switch(enCompType)
		{
		case COMP_POINT:
			{
				m_enCategory = CAT_BLOCK;
				NSSPRINTF(szQuery, _T("Select ExtLayer,ExtBlock , IsMultipleFeature from BlockFeatures where FNO = %d and USAGE %S and ORIENTATION %S"),
					m_nFNO, strUsage.c_str(), strOri.c_str());
				
				NSSTRING strFinalQuery = getQueryString(mapAttributes, szQuery);
				nRetVal = m_pDbMgr->executeQuery(strFinalQuery.c_str());
				if(nRetVal == -1)
				{
					WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'BlockFeatures\'"));
					delete compContainer;
					delete mapofXData;
					featureCont.release();
					delete mapAttrRef;
					return NS_FAIL;
				}
				variant_t layer, block;
				bool bExtBlk = m_pDbMgr->getValue(_T("ExtBlock"), block);
				if(bExtBlk)
					strBlkName = (NSSTRING)((_bstr_t)(block.bstrVal));
				else
				{
					// this determines the ghost block
					// why so blk name is not available in database
					strBlkName = _T("");
				}



				bool bExtLayer = m_pDbMgr->getValue(_T("ExtLayer"), layer);	
				if(bExtLayer)
					strLayerName = (NSSTRING)((_bstr_t)(layer.bstrVal));
				else
					strLayerName = _T("UNKNOWN_FEATURES");
				bIsCompBlk = true;

				////////////////////////////////////////////////////////////////////////////////////////////////////////
				variant_t varAValue;
				NSSTRING strIsMultipleFeature = _T("IsMultipleFeature");
				bool bResult = m_pDbMgr->getValue((TCHAR*)strIsMultipleFeature.c_str(), varAValue);

				if(bResult)
				{
					if(varAValue.vt == VT_BOOL)
					{
						bool bValue = (varAValue.boolVal);
						if(bValue == true) featureCont.setMultipleFeatureFlag(bValue)  ;
						//return bValue;
					}
				}

				//Get the Final block  for the CAD_LEVEL and CAD_COLOR 
				if(bExtBlk)
					getFinalBlockName(strBlkName, mapAttributes, m_nFNO, strUsage, strOri);
				////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
			break;
		case COMP_TEXTPOINT:
			{
				NSSPRINTF(szQuery, _T("Select AttributeNameDWG from FeatureComponents where FNO = %d and CNO = %d"),
					m_nFNO, pComp->getComponentNumber());
				nRetVal = m_pDbMgr->executeQuery(szQuery);
				if(nRetVal == -1)
				{
					WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'FeatureComponents\'"));
					delete compContainer;
					delete mapofXData;
					featureCont.release();
					delete mapAttrRef;
					return NS_FAIL;
					return NS_FAIL;
				}
				variant_t attributeName;
				bool bAttName = m_pDbMgr->getValue(_T("AttributeNameDWG"), attributeName);
				if(bAttName)
				{
					NSSTRING strAttName = (NSSTRING)((_bstr_t)(attributeName.bstrVal));
					NSSTRUPR((TCHAR*)strAttName.c_str(), strAttName.size()+1);
					AttrRefIter = mapAttrRef->find(strAttName);
					if(AttrRefIter != mapAttrRef->end())
					{
						pAttrRefMap = (*AttrRefIter).second;
						if(pAttrRefMap)
						{
							pAttrRef = pAttrRefMap->getAttrRef(pComp->getComponentID());
							if(pAttrRef)
								pAttrRef->setCompGeometry(pCompGeo);
						}
					}
				}
				break;
			}
		case COMP_COMPOSITE_POLYLINE:
        case COMP_COMPOSITE_MLINE:
		case COMP_BOUNDARY:
		case COMP_POLYGON:
		case COMP_COMPOSITE_POLYGON:
			{
				if(bIsCompBlk != true)
				{
					m_enCategory = CAT_SEGMENT;
					NSSPRINTF(szQuery, _T("Select ExtLineType, ExtLayer  from LinearFeatures where FNO = %d and USAGE %s and ORIENTATION %s"),
						m_nFNO, strUsage.c_str(), strOri.c_str());
					//MessageBox(NULL, szQuery, _T("QRY"), MB_OK);
					nRetVal = m_pDbMgr->executeQuery(szQuery);
					if(nRetVal == -1)
					{
						WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'LinearFeatures\'"));
						return NS_FAIL;
					}
					variant_t lineType, layer;
					bool bLineType = m_pDbMgr->getValue(_T("ExtLineType"), lineType);	
					bool bLayer = m_pDbMgr->getValue(_T("ExtLayer"), layer);
					if(bLayer)
						strLayerName = (NSSTRING)((_bstr_t)(layer.bstrVal));
					else
						strLayerName = _T("UNKNOWN_FEATURES");
					if(bLineType)
						strBlkName = (NSSTRING)((_bstr_t)(lineType.bstrVal));
					else
						strBlkName = _T("");
					m_attributeContainer.getXData(*mapofXData);
				}
			}
			break;
		default:
			break;

		}
		compContainer->add(pNewComp);
	}
	featureCont.setComponentContainer(compContainer);
	featureCont.setLayerName(strLayerName);
	featureCont.setXData(mapofXData);
	featureCont.setTypeName(strBlkName);
	featureCont.setAttributeMap(mapAttrRef);

	int nRet ;
	if(featureCont.getMultipleFeatureFlag())
	{
		nRet = m_pDwgMgr->createCompositeGeometryEx(&featureCont);
	}
	else
	{
		nRet = m_pDwgMgr->createCompositeGeometry(&featureCont);
	}
	


	delete compContainer;
	mapofXData->clear();
	delete mapofXData;
	featureCont.release();
	mapAttrRef->erase(mapAttrRef->begin(), mapAttrRef->end());
	delete mapAttrRef;

	TCHAR szMsg[_MAX_PATH + 1];
	if(NS_ERR_GHOST_BLOCK == nRet)
	{
		NSSPRINTF(szMsg, _T("Inserted Ghost Block for Usage = %s, Orientation = %s, FNO = %d, FID = %d"), m_strUsage.c_str(), m_strOrientation.c_str(), m_nFNO, m_nFID);
		WriteToLog(LOG_WARNINGS, szMsg);
		return NS_SUCCESS; // a entity has been created.
	}
	else if(NS_GHOST_BLOCK_NOT_FOUND == nRet)
	{
		NSSPRINTF(szMsg, _T("Error while writing Feature with FNO = %d and FID = %d : Ghost Block not found"), m_strUsage.c_str(), m_strOrientation.c_str(), m_nFNO, m_nFID);
		WriteToLog(LOG_WARNINGS, szMsg);
		return NS_SUCCESS; // a entity has been created.
	}
	else if(NS_NEW_LAYER_ADDED == nRet)
	{
		NSSPRINTF(szMsg, _T("New layer \'%s\'added for Feature with FNO = %d and FID = %d"), strLayerName.c_str(), m_nFNO, m_nFID);
		WriteToLog(LOG_WARNINGS, szMsg);
		return NS_SUCCESS; // a entity has been created.
	}
	else if(NS_BYLAYER_SET == nRet)
	{
		NSSPRINTF(szMsg, _T("Linetype not found for Feature with FNO = %d and FID = %d,  set to \'ByLayer\'"), m_nFNO, m_nFID);
		WriteToLog(LOG_WARNINGS, szMsg);
		return NS_SUCCESS; // a entity has been created.
	}
	return nRet;
}

void CNSFeature::setFID(int nFID)
{
	m_nFID = nFID;
}

void CNSFeature::setFNO(int nFNO)
{
	m_nFNO = nFNO;
}

void CNSFeature::setObject(CNSDWGMgr* objDwgMgr)
{
	m_pDwgMgr = objDwgMgr;
}

void CNSFeature::setObject(CNSDatabaseMgr* objDbMgr)
{
	m_pDbMgr = objDbMgr;
	m_compContainer.setObject(objDbMgr);
}

FeatureCategory CNSFeature::getCategory() const
{
	return m_enCategory;
}

void CNSFeature::setCategory(const FeatureCategory enCategory)
{
	m_enCategory = enCategory;
}

void CNSFeature::setElement(CNSTElement* pElement)
{
	m_pElement = pElement;
}
/*!
@Brief			From database retrive corresponding atributes for a feature and AtttributeNameInDWG
@Param [in]		Attribute inforamtion map
@Param [out]	Attribute inforamtion map used for transfer
@Return			true/false
@Date			27-07-2007
*/
int CNSFeature::getBlockAttributes(ATT_MAP &mapAttrRef, std::map<int, CNSAttribute*> &attMap)
{
	CNSAttributeRefMap* pAttrRefMap = NULL;

	CNSAttribute* pAttribute = NULL;
	std::map<int, CNSAttribute*>::iterator AttrItr;
	for(AttrItr = attMap.begin(); AttrItr != attMap.end(); AttrItr++)
	{
		variant_t value1;
		int nAttNo = (*AttrItr).first;
		TCHAR szQuery[_MAX_PATH + 1];
		NSSPRINTF(szQuery, _T("Select AttNameInDWG from Attributes where ANO = %d"), nAttNo);
		int nRetVal = m_pDbMgr->executeQuery(szQuery);
		if(nRetVal == -1)
		{
			WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'Attributes\'"));
			return NS_FAIL;
		}
		bool result = m_pDbMgr->getValue(_T("AttNameInDWG"), value1);
		NSSTRING strAttNameInDWG = _T("");
		if(result)
		{
			strAttNameInDWG = (_bstr_t)(value1.bstrVal);
			pAttribute = (*AttrItr).second;
			pAttrRefMap = new CNSAttributeRefMap;
			pAttribute->getData(pAttrRefMap, strAttNameInDWG);
			NSSTRUPR((TCHAR*)strAttNameInDWG.c_str(), strAttNameInDWG.size()+1);
			mapAttrRef.insert(ATT_MAP::value_type(strAttNameInDWG, pAttrRefMap));
		}
	}
	return NS_SUCCESS;
}

void CNSFeature::setFeatureType(NSSTRING strFeatType)
{
	m_strFeatureType = strFeatType;
}

NSSTRING CNSFeature::getFeatureType()const
{
	return m_strFeatureType;
}

void CNSFeature::getXData(STRING_MAP& mapOfXData)
{
	TCHAR szFormat[_MAX_PATH + 1];
	NSSPRINTF(szFormat, _T("%i"), m_nFID);
	mapOfXData.insert(STRING_MAP::value_type(_T("FID"), szFormat));
	NSSPRINTF(szFormat, _T("%i"), m_nFNO);
	mapOfXData.insert(STRING_MAP::value_type(_T("FNO"), szFormat));
	mapOfXData.insert(STRING_MAP::value_type(_T("Usage"), m_strUsage));
	mapOfXData.insert(STRING_MAP::value_type(_T("Orientation"), m_strOrientation));
	mapOfXData.insert(STRING_MAP::value_type(_T("State"), m_strState));
	m_relationship.getXData(mapOfXData);
}

int CNSFeature::getValue(STRING_MAP& mapOfXData, NSSTRING strType, NSSTRING &strValue)
{
	STRING_MAP::iterator TempItr;
	TempItr = mapOfXData.find(strType);
	if(TempItr == mapOfXData.end())
	{
		return NS_FAIL;
	}
	strValue = (*TempItr).second;
	return NS_SUCCESS;
}

/*!
@Brief			Except muliple component in all other cases the CID wil be one
				to retrive attribute value
@Param [in]		Value of attribute
@Param [out]	Attribute inforamtion map used for transfer
@Return			true/false
@Date			27-07-2007
*/
int CNSFeature::getValue(ATT_MAP& mapOfAttrRefs, NSSTRING strType, NSSTRING &strValue)
{
	ATT_MAP::iterator TempItr;
	NSSTRUPR((TCHAR*)strType.c_str(), strType.size()+1);
	TempItr = mapOfAttrRefs.find(strType);
	if(TempItr == mapOfAttrRefs.end())
	{
		return NS_FAIL;
	}
	CNSAttributeRefMap *pRefMap = (*TempItr).second;
	if(pRefMap != NULL)
	{
		CNSAttributeRef *pRef =	pRefMap->getAttrRef(1); // CID will be 1 only
		if(pRef)
		{
			strValue = pRef->getAttributeValue();
		}
	}
	return NS_SUCCESS;
}

void CNSFeature::setXData(STRING_MAP& mapOfXData)
{
	NSSTRING strTemp;
	if(NS_SUCCESS == getValue(mapOfXData, _T("FID"), strTemp))
		m_nFID = NSATOI(strTemp.c_str());
	getValue(mapOfXData, _T("Usage"), m_strUsage);
	getValue(mapOfXData, _T("Orientation"), m_strOrientation);
	getValue(mapOfXData, _T("State"), m_strState);
    getValue(mapOfXData, _T("TEXT_LABEL"), m_strTextLabel);
    if(NS_SUCCESS == getValue(mapOfXData, _T("FNO"), strTemp))
		m_nFNO = NSATOI(strTemp.c_str());
	m_compContainer.setXData(mapOfXData);
	m_relationship.setXData(mapOfXData);

}

void CNSFeature::setXData(ATT_MAP &mapOfAttrRefs)
{
	setBlockAttributes(mapOfAttrRefs);
	getValue(mapOfAttrRefs, _T("USAGE"), m_strUsage);
	getValue(mapOfAttrRefs, _T("ORIENTATION"), m_strOrientation);
}

void CNSFeature::setObject(CNSCompositeContainer *pCompositeContainer)
{
	m_pContainer = pCompositeContainer;
}

/*!
@Brief			Read the attribute information from DWG as XData and store
@Param [in]		string map of attribute
@Return			true/false
@Date			27-07-2007
*/
int CNSFeature::setAttributes(std::map<NSSTRING, NSSTRING> &mapBlkAtt)
{
	TCHAR szQuery[_MAX_PATH + 1];
//	NSSPRINTF(szQuery, _T("Select ANO from FeatureAttributes where FNO = %d and Required = 1"), m_nFNO);
	NSSPRINTF(szQuery, _T("Select ANO from FeatureAttributes where FNO = %d and Required <> 0"), m_nFNO);
	int nRetVal = m_pDbMgr->executeQuery(szQuery);
	if(nRetVal == -1)
	{
		WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'FeatureAttributes\'"));
		return NS_FAIL;
	}
	std::vector<_variant_t> vValues;
	m_pDbMgr->getValue(_T("ANO"), vValues);
	std::map<NSSTRING, NSSTRING>::iterator mapIter;
	NSSTRING strAttDwgName;
	NSSTRING strAttName;
	std::vector<_variant_t>::iterator resultIter;
	for(resultIter = vValues.begin(); resultIter != vValues.end(); resultIter++)
	{
		int ANO = (*resultIter);
		NSSPRINTF(szQuery, _T("Select * from Attributes where ANO = %d"), ANO, strAttDwgName.c_str());
		int nRetVal = m_pDbMgr->executeQuery(szQuery);
		if(nRetVal == -1)
		{
			WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'Attributes\'"));
			return NS_FAIL;
		}
		_variant_t strAName;
		_variant_t strADWGName;
		bool bResult1 = m_pDbMgr->getValue(_T("AName"), strAName);
		bool bResult2 = m_pDbMgr->getValue(_T("AttNameInDWG"), strAName);

		if(bResult1 && bResult2)
		{
            for(mapIter = mapBlkAtt.begin(); mapIter != mapBlkAtt.end(); mapIter++)
            {
              //  if(NSSTRCMP(((NSSTRING)(strAName.bstrVal)).c_str(), ((*mapIter).second).c_str()) == 0)
                if(NSSTRCMP((strAName.bstrVal), ((*mapIter).first).c_str()) == 0)
                {
                   	m_attributeContainer.addAttribute(1, ANO, (NSSTRING)(strAName.bstrVal), (*mapIter).second);
                    break;
                }
            }
            if( mapIter == mapBlkAtt.end())
            {
				m_attributeContainer.addAttribute(1, ANO, (NSSTRING)(strAName.bstrVal), _T(""));
            }

			//mapIter = mapBlkAtt.find((NSSTRING)(strAName.bstrVal));
			//if(mapIter != mapBlkAtt.end())
			//{
			//	m_attributeContainer.addAttribute(1, ANO, (NSSTRING)(strAName.bstrVal), (*mapIter).second);
			//}
			//else
			//{
			//	m_attributeContainer.addAttribute(1, ANO, (NSSTRING)(strAName.bstrVal), _T(""));
			//}
		}
	}
	return NS_SUCCESS;
}

/*!
@Brief			Read the attribute information and if it has component entry in 
				ComponentGeomertyType table then create TextPointGeometry componet
@Param [in]		map of attribute
@Return			succcess/failure
@Date			27-07-2007
*/
int CNSFeature::setAttributes(ATT_MAP &mapOfBlkAtt)
{
	TCHAR szQuery[_MAX_PATH + 1];
	int nRetVal = -1;
	//	CNSAttributeRef *pAttrRef = NULL;
	ATT_MAP::iterator AttrRefIter;
	NSSTRING strAttrName;
	std::map<NSSTRING, int> mapOfNameCNO;
    std::map<int, NSSTRING> mapOfDuplicateAttr;
	std::map<NSSTRING, int>::iterator AttrNoIter;
	std::map<int, NSSTRING>::iterator DuplAttrNoIter;
	{
		NSSPRINTF(szQuery, _T("Select CNO from FeatureComponents where FNO = %d"),
			m_nFNO);
		nRetVal = m_pDbMgr->executeQuery(szQuery);
		if(nRetVal == -1)
		{
			WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'FeatureComponents\'"));
			return NS_FAIL;
		}
	}
	std::vector<variant_t> vectCNO;
	m_pDbMgr->getValue(_T("CNO"), vectCNO);

	variant_t varAttrName;
	for(int nCNOCntr = 0; nCNOCntr < (int)vectCNO.size(); nCNOCntr++)
	{
		NSSPRINTF(szQuery, _T("Select AttributeNameDWG from FeatureComponents where FNO = %d and CNO = %d"),
			m_nFNO, vectCNO.at(nCNOCntr).intVal);
		m_pDbMgr->executeQuery(szQuery);
		if(true == m_pDbMgr->getValue(_T("AttributeNameDWG"), varAttrName))
		{
			NSSTRING strAttNameInDWG = (NSSTRING)(varAttrName.bstrVal);
			NSSTRUPR((TCHAR*)(strAttNameInDWG.c_str()), strAttNameInDWG.size()+1 );
            AttrNoIter = mapOfNameCNO.find((TCHAR*)strAttNameInDWG.c_str());
            if(AttrNoIter != mapOfNameCNO.end())
            {
                //This is the special case where the one attribute is to be represented as two components inside XML
                 mapOfDuplicateAttr.insert(std::map<int, NSSTRING>::value_type(vectCNO.at(nCNOCntr).iVal,strAttNameInDWG.c_str()));
            }
            else
            {
			    mapOfNameCNO.insert(std::map<NSSTRING, int>::value_type((TCHAR*)(strAttNameInDWG.c_str()), vectCNO.at(nCNOCntr).iVal));
            }
		}

	}

	NSSPRINTF(szQuery, _T("Select ANO from FeatureAttributes where FNO = %d and Required = 1"), m_nFNO);
	nRetVal = m_pDbMgr->executeQuery(szQuery);
	if(nRetVal == -1)
	{
		WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'FeatureAttributes\'"));
		return NS_FAIL;
	}
	std::vector<_variant_t> vValues;
	m_pDbMgr->getValue(_T("ANO"), vValues);
	std::map<NSSTRING, NSSTRING>::iterator mapIter;
	NSSTRING strAttDwgName;
	NSSTRING strAttName;
	std::vector<_variant_t>::iterator resultIter;
	for(resultIter = vValues.begin(); resultIter != vValues.end(); resultIter++)
	{
		int ANO = (*resultIter);
		NSSPRINTF(szQuery, _T("Select * from Attributes where ANO = %d"), ANO);
		int nRetVal = m_pDbMgr->executeQuery(szQuery);
		if(nRetVal == -1)
		{
			WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'Attributes\'"));
			return NS_FAIL;
		}
		_variant_t strAName;
		_variant_t strADWGName;
		bool bResult1 = m_pDbMgr->getValue(_T("AName"), strAName);
		bool bResult2 = m_pDbMgr->getValue(_T("AttNameInDWG"), strADWGName);
		CNSAttributeRefMap *pAttrRefMap = NULL;
		CNSAttributeRef *pAttRef = NULL;
		if(bResult1 && bResult2)
		{
			NSSTRING strAttNameInDWG = (NSSTRING)(strADWGName.bstrVal);
			NSSTRUPR((TCHAR*)strAttNameInDWG.c_str(),strAttNameInDWG.size()+ 1);
			AttrRefIter = mapOfBlkAtt.find(strAttNameInDWG);
			if(AttrRefIter != mapOfBlkAtt.end())
			{
				pAttrRefMap = (*AttrRefIter).second;
				if(pAttrRefMap != NULL)
				{
					AttrNoIter = mapOfNameCNO.find((TCHAR*)strAttNameInDWG.c_str());
					pAttRef = pAttrRefMap->getAttrRef(1);
					if(pAttRef)
					{
						if(AttrNoIter != mapOfNameCNO.end())
						{
							if(m_nFNO != 369)
							{
								m_compContainer.AddComponent(pAttRef->getCompGeometry(), (*AttrNoIter).second, 1);
                                //This is the special case where the one attribute is to be represented as two components inside XML
                                if(mapOfDuplicateAttr.size() > 0)
                                {
                                    for(DuplAttrNoIter = mapOfDuplicateAttr.begin(); DuplAttrNoIter != mapOfDuplicateAttr.end(); DuplAttrNoIter++)
                                	{
                                       if(NSSTRCMP(((*DuplAttrNoIter).second).c_str(), strAttNameInDWG.c_str()) == 0)
                                       {
						                	m_compContainer.AddComponent(pAttRef->getCompGeometry(), (*DuplAttrNoIter).first, 1);
                                       }
                                    }
                                }
							}
						}
						m_attributeContainer.addAttribute(1, ANO, (NSSTRING)(strAName.bstrVal), pAttRef->getAttributeValue());
					}
				}
			}
		}
	}

	NSSTRING strFeatureName = m_pContainer->getAt(0)->getName();
	createAttributes(strFeatureName,m_nFNO);


	return NS_SUCCESS;
}

void CNSFeature::setBlockAttributes(ATT_MAP &mapAttrRef)
{
	ATT_MAP::iterator itr;
	for(itr = mapAttrRef.begin(); itr != mapAttrRef.end(); itr++)
	{
		NSSTRING strAttName = (*itr).first;
		CNSAttributeRefMap *pRef = (*itr).second;
		CNSAttributeRef *pAttRef = pRef->getAttrRef(1);
		NSSTRING strAttValue = pAttRef->getAttributeValue();
		m_mapAttribute.insert(ATTRIBUTE_MAP::value_type(strAttName, strAttValue));
	}
}


NSSTRING CNSFeature::getQueryString(NSSTRING strQueryString)
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

int CNSFeature::assignRepresentation(CNSComponent *pComponent)
{
	if(pComponent == NULL) return NS_FAIL;

	TCHAR szQuery[_MAX_PATH + 1];
	NSSPRINTF(szQuery, _T("Select CompRepresentation from FeatureComponents where FNO = %d and CNO = %d"), m_nFNO, pComponent->getComponentNumber());
    int nDBRet = m_pDbMgr->executeQuery(szQuery);
	if(-1 == nDBRet)
	{
		WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'FeatureComponents\'"));
		return NS_FAIL;
	}
    variant_t varRepresentation;
    bool bGeoType = m_pDbMgr->getValue(_T("CompRepresentation"), varRepresentation);
	if(!bGeoType)
	{
//		TCHAR szMsg[_MAX_PATH + 1];
//		NSSPRINTF(szMsg, _T("CompRepresentation not found from FeatureComponents table for ComponentNumber = %d"), pComponent->getComponentNumber());
//		WriteToLog(LOG_OTHER, szMsg);
		return NS_FAIL;
	}
	pComponent->setCompRepresentation(varRepresentation.iVal);
}

bool CNSFeature::getInsertionPoint(CNSPoint3D &insertionPt)
{
	return m_compContainer.getInsertionPoint(insertionPt);
}

bool CNSFeature::getConnectivityData(std::vector<int> &vectNode1, std::vector<int> &vectNode2)
{
	return m_relationship.getConnectivityData(vectNode1, vectNode2);
}

bool CNSFeature::setCollinearPoints(std::vector<CNSPoint3D> &vectNode1Pts, std::vector<CNSPoint3D> &vectNode2Pts)
{
	return m_compContainer.setCollinearPoints(vectNode1Pts, vectNode2Pts);
}

int CNSFeature::getFNO() const
{
	return m_nFNO;
}

int CNSFeature::getFID() const
{
	return m_nFID;
}

std::wstring CNSFeature::getUsage() 
{
	STRING_MAP mapXdata;
	STRING_MAP::iterator it;
	m_attributeContainer.getXData(mapXdata);
	it=mapXdata.find(_T("USAGE"));
	if (it == mapXdata.end())
		return L"";
		
	return (*it).second;
}

int CNSFeature::createAttributes(NSSTRING strFeatureName, int nFNO)
{
	TCHAR szQuery[_MAX_PATH + 1];
	NSSPRINTF(szQuery, _T("Select * from FeatureAttributes where FNO = %d"), nFNO);
	int nRetVal = m_pDbMgr->executeQuery(szQuery);
	if(nRetVal == -1)
	{
		WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'FeatureAttributes\'"));
		return NS_FAIL;
	}
	std::vector<_variant_t> vValues;
	m_pDbMgr->getValue(_T("ANO"), vValues);
	std::map<NSSTRING, NSSTRING>::iterator mapIter;
	NSSTRING strAttDwgName;
	NSSTRING strAttName;
	std::vector<_variant_t>::iterator resultIter;
	for(resultIter = vValues.begin(); resultIter != vValues.end(); resultIter++)
	{
		int ANO = (*resultIter);
		NSSPRINTF(szQuery, _T("Select * from FeatureAttributes where FNO = %d and ANO = %d"), nFNO, ANO);
		int nRetVal = m_pDbMgr->executeQuery(szQuery);
		if(nRetVal == -1)
		{
			WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'FeatureAttributes\'"));
			return NS_FAIL;
		}
		_variant_t varAName;
		_variant_t varRequired;
		_variant_t varAValue;
		NSSTRING strColumnName, strAName, strValue;
		bool bResult1 = m_pDbMgr->getValue(_T("AName"), varAName);
		bool bResult2 = m_pDbMgr->getValue(_T("Required"), varRequired);
		if(bResult1 && bResult2)
		{
			if(varRequired.iVal == 2)
			{
				int nRetVal;
				strColumnName = (NSSTRING)((_bstr_t)(varAName.bstrVal));

				NSSPRINTF(szQuery, _T("Select fldAttributeValue from DefaultAttributes where  fldFeatureName = \'%s\' and fldAttributeName = \'%s\'"), strFeatureName.c_str(), strColumnName.c_str());
				             //Value = SELECT fldAttributeValue FROM DefaultAttributes WHERE  fldFeatureName = 'ABS'  and fldAttributeName = 'STATE'
				
				nRetVal = m_pDbMgr->executeQuery(szQuery);

				if(nRetVal == -1)
				{
					continue;		
				}
				
				NSSTRING strAttributeValue = _T("fldAttributeValue");
				bool bResult1 = m_pDbMgr->getValue((TCHAR*)strAttributeValue.c_str(), varAValue);
				if(bResult1)
				{
                    if(varAValue.vt == VT_BSTR)
                    {
					    strValue = (NSSTRING)((_bstr_t)(varAValue.bstrVal));
					    m_attributeContainer.addAttribute(1, ANO, strColumnName, strValue);
                    }
                    else
					{
						TCHAR szQuery[_MAX_PATH + 1];
						NSSPRINTF(szQuery, _T("Attribute cann't be added. The attribute column \'%s\' in NewLinearFeatures table should have \'Text\' datatype."), strColumnName.c_str());
						
						WriteToLog(LOG_OTHER, szQuery );
					}
				}
			}
			else if(varRequired.iVal == 3)
			{
				int nIndex = (int)strFeatureName.find_last_of(_T("_"));
				if(nIndex != -1)
				{
					NSSTRING strCode = strFeatureName.substr(nIndex+1, strFeatureName.length());
					strAName = (NSSTRING)((_bstr_t)(varAName.bstrVal));
					if(NSSTRCMP(strCode.c_str(), _T("NEW")) != 0)
						m_attributeContainer.addAttribute(1, ANO, strAName.c_str(), strCode);
				}
			}
            else if(varRequired.iVal == 4)
			{
                strAName = (NSSTRING)((_bstr_t)(varAName.bstrVal));
                m_attributeContainer.addAttribute(1, ANO, strAName.c_str(), m_strTextLabel);
            }
		}
	}
	
	return NS_SUCCESS;
}

CNSRelationShip CNSFeature::getRelationship()
{
	return m_relationship;
}

//This is the code to extract the "Block name" from the 
int CNSFeature::getFinalBlockName(NSSTRING& strBlkName,
								   std::map<int, CNSAttribute*>& mapAttributes,
								   int nFNO,
								   NSSTRING& strUsage,
								   NSSTRING& strOri)
{
	BOOL bCAD_LEVEL = FALSE;
	BOOL bCAD_COLOR = FALSE;
	NSSTRING sCAD_LEVEL_Value ;
	NSSTRING sCAD_COLOR_Value ;

	for(std::map<int, CNSAttribute*>::iterator ITMapOfAtt = mapAttributes.begin();
		ITMapOfAtt != mapAttributes.end();
		ITMapOfAtt++)
	{

		int nVal = ITMapOfAtt->first;  
		CNSAttribute*  ptrNSAttribute  = ITMapOfAtt->second ;
		NSSTRING sAttr =  ptrNSAttribute->getAttributeName();
		NSSTRING sCAD_LEVEL = L"CAD_LEVEL" ;
		NSSTRING sCAD_COLOR = L"CAD_COLOR" ;

		if(sAttr.compare(sCAD_LEVEL)==0)
		{
			bCAD_LEVEL = TRUE;
			ptrNSAttribute->getDataValue(1, sCAD_LEVEL_Value);
		}
		else if(sAttr.compare(sCAD_COLOR)==0)
		{
			bCAD_COLOR = TRUE;
			ptrNSAttribute->getDataValue(1, sCAD_COLOR_Value);
		}

	}

	if(bCAD_LEVEL==FALSE) 
		sCAD_LEVEL_Value = L"0";

	if(bCAD_COLOR==FALSE) 
		sCAD_COLOR_Value = L"0";


	if(bCAD_COLOR == FALSE && bCAD_LEVEL==FALSE)
		return -1;


	TCHAR szQuery[1024] = {NULL};
	NSSPRINTF(szQuery, _T("Select ExtBlock from AlternateExtBlock where FNO = %d and USAGE %s and ORIENTATION %s and CAD_LEVEL = %s and CAD_COLOR = %s "),
						  nFNO, strUsage.c_str(), strOri.c_str(),sCAD_LEVEL_Value.c_str(),sCAD_COLOR_Value.c_str());
	
	int nRetVal = m_pDbMgr->executeQuery(szQuery);
	if(nRetVal == -1)
	{
		WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'BlockFeatures\'"));
	//	delete compContainer;
		//delete mapofXData;
		//featureCont.release();
	//	delete mapAttrRef;
		return NS_FAIL;
	}
	variant_t layer, block;
	bool bExtBlk = m_pDbMgr->getValue(_T("ExtBlock"), block);
	if(bExtBlk)
		strBlkName = (NSSTRING)((_bstr_t)(block.bstrVal));


	
}

/*
@brief			code to compare criteria field  from FeatureComponents table and XML File
@Param(in)		strCriteria:- string TAG = VALUE
@Param(out)		
*/

bool CNSFeature::readCriteria(NSSTRING strCriteria)
{
	NSSTRING strTag, strValue;
	strCriteria.erase(remove_if(strCriteria.begin(),strCriteria.end(), isspace), strCriteria.end());
	if(NSSTRCMP(strCriteria.c_str(),_T("")) != 0)
	{
	   strTag = strCriteria.substr(0,strCriteria.find(_T("=")));
	   if(-1 != strCriteria.find(_T("=")))
	   {
	      strValue = strCriteria.substr(strCriteria.find(_T("=")) + 1, (strCriteria.size() - strCriteria.find(_T("="))));
		  strValue.erase(remove_if(strValue.begin(),strValue.end(), ispunct), strValue.end());
	   }
	
	   if(NSSTRCMP(strTag.c_str(), _T("ORIENTATION")) == 0)
	   {
		  if(NSSTRCMP(m_strOrientation.c_str(), strValue.c_str()) != 0)
		  return false;
	   }
	   else if(NSSTRCMP(strTag.c_str(), _T("USAGE")) == 0)
	   {
	 	  if(NSSTRCMP(m_strUsage.c_str(), strValue.c_str()) != 0)
		  return false;
	   }
	   else if(NSSTRCMP(strTag.c_str(), _T("STATE")) == 0)
	   {
		  if(NSSTRCMP(m_strState.c_str(), strValue.c_str()) != 0)
		  return false;
	   }
	   else
	   {
		  return false;
	   }
	}
	return true;

}


