#include "StdAfx.h"
#include "..\..\Include\NSBlock.h"

#include "NSImportedFeatureContainer.h"

int gMaxFID = 0;
// Constructor
CNSImportedFeatureContainer::CNSImportedFeatureContainer(void)
{
	m_nNewFeatures = 0;
	m_nModFeatures = 0;
	m_nDelFeatures = 0;
	m_pElement = NULL;
	m_pDwgMgr = NULL;
	m_pDbMgr = NULL;
	m_nFeatureNumber = 0;
}

// Destructor
CNSImportedFeatureContainer::~CNSImportedFeatureContainer(void)
{
	std::vector<CNSFeature*>::iterator featureItr;
	for(featureItr = m_vectModifiedFeatures.begin(); featureItr != m_vectModifiedFeatures.end(); featureItr++)
	{
		delete (*featureItr);
	}
	for(featureItr = m_vectNewFeatures.begin(); featureItr != m_vectNewFeatures.end(); featureItr++)
	{
		delete (*featureItr);
	}
}

/*!
@Brief			This function reads new, modified and deleted feature information from Drawing file.
@Return			NS_SUCCESS : If reading process successful.
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSImportedFeatureContainer::readDWG()
{
	std::map<NSSTRING, NSSTRING> mapBlkAtt;
	m_nFeatureNumber = -1;
	std::vector<CNSFeatureContainer*> vectModContainer;
	m_pDwgMgr->selectBlocks(vectModContainer, _T("MOD"), _T("EXT"));
    
    /*std::vector<CNSFeatureContainer*>::iterator modIter;
    for(modIter = vectModContainer.begin(); modIter != vectModContainer.end(); modIter++)
	{	
        STRING_MAP* mapXdata =(*modIter)->m_mapOfXData;
        std::map<NSSTRING,NSSTRING>::iterator mapItr;
        mapItr = mapXdata->find(_T("FID"));
        if(mapItr != mapXdata->end())
        {
            if(NSSTRLEN((*mapItr).second.c_str())>0)
		        vectModContainer.erase(modIter);
        }
        else
            vectModContainer.erase(modIter));
	}*/
	createFeatures(_T("MOD"), _T("EXT"), CAT_BLOCK, vectModContainer, m_vectModifiedFeatures);

	std::vector<CNSFeatureContainer*> vectModLinearContainer;
    std::vector<NSSTRING> vectExcludeLayers;
	m_pDwgMgr->selectPolyLines(vectModLinearContainer, vectExcludeLayers, _T("MOD"), _T("all"));
	createFeatures(_T("MOD"), _T("EXT"), CAT_SEGMENT, vectModLinearContainer, m_vectModifiedFeatures);

	std::vector<CNSFeatureContainer*> vectNewContainer;
	m_pDwgMgr->selectBlocks(vectNewContainer, _T("NEW"), _T("NEW"));

		//std::map<NSSTRING, CNSFeatureContainer*> mapNestedFeatureContainer;		
		getNestedFeatures(vectNewContainer, mapNestedFeatureContainer);
		normalizeSubFeatures(mapNestedFeatureContainer);

	//////createFeatures(_T("NEW"), _T("NEW"),CAT_BLOCK, vectNewContainer, m_vectNewFeatures);

	std::vector<CNSFeatureContainer*> vectCablePolyContainer;
    vectExcludeLayers.clear();
    
    TCHAR szQuery[_MAX_PATH + 1];
    NSSPRINTF(szQuery, _T("Select NewLayer from NewLinearFeatures where  NumberOfNodes = 0"));
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
//    std::vector<NSSTRING> szCadastreLayers;
	if(-1 != nDBRet)
	{
        std::vector<variant_t> vVectNewLayer;
        CNSDatabaseMgr::getInstance()->getValue(_T("NewLayer"), vVectNewLayer);
    
        std::vector<variant_t>::iterator varientVectItr;
        for(varientVectItr = vVectNewLayer.begin(); varientVectItr != vVectNewLayer.end(); varientVectItr++)
	    {
            vectExcludeLayers.push_back((NSSTRING)(*varientVectItr).bstrVal);
        }
      }
	m_pDwgMgr->selectPolyLines(vectCablePolyContainer, vectExcludeLayers, _T("NEW"), _T("all"), true, 1);
    createFeatures(_T("NEW"), _T("NEW"),CAT_SEGMENT, vectCablePolyContainer, m_vectNewFeatures);

	finalizeSubFeatures(mapNestedFeatureContainer);
	createSubFeatures(mapNestedFeatureContainer);
    createFeatures(_T("NEW"), _T("NEW"),CAT_BLOCK, vectNewContainer, m_vectNewFeatures);

	

   std::vector<CNSFeatureContainer*> vectDPContainer;
   m_pDwgMgr->selectPolyLines(vectDPContainer, vectExcludeLayers, _T("NEW"), _T("all"), true, 2);
   createFeatures(_T("NEW"), _T("NEW"),CAT_SEGMENT, vectDPContainer, m_vectNewFeatures);


	/*std::vector<CNSFeatureContainer*> vectNewPolyContainer;
	m_pDwgMgr->selectPolyLines(vectNewPolyContainer, _T("DEVELOPER_PLAN"), _T("all"), false);
	createFeatures(_T("NEW"), _T("NEW"),CAT_SEGMENT, vectNewPolyContainer, m_vectNewFeatures);*/

	/*std::vector<CNSFeatureContainer*> vectLabels;
    TCHAR szQuery[_MAX_PATH + 1];
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
	        m_pDwgMgr->selectTextLabels(vectLabels, szCadastreLayers);
            createFeatures(_T("NEW"), _T("NEW"),CAT_LABEL, vectLabels, m_vectNewFeatures);
        }
    }*/
    vectExcludeLayers.clear();
	std::vector<CNSFeatureContainer*> vectDelCableContainer;
	m_pDwgMgr->selectPolyLines(vectDelCableContainer, vectExcludeLayers, _T("_DEL"), _T("all"), false, 0);
	createDelPolylines(vectDelCableContainer);

	std::vector<CNSDelBlock*> vDelBlks;
	std::vector<CNSDelBlock*>::iterator delIter;
	m_pDwgMgr->selectDeletedBlocks(vDelBlks);
	for(delIter = vDelBlks.begin(); delIter != vDelBlks.end(); delIter++)
	{	
        if(NSSTRLEN((*delIter)->FID.c_str()) > 0)
		    m_mapDelFeatures.insert(std::map<NSSTRING, NSSTRING>::value_type((*delIter)->FID, (*delIter)->FNO));
	}
	CNSDelBlock *pDelBlk = NULL;
	for(delIter = vDelBlks.begin(); delIter != vDelBlks.end(); delIter++)
	{	
		pDelBlk = *delIter;
		delete pDelBlk;
	}

	
	mapNestedFeatureContainer.clear();

	return NS_SUCCESS;
}

