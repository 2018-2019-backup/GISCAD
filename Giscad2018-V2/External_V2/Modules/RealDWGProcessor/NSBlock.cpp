#include "StdAfx.h"
#include "..\Include\NSBlock.h"
#include "tchar.h"
#include <string>
using namespace std;

void getupperCaseString(TCHAR *pInput)
{
	NSSTRING strTemp = pInput;
	if(strTemp.length() > 0)
	{
#ifndef _INTERNAL_
	NSSTRUPR(pInput, strTemp.size()+1);
#else
	NSSTRUPR(pInput, strTemp.size()+1);
	//NSSTRUPR(pInput);
#endif
	}
}



CNSBlock::CNSBlock(void)
{
	m_dRotation = 0.0;
	m_nAlignment = 0;
}

CNSBlock::~CNSBlock(void)
{
	//map memory release here
}

CNSBlock::CNSBlock(NSSTRING pszBlockName, CNSPoint3D ptBlockInsertionPoint, double dRotation)
{
	
	m_pszBlockName = pszBlockName;
	/*m_ptBlockInsertionPoint.m_dXX = ptBlockInsertionPoint.m_dXX;
	m_ptBlockInsertionPoint.m_dYY = ptBlockInsertionPoint.m_dYY;
	m_ptBlockInsertionPoint.m_dZZ = ptBlockInsertionPoint.m_dZZ;*/
	m_dRotation = 0.0;	
	m_nAlignment = 0;
}


void CNSBlock::addAttribute(NSSTRING pszKey, NSSTRING pszValue)
{
//	m_mapAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(pszKey, pszValue));
}

int CNSBlock::getAttributeValue(NSSTRING pszKey, NSSTRING& pszValue)
{
/*	std::map<NSSTRING, NSSTRING>::iterator itrMap;
	itrMap = m_mapAttributes.find(pszKey);
	if(itrMap != m_mapAttributes.end())
	{ 
		pszValue = (*itrMap).second;
		return 0;
	}*/
	return 1;
}


NSSTRING CNSBlock::getBlockName()
{
	return m_pszBlockName;
}

void CNSBlock::getInsertionpoint(CNSPoint3D& Pt)
{
	NSSTRING strTemp = m_strPoslist;

	TCHAR* szNextToken;   //dummy 
	#ifdef _INTERNAL_
		/*Pt.m_dXX = NSATOF(NSSTRTOK ((TCHAR*)strTemp.c_str(), _T(" ")));
		Pt.m_dYY = NSATOF(NSSTRTOK (NULL, _T("\0")));*/
	    Pt.m_dXX = NSATOF(NSSTRTOK ((TCHAR*)strTemp.c_str(), _T(" "), &szNextToken));
		Pt.m_dYY = NSATOF(NSSTRTOK (NULL, _T("\0"), &szNextToken));
	#else
		Pt.m_dXX = NSATOF(NSSTRTOK ((TCHAR*)strTemp.c_str(), _T(" "), &szNextToken));
		Pt.m_dYY = NSATOF(NSSTRTOK (szNextToken, _T(" "), &szNextToken));
		Pt.m_dZZ = NSATOF(NSSTRTOK (NULL, _T("\0"), &szNextToken));
	#endif

//	Pt = m_ptBlockInsertionPoint;
}

double CNSBlock::getRotation()
{
	return m_dRotation;
}

int CNSBlock::getAlignment()
{
	return m_nAlignment;
}

void CNSBlock::setInsertionPoint(CNSPoint3D ptBlockInsertionPoint)
{
//	m_ptBlockInsertionPoint = ptBlockInsertionPoint;
}

void CNSBlock::setBlockName(NSSTRING pszBlockName)
{
	m_pszBlockName = pszBlockName;
}

void CNSBlock::setRotation(double dRotation)
{
	m_dRotation = dRotation;
}

void CNSBlock::setAlignment(int nAlignment)
{
	m_nAlignment = nAlignment;
}

void CNSBlock::setInsertionPoint(double dX, double dY, double dZ)
{
	TCHAR szFormat[(_MAX_PATH + 1)];
	
	#ifdef _INTERNAL_
		NSSPRINTF(szFormat,_T("%lf %lf"), dX, dY);
	#else
		NSSPRINTF(szFormat,(_MAX_PATH + 1), _T("%lf %lf %lf"), dX, dY, dZ);
	#endif

	m_strPoslist = szFormat;
/*	m_ptBlockInsertionPoint.m_dXX = dX;
	m_ptBlockInsertionPoint.m_dYY = dY;
	m_ptBlockInsertionPoint.m_dZZ = dZ;*/
}