/*!
@Brief			This function writes new, modified and deleted feature information into XML file
@Return			NS_SUCCESS : If reading process successful.
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSImportedFeatureContainer::writeXML()
{
	// Write Modified Feature information.
	CNSTElement oModFeaturesNode = m_pElement->createNode(_T("ModifiedFeatures"), _T(""), DEFAULT_NAMESPACE);
	std::vector<CNSFeature*>::iterator featureItr;
	for(featureItr = m_vectModifiedFeatures.begin(); featureItr != m_vectModifiedFeatures.end(); featureItr++)
	{
		int fNo =(*featureItr)->getFNO();
		// For End Shackle(Block) and BondArc(PolyLine) only import whose Usage is LV or HV
		if(fNo == 123 || fNo == 130) 
		{
			if((*featureItr)->getUsage() == _T("LV") || (*featureItr)->getUsage() == _T("HV"))
			{
			writeXMLFeatureNode(&oModFeaturesNode, (*featureItr));
			m_nModFeatures = m_nModFeatures + 1;
			}		
			else
				continue;
		}
		//Other than End Shackle and BondArc
		else
		{
			writeXMLFeatureNode(&oModFeaturesNode, (*featureItr));
			m_nModFeatures = m_nModFeatures + 1;
		}
	}



	//modifyCableEndConnections();

	// Write New Feature information
	CNSTElement oNewFeaturesNode = m_pElement->createNode(_T("NewFeatures"), _T(""), DEFAULT_NAMESPACE);
	for(featureItr = m_vectNewFeatures.begin(); featureItr != m_vectNewFeatures.end(); featureItr++)
	{
		// For End Shackle(Block) and BondArc(PolyLine) only import whose Usage is LV or HV
		int fNo =(*featureItr)->getFNO();
		if(fNo == 123 || fNo == 130) 
		{
			if((*featureItr)->getUsage() == _T("LV") || (*featureItr)->getUsage() == _T("HV"))
			{
				writeXMLFeatureNode(&oNewFeaturesNode, (*featureItr));
				m_nNewFeatures = m_nNewFeatures + 1;
			}
			else
				continue;
		}
		//Other than End Shackle and BondArc
		else
		{
		writeXMLFeatureNode(&oNewFeaturesNode, (*featureItr));
		m_nNewFeatures = m_nNewFeatures + 1;
		}
	}

	// Write Delete feature information
	CNSTElement oDelFeaturesNode = m_pElement->createNode(_T("DeletedFeatures"), _T(""), DEFAULT_NAMESPACE);
	// For Deleted Features
	std::map<NSSTRING, NSSTRING>::iterator delIter;
	for(delIter = m_mapDelFeatures.begin(); delIter != m_mapDelFeatures.end(); delIter++)
	{
		CNSTElement oDelFeature = oDelFeaturesNode.createNode(_T("DeletedFeature"), _T(""), DEFAULT_NAMESPACE);
		oDelFeature.createNode(_T("FID"), (*delIter).first, DEFAULT_NAMESPACE);
		oDelFeature.createNode(_T("FNO"), (*delIter).second, DEFAULT_NAMESPACE);
		m_nDelFeatures = m_nDelFeatures + 1;
	}
	return NS_SUCCESS;
}

/*!
@Brief			This function writes Feature node into XML file.
@Param [in]		pElement			:	Imported Features node
@Param [in]		pFeature			:	Feature object
@Return			NS_SUCCESS : If reading process successful.
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSImportedFeatureContainer::writeXMLFeatureNode(CNSTElement* pElement, CNSFeature* pFeature)
{
	

	CNSTElement featureNode = pElement->createNode(_T("Feature"), _T(""), DEFAULT_NAMESPACE);
	pFeature->setElement(&featureNode);
	if(NS_SUCCESS != pFeature->writeXML())
		return NS_FAIL;
	return NS_SUCCESS;
}

void CNSImportedFeatureContainer::setObject(CNSDWGMgr* pDwgMgr)
{
	m_pDwgMgr = pDwgMgr;
}

void CNSImportedFeatureContainer::setObject(CNSDatabaseMgr* pDbMgr)
{
	m_pDbMgr = pDbMgr;
}

void CNSImportedFeatureContainer::setElement(CNSTElement* pElement)
{
	m_pElement = pElement;
}

/*!
@Brief			This function creates the feature information depends on the feature type.
@Param [in]		strLayerType			:	Layer name
@Param [in]		strFeatureType			:	Feature type
@Param [in]		vectFeatureContainer	:	vector of features
@Param [in]		enCategory				:	feature category
@Param [in]		vectFeatures			:	vector of features
@Return			NS_SUCCESS : If reading process successful.
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSImportedFeatureContainer::createFeatures(NSSTRING strLayerType, NSSTRING strFeatureType, FeatureCategory enCategory, std::vector<CNSFeatureContainer*> &vectFeatureContainer, std::vector<CNSFeature*> &vectFeatures)
{
	updateFeatures(vectFeatureContainer/*, mapNestedFeatureContainer*/);

	CNSFeatureContainer* pFeatContainer = NULL;
	std::vector<CNSFeatureContainer*>::iterator it;

	for(it = vectFeatureContainer.begin(); it != vectFeatureContainer.end(); it++)
	{
		pFeatContainer = (*it);
		if(!pFeatContainer) continue;
		if ( pFeatContainer->getNestedFeatureFlag() == true) 
			continue;

		CNSFeature *pFeature = new CNSFeature;
		pFeature->setObject(pFeatContainer->m_pContainer);
		pFeature->setFeatureType(strLayerType.c_str());
		pFeature->setCategory(enCategory);
		pFeature->setObject(m_pDbMgr);
		if(NSSTRCMP(strLayerType.c_str(), _T("NEW")) == 0)
		{
			pFeature->setXData(*pFeatContainer->m_mapOfXData);

			if(enCategory != CAT_SEGMENT)
			{
				pFeature->setXData(*pFeatContainer->m_mapOfBlkAtt);
			}
		}
		else
		{
			pFeature->setXData(*pFeatContainer->m_mapOfXData);
		}
		if(NS_SUCCESS != pFeature->readDWG())
		{
			delete pFeature;
            continue;
		//	return NS_FAIL;		
		}
		if(enCategory != CAT_BLOCK && enCategory != CAT_LABEL)
		{
            if(NSSTRCMP(strLayerType.c_str(), _T("NEW")) != 0)
			    pFeature->setAttributes(*pFeatContainer->m_mapOfXData);
            else
			    pFeature->setAttributes(*pFeatContainer->m_mapOfBlkAtt);
		}
		else
		{
			pFeature->setAttributes(*pFeatContainer->m_mapOfBlkAtt);
		}
		if(pFeature->getFID() < gMaxFID)
		{
			gMaxFID = pFeature->getFID();
		}
		vectFeatures.push_back(pFeature);
	}

	for(it = vectFeatureContainer.begin(); it != vectFeatureContainer.end(); it++)
	{
		pFeatContainer = (*it);
		delete pFeatContainer->m_mapOfBlkAtt;
		delete pFeatContainer->m_mapOfXData;
		delete pFeatContainer->m_pContainer;
		delete pFeatContainer;
	}

	return NS_SUCCESS;
}

int CNSImportedFeatureContainer::createSubFeature(NSSTRING strLayerType, NSSTRING strFeatureType,FeatureCategory enCategory,   std::vector<CNSFeatureContainer*> &vectFeatureContainer, std::vector<CNSFeature*> &vectFeatures)
{
	//updateFeatures(vectFeatureContainer/*, mapNestedFeatureContainer*/);

	CNSFeatureContainer* pFeatContainer = NULL;
	std::vector<CNSFeatureContainer*>::iterator it;
	for(it = vectFeatureContainer.begin(); it != vectFeatureContainer.end(); it++)
	{
		pFeatContainer = (*it);
		if ( pFeatContainer->getNestedFeatureFlag() == true) 
			continue;

		COMPONENT_TYPE enType = pFeatContainer->getCompType(0);
		if(enType == CAT_INVALID)
			continue;

		if(enType == COMP_POINT)
			enCategory = CAT_BLOCK;
		else
			enCategory = CAT_SEGMENT;

		CNSFeature *pFeature = new CNSFeature;
		pFeature->setNestedFeature(true);
		pFeature->setObject(((*it)->m_pContainer));
		pFeature->setFeatureType(strLayerType.c_str());
		pFeature->setCategory(enCategory);
		pFeature->setObject(m_pDbMgr);
		if(NSSTRCMP(strLayerType.c_str(), _T("NEW")) == 0)
		{
			pFeature->setXData(*(*it)->m_mapOfXData);
			if(enCategory != CAT_SEGMENT)
				pFeature->setXData(*(*it)->m_mapOfBlkAtt);
		}
		else
		{
			pFeature->setXData(*(*it)->m_mapOfXData);
		}
		if(NS_SUCCESS != pFeature->readDWG())
		{
			delete pFeature;
            continue;
		//	return NS_FAIL;		
		}
		if(enCategory != CAT_BLOCK && enCategory != CAT_LABEL)
		{
            if(NSSTRCMP(strLayerType.c_str(), _T("NEW")) != 0)
			    pFeature->setAttributes(*(*it)->m_mapOfXData);
            else
			    pFeature->setAttributes(*(*it)->m_mapOfBlkAtt);
		}
		else
		{
			pFeature->setAttributes(*(*it)->m_mapOfBlkAtt);
		}
		if(pFeature->getFID() < gMaxFID)
		{
			gMaxFID = pFeature->getFID();
		}
		vectFeatures.push_back(pFeature);
	}

	//for(it = vectFeatureContainer.begin(); it != vectFeatureContainer.end(); it++)
	//{
	//	pFeatContainer = (*it);
	//	delete pFeatContainer->m_mapOfBlkAtt;
	//	delete pFeatContainer->m_mapOfXData;
	//	delete pFeatContainer->m_pContainer;
	//	delete pFeatContainer;
	//}

	return NS_SUCCESS;
}