//void CNSBlock::addXDATA(std::map<NSSTRING, NSSTRING> &mapXDataRead)
//{
///*	std::map<NSSTRING, NSSTRING>::iterator itrXDATA;
//		for(itrXDATA = mapXDataRead.begin(); itrXDATA != mapXDataRead.end(); itrXDATA++)
//		{
//			m_mapXDATA.insert(std::map<NSSTRING, NSSTRING>::value_type(itrXDATA->first, itrXDATA->second));
//		}*/
//}

void CNSFeatureContainer::updateAttributeRef(NSSTRING strKey, int nCID, CNSAttributeRef* pAttrRef)
{
	if(m_mapOfBlkAtt != NULL)
	{
		CNSAttributeRefMap *pRefMap = NULL;
		ATT_MAP::iterator AttRefItr = m_mapOfBlkAtt->find(strKey);
		if(AttRefItr != m_mapOfBlkAtt->end())
		{
			pRefMap = (*AttRefItr).second;
			m_mapOfBlkAtt->erase(AttRefItr);
			delete pRefMap;
		}

		addAttributeRef(strKey, nCID, pAttrRef);
	}
}

void CNSFeatureContainer::addAttributeRef(NSSTRING strKey, int nCID, CNSAttributeRef* pAttrRef)
{
	if(m_mapOfBlkAtt != NULL)
	{
		CNSAttributeRefMap *pRefMap = NULL;
		ATT_MAP::iterator AttRefItr = m_mapOfBlkAtt->find(strKey);
		if(AttRefItr != m_mapOfBlkAtt->end())
		{
			pRefMap = (*AttRefItr).second;
		}
		else
		{
			pRefMap = new CNSAttributeRefMap;
		}
		pRefMap->addAttRef(nCID, pAttrRef);
		#ifndef _INTERNAL_
			NSSTRUPR((TCHAR*)strKey.c_str(), strKey.size()+1);
		#else
		    NSSTRUPR((TCHAR*)strKey.c_str(), strKey.size()+1);
			//NSSTRUPR((TCHAR*)strKey.c_str());
		#endif		

		//m_mapOfBlkAtt->erase( m_mapOfBlkAtt->find( strKey ) );

		//m_mapOfBlkAtt->erase( strKey  );

		m_mapOfBlkAtt->insert(ATT_MAP::value_type(strKey, pRefMap));


	//	std::map<NSSTRING, CNSAttributeRef*>::iterator AttrRefItr;
	//	m_mapOfBlkAtt->insert(std::map<NSSTRING, CNSAttributeRef*>::value_type(strKey, pAttrRef));
	}
}

CNSAttributeRef* CNSFeatureContainer::findAttributeRef(NSSTRING strKey, int nCID)
{
	ATT_MAP::iterator AttrRefMapItr;
	CNSAttributeRefMap* pAttrRefMap = NULL;
	if(m_mapOfBlkAtt != NULL)
	{
		AttrRefMapItr = m_mapOfBlkAtt->find(strKey);
		if(AttrRefMapItr != m_mapOfBlkAtt->end())
		{
			pAttrRefMap = (*AttrRefMapItr).second;
			if(pAttrRefMap)
			{
				return pAttrRefMap->getAttrRef(nCID);
			}
			else
			{
				return NULL;
			}
		}
	}
	return NULL;
}


CNSFeatureContainer::CNSFeatureContainer(void)
{
	m_pContainer = NULL;
	m_mapOfBlkAtt = NULL;;
	m_mapOfXData = NULL;
	m_pNestedBlockInfo = NULL;
	m_pRelationInfo = NULL;
	m_bIsNestedFeature = false;
	m_bIsMultipleFeature = false;
	
}

void CNSFeatureContainer::setComponentContainer(CNSCompositeContainer* container)
{
	m_pContainer = container;
}

void CNSFeatureContainer::setBlockAttributes(STRING_MAP* mapAttributes)
{
	//m_mapOfBlkAtt = mapAttributes;
}