/*!
@Brief			Function used to created deleted componets in XML
@Param [in]		Deleted components vector
@Return			none
@Date			27-07-2007
*/
int CNSImportedFeatureContainer::createDelPolylines(std::vector<CNSFeatureContainer*> &vectDelCableContainer)
{
	// For Deleted Features
	std::vector<CNSFeatureContainer*>::iterator delIter;
	CNSFeatureContainer* pFeatureContainer = NULL;
	CNSFeature feature;
	NSSTRING strFNO, strFID;
	for(delIter = vectDelCableContainer.begin(); delIter != vectDelCableContainer.end(); delIter++)
	{
		pFeatureContainer = (*delIter);
		if(pFeatureContainer->m_mapOfXData != NULL)
		{
			feature.getValue(*pFeatureContainer->getXData(), _T("FID"), strFID);
			feature.getValue(*pFeatureContainer->m_mapOfXData, _T("FNO"), strFNO);
			m_mapDelFeatures.insert(std::map<NSSTRING, NSSTRING>::value_type(strFID, strFNO));
			
		}
	}
	for(delIter = vectDelCableContainer.begin(); delIter != vectDelCableContainer.end(); delIter++)
	{
		pFeatureContainer = *delIter;
		delete pFeatureContainer->m_mapOfBlkAtt;
		delete pFeatureContainer->m_mapOfXData;
		delete pFeatureContainer->m_pContainer;
		delete pFeatureContainer;
	}
	return NS_SUCCESS;
}
/*!
@Brief			Function used to pass cable end points as block insertion points
@Param [in]		
@Return			none
@Date			27-07-2007
*/
void CNSImportedFeatureContainer::modifyCableEndConnections()
{
	std::map<int, CNSPoint3D> mapOfBlkInsertionPts;
	std::map<int, CNSFeature*> mapOfCables;
	std::map<int, CNSPoint3D>::iterator insertionPtItr;
	std::map<int, CNSFeature*>::iterator FeatureItr;
	std::vector<CNSFeature*>::iterator NewFeatItr;
	CNSFeature *pFeature = NULL;
	CNSPoint3D insertionPt;
	FeatureCategory category = CAT_INVALID;
	for(NewFeatItr = m_vectNewFeatures.begin(); NewFeatItr != m_vectNewFeatures.end(); NewFeatItr++)
	{
		pFeature = *NewFeatItr;
		category = pFeature->getCategory();
		if(category == CAT_BLOCK)
		{
			pFeature->getInsertionPoint(insertionPt);
			mapOfBlkInsertionPts.insert(std::map<int, CNSPoint3D>::value_type(pFeature->getFID(), insertionPt));
		}
		else if(category == CAT_SEGMENT)
		{
			mapOfCables.insert(std::map<int, CNSFeature*>::value_type(pFeature->getFID(), pFeature));
		}
	}

	for(FeatureItr = mapOfCables.begin(); FeatureItr != mapOfCables.end(); FeatureItr++)
	{
		std::vector<int> vectNode1FID, vectNode2FID;
		std::vector<CNSPoint3D> vectNode1Pts, vectNode2Pts;
		pFeature = (*FeatureItr).second;
		if(true == pFeature->getConnectivityData(vectNode1FID, vectNode2FID))
		{
			for(int nIDCntr = 0; nIDCntr < (int)vectNode1FID.size(); nIDCntr++)
			{
				insertionPtItr = mapOfBlkInsertionPts.find(vectNode1FID[nIDCntr]);
				if(insertionPtItr != mapOfBlkInsertionPts.end())
				{
					vectNode1Pts.push_back((*insertionPtItr).second);
				}
			}
			for(int nIDCntr = 0; nIDCntr < (int)vectNode2FID.size(); nIDCntr++)
			{
				insertionPtItr = mapOfBlkInsertionPts.find(vectNode2FID[nIDCntr]);
				if(insertionPtItr != mapOfBlkInsertionPts.end())
				{
					vectNode2Pts.push_back((*insertionPtItr).second);
				}
			}
			pFeature->setCollinearPoints(vectNode1Pts, vectNode2Pts);
		}
	}
}

int CNSImportedFeatureContainer::getModFeatures()
{
	return m_nModFeatures;
}

int CNSImportedFeatureContainer::getNewFeatures()
{
	return m_nNewFeatures;
}

int CNSImportedFeatureContainer::getDelFeatures()
{
	return m_nDelFeatures;
}


bool CNSImportedFeatureContainer::isNestedFeature(CNSFeatureContainer* aNewFeature)
{

	std::map<NSSTRING, NSSTRING> mapXData ;
	std::map<NSSTRING, NSSTRING>::iterator xDataIter;
	NSSTRING strBlkName;
	NSSTRING strUsage;
	NSSTRING strOrien;
	TCHAR szQuery[_MAX_PATH + 1];
	CNSCompositeGeometry *pCompGeo = aNewFeature->m_pContainer->getAt(0);
	NSSTRING strLayerName = pCompGeo->getLayername();
	strBlkName = pCompGeo->getName();
	//ATTRIBUTE_MAP::iterator attItr;
	//int nDBRet = -1;
	//bool bIsLinear = false;
	//NSSTRING strMessage = _T("");

	_variant_t varAValue;


	 strUsage =  getATTVal (_T("USAGE"),aNewFeature );
	 strOrien =  getATTVal (_T("ORIENTATION"),aNewFeature );


	NSSPRINTF(szQuery, _T("Select IsNestedFeature from BlockFeatures where NewBlock = \'%s\' and Usage %s and Orientation %s"),
		strBlkName.c_str(), strUsage.c_str(), strOrien.c_str());
	
	    
		
		ATT_MAP *mapOfBlkAt = aNewFeature->getAttributeMap();

		ATTRIBUTE_MAP  mapOfAttribute ;

		ATT_MAP::iterator it;
		for(it = mapOfBlkAt->begin(); it != mapOfBlkAt->end(); it++)
		{
			mapOfAttribute.insert(STRING_MAP::value_type(((*it).first).c_str(),((*it).second)->getAttrRef(1)->getAttributeValue()));			
		}		


		CNSFeature tempFeat;
		NSSTRING strFinalQuery = tempFeat.getQueryString(mapOfAttribute,szQuery);
		int nRetVal = m_pDbMgr->executeQuery(strFinalQuery.c_str());	    


	//strMessage = _T("DataBase Error : Failed to access table \'BlockFeatures\'");

											

	if(nRetVal == -1)
	{
		return false;		
	}

	NSSTRING strIsNestedFeature = _T("IsNestedFeature");
	bool bResult = m_pDbMgr->getValue((TCHAR*)strIsNestedFeature.c_str(), varAValue);

	if(bResult)
	{
        if(varAValue.vt == VT_BOOL)
        {
			bool bValue = (varAValue.boolVal);
			if(bValue == true) aNewFeature->setNestedFeatureFlag(bValue)  ;
			return bValue;
        }
	}

			


return false;
}

NSSTRING CNSImportedFeatureContainer::getATTVal(NSSTRING tagname, CNSFeatureContainer* aNewFeature)
{
	    NSSTRING strAttVal;

	    ATT_MAP::iterator attItr;

	    ATT_MAP* attMap = aNewFeature->getAttributeMap();	

	    CNSAttributeRefMap *pRefMap = NULL;

	    attItr = 	attMap->find(tagname);


		if(attItr != aNewFeature->m_mapOfBlkAtt->end())
		{
			pRefMap = (*attItr).second;	
			ATTREF_MAP MapOfRefs;				
			pRefMap->getAttrReferences(MapOfRefs);			
				
			ATTREF_MAP::iterator AttrRefItr;	
			
			AttrRefItr = MapOfRefs.find(1);

					if(AttrRefItr != MapOfRefs.end())
					{
						CNSAttributeRef* att =  (*AttrRefItr).second;
						
						strAttVal = att->getAttributeValue();

					}
		}

		if(NSSTRCMP(strAttVal.c_str(), _T("")) == 0)
		{
			strAttVal = _T(" is null");
		}
		else
		{
			strAttVal = _T(" = \'") + strAttVal + _T("\'");
		}


	return strAttVal;
}