void CNSFeatureContainer::setAttributeMap(ATT_MAP* mapOfAtt)
{
	m_mapOfBlkAtt = mapOfAtt;	
	
}

ATT_MAP* CNSFeatureContainer::getAttributeMap()
{
	return m_mapOfBlkAtt;
}

void CNSFeatureContainer::setXData(STRING_MAP* mapXData)
{
	m_mapOfXData = mapXData;
}

void CNSFeatureContainer::setXData(NSSTRING strAppName, NSSTRING strAppValue)
{
	STRING_MAP::iterator XdataItr;
	STRING_MAP* mapOfXDATA = getXData();

			XdataItr = mapOfXDATA->find(strAppName);	

			if(XdataItr != mapOfXDATA->end())
			{
				XdataItr->second = strAppValue;
			}
	
	//return strAppValue;
}


COMPONENT_TYPE CNSFeatureContainer::getCompType(int nCompNumber)
{
	if(m_pContainer)
		return m_pContainer->getCompType(nCompNumber);
	return COMP_INVALID;
}

int Block_findString(NSSTRING strBaseString, NSSTRING strStringToBeFind)
{
	#ifndef _INTERNAL_
		NSSTRUPR((TCHAR*)strBaseString.c_str(), strBaseString.size()+1);
		NSSTRUPR((TCHAR*)strStringToBeFind.c_str(), strStringToBeFind.size()+1 );	
	#else
	    NSSTRUPR((TCHAR*)strBaseString.c_str(), strBaseString.size()+1);
		NSSTRUPR((TCHAR*)strStringToBeFind.c_str(), strStringToBeFind.size()+1 );
		/*NSSTRUPR((TCHAR*)strBaseString.c_str());
		NSSTRUPR((TCHAR*)strStringToBeFind.c_str());	*/
	#endif
	return (int)strBaseString.find(strStringToBeFind);
}

void CNSFeatureContainer::SetAssetNumberFromParent()
{
	NSSTRING strParentTag;
	NSSTRING AttValFromPar;

	ATT_MAP::iterator AttItr;
	STRING_MAP::iterator strItr;

	AttItr = m_mapOfBlkAtt->find(_T("ASSET_NUM"));	

	if(AttItr != m_mapOfBlkAtt->end())
	{
		CNSAttributeRefMap* AttRefMap = AttItr->second ;

		NSSTRING strAssetNum = ((*AttItr).second)->getAttrRef(1)->getAttributeValue();	

		NSSTRING strAttTag =  (*AttItr).first;
		size_t value = Block_findString(strAttTag, _T("<PASSET_NUM"));
		if (value != NSSTRING::npos)
		{
			return;
		}

		if(strAssetNum.size() > 12)
		{
			NSSTRING strN = strAssetNum.substr(11,strAssetNum.size() - 12);
			strParentTag = _T("ASSET_NUM") + strN;	
			strItr = m_mapOfParentAttributes.find(strParentTag);	
			if(strItr != m_mapOfParentAttributes.end())
			{
				AttValFromPar = strItr->second ;
				((*AttItr).second)->getAttrRef(1)->setAttributeValue(AttValFromPar);
			}
		}
	}
}



bool CNSFeatureContainer::getNestedFeatureFlag()
{
		return m_bIsNestedFeature;
}

bool CNSFeatureContainer::getMultipleFeatureFlag()
{
		return m_bIsMultipleFeature;
}

void CNSFeatureContainer::setNestedFeatureFlag(bool bNewVal)
{
	m_bIsNestedFeature = bNewVal;
}

void CNSFeatureContainer::setMultipleFeatureFlag(bool bNewVal)
{
	m_bIsMultipleFeature = bNewVal;
}

CNSCompositeContainer* CNSFeatureContainer::getComponentContainer()
{
	return m_pContainer;
}

STRING_MAP* CNSFeatureContainer::getBlockAttributes()
{
	return m_mapOfXData;
}
STRING_MAP* CNSFeatureContainer::getXData()
{
	return m_mapOfXData;
}

NSSTRING CNSFeatureContainer::getXData(NSSTRING strAppName)
{
	NSSTRING strAppValue;
	STRING_MAP::iterator XdataItr;
	STRING_MAP* mapOfXDATA = getXData();

			XdataItr = mapOfXDATA->find(strAppName);	

			if(XdataItr != mapOfXDATA->end())
			{
				strAppValue = XdataItr->second ;
			}
	
	return strAppValue;
}