NSSTRING CNSImportedFeatureContainer::getdataOf(NSSTRING strKey,STRING_MAP *pDataMap )
{
	NSSTRING strSerchResult = _T("");

	STRING_MAP::iterator XDATAItr;

	XDATAItr = pDataMap->find(strKey);					

	if(XDATAItr != pDataMap->end())
	{
		strSerchResult =  (*XDATAItr).second;										
	}

	return strSerchResult;
}

int CNSImportedFeatureContainer::getBitVal(int iDesimalValue, int iBitPos)
{
	int retbit = 0;

	for(int i = 0; i<= iBitPos ; i++)
	{
		retbit = (iDesimalValue >> i) & 1 ;
	}

	return retbit;
	
}

void CNSImportedFeatureContainer::getNestedFeatures(std::vector<CNSFeatureContainer*> vectNewContainer, std::map<NSSTRING, CNSFeatureContainer*> &mapNestedFeatureContainer)
{
	////// CREATING MAP OF NESTED FEATURES ONLY
	for(std::vector<CNSFeatureContainer*>::iterator it = vectNewContainer.begin(); it !=  vectNewContainer.end(); it++)
	{	
		if(isNestedFeature(*it))
		{	
			   	STRING_MAP *m_mapOfXData = (*it)->m_mapOfXData;

				STRING_MAP::iterator XDATAItr;
			
				XDATAItr = m_mapOfXData->find(_T("FID"));					

					if(XDATAItr != m_mapOfXData->end())
					{
						NSSTRING strFID =  (*XDATAItr).second;
						mapNestedFeatureContainer.insert(std::map<NSSTRING, CNSFeatureContainer*>::value_type(strFID,*it));	
					}				
				
		}
	}
	////// CREATING MAP OF NESTED FEATURES ONLY
}


/* normalizeSubFeatures is implemented to update the SubFeatures with actual data from parent where 
parent is referenced by <TAGS>*/
void CNSImportedFeatureContainer::normalizeSubFeatures(std::map<NSSTRING, CNSFeatureContainer*> &mapNestedFeatureContainer)
{
	/////// PROCESSING THE SUB-FEATURES WITH IN THE NESTED FEATURES
	for(std::map<NSSTRING, CNSFeatureContainer*>::iterator NFmapIt = mapNestedFeatureContainer.begin(); NFmapIt !=  mapNestedFeatureContainer.end(); NFmapIt++)
	{	
		
		std::map<NSSTRING, CNSFeatureContainer *> mapOfSubFeatures ;
		(*NFmapIt).second->getSubFeatureMap(mapOfSubFeatures);
			
				std::map<NSSTRING,CNSFeatureContainer *>::iterator SFmapIt;
				for(SFmapIt = mapOfSubFeatures.begin(); SFmapIt != mapOfSubFeatures.end(); SFmapIt++)
				{
					CNSRelationInfo* pRelationInfoOfSubFeat = (*SFmapIt).second->getRelationInfo();
					
					//////////////////////////////////////////////////////////////CONCATING FID
					NSSTRING strParentFID = (*NFmapIt).first ;

					
					NSSTRING strChildFID = pRelationInfoOfSubFeat->getFID(); //NSSTRING strChildFID = pRelationInfoOfSubFeat->m_strFID; 

					if(strChildFID.find(_T("-"))==0)
							strChildFID.replace(0, 1, _T(""));
					

					NSSTRING strConcatinatedFID = strParentFID + strChildFID ;

					
					 pRelationInfoOfSubFeat->setFID(strConcatinatedFID); //pRelationInfoOfSubFeat->m_strFID = strConcatinatedFID; 

					//////////////////////////////////////////////////////////////CONCATING FID



					 ////////////////////////// Update Sub-Features interrelation ship with reference to Parents FID
					 updateSubFeaturesInterRelationShip(strParentFID, pRelationInfoOfSubFeat);
					 ////////////////////////// Update Sub-Features interrelation ship with reference to Parents FID



					////////////////////////////////////////////////////////////// UPDATING Relationships with External features
						STRING_MAP *mapOfXDataOfParentBlock = (*NFmapIt).second->m_mapOfXData ;						
						int iIsParRef = (*SFmapIt).second->isParentReferenced();						

							if(getBitVal(iIsParRef,0))   // i.e. <PNODE1> found
							{	
								NSSTRING strParentsCon1 = getdataOf(_T("CONNECTIVITY_NODE1"),mapOfXDataOfParentBlock );
								(*SFmapIt).second->replaceParentReferenc(REL_NODE1,strParentsCon1 );
							}

							if(getBitVal(iIsParRef,1))   // i.e. <PNODE2> found
							{	
								NSSTRING strParentsCon2 = getdataOf(_T("CONNECTIVITY_NODE2"),mapOfXDataOfParentBlock );
								(*SFmapIt).second->replaceParentReferenc(REL_NODE2,strParentsCon2 );

								NSSTRING strPNodeIndex = (*SFmapIt).second->getIndexAtPNode2();

								(*SFmapIt).second->replaceParentReferenc(REL_NODE2,strParentsCon2,strPNodeIndex );
							}

							if(getBitVal(iIsParRef,2))   // i.e. <POWNERSHIP_PARENT> found
							{	
								NSSTRING strOwnerShipParent = getdataOf(_T("OWNERSHIP_PARENT"),mapOfXDataOfParentBlock );
								(*SFmapIt).second->replaceParentReferenc(REL_PARENT,strOwnerShipParent );
							}

							if(getBitVal(iIsParRef,3))   // i.e. <POWNERSHIP_CHILD> found
							{	
								NSSTRING strOwnerShipChild = getdataOf(_T("OWNERSHIP_CHILD"),mapOfXDataOfParentBlock );
								(*SFmapIt).second->replaceParentReferenc(REL_CHILD,strOwnerShipChild );
							}

							if(getBitVal(iIsParRef,4))   // i.e. <PMANYOWNERSHIP_CHILD> found
							{	
								NSSTRING strManyOwnerShipParent = getdataOf(_T("MANYOWNERSHIP_CHILD"),mapOfXDataOfParentBlock );
								(*SFmapIt).second->replaceParentReferenc(REL_MANYOWNERSHIP,strManyOwnerShipParent );
							}						
					//////////////////////////////////////////////////////////////UPDATING Relationships with External features

				}

	}
	/////// PROCESSING THE SUB-FEATURES WITH IN THE NESTED FEATURES

}

void CNSImportedFeatureContainer::updateSubFeaturesInterRelationShip(NSSTRING strParentFID, CNSRelationInfo* pRelationInfoOfSubFeat)
{
	NSSTRING strConnectivtyNode1 = pRelationInfoOfSubFeat->getConnectivtyNode1 ();
	NSSTRING strConnectivtyNode2 = pRelationInfoOfSubFeat->getConnectivtyNode2();
	NSSTRING strParent = pRelationInfoOfSubFeat->getParent();
	NSSTRING strChild = pRelationInfoOfSubFeat->getChild();
	NSSTRING strManyOwnership = pRelationInfoOfSubFeat->getManyOwnership();

	replaceAllInstance(strConnectivtyNode1, _T("-"),strParentFID);
	replaceAllInstance(strConnectivtyNode2, _T("-"),strParentFID);
	replaceAllInstance(strParent, _T("-"),strParentFID);
	replaceAllInstance(strChild, _T("-"),strParentFID);
	replaceAllInstance(strManyOwnership, _T("-"),strParentFID);

	// NSSTRING strTemp = _T("-2,222,<PNODE1>,-3,333");replaceAllInstance(strTemp , _T("-"),_T("-9")); // tesing purpose

	pRelationInfoOfSubFeat->setConnectivtyNode1(strConnectivtyNode1);
	pRelationInfoOfSubFeat->setConnectivtyNode2(strConnectivtyNode2);
	pRelationInfoOfSubFeat->setParent(strParent);
	pRelationInfoOfSubFeat->setChild(strChild);
	pRelationInfoOfSubFeat->setManyOwnership(strManyOwnership);
	
}

void CNSImportedFeatureContainer::replaceAllInstance(NSSTRING &stringToRepalce, NSSTRING strSearchChar ,NSSTRING stringToRepalceWith)
{
		//int L = stringToRepalce.length();

		NSSTRING st = _T(""); 

		for(int pos = 0; pos < stringToRepalce.length(); pos++)
		{			
				st = stringToRepalce.at(pos);	

				if(st==strSearchChar) 
						stringToRepalce.replace(pos,1,stringToRepalceWith);
		}
		

			//size_t found;
			//do
			//{
			//found=stringToRepalce.find(strSearchChar);

			////if (found!=string::npos)  
			//	stringToRepalce.replace(found,strSearchChar.length(),stringToRepalceWith);
			//} while (found!=string::npos);


}


void CNSImportedFeatureContainer::updateFeatures(std::vector<CNSFeatureContainer*> &vectFeatureContainer)
{ 

	for(std::vector<CNSFeatureContainer*>::iterator vIt = vectFeatureContainer.begin(); vIt !=  vectFeatureContainer.end(); vIt++)
	{

		NSSTRING strFID = (*vIt)->getXData(_T("FID"));

		NSSTRING strCONN1 = (*vIt)->getXData(_T("CONNECTIVITY_NODE1"));			
			update(strFID,strCONN1,REL_NODE1,(*vIt));

		NSSTRING strCONN2 = (*vIt)->getXData(_T("CONNECTIVITY_NODE2"));			
			update(strFID,strCONN2,REL_NODE2,(*vIt));

		NSSTRING strPAR = (*vIt)->getXData(_T("OWNERSHIP_PARENT"));				
			update(strFID,strPAR,REL_PARENT,(*vIt));

		NSSTRING strCHILD = (*vIt)->getXData(_T("OWNERSHIP_CHILD"));			
			update(strFID,strCHILD,REL_CHILD,(*vIt));

		NSSTRING strMANYPAR = (*vIt)->getXData(_T("MANYOWNERSHIP_CHILD"));		
			update(strFID,strMANYPAR,REL_MANYOWNERSHIP,(*vIt));	

	}
}



void CNSImportedFeatureContainer::getFIDFNOs(NSSTRING strXDATA, std::map<NSSTRING,NSSTRING> &mapOfFIDFNOs)
{
	
			int Len = strXDATA.length();

			NSSTRING ch1;

			for(int i = 0; i < Len ; i++)
			{
				ch1 = strXDATA.at(i);

				if(ch1 == _T("-"))
				{
					NSSTRING ch2;
					for(int j = i+1; j < Len ; j++)					
					{	
						ch2 = strXDATA.at(j);

						if(ch2 == _T(","))
						{
							NSSTRING strFID = strXDATA.substr(i,j-i);
							//vectOfFIDs.push_back(strFID);
									NSSTRING ch3;
									for(int k = j+1; k <= Len ; k++)					
									{
										ch3 = strXDATA.at(k);

										if(ch3 == _T(",")|| k == Len-1)
										{
											 NSSTRING strFNO ; 
												if(k==Len-1)
												{
													strFNO = strXDATA.substr(j+1,k-j);
												}
												else
												{
													strFNO = strXDATA.substr(j+1,k-j-1);
												}

											 mapOfFIDFNOs.insert(std::map<NSSTRING, NSSTRING>::value_type(strFID, strFNO));

											 i =k;
										     k = Len;// to exit from this inner loop
										}

										

									}
							//i =j+1;
							j = Len;// to exit from this inner loop
						}
					}
				}
			}
}