void CNSFeatureContainer::setLayerName(NSSTRING strLayer)
{
	m_strlayerName = strLayer;
}

void CNSFeatureContainer::setTypeName(NSSTRING strType)
{
	m_strType = strType;
}

NSSTRING CNSFeatureContainer::getLayerName()
{
	return m_strlayerName;
}

NSSTRING CNSFeatureContainer::getTypeName()
{
	return m_strType;
}

void CNSFeatureContainer::release()
{
	ATT_MAP::iterator AttrRefItr;
	CNSAttributeRefMap *pRef = NULL;
	if(m_mapOfBlkAtt != NULL)
	{
		for(AttrRefItr = m_mapOfBlkAtt->begin(); AttrRefItr != m_mapOfBlkAtt->end(); AttrRefItr++)
		{
			pRef = (*AttrRefItr).second;
			delete pRef;
		}
	}
}

CNSAttributeRefMap::CNSAttributeRefMap()
{

}

CNSAttributeRefMap::~CNSAttributeRefMap()
{
	ATTREF_MAP::iterator AttrRefItr;
	CNSAttributeRef *pRef = NULL;
	for(AttrRefItr = m_AttrRefMap.begin(); AttrRefItr != m_AttrRefMap.end(); AttrRefItr++)
	{
		pRef = (*AttrRefItr).second;
		delete pRef;
	}
}

void CNSAttributeRefMap::addAttRef(int nCID, CNSAttributeRef *pAttrRef)
{		
	m_AttrRefMap.insert(ATTREF_MAP::value_type(nCID, pAttrRef));
}
CNSAttributeRef* CNSAttributeRefMap::getAttrRef(int nCID)
{
	ATTREF_MAP::iterator AttRefItr = m_AttrRefMap.find(nCID);
	if(AttRefItr != m_AttrRefMap.end())
	{
		return (*AttRefItr).second;
	}
	return NULL;
}

int CNSAttributeRefMap::getSize()
{
	return (int)m_AttrRefMap.size();
}

CNSAttributeRef* CNSAttributeRefMap::getAt(int nIndex)
{
	if(nIndex < getSize())
		return m_AttrRefMap[nIndex];
	else 
		return NULL;
}

int CNSAttributeRefMap::getAttrReferences(ATTREF_MAP &MapOfRefs)
{
	ATTREF_MAP::iterator AttrRefItr;
	for(AttrRefItr = m_AttrRefMap.begin(); AttrRefItr != m_AttrRefMap.end(); AttrRefItr++)
	{
		MapOfRefs.insert(ATTREF_MAP::value_type((*AttrRefItr).first, (*AttrRefItr).second));
	}
	return NS_SUCCESS;
}





void CNSFeatureContainer::getSubFeatureMap(std::map<NSSTRING, CNSFeatureContainer *> &mapOfSubFeatures)
{
	if( m_pNestedBlockInfo != NULL )
	{
		mapOfSubFeatures = m_pNestedBlockInfo->getSubFeatureMap();
	}
}
void CNSFeatureContainer::createAnewSubFeature(CNSFeatureContainer *pSubFeatContainer)
{	
	if(m_pNestedBlockInfo == NULL ) 
		m_pNestedBlockInfo = new CNSNestedBlockInfo;

	if(pSubFeatContainer && pSubFeatContainer->m_pRelationInfo)
	{
		m_pNestedBlockInfo->createAnewSubFeature(pSubFeatContainer->m_pRelationInfo->getFID(),pSubFeatContainer);
	}
}

void CNSFeatureContainer::setRelation(NSSTRING strAppName, NSSTRING strAppValue, CNSFeatureContainer *pNestedFeatContainer)
{
	if(m_pRelationInfo == NULL ) 
		m_pRelationInfo = new CNSRelationInfo;

	if(NSSTRCMP(strAppName.c_str(),_T("FID"))==0)
	{
		//std::map<NSSTRING, NSSTRING>::iterator xDataIter;
		//xDataIter = pNestedFeatContainer->m_mapOfXData->find(_T("FID"));

		//if(xDataIter != pNestedFeatContainer->m_mapOfXData->end())
		//{
		//	NSSTRING strParentFID = (*xDataIter).second;
		//	strAppValue = strParentFID + strAppValue ;
		//}

		m_pRelationInfo->setFID(strAppValue);
	}

	if(NSSTRCMP(strAppName.c_str(),_T("FNO"))==0)
	{
		m_pRelationInfo->setFNO(strAppValue);
	}

	if(NSSTRCMP(strAppName.c_str(),_T("CONNECTIVITY_NODE1"))==0)
	{
		/*std::map<NSSTRING, NSSTRING>::iterator xDataIter;
		xDataIter = pNestedFeatContainer->m_mapOfXData->find(_T("CONNECTIVITY_NODE1"));

		if(xDataIter != pNestedFeatContainer->m_mapOfXData->end())
		{
			NSSTRING strParentCN1 = (*xDataIter).second;
			strAppValue = strParentCN1 + strAppValue ;
		}*/

		m_pRelationInfo->setConnectivtyNode1(strAppValue);
	}

	if(NSSTRCMP(strAppName.c_str(),_T("CONNECTIVITY_NODE2"))==0)
	{
		m_pRelationInfo->setConnectivtyNode2(strAppValue);
	}

	if(NSSTRCMP(strAppName.c_str(),_T("MANYOWNERSHIP_CHILD"))==0)
	{
		m_pRelationInfo->setManyOwnership(strAppValue);
	}

	if(NSSTRCMP(strAppName.c_str(),_T("OWNERSHIP_CHILD"))==0)
	{
		m_pRelationInfo->setChild(strAppValue);
	}

	if(NSSTRCMP(strAppName.c_str(),_T("OWNERSHIP_PARENT"))==0)
	{
		m_pRelationInfo->setParent(strAppValue);
	}

}


/*
@Brief			Returns integer based on Parent Reference in relationship info
					
				Return value is based on binary logic
				e.g. if it returns 9 then it means that parent is referenced in REL_NODE1 & REL_CHILD
				bacause decimal 9 is equal to binary 1001

@Return			int
*/
int CNSFeatureContainer::isParentReferenced()
{
	
	int retval = 0;

	if(m_pRelationInfo != NULL )
	{

	if(m_pRelationInfo->isParentReferenced(REL_NODE1))
										retval = retval + 1;
	if(m_pRelationInfo->isParentReferenced(REL_NODE2))
										retval = retval + 2;
	if(m_pRelationInfo->isParentReferenced(REL_PARENT))
										retval = retval + 4;
	if(m_pRelationInfo->isParentReferenced(REL_CHILD))
										retval = retval + 8;
	if(m_pRelationInfo->isParentReferenced(REL_MANYOWNERSHIP))
										retval = retval + 16;
	}

	return retval;
}

void CNSFeatureContainer::replaceParentReferenc(enum RelationType RelType ,const NSSTRING strActualStringtoReplaceWith, NSSTRING strPNodeIndex )
{
	m_pRelationInfo->replaceParentReferenc(RelType,strActualStringtoReplaceWith,strPNodeIndex );
}

CNSRelationInfo*  CNSFeatureContainer::getRelationInfo()
{
	return m_pRelationInfo;
}

// Constructor
CNSRelationInfo::CNSRelationInfo(void)
{
	 m_strFID = _T("");
	 m_strFNO = _T("");
	 m_strConnectivtyNode1 = _T("");
	 m_strConnectivtyNode2 = _T("");
	 m_strParent = _T("");
	 m_strChild = _T("");
	 m_strManyOwnership = _T("");
}

// Destructor
CNSRelationInfo::~CNSRelationInfo(void)
{
}

void CNSRelationInfo::setFID(const NSSTRING strFID)
{
	m_strFID = strFID;
}

void CNSRelationInfo::setFNO(const NSSTRING strFNO)
{
	m_strFNO = strFNO;
}

void CNSRelationInfo::setConnectivtyNode1(const NSSTRING strConnectivtyNode1)
{
	m_strConnectivtyNode1 = strConnectivtyNode1;
}

void CNSRelationInfo::setConnectivtyNode2 (const NSSTRING strConnectivtyNode2)
{
	m_strConnectivtyNode2 = strConnectivtyNode2;
}

void CNSRelationInfo::setParent(const NSSTRING strParent)
{
	m_strParent = strParent;
}

void CNSRelationInfo::setChild (const NSSTRING strChild)
{
	m_strChild = strChild;
}