void CNSImportedFeatureContainer::update(NSSTRING strFIDtoFind, NSSTRING strXDATA,enum RelationType RelType, CNSFeatureContainer *pFeatureContainer)
{

			//strXDATA = _T("-5,126,-4,126"); // testing purpose only , to del 
			
			std::map<NSSTRING,NSSTRING> mapOfFIDFNOs;
			getFIDFNOs(strXDATA,mapOfFIDFNOs);

			for(std::map<NSSTRING,NSSTRING>::iterator IDit = mapOfFIDFNOs.begin(); IDit !=  mapOfFIDFNOs.end(); IDit++)
    		{
					NSSTRING strFIDtoReplace = (*IDit).first ;
					NSSTRING strFNOtoReplace = (*IDit).second ;
					NSSTRING strToReplace = strFIDtoReplace + _T(",") + strFNOtoReplace;

					
					std::map<NSSTRING, CNSFeatureContainer*>::iterator NFItr;	

					NFItr = mapNestedFeatureContainer.find(strFIDtoReplace);	

						if(NFItr != mapNestedFeatureContainer.end())
						{
							CNSFeatureContainer* linkedFeature =  (*NFItr).second;

							std::map<NSSTRING, CNSFeatureContainer *> subFeatures ;
								linkedFeature->getSubFeatureMap(subFeatures);

								
								bool bValueReplaced_CN1 = false;
								bool bValueReplaced_CN2 = false;
								bool bValueReplaced_CHD = false;
								bool bValueReplaced_PAR = false;
								bool bValueReplaced_MNP = false;

								NSSTRING strAppName;
								NSSTRING strAppValue;

								for(std::map<NSSTRING, CNSFeatureContainer *>::iterator SFItr = subFeatures.begin(); SFItr !=  subFeatures.end(); SFItr++)
								{
									

									NSSTRING strFIDtoReplaceWith;	
									NSSTRING strFNOtoReplaceWith ;
									NSSTRING strToReplaceWith ;

									CNSFeatureContainer *pSubfeature = (*SFItr).second ;
				 					CNSRelationInfo*  relInfo = pSubfeature->getRelationInfo();

										 
									 strFIDtoReplaceWith = relInfo->getFID();	
									 strFNOtoReplaceWith = relInfo->getFNO();
									 strToReplaceWith = strFIDtoReplaceWith + _T(",") + strFNOtoReplaceWith;


/////////CONNECTIVITY_NODE//////////////////////////////////////////////////////////////////////////////////////////
									if(RelType == REL_NODE1 || RelType == REL_NODE2)
									{	
										if(RelType == REL_NODE1) strAppName = _T("CONNECTIVITY_NODE1");
										if(RelType == REL_NODE2) strAppName = _T("CONNECTIVITY_NODE2");

										strAppValue = pFeatureContainer->getXData(strAppName);
										
										//strAppValue = _T("-4,444,-102,9987,-3,333"); // for testing purpose , to del

										////////////////////////////////////////////////////////////////////////////////////
										size_t foundpos ;
										foundpos = strAppValue.find(strToReplace);

										if(foundpos!=string::npos )
										{
											NSSTRING strConn1 = relInfo->getConnectivtyNode1();
											NSSTRING strConn2 = relInfo->getConnectivtyNode2();

											size_t found ;

											found = strConn1.find(strFIDtoFind);
											if(found!=string::npos)
											{
												strAppValue.replace(strAppValue.find(strToReplace),strToReplace.length(),strToReplaceWith);          
											    pFeatureContainer->setXData(strAppName, strAppValue);
												bValueReplaced_CN1 = true;
											}

											found = strConn2.find(strFIDtoFind);
											if(found!=string::npos)
											{
												strAppValue.replace(strAppValue.find(strToReplace),strToReplace.length(),strToReplaceWith);          
											    pFeatureContainer->setXData(strAppName, strAppValue);
												bValueReplaced_CN2 = true;
											}

											
										}
										else
										{
											NSSTRING strConn1 = relInfo->getConnectivtyNode1();
											NSSTRING strConn2 = relInfo->getConnectivtyNode2();

											size_t found ;

											found = strConn1.find(strFIDtoFind);
											if(found!=string::npos)
											{
												strAppValue = strAppValue + _T(",") + strToReplaceWith;
												pFeatureContainer->setXData(strAppName, strAppValue);											
											}

											found = strConn2.find(strFIDtoFind);
											if(found!=string::npos)
											{
												strAppValue = strAppValue + _T(",") + strToReplaceWith;
												pFeatureContainer->setXData(strAppName, strAppValue);
											}
										}

									}
/////////CONNECTIVITY_NODE//////////////////////////////////////////////////////////////////////////////////////////

/////////OWNERSHIP_PARENT//////////////////////////////////////////////////////////////////////////////////////////									
									if(RelType == REL_PARENT)
									{
										strAppName = _T("OWNERSHIP_PARENT");

										strAppValue = pFeatureContainer->getXData(strAppName);										
										
										////////////////////////////////////////////////////////////////////////////////////
										size_t foundpos ;
										foundpos = strAppValue.find(strToReplace);

										if(foundpos!=string::npos && !bValueReplaced_PAR)
										{
											strAppValue.replace(strAppValue.find(strToReplace),strToReplace.length(),strToReplaceWith);          
											pFeatureContainer->setXData(strAppName, strAppValue);
											bValueReplaced_PAR = true;
										}
										else if(bValueReplaced_PAR)
										{
											NSSTRING strChild = relInfo->getChild();
											size_t found;

											found = strChild.find(strFIDtoFind);
											if(found!=string::npos)
											{	
												strAppValue = strAppValue + _T(",") + strToReplaceWith;
												pFeatureContainer->setXData(strAppName, strAppValue);
											}
										}

									}
/////////OWNERSHIP_PARENT//////////////////////////////////////////////////////////////////////////////////////////

/////////OWNERSHIP_CHILD//////////////////////////////////////////////////////////////////////////////////////////
									if(RelType == REL_CHILD)
									{
										strAppName = _T("OWNERSHIP_CHILD");

										strAppValue = pFeatureContainer->getXData(strAppName);										
										
										////////////////////////////////////////////////////////////////////////////////////
										size_t foundpos ;
										foundpos = strAppValue.find(strToReplace);

										if(foundpos!=string::npos && !bValueReplaced_CHD)
										{
											NSSTRING strPar = relInfo->getParent();
											size_t found;

											found = strPar.find(strFIDtoFind);
											if(found!=string::npos)
											{	
												strAppValue.replace(strAppValue.find(strToReplace),strToReplace.length(),strToReplaceWith);          
											    pFeatureContainer->setXData(strAppName, strAppValue);
											    bValueReplaced_CHD = true;
											}

											
										}
										else if(bValueReplaced_CHD)
										{
											NSSTRING strPar = relInfo->getParent();
											size_t found;

											found = strPar.find(strFIDtoFind);
											if(found!=string::npos)
											{	
												strAppValue = strAppValue + _T(",") + strToReplaceWith;
												pFeatureContainer->setXData(strAppName, strAppValue);
											}
										}

									}
/////////OWNERSHIP_CHILD//////////////////////////////////////////////////////////////////////////////////////////


/////////MANYOWNERSHIP_CHILD//////////////////////////////////////////////////////////////////////////////////////////									
									if(RelType == REL_MANYOWNERSHIP)
									{
										strAppName = _T("MANYOWNERSHIP_CHILD");

										strAppValue = pFeatureContainer->getXData(strAppName);										
										
										////////////////////////////////////////////////////////////////////////////////////
										size_t foundpos ;
										foundpos = strAppValue.find(strToReplace);

										if(foundpos!=string::npos && !bValueReplaced_MNP)
										{							

											NSSTRING strManyPar = relInfo->getManyOwnership();
											size_t found;

											found = strManyPar.find(strFIDtoFind);
											if(found!=string::npos)
											{	
												strAppValue.replace(strAppValue.find(strToReplace),strToReplace.length(),strToReplaceWith);          
											    pFeatureContainer->setXData(strAppName, strAppValue);
											    bValueReplaced_MNP = true;
											}

										}
										else if(bValueReplaced_MNP)
										{
											NSSTRING strManyPar = relInfo->getManyOwnership();
											size_t found;

											found = strManyPar.find(strFIDtoFind);
											if(found!=string::npos)
											{	
												strAppValue = strAppValue + _T(",") + strToReplaceWith;
												pFeatureContainer->setXData(strAppName, strAppValue);
											}
										}
									}
/////////MANYOWNERSHIP_CHILD//////////////////////////////////////////////////////////////////////////////////////////

								}
/////Finalize////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								

								if(RelType == REL_NODE1 && bValueReplaced_CN1 ==  false)
								{
									size_t found;

									found = strAppValue.find(strToReplace);
									if(found!=string::npos)
									{
										strAppValue.replace(found,strToReplace.length(),_T(""));          
										pFeatureContainer->setXData(strAppName, strAppValue);
									}
								}
								
								if(RelType == REL_NODE2 && bValueReplaced_CN2 ==  false)
								{
									size_t found;

									found = strAppValue.find(strToReplace);
									if(found!=string::npos)
									{
										strAppValue.replace(strAppValue.find(strToReplace),strToReplace.length(),_T(""));          
										pFeatureContainer->setXData(strAppName, strAppValue);
									}
								}

								if(RelType == REL_PARENT && bValueReplaced_PAR ==  false)
								{
									size_t found;

									found = strAppValue.find(strToReplace);
									if(found!=string::npos)
									{
										strAppValue.replace(strAppValue.find(strToReplace),strToReplace.length(),_T(""));          
										pFeatureContainer->setXData(strAppName, strAppValue);
									}
								}

								if(RelType == REL_CHILD && bValueReplaced_CHD ==  false)
								{
									size_t found;

									found = strAppValue.find(strToReplace);
									if(found!=string::npos)
									{
										strAppValue.replace(strAppValue.find(strToReplace),strToReplace.length(),_T(""));          
										pFeatureContainer->setXData(strAppName, strAppValue);
									}
								}

								if(RelType == REL_MANYOWNERSHIP && bValueReplaced_MNP ==  false)
								{
									size_t found;

									found = strAppValue.find(strToReplace);
									if(found!=string::npos)
									{
										strAppValue.replace(strAppValue.find(strToReplace),strToReplace.length(),_T(""));          
										pFeatureContainer->setXData(strAppName, strAppValue);
									}
								}


/////Finalize////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						}	

		    }



}


void CNSImportedFeatureContainer::createSubFeatures(std::map<NSSTRING, CNSFeatureContainer*> mapNestedFeatureContainer)
{
	
	for(std::map<NSSTRING, CNSFeatureContainer*>::iterator NFmapIt = mapNestedFeatureContainer.begin(); NFmapIt !=  mapNestedFeatureContainer.end(); NFmapIt++)
	{	
		std::map<NSSTRING, CNSFeatureContainer *> mapOfSubFeatures ;
		(*NFmapIt).second->getSubFeatureMap(mapOfSubFeatures);

		std::vector<CNSFeatureContainer*> vectFeatureContainer;
			
		std::map<NSSTRING,CNSFeatureContainer *>::iterator SFmapIt;
		for(SFmapIt = mapOfSubFeatures.begin(); SFmapIt != mapOfSubFeatures.end(); SFmapIt++)
		{	
			vectFeatureContainer.push_back((*SFmapIt).second);
		}					

		// createSubFeature(_T("NEW"), _T("NEW"),CAT_SEGMENT, vectFeatureContainer, m_vectNewFeatures);

		 createSubFeature(_T("NEW"), _T("NEW"),CAT_BLOCK, vectFeatureContainer, m_vectNewFeatures);

	}
	
}

void CNSImportedFeatureContainer::finalizeSubFeatures(std::map<NSSTRING, CNSFeatureContainer*> mapNestedFeatureContainer)
{
	for(std::map<NSSTRING, CNSFeatureContainer*>::iterator NFmapIt = mapNestedFeatureContainer.begin(); NFmapIt !=  mapNestedFeatureContainer.end(); NFmapIt++)
	{	
		std::map<NSSTRING, CNSFeatureContainer *> mapOfSubFeatures ;
		(*NFmapIt).second->getSubFeatureMap(mapOfSubFeatures);

		std::map<NSSTRING,CNSFeatureContainer *>::iterator SFmapIt;
		for(SFmapIt = mapOfSubFeatures.begin(); SFmapIt != mapOfSubFeatures.end(); SFmapIt++)
		{	
			CNSRelationInfo* pRelationInfo = (*SFmapIt).second->getRelationInfo();

           (*SFmapIt).second->setXData(_T("FID"),pRelationInfo->getFID());
		   (*SFmapIt).second->setXData(_T("FNO"),pRelationInfo->getFNO());
		   (*SFmapIt).second->setXData(_T("CONNECTIVITY_NODE1"),pRelationInfo->getConnectivtyNode1());
		   (*SFmapIt).second->setXData(_T("CONNECTIVITY_NODE2"),pRelationInfo->getConnectivtyNode2());
		   (*SFmapIt).second->setXData(_T("MANYOWNERSHIP_CHILD"),pRelationInfo->getManyOwnership());
		   (*SFmapIt).second->setXData(_T("OWNERSHIP_CHILD"),pRelationInfo->getChild());
		   (*SFmapIt).second->setXData(_T("OWNERSHIP_PARENT"),pRelationInfo->getParent());	   

			
		}
	}

}