void CNSRelationInfo::setManyOwnership(const NSSTRING strManyOwnership)
{
	m_strManyOwnership = strManyOwnership;
}

NSSTRING CNSRelationInfo::getFID()
{
	return  m_strFID;
}
NSSTRING CNSRelationInfo::getFNO()
{
	return  m_strFNO;
}
NSSTRING CNSRelationInfo::getConnectivtyNode1()
{
	return  m_strConnectivtyNode1;
}
NSSTRING CNSRelationInfo::getConnectivtyNode2()
{
	return  m_strConnectivtyNode2;
}
NSSTRING CNSRelationInfo::getParent()
{
	return  m_strParent;
}
NSSTRING CNSRelationInfo::getChild()
{
	return  m_strChild;
}
NSSTRING CNSRelationInfo::getManyOwnership()
{
	return  m_strManyOwnership;
}


bool CNSRelationInfo::isParentReferenced(enum RelationType RelType)
{
	NSSTRING strTemp ;

		if (RelType == REL_NODE1)
		{			
			NSSTRING str_Pnode1 = _T("<PNODE1>");		

			getupperCaseString((TCHAR*)m_strConnectivtyNode1.c_str());

			size_t found;
			found = m_strConnectivtyNode1.find(str_Pnode1);

			if (found!=string::npos)  
				return true; 
		}

		if (RelType == REL_NODE2)
		{

			NSSTRING str_Pnode2 = _T("<PNODE2"); //<PNODE2>...<PNODE2_1>...<PNODE2_2>....<PNODE2_N>
			getupperCaseString((TCHAR*)m_strConnectivtyNode2.c_str());
			size_t found;
			found = m_strConnectivtyNode2.find(str_Pnode2);

			if (found!=string::npos)  
				return true; 
		}

		if (RelType == REL_PARENT)
		{

			NSSTRING str_Pparent = _T("<POWNERSHIP_PARENT>");
			getupperCaseString((TCHAR*)m_strParent.c_str());
			size_t found;
			found = m_strParent.find(str_Pparent);

			if (found!=string::npos)  
				return true; 
		}

		if (RelType == REL_CHILD)
		{

			NSSTRING str_Pchild = _T("<POWNERSHIP_CHILD>");
			getupperCaseString((TCHAR*)m_strChild.c_str());
			size_t found;
			found = m_strChild.find(str_Pchild);

			if (found != string::npos)  
				return true; 

		}

		if (RelType == REL_MANYOWNERSHIP)
		{

			NSSTRING str_Pmanymwnership = _T("<PMANYOWNERSHIP_CHILD>");
			getupperCaseString((TCHAR*)m_strManyOwnership.c_str());

			size_t found;
			found = m_strManyOwnership.find(str_Pmanymwnership);

			if (found != string::npos)  
				return true; 
		}

	return false;
}


void CNSRelationInfo::replaceParentReferenc(enum RelationType RelType,const NSSTRING strActualStringtoReplaceWith, NSSTRING strPNodeIndex  )
{

		if (RelType == REL_NODE1)
		{			
			NSSTRING str_Pnode1 = _T("<PNODE1>");		

			size_t found;
			found=m_strConnectivtyNode1.find(str_Pnode1);

			if (found!=string::npos)  
				m_strConnectivtyNode1.replace(found,str_Pnode1.length(),strActualStringtoReplaceWith);
		}

		if (RelType == REL_NODE2)
		{
			if(strPNodeIndex==_T(""))
			{
				NSSTRING str_Pnode2 = _T("<PNODE2>");

				size_t found;
				found=m_strConnectivtyNode2.find(str_Pnode2);

				if (found!=string::npos)  
					m_strConnectivtyNode2.replace(found,str_Pnode2.length(),strActualStringtoReplaceWith);
			}
			else
			{
				std::map<NSSTRING,NSSTRING> mapOfFIDFNOs;
			    getFIDFNOs(strActualStringtoReplaceWith,mapOfFIDFNOs);
				NSSTRING FIDFNOpairAtIndex = mapOfFIDFNOs[strPNodeIndex];

				NSSTRING str_Pnode2 = _T("<PNODE2_") + strPNodeIndex  + _T(">"); 

				size_t found;
				found=m_strConnectivtyNode2.find(str_Pnode2);

				if (found!=string::npos)  
					m_strConnectivtyNode2.replace(found,str_Pnode2.length(),FIDFNOpairAtIndex);

			}
		}

		if (RelType == REL_PARENT)
		{

			NSSTRING str_Pparent = _T("<POWNERSHIP_PARENT>");

			size_t found;
			found=m_strParent.find(str_Pparent);

			if (found!=string::npos)  
				m_strParent.replace(found,str_Pparent.length(),strActualStringtoReplaceWith);
		}

		if (RelType == REL_CHILD)
		{

			NSSTRING str_Pchild = _T("<POWNERSHIP_CHILD>");

			size_t found;
			found=m_strChild.find(str_Pchild);

			if (found!=string::npos)  
				m_strChild.replace(found,str_Pchild.length(),strActualStringtoReplaceWith);

		}

		if (RelType == REL_MANYOWNERSHIP)
		{

			NSSTRING str_Pmanymwnership = _T("<PMANYOWNERSHIP_CHILD>");

			size_t found;
			found=m_strManyOwnership.find(str_Pmanymwnership);

			if (found!=string::npos)  
				m_strManyOwnership.replace(found,str_Pmanymwnership.length(),strActualStringtoReplaceWith);
		}
}


//CNSNestedBlockInfo::CNSNestedBlockInfo(void)
//{
//}

//CNSNestedBlockInfo::~CNSNestedBlockInfo(void)
//{
//}

void CNSNestedBlockInfo::createAnewSubFeature(NSSTRING strFID, CNSFeatureContainer *pSubFeatContainer)
{
	//if(m_SubFeaturesMap.find(strFID) == m_SubFeaturesMap.end())
		m_SubFeaturesMap.insert(std::map<NSSTRING, CNSFeatureContainer *>::value_type(strFID, pSubFeatContainer));
		//m_SubFeaturesMap.insert(std::map<long, long>::value_type(cnt, cnt));
	
}

std::map<NSSTRING, CNSFeatureContainer *> CNSNestedBlockInfo::getSubFeatureMap()
{
		return m_SubFeaturesMap;	
}

NSSTRING CNSFeatureContainer::getIndexAtPNode2()
{

	NSSTRING retval = _T("");

	if(m_pRelationInfo != NULL )
	{
		retval = m_pRelationInfo->getIndexAtPNode2();	
	}

	return retval;

	
}

NSSTRING CNSRelationInfo::getIndexAtPNode2()
{
	NSSTRING str_Pnode2 = _T("<PNODE2_"); //<PNODE2_1>...<PNODE2_2>....<PNODE2_N>
	getupperCaseString((TCHAR*)m_strConnectivtyNode2.c_str());
	size_t start;
	start = m_strConnectivtyNode2.find(str_Pnode2);

	if (start!=string::npos)
	{
		size_t end;
		NSSTRING str_end = _T(">");
		end = m_strConnectivtyNode2.find(str_end);
		
		if (end!=string::npos)
		{
			NSSTRING strN = m_strConnectivtyNode2.substr(start+8,end-(start+8));
			return strN;
		}	
		else
			return _T("");
	}
	else
		return _T("");
}

//NSSTRING CNSRelationInfo::getFIDFNOpairAtIndex(std::map<NSSTRING,NSSTRING> mapOfFIDFNOs, int Index)
//{
//	NSSTRING retval;
//
//	std::map<NSSTRING,NSSTRING>::iterator iter;
//	long i=0;
//	for(iter = mapOfFIDFNOs.begin(); iter != mapOfFIDFNOs.end(); iter++)
//	{
//		i++;
//		if(i == Index)
//		{
//			retval = ((*iter).first).c_str() ;
//			retval = retval + _T(",") + ((*iter).second).c_str() ;
//		}
//	}
//
//
//	return  retval;
//}

void CNSRelationInfo::getFIDFNOs(NSSTRING strXDATA, std::map<NSSTRING,NSSTRING> &mapOfFIDFNOs)
{
	TCHAR szCounter[5] = _T("");
	int iCounter = 0;

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

												iCounter++;

												NSSPRINTF(szCounter, _T("%d"), iCounter);
												NSSTRING strIndex(szCounter);

												NSSTRING strFIDFNOpair = strFID + _T(",") + strFNO;

												mapOfFIDFNOs.insert(std::map<NSSTRING, NSSTRING>::value_type(strIndex,strFIDFNOpair));

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




